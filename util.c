
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
#elif defined(CW_PLUGIN)
#include "Extra.h"                               // <-- neil, where is this file? :P
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "pilrc.h"
#include "util.h"
#include "font.h"

#ifdef WIN32
#define DIRSEPARATOR  '\\'
#else
#define DIRSEPARATOR  '/'
#endif

/*
 * Globals 
 */
FILE *vfhIn;
char szInFile[256];
char rgbZero[16];
BOOL vfErr;
extern int iline;

/*
 * Globals for output file handling
 */

static FILE *vfhOut;	/* file receiving binary resource data */
static int ibOut;	/* current output offset (within .bin being emitted) */
static int ibTotalOut;	/* total output offset (after .bins so far emitted) */

static FILE *vfhRes = NULL;  /* file receiving resource file ("-R") output */

static char szOutFileDir[FILENAME_MAX];  /* directory for *.bin files */

static char szOutResDBFile[FILENAME_MAX];  /* filename for final .ro file */
static char szTempFile[FILENAME_MAX];      /* temporary filename */

static VOID WriteOutResourceDB();

DEFPL(PLEXRESOURCEDIR);
typedef struct
{
  int type[4];
  int id;
  int offset;
}
RESOURCEDIRENTRY;
#define szRESOURCEDIRENTRY "b4,w,l"

static PLEXRESOURCEDIR resdir;

/*
 * Includes 
 */
char *includePaths[MAXPATHS];
int totalIncludePaths = 0;

/*-----------------------------------------------------------------------------
|	Error
|	
|		Report error, exit
------------------------------------------------------------WESC------------*/
VOID
Error(char *sz)
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
#elif defined(CW_PLUGIN)
  if (1)
    CWError(sz);
  else
#endif
  {
    fprintf(stderr, "\n");
    fprintf(stderr, sz);
    fprintf(stderr, "\n");
    exit(1);
  }
}

/*-----------------------------------------------------------------------------
|	Error2
-------------------------------------------------------------WESC------------*/
VOID
Error2(char *sz1,
       char *sz2)
{
  char szT[256];

  strcpy(szT, sz1);
  strcat(szT, sz2);
  Error(szT);
}

/*-----------------------------------------------------------------------------
|	Error3
-------------------------------------------------------------WESC------------*/
VOID
Error3(char *sz1,
       char *sz2,
       char *sz3)
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
void
ErrorLine2(char *sz,
           char *sz2)
{
#ifdef CW_PLUGIN
  CWErrorLine2(sz, sz2);
#else
  char szErr[256];

  if (sz2 == NULL)
    sprintf(szErr,
            ((vfVSErrors)
             ? "%s(%d): error : %s"
             : "%s:%d: error : %s"), szInFile, iline, sz);
  else
    sprintf(szErr,
            ((vfVSErrors)
             ? "%s(%d): error : %s %s"
             : "%s:%d: error : %s %s"), szInFile, iline, sz, sz2);
  Error(szErr);
#endif
}

/*-----------------------------------------------------------------------------
|	ErrorLine
|	
|		Report error w/ current line #
-------------------------------------------------------------WESC------------*/
void
ErrorLine(char *sz)
{
  ErrorLine2(sz, NULL);
}

/*-----------------------------------------------------------------------------
|	WarningLine
|	
|		Report error w/ current line #
-------------------------------------------------------------WESC------------*/
void
WarningLine(char *sz)
{
#ifdef CW_PLUGIN
  CWWarningLine(sz);
#else
  char szErr[256];

  sprintf(szErr,
          ((vfVSErrors)
           ? "%s(%d): warning : %s"
           : "%s:%d: warning : %s"), szInFile, iline, sz);
  fprintf(stderr, "\n");
  fprintf(stderr, szErr);
  fprintf(stderr, "\n");
#endif
}

/*-----------------------------------------------------------------------------
|	EmitB
|	
|		Emit a byte to the output
-------------------------------------------------------------WESC------------*/
void
EmitB(unsigned char b)
{
  DumpBytes(&b, 1);
}

