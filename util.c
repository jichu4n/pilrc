/*
 * @(#)util.c
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
 */

#ifdef WINGUI
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "pilrc.h"
#include "util.h"
#include "font.h"

/* Globals */
FILE *vfhOut;
FILE *vfhIn;
FILE *vfhRes;
int ibOut;	/* current output offset (from current .bin we are emitting)	 */
char szInFile[256];
char szOutFile[256];
char szOutFileDir[256];
char szFullName[256];
char rgbZero[16];
BOOL vfErr;
extern int iline;
extern BOOL vfQuiet;
extern BOOL vfVSErrors;

/* Includes */
char *includePaths[MAXPATHS];
int   totalIncludePaths = 0;

/*-----------------------------------------------------------------------------
|	Error
|	
|		Report error, exit
------------------------------------------------------------WESC------------*/
VOID Error(char *sz)
	{
#ifdef WINGUI
	if (vfWinGUI)
		{
		if (!vfErr)
			{
  			vfErr = fTrue;
			MessageBox(NULL, sz, NULL, MB_OK);
			}
		}
	else
#endif
		{
		fprintf(stderr, "\n");
		fprintf(stderr, sz);
		fprintf(stderr, "\n");
		exit (1);
		}
	}

/*-----------------------------------------------------------------------------
|	Error2
-------------------------------------------------------------WESC------------*/
VOID Error2(char *sz1, char *sz2)
	{
	char szT[256];

	strcpy(szT, sz1);
	strcat(szT, sz2);
	Error(szT);
	}

/*-----------------------------------------------------------------------------
|	Error3
-------------------------------------------------------------WESC------------*/
VOID Error3(char *sz1, char *sz2, char *sz3)
	{
	char szT[256];

	strcpy(szT, sz1);
	strcat(szT, sz2);
	strcat(szT, sz3);
	Error(szT);
	}


/*-----------------------------------------------------------------------------
|	ErrorLine2
|	
|		Report error w/ current line #
-------------------------------------------------------------WESC------------*/
void ErrorLine2(char *sz, char *sz2)
	{
	char szErr[256];
	
	if (sz2 == NULL)
		sprintf(szErr, 
                  ((vfVSErrors) ? "%s(%d): error : %s" : 
                                  "%s:%d: error : %s"),
		  szInFile, iline, sz);
	else
		sprintf(szErr, 
                  ((vfVSErrors) ? "%s(%d): error : %s %s" : 
                                  "%s:%d: error : %s %s"),
		  szInFile, iline, sz, sz2);
	Error(szErr);
	}



/*-----------------------------------------------------------------------------
|	ErrorLine
|	
|		Report error w/ current line #
-------------------------------------------------------------WESC------------*/
void ErrorLine(char *sz)
	{
	ErrorLine2(sz, NULL);
	}

/*-----------------------------------------------------------------------------
|	WarningLine
|	
|		Report error w/ current line #
-------------------------------------------------------------WESC------------*/
void WarningLine(char *sz)
	{
	char szErr[256];

	sprintf(szErr,
                ((vfVSErrors) ? "%s:(%d): warning : %s" : 
                                "%s:%d: warning : %s"),
	        szInFile, iline, sz);
	fprintf(stderr, "\n");
	fprintf(stderr, szErr);
	fprintf(stderr, "\n");
	}





/*-----------------------------------------------------------------------------
|	EmitB
|	
|		Emit a byte to the output
-------------------------------------------------------------WESC------------*/
void EmitB(unsigned char b)
	{
	DumpBytes(&b, 1);
	}

/*-----------------------------------------------------------------------------
|	EmitW
|	
|		Emit a word
-------------------------------------------------------------WESC------------*/
void EmitW(unsigned short w)
	{
#ifdef ARM	/* RMa little indian */
	EmitB((unsigned char) (w & 0xff));
	EmitB((unsigned char) ((w & 0xff00) >> 8));
#else			/* big indian */
	EmitB((unsigned char) ((w & 0xff00) >> 8));
	EmitB((unsigned char) (w & 0xff));
#endif
	}

