/*-----------------------------------------------------------------------------
| bitmap.c -- Bitmap converter 
|
|	David Dawson - dawson@pobox.com
|	05 Mar 1997
|
|	Wesc 08 Mar 1997: removed Windows dependencies
|
|   Most of the code bellow was taken verbatim out of pila written by
|   Darrin Massena.
|
| See pilrc.htm for documentation
-------------------------------------------------------------WESC------------*/
#pragma pack(2)

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <memory.h>
#include <string.h>
#include "pilrc.h"


typedef unsigned char byte;     /* b */
typedef unsigned short ushort;  /* us */
typedef unsigned long ulong;    /* ul */

typedef int bool;               /* f */


ulong LLoadX86(ulong ul)
	{
	byte *pb;
	ulong ulOut;

	pb = (byte *) &ul;
	ulOut = (ulong) ((*(pb+3)<<24) | (*(pb+2)<<16) | (*(pb+1)<<8) | (*pb));
	return ulOut;
	}

ushort WLoadX86(ushort w)
	{
	byte *pb;
	ushort wOut;
	
	pb = (byte *) &w;
	wOut = (ushort) ((*(pb+1)<<8) | (*pb));
	return wOut;
	}



#define SwapBytes(us)	(ushort)((((ushort)(us)) >> 8) | (((ushort)(us)) << 8))



/* Pilot bitmap resource format */
typedef struct _Bitmap
	{ /* bm */
    ushort cx;
    ushort cy;
    ushort cbRow;
    ushort ff;
    ushort ausUnk[4];
    byte abBits[1];
	} Bitmap;

/* Windows bitmap format */
typedef struct tagBITMAPFILEHEADER 
	{ /* bmfh */
	ushort  bfType; 
#ifdef __GNUC__
	ushort  bfSize1; 
	ushort  bfSize2; 
#else
	ulong   bfSize; 
#endif
	ushort  bfReserved1; 
	ushort  bfReserved2; 
#ifdef __GNUC__
	ushort  bfOffBits1; 
	ushort  bfOffBits2; 
#else
	ulong   bfOffBits; 
#endif
	} BITMAPFILEHEADER; 
 


typedef struct tagBITMAPINFOHEADER
	{ /* bmih */
	ulong  biSize; 
	long   biWidth; 
	long   biHeight; 
	ushort biPlanes; 
	ushort biBitCount;
	ulong  biCompression; 
	ulong  biSizeImage; 
	long   biXPelsPerMeter; 
	long   biYPelsPerMeter; 
	ulong  biClrUsed; 
	ulong  biClrImportant; 
	} BITMAPINFOHEADER; 

typedef struct tagRGBQUAD 
	{ /* rgbq */
    byte    rgbBlue; 
    byte    rgbGreen; 
    byte    rgbRed; 
    byte    rgbReserved; 
	} RGBQUAD; 
 

typedef struct tagBITMAPINFO 
	{ /* bmi */
	BITMAPINFOHEADER bmiHeader; 
	RGBQUAD          bmiColors[1]; 
	} BITMAPINFO; 
 




/* Test bit in a 1bpp bitmap */
bool TestBit(int cx, int cy, byte *pb, int x, int y, int cBitsAlign)
	{
    int cbRow = (cx + (cBitsAlign - 1)) & ~(cBitsAlign - 1);
    pb += (cbRow >> 3) * y + (x >> 3);
    return (*pb & (1 << (7 - (x & 7))));
	}

/* Set bit in a 1bpp bitmap */
void SetBit(int cx, int cy, byte *pb, int x, int y, int cBitsAlign)
	{
    int cbRow = (cx + (cBitsAlign - 1)) & ~(cBitsAlign - 1);
    pb += (cbRow >> 3) * y + (x >> 3);
    *pb |= (1 << (7 - (x & 7)));
	}

