                                       Author: Brandon Long (blong@fiction.net)
				      Version: 0.12
				         Date: 11-13-1997

pilrcui/X
============
pilrcui/X is a port of the Win32 pilrcui by Wes Cherry to the X Window
System using the GIMP GTK+ widget set (http://www.gimp.org/gtk/).

This port was done by Brandon Long (blong@ficiton.net).  The port ain't
pretty, but it is functional.  Some of those rough edges are because
I've done little to no X programming before.  Feel free to send me any
bugfixes you may have.

Compiling
------------
To compile, you need to have the GTK+ libraries on your system.  Edit 
the makefile, the lines XINCLUDE and XLIBS to point to the location of
the gtk include and libraries.  Then just type make.


Known bugs in 0.1
--------------------
  - All bugs in pilrcui/win32 are present in pilrcui/X, including lack of
    support for tables, bitmaps and gadgets, and a really funky looking
    graffitistateindicator.  
  - bitmaps aren't displayed because they aren't loaded by pilrc (well,
    they are loaded and saved off immediately)
  - No way to set the standard -L, -I, -R and -H options, yet
  - No support for rounded rectangles, yet
  - the form names aren't pretty (this is a feature of the underlying
    pilrc code that should be "fixable" but may not be easy to do
    cleanly)
  - not using the real pilot fonts, so don't have the symbol font
  - file selection doesn't limit to *.rcp, doesn't look like the
    standard gtk widget has this support

Changes in 0.11
----------------
  - Fixed the offset (origin) for non-full window forms
  - Fixed a missing pen change which caused use of a dotted line to have
    followon widgets to be drawn with a dotted line
  - Actually print the items in a list, and inverse the first entry.
    This isn't exactly like on the pilot itself, since we don't seem to
    have information on how many lines fit in the box, but its good
    enough
  - added option to pilot_text to have it calculate the height of the
    font and return it, needed for handling lists
  - now handle bold frame and noframe buttons

Changes in 0.12
---------------
  - Moved pilrc main() into main.c, now compiles both pilrc and pilrcui
    really easily.
  - Upgraded to 1.8 of pilrc code
