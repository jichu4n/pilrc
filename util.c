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


/* Includes */
char *includePaths[MAXPATHS];
int   totalIncludePaths = 0;


/* font widths for font 0 */
unsigned char mpchdxFont0[] =
	{
	0, 0, 0, 0, 0, 0, 0, 0,   /* 0 */
	0, 0, 0, 0, 0, 0, 0, 0,   /* 8 */
	0, 0, 0, 0, 0, 0, 0, 0,   /* 10 */
	0, 0, 0, 0, 0, 0, 0, 0,   /* 18 */
	2, 2, 4, 8, 6, 8, 7, 2,   /* 20 */
	4, 4, 6, 6, 3, 4, 2, 5,   /* 28 */
	5, 5, 5, 5, 5, 5, 5, 5,   /* 30	 // 0..7 */
	5, 5, 2, 3, 6, 6, 6, 5,   /* 38   // 8.. */
	8, 5, 5, 5, 6, 4, 4, 6,   /* 40 */
	6, 2, 4, 6, 5, 8, 6, 7,   /* 48 */
	5, 7, 5, 5, 6, 6, 6, 8,   /* 50 */
	6, 6, 6, 3, 5, 3, 6, 4,   /* 58 */
	3, 5, 5, 4, 5, 5, 4, 5,   /* 60 */
	5, 2, 3, 5, 2, 8, 5, 5,   /* 68 */
	5, 5, 4, 4, 4, 5, 5, 6,   /* 70 */
	6, 6, 4, 4, 2, 4, 7, 5,   /* 78 */
	5, 5, 3, 8, 5, 6, 6, 6,   /* 80 */
	4, 11,5, 4, 8,10,10,10,  /* 88 */
	10,3, 3, 5, 5, 4, 4, 7,  /* 90 */
	7, 10,4, 4, 8, 5, 5, 6,  /* 98 */
	2, 2, 6, 6, 8, 6, 2, 5,   /* a0 */
	4, 8, 5, 6, 6, 4, 8, 6,   /* a8			// (c).. */
	5, 6, 4, 4, 3, 5, 6, 2,   /* b0 */
	4, 2, 5, 6, 8, 8, 8, 5,   /* b8 */
	5, 5, 5, 5, 5, 5, 7, 5,   /* c0 */
	4, 4, 4, 4, 3, 2, 3, 3,   /* c8 */
	7, 6, 7, 7, 7, 7, 7, 5,   /* d0 */
	8, 6, 6, 6, 6, 6, 6, 6,   /* d8 */
	5, 5, 5, 5, 5, 5, 8, 4,   /* e0 */
	5, 5, 5, 5, 2, 2, 3, 3,   /* e8 */
	5, 5, 5, 5, 5, 5, 5, 6,   /* f0 */
	7, 5, 5, 5, 5, 6, 5, 6};

unsigned char mpchdxFont1[] =
	{
	5,5,5,5,5,5,5,5,
	5,2,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,
	2,3,6,10,6,13,9,3,
	5,5,6,6,3,5,3,6,
	6,6,6,6,6,6,6,6,
	6,6,3,3,6,6,6,6,
	10,7,7,6,7,5,5,8,
	8,3,5,7,6,10,7,8,
	7,8,8,6,7,7,8,11,
	7,7,7,4,6,4,6,5,
	4,6,6,5,6,6,5,6,
	6,3,4,6,3,9,6,6,
	6,6,5,5,6,6,6,9,
	6,6,5,5,3,5,7,5,
	6,5,3,9,5,6,5,5, /* 80 */
	4,17,6,5,10,10,10,10,
	10,3,3,5,5,4,4,6,
	7,10,5,5,10,5,5,7,
	2,3,6,7,8,7,3,6,
	4,8,6,8,6,5,8,6,
	5,6,4,4,4,6,7,2,
	4,2,6,8,9,9,9,6,
	7,7,7,7,7,7,9,6,
	5,5,5,5,3,3,3,3,
	8,7,8,8,8,8,8,6,
	8,7,7,7,7,7,7,8,
	6,6,6,6,6,6,9,5,
	6,6,6,6,3,3,3,3,
	6,6,6,6,6,6,6,7,
	8,6,6,6,6,6,6,6
	};

unsigned char mpchdxFont2[] =
	{
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 4, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	4, 2, 4, 9, 6, 11,8, 2,
	4, 4, 6, 8, 3, 4, 2, 5, 
	7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 2, 3, 7, 7, 7, 5, 
	11,9, 6, 7, 7, 6, 6, 8, 
	7, 3, 4, 7, 5, 10,7, 8, 	/* 48 */
	6, 8, 6, 5, 6, 7, 7, 11, 
	7, 6, 5, 3, 5, 3, 6, 6, 
	3, 6, 7, 6, 7, 7, 4, 7, 
	6, 3, 3, 6, 2, 10,7, 7, 
	7, 7, 4, 5, 4, 7, 6, 10, 
	6, 7, 6, 4, 2, 4, 7, 5, 	/* 78 */  
	7, 5, 3, 7, 5, 10,6, 6, 
	4, 13,5, 4, 9, 10,10,10, 
	10,3, 3, 5, 5, 5, 6, 12, 
	7, 11,5, 4, 12,5, 5, 8, 
	4, 2, 6, 6, 8, 8, 2, 6, 
	4, 10,5, 7, 7, 4, 10,4, 
	5, 6, 5, 4, 3, 6, 6, 2, 
	4, 3, 5, 7, 8, 8, 8, 5, 
	9, 9, 9, 9, 9, 9, 9, 7, 
	6, 6, 6, 6, 3, 2, 3, 3, 
	8, 7, 8, 8, 8, 8, 8, 6, 
	8, 7, 7, 7, 7, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 11,6, 
	7, 7, 7, 7, 3, 2, 3, 3, 
	6, 7, 7, 7, 7, 7, 7, 6, 
	7, 7, 7, 7, 7, 7, 7, 6};


