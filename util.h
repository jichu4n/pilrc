#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>

VOID Error(char *sz);
VOID Error2(char *sz1, char *sz2);
VOID Error3(char *sz1, char *sz2, char *sz3);
void ErrorLine(char *sz);
void WarningLine(char *sz);
void ErrorLine2(char *sz, char *sz2);
BOOL FSzEqI(char *sz1, char *sz2);
int WMin(int w1, int w2);
int WMax(int w1, int w2);

void EmitB(unsigned char b);
void EmitW(unsigned short w);
void EmitL(unsigned long l);

/*void SwapBytes(void *pv, int cb); */
/*void SwapLong(void *pl); */
/*void SwapStruct(void *pv, char *szPic); */
VOID OpenOutput(char *szBase, int id);
VOID CloseOutput();
VOID OpenResFile(char *szFile);
VOID CloseResFile();
void DumpBytes(void *pv, int cb);
void PadWordBoundary();
int IbOut();
VOID SetOutFileDir(char *sz);
char *FindAndOpenFile(char *szIn, char *mode, FILE **returnFile );

extern char rgbZero[];
extern FILE *vfhIn;

#define MAXPATHS 20
extern char *includePaths[];
extern int   totalIncludePaths;




	


#endif /*__UTIL_H__*/
