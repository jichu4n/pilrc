
/*
 * @(#)font.c
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

// #include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pilrc.h"
#include "util.h"

/*
 * Header definitions and token types 
 */

#define h_fontType 0                             /* font type */
#define h_firstChar 1                            /* ASCII code of first character */
#define h_lastChar 2                             /* ASCII code of last character */
#define h_maxWidth 3                             /* maximum character width */
#define h_kernMax 4                              /* negative of maximum character kern */
#define h_nDescent 5                             /* negative of descent */
#define h_fRectWidth 6                           /* width of font rectangle */
#define h_fRectHeight 7                          /* height of font rectangle */
#define h_owTLoc 8                               /* offset to offset/width table */
#define h_ascent 9                               /* ascent */
#define h_descent 10                             /* descent */
#define h_leading 11                             /* leading */
#define h_rowWords 12                            /* row width of bit image in words (16bits) */

#define g_glyph 20                               /* new glyph token */
#define g_offset 21                              /* offset token */
#define g_width 22                               /* width token */
#define g_bitmap 99                              /* bitmap data */

/*
 * some foreign font handing function 
 */
int (*pfnChkCode) (unsigned char *cp,
                   int *pdx);

/*
 * Some globals to keep track of things for error reporting 
 */

static char *filename;
static unsigned int lineno;
int vfontType;

/*
 * Global to hold the font widths and offsets 
 */

typedef struct
{
  char offset;
  char width;
}
FontCharInfoType;
FontCharInfoType *fntOW[256];
unsigned int fntH[256];

int
IsDBCSNone(unsigned char *cp,
           int *pdxChar)
{
  return 0;
}

/*-----------------------------------------------------------------------------
|	IsBIG5
|	
|		Check the double byte char is BIG5 coded
-------------------------------------------------------------DLIN------------*/
int
IsBIG5(unsigned char *cp,
       int *pdxChar)
{

  /*
   * BIG-5 code rule
   * first  byte range 0x81..0xfe             (high byte)
   * second byte range 0x40..0x7e, 0xa1..0xfe (low  byte)
   * 
   * default symbols range 0xa140..0xa3bf
   * often used word range 0xa440..0xc67e
   * seldom                0xc940..0xf9d5
   * 
   * full typed 0..9       0xa2af..0xa2b8
   * A..Z       0xa2cf..0xa2e8
   * a..z       0xa2e9..0xa343
   * sound symbol b..u     0xa374..0xa3ba
   * sound level .1234     0xa3bb..0xa3bf
   * 
   * full typed space      0xa140
   * cama       0xa141
   * 
   */
  if ((*cp >= 0x81 && *cp <= 0xfe)
      && ((*(cp + 1) >= 0x40 && *(cp + 1) <= 0x7e)
          || (*(cp + 1) >= 0xa1 && *(cp + 1) <= 0xfe)))
  {
    *pdxChar = 13;
    return 2;
  }
  return 0;

}

/*-----------------------------------------------------------------------------
|	IsJapanese
|	
|		Check the double byte char is Japanese coded
-------------------------------------------------------------DLIN------------*/
int
IsJapanese(unsigned char *cp,
           int *pdxChar)
{

  if ((*cp >= 0xa1 && *cp <= 0xdf))
  {
    *pdxChar = 5;
    return 1;
  }
  else
    if ((((*cp >= 0x81) && (*cp <= 0x9f)) ||
         ((*cp >= 0xe0) && (*cp <= 0xef))) &&
        (((*(cp + 1) >= 0x40) && (*(cp + 1) <= 0x7e)) ||
         ((*(cp + 1) >= 0x80) && (*(cp + 1) <= 0xfc))))
  {
    *pdxChar = 9;                                /* not sure about this */
    return 2;
  }
  return 0;

}

