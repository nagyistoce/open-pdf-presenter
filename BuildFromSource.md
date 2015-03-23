This page explains how to build open-pdf-presenter from source.

# Dependencies #

To build open-pdf-presenter, you need some packages installed on your system. This section describes which packages are those and how to install them for Ubuntu and OSX. You can get an idea of which packages you need for other GNU/Linux distributions with the Ubuntu section.

I hope I have not missed any dependency. If you are not able to build open-pdf-presenter due to missing dependencies, please let me know.

## Ubuntu ##

To build open-pdf-presenter, you need the following dependencies installed on your system:
```
build-essential
qt4-qmake
libqt4-dev
qt4-dev-tools
libpoppler-qt4-dev
```

You can ensure that these packages are installed with the following command line:
```
$ sudo apt-get install build-essential qt4-qmake libqt4-dev libpoppler-qt4-dev
```

Finally, you will need GIT to get open-pdf-presenter's source code. Ensure that you have it installed on your system:
```
$ sudo apt-get install git
```

## OSX ##

In OSX you can use MacPorts to install the dependencies and then build open-pdf-presenter. So, you must first install MacPorts. Check this page for instructions: http://www.macports.org/install.php

Besides MacPorts, you also need the command line tools that come with Xcode. So, start with installing Xcode: https://developer.apple.com/xcode/

Then, use Xcode to install the command line tools, as the following screenshots show.

First, access Xcode preferences:
![http://open-pdf-presenter.googlecode.com/files/Xcode-preferences-all.png](http://open-pdf-presenter.googlecode.com/files/Xcode-preferences-all.png)

Then, install the command line tools in the downloads tab:
![http://open-pdf-presenter.googlecode.com/files/Xcode-downloads.png](http://open-pdf-presenter.googlecode.com/files/Xcode-downloads.png)

At this point, all that is left is to use MacPorts to install the dependencies. Use the following command lines:
```
$ sudo port install qt4-mac-devel +debug
$ sudo port install poppler +qt4 +quartz
```

Finally, you will need GIT to get open-pdf-presenter's source code. You can get a GIT installer for OSX here: http://code.google.com/p/git-osx-installer/

**DISCLAIMER/ACKNOWLEDGEMENTS**: I do not own a Mac and I never had, so my knowledge about how stuff works in Mac is very limited. This section was possible with the effort of Marta Teixeira (thanks Marta), who is an open-pdf-presenter user that recently bought her first Mac. So, if you are a senior Mac user and you find any errors in this guide, please let me know! This guide was tested in OSX version 10.7.3 (Lion).

# Getting the source #

## From the git repository ##


Clone the repository:
```
$ git clone https://code.google.com/p/open-pdf-presenter/
$ cd open-pdf-presenter
```

The main branch, selected by default, always contains the latest release. All the really unstable and bleeding-edge stuff is hidden on other branches.

## From a downloadable tarball file ##

Browse [the downloads section of open-pdf-presenter](http://code.google.com/p/open-pdf-presenter/downloads/list) and download a source code tarball file.
Then, change directory to where you downloaded that file and unpack it.
Let's say that you downloaded the tarball file named **open-pdf-presenter-v0.1.tar.gz**
```
$ tar xvf open-pdf-presenter-v0.1.tar.gz
$ cd open-pdf-presenter
```

# Building open-pdf-presenter #

To build open-pdf-presenter, start by generating the makefile:
```
$ qmake
```

Then, compile open-pdf-presenter using the makefile:
```
$ make
```

At this point, there is an executable file called "open-pdf-presenter" on the project's root directory. You can run this file directly (see the next section). You can also install open-pdf-presenter using

```
$ sudo make install
```

This installs a desktop file that creates a new entry on the context menu that enables you to use open-pdf-presenter directly from your desktop environment.

# Running open-pdf-presenter #

The build process that this page describes generates a binary file named **open-pdf-presenter** on the root directory of the project.

Running the executable file with no arguments opens up a start screen that enables you to configure the presentation (duration, PDF file, notes file, etc).

Check out the [user guide](UserGuide.md) to take full advantage of open-pdf-presenter's features!