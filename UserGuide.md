# Command line options #
```
USAGE: 

   ./open-pdf-presenter  [-n <XML file>] [-l] [-e <Transition Effect>] 
                         [-t <Transition duration>] [-d <Duration>] [-s] [-r]
                         [--] [--version] [-h] <PDF file>


Where: 

   -n <XML file>,  --notes <XML file>
     Notes file

   -l,  --list
     List available transitions and exit

   -e <Transition Effect>,  --effect <Transition Effect>
     Transition effect to use during presentation

   -t <Transition duration>,  --transition <Transition duration>
     Duration of the transition effect between slides, in mseconds

   -d <Duration>,  --duration <Duration>
     Presentation duration, in seconds

   -s,  --skip
     Skip start screen

   -r,  --rehearse
     Enable rehearse mode

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <PDF file>
     The PDF file with the presentation slides
```
# Supported Keys #

  * **ESC/Q** Exits program
  * **B/PERIOD** Fades slide to black
  * **W** Fades slide to white
  * **RIGHT ARROW/DOWN ARROW/SPACE/PAGE DOWN** Go to next slide
  * **LEFT ARROW/UP ARROW/BACKSPACE/PAGE UP** Go to previous slide
  * **R** Restarts the presentation, resetting timers
  * **S** Swaps the main screen with the auxiliary screen
  * **TAB** Shows/Hides the slide selection grid on the auxiliary screen
  * **N** Shows/Hides the notes console on the auxiliary screen

# Notes #

The notes are specified using an XML file. The file `resources/notes/notes.xsd` contains the schema.

## Example ##

```
<notes>
  <note number="1">
    Note for slide 1
  </note>
  <note number="2,3,4">
   Note for slide 2,3, and 4
  </note>
  <note range="5-10">
    Note for slides 5 through 10
  </note>
  <note>
    Ignored, no slide specified
  </note>
  <note number="1,3">
    Ignored, second note for same slides
  </note>
  <note range="4-11">
    Note for slide 11, ignored for overlapping range 4-10
  </note>
</notes>
```