/*-----------------------------------------------------------------------------
|	EmitL
|	
|		emit a long
-------------------------------------------------------------WESC------------*/
void EmitL(unsigned long l)
	{
#ifdef ARM	/* RMa little indian */
	EmitB((unsigned char) (l & 0xff));
	EmitB((unsigned char) ((l & 0xff00) >> 8));
	EmitB((unsigned char) ((l & 0xff0000) >> 16));
	EmitB((unsigned char) ((l & 0xff000000) >> 24));
#else			/* big indian */
	EmitB((unsigned char) ((l & 0xff000000) >> 24));
	EmitB((unsigned char) ((l & 0xff0000) >> 16));
	EmitB((unsigned char) ((l & 0xff00) >> 8));
	EmitB((unsigned char) (l & 0xff));
#endif
	}


/*-----------------------------------------------------------------------------
|	IbOut
|
|		Return current output file file offset
-------------------------------------------------------------WESC------------*/
int IbOut()
	{
	return ibOut;
	}

/*-----------------------------------------------------------------------------
|	DumpBytes
|	
|		Emit bytes to current output file
-------------------------------------------------------------WESC------------*/
void DumpBytes(void *pv, int cb)
	{
#ifdef HEXOUT		/* RMa activate Hex dump in debug */
	BYTE *pb;
	int ib;
	static int nbrBytesOut;
	static int ibLine;
	static BYTE rgbLine[16];

	if (!pv)		/* RMa little hack to display clean Hex output */
		{
		if ((ibLine > 0) && (ibLine < 16))
			{
			for (ib = ibLine; ib < 16; ib++)
				{
				rgbLine[ib] = ' ';
				printf("   ");
				if (ibLine == 8)
					printf(" ");
				}
			printf(" ");
			for (ib = 0; ib < 16; ib++)
				{
				if (isprint(rgbLine[ib]))
					printf("%c", rgbLine[ib]);
				else
					printf(".");
				}
			printf("\n");
			}
		ibLine = 0;
		nbrBytesOut = 0;
		return;
		}
#endif
	if (vfWinGUI)
		return;
/*#ifdef BINOUT */
	fwrite(pv, cb, 1, vfhOut);
	ibOut += cb;
/*#endif */
#ifdef HEXOUT 
	pb = (BYTE *)pv;
	while(cb--)
		{
		if (ibLine == 0)
			printf("%08x  ", nbrBytesOut);
		rgbLine[ibLine++] = *pb;
		printf("%02x ", *pb);
		pb++;
//		ibOut++;					/* RMa bug  correction */
		if (ibLine == 8)
			printf(" ");
		if (ibLine == 16)
			{
			nbrBytesOut += ibLine;
			ibLine = 0;
			printf(" ");
			for (ib = 0; ib < 16; ib++)
				{
				if (isprint(rgbLine[ib]))
					printf("%c", rgbLine[ib]);
				else
					printf(".");
				}
			printf("\n");
			}		
		}
#endif		
	}	
	
/*-----------------------------------------------------------------------------
|	PadWordBoundary
|	
|		Pads output to a word boundary by emitting a 0 if necessary
-------------------------------------------------------------WESC------------*/
void PadWordBoundary()
	{
#ifdef ARM	/* RMa little indian */
/*	4 % 4 = 0
	5 % 4 = 1
	6 % 4 = 2
	7 % 4 = 3
	8 % 4 = 0	*/
		switch (ibOut % 4)
		{
		case 1 :
			DumpBytes(rgbZero, 1);
			/* fall thru */
		case 2 :
			DumpBytes(rgbZero, 1);
			/* fall thru */
		case 3 :
			DumpBytes(rgbZero, 1);			
			/* fall thru */
		case 0 :
			break;
		}
#else
	if (ibOut & 1)
		DumpBytes(rgbZero, 1);
#endif
	}

