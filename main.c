
/*
 * @(#)main.c
 *
 * Copyright 1997-1999, Wes Cherry   (mailto:wesc@technosis.com)
 *           2000-2001, Aaron Ardiri (mailto:aaron@ardiri.com)
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
 *     20-Nov-2000 Renaud malaval (rmalaval@palm.com)
 *                 Add full support of PalmOS 3.5 (bitfields, structures, ...)
 *                 Add resource LAUNCHERCATEGORY (taic)
 *                 Add support for Little Endian 32 bits: LE32 (ARM and NT)
 *     Jan-2001    Regis Nicolas
 *                 Merged 68K and LE32 version into one binary
 *     12-Jan-2001 Renaud Malaval
 *                 Added 'wrdl' resource support
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilrc.h"
#include "restype.h"

/**
 * Display the usage information for PilRC.
 */
void
Usage(void)
{
  printf
    ("\nThis program is free software; you may redistribute it under the\n"
     "terms of the GNU General Public License. This program has absolutely\n"
     "no warranty, you use it AS IS at your own risk.\n\n"
     "usage: pilrc {<options>} infile [outfiledir]\n\n" "Options:\n"
     "        -L LANGUAGE  Compile resources for specific language\n"
     "        -I <path>    Search for bitmap and include files in <path>\n"
     "                     More than one -I <path> options may be given\n"
     "                     The current directory is always searched\n"
     "        -R <resfile> Generate JUMP/PilA .res file\n"
     "        -ro          Generate resource database file instead of .bins\n"
     "        -o <filedir> Equivalent to [outfiledir]\n"
     "        -H <incfile> Autoassign IDs and write .h file with #defines\n"
     "        -D <macro>   Define a pre-processor macro symbol\n"
     "        -F5          Use Big5 Chinese font widths\n"
     "        -Fkt         Use Korean font widths (hantip font)\n"
     "        -Fkm         Use Korean font widths (hanme font)\n"
     "        -Fg          Use GB Chinese font widths\n"
     "        -Fj          Use Japense font widths\n"
     "        -Fh          Use Hebrew font widths\n"
     "        -rtl         Right to left support\n"
     "        -q           Less noisy output\n"
     "        -V           Generate M$ (VS-type) error/warning output\n"
     "        -allowEditID Allow edit menu IDs (10000-10007)\n"
     "        -PalmRez     Generate res with PalmRez option\n"
     "        -LE32        Generate 32 bit little endian (ARM, NT) resources\n");

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
main(int cArg,
     char *rgszArg[])
{
  char *szOutputPath;
  char *szInputFile;
  char *szResFile;
  char *szMacro;
  char *szValue;
  char *szIncFile;
  int i;
  int fontType;
  int macroValue;

  // display the (c) string
  printf("PilRC v2.8 patch release 6\n");
  printf("  Copyright 1997-1999 Wes Cherry   (wesc@ricochet.net)\n");
  printf("  Copyright 2000-2001 Aaron Ardiri (aaron@ardiri.com)\n");

  // initialize
  if (cArg < 2)
    Usage();
  vfCheckDupes = 1;
  szResFile = NULL;
  szIncFile = NULL;
  fontType = fontDefault;

  szOutputPath = ".";

  vfPrcName = NULL;
  vfPrcCreator = NULL;
  vfPrcType = NULL;

  // process as many command line arguments as possible
  for (i = 1; i < cArg; i++)
  {
    // no more options to process
    if (rgszArg[i][0] != '-')
      break;

    // language
    if (FSzEqI(rgszArg[i], "-L"))
    {
      if (i++ == cArg)
        Usage();

      szLanguage = rgszArg[i];
      continue;
    }

    // include directory(s)
    if (FSzEqI(rgszArg[i], "-I"))
    {
      if (i++ == cArg)
        Usage();

      if (totalIncludePaths == MAXPATHS)
      {
        printf("Too many include paths!\n\n");
        Usage();
      }
      includePaths[totalIncludePaths++] = rgszArg[i];
      continue;
    }

    // define macro(s) for #ifdef's
    if (FSzEqI(rgszArg[i], "-D"))
    {
      if (i++ == cArg)
        Usage();

      szMacro = strdup(rgszArg[i]);

      // Check if there is a value defined for the macro, otherwise use '1'
      if ((szValue = strchr(szMacro, '=')) != NULL)
      {
        *szValue++ = '\0';
        macroValue = atoi(szValue);
      }
      else
      {
        macroValue = 1;
      }

      AddSym(szMacro, macroValue);
      free(szMacro);
      continue;
    }

    // resource file for PILA?
    if (FSzEqI(rgszArg[i], "-R"))
    {
      if (i++ == cArg)
        Usage();

      szResFile = rgszArg[i];
      continue;
    }

    // header file generation (autoID for ALL)
    if (FSzEqI(rgszArg[i], "-H"))
    {
      if (i++ == cArg)
        Usage();

      szIncFile = rgszArg[i];
      vfAutoId = fTrue;
      continue;
    }

    // be quiet?
    if (FSzEqI(rgszArg[i], "-q"))
    {
      vfQuiet = fTrue;
      continue;
    }

    // allow "edit" ID's?
    if (FSzEqI(rgszArg[i], "-allowEditID"))
    {
      vfAllowEditIDs = fTrue;
      continue;
    }

    // No default flag for form and object form
    if (FSzEqI(rgszArg[i], "-PalmRez"))
    {
      vfPalmRez = fTrue;
      continue;
    }

    // M$ (VS-type) error/warning ouput (regis_nicolas@palm.com)
    if (FSzEqI(rgszArg[i], "-V"))
    {
      vfVSErrors = fTrue;
      continue;
    }

    // font hebrew
    if (FSzEqI(rgszArg[i], "-Fh"))
    {
      fontType = fontHebrew;
      continue;
    }

    // font japanese
    if (FSzEqI(rgszArg[i], "-Fj"))
    {
      fontType = fontJapanese;
      continue;
    }

    // font chinese big 5
    if (FSzEqI(rgszArg[i], "-F5"))
    {
      fontType = fontChineseBig5;
      continue;
    }

    // font chinese GB
    if (FSzEqI(rgszArg[i], "-Fg"))
    {
      fontType = fontChineseGB;
      continue;
    }

    // font korean (jmjeong@oopsla.snu.ac.kr)
    if (FSzEqI(rgszArg[i], "-Fkm"))
    {
      fontType = fontKoreanHanme;
      continue;
    }

    // font korean (jmjeong@oopsla.snu.ac.kr)
    if (FSzEqI(rgszArg[i], "-Fkt"))
    {
      fontType = fontKoreanHantip;
      continue;
    }

    // right to left?
    if (FSzEqI(rgszArg[i], "-rtl"))
    {
      vfRTL = fTrue;
      continue;
    }

    // LE32
    if (FSzEqI(rgszArg[i], "-LE32"))
    {
      vfLE32 = fTrue;
      continue;
    }

    // Output a 'ro' File
    if (FSzEqI(rgszArg[i], "-ro"))
    {
      vfPrc = fTrue;
      continue;
    }

    // Output filename
    if (FSzEqI(rgszArg[i], "-o"))
    {
      if (i++ == cArg)
       Usage();

      szOutputPath = rgszArg[i];
      continue;
    }

    // name definition for prc output?
    if (FSzEqI(rgszArg[i], "-name"))
    {
      if (i++ == cArg)
        Usage();

      vfPrcName = rgszArg[i];
      continue;
    }

    // creator definition for prc output?
    if (FSzEqI(rgszArg[i], "-creator"))
    {
      if (i++ == cArg)
        Usage();

      vfPrcCreator = rgszArg[i];
      continue;
    }

    // type definition for prc output?
    if (FSzEqI(rgszArg[i], "-type"))
    {
      if (i++ == cArg)
        Usage();

      vfPrcType = 0;

      vfPrcType = rgszArg[i];
      continue;
    }

    // unknown argument?
    Usage();
  }
  if ((cArg - i) < 1)
    Usage();
  printf("\n");

  // determine the input path
  szInputFile = rgszArg[i++];

  // determine the ouput path
  if (cArg != i)
    szOutputPath = rgszArg[i++];

  // last minute check? (extra stuff?)
  if (cArg != i)
    Usage();

  if (vfLE32)
    printf("Generating LE32 resources (ARM/NT) from '%s'.\n", szInputFile);
  else
    printf("Generating 68K resources from '%s'.\n", szInputFile);

  ResTypeInit();
  CbInit();
  ParseFile(szInputFile, szOutputPath, szResFile, szIncFile, fontType);

  return 0;
}
