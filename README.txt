----------------------------------------------------------------------------
 PilRC version 2.8 - patch release 6                           May 26, 2001
----------------------------------------------------------------------------

  INSTALLATION:
  =============

  PilRC is an application  that takes a resource script file and generates 
  one or more binary resource files  that are to be  used when  developing 
  for  the  Palm Computing Platform.  PilRCUI gives you a  preview of your
  resource file. 

  Copyright (C) 1997-1999 Wes Cherry
  Copyright (C) 2000-2001 Aaron Ardiri

  This program is free software;  you can redistribute it and/or modify it 
  under the terms of the GNU  General  Public  License as published by the 
  Free Software Foundation;  either version  2 of the License, or (at your 
  option) any later version.

  This  program  is  distributed in  the hope that it will be useful,  but 
  WITHOUT   ANY   WARRANTY;   without   even   the  implied   warranty  of 
  MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR  PURPOSE.  See  the  GNU 
  General Public License for more details.

  You should  have received a copy of the GNU General Public License along 
  with this program;  if not, write to the Free Software Foundation, Inc., 
  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  You should obtain the latest version of PilRC before compiling it. It is
  available from the following location:

    http://www.ardiri.com/index.cfm?redir=palm&cat=pilrc&subcat=download

  If you wish to link to the PilRC development website, please provide the
  link as it is shown. Scott Ludwig has kindly donated the redirection url
  of that links directly to the PilRC pages for convenience:

    http://www.pilrc.com/

  BUILDING
  ========

  The  source code and  binaries are  available  free of  charge  and  are 
  Copyrighted 1997-2000 by Wes Cherry and Aaron Ardiri. We'd encourage you
  to send modifications (bug-fixes and/or improvements) of PilRC to  Aaron 
  Ardiri so that they can be integrated into the next release(s). 

  If you are compiling under a  UN*X development environment, you may wish
  to remove the  ^M (return chars) from the source code using the src2unix
  script:

     ./src2unix.sh
 
  Some UN*X compilers complain about ^M chars, and this is an easy fix :)
 
  > generic "compile"

  A generic  Makefile has  been included that compiles using a very simple
  structure  that can be edited  to suit your  own needs.  It has not been 
  configured against your system and may require some modification.  PilRC
  can be compiled by typing the following at your command prompt:

     make -f Makefile.generic

  PilRCUI can be compiled  by typing the following at your  command prompt
  when using a system with "gtk" support (unix systems mainly):

     make -f Makefile.gui

  NOTE: PilRCUI has not been updated, and may not build correctly - unless
        there is a fix for this, you should ignore it and use another tool
        such as 'pilrcedit' to do resource editing.
 
  > autoconf + automake installed:

  If you  have autoconf  and automake installed,  building  and installing 
  PilRC is as  simple as doing the following commands at your shell prompt.

     ./autogen.sh
     make
     make install

  > NO autoconf + automake installed:

  If you do not have  autoconf and  automake installed (as with the cygnus 
  distribution  for win32),  building and installing PilRC is as simple as 
  doing the following commands at your shell prompt.

     ./configure
     make
     make install

  > solaris

  PilRC often produces "core dumps" under solaris when  processing bitmap 
  resources (reading BMP files). This is due to the byte alignment of the 
  BMP headers and the optimization performed by the solaris gcc compiler.

  Remove the "-O2" option  from the CCFLAGS  variable in the Makefile and
  the execution of PilRC should have no problems :))

  > win32 (Microsoft Dev Studio)

  If you have  Microsoft Dev Studio installed, "pilrc.dsp"  is the project 
  file and you can compile PilRC using the following command:

     nmake /f "pilrc.mak"
  
  Note that these files are no longer supported, and have not changed.

  > OS/2 Warp

  PilRC has been built on OS/2 that has E. Matthes' EMX package and DMAKE 
  by Dennis Vadura installed. Please read the additional information 
  provided in "makefile.os2". Compilation is done as follows:

     dmake -f makefile.os2

  Note that these files are no longer supported, and have not changed.

  Thankyou for downloading and using PilRC! 

  // az
  aaron@ardiri.com

----------------------------------------------------------------------------