/*-----------------------------------------------------------------------------
|	SetOutFileDir
|	
|		Set output file path -- no trailing \
-------------------------------------------------------------WESC------------*/
VOID SetOutFileDir(char *sz)
	{
	strcpy(szOutFileDir, sz);
	}	

/*-----------------------------------------------------------------------------
|	OpenOutput
|	
|		Open output file of the form %outfiledir%\TTTTXXXX.bin ,
|	
|	where TTTT is the base 4 character mac/pilot res type and
|	XXXX is the resource id as a 4 digit hex number
-------------------------------------------------------------WESC------------*/
VOID OpenOutput(char *szBase, int id)
	{
	
/*#ifdef BINOUT */
	if (vfWinGUI)
		return;
	Assert(vfhOut == NULL);
#if WIN32
	sprintf (szOutFile, "%s\\%s%04x.bin", szOutFileDir, szBase, id);
#else
	sprintf (szOutFile, "%s/%s%04x.bin", szOutFileDir, szBase, id);
#endif
	vfhOut = fopen(szOutFile, "wb");
	if (vfhOut == NULL)
		Error3("Unable to open:", szOutFile, strerror(errno));
	if (!vfQuiet)
		printf("Writing %s\n", szOutFile);
	ibOut = 0;
/*#endif */

	if (vfhRes != NULL)
		{
		fprintf(vfhRes, "\tres '%s', %d, \"%s\"\n", szBase, id, szOutFile);
		}
	}

/*-----------------------------------------------------------------------------
|	CloseOutput
-------------------------------------------------------------WESC------------*/
VOID CloseOutput()
	{
#ifdef HEXOUT			/* RMA call little hack to display clean Hex output */
	DumpBytes(NULL, 0);
#endif
/*#ifdef BINOUT */
	if (vfWinGUI)
		return;
	if (!vfQuiet)
		printf("%d bytes\n", ibOut);
	if (vfhOut != NULL)
		{
		fclose(vfhOut);
		vfhOut = NULL;
		}
/*#endif */
	}


VOID OpenResFile(char *sz)
	{
	if (vfWinGUI)
		return;

	if (sz == NULL)
		return;
	vfhRes = fopen(sz, "wt");
	if (vfhRes == NULL)
		Error3("Unable to open:", sz, strerror(errno));
	if (!vfQuiet)
		printf("Generating res file: %s\n", sz);
	}
	
VOID CloseResFile()
	{
	if (vfWinGUI)
		return;

	if (vfhRes != NULL)
		{
		fclose(vfhRes);
		vfhRes = NULL;
		}	
	}	



/*-----------------------------------------------------------------------------
|	FindAndOpenFile
-------------------------------------------------------------DAVE------------*/
char *FindAndOpenFile(char *szIn, char *mode, FILE **returnFile )
{
	FILE *file = fopen(szIn, mode);

	if ( file == NULL ) 
	{

		int i;
		for ( i = 0; i < totalIncludePaths; i++ )
		{
#ifdef WIN32
			sprintf( szFullName, "%s\\%s", includePaths[i], szIn);
#else
			sprintf( szFullName, "%s/%s", includePaths[i], szIn);
#endif
		
			file = fopen( szFullName, mode);
			if ( file != NULL )
			{
			   break;
			}
/* RNi: Why is this test here ? It breaks the multiple path searching !!!
			else
				Error2("Could not open file: ", szFullName);
*/
		}

		if ( i == totalIncludePaths ) {
		    Error2( "Unable to find ", szIn );
		}

		*returnFile = file;
		return szFullName;
	} 
	else 
	{
		*returnFile = file;
		return szIn;
	}
}

/* case insenstitive string comparison	*/
BOOL FSzEqI(char *sz1, char *sz2)
	{
	while (tolower(*sz1) == tolower(*sz2))
		{
		if (*sz1 == 0)
			return fTrue;
		sz1++;
		sz2++;
		}
	return fFalse;
	}


int WMin(int w1, int w2)
	{
	return w1 < w2 ? w1 : w2;
	}

int WMax(int w1, int w2)
	{
	return w1 > w2 ? w1 : w2;
	}
	