/*-----------------------------------------------------------------------------
| IsKoreanHanme
|	
|	Check the double byte char is Korean coded(Large Font, for HanMe)
-----------------------------------------------------------------------------*/
int
IsKoreanHanme(unsigned char *cp,
              int *pdxChar)
{

  /*
   * Korean code rule (by JaeMok Jeong)
   * first  byte range 0xb0..0xc8             (high byte)
   * second byte range 0xa1..0xfe             (low  byte)
   */
  if ((*cp >= 0xb0 && *cp <= 0xc8)
      && (*(cp + 1) >= 0xa1 && *(cp + 1) <= 0xfe))
  {
    *pdxChar = 11;                               /* not sure about this, hanme font width */
    return 2;
  }

  return 0;
}

/*-----------------------------------------------------------------------------
| IsKoreanHantip
|	
|	Check the double byte char is Korean coded(Small Font, for Hantiip)
-----------------------------------------------------------------------------*/
int
IsKoreanHantip(unsigned char *cp,
               int *pdxChar)
{

  /*
   * Korean code rule (by JaeMok Jeong)
   * first  byte range 0xb0..0xc8             (high byte)
   * second byte range 0xa1..0xfe             (low  byte)
   */
  if ((*cp >= 0xb0 && *cp <= 0xc8)
      && (*(cp + 1) >= 0xa1 && *(cp + 1) <= 0xfe))
  {
    *pdxChar = 8;                                /* hantip font width = 8 */
    return 2;
  }
  return 0;
}

/*
 * Report an error with line number and filename 
 */

static void
ErrorX(char *s)
{
  char er[512];

  sprintf(er, "ERROR: %s, line %u, %s", s, lineno, filename);
  Error(er);
}

/*
 * Report a warning with line number and filename 
 */

static void
WarningX(char *s)
{
  fprintf(stderr, "WARNING: %s, line %u, %s", s, lineno, filename);
}

/*
 * Parse out a line and return it's token and value 
 */

static void
ParseLine(char *s,
          int *token,
          int *value)
{
  size_t i, len = strlen(s);
  char *s1;

  *token = g_bitmap;
  *value = 0;

  /*
   * first check for normal "token value" line 
   */
  for (i = 0; i < len; i++)
  {
    s[i] = toupper(s[i]);
    if (s[i] == ' ' || s[i] == 9)
    {
      s[i] = 0;
      if (!strcmp(s, "FONTTYPE"))
        *token = h_fontType;
      if (!strcmp(s, "MAXWIDTH"))
        *token = h_maxWidth;
      if (!strcmp(s, "KERNMAX"))
        *token = h_kernMax;
      if (!strcmp(s, "NDESCENT"))
        *token = h_nDescent;
      if (!strcmp(s, "FRECTWIDTH"))
        *token = h_fRectWidth;
      if (!strcmp(s, "FRECTHEIGHT"))
        *token = h_fRectHeight;
      if (!strcmp(s, "ASCENT"))
        *token = h_ascent;
      if (!strcmp(s, "DESCENT"))
        *token = h_descent;
      if (!strcmp(s, "LEADING"))
        *token = h_leading;
      if (!strcmp(s, "GLYPH"))
        *token = g_glyph;
      if (!strcmp(s, "OFFSET"))
        *token = g_offset;
      if (!strcmp(s, "WIDTH"))
        *token = g_width;

      if (*token == g_bitmap)
        ErrorX("Invalid Token");
      break;

    }
  }

  /*
   * determe the value 
   */
  if (*token != g_bitmap)
  {
    i++;
    if ((len - i) == 3 && s[i] == 39 && s[i + 2] == 39)
      *value = s[i + 1];
    else
    {
      *value = strtol(s + i, &s1, 10);
      if (s1[0] != 0)
        ErrorX("Invalid Value");
    }
  }

}

/*
 * Ugly routine to call after every glyph 
 */

