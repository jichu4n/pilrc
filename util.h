
/*
 * @(#)util.h
 *
 * Copyright 1997-1999, Wes Cherry   (mailto:wesc@technosis.com)
 *           2000-2003, Aaron Ardiri (mailto:aaron@ardiri.com)
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

#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include "std.h"

/*
 * RMa add : Debug mode define 
 */
#ifdef _DEBUG
#define		HEXOUT		1
#endif

VOID Error(char *sz);
VOID Error2(char *sz1,
            char *sz2);
VOID Error3(char *sz1,
            char *sz2,
            char *sz3);
VOID ErrorLine(char *sz);
VOID WarningLine(char *sz);
VOID ErrorLine2(char *sz,
                char *sz2);
BOOL FSzEqI(const char *sz1,
            const char *sz2);
int WMin(int w1,
         int w2);
int WMax(int w1,
         int w2);

VOID EmitB(BYTE b);
VOID EmitW(unsigned short w);
VOID EmitL(unsigned long l);

VOID intstrncpy(p_int * dst,
                const char *src,
                int n);

/*
 * void SwapBytes(void *pv, int cb); 
 */

/*
 * void SwapLong(void *pl); 
 */

/*
 * void SwapStruct(void *pv, char *szPic); 
 */
VOID OpenOutput(char *szBase,
                int id);
VOID CloseOutput();
FILE *getOpenedOutputFile();

VOID SetOutFileDir(char *sz);

VOID OpenResDBFile(char *szFile);
VOID CloseResDBFile();

VOID OpenResFile(char *szFile);
VOID CloseResFile();
VOID DumpBytes(void *pv,
               int cb);
VOID PadBoundary();
VOID PadWordBoundary();
int IbOut();
void AddAccessPath(const char *path);
void FreeAccessPathsList(void);
char *FindAndOpenFile(char *szIn,
                      char *mode,
                      FILE ** returnFile);

void SetDependsTarget(const char *szIn);
void InitDependsList(void);
void AddEntryToDependsList(const char *filename);
void OutputDependsList(FILE *dependsFile);
void FreeDependsList(void);

extern char rgbZero[];
extern FILE *vfhIn;

#ifdef CW_PLUGIN
// XXX ncr
#undef feof
#define feof(f)    MyFeof(f)
#endif

#endif
