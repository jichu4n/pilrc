/*
 * @(#)bitmap.c
 *
 * Copyright 1997-1999, Wes Cherry   (wesc@technosis.com)
 * Copyright      2000, Aaron Ardiri (ardiri@palmgear.com)
 *
 * This source file was generated as part of the PilRC application
 * that is  used by developers  of the Palm Computing Platform  to
 * translate  a resource script (.rcp)  into binary resource files 
 * required to generate Palm Computing Platform applications. 
 * 
 * The source code and  binaries are available free  of charge and 
 * are to be used on an "as-is" basis. The developers of PilRC are
 * to provide no warranty, either expressed or  implied due to the 
 * use of the software. 
 *
 * Revisions:
 * ----------
 *
 * 05-Mar-97  David Dawson     file created
 * 08-Mar-97  Wes Cherry       removed windows dependancies
 * 07-Feb-00  Aaron Ardiri     color bitmap resources integrated
 * 13-Feb-00  Aaron Ardiri     fixed RGB -> Index for MASK images
 *
 * PilRC is open-source,  that means that as a  developer can take 
 * part in the development of the software. Any modifications (bug
 * fixes or improvements) should be sent to the current maintainer
 * of PilRC.  This will ensure the entire Palm Computing  Platform
 * development  community can obtain the  latest and greatest from 
 * PilRC.
 *
 * Credits:
 * --------
 * 
 * Andrew Bulhak   PBM conversion
 * Hoshi Takanori  XBM, PBITM and compression
 * Aaron Ardiri    color bitmap resource integration
 */

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

typedef unsigned char  PILRC_BYTE;    /* b  */
typedef unsigned short PILRC_USHORT;  /* us */
typedef unsigned int   PILRC_ULONG;   /* ul */
typedef int            bool;	      /* f  */

//
// local function prototypes
//

static PILRC_ULONG  LLoadX86(PILRC_ULONG w);
static PILRC_USHORT WLoadX86(PILRC_USHORT w);

static int  BMP_RGBToColorIndex(int, int, int);
static int  BMP_GetBits1bpp(int, PILRC_BYTE *, int, int, int);
static void BMP_SetBits1bpp(int, PILRC_BYTE *, int, int, int);
static void BMP_SetBits2bpp(int, PILRC_BYTE *, int, int, int, int);
static int  BMP_GetBits4bpp(int, PILRC_BYTE *, int, int, int);
static void BMP_SetBits4bpp(int, PILRC_BYTE *, int, int, int, int);
static int  BMP_GetBits8bpp(int, PILRC_BYTE *, int, int, int);
static void BMP_SetBits8bpp(int, PILRC_BYTE *, int, int, int, int);
static void BMP_ConvertWindowsBitmap(RCBITMAP *, PILRC_BYTE *, int, BOOL);
static void BMP_ConvertTextBitmap(RCBITMAP *, PILRC_BYTE *, int);
static void BMP_ConvertX11Bitmap(RCBITMAP *, PILRC_BYTE *, int);
static void BMP_ConvertPBMBitmap(RCBITMAP *, PILRC_BYTE *, int);
static void BMP_CompressBitmap(RCBITMAP *, int, BOOL);
static void BMP_CompressDumpBitmap(RCBITMAP *, int, int, BOOL, BOOL);
static void BMP_InvalidExtension(char *);

//
// data structures
//

/* 
 * Microsoft Windows .BMP file format  
 */
typedef struct tagBITMAPFILEHEADER 
{ 
#ifdef __GNUC__
  PILRC_USHORT     bfType;       // gcc will only align the structure 
  PILRC_USHORT     bfSize1;      // correctly this way :P
  PILRC_USHORT     bfSize2;
  PILRC_USHORT     bfReserved1; 
  PILRC_USHORT     bfReserved2; 
  PILRC_USHORT     bfOffBits1;
  PILRC_USHORT     bfOffBits2;
#else
  PILRC_USHORT     bfType; 
  PILRC_ULONG      bfSize; 
  PILRC_USHORT     bfReserved1; 
  PILRC_USHORT     bfReserved2; 
  PILRC_ULONG      bfOffBits; 
#endif
} BITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER
{ 
  PILRC_ULONG      biSize; 
  long             biWidth; 
  long             biHeight; 
  PILRC_USHORT     biPlanes; 
  PILRC_USHORT     biBitCount;
  PILRC_ULONG      biCompression; 
  PILRC_ULONG      biSizeImage; 
  long             biXPelsPerMeter; 
  long             biYPelsPerMeter; 
  PILRC_ULONG      biClrUsed; 
  PILRC_ULONG      biClrImportant; 
} BITMAPINFOHEADER; 

