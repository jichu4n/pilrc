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
|	PBM conversion by Andrew Bulhak (acb@dev.null.org)
|	XBM, PBITM and compression by Hoshi Takanori (hoshi@sra.co.jp)
|
| See pilrc.htm for documentation
-------------------------------------------------------------WESC------------*/
#ifndef __GNUC__
#pragma pack(2)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#include "pilrc.h"


typedef unsigned char  PILRC_BYTE;	  /* b */
typedef unsigned short PILRC_USHORT;  /* us */
typedef unsigned int PILRC_ULONG;	  /* ul */


typedef int bool;					  /* f */

static
int PalmPalette256[256][3] = 
{
  { 255, 255, 255 }, { 255, 204, 255 }, { 255, 153, 255 }, { 255, 102, 255 }, 
  { 255,  51, 255 }, { 255,   0, 255 }, { 255, 255, 204 }, { 255, 204, 204 }, 
  { 255, 153, 204 }, { 255, 102, 204 }, { 255,  51, 204 }, { 255,   0, 204 }, 
  { 255, 255, 153 }, { 255, 204, 153 }, { 255, 153, 153 }, { 255, 102, 153 }, 
  { 255,  51, 153 }, { 255,   0, 153 }, { 204, 255, 255 }, { 204, 204, 255 },
  { 204, 153, 255 }, { 204, 102, 255 }, { 204,  51, 255 }, { 204,   0, 255 },
  { 204, 255, 204 }, { 204, 204, 204 }, { 204, 153, 204 }, { 204, 102, 204 },
  { 204,  51, 204 }, { 204,   0, 204 }, { 204, 255, 153 }, { 204, 204, 153 },
  { 204, 153, 153 }, { 204, 102, 153 }, { 204,  51, 153 }, { 204,   0, 153 },
  { 153, 255, 255 }, { 153, 204, 255 }, { 153, 153, 255 }, { 153, 102, 255 },
  { 153,  51, 255 }, { 153,   0, 255 }, { 153, 255, 204 }, { 153, 204, 204 },
  { 153, 153, 204 }, { 153, 102, 204 }, { 153,  51, 204 }, { 153,   0, 204 },
  { 153, 255, 153 }, { 153, 204, 153 }, { 153, 153, 153 }, { 153, 102, 153 },
  { 153,  51, 153 }, { 153,   0, 153 }, { 102, 255, 255 }, { 102, 204, 255 },
  { 102, 153, 255 }, { 102, 102, 255 }, { 102,  51, 255 }, { 102,   0, 255 },
  { 102, 255, 204 }, { 102, 204, 204 }, { 102, 153, 204 }, { 102, 102, 204 },
  { 102,  51, 204 }, { 102,   0, 204 }, { 102, 255, 153 }, { 102, 204, 153 },
  { 102, 153, 153 }, { 102, 102, 153 }, { 102,  51, 153 }, { 102,   0, 153 },
  {  51, 255, 255 }, {  51, 204, 255 }, {  51, 153, 255 }, {  51, 102, 255 },
  {  51,  51, 255 }, {  51,   0, 255 }, {  51, 255, 204 }, {  51, 204, 204 },
  {  51, 153, 204 }, {  51, 102, 204 }, {  51,  51, 204 }, {  51,   0, 204 },
  {  51, 255, 153 }, {  51, 204, 153 }, {  51, 153, 153 }, {  51, 102, 153 },
  {  51,  51, 153 }, {  51,   0, 153 }, {   0, 255, 255 }, {   0, 204, 255 },
  {   0, 153, 255 }, {   0, 102, 255 }, {   0,  51, 255 }, {   0,   0, 255 },
  {   0, 255, 204 }, {   0, 204, 204 }, {   0, 153, 204 }, {   0, 102, 204 },
  {   0,  51, 204 }, {   0,   0, 204 }, {   0, 255, 153 }, {   0, 204, 153 },
  {   0, 153, 153 }, {   0, 102, 153 }, {   0,  51, 153 }, {   0,   0, 153 },
  { 255, 255, 102 }, { 255, 204, 102 }, { 255, 153, 102 }, { 255, 102, 102 },
  { 255,  51, 102 }, { 255,   0, 102 }, { 255, 255,  51 }, { 255, 204,  51 },
  { 255, 153,  51 }, { 255, 102,  51 }, { 255,  51,  51 }, { 255,   0,  51 },
  { 255, 255,   0 }, { 255, 204,   0 }, { 255, 153,   0 }, { 255, 102,   0 },
  { 255,  51,   0 }, { 255,   0,   0 }, { 204, 255, 102 }, { 204, 204, 102 },
  { 204, 153, 102 }, { 204, 102, 102 }, { 204,  51, 102 }, { 204,   0, 102 },
  { 204, 255,  51 }, { 204, 204,  51 }, { 204, 153,  51 }, { 204, 102,  51 },
  { 204,  51,  51 }, { 204,   0,  51 }, { 204, 255,   0 }, { 204, 204,   0 },
  { 204, 153,   0 }, { 204, 102,   0 }, { 204,  51,   0 }, { 204,   0,   0 },
  { 153, 255, 102 }, { 153, 204, 102 }, { 153, 153, 102 }, { 153, 102, 102 },
  { 153,  51, 102 }, { 153,   0, 102 }, { 153, 255,  51 }, { 153, 204,  51 },
  { 153, 153,  51 }, { 153, 102,  51 }, { 153,  51,  51 }, { 153,   0,  51 },
  { 153, 255,   0 }, { 153, 204,   0 }, { 153, 153,   0 }, { 153, 102,   0 },
  { 153,  51,   0 }, { 153,   0,   0 }, { 102, 255, 102 }, { 102, 204, 102 },
  { 102, 153, 102 }, { 102, 102, 102 }, { 102,  51, 102 }, { 102,   0, 102 },
  { 102, 255,  51 }, { 102, 204,  51 }, { 102, 153,  51 }, { 102, 102,  51 },
  { 102,  51,  51 }, { 102,   0,  51 }, { 102, 255,   0 }, { 102, 204,   0 },
  { 102, 153,   0 }, { 102, 102,   0 }, { 102,  51,   0 }, { 102,   0,   0 },
  {  51, 255, 102 }, {  51, 204, 102 }, {  51, 153, 102 }, {  51, 102, 102 },
  {  51,  51, 102 }, {  51,   0, 102 }, {  51, 255,  51 }, {  51, 204,  51 },
  {  51, 153,  51 }, {  51, 102,  51 }, {  51,  51,  51 }, {  51,   0,  51 },
  {  51, 255,   0 }, {  51, 204,   0 }, {  51, 153,   0 }, {  51, 102,   0 },
  {  51,  51,   0 }, {  51,   0,   0 }, {   0, 255, 102 }, {   0, 204, 102 },
  {   0, 153, 102 }, {   0, 102, 102 }, {   0,  51, 102 }, {   0,   0, 102 },
  {   0, 255,  51 }, {   0, 204,  51 }, {   0, 153,  51 }, {   0, 102,  51 },
  {   0,  51,  51 }, {   0,   0,  51 }, {   0, 255,   0 }, {   0, 204,   0 },
  {   0, 153,   0 }, {   0, 102,   0 }, {   0,  51,   0 }, {  17,  17,  17 },
  {  34,  34,  34 }, {  68,  68,  68 }, {  85,  85,  85 }, { 119, 119, 119 },
  { 136, 136, 136 }, { 170, 170, 170 }, { 187, 187, 187 }, { 221, 221, 221 },
  { 238, 238, 238 }, { 192, 192, 192 }, { 128,   0,   0 }, { 128,   0, 128 },
  {   0, 128,   0 }, {   0, 128, 128 }, {   0,   0,   0 }, {   0,   0,   0 },
  {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
  {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
  {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
  {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
  {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
  {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }
};
#define COLOR_TABLE_SIZE (sizeof(PalmPalette256) + 2)

#define sqr(x) ((x)*(x))

/**
 * Convert a RGB triplet to a index within the PalmPalette256[][] table.
 *
 * @param r the red RGB value
 * @param g the green RGB value
 * @param b the blue RGB value
 * @return the index of the RGB triplet in the PalmPalette256 table.
 */
static
int 
RGBToColorIndex(int r, int g, int b)
{
  int index = 0;
  int *diffArray;
  int lowValue;
  int i;

  // generate the color "differences" for all colors in the palette
  diffArray = (int *)malloc(256 * sizeof(int));
  for (i=0; i < 256; i++) {
    diffArray[i] = sqr(PalmPalette256[i][0] - r) +
                   sqr(PalmPalette256[i][1] - g) + 
                   sqr(PalmPalette256[i][2] - b);
  }

  // find the palette index that has the smallest color "difference"
  index    = 0;
  lowValue = diffArray[0];
  for (i=1; i<256; i++) {
    if (diffArray[i] < lowValue) {
      lowValue = diffArray[i];
      index    = i;
    }
  }

  // clean up
  free(diffArray);

  return index;
}

PILRC_ULONG LLoadX86(PILRC_ULONG ul)
{
	PILRC_BYTE *pb;
	PILRC_ULONG ulOut;

	pb = (PILRC_BYTE *) &ul;
	ulOut = (PILRC_ULONG) ((*(pb+3)<<24) | (*(pb+2)<<16) | (*(pb+1)<<8) | (*pb));
	return ulOut;
}

PILRC_USHORT WLoadX86(PILRC_USHORT w)
{
	PILRC_BYTE *pb;
	PILRC_USHORT wOut;

	pb = (PILRC_BYTE *) &w;
	wOut = (PILRC_USHORT) ((*(pb+1)<<8) | (*pb));
	return wOut;
}



/* #define SwapPILRC_BYTEs(us)	(PILRC_USHORT)((((PILRC_USHORT)(us)) >> 8) | (((PILRC_USHORT)(us)) << 8)) */


/* Windows bitmap format */
typedef struct tagBITMAPFILEHEADER 
	{ /* bmfh */
	PILRC_USHORT  bfType; 
#ifdef __GNUC__
	PILRC_USHORT  bfSize1;
	PILRC_USHORT  bfSize2;
#else
	PILRC_ULONG   bfSize; 
#endif
	PILRC_USHORT  bfReserved1; 
	PILRC_USHORT  bfReserved2; 
#ifdef __GNUC__
	PILRC_USHORT  bfOffBits1;
	PILRC_USHORT  bfOffBits2;
#else
	PILRC_ULONG   bfOffBits; 
#endif
	} BITMAPFILEHEADER; 



typedef struct tagBITMAPINFOHEADER
	{ /* bmih */
	PILRC_ULONG  biSize; 
	long   biWidth; 
	long   biHeight; 
	PILRC_USHORT biPlanes; 
	PILRC_USHORT biBitCount;
	PILRC_ULONG  biCompression; 
	PILRC_ULONG  biSizeImage; 
	long   biXPelsPerMeter; 
	long   biYPelsPerMeter; 
	PILRC_ULONG  biClrUsed; 
	PILRC_ULONG  biClrImportant; 
	} BITMAPINFOHEADER; 

typedef struct tagRGBQUAD 
	{ /* rgbq */
	PILRC_BYTE    rgbBlue; 
	PILRC_BYTE    rgbGreen; 
	PILRC_BYTE    rgbRed; 
	PILRC_BYTE    rgbReserved; 
	} RGBQUAD; 


typedef struct tagBITMAPINFO 
	{ /* bmi */
	BITMAPINFOHEADER bmiHeader; 
	RGBQUAD          bmiColors[1]; 
	} BITMAPINFO; 



int CbRow(int cx, int cbpp, int cBitsAlign)
	{
	return ((cx*cbpp + (cBitsAlign - 1)) & ~(cBitsAlign - 1))/8;
	}


/* Test bit in a 1bpp bitmap */
bool TestBit(int cx, PILRC_BYTE *pb, int x, int y, int cBitsAlign)
{
	int cbRow;
	
	cbRow = CbRow(cx, 1, cBitsAlign);
	pb += cbRow * y + (x >> 3);
	return (*pb & (1 << (7 - (x & 7))));
}

/* Set bit in a 1bpp bitmap */
void SetBit(int cx, PILRC_BYTE *pb, int x, int y, int cBitsAlign)
{
	int cbRow;

	cbRow = CbRow(cx, 1, cBitsAlign);
	pb += cbRow * y + (x >> 3);
	*pb |= (1 << (7 - (x & 7)));
}


/* Set bit in a 8bpp bitmap */
int GetBits8bpp(int cx, PILRC_BYTE *pb, int x, int y, int cBitsAlign)
{
	int cbRow;

	cbRow = CbRow(cx, 8, cBitsAlign);
	pb += cbRow * y + x;

	return *pb;
}

/* Set bit in a 8bpp bitmap */
void SetBits8bpp(int cx, PILRC_BYTE *pb, int x, int y, int bits, int cBitsAlign)
{
	int cbRow;

	cbRow = CbRow(cx, 8, cBitsAlign);
	pb += cbRow * y + x;
	*pb = bits;
}

/* gets bits in a 4bpp bitmap */
int GetBits4bpp(int cx, PILRC_BYTE *pb, int x, int y, int cBitsAlign)
{
	int cbRow;

	cbRow = CbRow(cx, 4, cBitsAlign);
	pb += cbRow * y + (x >> 1);
	if (x & 1)
		return (*pb & 0xf);
	else
		return (*pb & 0xf0) >> 4;
}

/* Set bit in a 4bpp bitmap */
void SetBits4bpp(int cx, PILRC_BYTE *pb, int x, int y, int bits, int cBitsAlign)
{
	int cbRow;

	cbRow = CbRow(cx, 4, cBitsAlign);
	pb += cbRow * y + (x >> 1);
	if (x & 1)
		*pb |= bits;
	else
		*pb |= (bits << 4);
}

/* Set bit in a 2bpp bitmap */
void SetBits2bpp(int cx, PILRC_BYTE *pb, int x, int y, int bits, int cBitsAlign)
{
	int msk;
	int cbRow;

	cbRow = CbRow(cx, 2, cBitsAlign);
	pb += cbRow * y + (x >> 2);
	msk = (bits << ((3 - (x & 3))*2));
	*pb |= msk;
	
}


/* Convert from .bmp to Pilot resource data */
void ConvertWindowsBitmap(RCBITMAP *rcbmp, PILRC_BYTE *pbResData, int bitmaptype, BOOL colortable)
{
	PILRC_BYTE *pbSrc;
	int cbRow;
	int x, y, colorData;
	int cbHeader, dx, dy, cbits;
	int cbitsPel;
	BITMAPINFO *pbmi;
	BITMAPINFOHEADER bmi;
	BOOL fNonGreyWarning;

	pbmi = (BITMAPINFO *)(pbResData + sizeof(BITMAPFILEHEADER)); 

	/* alignment of structure is incorrect for sparc.  Copy
	 structure to known alignment before extracting bitmap parameters
	 */
	memcpy (&bmi, pbmi,sizeof(BITMAPINFOHEADER));
	cbHeader = LLoadX86(bmi.biSize);
	dx = LLoadX86(bmi.biWidth);
	dy = LLoadX86(bmi.biHeight);
	cbits = WLoadX86(bmi.biBitCount);
/*	printf ("bitmap width:%d  dy:%d  size:%d  bitcount:%d\n",
		dx, dy, cbHeader, cbits); */

	pbSrc = ((PILRC_BYTE *)pbmi) + cbHeader + sizeof(RGBQUAD) * (1 << cbits);

	/* If image not 1bpp, bail	 */

	cbitsPel = -1;
	colorData = 0;
        switch (bitmaptype) 
        {
          case rwBitmap:
	       if (cbits != 1)
		 ErrorLine ("Bitmap not monochrome");
	       cbitsPel = 1;
               break;

          case rwBitmapGrey:
	       if (cbits != 4)
		 ErrorLine ("Bitmap not 16 color");
	       cbitsPel = 2;
               break;

          case rwBitmapGrey16:
	       if (cbits != 4)
		 ErrorLine ("Bitmap not 16 color");
	       cbitsPel = 4;
               break;

          case rwBitmapColor:
	       if (cbits != 8)
		 ErrorLine ("Bitmap not 256 color");
	       cbitsPel = 8;
	       if (colortable) colorData = 1026;
               break;

          default:
               Assert(fFalse);
               break;
        }

	/* Alloc what we need for image data */
	/* Pilot images are word aligned */
	
	cbRow = ((dx*cbitsPel + 15) & ~15) / 8;
	rcbmp->cbDst = (cbRow * dy) + colorData;
	rcbmp->pbBits = malloc(rcbmp->cbDst);

	/* Image data has been inverted for Macintosh, so invert back */

	memset(rcbmp->pbBits, 0, rcbmp->cbDst);

        switch (bitmaptype) 
        {
          case rwBitmap:
          case rwBitmapGrey:
          case rwBitmapGrey16:
               rcbmp->pixelsize= cbitsPel;
               rcbmp->version  = 1;
               break;

          case rwBitmapColor:
               rcbmp->pixelsize= cbitsPel;
               rcbmp->version  = 2;

	       if (colortable) {

		 PILRC_BYTE *tmpPtr;
		 int i;

		 rcbmp->ff |= 0x4000;
	    	 tmpPtr     = rcbmp->pbBits;
	    	 *tmpPtr++  = 0x10;
	    	 *tmpPtr++  = 0x00;

		 // extract the color table
		 for (i=0; i<256; i++) {
		   *tmpPtr++ = i;
		   *tmpPtr++ = pbmi->bmiColors[i].rgbRed;
		   *tmpPtr++ = pbmi->bmiColors[i].rgbGreen;
		   *tmpPtr++ = pbmi->bmiColors[i].rgbBlue;
		 }
	       }
	       else {

		 int *paletteXref;
		 int i;

		 paletteXref = (int *)malloc(256 * sizeof(int));
		 for (i=0; i<256; i++) {
		   paletteXref[i] = 
			RGBToColorIndex(pbmi->bmiColors[i].rgbRed,
			                pbmi->bmiColors[i].rgbGreen,
			                pbmi->bmiColors[i].rgbBlue);
		 }

		 for (y=0; y<dy; y++) {
		   for (x=0; x<dx; x++) {

			int w, yT = y;
			if (dy > 0)
				yT = dy - y - 1;
	
			w = GetBits8bpp(dx, pbSrc, x, yT, 32);
			SetBits8bpp(dx, pbSrc, x, yT, paletteXref[w], 32);
		   }
		 }

		 free(paletteXref);
	       }

               break;

          default:
               break;
        }

	/* Convert from source bitmap format (DWORD aligned) to dst format (word 
aligned). */
	
	fNonGreyWarning = fFalse;
	for (y = 0; y < dy; y++)
		{
		for (x = 0; x < dx; x++)
			{
			/* Reverse bits so we get WYSIWYG in msdev (white
				pixels become background (0), black pixels become
				foreground (1)). */

			int yT = y;
			if (dy > 0)
				yT = dy - y - 1;

        switch (bitmaptype) 
        {
          case rwBitmap:
				{
				if (!TestBit(dx, pbSrc, x, yT, 32))
					{
					SetBit(dx, rcbmp->pbBits, x, y, 16);
					}
				}
               break;

          case rwBitmapGrey:
				{
				int w;
				
				w = GetBits4bpp(dx, pbSrc, x, yT, 32);
				switch (w)
					{
				default:    /* lt grey */
					if (!fNonGreyWarning)	
						{
						WarningLine("Bitmap has non black,white,dkgray,ltgray pixels");
						fNonGreyWarning = fTrue;
						}
					w = 2;	
					break;
				case 0:	  	/* black (on) */
					w = 3;   
					break;
				case 7:	   /* lt gray (1/3 on) */
					w = 1;
					break;
				case 8:	   /* dk gray (2/3 on) */
					w = 2;
					break;				
				case 15:		/* white  (not on) */
					w = 0;	
					break;
					}
				SetBits2bpp(dx, rcbmp->pbBits, x, y, w, 16);
				}
               break;

          case rwBitmapGrey16:
				{
				int w = 15 - GetBits4bpp(dx, pbSrc, x, yT, 32);
				SetBits4bpp(dx, rcbmp->pbBits, x, y, w, 16);
				}
               break;

          case rwBitmapColor:
				{
		  int w = GetBits8bpp(dx, pbSrc, x, yT, 32);
		  SetBits8bpp(dx, (rcbmp->pbBits+colorData), x, y, w, 16);
				}
               break;
	}
			}
	}
	rcbmp->cx = (int) dx;
	rcbmp->cy = (int) dy;
	rcbmp->cbRow = (int) cbRow;
}



/* Skip a newline */
int skip_newline(PILRC_BYTE *data, int size, int i)
{
	while (i < size && (data[i] != '\n' && data[i] != '\r'))
		{
		++i;
		}
	if (i + 1 < size && data[i] == '\r' && data[i + 1] == '\n')
		{
		i += 2;
		}
	else if (i < size)
		{
		++i;
		}
	return i;
}

/* Convert from .pbitm to Pilot resource data */
void ConvertTextBitmap(RCBITMAP *rcbmp, PILRC_BYTE *data, int size)
{
	int i, x, y;

	/* Count width and height of the image. */
	rcbmp->cx = 0;
	rcbmp->cy = 0;
	for (i = 0; i < size; i = skip_newline(data, size, i))
		{
		int j = i;
		while (j < size && (data[j] != '\n' && data[j] != '\r'))
			{
			++j;
			}
		if (rcbmp->cx < j - i)
			{
			rcbmp->cx = j - i;
			}
		++rcbmp->cy;
		}

	/* Allocate image buffer. */
	rcbmp->cbRow = ((rcbmp->cx + 15) & ~15) / 8;
	rcbmp->cbDst = rcbmp->cbRow * rcbmp->cy;
	rcbmp->pbBits = malloc(rcbmp->cbDst);
	memset(rcbmp->pbBits, 0, rcbmp->cbDst);

	/* Convert the image. */
	x = 0;
	y = 0;
	for (i = 0; i < size; )
		{
		if (data[i] == '\n' || data[i] == '\r')
			{
			x = 0;
			++y;
			i = skip_newline(data, size, i);
			}
		else
			{
			if (data[i] != ' ' && data[i] != '-')
				{
				rcbmp->pbBits[y * rcbmp->cbRow + (x >> 3)] |= (1 << (7 - (x & 7)));
				}
			++x;
			++i;
			}
		}
}

/* Convert from .xbm to Pilot resource data */
void ConvertX11Bitmap(RCBITMAP *rcbmp, PILRC_BYTE *data, int size)
{
	static PILRC_BYTE rev[] = {
		0,	/* 0000 */
		8,	/* 1000 */
		4,	/* 0100 */
		12,	/* 1100 */
		2,	/* 0010 */
		10,	/* 1010 */
		6,	/* 0110 */
		14,	/* 1110 */
		1,	/* 0001 */
		9,	/* 1001 */
		5,	/* 0101 */
		13,	/* 1101 */
		3,	/* 0011 */
		11,	/* 1011 */
		7,	/* 0111 */
		15	/* 1111 */
	};

	char name_and_type[80], *type;
	int i, value, pos;

	/* Read X11 bitmap header. */
	for (i = 0; i < size; i = skip_newline(data, size, i))
		{
		if (sscanf((const char *)data + i, "#define %79s %d", name_and_type, &value) == 2)
			{
			type = strrchr(name_and_type, '_');
			type = (type == NULL) ? name_and_type : type + 1;
			if (strcmp(type, "width") == 0)
				{
				rcbmp->cx = value;
				}
			else if (strcmp(type, "height") == 0)
				{
				rcbmp->cy = value;
				}
			}
		if (sscanf((const char *)data + i, "static unsigned char %s = {", name_and_type) == 1
			 || sscanf((const char *)data + i, "static char %s = {", name_and_type) == 1)
			{
			type = strrchr(name_and_type, '_');
			type = (type == NULL) ? name_and_type : type + 1;
			if (strcmp(type, "bits[]") == 0 && rcbmp->cx > 0 && rcbmp->cy > 0)
				{
				rcbmp->cbRow = ((rcbmp->cx + 15) & ~15) / 8;
				rcbmp->cbDst = rcbmp->cbRow * rcbmp->cy;
				rcbmp->pbBits = malloc(rcbmp->cbDst);
				break;
				}
			}
		}

	if (rcbmp->pbBits == NULL)
		{
		ErrorLine("Invalid X11 bitmap");
		}

	/* Read X11 bitmap data. */
	memset(rcbmp->pbBits, 0, rcbmp->cbDst);
	value = 0;
	pos = 0;
	for (i = skip_newline(data, size, i); i < size; ++i)
		{
		PILRC_BYTE c = data[i];
		if (c == ',' || c == '}')
			{
			if (pos < rcbmp->cbDst)
				{
				rcbmp->pbBits[pos++] = (unsigned char) value;
				if (rcbmp->cx % 16 > 0 && rcbmp->cx % 16 < 9
					 && pos % rcbmp->cbRow == rcbmp->cbRow - 1)
					{
					++pos;
					}
				}
			value = 0;
			}
		else
			{
			if (c >= '0' && c <= '9')
				{
				value = (value >> 4) + (rev[c - '0'] << 4);
				}
			else if (c >= 'A' && c <= 'F')
				{
				value = (value >> 4) + (rev[c - 'A' + 10] << 4);
				}
			else if (c >= 'a' && c <= 'f')
				{
				value = (value >> 4) + (rev[c - 'a' + 10] << 4);
				}
			}
		}
}


/*  convert from a PBM bitmap to Pilot resource data */
void ConvertPBMBitmap(RCBITMAP *prcbmp, PILRC_BYTE *pbData, int cb)
{
	char type;
	char c;
	int width, height;

	int x,y,r;
	PILRC_BYTE *pb;
	PILRC_BYTE *pbMac;

	pb = pbData;
	pbMac = pb+cb;
	if (*pb++ !='P')
		{ErrorLine("Not a PBM file.");}
	type=*pb++;
	if (type!='1' && type!='4')
		{ErrorLine("Not a monochrome bitmap.");}

	do
		{c=*pb++;} 
	while (isspace(c));  
	pb--;
	
	while (c=='#')
		{
		do
			{c=*pb++;} while (c!='\n');  
		c=*pb++; pb--;
		}

	/* fscanf(pFile, "%d %d", &width, &height); */
	sscanf((const char *)pb, "%d %d", &width, &height);
	do
		{c=*pb++;}
	while (c != '\n');
	pb--;

	do
		{c=*pb++;}
	while (isspace(c));
	pb--;

	while (c=='#')
		{
		do
			{c=*pb++;} while (c!='\n');  
		c=*pb++; pb--;
		}

	memset(prcbmp, 0, sizeof(RCBITMAP));

	prcbmp->cbRow = ((width + 15) & ~15) / 8;
	prcbmp->cbDst = prcbmp->cbRow * height;
	prcbmp->pbBits = malloc(prcbmp->cbDst);
	memset(prcbmp->pbBits, 0, prcbmp->cbDst);

	/*  read the bits */
	if (type=='1')
		{
		/* ASCII */
		for (y=0; y<height; y++)
			{
			for (x=0; x<width; x++)
				{
				do
					{c=*pb++;} 
				while (c!='0' && c!='1');
				if (c=='1')	 SetBit(width, prcbmp->pbBits, x, y, 16);
				}
			}
		}
	else
		{
		/* binary */
		for (y=0; y<height; y++)
			{
			for (r=0; r<width; r+=8)
				{
				c=*pb++;
				for (x=0; x+r<width && x<8; x++)
					{
					if (c&0x80)
						SetBit(width,  
								 prcbmp->pbBits, 
								 x+r, y, 16);
					c<<=1;
					}
				}
			}
		}
	prcbmp->cx = width;
	prcbmp->cy = height;
}


/* Compress bitmap data */
void CompressBitmap(RCBITMAP *rcbmp, int compress, BOOL colortable)
{
	unsigned char *bits;
	int size, i, j, k, flag;

	size = 2 + (colortable? 1026 : 0);
	bits = malloc(size + (rcbmp->cbRow + ((rcbmp->cbRow + 7) / 8)) * rcbmp->cy);
	for (i = 0; i < rcbmp->cy; ++i)
		{
		flag = 0;
		for (j = 0; j < rcbmp->cbRow; ++j)
			{
			if (i == 0 || rcbmp->pbBits[i * rcbmp->cbRow + j] !=
				 rcbmp->pbBits[(i - 1) * rcbmp->cbRow + j])
				{
				flag |= (0x80 >> (j & 7));
				}
			if ((j & 7) == 7 || j == rcbmp->cbRow - 1)
				{
				bits[size++] = (unsigned char) flag;
				for (k = (j & ~7); k <= j; ++k)
					{
					if (((flag <<= 1) & 0x100) != 0)
						{
						bits[size++] = rcbmp->pbBits[i * rcbmp->cbRow + k];
						}
					}
				flag = 0;
				}
			}
		}

	if (compress == rwForceCompress || size < rcbmp->cbDst)
		{

		if (colortable) {

			int i;
			for (i=0; i<1026; i++) 
				bits[i] = rcbmp->pbBits[i];
			bits[1026] = (unsigned char)(size >> 8);
			bits[1027] = (unsigned char)size;
		}
		else {
			bits[0] = (unsigned char)(size >> 8);
			bits[1] = (unsigned char)size;
		}
		free(rcbmp->pbBits);

		rcbmp->ff |= 0x8000;
		rcbmp->pbBits = bits;
		rcbmp->cbDst = size;
		}
	else
		{
		free(bits);
		}
}

/* Compress and dump Pilot resource data */
void CompressDumpBitmap(RCBITMAP *rcbmp, int isIcon, int compress, BOOL colortable, BOOL partOfFamily)
{
	if ((isIcon==1) && !(rcbmp->cx == 32 && rcbmp->cy == 32) && !(rcbmp->cx == 22 && rcbmp->cy == 22))
		{
		ErrorLine("Icon resource not 32x32 or 22x22 (preferred)");
		}

	if ((isIcon==2) && (rcbmp->cx != 15 || rcbmp->cy != 9))
		{
		ErrorLine("Small icon resource not 15x9");
		}

	if (compress == rwAutoCompress || compress == rwForceCompress)
		{
		CompressBitmap(rcbmp, compress, colortable);
		}

// is this part of a bitmap family?
if (partOfFamily) {

  // 16 bytes for "bitmap header"
  rcbmp->nextDepthOffset = (16 + rcbmp->cbDst) >> 2;
  rcbmp->cbDst           = (rcbmp->nextDepthOffset - 4) << 2;
  rcbmp->pbBits          = (PILRC_BYTE *)realloc(rcbmp->pbBits, rcbmp->cbDst); 
}

	CbEmitStruct(rcbmp, szRCBITMAP, NULL, fTrue);
	DumpBytes(rcbmp->pbBits, rcbmp->cbDst);
	free(rcbmp->pbBits);
}

void InvalidExtension(char *fileName)
{
	char pchLine[300];
	sprintf(pchLine, 
			  "Bitmap file extension not recognized for file %s\n"
			  "\tSupported extensions: .BMP (Windows bitmap)"
			  ", .pbitm (txt2bitm), .xbm (X11 bitmap)"
			  , fileName);
	ErrorLine(pchLine);
}

void DumpBitmap(char *fileName, int isIcon, int compress, int bitmaptype, BOOL colortable, BOOL partOfFamily)
{
	PILRC_BYTE *pBMPData;
	char *pchExt;
	FILE *pFile;
	long size;
	RCBITMAP rcbmp;

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

	memset(&rcbmp, 0, sizeof(RCBITMAP));
	if (FSzEqI(pchExt, "bmp"))
		{
		ConvertWindowsBitmap(&rcbmp, pBMPData, bitmaptype, colortable);
		CompressDumpBitmap(&rcbmp, isIcon, compress, colortable, partOfFamily);
		}
	else if (FSzEqI(pchExt, "pbitm"))
		{
		ConvertTextBitmap(&rcbmp, pBMPData, size);
		CompressDumpBitmap(&rcbmp, isIcon, compress, fFalse, partOfFamily);
		}
	else if (FSzEqI(pchExt, "xbm"))
		{
		ConvertX11Bitmap(&rcbmp, pBMPData, size);
		CompressDumpBitmap(&rcbmp, isIcon, compress, fFalse, partOfFamily);
		}
	else if (FSzEqI(pchExt, "pbm"))
		{
		ConvertPBMBitmap(&rcbmp, pBMPData, size);
		CompressDumpBitmap(&rcbmp, isIcon, compress, fFalse, partOfFamily);
		}
	else
		{
		InvalidExtension(fileName);
		}
	free(pBMPData);
}
