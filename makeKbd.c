
/*
 * @(#)makeKbd.c
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
 *     23-Feb-2001 renaud malaval
 *                 creation based on makeKbd.c
 */

#include 	<stdio.h>
#include 	<string.h>

#include		"std.h"
#include		"chars.h"
#define NON_INTERNATIONAL
#include		"CharLatin.h"

#include		"pilrc.h"
#include		"makeKbd.h"
#include		"util.h"
#include		"restype.h"

char *keyTopLabels[numKeyTopLabels] = {
  "cap",
  "shift",
  "space"
};

#define	kNumAlphaKeys_US			38
#define	kNumAlphaKeys_France		43
#define	kNumAlphaKeys_Germany	42

#define	kNumAccentKeys_US			38
#define	kNumAccentKeys_Other		39

int
keyboardKeyEmit(KeyboardKeyType * pBuffer,
                int index,
                BOOL fEmit)
{
  int keyboardResourceSize = 0;

  while (index--)
  {
    keyboardResourceSize +=
      CbEmitStruct(pBuffer, szRCKEYBOARDKEY, NULL, fEmit);
    pBuffer++;
  }
  return keyboardResourceSize;
}

void
createAndSaveCountryKeyboard(int country,
                             int resID)
{
  static unsigned char kbdResImage[4096];
  unsigned char *pRunning;
  KeyboardLayoutType *pkeyboardLayout;
  KeyboardKeyType *pkeyboardKey;
  unsigned char *pFirstKeyTopLabel, *pThisKeyTopLabel;
  int thisKeyTopLabelSize;
  int i;

  //      int                                                     keyboardResourceSize = 0;
  int numAlphaKeys;
  int numAccentKeys;
  int stringsSize;
  int realResOffset;

  KeyboardKeyType *pAlphaKeys;
  KeyboardLayoutType *pAlphaKeyboardLayout;
  KeyboardKeyType *pPunc2Keys;
  KeyboardLayoutType *pPunc2KeyboardLayout;
  KeyboardKeyType *pPunc1Keys;
  KeyboardLayoutType *pPunc1KeyboardLayout;
  KeyboardKeyType *pNumberKeys;
  KeyboardLayoutType *pNumberKeyboardLayout;
  KeyboardKeyType *pAccentKeys;
  KeyboardLayoutType *pAccentKeyboardLayout;

  KeyboardKeyType alphaKeys_US[kNumAlphaKeys_US] = {
    // First row
    {'Q', 'Q', 'q', stdAlphaKeyWidth, 0},
    {'W', 'W', 'w', stdAlphaKeyWidth, 0},
    {'E', 'E', 'e', stdAlphaKeyWidth, 0},
    {'R', 'R', 'r', stdAlphaKeyWidth, 0},
    {'T', 'T', 't', stdAlphaKeyWidth, 0},
    {'Y', 'Y', 'y', stdAlphaKeyWidth, 0},
    {'U', 'U', 'u', stdAlphaKeyWidth, 0},
    {'I', 'I', 'i', stdAlphaKeyWidth, 0},
    {'O', 'O', 'o', stdAlphaKeyWidth, 0},
    {'P', 'P', 'p', stdAlphaKeyWidth, 0},
    {kbdBackspaceKey, kbdBackspaceKey, kbdBackspaceKey,
     2 * stdAlphaKeyWidth + 1, kbdBackspaceKey},
    // Second row
    {kbdTabKey, kbdTabKey, kbdTabKey, stdAlphaKeyWidth + 3, kbdTabKey},
    {'A', 'A', 'a', stdAlphaKeyWidth, 0},
    {'S', 'S', 's', stdAlphaKeyWidth, 0},
    {'D', 'D', 'd', stdAlphaKeyWidth, 0},
    {'F', 'F', 'f', stdAlphaKeyWidth, 0},
    {'G', 'G', 'g', stdAlphaKeyWidth, 0},
    {'H', 'H', 'h', stdAlphaKeyWidth, 0},
    {'J', 'J', 'j', stdAlphaKeyWidth, 0},
    {'K', 'K', 'k', stdAlphaKeyWidth, 0},
    {'L', 'L', 'l', stdAlphaKeyWidth, 0},
    {':', ';', ';', stdAlphaKeyWidth - 1, 0},
    {'"', '\'', '\'', stdAlphaKeyWidth - 1, 0},
    // Third row
    {kbdCapsKey, kbdCapsKey, kbdCapsKey, stdAlphaKeyWidth * 3 / 2 + 2,
     keyTopCapLabel},
    {'Z', 'Z', 'z', stdAlphaKeyWidth, 0},
    {'X', 'X', 'x', stdAlphaKeyWidth, 0},
    {'C', 'C', 'c', stdAlphaKeyWidth, 0},
    {'V', 'V', 'v', stdAlphaKeyWidth, 0},
    {'B', 'B', 'b', stdAlphaKeyWidth, 0},
    {'N', 'N', 'n', stdAlphaKeyWidth, 0},
    {'M', 'M', 'm', stdAlphaKeyWidth, 0},
    {'!', ',', ',', stdAlphaKeyWidth, 0},
    {'*', '.', '.', stdAlphaKeyWidth, 0},
    {kbdReturnKey, kbdReturnKey, kbdReturnKey, stdAlphaKeyWidth + 6, kbdReturnKey},     // carriage return
    // Fourth row
    {kbdShiftKey, kbdShiftKey, kbdShiftKey, stdAlphaKeyWidth * 2 + 4,
     keyTopShiftLabel},
    {' ', ' ', ' ', 7 * stdAlphaKeyWidth + 10, keyTopSpaceLabel},
    {'_', '-', '-', stdAlphaKeyWidth, 0},
    {'?', '/', '/', stdAlphaKeyWidth, 0},
  };
  KeyboardLayoutType alphaKeyboardLayout_US = {
    {{-1, 65}, {158, keyboardRows * 14 + 1}},
    14, keyboardFont, 0,
    {11, 12, 11, 4},
    34, 23, fTrue
  };

  KeyboardKeyType alphaKeys_France[kNumAlphaKeys_France] = {
    // First row
    {'A', 'A', 'a', stdAlphaKeyWidth, 0},
    {'Z', 'Z', 'z', stdAlphaKeyWidth, 0},
    {'E', 'E', 'e', stdAlphaKeyWidth, 0},
    {'R', 'R', 'r', stdAlphaKeyWidth, 0},
    {'T', 'T', 't', stdAlphaKeyWidth, 0},
    {'Y', 'Y', 'y', stdAlphaKeyWidth, 0},
    {'U', 'U', 'u', stdAlphaKeyWidth, 0},
    {'I', 'I', 'i', stdAlphaKeyWidth, 0},
    {'O', 'O', 'o', stdAlphaKeyWidth, 0},
    {'P', 'P', 'p', stdAlphaKeyWidth, 0},
    {upUGraveChr, upUGraveChr, lowUGraveChr, stdAlphaKeyWidth, 0},
    {kbdBackspaceKey, kbdBackspaceKey, kbdBackspaceKey,
     1 * stdAlphaKeyWidth + 1, kbdBackspaceKey},
    // Second row
    {kbdTabKey, kbdTabKey, kbdTabKey, stdAlphaKeyWidth + 3, kbdTabKey},
    {'Q', 'Q', 'q', stdAlphaKeyWidth, 0},
    {'S', 'S', 's', stdAlphaKeyWidth, 0},
    {'D', 'D', 'd', stdAlphaKeyWidth, 0},
    {'F', 'F', 'f', stdAlphaKeyWidth, 0},
    {'G', 'G', 'g', stdAlphaKeyWidth, 0},
    {'H', 'H', 'h', stdAlphaKeyWidth, 0},
    {'J', 'J', 'j', stdAlphaKeyWidth, 0},
    {'K', 'K', 'k', stdAlphaKeyWidth, 0},
    {'L', 'L', 'l', stdAlphaKeyWidth, 0},
    {'M', 'M', 'm', stdAlphaKeyWidth - 1, 0},
    {'*', '-', '-', stdAlphaKeyWidth - 1, 0},
    // Third row
    {kbdCapsKey, kbdCapsKey, kbdCapsKey, stdAlphaKeyWidth * 3 / 2 + 2,
     kbdCapsKey},
    {'W', 'W', 'w', stdAlphaKeyWidth, 0},
    {'X', 'X', 'x', stdAlphaKeyWidth, 0},
    {'C', 'C', 'c', stdAlphaKeyWidth, 0},
    {'V', 'V', 'v', stdAlphaKeyWidth, 0},
    {'B', 'B', 'b', stdAlphaKeyWidth, 0},
    {'N', 'N', 'n', stdAlphaKeyWidth, 0},
    {'?', ',', ',', stdAlphaKeyWidth, 0},
    {'.', ';', ';', stdAlphaKeyWidth, 0},
    {'/', ':', ':', stdAlphaKeyWidth, 0},
    {kbdReturnKey, kbdReturnKey, kbdReturnKey, stdAlphaKeyWidth + 6, kbdReturnKey},     // carriage return
    // Fourth row
    {kbdShiftKey, kbdShiftKey, kbdShiftKey, stdAlphaKeyWidth * 2 + 4,
     kbdShiftKey},
    {'_', '"', '"', stdAlphaKeyWidth, 0},
    {'!', '\'', '\'', stdAlphaKeyWidth, 0},
    {' ', ' ', ' ', 3 * stdAlphaKeyWidth + 10, 0},
    {'^', lowEAcuteChr, lowEAcuteChr, stdAlphaKeyWidth, 0},
    {sectionChr, lowEGraveChr, lowEGraveChr, stdAlphaKeyWidth, 0},
    {upCCedillaChr, upCCedillaChr, lowCCedillaChr, stdAlphaKeyWidth, 0},
    {microChr, lowAGraveChr, lowAGraveChr, stdAlphaKeyWidth, 0},
  };
  KeyboardLayoutType alphaKeyboardLayout_France = {
    {{-1, 65}, {158, keyboardRows * 14 + 1}},
    14, keyboardFont, 0,
    {12, 12, 11, 8},
    35, 24, fTrue
  };

  KeyboardKeyType alphaKeys_Germany[kNumAlphaKeys_Germany] = {
    // First row
    {'Q', 'Q', 'q', stdAlphaKeyWidth, 0},
    {'W', 'W', 'w', stdAlphaKeyWidth, 0},
    {'E', 'E', 'e', stdAlphaKeyWidth, 0},
    {'R', 'R', 'r', stdAlphaKeyWidth, 0},
    {'T', 'T', 't', stdAlphaKeyWidth, 0},
    {'Z', 'Z', 'z', stdAlphaKeyWidth, 0},
    {'U', 'U', 'u', stdAlphaKeyWidth, 0},
    {'I', 'I', 'i', stdAlphaKeyWidth, 0},
    {'O', 'O', 'o', stdAlphaKeyWidth, 0},
    {'P', 'P', 'p', stdAlphaKeyWidth, 0},
    {upUDiaeresisChr, upUDiaeresisChr, lowUDiaeresisChr, stdAlphaKeyWidth,
     0},
    {kbdBackspaceKey, kbdBackspaceKey, kbdBackspaceKey,
     1 * stdAlphaKeyWidth + 1, kbdBackspaceKey},
    // Second row
    {kbdTabKey, kbdTabKey, kbdTabKey, stdAlphaKeyWidth + 3, kbdTabKey},
    {'A', 'A', 'a', stdAlphaKeyWidth, 0},
    {'S', 'S', 's', stdAlphaKeyWidth, 0},
    {'D', 'D', 'd', stdAlphaKeyWidth, 0},
    {'F', 'F', 'f', stdAlphaKeyWidth, 0},
    {'G', 'G', 'g', stdAlphaKeyWidth, 0},
    {'H', 'H', 'h', stdAlphaKeyWidth, 0},
    {'J', 'J', 'j', stdAlphaKeyWidth, 0},
    {'K', 'K', 'k', stdAlphaKeyWidth, 0},
    {'L', 'L', 'l', stdAlphaKeyWidth, 0},
    {upODiaeresisChr, upODiaeresisChr, lowODiaeresisChr,
     stdAlphaKeyWidth - 1, 0},
    {upADiaeresisChr, upADiaeresisChr, lowADiaeresisChr,
     stdAlphaKeyWidth - 1, 0},
    // Third row
    {kbdCapsKey, kbdCapsKey, kbdCapsKey, stdAlphaKeyWidth * 3 / 2 + 2,
     kbdCapsKey},
    {'Y', 'Y', 'y', stdAlphaKeyWidth, 0},
    {'X', 'X', 'x', stdAlphaKeyWidth, 0},
    {'C', 'C', 'c', stdAlphaKeyWidth, 0},
    {'V', 'V', 'v', stdAlphaKeyWidth, 0},
    {'B', 'B', 'b', stdAlphaKeyWidth, 0},
    {'N', 'N', 'n', stdAlphaKeyWidth, 0},
    {'M', 'M', 'm', stdAlphaKeyWidth, 0},
    {';', ',', ',', stdAlphaKeyWidth, 0},
    {':', '.', '.', stdAlphaKeyWidth, 0},
    {kbdReturnKey, kbdReturnKey, kbdReturnKey, stdAlphaKeyWidth + 6, kbdReturnKey},     // carriage return
    // Fourth row
    {kbdShiftKey, kbdShiftKey, kbdShiftKey, stdAlphaKeyWidth * 2 + 4,
     kbdShiftKey},
    {'_', '-', '-', stdAlphaKeyWidth, 0},
    {'!', '"', '"', stdAlphaKeyWidth, 0},
    {' ', ' ', ' ', 4 * stdAlphaKeyWidth + 10, 0},
    {'?', lowSharpSChr, lowSharpSChr, stdAlphaKeyWidth, 0},
    {'*', '+', '+', stdAlphaKeyWidth, 0},
    {'\'', '#', '#', stdAlphaKeyWidth, 0},
  };
  KeyboardLayoutType alphaKeyboardLayout_Germany = {
    {{-1, 65}, {158, keyboardRows * 14 + 1}},
    14, keyboardFont, 0,
    {12, 12, 11, 7},
    35, 24, fTrue
  };

  // Number keyboard
  // List the punctuation keyboards first
#define numPunc2Keys 12
  KeyboardKeyType punc2Keys_US[numPunc2Keys] = {
    // First row
    {'-', '-', '-', stdPuncKeyWidth, 0},
    {'+', '+', '+', stdPuncKeyWidth, 0},
    {kbdBackspaceKey, kbdBackspaceKey, kbdBackspaceKey,
     2 * stdPuncKeyWidth, kbdBackspaceKey},
    // Second row
    {'/', '/', '/', stdPuncKeyWidth, 0},
    {'*', '*', '*', stdPuncKeyWidth, 0},
    {':', ':', ':', stdPuncKeyWidth, 0},
    {kbdTabKey, kbdTabKey, kbdTabKey, stdPuncKeyWidth, kbdTabKey},
    // Third row
    {'.', '.', '.', stdPuncKeyWidth, 0},
    {',', ',', ',', stdPuncKeyWidth, 0},
    {kbdReturnKey, kbdReturnKey, kbdReturnKey, 2 * stdPuncKeyWidth, kbdReturnKey},      // carriage return
    // Fourth row
    {' ', ' ', ' ', 3 * stdPuncKeyWidth, keyTopSpaceLabel},
    {'%', '%', '%', stdPuncKeyWidth, 0},
  };
  KeyboardKeyType punc2Keys_Other[numPunc2Keys] = {
    // First row
    {'-', '-', '-', stdPuncKeyWidth, 0},
    {'+', '+', '+', stdPuncKeyWidth, 0},
    {kbdBackspaceKey, kbdBackspaceKey, kbdBackspaceKey,
     2 * stdPuncKeyWidth, kbdBackspaceKey},
    // Second row
    {'/', '/', '/', stdPuncKeyWidth, 0},
    {'*', '*', '*', stdPuncKeyWidth, 0},
    {':', ':', ':', stdPuncKeyWidth, 0},
    {kbdTabKey, kbdTabKey, kbdTabKey, stdPuncKeyWidth, kbdTabKey},
    // Third row
    {'.', '.', '.', stdPuncKeyWidth, 0},
    {',', ',', ',', stdPuncKeyWidth, 0},
    {kbdReturnKey, kbdReturnKey, kbdReturnKey, 2 * stdPuncKeyWidth, kbdReturnKey},      // carriage return
    // Fourth row
    {' ', ' ', ' ', 3 * stdPuncKeyWidth, 0},
    {'%', '%', '%', stdPuncKeyWidth, 0},
  };
  KeyboardLayoutType punc2KeyboardLayout_All = {
    {{109, 69}, {stdPuncKeyWidth * 4 + 1, keyboardRows * 13 + 1}},
    13, keyboardFont, 0,
    {3, 4, 3, 2},
    kbdNoKey, kbdNoKey, fTrue
  };

#define numPunc1Keys 16
  KeyboardKeyType punc1Keys_Germany[numPunc1Keys] = {
    // First row
    {'$', '$', '$', stdPuncKeyWidth, 0},
    {chrEuroSign, chrEuroSign, chrEuroSign, stdPuncKeyWidth, 0},
    {poundChr, poundChr, poundChr, stdPuncKeyWidth, 0},
    {yenChr, yenChr, yenChr, stdPuncKeyWidth, 0},
    // Second row
    {'[', '[', '[', stdPuncKeyWidth, 0},
    {']', ']', ']', stdPuncKeyWidth, 0},
    {'{', '{', '{', stdPuncKeyWidth, 0},
    {'}', '}', '}', stdPuncKeyWidth, 0},
    // Third row
    {'<', '<', '<', stdPuncKeyWidth, 0},
    {'>', '>', '>', stdPuncKeyWidth, 0},
    {'\\', '\\', '\\', stdPuncKeyWidth, 0},
    {microChr, microChr, microChr, stdPuncKeyWidth, 0},
    // Fourth row
    {'@', '@', '@', stdPuncKeyWidth, 0},
    {'~', '~', '~', stdPuncKeyWidth, 0},
    {'&', '&', '&', stdPuncKeyWidth, 0},
    {sectionChr, sectionChr, sectionChr, stdPuncKeyWidth, 0},
  };
  KeyboardKeyType punc1Keys_Other[numPunc1Keys] = {
    // First row
    {'$', '$', '$', stdPuncKeyWidth, 0},
    {chrEuroSign, chrEuroSign, chrEuroSign, stdPuncKeyWidth, 0},
    {poundChr, poundChr, poundChr, stdPuncKeyWidth, 0},
    {yenChr, yenChr, yenChr, stdPuncKeyWidth, 0},
    // Second row
    {'[', '[', '[', stdPuncKeyWidth, 0},
    {']', ']', ']', stdPuncKeyWidth, 0},
    {'{', '{', '{', stdPuncKeyWidth, 0},
    {'}', '}', '}', stdPuncKeyWidth, 0},
    // Third row
    {'<', '<', '<', stdPuncKeyWidth, 0},
    {'>', '>', '>', stdPuncKeyWidth, 0},
    {'\\', '\\', '\\', stdPuncKeyWidth, 0},
    {'=', '=', '=', stdPuncKeyWidth, 0},
    // Fourth row
    {'@', '@', '@', stdPuncKeyWidth, 0},
    {'~', '~', '~', stdPuncKeyWidth, 0},
    {'&', '&', '&', stdPuncKeyWidth, 0},
    {'#', '#', '#', stdPuncKeyWidth, 0},
  };
  KeyboardLayoutType punc1KeyboardLayout_All = {
    {{2, 69}, {stdPuncKeyWidth * 4 + 1, keyboardRows * 13 + 1}},
    13, keyboardFont, 0,
    {4, 4, 4, 4},
    kbdNoKey, kbdNoKey, fFalse
  };

#define numNumberKeys 12
  KeyboardKeyType numberKeys_All[numNumberKeys] = {
    // First row
    {'1', '1', '1', stdNumberKeyWidth, 0},
    {'2', '2', '2', stdNumberKeyWidth, 0},
    {'3', '3', '3', stdNumberKeyWidth, 0},
    // Second row
    {'4', '4', '4', stdNumberKeyWidth, 0},
    {'5', '5', '5', stdNumberKeyWidth, 0},
    {'6', '6', '6', stdNumberKeyWidth, 0},
    // Third row
    {'7', '7', '7', stdNumberKeyWidth, 0},
    {'8', '8', '8', stdNumberKeyWidth, 0},
    {'9', '9', '9', stdNumberKeyWidth, 0},
    // Fourth row
    {'(', '(', '(', stdNumberKeyWidth, 0},
    {'0', '0', '0', stdNumberKeyWidth, 0},
    {')', ')', ')', stdNumberKeyWidth, 0},
  };
  KeyboardLayoutType numberKeyboardLayout_All = {
    {{49, 59}, {stdNumberKeyWidth * 3 + 1, keyboardRows * 16 + 1}},
    16, keyboardLargeFont, 0,
    {3, 3, 3, 3},
    kbdNoKey, kbdNoKey, fFalse
  };

  KeyboardKeyType accentKeys_US[kNumAccentKeys_US] = {
    // First row
    {upAAcuteChr, upAAcuteChr, lowAAcuteChr, stdAccentKeyWidth, 0},
    {upAGraveChr, upAGraveChr, lowAGraveChr, stdAccentKeyWidth, 0},
    {upADiaeresisChr, upADiaeresisChr, lowADiaeresisChr, stdAccentKeyWidth,
     0},
    {upACircumflexChr, upACircumflexChr, lowACircumflexChr,
     stdAccentKeyWidth, 0},
    {upARingChr, upARingChr, lowARingChr, stdAccentKeyWidth, 0},
    {upATildeChr, upATildeChr, lowATildeChr, stdAccentKeyWidth, 0},
    {upAEChr, upAEChr, lowAEChr, stdAccentKeyWidth, 0},
    {upCCedillaChr, upCCedillaChr, lowCCedillaChr, stdAccentKeyWidth, 0},
    {upNTildeChr, upNTildeChr, lowNTildeChr, stdAccentKeyWidth, 0},
    {upOSlashChr, upOSlashChr, lowOSlashChr, stdAccentKeyWidth, 0},
    {kbdBackspaceKey, kbdBackspaceKey, kbdBackspaceKey,
     2 * stdAlphaKeyWidth + 1, kbdBackspaceKey},
    // Second row
    {kbdTabKey, kbdTabKey, kbdTabKey, stdAccentKeyWidth + 3, kbdTabKey},
    {upEAcuteChr, upEAcuteChr, lowEAcuteChr, stdAccentKeyWidth, 0},
    {upEGraveChr, upEGraveChr, lowEGraveChr, stdAccentKeyWidth, 0},
    {upEDiaeresisChr, upEDiaeresisChr, lowEDiaeresisChr, stdAccentKeyWidth,
     0},
    {upECircumflexChr, upECircumflexChr, lowECircumflexChr,
     stdAccentKeyWidth, 0},
    {upIAcuteChr, upIAcuteChr, lowIAcuteChr, stdAccentKeyWidth, 0},
    {upIGraveChr, upIGraveChr, lowIGraveChr, stdAccentKeyWidth, 0},
    {upIDiaeresisChr, upIDiaeresisChr, lowIDiaeresisChr, stdAccentKeyWidth,
     0},
    {upICircumflexChr, upICircumflexChr, lowICircumflexChr,
     stdAccentKeyWidth, 0},
    {lowSharpSChr, lowSharpSChr, lowSharpSChr, stdAccentKeyWidth, 0},
    {upYAcuteChr, upYAcuteChr, lowYAcuteChr, stdAccentKeyWidth - 1, 0},
    {upYDiaeresisChr, upYDiaeresisChr, lowYDiaeresisChr,
     stdAccentKeyWidth - 1, 0},
    // Third row
    {kbdCapsKey, kbdCapsKey, kbdCapsKey, stdAccentKeyWidth * 3 / 2 + 2,
     keyTopCapLabel},
    {upOAcuteChr, upOAcuteChr, lowOAcuteChr, stdAccentKeyWidth, 0},
    {upOGraveChr, upOGraveChr, lowOGraveChr, stdAccentKeyWidth, 0},
    {upODiaeresisChr, upODiaeresisChr, lowODiaeresisChr, stdAccentKeyWidth,
     0},
    {upOCircumflexChr, upOCircumflexChr, lowOCircumflexChr,
     stdAccentKeyWidth, 0},
    {upOTildeChr, upOTildeChr, lowOTildeChr, stdAccentKeyWidth, 0},
    {upUAcuteChr, upUAcuteChr, lowUAcuteChr, stdAccentKeyWidth, 0},
    {upUGraveChr, upUGraveChr, lowUGraveChr, stdAccentKeyWidth, 0},
    {upUDiaeresisChr, upUDiaeresisChr, lowUDiaeresisChr, stdAccentKeyWidth,
     0},
    {upUCircumflexChr, upUCircumflexChr, lowUCircumflexChr,
     stdAccentKeyWidth, 0},
    {kbdReturnKey, kbdReturnKey, kbdReturnKey, stdAccentKeyWidth + 6, kbdReturnKey},    // carriage return
    // Fourth row
    {kbdShiftKey, kbdShiftKey, kbdShiftKey, stdAccentKeyWidth * 2 + 4,
     keyTopShiftLabel},
    {' ', ' ', ' ', 7 * stdAccentKeyWidth + 10, keyTopSpaceLabel},
    {invertedExclamationChr, invertedExclamationChr, invertedQuestionChr,
     stdAccentKeyWidth, 0},
    {rightGuillemetChr, rightGuillemetChr, leftGuillemetChr,
     stdAccentKeyWidth, 0},
  };
  KeyboardLayoutType accentKeyboardLayout_US = {
    {{-1, 65}, {158, keyboardRows * 14 + 1}},
    14, keyboardFont, 0,
    {11, 12, 11, 4},
    34, 23, fTrue
  };
  KeyboardKeyType accentKeys_Other[kNumAccentKeys_Other] = {
    // First row
    {upAAcuteChr, upAAcuteChr, lowAAcuteChr, stdAccentKeyWidth, 0},
    {upAGraveChr, upAGraveChr, lowAGraveChr, stdAccentKeyWidth, 0},
    {upADiaeresisChr, upADiaeresisChr, lowADiaeresisChr, stdAccentKeyWidth,
     0},
    {upACircumflexChr, upACircumflexChr, lowACircumflexChr,
     stdAccentKeyWidth, 0},
    {upARingChr, upARingChr, lowARingChr, stdAccentKeyWidth, 0},
    {upATildeChr, upATildeChr, lowATildeChr, stdAccentKeyWidth, 0},
    {upAEChr, upAEChr, lowAEChr, stdAccentKeyWidth, 0},
    {upCCedillaChr, upCCedillaChr, lowCCedillaChr, stdAccentKeyWidth, 0},
    {upNTildeChr, upNTildeChr, lowNTildeChr, stdAccentKeyWidth, 0},
    {upOSlashChr, upOSlashChr, lowOSlashChr, stdAccentKeyWidth, 0},
    {kbdBackspaceKey, kbdBackspaceKey, kbdBackspaceKey,
     2 * stdAlphaKeyWidth + 1, kbdBackspaceKey},
    // Second row
    {kbdTabKey, kbdTabKey, kbdTabKey, stdAccentKeyWidth + 3, kbdTabKey},
    {upEAcuteChr, upEAcuteChr, lowEAcuteChr, stdAccentKeyWidth, 0},
    {upEGraveChr, upEGraveChr, lowEGraveChr, stdAccentKeyWidth, 0},
    {upEDiaeresisChr, upEDiaeresisChr, lowEDiaeresisChr, stdAccentKeyWidth,
     0},
    {upECircumflexChr, upECircumflexChr, lowECircumflexChr,
     stdAccentKeyWidth, 0},
    {upIAcuteChr, upIAcuteChr, lowIAcuteChr, stdAccentKeyWidth, 0},
    {upIGraveChr, upIGraveChr, lowIGraveChr, stdAccentKeyWidth, 0},
    {upIDiaeresisChr, upIDiaeresisChr, lowIDiaeresisChr, stdAccentKeyWidth,
     0},
    {upICircumflexChr, upICircumflexChr, lowICircumflexChr,
     stdAccentKeyWidth, 0},
    {lowSharpSChr, lowSharpSChr, lowSharpSChr, stdAccentKeyWidth, 0},
    {upYAcuteChr, upYAcuteChr, lowYAcuteChr, stdAccentKeyWidth - 1, 0},
    {upYDiaeresisChr, upYDiaeresisChr, lowYDiaeresisChr,
     stdAccentKeyWidth - 1, 0},
    // Third row
    {kbdCapsKey, kbdCapsKey, kbdCapsKey, stdAccentKeyWidth * 3 / 2 + 2,
     kbdCapsKey},
    {upOAcuteChr, upOAcuteChr, lowOAcuteChr, stdAccentKeyWidth, 0},
    {upOGraveChr, upOGraveChr, lowOGraveChr, stdAccentKeyWidth, 0},
    {upODiaeresisChr, upODiaeresisChr, lowODiaeresisChr, stdAccentKeyWidth,
     0},
    {upOCircumflexChr, upOCircumflexChr, lowOCircumflexChr,
     stdAccentKeyWidth, 0},
    {upOTildeChr, upOTildeChr, lowOTildeChr, stdAccentKeyWidth, 0},
    {upUAcuteChr, upUAcuteChr, lowUAcuteChr, stdAccentKeyWidth, 0},
    {upUGraveChr, upUGraveChr, lowUGraveChr, stdAccentKeyWidth, 0},
    {upUDiaeresisChr, upUDiaeresisChr, lowUDiaeresisChr, stdAccentKeyWidth,
     0},
    {upUCircumflexChr, upUCircumflexChr, lowUCircumflexChr,
     stdAccentKeyWidth, 0},
    {kbdReturnKey, kbdReturnKey, kbdReturnKey, stdAccentKeyWidth + 6, kbdReturnKey},    // carriage return
    // Fourth row
    {kbdShiftKey, kbdShiftKey, kbdShiftKey, stdAccentKeyWidth * 2 + 4,
     kbdShiftKey},
    {upOEChr, upOEChr, lowOEChr, stdAccentKeyWidth, 0},
    {' ', ' ', ' ', 6 * stdAccentKeyWidth + 10, 0},
    {invertedExclamationChr, invertedExclamationChr, invertedQuestionChr,
     stdAccentKeyWidth, 0},
    {rightGuillemetChr, rightGuillemetChr, leftGuillemetChr,
     stdAccentKeyWidth, 0},
  };
  KeyboardLayoutType accentKeyboardLayout_Other = {
    {{-1, 65}, {158, keyboardRows * 14 + 1}},
    14, keyboardFont, 0,
    {11, 12, 11, 5},
    34, 23, fTrue
  };

  switch (country)
  {
    case COUNTRY_UNITED_STATES:
      numAlphaKeys = kNumAlphaKeys_US;
      pAlphaKeys = alphaKeys_US;
      pAlphaKeyboardLayout = &alphaKeyboardLayout_US;
      pPunc2Keys = punc2Keys_US;
      pPunc2KeyboardLayout = &punc2KeyboardLayout_All;
      pPunc1Keys = punc1Keys_Other;
      pPunc1KeyboardLayout = &punc1KeyboardLayout_All;
      pNumberKeys = numberKeys_All;
      pNumberKeyboardLayout = &numberKeyboardLayout_All;
      numAccentKeys = kNumAccentKeys_US;
      pAccentKeys = accentKeys_US;
      pAccentKeyboardLayout = &accentKeyboardLayout_US;
      break;

    case COUNTRY_FRANCE:
      numAlphaKeys = kNumAlphaKeys_France;
      pAlphaKeys = alphaKeys_France;
      pAlphaKeyboardLayout = &alphaKeyboardLayout_France;
      pPunc2Keys = punc2Keys_Other;
      pPunc2KeyboardLayout = &punc2KeyboardLayout_All;
      pPunc1Keys = punc1Keys_Other;
      pPunc1KeyboardLayout = &punc1KeyboardLayout_All;
      pNumberKeys = numberKeys_All;
      pNumberKeyboardLayout = &numberKeyboardLayout_All;
      numAccentKeys = kNumAccentKeys_Other;
      pAccentKeys = accentKeys_Other;
      pAccentKeyboardLayout = &accentKeyboardLayout_Other;
      break;

    case COUNTRY_GERMANY:
      numAlphaKeys = kNumAlphaKeys_Germany;
      pAlphaKeys = alphaKeys_Germany;
      pAlphaKeyboardLayout = &alphaKeyboardLayout_Germany;
      pPunc2Keys = punc2Keys_Other;
      pPunc2KeyboardLayout = &punc2KeyboardLayout_All;
      pPunc1Keys = punc1Keys_Germany;
      pPunc1KeyboardLayout = &punc1KeyboardLayout_All;
      pNumberKeys = numberKeys_All;
      pNumberKeyboardLayout = &numberKeyboardLayout_All;
      numAccentKeys = kNumAccentKeys_Other;
      pAccentKeys = accentKeys_Other;
      pAccentKeyboardLayout = &accentKeyboardLayout_Other;
      break;

    default:
      Error("Unknown country keyboard");
      return;
      break;
  }

  // Copy the KeyboardLayout structures
  pkeyboardLayout = (KeyboardLayoutType *) kbdResImage;
  memcpy(pkeyboardLayout, pAlphaKeyboardLayout, sizeof(KeyboardLayoutType));
  memcpy(pkeyboardLayout + 1, pNumberKeyboardLayout,
         sizeof(KeyboardLayoutType));
  memcpy(pkeyboardLayout + 2, pPunc1KeyboardLayout,
         sizeof(KeyboardLayoutType));
  memcpy(pkeyboardLayout + 3, pPunc2KeyboardLayout,
         sizeof(KeyboardLayoutType));
  memcpy(pkeyboardLayout + 4, pAccentKeyboardLayout,
         sizeof(KeyboardLayoutType));
  realResOffset = CbEmitStruct(pkeyboardLayout, szRCKEYBOARDLAYOUT, NULL, fFalse);      /* pAlphaKeyboardLayout */
  realResOffset += CbEmitStruct(pkeyboardLayout + 1, szRCKEYBOARDLAYOUT, NULL, fFalse); /* pNumberKeyboardLayout */
  realResOffset += CbEmitStruct(pkeyboardLayout + 2, szRCKEYBOARDLAYOUT, NULL, fFalse); /* pPunc1KeyboardLayout */
  realResOffset += CbEmitStruct(pkeyboardLayout + 3, szRCKEYBOARDLAYOUT, NULL, fFalse); /* pPunc2KeyboardLayout */
  realResOffset += CbEmitStruct(pkeyboardLayout + 4, szRCKEYBOARDLAYOUT, NULL, fFalse); /* pAccentKeyboardLayout */

  pRunning = (char *)(pkeyboardLayout + 5);

  // Copy the alpha keycaps
  if (numAlphaKeys != pAlphaKeyboardLayout->keysPerRow[0] +
      pAlphaKeyboardLayout->keysPerRow[1] +
      pAlphaKeyboardLayout->keysPerRow[2] +
      pAlphaKeyboardLayout->keysPerRow[3])
  {
    Error("Wrong number of alpha keys");
    return;
  }
  pkeyboardLayout->keys = realResOffset;         // offset to keys
  memcpy(pRunning, pAlphaKeys, sizeof(KeyboardKeyType) * numAlphaKeys);
  pRunning += sizeof(KeyboardKeyType) * numAlphaKeys;
  realResOffset += keyboardKeyEmit(pAlphaKeys, numAlphaKeys, fFalse);   /* pAlphaKeys */

  // Copy the number keycaps
  if (numNumberKeys != pNumberKeyboardLayout->keysPerRow[0] +
      pNumberKeyboardLayout->keysPerRow[1] +
      pNumberKeyboardLayout->keysPerRow[2] +
      pNumberKeyboardLayout->keysPerRow[3])
  {
    Error("Wrong number of keys");
    return;
  }
  (pkeyboardLayout + 1)->keys = realResOffset;   // offset to keys
  memcpy(pRunning, pNumberKeys, sizeof(KeyboardKeyType) * numNumberKeys);
  pRunning += sizeof(KeyboardKeyType) * numNumberKeys;
  realResOffset += keyboardKeyEmit(pNumberKeys, numNumberKeys, fFalse); /* pNumberKeys */

  // Copy the punc1 keycaps
  if (numPunc1Keys != pPunc1KeyboardLayout->keysPerRow[0] +
      pPunc1KeyboardLayout->keysPerRow[1] +
      pPunc1KeyboardLayout->keysPerRow[2] +
      pPunc1KeyboardLayout->keysPerRow[3])
  {
    Error("Wrong number of punc1 keys");
    return;
  }
  (pkeyboardLayout + 2)->keys = realResOffset;   // offset to keys
  memcpy(pRunning, pPunc1Keys, sizeof(KeyboardKeyType) * numPunc1Keys);
  pRunning += sizeof(KeyboardKeyType) * numPunc1Keys;
  realResOffset += keyboardKeyEmit(pPunc1Keys, numPunc1Keys, fFalse);   /* pPunc1Keys */

  // Copy the punc2 keycaps
  if (numPunc2Keys != pPunc2KeyboardLayout->keysPerRow[0] +
      pPunc2KeyboardLayout->keysPerRow[1] +
      pPunc2KeyboardLayout->keysPerRow[2] +
      pPunc2KeyboardLayout->keysPerRow[3])
  {
    Error("Wrong number of punc2 keys");
    return;
  }
  (pkeyboardLayout + 3)->keys = realResOffset;   // offset to keys
  memcpy(pRunning, pPunc2Keys, sizeof(KeyboardKeyType) * numPunc2Keys);
  pRunning += sizeof(KeyboardKeyType) * numPunc2Keys;
  realResOffset += keyboardKeyEmit(pPunc2Keys, numPunc2Keys, fFalse);   /* pPunc2Keys */

  // Copy the accent keycaps
  if (numAccentKeys != pAccentKeyboardLayout->keysPerRow[0] +
      pAccentKeyboardLayout->keysPerRow[1] +
      pAccentKeyboardLayout->keysPerRow[2] +
      pAccentKeyboardLayout->keysPerRow[3])
  {
    Error("Wrong number of accent keys");
    return;
  }
  (pkeyboardLayout + 4)->keys = realResOffset;   // offset to keys
  memcpy(pRunning, pAccentKeys, sizeof(KeyboardKeyType) * numAccentKeys);
  pRunning += sizeof(KeyboardKeyType) * numAccentKeys;
  realResOffset += keyboardKeyEmit(pAccentKeys, numAccentKeys, fFalse); /* pAccentKeys */

  // Copy the key top labels and point keys to them
  pFirstKeyTopLabel = pRunning;
  stringsSize = 0;
  for (i = 0; i < numKeyTopLabels; i++)
  {
    pThisKeyTopLabel = pRunning;
    strcpy((char *)pThisKeyTopLabel, keyTopLabels[i]);
    thisKeyTopLabelSize = strlen((char *)pThisKeyTopLabel) + 1;
    stringsSize += thisKeyTopLabelSize;
    pRunning += thisKeyTopLabelSize;
    // For all keys if they use a string label update the offset
    pkeyboardKey = (KeyboardKeyType *) (pkeyboardLayout + 5);
    while ((char *)pkeyboardKey < (char *)pFirstKeyTopLabel)
    {
      if (pkeyboardKey->labelOffset == (firstKeyTopLabel + i))
        pkeyboardKey->labelOffset = realResOffset;
      pkeyboardKey++;
    }
    realResOffset += thisKeyTopLabelSize;
  }

  // We write the datas to disk
  OpenOutput(kPalmResType[kKeyboardType], resID);       /* RMa "tkbd" */

  pkeyboardLayout = (KeyboardLayoutType *) kbdResImage;

  // Emit the KeyboardLayout structures
  CbEmitStruct(pkeyboardLayout, szRCKEYBOARDLAYOUT, NULL, fTrue);       /* pAlphaKeyboardLayout */
  CbEmitStruct(pkeyboardLayout + 1, szRCKEYBOARDLAYOUT, NULL, fTrue);   /* pNumberKeyboardLayout */
  CbEmitStruct(pkeyboardLayout + 2, szRCKEYBOARDLAYOUT, NULL, fTrue);   /* pPunc1KeyboardLayout */
  CbEmitStruct(pkeyboardLayout + 3, szRCKEYBOARDLAYOUT, NULL, fTrue);   /* pPunc2KeyboardLayout */
  CbEmitStruct(pkeyboardLayout + 4, szRCKEYBOARDLAYOUT, NULL, fTrue);   /* pAccentKeyboardLayout */
  // Emit the alpha keycaps
  pRunning = (char *)(pkeyboardLayout + 5);
  keyboardKeyEmit((KeyboardKeyType *) pRunning, numAlphaKeys, fTrue);   /* pAlphaKeys */
  pRunning += sizeof(KeyboardKeyType) * numAlphaKeys;
  // Emit the number keycaps
  keyboardKeyEmit((KeyboardKeyType *) pRunning, numNumberKeys, fTrue);  /* pNumberKeys */
  pRunning += sizeof(KeyboardKeyType) * numNumberKeys;
  // Emit the punc1 keycaps
  keyboardKeyEmit((KeyboardKeyType *) pRunning, numPunc1Keys, fTrue);   /* pPunc1Keys */
  pRunning += sizeof(KeyboardKeyType) * numPunc1Keys;
  // Emit the punc2 keycaps
  keyboardKeyEmit((KeyboardKeyType *) pRunning, numPunc2Keys, fTrue);   /* pPunc2Keys */
  pRunning += sizeof(KeyboardKeyType) * numPunc2Keys;
  // Emit the accent keycaps
  keyboardKeyEmit((KeyboardKeyType *) pRunning, numAccentKeys, fTrue);  /* pAccentKeys */
  pRunning += sizeof(KeyboardKeyType) * numAccentKeys;
  // Emit the strings
  DumpBytes(pRunning, stringsSize);

  CloseOutput();
}