/*-----------------------------------------------------------------------------
|	EmitW
|	
|		Emit a word
-------------------------------------------------------------WESC------------*/
void
EmitW(unsigned short w)
{
  if (vfLE32)
  {
    /*
     * RMa little indian 
     */
    EmitB((unsigned char)(w & 0xff));
    EmitB((unsigned char)((w & 0xff00) >> 8));
  }
  else
  {
    /*
     * big indian 
     */
    EmitB((unsigned char)((w & 0xff00) >> 8));
    EmitB((unsigned char)(w & 0xff));
  }
}

/*-----------------------------------------------------------------------------
|	EmitL
|	
|		emit a long
-------------------------------------------------------------WESC------------*/
void
EmitL(unsigned long l)
{
  if (vfLE32)
  {
    /*
     * RMa little indian 
     */
    EmitB((unsigned char)(l & 0xff));
    EmitB((unsigned char)((l & 0xff00) >> 8));
    EmitB((unsigned char)((l & 0xff0000) >> 16));
    EmitB((unsigned char)((l & 0xff000000) >> 24));
  }
  else
  {
    /*
     * big indian 
     */
    EmitB((unsigned char)((l & 0xff000000) >> 24));
    EmitB((unsigned char)((l & 0xff0000) >> 16));
    EmitB((unsigned char)((l & 0xff00) >> 8));
    EmitB((unsigned char)(l & 0xff));
  }
}

/*-----------------------------------------------------------------------------
|	IbOut
|
|		Return current output file file offset
-------------------------------------------------------------WESC------------*/
int
IbOut()
{
  return ibOut;
}

