
/*
 * @(#)macres.h
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

typedef struct _fobj
{
  Word idObj;
  DWord rid;
}
RFOBJ;

typedef struct _FRM
{
  RectangleType rc;
  Word rfUsable;
  Word rfModal;
  Word rfSaveBehind;
  Word wUnused;
  Word wUnused2;
  Word formId;
  Word helpRscId;
  Word menuRscId;
  Word defaultButton;
  Word wUnused3;
  Word wUnused4;
  Word nuMObjects;
  FOBJ rgfobj[1];
}
RFORM;

typedef struct _rbtn
{
  Word btnId;
  RectangleType rc;
  Word rfUsable;
  Word rfLeftAnchor;
  Word rfFrame;
  Word rfNonBoldFrame;
  Word rfFont;
}
RBTN;

/*
 * #define CbBtn(pbtn) (sizeof(RBTN)-256+strlen(pbtn->szLabel)+1) 
 */

#define rfTrue 0x100
#define rfFalse 0

#define RfBool(f) ((f) ? rfTrue : rfFalse)
#defien RfByte(b) ((b) << 8)
