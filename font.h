#ifndef __FONT_H__
#define __FONT_H__

// font types
#define fontDefault      0
#define fontHebrew       1
#define fontJapanese     2	
#define fontChineseBig5  3
#define fontChineseGB    4
#define fontKoreanHanme  5
#define fontKoreanHantip 6

extern int  (*pfnChkCode)(unsigned char *cp, int *pdx);

extern void DumpFont(char *, int);
extern void InitFontMem(int);
extern void FreeFontMem();

extern int DxCalcRgdx(unsigned char *, int , int *);
extern int DxCalcExtent(unsigned char *, int);
extern int DxChar(int, int);
extern int DyFont(int);
extern int GetFontType();

#endif /*__FONT_H__ */
