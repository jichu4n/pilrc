----------------------------------------------------------------------------
 PilRC version 2.5b4                                           June 6, 2000
----------------------------------------------------------------------------

  INSTALLATION:
  =============

  PilRC is an application  that takes a resource script file and generates 
  one or more binary resource files  that are to be  used when  developing 
  for  the  Palm Computing Platform.  PilRCUI gives you a  preview of your
  resource file. 

  You should obtain the latest version of PilRC before compiling it. It is
  available from the following location:

    http://www.ardiri.com/index.cfm?redir=palm&cat=pilrc

  If you wish to link to the PilRC development website, please provide the
  as it is shown.

  BUILDING
  ========

  The  source code and  binaries are  available  free of  charge  and  are 
  Copyrighted 1997-2000 by Wes Cherry and Aaron Ardiri.  Any modifications 
  (bug-fixes and/or improvements) to  PilRC should be sent to Aaron Ardiri
  so that they can be integrated into the next release(s). 

  > generic "compile"

  A generic  Makefile has  been included that compiles using a very simple
  structure  that can be edited  to suit your  own needs.  It has not been 
  configured against your system and may require some modification.  PilRC
  can be compiled by typing the following at your command prompt:

     make -f Makefile.generic

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
