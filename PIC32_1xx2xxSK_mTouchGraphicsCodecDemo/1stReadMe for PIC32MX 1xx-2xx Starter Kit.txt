PIC32MX1xx/2xx Starter Kit Demo: mTouchGraphicsCodecDemo
============================================================================

What This Demo Does
-------------------
This demonstration provides a simple music player which can play up to 16
different dot wave files (.wav) loaded onto a 4 GB micro SD card.  The boards's
TFT display, three switches, and four button mTouch slider pad are used to
display application status and allow the user to select and play one of the
SD card's music files.

The demonstration software is preloaded on the board.  Each starter kit board
comes with a 4 GB micro SD card that is preloaded with a music selection, which
is selectable using the boards 3 switches and 4 button mTouch.  Music volume is
controlled by using the 4 button mTouch panel as a "slider".


Required Development Resources:
-------------------------------
a. PIC32MX1xx/2xx Starter Kit,
   including:
   * Starter Kit Board
   * 4 GB micro SD memory card with .wav files
   * USB cable

b. MPLAB IDE V8.63 or MPLAB X Beta 7

c. C32 Compiler V2.01 (optional for rebuilding the application)

d. Headphones or earphones


Optional Development Resources:
-------------------------------
a. PICKit Serial Analyzer (PKSA) (DV164122)
   (For use in evaluating mTouch performance)


Running the Demo:
-----------------
It is necessary to first power-up the starter kit board from your PC with MPLAB
running.  Start MPLAB.  Connect the USB cable from the starter kit.  The driver
for the  on-board debugger should load, providing power to the board.  After
this it should be possible to run the application without having MPLAB running
by simply plugging in the USB cable.

On startup the application will play a preselected piece of music.  Then you
can use the three switches and 4-button mTouch panel to navigate to other music
selection slots in the application's menu that you can fill with additional
music. (Details on how to do this are found below.)


Board Layout and Features:
--------------------------

        +===========================================+
        |+-TFT DISPLAY-(220x176)-------------------+|
        ||                                 |   ' " ||
        ||                                 |   ' " ||
        || PIC32MC2XX        BUTTON 4 LABEL|(O)' m ||  On Bottom:
        ||  Starter Kit                    |   ' T ||] <- USB jack for
        ||   mTouch                        |   ' O ||] <- power and
        ||    + CODEC                      |   ' U ||] <- debugging
        ||     + Graphics    BUTTON 3 LABEL|(O)' C ||
