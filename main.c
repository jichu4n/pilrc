/*-----------------------------------------------------------------------------
| main.c -- a resource compiler for the US Robotics Pilot
|
|	Wes Cherry - wesc@ricochet.net
|	29 Oct 1996
|
| This file contains the driver for the command line version of pilrc.
|
| See pilrc.htm for documentation
-------------------------------------------------------------WESC------------*/
#include <stdio.h>
#include <stdlib.h>
#include "pilrc.h"

void Usage()
	{
    Error("usage: pilrc {<options>} infile [outfiledir]\n\n"
	  "Options:\n"
	  "        -L LANGUAGE  Compile resources for specific language\n"
	  "        -I <path>    Search for bitmap and include files in <path>\n"
	  "                     More than one -I <path> options may be given\n"
	  "                     The current directory is always searched\n"
	  "        -R <resfile> Generate JUMP/PilA .res file\n"
	  "        -H <incfile> Autoassign IDs and write .h file with #defines\n"
	  "        -F5          Use Big5 Chinese font widths\n"
	  "        -Fg          Use GB Chinese font widths\n"
	  "        -Fj          Use Japense font widths\n"
	  "        -Fh          Use Hebrew font widths\n"
	  "        -rtl         Right to left support\n"
	  "        -q           Less noisy output\n"
	  );

    exit( 1 );
	}

/*-----------------------------------------------------------------------------
|	main
-------------------------------------------------------------WESC------------*/
int
main(int cArg, char *rgszArg[])
	{
	char *szOutputPath;
	char *szInputFile;
	char *szResFile;
	char *szIncFile;
	int i;
	int fontType;
	
	printf("PilRC v2.5\n");
	printf("  Copyright 1997-1999 Wes Cherry   (wesc@ricochet.net)\n");
	printf("  Copyright 2000      Aaron Ardiri (ardiri@palmgear.com)\n");

	vfCheckDupes = 1;	
	if (cArg < 2)
		{
	    Usage();
		}

	szResFile = NULL;
	szIncFile = NULL;
	fontType = fontDefault;
	for (i = 1; i < cArg; i++)
		{
	    if (rgszArg[i][0] != '-')
			{
			break;
			}

	
	    if (FSzEqI(rgszArg[i], "-L"))
			{
			if (i++ == cArg)
				{
				Usage();
				}

			szLanguage = rgszArg[i];
			continue;
			}

	    if (FSzEqI(rgszArg[i], "-I"))
			{
			if (i++ == cArg)
				{
				Usage();
				}

			if (totalIncludePaths == MAXPATHS)
				{
				printf("Too many include paths!\n\n");
				Usage();
				}

			includePaths[ totalIncludePaths++ ] = rgszArg[i];
			continue;
			}

	    if (FSzEqI(rgszArg[i], "-R"))
			{
			if (i++ == cArg)
				{
				Usage();
				}
			szResFile = rgszArg[i];
			continue;
			}
			

	    if (FSzEqI(rgszArg[i], "-H"))
			{
			if (i++ == cArg)
				{
				Usage();
				}
			szIncFile = rgszArg[i];
			vfAutoId = fTrue;
			continue;
			}
			
	    if (FSzEqI(rgszArg[i], "-q"))
			{
			vfQuiet = fTrue;
			continue;
			}
			
	    if (FSzEqI(rgszArg[i], "-Fh"))
			{
			fontType = fontHebrew;
			continue;
			}
			
	    if (FSzEqI(rgszArg[i], "-Fj"))
			{
			fontType = fontJapanese;
			continue;
			}
			
	    if (FSzEqI(rgszArg[i], "-F5"))
			{
			fontType = fontChineseBig5;
			continue;
			}
			
	    if (FSzEqI(rgszArg[i], "-Fg"))
			{
			fontType = fontChineseGB;
			continue;
			}
			
	    if (FSzEqI(rgszArg[i], "-rtl"))
			{
			vfRTL = fTrue;
			continue;
			}
			
			
		Usage();
		}

	if (cArg - i < 1)
		{
	    Usage();
		}

	szInputFile  = rgszArg[i++];

	if (cArg != i)
		{
	    szOutputPath = rgszArg[i++];
		}
	else
		{
	    szOutputPath = ".";
		}

	if (cArg != i)
		{
	    Usage();
		}

	printf("\n");

	ParseFile(szInputFile, szOutputPath, szResFile, szIncFile, fontType);

	return 0;
	}
