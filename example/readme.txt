pilrc example readme.txt
Wes Cherry
wesc@ricochet.net


Files
-----
[Common]
pilrctst.rcp		: sample script
pilrcic.bmp		: icon file, Windows .BMP format
pilrc.bmp		: FORMBITMAP bitmap, WIndows .BMP format

[GNU CC]
pilrctst.c		: C test harness 
pilrctst.h		: include file for C test harness
Makefile		: makefile for C test harness
runme.bat		: sets up cygnus-win32 environment (may require tweaking)
runme.cmd		: OS/2 cmd file.  sets up cygnus-win32 environment (may require tweaking)

[PILA]
pilrctst.asm		: Asm test harness
pilrctst.inc		: include file for asm test harness
mkasm.bat		: batch file to build pilrctst.prc  (may require tweaking)
mkasm.cmd		: OS/2 cmd file to build pilrctst.prc


[GNU CC]
This is a sample ,c and .rcp file demonstrating pilrc.  Building
this program requires PilotGcc.  

Pilot-Gcc for Win32 version 0.4.0 is now available at:
http://www.ftpx.com/pilotgcc/gccwin32.html

Pilot-Gcc for unix platforms is at:
ftp://ftp.pfnet.com/pub/PalmOS/prc-tools.0.4.0.tar.gz
ftp://ftp.pfnet.com/pub/PalmOS/prc-example.0.4.0.tar.gz

This example will not compile using pilrc 1.1 which is a part of the
unix release.  You'll either have to recompile the sources for unix or
wait until the unix folks incorporate pilrc 1.3 into their release.

To build pilrctst.prc under Win32 Pilot-Gcc, first run Runme.bat, then
type make.  

[PILA]
This is a sample .asm and .rcp file demonstrating pilrc.  Building this
program requires Pila

Pila is available at:

http://www.massena.com/darrin/pilot/index.html

To build pilrctst.prc under Pila, run mkasm.bat