Music   ||        Demo                     |   ' H ||
Time    ||                                 |   ' " ||
Display--> mmm/NNN s                       |   ' S ||
        ||                   BUTTON 2 LABEL|(O)' L ||
        ||(0) SWITCH 3 LABEL               |   ' I ||
        ||                                 |   ' D ||
        ||(0) SWITCH 2 LABEL               |   ' E ||
        ||                   BUTTON 1 LABEL|(O)' R ||
        ||(0) SWITCH 1 LABEL               |   ' " ||
        ||                                 |   ' " ||
        |+-----------------------------------------+|
        |                                           |
        | On Bottom:                        |=====| |
       [| <-                                |     | |
       [| <- Micro      LED 2  LED 1        |     | |
       [| <- SD Card    [==]   [==]         |m(4)m| |
       [| <- socket                         |T   T| |
       [| <- for              +---------+   |O   O| |
       [| <- Music            |+-------+|   |U   U| |
       [| <- .wav's           || PIC32 ||   |C   C| |
        |                     || MX220 ||   |H   H| |
        |                     || F032D ||   |     | |
        |                     || ===== ||   |B(3)B| |
        |                     |+-------+|   |U   U| |
        |                     +---------+   |T   T| |
        |                                   |T   T| |
        |                                   |O   O| |
        |                                   |N   N| |
        |                                   |     | |
        |                                   |&(2)&| |
        |    +-------+                      |     | |
        |    | CODEC |                      |S   S| |
        |    +-------+                      |L   L| |
        |                                   |I   I| |
        |                                   |D   D| |
        |                  SWITCH 3 (0)     |E   E| |
        |                                   |R   R| |
        |+------------+    SWITCH 2 (0)     | (1) | |
        || HEADDPHONE |                     |     | |
        |+-+  JACK    |    SWITCH 1 (0)     |     | |
        |  +----------+                     |=====| |
        |                                           |
        |                                           |
        |                                           |
        +===========================================+


The TFT display provides switch and button labels when the application is
waiting for a user input or command.  The right-most side of the display
shows the mTouch-slider-controlled volume selected by the user when music
is playing.

NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE

    The TFT display does not support user inputs.  User inputs are provided
    using the three switches and four mTouch buttons on the PC board.

    The switch and button functions can easily be transferred to an custom-
    designed ITO display panel to support user inputs directly to the display,
    but that is beyond the scope of this starter kit project.

NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE

In user input/command mode the application indicates on the display which
switch or button is being asserted on the board.  All mTouch buttons implement
an  "assert on release" for the music selected.  The asserted button is marked
by an inner circle on the display's button icon that stays on until the next
button is asserted by the user.  The three switches are debounced by the
application and the corresponding switch icon is displayed for a short time
after the switch is closed by the user's press.

The board's LED 2 will light whenever music is being played by the application.


About the Music:
----------------
Dot wave files are stored on the memory card using eight dot three file names
from File0.wav through FileF.wav.  A single music selection is preloaded onto
the memory card after conversion to the .WAV format.

File #
|  MP3/4 File Name                    Size      Artist         Album
0  Artifact.mp3                       8,380 KB  Kevin MacLeod  African

Kevin MacLeod's music was been downloaded from

                http://incompetech.com/m/c/royalty-free/


Running the demonstration without using MPLab:
----------------------------------------------
You can use a bench supply to provide 3.3 V using test points TP2 (3.3V) and
TP4 (ground).  You can also simply use a 5 V "wall wart" power supply with a
USB connector.  In both cases you are bypassing the startup control of the
application provided by the debugger controller (on the board's reverse side).

Not using the debugger controller can cause the application to freeze or not
play music correctly.  In this case press Switch 1 one or more times until the
application restarts.  This should fix the problem.


Troubleshooting:
----------------
If the application produces error messages on the screen or if it doesn't play
music correctly, try restarting the application by pressing Switch 1 once or
twice.  You may also need to remove and reseat the memory card, paying
particular attention to inserting the memory card back into the socket with the
proper alignment.

You might also want to do a chkdsk (check disk) on the memory card, provided
you have an adaptor that allows your PC to mount the memory card as an external
disk drive (such as E: or F:).  Note that the memory card has been reformatted
as a FAT16 device.

Please note that any new memory card you care to use must be at least a
"Class 6" or faster card to support the data rates needed for music playback.
You will also need to reformat the card as a FAT16 partition using the
following DOS commands:

    diskpart /s RepartitionEScript.txt
    format E: /Q /FS:FAT /V:MusicWAVs
        
Where the file RepartitionEScript.txt contains:

    REM This diskpart script repartitions a 8 GByte micro SD card to 2 GBytes,
    REM allowing the use of a FAT16 partition instead of FAT32.
    LIST volume
    SELECT volume=E
    DETAIL volume
    DELETE partition 
    CREATE partition primary size=2000
    EXIT

You can also replace the music provided with the starter kit, just as long as
you name the files File0.wav through FileF.wav, and you update the application's
main routine with new music selection names and/or categories.  The header file

     .\Source Files\mTouchGraphicsCodecDemo\MusicDefines.h defines
     
allows you to define the music categories, music track names, and play time.


Compiling and Downloading the Demo:
-----------------------------------
1. Extract the contents of the .ZIP file to someplace convenient on your hard
   drive.  The folder's MPLAB project is self-contained, and can be moved to
   any location after extraction, as long as the directory names inside the
   project are not changed or the contents moved.


   The "Starter Kit on Board" tab of the output window should display something
   like this:

        Starter Kit on Board detected
        Connecting to Starter Kit on Board...
        Running self test...
        Self test completed

        PIC32MX1xx/2xx STARTER KIT by Microchip Technology Incorporated

        Firmware Suite Version...... 01.26.71
        Firmware type......................PIC32MX
        Starter Kit on Board Connected.
        Target Detected
        Device ID Revision = 00000000

6. Build the project by selecting Project>Build All.

7. Download your code into the evaluation board microcontroller by
   selecting Debugger>Programming>Program All Memories.

8. Run the demo code by selecting Debugger>Run.

   You should see the same application behavior as before when the board ran
   the preprogrammed demonstration software.


NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE

     HERE'S WHERE WE WARN YOU ABOUT PROJECT FEATURES YOU SHOULD NOT CHANGE.

1) ==>The procdefs.ld linker script file in the main project directory.<==
Leave this alone.  It has been modified to reduce the interrupt table size and
allow the application to fit within memory.  Delete, move, or rename this file
and the project will no longer build.

2) ==>MIPS16 code generation.<==
Another trick we've used is to instruct the compiler to generate MIPS16 code.
This increases code density.  But code executed from within an ISR must be in
MIPS32.  This happens automatically for all ISRs.  But the timer callback
function must be compiled with MIPS32 code as well.  We implement this by
using a custom build option. (Do a right mouse click on the file to bring up
this setting window.)

NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE


References:
-----------
  PICKit Serial Analyzer User's Guide - DS51647C
  AN1250, Microchip CTMU for Capacitive Touch Applications - DS01250A
  ProfiLab-Expert Software:
                       http://www.abacom-online.de/uk/html/profilab-expert.html


