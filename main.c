/*
 * @(#)main.c
 *
 * Copyright 1997-1999, Wes Cherry   (mailto:wesc@technosis.com)
 *                2000, Aaron Ardiri (mailto:aaron@ardiri.com)
 * All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;  either version 2, or (at your option)
 * any version.
 *
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT  ANY  WARRANTY;   without  even   the  implied  warranty  of 
 * MERCHANTABILITY  or FITNESS FOR A  PARTICULAR  PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You  should have  received a  copy of the GNU General Public License
 * along with this program;  if not,  please write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Revisions:
 * ==========
 *
 * pre 18-Jun-2000 <numerous developers>
 *                 creation
 *     18-Jun-2000 Aaron Ardiri
 *                 GNU GPL documentation additions
 */

#include <stdio.h>
#include <stdlib.h>
#include "pilrc.h"

/**
 * Display the usage information for PilRC.
 */
void 
Usage(void)
{
  Error("This program is free software; you may redistribute it under the\n"
  	"terms of the GNU General Public License. This program as absolutely\n"
	"no warranty, you use it AS IS at your own risk.\n\n"
	"usage: pilrc {<options>} infile [outfiledir]\n\n"
        "Options:\n"
        "        -L LANGUAGE  Compile resources for specific language\n"
        "        -I <path>    Search for bitmap and include files in <path>\n"
        "                     More than one -I <path> options may be given\n"
        "                     The current directory is always searched\n"
        "        -R <resfile> Generate JUMP/PilA .res file\n"
        "        -H <incfile> Autoassign IDs and write .h file with #defines\n"
        "        -F5          Use Big5 Chinese font widths\n"
        "        -Fkt         Use Korean font widths (hantip font)\n"
        "        -Fkm         Use Korean font widths (hanme font)\n"
        "        -Fg          Use GB Chinese font widths\n"
        "        -Fj          Use Japense font widths\n"
        "        -Fh          Use Hebrew font widths\n"
        "        -rtl         Right to left support\n"
        "        -q           Less noisy output\n"
        "        -V           Generate M$ (VS-type) error/warning output\n"
  );

  exit(1);
}

/**
 * The main entry point of PilRC.
 *
 * @param cArg    the number of command line arguments.
 * @param rgszArg a reference to a list of command line argument strings.
 * @return program exit status, zero successful, non-zero otherwise.
 */
int
main(int  cArg, 
     char *rgszArg[])
{
  char *szOutputPath;
  char *szInputFile;
  char *szResFile;
  char *szIncFile;
  int  i;
  int  fontType;
	
  // display the (c) string
  printf("PilRC v2.5b9\n");
  printf("  Copyright 1997-1999 Wes Cherry   (wesc@ricochet.net)\n");
  printf("  Copyright 2000      Aaron Ardiri (ardiri@palmgear.com)\n");

  // initialize
  if (cArg < 2) Usage();
  vfCheckDupes = 1;	
  szResFile    = NULL;
  szIncFile    = NULL;
  fontType     = fontDefault;

  // process as many command line arguments as possible
  for (i=1; i<cArg; i++) {

    // no more options to process
    if (rgszArg[i][0] != '-') break;

    // language
    if (FSzEqI(rgszArg[i], "-L")) {
      if (i++ == cArg) Usage();

      szLanguage = rgszArg[i];
      continue;
    }

    // include directory(s)
    if (FSzEqI(rgszArg[i], "-I")) {
      if (i++ == cArg) Usage();

      if (totalIncludePaths == MAXPATHS) {
        printf("Too many include paths!\n\n");
        Usage();
      }
      includePaths[ totalIncludePaths++ ] = rgszArg[i];
      continue;
    }

    // resource file for PILA?
    if (FSzEqI(rgszArg[i], "-R")) {
      if (i++ == cArg) Usage();

      szResFile = rgszArg[i];
      continue;
    }
			
    // header file generation (autoID for ALL)
    if (FSzEqI(rgszArg[i], "-H")) {
      if (i++ == cArg) Usage();

      szIncFile = rgszArg[i];
      vfAutoId  = fTrue;
      continue;
    }
			
    // be quiet?
    if (FSzEqI(rgszArg[i], "-q")) {
      vfQuiet = fTrue;
      continue;
    }

    // M$ (VS-type) error/warning ouput (regis_nicolas@palm.com)
    if (FSzEqI(rgszArg[i], "-V")) {
      vfVSErrors = fTrue;
      continue;
    }
			
    // font hebrew
    if (FSzEqI(rgszArg[i], "-Fh")) {
      fontType = fontHebrew;
      continue;
    }
			
    // font japanese
    if (FSzEqI(rgszArg[i], "-Fj")) {
      fontType = fontJapanese;
      continue;
    }
			
    // font chinese big 5
    if (FSzEqI(rgszArg[i], "-F5")) {
      fontType = fontChineseBig5;
      continue;
    }
			
    // font chinese GB
    if (FSzEqI(rgszArg[i], "-Fg")) {
      fontType = fontChineseGB;
      continue;
    }

    // font korean (jmjeong@oopsla.snu.ac.kr)
    if (FSzEqI(rgszArg[i], "-Fkm")) {
      fontType = fontKoreanHanme;
      continue;
    }

    // font korean (jmjeong@oopsla.snu.ac.kr)
    if (FSzEqI(rgszArg[i], "-Fkt")) {
      fontType = fontKoreanHantip;
      continue;
    }
			
    // right to left?
    if (FSzEqI(rgszArg[i], "-rtl")) {
      vfRTL = fTrue;
      continue;
    }
			
    // unknown argument?
    Usage();
  }
  if ((cArg-i) < 1) Usage();
  printf("\n");

  // determine the input path
  szInputFile  = rgszArg[i++];

  // determine the ouput path
  if (cArg != i) szOutputPath = rgszArg[i++];
  else szOutputPath = ".";

  // last minute check? (extra stuff?)
  if (cArg != i) Usage();

  ParseFile(szInputFile, szOutputPath, szResFile, szIncFile, fontType);

  return 0;
}