/*-----------------------------------------------------------------------------
|	DumpBytes
|	
|		Emit bytes to current output file
-------------------------------------------------------------WESC------------*/
void
DumpBytes(void *pv,
          int cb)
{
#ifdef CW_PLUGIN
  CWDumpBytes(pv, cb);
#else
#ifdef HEXOUT                                    /* RMa activate Hex dump in debug */
  BYTE *pb;
  int ib;
  static int nbrBytesOut;
  static int ibLine;
  static BYTE rgbLine[16];

  if (!pv)
  {                                              /* RMa little hack to display clean Hex output */
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

  /*
   * #ifdef BINOUT 
   */
  fwrite(pv, cb, 1, vfhOut);
  ibOut += cb;

  /*
   * #endif 
   */
#ifdef HEXOUT
  pb = (BYTE *) pv;
  while (cb--)
  {
    if (ibLine == 0)
      printf("%08x  ", nbrBytesOut);
    rgbLine[ibLine++] = *pb;
    printf("%02x ", *pb);
    pb++;
    //              ibOut++;                                        /* RMa bug  correction */
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
#endif
}

/*-----------------------------------------------------------------------------
|	PadBoundary
|	
|		Pads output to a word or a long boundary by emitting a 0 if necessary
-------------------------------------------------------------WESC------------*/
void
PadBoundary()
{
  if (vfLE32)
  {
    if (ibOut & 3)
      DumpBytes(rgbZero, 4 - (ibOut & 3));
  }
  else if (ibOut & 1)
    DumpBytes(rgbZero, 1);
}

/*-----------------------------------------------------------------------------
|	PadWordBoundary
|	
|		Pads output to a word boundary by emitting a 0 if necessary
-------------------------------------------------------------WESC------------*/
void
PadWordBoundary()
{
  if (ibOut & 1)
    DumpBytes(rgbZero, 1);
}

/*-----------------------------------------------------------------------------
|	SetOutFileDir
|	
|		Set output file path -- no trailing / or \ 
-------------------------------------------------------------WESC------------*/
VOID
SetOutFileDir(char *sz)
{
  if (sz == NULL || strcmp(sz, ".") == 0)
    strcpy(szOutFileDir, "");
  else
    sprintf(szOutFileDir, "%s%c", sz, DIRSEPARATOR);
}

/*-----------------------------------------------------------------------------
|      OpenResDBFile
|
|              Set up to write a PRC formatted .ro file
-------------------------------------------------------------JOHN------------*/

static void
RemoveTempFile()
{
  if (*szTempFile)
  {
    remove(szTempFile);
    strcpy(szTempFile, "");
  }
}

VOID
OpenResDBFile(char *sz)
{
  static BOOL registered = fFalse;

  strcpy(szOutResDBFile, sz);

  strcpy(szTempFile, szOutResDBFile);
  szTempFile[strlen(szTempFile) - 1] = '~';

  remove(szTempFile);

  if (!registered)
  {
    atexit(RemoveTempFile);
    registered = fTrue;
  }

  PlexInit(&resdir, sizeof(RESOURCEDIRENTRY), 64, 64);
}

VOID
CloseResDBFile()
{
  if (!vfQuiet)
    printf("Collecting *.bin files into %s\n", szOutResDBFile);

  Assert(vfhOut == NULL);
  vfhOut = fopen(szOutResDBFile, "wb");
  if (vfhOut == NULL)
    Error3("Unable to open:", szOutResDBFile, strerror(errno));

  WriteOutResourceDB();
  fclose(vfhOut);
  RemoveTempFile();
  PlexFree(&resdir);
}

/*-----------------------------------------------------------------------------
|	OpenOutput
|	
|		Open output file of the form %outfiledir%\TTTTXXXX.bin ,
|	
|	where TTTT is the base 4 character mac/pilot res type and
|	XXXX is the resource id as a 4 digit hex number
-------------------------------------------------------------WESC------------*/
VOID
OpenOutput(char *szBase,
           int id)
{
#ifdef CW_PLUGIN
  CWOpenOutput(szBase, id);
#else
  char szPrettyName[FILENAME_MAX];
  char *szFileName;
  char *szMode;

  /*
   * #ifdef BINOUT 
   */
  if (vfWinGUI)
    return;

  Assert(vfhOut == NULL);

  if (vfPrc)
  {
    RESOURCEDIRENTRY entry;
    intstrncpy(entry.type, szBase, 4);
    entry.id = id;
    entry.offset = ibTotalOut;
    PlexAddElement(&resdir, &entry);

    sprintf(szPrettyName, "temporary %s%04x.bin", szBase, id);
    szFileName = szTempFile;
    szMode = "ab";
  }
  else
  {
    sprintf(szPrettyName, "%s%s%04x.bin", szOutFileDir, szBase, id);
    szFileName = szPrettyName;
    szMode = "wb";
  }

  vfhOut = fopen(szFileName, szMode);
  if (vfhOut == NULL)
    Error3("Unable to open:", szFileName, strerror(errno));

  if (!vfQuiet)
    printf("Writing %s\n", szPrettyName);
  ibOut = 0;

  /*
   * #endif 
   */

  if (vfhRes)
  {
    fprintf(vfhRes, "\tres '%s', %d, \"%s\"\n", szBase, id, szPrettyName);
  }
#endif
}

/*-----------------------------------------------------------------------------
|	CloseOutput
-------------------------------------------------------------WESC------------*/
VOID
CloseOutput()
{
#ifdef CW_PLUGIN
  CWCloseOutput();
#else
#ifdef HEXOUT                                    /* RMA call little hack to display clean Hex output */
  DumpBytes(NULL, 0);
#endif

  /*
   * #ifdef BINOUT 
   */
  if (vfWinGUI)
    return;

  if (!vfQuiet)
    printf("%d bytes\n", ibOut);
  ibTotalOut += ibOut;
  if (vfhOut != NULL)
  {
    fclose(vfhOut);
    vfhOut = NULL;
  }

  /*
   * #endif 
   */
#endif
}

/*-----------------------------------------------------------------------------
|	getOpenedOutputFile
-------------------------------------------------------------RMa-------------*/
FILE *
getOpenedOutputFile()
{
  if (vfhOut == NULL)
    Error("No output file opened");
  return vfhOut;
}

VOID
OpenResFile(char *sz)
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

VOID
CloseResFile()
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
char *
FindAndOpenFile(char *szIn,
                char *mode,
                FILE ** returnFile)
{
  FILE *file = fopen(szIn, mode);

  if (file == NULL)
  {
    static char szFullName[FILENAME_MAX];
    int i;

    for (i = 0; i < totalIncludePaths; i++)
    {
      sprintf(szFullName, "%s%c%s", includePaths[i], DIRSEPARATOR, szIn);

      file = fopen(szFullName, mode);
      if (file != NULL)
      {
        break;
      }
    }

    if (i == totalIncludePaths)
    {
      ErrorLine2("Unable to find ", szIn);
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

/*
 * case insenstitive string comparison  
 */
BOOL
FSzEqI(char *sz1,
       char *sz2)
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

int
WMin(int w1,
     int w2)
{
  return w1 < w2 ? w1 : w2;
}

int
WMax(int w1,
     int w2)
{
  return w1 > w2 ? w1 : w2;
}

/*-----------------------------------------------------------------------------
|      intstrncpy
-------------------------------------------------------------JOHN------------*/
VOID
intstrncpy(p_int *dst,
          const char *src,
          int n)
{
  while (n > 0)
  {
    n--;
    if ((*dst++ = *src++) == 0)
      break;
  }

  while (n > 0)
  {
    n--;
    *dst++ = 0;
  }
}

/*-----------------------------------------------------------------------------
|      WriteOutResourceDB
-------------------------------------------------------------JOHN------------*/

typedef struct
{
  p_int name[32];      /* b32 */
  p_int attr;          /* w */
  p_int version;       /* w */
  p_int created;       /* l */
  p_int modified;      /* l */
  //p_int backup;      /* zl */
  //p_int modnum;      /* zl */
  //p_int appinfo;     /* zl */
  //p_int sortinfo;    /* zl */
  p_int type[4];       /* b4 */
  p_int creator[4];    /* b4 */
  //p_int uidseed;     /* zl */
  //p_int nextlist;    /* zl */
  p_int nrecords;      /* w */
}
DBHEADER;
#define szDBHEADER "b32,w,w,l,l,zl4,b4,b4,zl2,w"

static VOID
WriteOutResourceDB()
{
  DBHEADER head;
  char buf[4096];
  int head_offset;
  FILE *f;
  int i;
  size_t n;
  BOOL saveLE32;

  /* Even resources with LE32 contents go into a M68K-style PRC file.  */
  saveLE32 = vfLE32;
  vfLE32 = fFalse;

  /* It was intended not to provide facilities to set the name, attributes,
     type, creator, timestamps, etc because that's not really PilRC's job.
     We're not generating fully flexible .prc files, we're really just using
     the PRC format as an archive format.  But in the end we've provided
     command line options for name, type, and creator.  Apparently it is
     too difficult to use this:  :-)
       pilrc -prc foo.rcp
       build-prc -n NAME -t TYPE -c CRID foo.ro

     The random number for the timestamps corresponds to 1996-05-16
     11:14:40, which is the same fixed number emitted by build-prc from
     prc-tools 0.5.0.  A little bit of history lives on.  :-)

     We output a constant time because it doesn't seem to be worth getting
     this right for a temporary file which won't be distributed, because
     it's non-trivial to output the correct time in a portable way, and
     especially because variable timestamps embedded in object files are
     the spawn of the devil: they make it harder to determine when anything
     has really changed -- cmp always detects differences after a rebuild.
     This is an issue in certain debugging scenarios that you never want
     to encounter.  */

  intstrncpy(head.name, (vfPrcName)? vfPrcName : szOutResDBFile, 32);
  head.attr = 1;  /* dmHdrAttrResDB */
  head.version = 1;
  head.created = head.modified = 0xadc0bea0;
  intstrncpy(head.type, (vfPrcType)? vfPrcType : "RESO", 4);
  intstrncpy(head.creator, (vfPrcCreator)? vfPrcCreator : "pRES", 4);
  head.nrecords = PlexGetCount(&resdir);

  head_offset = CbEmitStruct(&head, szDBHEADER, NULL, fTrue);
  head_offset += head.nrecords * CbStruct(szRESOURCEDIRENTRY);
  head_offset += 2;  /* Allow for that daft gap */

  for (i = 0; i < head.nrecords; i++)
  {
    RESOURCEDIRENTRY *e = PlexGetElementAt(&resdir, i);
    e->offset += head_offset;
    CbEmitStruct(e, szRESOURCEDIRENTRY, NULL, fTrue);
  }

  CbEmitStruct(NULL, "zb2", NULL, fTrue);  /* The dreaded gap */

  f = fopen(szTempFile, "rb");
  if (f == NULL)
    Error3("Unable to open:", szTempFile, strerror(errno));

  while ((n = fread(buf, 1, sizeof buf, f)) > 0)
    DumpBytes(buf, n);

  fclose(f);

  vfLE32 = saveLE32;
}