typedef struct tagRGBQUAD 
{
  PILRC_BYTE       rgbBlue; 
  PILRC_BYTE       rgbGreen; 
  PILRC_BYTE       rgbRed; 
  PILRC_BYTE       rgbReserved; 
} RGBQUAD; 

typedef struct tagBITMAPINFO 
{
  BITMAPINFOHEADER bmiHeader; 
  RGBQUAD          bmiColors[1]; 
} BITMAPINFO; 

/*
 * The 8bit-256 color system palette for Palm Computing Devices.
 */
static int PalmPalette256[256][3] = 
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
#define COLOR_TABLE_SIZE 1026

//
// code
//

/**
 * Convert a RGB triplet to a index within the PalmPalette256[][] table.
 *
 * @param r the red RGB value
 * @param g the green RGB value
 * @param b the blue RGB value
 * @return the index of the RGB triplet in the PalmPalette256 table.
 */
static int 
BMP_RGBToColorIndex(int r, 
                    int g, 
                    int b)
{
  int index, lowValue, i, *diffArray;

  // masking value (black) should be (0,0,0) and index 255
  if ((r | g | b) == 0) index = 0xff;

  // a typical "closest" match will do fine
  else {

    // generate the color "differences" for all colors in the palette
    diffArray = (int *)malloc(256 * sizeof(int));
    for (i=0; i < 256; i++) {
      diffArray[i] = ((PalmPalette256[i][0]-r)*(PalmPalette256[i][0]-r)) +
                     ((PalmPalette256[i][1]-g)*(PalmPalette256[i][1]-g)) +
                     ((PalmPalette256[i][2]-b)*(PalmPalette256[i][2]-b));
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
  }

  return index;
}

/**
 * Convert an unsigned long from x86 to Palm Computing Platform format.
 *
 * @param ul the unsigned long to convert
 * @return the unsigned long in Palm Computing Platform format.
 */
static PILRC_ULONG 
LLoadX86(PILRC_ULONG ul)
{
  PILRC_BYTE *pb;
  PILRC_ULONG ulOut;

  pb    = (PILRC_BYTE *)&ul;
  ulOut = (PILRC_ULONG)((*(pb+3)<<24) | (*(pb+2)<<16) | (*(pb+1)<<8) | (*pb));

  return ulOut;
}

/**
 * Convert an unsigned short from x86 to Palm Computing Platform format.
 *
 * @param ul the unsigned short to convert
 * @return the unsigned short in Palm Computing Platform format.
 */
static PILRC_USHORT 
WLoadX86(PILRC_USHORT w)
{
  PILRC_BYTE   *pb;
  PILRC_USHORT wOut;

  pb   = (PILRC_BYTE *)&w;
  wOut = (PILRC_USHORT)((*(pb+1)<<8) | (*pb));

  return wOut;
}

/**
 * Get the number of bytes each bitmap row requires.
 *
 * @param cx         the width of the bitmap.
 * @param cbpp       the number of bits per byte.
 * @param cBitsAlign the os-dependant byte alignment definition.
 * @return the number of bytes each bitmap row requires.
 */
static int 
BMP_CbRow(int cx, 
          int cbpp, 
          int cBitsAlign)
{
  return ((cx*cbpp + (cBitsAlign-1)) & ~(cBitsAlign-1)) >> 3;
}

/**
 * Get a single bit from a 1bpp bitmap 
 *
 * @param cx         the width of the bitmap.
 * @param pb         a reference to the bitmap resource. 
 * @param x          the x-coordinate of the pixel to process.
 * @param y          the y-coordinate of the pixel to process.
 * @param cBitsAlign the os-dependant byte alignment definition.
 * @return zero if the bit not set, non-zero otherwise.
 */
static int 
BMP_GetBits1bpp(int        cx, 
                PILRC_BYTE *pb, 
                int        x, 
                int        y, 
                int        cBitsAlign)
{
  int cbRow;
	
  cbRow = BMP_CbRow(cx, 1, cBitsAlign);
  pb   += cbRow * y + (x >> 3);

  return (*pb & (0x01 << (7 - (x & 7))));
}

/**
 * Set a single bit in a 1bpp bitmap 
 *
 * @param cx         the width of the bitmap.
 * @param pb         a reference to the bitmap resource. 
 * @param x          the x-coordinate of the pixel to process.
 * @param y          the y-coordinate of the pixel to process.
 * @param cBitsAlign the os-dependant byte alignment definition.
 */
static void 
BMP_SetBits1bpp(int        cx, 
                PILRC_BYTE *pb, 
                int        x, 
                int        y, 
                int        cBitsAlign)
{
  int cbRow;

  cbRow = BMP_CbRow(cx, 1, cBitsAlign);
  pb   += cbRow * y + (x >> 3);
  *pb  |= (0x01 << (7 - (x & 7)));
}

/**
 * Set bits in a 2bpp bitmap 
 *
 * @param cx         the width of the bitmap.
 * @param pb         a reference to the bitmap resource. 
 * @param x          the x-coordinate of the pixel to process.
 * @param y          the y-coordinate of the pixel to process.
 * @param bits       the bits to set (0..3).
 * @param cBitsAlign the os-dependant byte alignment definition.
 */
static void 
BMP_SetBits2bpp(int        cx, 
                PILRC_BYTE *pb, 
                int        x, 
                int        y, 
                int        bits, 
                int        cBitsAlign)
{
  int cbRow;

  cbRow = BMP_CbRow(cx, 2, cBitsAlign);
  pb   += cbRow * y + (x >> 2);
  *pb  |= (bits << ((3 - (x & 3))*2));
}

/**
 * Get bits from a 4bpp bitmap 
 *
 * @param cx         the width of the bitmap.
 * @param pb         a reference to the bitmap resource. 
 * @param x          the x-coordinate of the pixel to process.
 * @param y          the y-coordinate of the pixel to process.
 * @param cBitsAlign the os-dependant byte alignment definition.
 * @return the bit representation for the (x,y) pixel.
 */
static int 
BMP_GetBits4bpp(int        cx, 
                PILRC_BYTE *pb, 
                int        x, 
                int        y, 
                int        cBitsAlign)
{
  int cbRow;

  cbRow = BMP_CbRow(cx, 4, cBitsAlign);
  pb   += cbRow * y + (x >> 1);

  return ((x & 1) != 0) ? (*pb & 0x0f) : ((*pb & 0xf0) >> 4);
}

/**
 * Set bits in a 4bpp bitmap 
 *
 * @param cx         the width of the bitmap.
 * @param pb         a reference to the bitmap resource. 
 * @param x          the x-coordinate of the pixel to process.
 * @param y          the y-coordinate of the pixel to process.
 * @param bits       the bits to set (0..3).
 * @param cBitsAlign the os-dependant byte alignment definition.
 */
static void 
BMP_SetBits4bpp(int        cx, 
                PILRC_BYTE *pb, 
                int        x, 
                int        y,  
                int        bits, 
                int        cBitsAlign)
{
  int cbRow;

   cbRow = BMP_CbRow(cx, 4, cBitsAlign);
   pb   += cbRow * y + (x >> 1);
   *pb  |= ((x & 1) != 0) ? bits : (bits << 4);
}

/**
 * Get bits from a 8bpp bitmap 
 *
 * @param cx         the width of the bitmap.
 * @param pb         a reference to the bitmap resource. 
 * @param x          the x-coordinate of the pixel to process.
 * @param y          the y-coordinate of the pixel to process.
 * @param cBitsAlign the os-dependant byte alignment definition.
 * @return the bit representation for the (x,y) pixel.
 */
static int 
BMP_GetBits8bpp(int        cx, 
                PILRC_BYTE *pb, 
                int        x, 
                int        y, 
                int        cBitsAlign)
{
  int cbRow;

  cbRow = BMP_CbRow(cx, 8, cBitsAlign);
  pb   += cbRow * y + x;

  return *pb;
}

/**
 * Set bits in a 8bpp bitmap 
 *
 * @param cx         the width of the bitmap.
 * @param pb         a reference to the bitmap resource. 
 * @param x          the x-coordinate of the pixel to process.
 * @param y          the y-coordinate of the pixel to process.
 * @param bits       the bits to set (0..3).
 * @param cBitsAlign the os-dependant byte alignment definition.
 */
static void 
BMP_SetBits8bpp(int        cx, 
                PILRC_BYTE *pb, 
                int        x, 
                int        y, 
                int        bits, 
                int        cBitsAlign)
{
  int cbRow;

  cbRow = BMP_CbRow(cx, 8, cBitsAlign);
  pb   += cbRow * y + x;
  *pb   = bits;
}

/**
 * Convert a Microsoft Windows BMP file to Palm Computing resource data.
 *
 * @param rcbmp      a reference to the Palm Computing resource data.
 * @param pbResData  a reference to the bitmap resource. 
 * @param bitmaptype the type of bitmap (B+W, Grey, Grey16 or Color)?
 * @param colortable does a color table need to be generated?
 */
static void 
BMP_ConvertWindowsBitmap(RCBITMAP   *rcbmp, 
                         PILRC_BYTE *pbResData, 
                         int        bitmaptype, 
                         BOOL       colortable)
{
  PILRC_BYTE       *pbSrc;
  int              x, y, dx, dy, colorDat;
  int              cbRow, cbHeader, cbits, cbitsPel;
  BITMAPINFO       *pbmi;
  BITMAPINFOHEADER bmi;
  BOOL             fNonGreyWarning;

  pbmi = (BITMAPINFO *)(pbResData + sizeof(BITMAPFILEHEADER)); 
  memcpy (&bmi, pbmi,sizeof(BITMAPINFOHEADER));

  cbHeader = LLoadX86(bmi.biSize);
  dx       = LLoadX86(bmi.biWidth);
  dy       = LLoadX86(bmi.biHeight);
  cbits    = WLoadX86(bmi.biBitCount);
  pbSrc    = ((PILRC_BYTE *)pbmi) + cbHeader + (sizeof(RGBQUAD) * (1 << cbits));
  cbitsPel = -1;
  colorDat = 0;

  // check the format of the bitmap
  switch (bitmaptype) 
  {
    case rwBitmap:
         if (cbits != 1) ErrorLine ("Bitmap not monochrome");
         cbitsPel = 1;
         break;

    case rwBitmapGrey:
         if (cbits != 4) ErrorLine ("Bitmap not 16 color");
         cbitsPel = 2;
         break;

    case rwBitmapGrey16:
         if (cbits != 4) ErrorLine ("Bitmap not 16 color");
         cbitsPel = 4;
         break;

    case rwBitmapColor:
         if (cbits != 8) ErrorLine ("Bitmap not 256 color");
         cbitsPel = 8;
         if (colortable) colorDat = COLOR_TABLE_SIZE;
         break;

    default:
         Assert(fFalse);
         break;
  }

  // allocate memory for image data (word aligned)
  cbRow         = ((dx*cbitsPel + 15) & ~15) >> 3;
  rcbmp->cbDst  = (cbRow * dy) + colorDat;
  rcbmp->pbBits = (unsigned char *)malloc(rcbmp->cbDst);
  memset(rcbmp->pbBits, 0, rcbmp->cbDst);

  // configure the bitmap header
  switch (bitmaptype) 
  {
    case rwBitmap:
    case rwBitmapGrey:
    case rwBitmapGrey16:
         rcbmp->pixelsize = cbitsPel;
         rcbmp->version   = 1;
         break;

    case rwBitmapColor:
         rcbmp->pixelsize = cbitsPel;
         rcbmp->version   = 2;

         // do we need to store a color table?
	 if (colortable) {

           PILRC_BYTE *tmpPtr;
           int        i;

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

         // ok, we should match the bitmap to the system color table
         else {

           int *paletteXref;
           int i;

           // generate the cross reference table
           paletteXref = (int *)malloc(256 * sizeof(int));
           for (i=0; i<256; i++) {
             paletteXref[i] = 
               BMP_RGBToColorIndex(pbmi->bmiColors[i].rgbRed,
                                   pbmi->bmiColors[i].rgbGreen,
                                   pbmi->bmiColors[i].rgbBlue);
           }

           // adjust the bitmap to reflect the closest matching
           for (y=0; y<dy; y++) {
             for (x=0; x<dx; x++) {

               int w;
               int yT = (dy > 0) ? dy - y - 1 : y;

               w = BMP_GetBits8bpp(dx, pbSrc, x, yT, 32);
               BMP_SetBits8bpp(dx, pbSrc, x, yT, paletteXref[w], 32);
             }
           }

           // clean up
           free(paletteXref);
         }
         break;

    default:
         break;
  }

  // convert from .bmp to binary format
  fNonGreyWarning = fFalse;
  for (y = 0; y < dy; y++) {
    for (x = 0; x < dx; x++) {

      int yT = (dy > 0) ? dy - y - 1 : y;

      // what type of bitmap are we dealing with?
      switch (bitmaptype) 
      {
        case rwBitmap:
             if (BMP_GetBits1bpp(dx, pbSrc, x, yT, 32) == 0) {
               BMP_SetBits1bpp(dx, rcbmp->pbBits, x, y, 16);
             }
             break;

        case rwBitmapGrey:
             {
               int w;
               int grey2bpp[] = { 3,1,1,1,1,1,1,1,2,1,1,1,1,1,1,0 };
				
               w = BMP_GetBits4bpp(dx, pbSrc, x, yT, 32);
               switch (w)
               {
                 case 0x00: case 0x07: case 0x08: case 0x0f:
                      break;

                 default:
                      if (!fNonGreyWarning) {
                        WarningLine("BitmapGrey uses 'non grey' colors");
                        fNonGreyWarning = fTrue;
                      }                            // color = light grey
                      break;
               }
               BMP_SetBits2bpp(dx, rcbmp->pbBits, x, y, grey2bpp[w], 16);
             }
             break;

        case rwBitmapGrey16:
             {
               int w;

               w = 15 - BMP_GetBits4bpp(dx, pbSrc, x, yT, 32);
               BMP_SetBits4bpp(dx, rcbmp->pbBits, x, y, w, 16);
             }
             break;

        case rwBitmapColor:
             {
               int w;

               w = BMP_GetBits8bpp(dx, pbSrc, x, yT, 32);
               BMP_SetBits8bpp(dx, (rcbmp->pbBits+colorDat), x, y, w, 16);
             }
             break;

        default:
             break;
      }
    }
  }

  // store width/height information in bitmap
  rcbmp->cx    = (int)dx;
  rcbmp->cy    = (int)dy;
  rcbmp->cbRow = (int)cbRow;
}

/* Skip a newline */
static int 
// skip_newline(PILRC_BYTE *data, int size, int i)
BMP_SkipNewLine(PILRC_BYTE *data, int size, int i)
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

/**
 * Convert a .pbitm file to Palm Computing resource data.
 *
 * @param rcbmp      a reference to the Palm Computing resource data.
 * @param data       a reference to the bitmap resource. 
 * @param size       the size the bitmap resource.
 */
static void 
BMP_ConvertTextBitmap(RCBITMAP   *rcbmp, 
                      PILRC_BYTE *data, 
                      int        size)
{
  int i, x, y;

  // determine the width and height of the image
  rcbmp->cx = 0;
  rcbmp->cy = 0;
  for (i=0; i<size; i=BMP_SkipNewLine(data, size, i)) {
    int j = i;
    while ((j < size) && (data[j] != '\n') && (data[j] != '\r')) j++;
    if (rcbmp->cx < (j-i)) rcbmp->cx = j-i;
    ++rcbmp->cy;
  }

  // allocate image buffer
  rcbmp->cbRow  = ((rcbmp->cx + 15) & ~15) / 8;
  rcbmp->cbDst  = rcbmp->cbRow * rcbmp->cy;
  rcbmp->pbBits = malloc(rcbmp->cbDst);
  memset(rcbmp->pbBits, 0, rcbmp->cbDst);

  // convert the image
  x = 0;
  y = 0;
  for (i=0; i < size; ) {
    if ((data[i] == '\n') || (data[i] == '\r')) {
      x = 0; y++;
      i = BMP_SkipNewLine(data, size, i);
    }
    else {
      if ((data[i] != ' ') && (data[i] != '-')) 
        rcbmp->pbBits[(y*rcbmp->cbRow) + (x >> 3)] |= (1 << (7 - (x & 7)));

      x++; i++;
    }
  }
}

/**
 * Convert a .xbm file to Palm Computing resource data.
 *
 * @param rcbmp      a reference to the Palm Computing resource data.
 * @param data       a reference to the bitmap resource. 
 * @param size       the size the bitmap resource.
 */
static void 
BMP_ConvertX11Bitmap(RCBITMAP   *rcbmp, 
                     PILRC_BYTE *data, 
                     int        size)
{
  static PILRC_BYTE rev[] = { 0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15 };

  char name_and_type[80], *type;
  int  i, value, pos;

  // read X11 bitmap header
  for (i=0; i<size; i=BMP_SkipNewLine(data, size, i)) {

    int result[2];

    // try and get the width and height
    result[0] = sscanf((const char *)data + i, 
                       "#define %79s %d", name_and_type, &value);
    if (result[0] == 2) {

      type = strrchr(name_and_type, '_');
      type = (type == NULL) ? name_and_type : type + 1;
      if (strcmp(type, "width") == 0)
        rcbmp->cx = value;
      else 
      if (strcmp(type, "height") == 0)
        rcbmp->cy = value;
    }

    // look for the data token (is there data)?
    result[0] = sscanf((const char *)data + i, 
                       "static unsigned char %s = {", name_and_type);
    result[1] = sscanf((const char *)data + i, 
                       "static char %s = {", name_and_type);
    if (result[0] || result[1]) {

      type = strrchr(name_and_type, '_');
      type = (type == NULL) ? name_and_type : type + 1;
      if (strcmp(type, "bits[]") == 0 && rcbmp->cx > 0 && rcbmp->cy > 0) {
        rcbmp->cbRow = ((rcbmp->cx + 15) & ~15) / 8;
        rcbmp->cbDst = rcbmp->cbRow * rcbmp->cy;
        rcbmp->pbBits = malloc(rcbmp->cbDst);

        i = size; // get out of the loop
      }
    }
  }
  
  if (rcbmp->pbBits == NULL) ErrorLine("Invalid X11 bitmap");

  // read the X11 bitmap data
   memset(rcbmp->pbBits, 0, rcbmp->cbDst);
   value = 0;
   pos   = 0;
   for (i=BMP_SkipNewLine(data, size, i); i<size; i++) {

     PILRC_BYTE c = tolower(data[i]);

     // termination of a line?
     if ((c == ',') || (c == '}')) {
       if (pos < rcbmp->cbDst) {
         rcbmp->pbBits[pos++] = (unsigned char) value;
	 if (((rcbmp->cx % 16) > 0) && ((rcbmp->cx % 16) < 9) && 
             ((pos % rcbmp->cbRow) == (rcbmp->cbRow - 1))) pos++;
       }
       value = 0;
     }
     else

     // numerical data? - process accordingly (hexagonal)
     if ((c >= '0') && (c <= '9'))
       value = (value >> 4) + (rev[c-'0'] << 4);
     else 
     if ((c >= 'a') && (c <= 'f'))
       value = (value >> 4) + (rev[c-'a'+10] << 4);
  }
}

/**
 * Convert a .pbm file to Palm Computing resource data.
 *
 * @param rcbmp      a reference to the Palm Computing resource data.
 * @param data       a reference to the bitmap resource. 
 * @param cb         the size the bitmap resource.
 */
static void 
BMP_ConvertPBMBitmap(RCBITMAP   *rcbmp, 
                     PILRC_BYTE *pbData, 
                     int        cb)
{
  PILRC_BYTE *pb;
  PILRC_BYTE *pbMac;
  char       c, type;
  int        x, y, r;
  int        width, height;

  // check the bitmap
  pb    = pbData;
  pbMac = pb+cb;
  if (*pb++ !='P') ErrorLine("Not a PBM file.");

  type=*pb++;
  if ((type != '1') && (type != '4')) ErrorLine("Not a monochrome bitmap.");

  // ignore any junk 
  do { c = *pb++; } while (isspace(c)); pb--;
  while (c=='#') { do {c=*pb++;} while (c!='\n'); c=*pb++; pb--; }

  // try and get the width and height of the bitmap
  sscanf((const char *)pb, "%d %d", &width, &height);

  // ignore any junk 
  do {c=*pb++;} while (c != '\n'); pb--;
  do {c=*pb++;} while (isspace(c)); pb--;
  while (c=='#') { do {c=*pb++;} while (c!='\n');  c=*pb++; pb--; }

  memset(rcbmp, 0, sizeof(RCBITMAP));
  rcbmp->cbRow  = ((width + 15) & ~15) / 8;
  rcbmp->cbDst  = rcbmp->cbRow * height;
  rcbmp->pbBits = malloc(rcbmp->cbDst);
  memset(rcbmp->pbBits, 0, rcbmp->cbDst);

  // read the bits
  switch (type) 
  {
    case '1':  // ASCII
         for (y=0; y<height; y++) {
           for (x=0; x<width; x++) {
             do { c=*pb++; } while ((c != '0') && (c != '1'));
             if (c == '1')
               BMP_SetBits1bpp(width, rcbmp->pbBits, x, y, 16);
           }
         }
         break;

    case '4':  // BINARY
         for (y=0; y<height; y++) {
           for (r=0; r<width; r+=8) {
             c = *pb++;
             for (x=0; ((x+r) < width) && (x < 8); x++) {
               if ((c & 0x80) != 0)
                 BMP_SetBits1bpp(width, rcbmp->pbBits, x+r, y, 16);
               c<<=1;
             }
           }
         }
         break;

    default:
         Assert(fFalse);
         break;
  }

  rcbmp->cx = width;
  rcbmp->cy = height;
}

/**
 * Compress a Bitmap (tBmp or tAIC) resource.
 * 
 * @param rcbmp      a reference to the Palm Computing resource data.
 * @param compress   compression style?
 * @param colortable does a color table need to be generated?
 */
static void 
BMP_CompressBitmap(RCBITMAP *rcbmp, 
                   int      compress, 
                   BOOL     colortable)
{
  unsigned char *bits;
  int           size, msize, i, j, k, flag;

  // determine how much memory is required for compression (hopefully less)
  size  = 2 + (colortable? COLOR_TABLE_SIZE : 0);
  msize = size + ((rcbmp->cbRow + ((rcbmp->cbRow + 7) / 8)) * rcbmp->cy);

  // allocat memory and clear
  bits = (unsigned char *)malloc(msize * sizeof(unsigned char));
  memset(bits, 0, msize * sizeof(unsigned char));

  // do the compression (at least, attempt it)
  for (i=0; i<rcbmp->cy; i++) {
    flag = 0;
    for (j=0; j<rcbmp->cbRow; j++ ) {
      if ((i == 0) || 
          (rcbmp->pbBits[(i * rcbmp->cbRow) + j] !=
	   rcbmp->pbBits[((i-1) * rcbmp->cbRow) + j])) {
        flag |= (0x80 >> (j & 7));
      }
      if (((j & 7) == 7) || (j == (rcbmp->cbRow-1))) {
        bits[size++] = (unsigned char) flag;
        for (k = (j & ~7); k <= j; ++k) {
          if (((flag <<= 1) & 0x100) != 0)
            bits[size++] = rcbmp->pbBits[i * rcbmp->cbRow + k];
        }
        flag = 0;
      }
    }
  }

  // if we must compress, or if it was worth it, save!
  if (compress == rwForceCompress || size < rcbmp->cbDst) {

    // do we have a color table?
    if (colortable) {

      int i;

      // copy the color table (dont forget it!)
      for (i=0; i<COLOR_TABLE_SIZE; i++) 
        bits[i] = rcbmp->pbBits[i];

      bits[COLOR_TABLE_SIZE]   = (unsigned char)(size >> 8);
      bits[COLOR_TABLE_SIZE+1] = (unsigned char)size;
    }
    else {
      bits[0] = (unsigned char)((size & 0xff00) >> 8);
      bits[1] = (unsigned char) (size & 0x00ff);
    }

    // change the data chunk to the newly compressed data
    free(rcbmp->pbBits);
    rcbmp->ff    |= 0x8000;
    rcbmp->pbBits = bits;
    rcbmp->cbDst  = size;
  }
  else 
    free(bits);
}

/**
 * Compress and Dump a single Bitmap (tBmp or tAIC) resource.
 * 
 * @param rcbmp      a reference to the Bitmap resource
 * @param isIcon     an icon? 0 = bitmap, 1 = normal, 2 = small
 * @param compress   compression style?
 * @param colortable does a color table need to be generated?
 * @param multibit   should this bitmap be prepared for multibit? 
 */
static void 
BMP_CompressDumpBitmap(RCBITMAP *rcbmp, 
                       int      isIcon, 
                       int      compress, 
                       BOOL     colortable, 
                       BOOL     multibit)
{
  // anything specific with icons here?
  switch (isIcon) 
  {
    case 1:
         if (((rcbmp->cx != 32) || (rcbmp->cy != 32)) && 
             ((rcbmp->cx != 32) || (rcbmp->cy != 22)) &&
             ((rcbmp->cx != 22) || (rcbmp->cy != 22))) {
           ErrorLine("Icon resource not 32x32, 32x22 or 22x22 (preferred)");
         }
         break;

    case 2:
         if ((rcbmp->cx != 15) && (rcbmp->cy != 9)) {
           ErrorLine("Small icon resource not 15x9");
         }
         break;

    default:
         break;
  }

  // do we need to do some compression?
  if ((compress == rwAutoCompress) || (compress == rwForceCompress)) {
    BMP_CompressBitmap(rcbmp, compress, colortable);
  }

  // is this single resource part of a multibit bitmap family?
  if (multibit) {

    // determine the next depth offset (# dwords)
    rcbmp->nextDepthOffset = 4 + (rcbmp->cbDst >> 2);  // 16 bytes - header

    // if we need to, round up to the nearest dword and get more memory
    if ((rcbmp->cbDst % 4) != 0) {

      int i, oldSize = rcbmp->cbDst;

      rcbmp->nextDepthOffset++;
      rcbmp->cbDst  = (rcbmp->nextDepthOffset - 4) << 2;
      rcbmp->pbBits = (PILRC_BYTE *)realloc(rcbmp->pbBits, rcbmp->cbDst); 

      // need to clear extra memory that was allocated?
      for (i=oldSize; i<rcbmp->cbDst; i++) rcbmp->pbBits[i] = 0x00; // clear it
    }
  }

  // dump the bitmap header and data
  CbEmitStruct(rcbmp, szRCBITMAP, NULL, fTrue);
  DumpBytes(rcbmp->pbBits, rcbmp->cbDst);

  // clean up
  free(rcbmp->pbBits);
}

/**
 * The fileName for the resource cannot be processed.
 *
 * @param fileName the source file name
 */
static void 
BMP_InvalidExtension(char *fileName)
{
  char pchLine[300];
  sprintf(pchLine, 
          "Bitmap file extension not recognized for file %s\n"
          "\tSupported extensions: .BMP, .pbitm, .xbm and .pbm",
          fileName);
  ErrorLine(pchLine);
}

/**
 * Dump a single Bitmap (tBmp or tAIC) resource.
 * 
 * @param fileName   the source file name  
 * @param isIcon     an icon? 0 = bitmap, 1 = normal, 2 = small
 * @param compress   compression style?
 * @param bitmaptype the type of bitmap (B+W, Grey, Grey16 or Color)?
 * @param colortable does a color table need to be generated?
 * @param multibit   should this bitmap be prepared for multibit? 
 */
extern void DumpBitmap(char *fileName, 
                       int  isIcon, 
                       int  compress, 
                       int  bitmaptype, 
                       BOOL colortable,
                       BOOL multibit)
{
  PILRC_BYTE *pBMPData;
  char       *pchExt;
  FILE       *pFile;
  long       size;
  RCBITMAP   rcbmp;

  // determine the size of the resource to load
  fileName = FindAndOpenFile(fileName, "rb", &pFile);
  fseek(pFile,0,SEEK_END);
  size = ftell(pFile);
  rewind(pFile);

  // alocate memory and load the resource in
  pBMPData = malloc(size);
  if (pBMPData == NULL) {
    char pchLine[200];
    sprintf(pchLine, "Resource %s too big to fit in memory", fileName);
    ErrorLine(pchLine);
  }
  fread(pBMPData, 1, size, pFile);
  fclose(pFile);

  // obtain the file extension
  pchExt = strrchr(fileName, '.');
  if ((strlen(fileName) < 5) || (pchExt == NULL)) {
    BMP_InvalidExtension(fileName);
  }
  pchExt++;

  // convert the resource into a binary resource
  memset(&rcbmp, 0, sizeof(RCBITMAP));
  if (FSzEqI(pchExt, "bmp")) {
    BMP_ConvertWindowsBitmap(&rcbmp, pBMPData, bitmaptype, colortable);
    BMP_CompressDumpBitmap(&rcbmp, isIcon, compress, colortable, multibit);
  }
  else 
  if (FSzEqI(pchExt, "pbitm")) {
    BMP_ConvertTextBitmap(&rcbmp, pBMPData, size);
    BMP_CompressDumpBitmap(&rcbmp, isIcon, compress, fFalse, multibit);
  }
  else 
  if (FSzEqI(pchExt, "xbm")) {
    BMP_ConvertX11Bitmap(&rcbmp, pBMPData, size);
    BMP_CompressDumpBitmap(&rcbmp, isIcon, compress, fFalse, multibit);
  }
  else 
  if (FSzEqI(pchExt, "pbm")) {
    BMP_ConvertPBMBitmap(&rcbmp, pBMPData, size);
    BMP_CompressDumpBitmap(&rcbmp, isIcon, compress, fFalse, multibit);
  }
  else {
    BMP_InvalidExtension(fileName);
  }

  // clean up
  free(pBMPData);
}