/* Convert from .bmp to Pilot resource data */
void ConvertDumpWindowsBitmap(byte *pbResData, int isIcon)
	{
    byte *pbSrc;
    int cbDst, cbRow;
	RCBITMAP rcbmp;
    int x, y;
    BITMAPINFO *pbmi;
	
	pbmi = (BITMAPINFO *)(pbResData + sizeof(BITMAPFILEHEADER));

	pbmi->bmiHeader.biSize = LLoadX86(pbmi->bmiHeader.biSize);
	pbmi->bmiHeader.biWidth = LLoadX86(pbmi->bmiHeader.biWidth);
	pbmi->bmiHeader.biHeight = LLoadX86(pbmi->bmiHeader.biHeight);
	pbmi->bmiHeader.biBitCount = WLoadX86(pbmi->bmiHeader.biBitCount);

    pbSrc = ((byte *)pbmi) + pbmi->bmiHeader.biSize + sizeof(RGBQUAD) * 2;

    /* If image not 1bpp, bail	 */

    if (pbmi->bmiHeader.biBitCount != 1)
		{
        ErrorLine ("Bitmap not monochrome");
		}

    /* Alloc what we need for image data */
    /* Pilot images are word aligned */

	memset(&rcbmp, 0, sizeof(RCBITMAP));

    cbRow = ((pbmi->bmiHeader.biWidth + 15) & ~15) / 8;
    cbDst = cbRow * pbmi->bmiHeader.biHeight;
	rcbmp.pbBits = malloc(cbDst);

    /* Image data has been inverted for Macintosh, so invert back */

    memset(rcbmp.pbBits, 0, cbDst);

    /* Convert from source bitmap format (DWORD aligned) to dst format (word aligned). */

    for (y = 0; y < pbmi->bmiHeader.biHeight; y++)
		{
        for (x = 0; x < pbmi->bmiHeader.biWidth; x++)
			{
            /* Reverse bits so we get WYSIWYG in msdev (white
               pixels become background (0), black pixels become
               foreground (1)). */

            int yT = y;
            if (pbmi->bmiHeader.biHeight > 0)
                yT = pbmi->bmiHeader.biHeight - y - 1;

            if (!TestBit(pbmi->bmiHeader.biWidth, pbmi->bmiHeader.biHeight,
                         pbSrc, x, yT, 32))
				{
                SetBit(pbmi->bmiHeader.biWidth, pbmi->bmiHeader.biHeight,
                       rcbmp.pbBits, x, y, 16);
				}
			}
		}
    rcbmp.cx = (int) pbmi->bmiHeader.biWidth;
	rcbmp.cy = (int) pbmi->bmiHeader.biHeight;
	rcbmp.cbRow = (int) cbRow;
	
	if (isIcon && 
		 (abs(pbmi->bmiHeader.biHeight) !=32 || 
		   pbmi->bmiHeader.biWidth != 32))
		{
		ErrorLine("Icon resource not 32x32");
		}

	CbEmitStruct(&rcbmp, szRCBITMAP, NULL, fTrue);
	DumpBytes(rcbmp.pbBits, cbDst);
	free(rcbmp.pbBits);
	}


void InvalidExtension(char *fileName)
	{
	char pchLine[300];
	sprintf(pchLine, 
			 "Bitmap file extension not recognized for file %s\n"
			 "\tSupported extensions: .BMP (Windows bitmap)"
			 , fileName);
	ErrorLine(pchLine);
	}

void DumpBitmap(char *fileName, int isIcon)
	{
	byte *pBMPData;
	char *pchExt;
	FILE *pFile;
	long size;

	pchExt = strrchr(fileName, '.');

	if ((strlen(fileName) < 5) || (pchExt == NULL))
		{
		InvalidExtension(fileName);
		}
	
	fileName = FindAndOpenFile(fileName, "rb", &pFile);

	fseek(pFile,0,SEEK_END);
	size = ftell(pFile);
	rewind(pFile);
		
	pBMPData = malloc(size);
	if (pBMPData == NULL)
		{
		char pchLine[200];
		sprintf(pchLine, "Resource %s too big to fit in memory", fileName);
		ErrorLine(pchLine);
		}

	fread(pBMPData, 1, size, pFile);
	fclose(pFile);

	pchExt = strrchr(fileName, '.');
	if (!pchExt) 
		{
		InvalidExtension(fileName);
		}

	pchExt++;

	if (FSzEqI(pchExt, "bmp"))
		{
		ConvertDumpWindowsBitmap(pBMPData, isIcon);
        }
	else
		{
		InvalidExtension(fileName);
		}
	free(pBMPData);
	}
