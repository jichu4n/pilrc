/*
 * @(#)restype.c
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

#include "restype.h"

char	*kPalmResType[kMaxNumberResType] =
{
#ifdef ARM
/*  0 */	"tAIN",
/*  1 */	"tSTR",
/*  2 */	"tver",
/*  3 */	"tAIS",
/*  4 */	"taic",
/*  5 */	"tSTL",
/*  6 */	"aaib",
/*  7 */	"abmp",
/*  8 */	"absb",
/*  9 */	"aalt",
/* 10 */	"akbd",
/* 11 */	"amnu",
/* 12 */	"afnt",
/* 13 */	"afti",
/* 14 */	"amid",
/* 15 */	"aclt",
/* 16 */	"acbr",
/* 17 */	"aint",
/* 18 */	"afrm",
/* 19 */	"awrd",
/* 20 */	"APPL",
/* 21 */	"TRAP",
/* 22 */	"tTTL",
/* 23 */	"tLBL",
#else
/*  0 */	"tAIN",
/*  1 */	"tSTR",
/*  2 */	"tver",
/*  3 */	"tAIS",
/*  4 */	"taic",
/*  5 */	"tSTL",
/*  6 */	"tAIB",
/*  7 */	"Tbmp",
/*  8 */	"Tbsb",
/*  9 */	"Talt",
/* 10 */	"tkbd",
/* 11 */	"MBAR",
/* 12 */	"NFNT",
/* 13 */	"fnti",
/* 14 */	"MIDI",
/* 15 */	"tclt",
/* 16 */	"tcbr",
/* 17 */	"tint",
/* 18 */	"tFRM",
/* 19 */	"wrdl",
/* 20 */	"APPL",
/* 21 */	"TRAP",
/* 22 */	"tTTL",
/* 23 */	"tLBL",
#endif
};

