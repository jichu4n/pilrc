#ifndef __FONT_H__
#define __FONT_H__

void DumpFont(char *pchFileName, int fontid);
void InitFontMem(int fontType);
void FreeFontMem();

int DxCalcRgdx(unsigned char *sz, int ifnt, int *rgdx);
int DxCalcExtent(unsigned char *sz, int ifnt);
int DxChar(int ch, int ifnt);
int DyFont(int ifnt);
int GetFontType();


/* Font Types */
#define fontDefault 0
#define fontHebrew 1
#define fontJapanese 2	
#define fontChineseBig5 3
#define fontChineseGB 4
#define fontKoreanHanme 5
#define fontKoreanHantip 6

#endif /*__FONT_H__ */
