Pilrc 1.5 readme.txt
Wes Cherry
wesc@ricochet.net

Attached is the latest build of pilrc and pilrcui -- a resource compiler 
for Pilot applications.

PilRC takes a resource script file and emits one or more binary resource files
which are then included by your project's .rc, .asm or other resource file.

PilrcUI gives you a quick GUI preview of your resource file.  Clicking on
the client area causes it to reload.

See doc\pilrc.htm for information on PilRC syntax
Examples for Pila and Gnu CC are in the example subdirectory. There is a 
readme.txt there which explains more.

Let me know if you run into any bugs, have feature suggestions or whatever.
PilRC is freeware.  Source code is available.

What's new in 1.5
-----------------
o Now checks for duplicate form, menu, string, alert and form object ids
o Also checks for invalid ids (not in 0..9999)

What's new in 1.4
-----------------
o New -R command line argument: emits .res files -- a list of resources emitted by pilrc.  Include this in your PilA or JUMP program
o New -q command line argument: Quiet mode -- less noisy output
o Sample .c file now builds and loads correctly on Pilots

What's new in 1.3
-----------------
o Support for .java include files (Thanks to David Dawson)
o New BITMAP and ICON commands convert Windows .BMP files to pilot bitmaps (Thanks to David Dawson)
o Sample .rcp file, .c and .asm files which contain examples for each control type
o New -I command line argument to specify include search paths
o Updated user manual (Thanks to David Dawson)
o Bug fixes

