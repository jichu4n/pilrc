Pilrc readme.txt
Wes Cherry
wesc@ricochet.net

Attached is the latest build of pilrc and pilrcui -- a resource compiler 
for Pilot applications.

PilRC takes a resource script file and emits one or more binary resource files
which are then included by your project's .rc, .asm or other resource file.

PilrcUI gives you a quick GUI preview of your resource file.  Clicking on
the client area causes it to reload.

See doc\pilrc.htm for information on PilRC syntax
Examples for Pila and Gnu CC are in the example\ subdirectory. There is a 
readme.txt there which explains more.

Let me know if you run into any bugs, have feature suggestions or whatever.
PilRC is freeware.  

Building PilRC
--------------
Source code is available.  You are free to make enhancements, but please
send the changes back to me so I can fold them into the main sources.

PilRC has been ported to a number of platforms.  If you wish to run pilrc on a non-Win32
platform, first download http://www.scumby.com/scumbysoft/pilot/pilrc/pilrcsrc.zip, then 
follow the environment specific directions below.

Win32:
------
pilrc.dsp is a MSDev 5.0 project file
pilrc.mak should compile using nmake /f "pilrc.mak"


Unix/Linux/Solaris/XWindows
---------------------------
See doc\Readme.X for info on how to build a X Window version of PilRCUI using
the GIMP GTK+ widget set.  Thanks to Brandon Long for the XWindows work.  You
might want to check http://www.fiction.net/pilot/tools.html for more info.

OS/2 Warp
---------
See doc\Readme.OS2 for info on how to build PilRC for OS/2 Warp platforms.  Though I haven't
tested this, the OS/2 version should run on DOS machines using the EMX libraries.

DOS
---
See doc\Readme.dos.

VSlick Users
------------
I have included a rcp.vlx file for color coding of rcp files.  
Thanks to Chris Pratt (pratt@platinum.com) for the original work.
I have had problems making the file extensions stick in vslick 3.0.  
This seems to be a bug in vslick.

