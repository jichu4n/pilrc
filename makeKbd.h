
/*
 * @(#)makeKbd.h
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
 *                 creation
 */

#ifndef __MAKEKBD_H__
#define __MAKEKBD_H__

#define  	COUNTRY_UNITED_STATES	0
#define  	COUNTRY_FRANCE				1
#define  	COUNTRY_GERMANY			2

// Data structures from Keyboard.h

#define kbdReturnKey		linefeedChr
#define kbdTabKey			tabChr
#define kbdBackspaceKey	backspaceChr
#define kbdShiftKey		2
#define kbdCapsKey		1
#define kbdNoKey			-1

// Data structures from Keyboard.c

#define keyboardFont			0
#define keyboardLargeFont	2

enum
{ alphaKeyboard = 0, puncKeyboard = 1, accentKeyboard = 2 };

#define stdAlphaKeyWidth (156 /  12)
#define stdAccentKeyWidth (156 /  12)
#define stdNumberKeyWidth 19
#define stdPuncKeyWidth 11

// These are the keyboard layout structures.  They define the keys for all the
// keyboards.

//#pragma pcrelstrings off

#define keyTopCapLabel		16
#define keyTopShiftLabel	17
#define keyTopSpaceLabel	18
#define firstKeyTopLabel	keyTopCapLabel
#define numKeyTopLabels		3

void createAndSaveCountryKeyboard(int country,
                                  int resID);

#endif                                           // __MAKEKBD_H__
