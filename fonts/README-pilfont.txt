PilFont - A crude font decompiler for PalmOS
Copyright (C) 1998 David Turnbull

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA

Send bug reports to: dpt@newsguy.com


PURPOSE

This program is designed to convert a PalmOS font resource (NFNT) into an
ASCII format that can be studied, modified, or compiled with PilRC.  It is also
a reference for the structure of a font resource.

The program makes very little effort at detecting an error condition, but if
you give it valid data it will provide accurate results.


COMPILING

The code should be fully portable on any ANSI/ISO compiler.


BASICS

usage:  pilfont infile outfile {offset}
 
infile  - the file containing the binary font data
outfile - file to receive the ASCII font data
offset  - optional paramater to specify beginning of font in infile


ADVANCED

Here are some offsets you can use to examine fonts that are commonly available.

0  - .bin font resources created by PilRC
88 - A PDB file containing only a single font

Some ROM files I had on hand.  The built in fonts all had the same fontType of
0x9000 so you should be able to easily search your own ROM files as well.

Font             Pilot V1  Pro       Debug V2
---------------  --------  --------  --------
0, stdFont       287434    72062     268666
1, boldFont      206140    69142     74324
2, largeFont     290046    340568    271300
3, symbolFont    141212    75364     77912
4, symbol11Font  141720    74816     77364
5, symbol7Font   141600    74696     77244
6, ledFont       141868    74964     77512
