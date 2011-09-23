/*
    This file is part of open-pdf-presenter.

    open-pdf-presenter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    open-pdf-presenter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with open-pdf-presenter.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "renderer.h"

#include <QPointF>

#define TEST_DPI 96.0

Slide::Slide(QImage image) {
	this->image = image;
}

QImage Slide::asImage() {
	return this->image;
}

QPixmap Slide::asPixmap() {
	return QPixmap::fromImage(this->image);
}

QRect Slide::computeUsableArea(QRect geometry) {
	double ratio = ((double) this->image.width()) / ((double) this->image.height());
	double screenRatio = ((double) geometry.width()) / ((double) geometry.height());

	if (screenRatio < ratio) {
		// Complex computation!
		// The following assignment looks pointless but reveals important info
		// Do not remove it (and the others that follow)
		geometry.setWidth(geometry.width());
		geometry.setHeight(((double) geometry.width()) / ratio);
	}
	else  if (screenRatio > ratio) {
		geometry.setHeight(geometry.height());
		geometry.setWidth(((double) geometry.height()) * ratio);
	}
	else {
		geometry.setWidth(geometry.width());
		geometry.setHeight(geometry.height());
	}

	return geometry;
}

QPoint Slide::computeScaleFactor(QRect geometry, int multiplier) {
	geometry = this->computeUsableArea(geometry);

	int xScaleFactor, yScaleFactor;
	xScaleFactor = ((double) geometry.width() / ((double) this->image.width())) * multiplier;
	yScaleFactor = ((double) geometry.height() / ((double) this->image.height())) * multiplier;

	return QPoint(xScaleFactor,yScaleFactor);
}

Type SlideRenderedEvent::TYPE = Type();

SlideRenderedEvent::SlideRenderedEvent(int slideNumber, Slide slide) : slide(slide) {
	this->slideNumber = slideNumber;
}

Type * SlideRenderedEvent::getAssociatedType() {
	return &SlideRenderedEvent::TYPE;
}

void SlideRenderedEvent::dispatch(IEventHandler *handler) {
	((SlideRenderedEventHandler*)handler)->onSlideRendered(this);
}

int SlideRenderedEvent::getSlideNumber() {
	return this->slideNumber;
}

Slide SlideRenderedEvent::getSlide() {
	return this->slide;
}

Renderer::Renderer(IEventBus * bus, Poppler::Document * document, QRect geometry) :loadingSlide(QImage(QString(":/presenter/loadingslide.svg"))) {
	this->bus = bus;
	this->document = document;
	this->currentGeometry = geometry;

	this->slides = new QList<Slide>();
	this->testSlides = new QList<Slide>();
	this->loadedSlides = new QList<bool>();
	this->loadedTestSlides = new QList<bool>();

	for (int i = 0 ; i < this->document->numPages() ; i++) {
		this->slides->append(this->loadingSlide);
		this->testSlides->append(this->loadingSlide);
		this->loadedSlides->append(false);
		this->loadedTestSlides->append(false);
	}

	this->mutex = new QMutex();
	this->geometryChanged = new QWaitCondition();

	this->thread = new RendererThread(this);
	this->stopThread = false;
}

void Renderer::start() {
	this->thread->start();
}

Renderer::~Renderer() {
	this->mutex->lock();
	this->stopThread = true;
	this->geometryChanged->wakeAll();
	this->mutex->unlock();

	this->thread->wait();
	delete this->thread;
	delete this->mutex;
	delete this->geometryChanged;

	delete this->slides;
	delete this->testSlides;
	delete this->loadedSlides;
}

void Renderer::setGeometry(QRect geometry) {
	this->mutex->lock();
	this->currentGeometry = geometry;
	this->loadedSlides->clear();
	for (int i = 0 ; i < this->document->numPages() ; i++)
		this->loadedSlides->append(false);

	this->geometryChanged->wakeAll();
	this->mutex->unlock();
}

Slide Renderer::getSlide(int slideNumber) {
	this->mutex->lock();
	Slide ret = this->slides->at(slideNumber);
	this->mutex->unlock();

	return ret;
}

void Renderer::run() {
	this->mutex->lock();
	while(1) {
		bool renderedAny = false;
		for (int i = 0 ; i < this->document->numPages() ; i++) {
			if (this->stopThread) {
				this->mutex->unlock();
				return;
			}
			if (!this->loadedSlides->at(i)) {
				this->mutex->unlock();
				renderedAny = true;
				Slide newSlide = this->renderSlide(i);
				this->mutex->lock();
				this->slides->replace(i,newSlide);
				this->loadedSlides->replace(i,true);
				this->bus->fire(new SlideRenderedEvent(i,newSlide));
			}
		}
		if (!renderedAny)
			this->geometryChanged->wait(this->mutex);
	}
}

Slide Renderer::renderSlide(int slideNumber) {
	this->fillTestSlideSize(slideNumber);

	Slide testSlide =  this->testSlides->at(slideNumber);

	QPoint scaleFactor = testSlide.computeScaleFactor(this->currentGeometry,TEST_DPI);

	Poppler::Page * pdfPage = this->document->page(slideNumber);
	QImage image = pdfPage->renderToImage(scaleFactor.x(),scaleFactor.y());
	delete pdfPage;

	return Slide(image);
}

void Renderer::fillTestSlideSize(int slideNumber) {
	if (!this->loadedTestSlides->at(slideNumber)) {
		Poppler::Page * testPage = this->document->page(slideNumber);
		Slide testSlide = Slide(testPage->renderToImage(TEST_DPI, TEST_DPI));
		this->testSlides->replace(slideNumber,testSlide);
		this->loadedTestSlides->replace(slideNumber,true);
		delete testPage;
	}
}

RendererThread::RendererThread(Renderer *renderer) {
	this->renderer = renderer;
}

void RendererThread::run() {
	this->renderer->run();
}
