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
	
	if (sz2 == NULL)  // updated: Christian Falch
		sprintf(szErr, "%s:%d: error : %s", szInFile, iline, sz);
	else
		sprintf(szErr, "%s:%d: error : %s %s", szInFile, iline, sz, sz2);
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

	sprintf(szErr, "%s:%d: warning : %s", szInFile, iline, sz);
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

#ifdef WIN322
		  sprintf( szFullName, "%s\\%s", includePaths[i], szIn);
#else
		  sprintf( szFullName, "%s/%s", includePaths[i], szIn);
#endif
		
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
	