static void
CloseGlyph(int *header,
           int *width,
           int *row,
           int *col,
           int autoWidth,
           int autoRectWidth)
{
  if (*width > header[h_maxWidth])
  {
    if (autoWidth)
      header[h_maxWidth] = *width;
    else
      WarningX("Width exceeds maxWidth definition");
  }

  if (*width > header[h_fRectWidth])
  {
    if (autoRectWidth)
      header[h_fRectWidth] = *width;
    else
      WarningX("Width exceeds fRectWidth definition");
  }

  if (!header[h_fRectHeight])
    header[h_fRectHeight] = *row;
  if (!(*row) || header[h_fRectHeight] != *row)
    ErrorX("Invalid height on previous glyph");

  *row = 0;
  *col += *width;
}

/*
 * The main function of this module 
 */

void
DumpFont(char *pchFileName,
         int fntNo)
{
#define		kArraySize		257      /* RMa add 1 for handle missing char (256 +1) */
  FILE *in;
  char s[kArraySize], *s1, *bitmap[kArraySize];
  unsigned short int coltable[kArraySize];
  size_t x;
  int token, value, header[13];
  int curChar = -1;
  int autoWidth = 1;
  int autoRectWidth = 1;
  int row = 0;
  int col = 0;
  int width = 0;
  unsigned short bit[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

  memset(header, 0, sizeof(header));
  memset(bitmap, 0, sizeof(bitmap));

  filename = FindAndOpenFile(pchFileName, "rt", &in);
  lineno = 0;
  if (fntOW[fntNo])
    free(fntOW[fntNo]);
  if (!(fntOW[fntNo] = malloc(kArraySize * sizeof(FontCharInfoType))))
    Error("Out of memory");
  memset(fntOW[fntNo], -1, kArraySize * sizeof(FontCharInfoType));

  while (fgets(s, sizeof(s), in))
  {
    lineno++;

    /*
     * Remove leading and trailing whitespace 
     */
    for (s1 = s; s1[0] == ' ' || s1[0] == 9; s1++) ;
    for (x = strlen(s1); x; x--)
    {
      if (s1[x - 1] == ' ' || s1[x - 1] == 9 || s1[x - 1] == 10
          || s1[x - 1] == 13)
        s1[x - 1] = 0;
      else
        break;
    }

    if (s1[0] && (s[0] != 47 || s[1] != 47))
    {                                            /* skip blank lines and comment lines */
      ParseLine(s1, &token, &value);
      if (token < g_glyph)
      {
        if (curChar >= 0)
          ErrorX("Header must precede glyphs");
        header[token] = value;
        if (token == h_maxWidth)
          autoWidth = 0;
        if (token == h_fRectWidth)
          autoRectWidth = 0;
      }

      /*
       * Do new char processing 
       */
      if (token == g_glyph)
      {
        if (curChar >= 0)
          CloseGlyph(header, &width, &row, &col, autoWidth, autoRectWidth);

        if (value == -1)
        {
          /*
           * Handle missing char 
           */
          curChar++;
          fntOW[fntNo][curChar].offset = 0;
          coltable[curChar] = col;
          continue;
        }
        if (value < 0 || value > 255)
          ErrorX("Glyph number out of range");
        if (value <= curChar)
          ErrorX("Glyph out of sequence");

        for (x = curChar + 1; x <= (size_t) value; x++)
          coltable[x] = col;
        if (curChar < 0)
          header[h_firstChar] = value;
        curChar = value;
        header[h_lastChar] = value;
        fntOW[fntNo][curChar].offset = 0;
        continue;
      }

      if (token == g_offset)
      {
        if (curChar < 0)
          ErrorX("Unexpected OFFSET token");
        fntOW[fntNo][curChar].offset = value;
        continue;
      }

      if (token == g_width)
      {
        if (curChar < 0)
          ErrorX("Unexpected WIDTH token");
        fntOW[fntNo][curChar].width = value;
        continue;
      }

      if (token == g_bitmap)
      {
        if (row == 0)
        {
          width = strlen(s1);
          if (fntOW[fntNo][curChar].width == -1)
            fntOW[fntNo][curChar].width = width;
        }
        else if (width != (int)strlen(s1))
          ErrorX("Invalid width");

        if (!bitmap[row])
        {
          if (!(bitmap[row] = malloc(1024)))
            Error("Out of memory");
          memset(bitmap[row], 0, 1024);

        }

        for (x = 0; x < (size_t) width; x++)
        {
          if (s1[x] != '-' && s1[x] != '.')
          {
            bitmap[row][(col + x) >> 3] |= bit[(col + x) & 0x7];
          }
        }
        if (++row > 255)
          ErrorX("Too many rows");
        continue;
      }

    }

  }
  if (!feof(in))
    Error2("Error reading file: ", filename);

  fclose(in);

  /*
   * Write the data 
   */

  if (curChar < 0)
    ErrorX("No glyphs");
  CloseGlyph(header, &width, &row, &col, autoWidth, autoRectWidth);
  fntH[fntNo] = header[h_fRectHeight];

  header[h_rowWords] = (col + 15) / 16;
  header[h_owTLoc] =
    header[h_rowWords] * header[h_fRectHeight] + 8 + header[h_lastChar] -
    header[h_firstChar];

  CbEmitStruct(header, "wwwwwwwwwwwww", NULL, fTrue);

  for (x = 0; x < (size_t) header[h_fRectHeight]; x++)
  {
    DumpBytes(bitmap[x], header[h_rowWords] * 2);
    free(bitmap[x]);
  }

  for (x = header[h_firstChar]; x <= (size_t) header[h_lastChar] + 1; x++)
    EmitW(coltable[x]);
  EmitW((unsigned short)col);
  //      EmitW(0);                       /* RMa Remove this bug value */

  DumpBytes(&fntOW[fntNo][header[h_firstChar]],
            (header[h_lastChar] - header[h_firstChar] + 2) * 2);
  EmitW(0xFFFF);                                 /* RMa bug correction last value in file must be 0xFFFF not 0 */

}

static void
SetBuiltinFont(int fntNo,
               unsigned char *rgbWidths,
               int ichStart,
               int cch)
{
  int ich;

  Assert(ichStart + cch <= 256);
  if (fntOW[fntNo] == NULL)
  {
    if (!(fntOW[fntNo] = malloc(256 * sizeof(FontCharInfoType))))
      Error("Out of memory");
    memset(fntOW[fntNo], -1, 256 * sizeof(FontCharInfoType));
  }
  for (ich = ichStart; ich < ichStart + cch; ich++)
  {
    fntOW[fntNo][ich].width = rgbWidths[ich - ichStart];
  }
}

/*
 * Call to set up memory with standard font data 
 */

void
InitFontMem(int fontType)
{

  static unsigned char font0[] = {
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 2, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 11, 5, 8, 8,
    6, 5, 5, 5, 5, 5, 5, 5,
    2, 2, 4, 8, 6, 8, 7, 2,
    4, 4, 6, 6, 3, 4, 2, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 2, 3, 6, 6, 6, 5,
    8, 5, 5, 5, 6, 4, 4, 6,
    6, 2, 4, 6, 5, 8, 6, 7,
    5, 7, 5, 5, 6, 6, 6, 8,
    6, 6, 6, 3, 5, 3, 6, 4,
    3, 5, 5, 4, 5, 5, 4, 5,
    5, 2, 3, 5, 2, 8, 5, 5,
    5, 5, 4, 4, 4, 5, 5, 6,
    6, 6, 4, 4, 2, 4, 7, 5,
    5, 5, 3, 8, 5, 6, 6, 6,
    4, 11, 5, 4, 8, 10, 10, 10,
    10, 3, 3, 5, 5, 4, 4, 7,
    7, 10, 4, 4, 8, 5, 5, 6,
    2, 2, 6, 6, 8, 6, 2, 5,
    4, 8, 5, 6, 6, 4, 8, 6,
    5, 6, 4, 4, 3, 5, 6, 2,
    4, 2, 5, 6, 8, 8, 8, 5,
    5, 5, 5, 5, 5, 5, 7, 5,
    4, 4, 4, 4, 3, 2, 3, 3,
    7, 6, 7, 7, 7, 7, 7, 5,
    8, 6, 6, 6, 6, 6, 6, 6,
    5, 5, 5, 5, 5, 5, 8, 4,
    5, 5, 5, 5, 2, 2, 3, 3,
    5, 5, 5, 5, 5, 5, 5, 6,
    7, 5, 5, 5, 5, 6, 5, 6
  };

  static unsigned char font1[] = {
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 2, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 11, 5, 10, 8,
    6, 6, 5, 5, 5, 5, 5, 5,
    2, 3, 6, 10, 6, 13, 9, 3,
    5, 5, 6, 6, 3, 5, 3, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 3, 3, 6, 6, 6, 6,
    10, 7, 7, 6, 7, 5, 5, 8,
    8, 3, 5, 7, 6, 10, 7, 8,
    7, 8, 8, 6, 7, 7, 8, 11,
    7, 7, 7, 4, 6, 4, 6, 5,
    4, 6, 6, 5, 6, 6, 5, 6,
    6, 3, 4, 6, 3, 9, 6, 6,
    6, 6, 5, 5, 6, 6, 6, 9,
    6, 6, 5, 5, 3, 5, 7, 5,
    6, 5, 3, 9, 5, 6, 5, 5,
    4, 17, 6, 5, 10, 10, 10, 10,
    10, 3, 3, 5, 5, 4, 4, 6,
    7, 10, 5, 5, 10, 5, 5, 7,
    2, 3, 6, 7, 8, 7, 3, 6,
    4, 8, 6, 8, 6, 5, 8, 6,
    5, 6, 4, 4, 4, 6, 7, 2,
    4, 2, 6, 8, 9, 9, 9, 6,
    7, 7, 7, 7, 7, 7, 9, 6,
    5, 5, 5, 5, 3, 3, 3, 3,
    8, 7, 8, 8, 8, 8, 8, 6,
    8, 7, 7, 7, 7, 7, 7, 8,
    6, 6, 6, 6, 6, 6, 9, 5,
    6, 6, 6, 6, 3, 3, 3, 3,
    6, 6, 6, 6, 6, 6, 6, 7,
    8, 6, 6, 6, 6, 6, 6, 6
  };

  static unsigned char font2[] = {

    5, 5, 5, 5, 5, 5, 5, 5,
    5, 4, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 11, 5, 10, 8,
    10, 7, 5, 5, 5, 5, 5, 5,
    4, 2, 4, 9, 6, 11, 8, 2,
    4, 4, 6, 8, 3, 4, 2, 5,
    7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 2, 3, 7, 7, 7, 5,
    11, 9, 6, 7, 7, 6, 6, 8,
    7, 3, 4, 7, 5, 10, 7, 8,
    6, 8, 6, 5, 6, 7, 7, 11,
    7, 6, 5, 3, 5, 3, 6, 6,
    3, 6, 7, 6, 7, 7, 4, 7,
    6, 3, 3, 6, 2, 10, 7, 7,
    7, 7, 4, 5, 4, 7, 6, 10,
    6, 7, 6, 4, 2, 4, 7, 5,
    7, 5, 3, 7, 5, 10, 6, 6,
    4, 13, 5, 4, 9, 10, 10, 10,
    10, 3, 3, 5, 5, 5, 6, 12,
    7, 11, 5, 4, 12, 5, 5, 8,
    4, 2, 6, 6, 8, 8, 2, 6,
    4, 10, 5, 7, 7, 4, 10, 4,
    5, 6, 5, 4, 3, 6, 6, 2,
    4, 3, 5, 7, 8, 8, 8, 5,
    9, 9, 9, 9, 9, 9, 9, 7,
    6, 6, 6, 6, 3, 2, 3, 3,
    8, 7, 8, 8, 8, 8, 8, 6,
    8, 7, 7, 7, 7, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 11, 6,
    7, 7, 7, 7, 3, 2, 3, 3,
    6, 7, 7, 7, 7, 7, 7, 6,
    7, 7, 7, 7, 7, 7, 6, 7
  };

  static unsigned char font3[] = {

    5, 5, 5, 11, 10, 10, 10, 11,
    11, 7, 4, 6, 6, 8, 8, 8,
    8, 8, 8, 7, 7, 7, 6, 10,
    10, 10, 10, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5
  };

  static unsigned char font4[] = {

    12, 12, 8, 8, 12, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5
  };

  static unsigned char font5[] = {
    5, 11, 11, 11, 11, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5
  };

  static unsigned char font6[] = {
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 4, 6, 4, 7,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9
  };

  static unsigned char font7[] = {
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 2, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 11, 5, 8, 8,
    9, 6, 5, 5, 5, 5, 5, 5,
    2, 3, 6, 8, 7, 11, 9, 3,
    4, 4, 8, 7, 3, 5, 3, 5,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 3, 4, 7, 6, 7, 6,
    10, 9, 7, 7, 7, 6, 6, 7,
    7, 3, 7, 7, 6, 9, 8, 7,
    7, 8, 7, 7, 7, 7, 8, 10,
    8, 7, 6, 4, 5, 4, 8, 8,
    4, 6, 6, 6, 6, 6, 5, 6,
    6, 3, 4, 6, 3, 9, 6, 6,
    6, 6, 5, 6, 5, 6, 7, 9,
    6, 6, 5, 7, 3, 7, 8, 5,
    6, 5, 3, 7, 6, 9, 6, 6,
    6, 12, 7, 4, 9, 10, 10, 10,
    10, 3, 3, 6, 6, 8, 7, 9,
    7, 8, 6, 4, 9, 5, 5, 7,
    1, 3, 6, 7, 8, 7, 2, 5,
    6, 8, 4, 7, 7, 3, 8, 8,
    5, 7, 4, 4, 4, 7, 8, 3,
    4, 3, 4, 7, 8, 8, 8, 6,
    9, 9, 9, 9, 9, 9, 11, 7,
    6, 6, 6, 6, 4, 4, 5, 4,
    8, 8, 7, 7, 7, 7, 7, 6,
    7, 7, 7, 7, 7, 7, 7, 7,
    6, 6, 6, 6, 6, 6, 9, 6,
    6, 6, 6, 6, 4, 4, 5, 4,
    6, 6, 6, 6, 6, 6, 6, 7,
    6, 6, 6, 6, 6, 6, 6, 6
  };

  /*
   * starts at 0xe0 
   */
  static unsigned char font0Hebrew[] = {
    6, 6, 5, 6, 6, 3, 4, 6,
    6, 3, 6, 5, 6, 6, 6, 3,
    4, 6, 6, 6, 6, 6, 6, 6,
    6, 7, 7, 2, 2, 2, 2, 2
  };

  static unsigned char font1Hebrew[] = {
    7, 7, 6, 7, 7, 3, 5, 7,
    8, 3, 6, 6, 6, 7, 8, 3,
    5, 7, 8, 7, 7, 6, 6, 7,
    6, 9, 8, 2, 2, 2, 2, 2
  };

  static unsigned char font2Hebrew[] = {
    7, 7, 6, 7, 7, 4, 6, 7,
    7, 4, 7, 7, 7, 7, 7, 4,
    4, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 2, 2, 2, 2, 2
  };

  /*
   * BUG! font7Hebrew? 
   */

  memset(fntOW, 0, sizeof(fntOW));
  memset(fntH, 0, sizeof(fntH));

  vfontType = fontType;
  pfnChkCode = IsDBCSNone;

  SetBuiltinFont(0, font0, 0, sizeof(font0));
  SetBuiltinFont(1, font1, 0, sizeof(font1));
  SetBuiltinFont(2, font2, 0, sizeof(font2));
  SetBuiltinFont(3, font3, 0, sizeof(font3));
  SetBuiltinFont(4, font4, 0, sizeof(font4));
  SetBuiltinFont(5, font5, 0, sizeof(font5));
  SetBuiltinFont(6, font6, 0, sizeof(font6));
  SetBuiltinFont(7, font7, 0, sizeof(font7));

  fntH[0] = 11;
  fntH[1] = 11;
  fntH[2] = 14;
  fntH[3] = 10;
  fntH[4] = 11;
  fntH[5] = 8;
  fntH[6] = 19;
  fntH[7] = 15;

  switch (fontType)
  {
    case fontChineseBig5:
      pfnChkCode = IsBIG5;
      break;
    case fontChineseGB:
      Error("GB character widths not yet implemented!");
      break;
    case fontJapanese:
      pfnChkCode = IsJapanese;
      break;
    case fontKoreanHanme:
      pfnChkCode = IsKoreanHanme;
      break;
    case fontKoreanHantip:
      pfnChkCode = IsKoreanHantip;
      break;
    case fontHebrew:
      SetBuiltinFont(0, font0Hebrew, 0xe0, sizeof(font0Hebrew));
      SetBuiltinFont(1, font1Hebrew, 0xe0, sizeof(font1Hebrew));
      SetBuiltinFont(2, font2Hebrew, 0xe0, sizeof(font2Hebrew));
      break;
  }

}

/*
 * Call to free the memory 
 */

void
FreeFontMem()
{
  int i;

  for (i = 0; i < 256; i++)
    if (fntOW[i])
      free(fntOW[i]);

}

/*-----------------------------------------------------------------------------
|	DxCalcRgdx
|	
|		Get extent of pilot string, placing each char width in rgdx
-------------------------------------------------------------WESC------------*/
int
DxCalcRgdx(unsigned char *sz,
           int ifnt,
           int *rgdx)
{
  unsigned char *pch;
  int dx;

  if (sz == NULL)
    return 0;
  dx = 0;

  for (pch = sz; *pch != 0; pch++)
  {
    if (rgdx != NULL)
      rgdx[pch - sz] = DxChar(*pch, ifnt);
    dx += DxChar(*pch, ifnt);
  }
  return dx;
}

/*-----------------------------------------------------------------------------
|	DxCalcExtent
|	
|		Calc extent of string -- BUG! only works for font 0 (and bold)
-------------------------------------------------------------WESC------------*/
int
DxCalcExtent(unsigned char *sz,
             int ifnt)
{
  unsigned char *pch;
  int dx;

  if (sz == NULL)
    return 0;
  dx = 0;

  for (pch = sz; *pch != 0; pch++)
  {
    int dxChar;
    int cch;

    cch = (*pfnChkCode) (pch, &dxChar);
    if (cch > 0)
    {                                            /* Daniel Lin 99/04/07 */
      dx += dxChar;
      pch += cch - 1;                            /* double byte chars */
    }
    else
      dx += DxChar(*pch, ifnt);
  }
  return dx;
}

int
DxChar(int ch,
       int ifnt)
{
  int dx;

  if (ifnt < 0 || ifnt >= 255)
    ErrorLine("Invalid F	ontID");
  if (!fntOW[ifnt])
    ErrorLine("Font not defined");
  dx = fntOW[ifnt][ch].width;
  if (dx == -1)
    dx = 0;
  return dx;
}

int
DyFont(int ifnt)
{
  if (ifnt < 0 || ifnt >= 255)
    ErrorLine("Invalid FontID");
  return fntH[ifnt];
}

int
GetFontType()
{
  return vfontType;
}
