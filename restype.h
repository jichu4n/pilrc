/*
 * @(#)restype.h
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
 *     18-Aug-2000 RMa
 *                 creation
 */

#ifndef __RESTYPE_H__
#define __RESTYPE_H__

#define k68k_ALIGN_16		0
#define kARM_ALIGN_32		1
/*	not use now
	#define kARM_ALIGN_16	3
	#define k68k_ALIGN_32	4
*/

#define kAinRscType				0
#define kStrRscType				1
#define kVerRscType				2
#define kAppInfoStringsRscType	3
#define kDefaultCategoryRscType	4
#define kStrListRscType			5

#define kIconType				6
#define kBitmapRscType			7
#define kBsBitmapRscType		8
#define kAlertRscType			9
#define kBdRscType				10
#define kMenuRscType			11
#define kFontRscType			12
#define kFontIndexType			13
#define kMidiRscType			14
#define kColorTableRscType		15
#define kMenuCtlRscType			16
#define kConstantRscType		17
#define kFormRscType			18
#define kWrdListRscType			19
#define kApplicationType		20
#define kTrapType				21
#define kTitleTextType			22
#define kLabelTextType			23

#define kMaxNumberResType		24


extern char	*kPalmResType[kMaxNumberResType];


#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus 
}
#endif

#endif // __RESTYPE_H__