unsigned char mpchdxFont3[] =				
	{
	5,	/* nil */
	5,	/* nil */
	5,	/* nil */
	11, /* big left arrow */
	10,	/* big right arrow */
	10, /* big up arrow */
	10,	/* big dn arrow */
	11,	/* small dn arrow */
	11, /* small up arrow */
	7,	/* textbox? */
	4,	/* i */
	6,	/* white page paper */
	6,	/* black page paper */
	19,	/* GSI cap */
	19,	/* GSI num */
	19,	/* GSI sift */
	19,	/* GSI punct */
	19,	/* GSI intl */
	19,	/* GSI blank */
	7,	/* diamond */
	};

unsigned char mpchdxFont4[] =				
	{
	12,	/* Unchecked Checkbox */
	12,	/* Checked Checkbox */
	8,	/* Left Triangle */
	8,	/* Right Triangle */
	12,	/* Info */
	};

unsigned char mpchdxFont5[] =
	{
	5,	/* Square */
	11,	/* UpScroll */
	11,	/* DnScroll */
	11, /* UpScroll Disabled */
	11,	/* DnScroll Disabled */
	};


int DxChar(int ch, int ifnt)
	{
	switch (ifnt)
		{
	case 0:
		return mpchdxFont0[ch];
	case 1:
		return mpchdxFont1[ch];
	case 2:
		return mpchdxFont2[ch];
	case 3:
		if (ch < sizeof(mpchdxFont3))
			return mpchdxFont3[ch];
		break;
	case 4:
		if (ch < sizeof(mpchdxFont4))
			return mpchdxFont4[ch];
		break;
	case 5:
		if (ch < sizeof(mpchdxFont5))
			return mpchdxFont5[ch];
		}
	// default width is 5 for the upper fonts
	return 5;
	}

/*-----------------------------------------------------------------------------
|	DxCalcRgdx
|	
|		Get extent of pilot string, placing each char width in rgdx
|	BUG! only works for font 0
-------------------------------------------------------------WESC------------*/
int DxCalcRgdx(unsigned char *sz, int ifnt, int *rgdx)
	{
	unsigned char *pch;
	int dx;

	if (sz == NULL)
		return 0;
	dx = 0;

	for (pch = sz; *pch != 0; pch++)
		{
		if (rgdx != NULL)
			rgdx[pch-sz] = DxChar(*pch, ifnt);
		dx += DxChar(*pch, ifnt);
		}
	return dx;
	}	
	
/*-----------------------------------------------------------------------------
|	DxCalcExtent
|	
|		Calc extent of string -- BUG! only works for font 0 (and bold)
-------------------------------------------------------------WESC------------*/
int DxCalcExtent(unsigned char *sz, int ifnt)
	{
	unsigned char *pch;
	int dx;

	if (sz == NULL)
		return 0;
	dx = 0;

	for (pch = sz; *pch != 0; pch++)
		{
		dx += DxChar(*pch, ifnt);
		}
	return dx;
	}	




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
		sprintf(szErr, "%s(%d) : error : %s", szInFile, iline, sz);
	else
		sprintf(szErr, "%s(%d) : error : %s %s", szInFile, iline, sz, sz2);
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

	sprintf(szErr, "%s(%d) : warning : %s", szInFile, iline, sz);
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
	EmitB((unsigned char) ((w & 0xff00) >> 8));
	EmitB((unsigned char) (w & 0xff));
	}

/*-----------------------------------------------------------------------------
|	EmitL
|	
|		emit a long
-------------------------------------------------------------WESC------------*/
void EmitL(unsigned long l)
	{
	EmitB((unsigned char) ((l & 0xff000000) >> 24));
	EmitB((unsigned char) ((l & 0xff0000) >> 16));
	EmitB((unsigned char) ((l & 0xff00) >> 8));
	EmitB((unsigned char) (l & 0xff));
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
	if (vfWinGUI)
		return;
/*#ifdef BINOUT */
	fwrite(pv, cb, 1, vfhOut);
	ibOut += cb;
/*#endif */
#ifdef HEXOUT 
	BYTE *pb;
	
	pb = (BYTE *)pv;
	while(cb--)
		{
		int ib;
		static int ibLine;
		static BYTE rgbLine[16];
		
		if (ibLine == 0)
			printf("%08x   ", ibOut);
		rgbLine[ibLine++] = *pb;
		printf("%02x ", *pb);
		pb++;
		ibOut++;
		if (ibLine == 16)
			{
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
	if (ibOut & 1)
		DumpBytes(rgbZero, 1);
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
		for ( i = 0; i < totalIncludePaths; i++ ) {

		    sprintf( szFullName, "%s\\%s", includePaths[i], szIn);
		
		    file = fopen( szFullName, mode);
		    if ( file != NULL )
			{
			    break;
			}
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
	

