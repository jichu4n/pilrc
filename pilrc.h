
/*
 * @(#)pilrc.h
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
 *     23-Jun-2000 Mark Garlanger
 *                 Additions to support #ifdef/#ifndef/#else/#endif in
 *                 both .rcp files and .h files.
 *     20-Nov-2000 Renaud Malaval
 *                 additions PalmOS 3.5 support
 *                 additions to support LE32 resouces
 */

#ifndef _pilrc_h
#define _pilrc_h                                 // RMA : multiples include protection

#if (SIZEOF_INT == SIZEOF_CHAR_P)
typedef int p_int;
#elif (SIZEOF_LONG == SIZEOF_CHAR_P)
typedef long p_int;
#endif

#if (SIZEOF_SHORT * 2 == SIZEOF_CHAR_P)
typedef short p_short;
#elif (SIZEOF_INT * 2 == SIZEOF_CHAR_P)
typedef int p_short;
#endif

#ifdef WIN32                                     // little endian
#define HOST_LITTLE_ENDIAN
#else                                            // big endian
#define HOST_BIG_ENDIAN
#endif

#include "std.h"
#include "util.h"
#include "lex.h"
#include "plex.h"
#include "font.h"

/*-----------------------------------------------------------------------------
|	 PILOT STRUCTS
|
|
|	a note on the strings following some of the following structures:
|
|	the strings define how to dump the structure out in the same format as the
|	pilot structs.  This way you do not have to include pilot include files
|	which are definitely not compiler/processor independent.  
|
|	note:  we make the assumption that sizeof(p_int) == sizeof(void *) -- the emitter
|	moves through the structs as if they were an array of ints and emits the struct
|	according to the template
|
|	template syntax:
|
|	base types:
|		b : byte (8 bits)
|		w : word (16 bits)
|		l : long (32 bits)
|		t : bit (1 bit of 8 bits)
|		u : bits (1 bits of 16 bits)		// RMa additions
|		p : pointer (32 bits)
|
|	prefixes: (optional)
|		z : zero -- there is no corresponding field in our structs -- emit the
|			base type filled w/ 0
|
|	suffixes: (optional)
|		# : (decimal numeric) --  number of times to repeat the previous type.
|			For bits (t) this specifies the number of bits to shift in.
|			Note that the bits must not cross byte boundaries in any one field emission.
-------------------------------------------------------------WESC------------*/

#define leftAlign 0
#define centerAlign 1
#define rightAlign 2

#define noButtonFrame 0
#define standardButtonFrame 1
#define boldButtonFrame 2
#define rectangleButtonFrame 3

#define Enum(type) typedef enum type
#define EndEnum

Enum(controlStyles)
{
  buttonCtl, pushButtonCtl, checkboxCtl, popupTriggerCtl, selectorTriggerCtl, repeatingButtonCtl, sliderCtl, feedbackSliderCtl  /* RMa add support sliders */
}

EndEnum ControlStyles;

Enum(alertTypes)
{
informationAlert, confirmationAlert, warningAlert, errorAlert}

EndEnum AlertType;

Enum(formObjects)
{
  frmFieldObj, frmControlObj, frmListObj, frmTableObj, frmBitmapObj, frmLineObj, frmFrameObj, frmRectangleObj, frmLabelObj, frmTitleObj, frmPopupObj, frmGraffitiStateObj, frmGadgetObj, frmScrollbarObj, frmSliderObj, /* RMa add support sliders */
    frmGraphicalControlObj                       /* RMa add support graphical control */
}

EndEnum FormObjectKind;

Enum(fontID)
{
stdFont, boldFont, largeFont, symbolFont, symbol11Font, symbol7Font, ledFont}

EndEnum FontID;

typedef struct _rcpoint
{
  p_int x;
  p_int y;
}
RCPOINT;

typedef struct _Rcrect
{
  RCPOINT topLeft;
  RCPOINT extent;
}
RCRECT;

/************************************************\
 * RMa update all structure to be compleant with
 *  PalmOS 3.5
 *  LE32 ( 16 and 32 bits memory alignment)
\************************************************/

/*
 * window.h 
 */
typedef struct _rcframebits
{
  p_int cornerDiam;                              /* u8   */
  /*
   * p_int reserved;           zu3 
   */
  p_int threeD;                                  /* u   */
  p_int shadowWidth;                             /* u2  */
  p_int width;                                   /* u2  */
}
RCFRAMEBITS;                                     /* u8zu3uu2u2 */

typedef struct _rcwindowflags
{
  /*
   * Word format:1;      zu 
   */
  /*
   * Word offscreen:1;   zu 
   */
  p_int modal;                                   /* u  */
  p_int focusable;                               /* u  */
  /*
   * Word enabled:1;    zu 
   */
  /*
   * Word visible:1;     zu  
   */
  p_int dialog;                                  /* u */
  /*
   * Word freeBitmap:1   zu 
   */
  /*
   * Word reserved :8;   zu8 
   */
}
RCWINDOWFLAGS;                                   /* zuzuuu zuzuuzu zu8 */

typedef struct _rcwindowBA16Type
{
  p_int displayWidth;                            /* w */
  p_int displayHeight;                           /* w */
  /*
   * p_int displayAddr;                                        zl 
   */
  RCWINDOWFLAGS windowFlags;                     /* zuzuuu zuzuuzu zu8 */
  RCRECT windowBounds;                           /* w4 */
  /*
   * RCRECT clippingBounds;                          zw4 
   */
  /*
   * BitmapPtr bitmapP;                              zl 
   */
  RCFRAMEBITS frameType;                         /* u8zu3uu2u2 */
  /*
   * GraphicStatePtr drawStateP;             zl 
   */
  /*
   * struct RCWindowBA16Type *nextWindow; zl 
   */
}
RCWindowBA16Type;

#define szRCWindowBA16 "w,w,zl,zuzuuuzuzuuzuzu8,w4,zw4,zl,u8zu3uu2u2,zl,zl"

typedef struct _rcwindowBA32Type
{
  p_int displayWidth;                            /* w */
  p_int displayHeight;                           /* w */
  /*
   * p_int displayAddr;                                        zl 
   */
  RCRECT windowBounds;                           /* w4 */
  /*
   * RCRECT clippingBounds;                          zw4 
   */
  /*
   * BitmapPtr bitmapP;                              zl 
   */
  RCWINDOWFLAGS windowFlags;                     /* zuzuuu zuzuuzu zu8 */
  RCFRAMEBITS frameType;                         /* u8zu3uu2u2 */
  /*
   * GraphicStatePtr drawStateP;             zl 
   */
  /*
   * struct RCWindow32Type *nextWindow;  zl 
   */
}
RCWindowBA32Type;

#define szRCWindowBA32 "w,w,zl,w4,zw4,zl,zuzuuuzuzuuzuzu8,u8zu3uu2u2,zl,zl"

/*
 * ------------------ List ------------------ 
 */
typedef struct _rclistattr
{
  p_int usable;                                  /* u */
  p_int enabled;                                 /* u */
  p_int visible;                                 /* u */
  p_int poppedUp;                                /* u */
  p_int hasScrollBar;                            /* u */
  p_int search;                                  /* u */
  /*
   * p_int reserved;           zu10 
   */
}
RCLISTATTR;                                      /* uuuuuuzu10 */

typedef struct _rcListBA16Type
{
  p_int id;                                      /* w */
  RCRECT bounds;                                 /* w4 */
  RCLISTATTR attr;                               /* uuuuuuzu10 */
  char *itemsText;                               /* p */
  p_int numItems;                                /* w */
  /*
   * Word currentItem;       zw 
   */
  /*
   * Word topItem;                   zw 
   */
  p_int font;                                    /* b  */
  /*
   * UInt8 reserved;             zb 
   */
  /*
   * WinHandle popupWin;     zl 
   */
  /*
   * ListDrawDataFuncPtr  drawItemsCallback;  zl 
   */
  /*
   * private, not stored into file 
   */
  int cbListItems;
}
RCListBA16Type;

#define szRCListBA16 "w,w4,uuuuuuzu10,p,w,zw,zw,bzb,zl,zl"

typedef struct _rclistBA32Type
{
  p_int id;                                      /* w */
  RCLISTATTR attr;                               /* uuuuuuzu10 */
  RCRECT bounds;                                 /* w4 */
  char *itemsText;                               /* p */
  p_int numItems;                                /* w */
  /*
   * Word currentItem;       zw 
   */
  /*
   * Word topItem;                   zw 
   */
  p_int font;                                    /* b  */
  /*
   * UInt8 reserved;             zb 
   */
  /*
   * WinHandle popupWin;     zl 
   */
  /*
   * ListDrawDataFuncPtr  drawItemsCallback;  zl 
   */
  /*
   * private, not stored into file 
   */
  int cbListItems;
}
RCListBA32Type;

#define szRCListBA32 "w,uuuuuuzu10,w4,p,w,zw,zw,bzb,zl,zl"

typedef union _rcListType
{
  RCListBA16Type s16;
  RCListBA32Type s32;
}
RCListType;

/*
 * ------------------ Field ------------------ 
 */
typedef struct _rcfieldattr
{
  p_int usable;                                  /* u  */
  p_int visible;                                 /* u  */
  p_int editable;                                /* u  */
  p_int singleLine;                              /* u  */

  p_int hasFocus;                                /* u  */
  p_int dynamicSize;                             /* u  */
  p_int insPtVisible;                            /* u  */
  p_int dirty;                                   /* u  */

  p_int underlined;                              /* u2 */
  p_int justification;                           /* u2 */
  p_int autoShift;                               /* u  */
  p_int hasScrollBar;                            /* u  */
  p_int numeric;                                 /* u  */
  /*
   * zu 
   */
}
RCFIELDATTR;                                     /* uuuuuuuu u2u2uuuzu */

typedef struct _rcFieldBA16Type
{
  p_int id;                                      /* w */
  RCRECT rect;                                   /* w4 */
  RCFIELDATTR attr;                              /* uuuuuuuu u2u2uuuzu */
  char *text;                                    /* p */
  /*
   * VoidHand                    zl       textHandle;                      block the contains the text string 
   */
  /*
   * LineInfoPtr         zl       lines; 
   */
  /*
   * Word                        zw       textLen; 
   */
  /*
   * Word                        zw       textBlockSize; 
   */
  p_int maxChars;                                /* w */
  /*
   * p_int selFirstPos;    zw  
   */
  /*
   * p_int selLastPos;     zw 
   */
  /*
   * p_int insPtXPos;      zw 
   */
  /*
   * p_int insPtYPos;      zw 
   */
  p_int fontID;                                  /* b */
  /*
   * p_int reserved;           zb 
   */
}
RCFieldBA16Type;

#define szRCFieldBA16 "w,w4,uuuuuuuu,u2u2uuuzu,p,zl,zl,zw,zw,w,zw,zw,zw,zw,b,zb"

typedef struct _rcFieldBA32Type
{
  RCRECT rect;                                   /* w4 */
  p_int id;                                      /* w */
  RCFIELDATTR attr;                              /* uuuuuuuu u2u2uuuzu */
  char *text;                                    /* p */
  /*
   * VoidHand                    zl       textHandle;                      block the contains the text string 
   */
  /*
   * LineInfoPtr         zl       lines; 
   */
  /*
   * Word                        zw       textLen; 
   */
  /*
   * Word                        zw       textBlockSize; 
   */
  p_int maxChars;                                /* w */
  /*
   * p_int selFirstPos;    zw  
   */
  /*
   * p_int selLastPos;     zw 
   */
  /*
   * p_int insPtXPos;      zw 
   */
  /*
   * p_int insPtYPos;      zw 
   */
  p_int fontID;                                  /* b */
  /*
   * p_int reserved;           zb 
   */
}
RCFieldBA32Type;

#define szRCFieldBA32 "w4,w,uuuuuuuu,u2u2uuuzu,p,zl,zl,zw,zw,w,zw,zw,zw,zw,b,zb"

typedef union _rcFieldType
{
  RCFieldBA16Type s16;
  RCFieldBA32Type s32;
}
RCFieldType;

/*
 * ------------------ Table ------------------ 
 */
typedef struct _rcTableColumnAttrBA16Type
{
  p_int width;                                   /* w  */
  /*
   * p_int reserved1;                                              zt5 
   */
  p_int masked;                                  /* t  */
  p_int editIndicator;                           /* t  */
  p_int usable;                                  /* t */
  /*
   * p_int reserved2;                                                  zb 
   */
  p_int spacing;                                 /* w  */
  /*
   * TableDrawItemFuncPtr drawCallback;           zl 
   */
  /*
   * TableLoadDataFuncPtr loadDataCallback;   zl 
   */
  /*
   * TableSaveDataFuncPtr saveDataCallback;   zl 
   */
}
RCTableColumnAttrBA16Type;

#define szRCTableColumnAttrBA16 "w,zt5tttzb,w,zl,zl,zl"

typedef struct _rcTableColumnAttrBA32Type
{
  p_int width;                                   /* w  */
  p_int spacing;                                 /* w  */
  /*
   * p_int reserved1;                                              zt5 
   */
  p_int masked;                                  /* t  */
  p_int editIndicator;                           /* t  */
  p_int usable;                                  /* t */
  /*
   * p_int reserved2;                                                  zb 
   */
  /*
   * p_int reserved3;                                                  zw 
   */
  /*
   * TableDrawItemFuncPtr drawCallback;                   zl 
   */
  /*
   * TableLoadDataFuncPtr loadDataCallback;   zl 
   */
  /*
   * TableSaveDataFuncPtr saveDataCallback;   zl 
   */
}
RCTableColumnAttrBA32Type;

#define szRCTableColumnAttrBA32 "w,w,zt5tttzb,zw,zl,zl,zl"

#define szRCTABLECOLUMNATTR (vfLE32?szRCTableColumnAttrBA32:szRCTableColumnAttrBA16)
typedef union _RCTABLECOLUMNATTR
{
  RCTableColumnAttrBA16Type s16;
  RCTableColumnAttrBA32Type s32;
}
RCTABLECOLUMNATTR;

typedef struct _rcTableRowAttrType
{
  p_int id;                                      /* w  */
  p_int height;                                  /* w  */
  /*
   * DWord data;             zl 
   */
  /*
   * p_int reserved1;                  zt7 
   */
  p_int usable;                                  /* t  */
  /*
   * p_int reserved2;                  zt4 
   */
  p_int masked;                                  /* t  */
  p_int invalid;                                 /* t  */
  p_int staticHeight;                            /* t  */
  p_int selectable;                              /* t  */
  /*
   * p_int reserved3;                  zw 
   */
}
RCTABLEROWATTR;

#define szRCTABLEROWATTR "w,w,zl,zt7t,zt4tttt,zw"

/*
 * this is bogus...don't know why 
 */
#define szRCTABLEPADDING "zl,zl"

typedef struct RCTableAttrType
{
  p_int visible;                                 /* u */
  p_int editable;                                /* u */
  p_int editing;                                 /* u */
  p_int selected;                                /* u */
  p_int hasScrollBar;                            /* u */
  /*
   * p_int reserved;                   zu11 
   */
}
RCTABLEATTR;                                     /* uuuu uzu11 */

typedef struct _rcTableBA16Type
{
  p_int id;                                      /* w  */
  RCRECT bounds;                                 /* w4 */
  RCTABLEATTR attr;                              /* uuuu uzu11 */
  p_int numColumns;                              /* w */
  p_int numRows;                                 /* w */
  p_int currentRow;                              /* w */
  p_int currentColumn;                           /* w */
  p_int topRow;                                  /* w */
  /*
   * TableColumnAttrType * columnAttrs;           zl 
   */
  /*
   * TableRowAttrType *      rowAttrs;            zl 
   */
  /*
   * TableItemPtr            items;                   zl 
   */
  RCFieldBA16Type currentField;
  /*
   * not emitted 
   */
  p_int *rgdxcol;
}
RCTableBA16Type;

#define szRCTableBA16 "w,w4,uuuuuzu11,w,w,w,w,w,zl,zl,zl" szRCFieldBA16
typedef struct _rcTableBA32Type
{
  p_int id;                                      /* w  */
  RCTABLEATTR attr;                              /* uuuu uzu11 */
  RCRECT bounds;                                 /* w4 */
  p_int numColumns;                              /* w */
  p_int numRows;                                 /* w */
  p_int currentRow;                              /* w */
  p_int currentColumn;                           /* w */
  p_int topRow;                                  /* w */
  /*
   * p_int reserved                                    zw 
   */
  /*
   * TableColumnAttrType *   columnAttrs; zl 
   */
  /*
   * TableRowAttrType *      rowAttrs;            zl 
   */
  /*
   * TableItemPtr            items;                    zl 
   */
  RCFieldBA32Type currentField;
  /*
   * not emitted 
   */
  p_int *rgdxcol;
}
RCTableBA32Type;

#define szRCTableBA32 "w,uuuuuzu11,w4,w,w,w,w,w,zw,zl,zl,zl" szRCFieldBA32

typedef union _rcTableType
{
  RCTableBA16Type s16;
  RCTableBA32Type s32;
}
RCTableType;

/*
 * ------------------ Form Label ------------------ 
 */

/*
 * form.h 
 */
typedef struct _rcformobjattr
{
  p_int usable;                                  /* t,zt7 (opt) */
  /*
   * p_int reserved            zb 
   */
}
RCFORMOBJATTR;                                   /* tzt7,zb */

typedef struct _rcFormLabelBA16Type
{
  p_int id;                                      /* w */
  RCPOINT pos;                                   /* w2 */
  RCFORMOBJATTR attr;                            /* uzu15 */
  p_int fontID;                                  /* b */
  /*
   * p_int reserved        zb 
   */
  char *text;                                    /* p */
}
RCFormLabelBA16Type;

#define szRCFormLabelBA16 "w,w2,uzu15,b,zb,p"

typedef struct _rcFormLabelBA32Type
{
  RCPOINT pos;                                   /* w2 */
  char *text;                                    /* p */
  p_int id;                                      /* w */
  RCFORMOBJATTR attr;                            /* uzu15 */
  p_int fontID;                                  /* b */
  /*
   * p_int reserved        zb 
   */
  /*
   * p_int padding;            zw 
   */
}
RCFormLabelBA32Type;

#define szRCFormLabelBA32 "w2,p,w,uzu15,b,zb,zw"

typedef union _rcFormLabelType
{
  RCFormLabelBA16Type s16;
  RCFormLabelBA32Type s32;
}
RCFormLabelType;

/*
 * ------------------ Form Title ------------------ 
 */
typedef struct _rcformtitle
{
  RCRECT rect;                                   /* w4 */
  char *text;                                    /* p */
}
RCFORMTITLE;

#define szRCFORMTITLE "w4,p"

/*
 * ------------------ Form Popup ------------------ 
 */
typedef struct _rcformpopup
{
  p_int controlID;                               /* w */
  p_int listID;                                  /* w */
}
RCFORMPOPUP;

#define szRCFORMPOPUP "ww"

/*
 * ------------------ Form Graffiti State ------------------ 
 */
typedef struct _rcgsi
{
  RCPOINT pos;                                   /* w2 */
}
RCFORMGRAFFITISTATE;

#define szRCFORMGRAFFITISTATE "w2"

/*
 * ------------------ Form Gadget ------------------ 
 */
typedef struct _rcFormGadgetAttr
{
  p_int usable;                                  /* u */
  p_int extended;                                /* u */
  p_int visible;                                 /* u */
  /*
   * p_int reserved            zu13 
   */
}
RCFormGadgetAttr;                                /* uuuzu13 */

typedef struct _rcformgadget
{
  p_int id;                                      /* w */
  RCFormGadgetAttr attr;                         /* uuuzu13 */
  RCRECT rect;                                   /* w4 */
  /*
   * VoidPtr data;                                   zl 
   */
  /*
   * FormGadgetHandlerType        *handler   zl 
   */
}
RCFORMGADGET;

#define szRCFORMGADGET "w,uuuzu13,w4,zl,zl"

/*
 * ------------------ Form Bitmap ------------------ 
 */
typedef struct _rcFormBitMapBA16Type
{
  RCFORMOBJATTR attr;                            /* uzu15 */
  RCPOINT pos;                                   /* w2 */
  p_int rscID;                                   /* w */
}
RCFormBitMapBA16Type;

#define szRCFormBitMapBA16 "uzu15,w2,w"

typedef struct _rcFormBitMapBA32Type
{
  RCFORMOBJATTR attr;                            /* uzu15 */
  p_int rscID;                                   /* w */
  RCPOINT pos;                                   /* w2 */
}
RCFormBitMapBA32Type;

#define szRCFormBitMapBA32 "uzu15,w,w2"

typedef union _rcFormBitMapType
{
  RCFormBitMapBA16Type s16;
  RCFormBitMapBA32Type s32;
}
RCFormBitMapType;

/*
 * ------------------ Control ------------------ 
 */
typedef struct _rccontrolattr
{
  p_int usable;                                  /* u  */
  p_int enabled;                                 /* u  */
  p_int visible;                                 /* u  *//* OS use internal */
  p_int on;                                      /* u  */
  p_int leftAnchor;                              /* u  */
  p_int frame;                                   /* u3 */

  p_int drawnAsSelected;                         /* u  */
  p_int graphical;                               /* u  */
  p_int vertical;                                /* u  */
  /*
   * p_int reserved;                   zu5 
   */
}
RCCONTROLATTR;                                   /* uuuuuu3 uuuzu5 */

typedef struct _rcControlBA16Type
{
  p_int id;                                      /* w */
  RCRECT bounds;                                 /* w4 */
  union
  {
    struct
    {
      p_short backgroundid;                      /* l/2 switch order on intel */
      p_short thumbid;                           /* l/2 */
    }
    ids;
    char *text;                                  /* p */
  }
  u;
  RCCONTROLATTR attr;                            /* uuuuuu3 uuuzu5 */
  p_int style;                                   /* b */
  p_int font;                                    /* b */
  p_int group;                                   /* b */
}
RCControlBA16Type;

#define szRCControlBA16 "w,w4,p,uuuuuu3,uuuzu5,b,b,b,zb"
#define szRCGraphicalControlBA16 "w,w4,l,uuuuuu3,uuuzu5,b,b,b,zb"

typedef struct _rcControlBA32Type
{
  p_int id;                                      /* w */
  RCCONTROLATTR attr;                            /* uuuuuu3 uuuzu5 */
  RCRECT bounds;                                 /* w4 */
  union
  {
    struct
    {
      p_short backgroundid;                      /* l/2 switch order on intel */
      p_short thumbid;                           /* l/2 */
    }
    ids;
    char *text;                                  /* p */
  }
  u;
  p_int style;                                   /* b */
  p_int font;                                    /* b */
  p_int group;                                   /* b */
  /*
   * p_int reserved;                           zb 
   */
}
RCControlBA32Type;

#define szRCControlBA32 "w,uuuuuu3,uuuzu5,w4,p,b,b,b,zb"
#define szRCGraphicalControlBA32 "w,uuuuuu3,uuuzu5,w4,l,b,b,b,zb"

typedef union _rcControlType
{
  RCControlBA16Type s16;
  RCControlBA32Type s32;
}
RCControlType;

/*
 * ------------------ Slider ------------------ 
 */
typedef struct _SliderControlBA16Type
{
  p_int id;                                      /* w  */
  RCRECT bounds;                                 /* w4 */

  /*
   * reversing this 2 param to manage word ordering 
   */
  p_short backgroundid;                          /* l  */// overlays text in ControlBA16Type
  p_short thumbid;                               /*     */// overlays text in ControlBA16Type
  RCCONTROLATTR attr;                            /* uuuuuu3,uuuzu5 */// graphical *is* set
  p_int style;                                   /* b */// must be sliderCtl or repeatingSliderCtl
  /*
   * UInt8                        reserved;                  zb 
   */
  p_int minValue;                                /* w */
  p_int maxValue;                                /* w */
  p_int pageSize;                                /* w *//* pageJump == pageSize */
  p_int value;                                   /* w */
  /*
   * MemPtr                       activeSliderP;     zl 
   */
}
RCSliderControlBA16Type;

#define szRCSliderControlBA16 "w,w4,l,uuuuuu3,uuuzu5,b,zb,w,w,w,w,zl"

typedef struct _SliderControlBA32Type
{
  p_int id;                                      /* w */
  RCCONTROLATTR attr;                            /* uuuuuu3,uuuzu5 */// graphical *is* set
  RCRECT bounds;                                 /* w4 */
  p_short thumbid;                               /*    */// overlays text in ControlBA16Type
  p_short backgroundid;                          /* l  */// overlays text in ControlBA16Type
  p_int style;                                   /* b  */// must be sliderCtl or repeatingSliderCtl
  /*
   * UInt8                        reserved;                  zb 
   */
  p_int minValue;                                /* w */
  p_int maxValue;                                /* w */
  p_int pageSize;                                /* w */
  p_int value;                                   /* w */
  /*
   * p_int                          reserved2;                 zw 
   */
  /*
   * MemPtr                       activeSliderP;     zl 
   */
}
RCSliderControlBA32Type;

#define szRCSliderControlBA32 "w,uuuuuu3,uuuzu5,w4,l,b,zb,w,w,w,w,zw,zl"

typedef union _rcSliderControlType
{
  RCSliderControlBA16Type s16;
  RCSliderControlBA32Type s32;
}
RCSliderControlType;

/*
 * ------------------ ScrollBar ------------------ 
 */
typedef struct _rcscrollbarattr
{
  p_int usable;                                  /* t */
  p_int visible;                                 /* t */
  p_int hilighted;                               /* t */
  p_int shown;                                   /* t */
  p_int activeRegion;                            /* t4 */
  /*
   * p_int reserved;                   zb 
   */
}
RCSCROLLBARATTR;

typedef struct _rcscrollbar
{
  RCRECT bounds;                                 /* w4 */
  p_int id;                                      /* w */
  RCSCROLLBARATTR attr;                          /* ttttt4,zb */
  p_int value;                                   /* w */
  p_int minValue;                                /* w */
  p_int maxValue;                                /* w */
  p_int pageSize;                                /* w */
  /*
   * Short penPosInCar;
 *//*
 * * * * * * * * * zw 
 */
  /*
   * Short savePos;
 *//*
 * * * * * * * * * zw 
 */
}
RCSCROLLBAR;

#define szRCSCROLLBAR "w4,w,ttttt4,zb,w,w,w,w,zw,zw"

/*
 * ------------------ Forms ------------------ 
 */
typedef union _RCFORMOBJECT
{
  void *ptr;
  RCFieldType *field;
  RCControlType *control;
  RCSliderControlType *slider;
  RCListType *list;
  RCTableType *table;
  RCFormBitMapType *bitmap;
  RCFormLabelType *label;

  /*
   * RCFORMLINE *                         line; 
   */

  /*
   * RCFORMFRAME *                        frame; 
   */

  /*
   * RCFORMRECTANGLE *            rectangle; 
   */
  RCFORMTITLE *title;
  RCFORMPOPUP *popup;
  RCFORMGRAFFITISTATE *grfState;
  RCFORMGADGET *gadget;
  RCSCROLLBAR *scrollbar;
}
RCFORMOBJECT;

typedef struct _rcFormObjListBA16Type
{
  p_int objectType;                              /* b  */
  /*
   * p_int reserved;                                   zb 
   */
  union
  {
    RCFORMOBJECT object;                         /* l */
    p_int ibobj;
  }
  u;
}
RCFormObjListBA16Type;

#define szRCFormObjListBA16 "b,zb,l"

typedef struct _rcFormObjListBA32Type
{
  union
  {
    RCFORMOBJECT object;                         /* l */
    p_int ibobj;
  }
  u;
  p_int objectType;                              /* b */
  /*
   * p_int reserved;                                   zb 
   */
  /*
   * p_int padding;                            zw 
   */
}
RCFormObjListBA32Type;

#define szRCFormObjListBA32 "l,b,zb,zw"

#define szRCFORMOBJLIST (vfLE32?szRCFormObjListBA32:szRCFormObjListBA16)
typedef union _RCFORMOBJLIST
{
  RCFormObjListBA16Type s16;
  RCFormObjListBA32Type s32;
}
RCFORMOBJLIST;

typedef struct _rcformattr
{
  p_int usable;                                  /* u */
  p_int enabled;                                 /* u */
  p_int visible;                                 /* u */
  p_int dirty;                                   /* u */
  p_int saveBehind;                              /* u */
  p_int graffitiShift;                           /* u */
  p_int globalsAvailable;                        /* u */
  p_int doingDialog;                             /* u */
  p_int exitDialog;                              /* u */
  /*
   * p_int reserved;                           zu7 
   */
  /*
   * p_int reserved2;                  zw 
   */
}
RCFORMATTR;                                      /* uuuuuuuu,uzu7,zw */

typedef struct _rcFormBA16Type
{
  RCWindowBA16Type window;
  p_int formId;                                  /* w  */
  RCFORMATTR attr;                               /* uuuuuuuu uzu7 zw */
  /*
   * WinHandle bitsBehindForm;               zl 
   */
  /*
   * FormEventHandlerPtr handler;            zl 
   */
  /*
   * Word focus;                                             zw 
   */
  p_int defaultButton;                           /* w  */
  p_int helpRscId;                               /* w  */
  p_int menuRscId;                               /* w  */
  p_int numObjects;                              /* w  */
  /*
   * RCFormObjListBA16Type *objects;     zl 
   */
}
RCFormBA16Type;

#define szRCFormBA16 szRCWindowBA16 ",w,uuuuuuuuuzu7,zw,zl,zl,zw,w,w,w,w,zl"
typedef struct _rcFormBA32Type
{
  RCWindowBA32Type window;
  RCFORMATTR attr;                               /* uuuuuuuu uzu7 zw */
  /*
   * WinHandle bitsBehindForm;               zl 
   */
  /*
   * FormEventHandlerPtr handler;            zl 
   */
  p_int formId;                                  /* w  */
  /*
   * Word focus;                                             zw 
   */
  p_int defaultButton;                           /* w  */
  p_int helpRscId;                               /* w  */
  p_int menuRscId;                               /* w  */
  p_int numObjects;                              /* w  */
  /*
   * RCFormObjListBA32Type *objects;         zl 
   */
}
RCFormBA32Type;

#define szRCFormBA32 szRCWindowBA32 ",uuuuuuuuuzu7,zw,zl,zl,w,zw,w,w,w,w,zl"

#define szRCFORM (vfLE32?szRCFormBA32:szRCFormBA16)

/*-----------------------------------------------------------------------------
|	MENUS
-------------------------------------------------------------WESC------------*/

typedef struct _rcmenuitem
{
  p_int id;                                      /* w */
  p_int command;                                 /* b */
  p_int hidden;                                  /* t */
  /*
   * p_int reserved;         zt7 
   */
  char *itemStr;                                 /* l */
}
RCMENUITEM;

#define szRCMENUITEM "w,b,tzt7,l"

typedef struct _rcMenuPullDownBA16Type
{
  /*
   * WinHandle menuWin;       zl 
   */
  RCRECT bounds;                                 /* w4 */
  /*
   * WinHandle bitsBehind;    zl 
   */
  RCRECT titleBounds;                            /* w4 */
  char *title;                                   /* l */
  p_int hidden;                                  /* u */
  p_int numItems;                                /* u15 */
  RCMENUITEM *items;                             /* l */
}
RCMenuPullDownBA16Type;

#define szRCMenuPullDownBA16 "zl,w4,zl,w4,l,uu15,l"

typedef struct _rcMenuPullDownBA32Type
{
  /*
   * WinHandle menuWin;       zl 
   */
  RCRECT bounds;                                 /* w4 */
  /*
   * WinHandle bitsBehind;    zl 
   */
  RCRECT titleBounds;                            /* w4 */
  char *title;                                   /* l  */
  p_int hidden;                                  /* u  */
  /*
   * p_int reserved                         zu15 
   */
  p_int numItems;                                /*     w  */
  RCMENUITEM *items;                             /* l  */
}
RCMenuPullDownBA32Type;

#define szRCMenuPullDownBA32 "zl,w4,zl,w4,l,uzu15,w,l"

#define szRCMENUPULLDOWN (vfLE32?szRCMenuPullDownBA32:szRCMenuPullDownBA16)
typedef union _RCMENUPULLDOWN
{
  RCMenuPullDownBA16Type s16;
  RCMenuPullDownBA32Type s32;
}
RCMENUPULLDOWN;

typedef struct _RCMENUBARATTR
{
  p_int visible;                                 /* u  */
  /*
   * WORD commandPending;    zu 
   */
  /*
   * WORD insPtEnabled;      zu 
   */
  /*
   * p_int needsRecalc;                zu 
   */
  /*
   * p_int reserved;                           zu12 
   */
}
RCMENUBARATTR;

typedef struct _rcMenuBarBA16Type
{
  /*
   * WinHandle barWin;                               zl 
   */
  /*
   * WinHandle bitsBehind;                           zl 
   */
  /*
   * WinHandle savedActiveWin;               zl 
   */
  /*
   * WinHandle bitsBehindStatus;     zl 
   */
  RCMENUBARATTR attr;                            /* uzu3zu12 */
  /*
   * SWord curMenu;                                  zw 
   */
  p_int curItem;                                 /* w  */
  /*
   * SDWord commandTick;                     zl 
   */
  p_int numMenus;                                /* w   number of menus */
  /*
   * MenuPullDownPtr menus;                  zl  array of menus 
   */
}
RCMenuBarBA16Type;

#define szRCMenuBarBA16 "zl,zl,zl,zl,uzu3zu12,zw,w,zl,w,zl"
typedef struct _rcMenuBarBA32Type
{
  /*
   * WinHandle barWin;                               zl 
   */
  /*
   * WinHandle bitsBehind;                           zl 
   */
  /*
   * WinHandle savedActiveWin;               zl 
   */
  /*
   * WinHandle bitsBehindStatus;     zl 
   */
  RCMENUBARATTR attr;                            /* uzu3zu12 */
  /*
   * SWord curMenu;                                  zw 
   */
  p_int curItem;                                 /* w  */
  p_int numMenus;                                /* w   number of menus */
  /*
   * SDWord commandTick;                     zl 
   */
  /*
   * MenuPullDownPtr menus;                  zl  array of menus 
   */
}
RCMenuBarBA32Type;

#define szRCMenuBarBA32 "zl,zl,zl,zl,uzu3zu12,zw,w,w,zl,zl"

#define szRCMENUBAR (vfLE32?szRCMenuBarBA32:szRCMenuBarBA16)
typedef union _RCMENUBAR
{
  RCMenuBarBA16Type s16;
  RCMenuBarBA32Type s32;
}
RCMENUBAR;

/*-----------------------------------------------------------------------------
|	ALERTS
-------------------------------------------------------------WESC------------*/

typedef struct _rcALERTTEMPLATE
{
  p_int alertType;                               /* w */
  p_int helpRscID;                               /* w */
  p_int numButtons;                              /* w */
  p_int defaultButton;                           /* w */
}
RCALERTTEMPLATE;

#define szRCALERTTEMPLATE "w,w,w,w"

/*-----------------------------------------------------------------------------
|	BITMAP
-------------------------------------------------------------WESC------------*/
typedef struct _rcBitmapFlagsType
{
  p_int compressed;                              /* u *//* Data format:  0=raw; 1=compressed */
  p_int hasColorTable;                           /* u *//* if true, color table stored before bits[] */
  p_int hasTransparency;                         /* u *//* true if transparency is used */
  p_int indirect;                                /* u *//* true if bits are stored indirectly */
  /*
   * Never set this flag. Only the display (screen) bitmap has the indirect bit set. 
   */
  p_int forScreen;                               /* u *//* system use only */
  p_int directColor;                             /* u  *//* direct color bitmap */
  //p_int reserved;                 /* zu10 */ 
}
RCBitmapFlagsType;

typedef struct _rcBITMAP
{                                                /* bm */
  p_int cx;                                      /* w */
  p_int cy;                                      /* w */
  p_int cbRow;                                   /* w */
  RCBitmapFlagsType flags;                       /* uuuuuuzu10 *//* RMa struct updated */
  p_int pixelsize;                               /* b */
  p_int version;                                 /* b */
  p_int nextDepthOffset;                         /* w */
  p_int transparentIndex;                        /* b */
  p_int compressionType;                         /* b */
  /*
   * ushort and_reserved_and_colorTable[3] 
 *//*
 * * * * * * * * * z1w 
 */
  unsigned char *pbBits;

  /*
   * private, not stored into file 
   */
  int cbDst;
}
RCBITMAP;

#define szRCBITMAP "w,w,w,uuuuuuzu10,b,b,w,b,b,zw"

/*-----------------------------------------------------------------------------
|	FONT
-------------------------------------------------------------RMa------------*/
typedef struct _rcFONTCHARINFO
{
  p_int offset;                                  /* b */
  p_int width;                                   /* b */
}
RCFONTCHARINFOTYPE,
 *RCFONTCHARINFOTYPEPTR;

#define szRCFONTCHARINFO "b,b"

typedef struct _rcFONT
{
  p_int fontType;                                /* w */// font type
  p_int firstChar;                               /* w */// ASCII code of first character
  p_int lastChar;                                /* w */// ASCII code of last character
  p_int maxWidth;                                /* w */// maximum character width
  p_int kernMax;                                 /* w */// negative of maximum character kern
  p_int nDescent;                                /* w */// negative of descent
  p_int fRectWidth;                              /* w */// width of font rectangle
  p_int fRectHeight;                             /* w */// height of font rectangle
  p_int owTLoc;                                  /* w */// offset to offset/width table
  p_int ascent;                                  /* w */// ascent
  p_int descent;                                 /* w */// descent
  p_int leading;                                 /* w */// leading
  p_int rowWords;                                /* w */// row width of bit image / 2
}
RCFONTTYPE;

#define szRCFONT "w,w,w,w,w,w,w,w,w,w,w,w"

/*-----------------------------------------------------------------------------
|	Graffiti Input Area
-------------------------------------------------------------RMa------------*/

typedef struct _rcSilkAreaBA16Type
{
  RCRECT bounds;                                 /* w4 */
  p_int areaType;                                /* l */
  p_int areaIndex;                               /* w */
}
RCSilkAreaBA16Type;

#define szRCSilkAreaBA16EmitStr "w4,l,w"
typedef struct _rcSilkAreaBA32Type
{
  RCRECT bounds;                                 /* w4 */
  p_int areaType;                                /* l */
  p_int areaIndex;                               /* w */
}
RCSilkAreaBA32Type;

#define szRCSilkAreaBA32EmitStr "w4,l,w,zw"

#define szRCSILKAREA (vfLE32?szRCSilkAreaBA32EmitStr:szRCSilkAreaBA16EmitStr)
typedef union _RCSILKAREA
{
  RCSilkAreaBA16Type s16;
  RCSilkAreaBA32Type s32;
}
RCSILKAREA;

typedef struct _rcSilkButtonBA16Type
{
  //      p_int             buttonCount;            /* w */
  RCRECT bounds;                                 /* w4 */
  p_int keyDownChr;                              /* w */
  p_int keyDownKeyCode;                          /* w */
  p_int keyDownModifiers;                        /* w */
}
RCSilkButtonBA16Type;

#define szRCSilkButtonBA16EmitStr "w4,w,w,w"
typedef struct _rcSilkButtonBA32Type
{
  //      p_int             buttonCount;            /* w */
  RCRECT bounds;                                 /* w4 */
  p_int keyDownChr;                              /* w */
  p_int keyDownKeyCode;                          /* w */
  p_int keyDownModifiers;                        /* w */
}
RCSilkButtonBA32Type;

#define szRCSilkButtonBA32EmitStr "w4,w,w,w,zw"

#define szRCSILKBUTTON (vfLE32?szRCSilkButtonBA32EmitStr:szRCSilkButtonBA16EmitStr)
typedef union _RCSILKBUTTON
{
  RCSilkButtonBA16Type s16;
  RCSilkButtonBA32Type s32;
}
RCSILKBUTTON;

typedef struct _rcSilkBA16Type
{
  p_int version;                                 /* w */
  p_int vendorCreator;                           /* l */
  p_int localeLanguage;                          /* w */
  p_int localeCountry;                           /* w */
  p_int areaCount;                               /* w */
}
RCSilkBA16Type;

#define szRCSilkBA16EmitStr "w,l,w,w,w"
typedef struct _rcSilkBA32Type
{
  p_int version;                                 /* w */
  p_int areaCount;                               /* w */
  p_int vendorCreator;                           /* l */
  p_int localeLanguage;                          /* w */
  p_int localeCountry;                           /* w */
}
RCSilkBA32Type;

#define szRCSilkBA32EmitStr "w,w,l,w,w"

#define szRCSILK (vfLE32?szRCSilkBA32EmitStr:szRCSilkBA16EmitStr)
typedef union _RCSILK
{
  RCSilkBA16Type s16;
  RCSilkBA32Type s32;
}
RCSILK;

/*-----------------------------------------------------------------------------
|	cnty Contry
|	An array of these structures (one per country) is kept in the system
|	resource.
|	Preferences.h, DateTime.h, Localize.h, 
-------------------------------------------------------------RMa------------*/
#define countryNameLength		20
#define currencyNameLength		20
#define currencySymbolLength	6

typedef struct CountryPreferencesBA16Type
{
  p_int country;                                 // Country the structure represents
  //      p_int             filler1;                                                                                                        // (Word alignment)
  p_int countryName[countryNameLength];
  p_int dateFormat;                              // Format to display date in
  p_int longDateFormat;                          // Format to display date in
  p_int weekStartDay;                            // Sunday or Monday
  p_int timeFormat;                              // Format to display time in
  p_int numberFormat;                            // Format to display numbers in
  //      p_int             filler2;                                                                                                        // (Word alignment)
  p_int currencyName[currencyNameLength];        // Dollars
  p_int currencySymbol[currencySymbolLength];    // $
  p_int uniqueCurrencySymbol[currencySymbolLength];     // US$
  p_int currencyDecimalPlaces;                   // 2 for 1.00
  p_int daylightSavings;                         // Type of daylight savings correction
  p_int minutesWestOfGMT;                        // minutes west of Greenwich
  p_int measurementSystem;                       // metric, english, etc.
  //      p_int             filler3;                                                                                                        // (Word alignment)
}
CountryPreferencesBA16Type;

#define szRCCountryBA16EmitStr "b,zb,b20,b,b,b,b,b,zb,b20,b6,b6,b,b,l,b,zb"

typedef struct CountryPreferencesBA32Type
{
  p_int country;                                 // Country the structure represents - 1 byte, 0
  p_int dateFormat;                              // Format to display date in - 1 byte, 1
  p_int longDateFormat;                          // Format to display date in - 1 byte, 2
  p_int weekStartDay;                            // Sunday or Monday - 1 byte, 3
  p_int countryName[countryNameLength];          // 20 bytes, 4
  p_int timeFormat;                              // Format to display time in - 1 byte, 24
  p_int numberFormat;                            // Format to display numbers in - 1 byte, 25
  p_int daylightSavings;                         // Type of daylight savings correction - 1 byte, 26
  p_int measurementSystem;                       // metric, english, etc. - 1 byte, 27
  p_int currencyName[currencyNameLength];        // Dollars - 20 bytes, 28
  p_int currencySymbol[currencySymbolLength];    // $ - 6 bytes, 48
  p_int uniqueCurrencySymbol[currencySymbolLength];     // US$ - 6 bytes, 54
  p_int minutesWestOfGMT;                        // minutes west of Greenwich - 4 bytes, 60
  p_int currencyDecimalPlaces;                   // 2 for 1.00 - 1 byte, 61
  //      p_int             reserved1;                                                                                              // padding
  //      p_int             reserved2;                                                                                              // padding
}
CountryPreferencesBA32Type;

#define szRCCountryBA32EmitStr "b,b,b,b,b20,b,b,b,b,b20,b6,b6,l,b,zb,zw"

#define szRCCOUNTRY (vfLE32?szRCCountryBA32EmitStr:szRCCountryBA16EmitStr)
typedef union _RCCOUNTRY
{
  CountryPreferencesBA16Type s16;
  CountryPreferencesBA32Type s32;
}
RCCOUNTRY;

/*-----------------------------------------------------------------------------
|	feat feature
-------------------------------------------------------------RMa------------*/
typedef struct ROMFtrFeatureBA16Type
{
  p_int num;                                     // feature number
  p_int value;                                   // feature value
}
ROMFtrFeatureBA16Type;

#define szRCFeatureBA16EmitStr "w,l"

typedef struct ROMFtrFeatureBA32Type
{
  p_int num;                                     // feature number
  //      p_int             reserved;
  p_int value;                                   // feature value
}
ROMFtrFeatureBA32Type;

#define szRCFeatureBA32EmitStr "w,zw,l"

#define szRCFEATUREFEATURE (vfLE32?szRCFeatureBA32EmitStr:szRCFeatureBA16EmitStr)
typedef union _RCFEATUREFEATURE
{
  ROMFtrFeatureBA16Type s16;
  ROMFtrFeatureBA32Type s32;
}
RCFEATUREFEATURE;

typedef struct ROMFtrCreatorBA16Type
{
  p_int creator;                                 // feature creator
  p_int numEntries;                              // # of entries
  //      ROMFtrFeatureBA16Type   feature[1];                     // variable size array of Features
}
ROMFtrCreatorBA16Type;

#define szRCFeatureCreatorBA16EmitStr "l,w"

typedef struct ROMFtrCreatorBA32Type
{
  p_int creator;                                 // feature creator
  p_int numEntries;                              // # of entries
  //      p_int                                             reserved;
  //      ROMFtrFeatureBA32Type   feature[1];                     // variable size array of Features
}
ROMFtrCreatorBA32Type;

#define szRCFeatureCreatorBA32EmitStr "l,w,zw"

#define szRCFEATURECREATOR (vfLE32?szRCFeatureCreatorBA32EmitStr:szRCFeatureCreatorBA16EmitStr)
typedef union _RCFEATURECREATOR
{
  ROMFtrCreatorBA16Type s16;
  ROMFtrCreatorBA32Type s32;
}
RCFEATURECREATOR;

typedef struct ROMFtrTableBA16Type
{
  p_int numEntries;                              // # of entries
  //      ROMFtrCreatorBA16Type   creator[1];                     // var. size array of Creators
}
ROMFtrTableBA16Type;

#define szRCFeatureTableBA16EmitStr "w"

typedef struct ROMFtrTableBA32Type
{
  p_int numEntries;                              // # of entries
  //      p_int                                             reserved;
  //      ROMFtrCreatorBA32Type   creator[1];                     // var. size array of Creators
}
ROMFtrTableBA32Type;

#define szRCFeatureTableBA32EmitStr "w,zw"

#define szRCFEATURE (vfLE32?szRCFeatureTableBA32EmitStr:szRCFeatureTableBA16EmitStr)
typedef union _RCFEATURE
{
  ROMFtrTableBA16Type s16;
  ROMFtrTableBA32Type s32;
}
RCFEATURE;

/*-----------------------------------------------------------------------------
|	KEYBOARD
-------------------------------------------------------------RMa------------*/
#define keyboardRows		4
#define numKeyboards		3                // alpha, punc/number, and p_int'l

typedef struct _KeyboardKeyBA16Type
{
  p_int shiftedKey;                              // key to use if shift is on
  p_int capsKey;                                 // key to use if caps lock is on
  p_int unshiftedKey;
  p_int width;                                   // the width of the key.  The text is drawn centered.
  p_int labelOffset;                             // if there is a label display it instead
  // of a key (all modes)
}
KeyboardKeyType;

#define szRCKeyboardKeyBA16EmitStr "b,b,b,b,w"
#define szRCKeyboardKeyBA32EmitStr "b,b,b,b,l"

#define szRCKEYBOARDKEY (vfLE32?szRCKeyboardKeyBA32EmitStr:szRCKeyboardKeyBA16EmitStr)

typedef struct _keyboardlayoutBA16Type
{
  RCRECT bounds;
  p_int rowHeight;
  p_int font;
  p_int keys;
  p_int keysPerRow[keyboardRows];
  p_int shiftKey;
  p_int capsKey;
  p_int lastLayoutInKeyboard;

  /*
   * p_int                          reserved 
   */
}
KeyboardLayoutType;

#define szRCKeyboardLayoutBA16EmitStr "w4,b,b,w,b4,b,b,b,zb"
#define szRCKeyboardLayoutBA32EmitStr "w4,b,b,w,b4,b,b,b,zb"

#define szRCKEYBOARDLAYOUT (vfLE32?szRCKeyboardLayoutBA32EmitStr:szRCKeyboardLayoutBA16EmitStr)

/*-----------------------------------------------------------------------------
These macros are used to manipulate PalmOS Objects without the need of taking
care of byte ordering.
Depending on the value of vfLE32, it uses the appropriate struct in the union
defining each object.
Using these macros allows to avoid to test vfLE32 and write the code twice...
-------------------------------------------------------------RNi-------------*/
#define BAFIELD(obj, field) (vfLE32 ? obj.s32.field : obj.s16.field)
#define PBAFIELD(pobj, field) (vfLE32 ? pobj->s32.field : pobj->s16.field)
#define SETBAFIELD(obj, field, value) do { if (vfLE32) obj.s32.field = (value); else obj.s16.field = (value); } while (0)
#define SETPBAFIELD(pobj, field, value) do { if (vfLE32) pobj->s32.field = (value); else pobj->s16.field = (value); } while (0)
#define BAFIELD16(obj, field) obj.s16.field
#define BAFIELD32(obj, field) obj.s32.field
#define PBAFIELD16(obj, field) obj->s16.field
#define PBAFIELD32(obj, field) obj->s32.field

/*-----------------------------------------------------------------------------
|	Other PILRC types and such
-------------------------------------------------------------WESC------------*/

//#define ifrmMax 32
DEFPL(PLEXFORMOBJLIST)
     typedef struct _FRMBA16
     {
       RCFormBA16Type form;
       PLEXFORMOBJLIST pllt;
       //      RCFORMOBJLIST *rglt;
     }
FRMBA16Type;
     typedef struct _FRMBA32
     {
       RCFormBA32Type form;
       PLEXFORMOBJLIST pllt;
       //      RCFORMOBJLIST *rglt;
     }
FRMBA32Type;

     typedef union _FRM
     {
       FRMBA16Type s16;
       FRMBA32Type s32;
     }
FRM;

     void ErrorLine(char *sz);

/*
 * Translation Entry -- used to map to foreign languages 
 */
     typedef struct _te
     {
       char *szOrig;
       char *szTrans;
       struct _te *pteNext;
     }
TE;

/*
 * Symbol Table 
 */
     typedef struct _sym
     {
       char *sz;
       int wVal;
       BOOL fAutoId;
       struct _sym *psymNext;
     }
SYM;

/*
 * RCPFILE -- contains everything in a .rcp file 
 */
DEFPL(PLEXFRM)
     typedef struct _rcpfile
     {
       PLEXFRM plfrm;
     }
RCPFILE;
     void FreeRcpfile(RCPFILE * prcpfile);

/*
 * ReservedWord 
 */
     typedef enum
     {
       rwFLD, rwLST, rwTBL, rwFBM, rwLBL, rwTTL, rwPUL, rwGSI, rwGDT,
       rwBTN, rwPBN, rwCBX, rwPUT, rwSLT, rwREP, rwSLD, rwSCL,

       rwForm, rwBegin, rwEnd, rwModal, rwSaveBehind, rwNoSaveBehind,
       rwHelpId, rwDefaultBtnId, rwMenuId,
       rwEnabled, rwDisabled, rwUsable, rwNonUsable, rwLeftAnchor,
       rwRightAnchor, rwGroup, rwFont,
       rwFrame, rwNoFrame, rwBoldFrame, rwVertical, rwGraphical,

       rwEditable, rwNonEditable, rwUnderlined, rwSingleLine,
       rwMultipleLines, rwDynamicSize, rwLeftAlign, rwHasScrollBar,
       rwRightAlign, rwMaxChars,
       rwVisibleItems, rwAutoShift, rwNumeric,
       rwChecked,

       rwBitmap,
       rwBitmapGrey,
       rwBitmapGrey16,
       rwBitmapColor16,
       rwBitmapColor256,
       rwBitmapColor16k,
       rwBitmapColor24k,
       rwBitmapColor32k,
       rwBitmapFamily,
       rwBitmapFamily_special,
       rwInteger,
       rwWordList,
       rwFontIndex,
       rwPaletteTable,
       rwGraffitiInputArea,
       rwCreator,
       rwLanguage,
       rwCountry,
       rwArea,
       rwScreen,
       rwGraffiti,
       rwAreaIndex,
       rwKeyDownChr,
       rwKeyDownKeyCode,
       rwKeyDownModifiers,
       rwCountryLocalisation,
       rwNumber,
       rwName,
       rwDateFormat,
       rwLongDateFormat,
       rwWeekStartDay,
       rwTimeFormat,
       rwNumberFormat,
       rwCurrencyName,
       rwCurrencySymbol,
       rwCurrencyUniqueSymbol,
       rwCurrencyDecimalPlaces,
       rwDaylightSavings,
       rwMinutesWestOfGmt,
       rwMeasurementSystem,
       rwFeature,
       rwEntry,
       rwKeyboard,
       rwLongWordList,
       rwByteList,
       rwDefaultItem,
       rwMidi,
       rwBootScreenFamily,
       rwPrevLeft, rwPrevRight, rwPrevWidth, rwPrevTop, rwPrevBottom,
       rwPrevHeight,
       rwMenu,
       rwPullDown,
       rwMenuItem,
       rwSeparator,

       rwValue,
       rwMinValue,
       rwMaxValue,
       rwPageSize,

       rwFeedback,
       rwThumbID,
       rwBackgroundID,
       rwBitmapID,
       rwSelectedBitmapID,

       rwAlert,
       rwMessage,
       rwButtons,
       rwDefaultBtn,

       rwInformation,                            /* order assumed */
       rwConfirmation,
       rwWarning,
       rwError,

       rwVersion,
       rwString,
       rwStringTable,
       rwFile,
       rwApplicationIconName,
       rwApplication,
       rwLauncherCategory,                       /* RMa add: application default launcher category */
       rwCategories,

       rwTranslation,

       rwCenter,
       rwRight,
       rwBottom,
       rwAuto,
       rwAt,
       rwId,
       rwAutoId,

       rwNumColumns,
       rwNumRows,
       rwColumnWidths,

       rwInclude,
       rwDefine,
       rwEqu,
       rwIfdef,
       rwIfndef,
       rwElse,
       rwEndif,
       rwExtern,
       rwIcon,
       //      rwIconGrey,
       //      rwIconGrey16,
       //      rwIconColor,     
       // Aaron Ardiri 
       // - removed these due to be purely backward 
       //   compatable in the applications launcher
       rwIconFamily,

       rwIconSmall,
       //      rwIconSmallGrey,
       //      rwIconSmallGrey16,
       //      rwIconSmallColor,
       // Aaron Ardiri 
       // - removed these due to be purely backward 
       //   compatable in the applications launcher
       rwIconSmallFamily,

       rwTrap,

       rwFontId,

       rwHex,
       rwData,

       rwTransparency,
       rwTransparencyIndex,
       rwNoColorTable,
       rwColorTable,

       rwNoCompress,
       rwAutoCompress,
       rwForceCompress,

       rwPublic,
       rwShort,
       rwInt,
       rwStatic,
       rwFinal,

       rwNil
     }
RW;

     typedef struct _rwt
     {
       char *sz1;
       char *sz2;
       RW rw;
     }
RWT;

/*
 * could just use index into this table as rw but that is kinda fragile. 
 */
#ifdef EMITRWT
     RWT rgrwt[] = {
       {"tFLD", "field", rwFLD},
       {"tLST", "list", rwLST},                  /* List */
       {"tTBL", "table", rwTBL},                 /* Table */
       {"tFBM", "formbitmap", rwFBM},            /* Form Bitmap */
       {"tLBL", "label", rwLBL},                 /* Label */
       {"tTTL", "title", rwTTL},                 /* Title                */
       {"tPUL", "popuplist", rwPUL},
       {"tGSI", "graffitistateindicator", rwGSI},       /* Graffiti State */
       {"tGDT", "gadget", rwGDT},                /* Gadget */
       {"tSCL", "scrollbar", rwSCL},             /* Scrollbar */

       {"tBTN", "button", rwBTN},
       {"tPBN", "pushbutton", rwPBN},
       {"tCBX", "checkbox", rwCBX},              /*  */
       {"tPUT", "popuptrigger", rwPUT},          /* Popup trigger */
       {"tSLT", "selectortrigger", rwSLT},
       {"tREP", "repeatbutton", rwREP},
       {"tSLD", "slider", rwSLD},                /* RMa add */
       {"form", "tFRM", rwForm},
       {"begin", NULL, rwBegin},
       {"end", NULL, rwEnd},
       {"modal", NULL, rwModal},
       {"savebehind", NULL, rwSaveBehind},
       {"nosavebehind", NULL, rwNoSaveBehind},
       {"helpid", NULL, rwHelpId},
       {"defaultbtnid", NULL, rwDefaultBtnId},
       {"menuid", NULL, rwMenuId},

       {"enabled", NULL, rwEnabled},
       {"disabled", NULL, rwDisabled},
       {"usable", NULL, rwUsable},
       {"nonusable", NULL, rwNonUsable},
       {"leftanchor", NULL, rwLeftAnchor},
       {"rightanchor", NULL, rwRightAnchor},
       {"group", NULL, rwGroup},
       {"font", NULL, rwFont},

       {"frame", NULL, rwFrame},
       {"noframe", NULL, rwNoFrame},
       {"boldframe", NULL, rwBoldFrame},

       {"vertical", NULL, rwVertical},
       {"graphical", NULL, rwGraphical},
       {"editable", NULL, rwEditable},
       {"noneditable", NULL, rwNonEditable},
       {"underlined", NULL, rwUnderlined},
       {"singleline", NULL, rwSingleLine},
       {"multiplelines", "multipleline", rwMultipleLines},
       {"dynamicsize", NULL, rwDynamicSize},
       {"leftalign", NULL, rwLeftAlign},
       {"hasscrollbar", NULL, rwHasScrollBar},
       {"rightalign", NULL, rwRightAlign},
       {"maxchars", NULL, rwMaxChars},
       {"autoshift", NULL, rwAutoShift},
       {"numeric", NULL, rwNumeric},

       {"visibleitems", NULL, rwVisibleItems},

       {"value", NULL, rwValue},
       {"min", "minvalue", rwMinValue},
       {"max", "maxvalue", rwMaxValue},
       {"pagesize", NULL, rwPageSize},

       {"feedback", NULL, rwFeedback},
       {"thumbid", NULL, rwThumbID},
       {"backgroundid", NULL, rwBackgroundID},
       {"bitmapid", NULL, rwBitmapID},
       {"selectedbitmapid", NULL, rwSelectedBitmapID},

       {"checked", "on", rwChecked},
       {"bitmap", NULL, rwBitmap},
       {"bitmapgrey", "bitmapgray", rwBitmapGrey},
       {"bitmapgrey16", "bitmapgray16", rwBitmapGrey16},
       {"bitmapcolor16", "bitmapcolour16", rwBitmapColor16},
       {"bitmapcolor", "bitmapcolour", rwBitmapColor256},

       {"bitmapcolor16k", "bitmapcolour16k", rwBitmapColor16k},
       {"bitmapcolor24k", "bitmapcolour24k", rwBitmapColor24k},
       {"bitmapcolor32k", "bitmapcolour32k", rwBitmapColor32k},
       {"bitmapfamily", NULL, rwBitmapFamily},
       {"bitmapfamilyspecial", NULL, rwBitmapFamily_special},
       {"integer", NULL, rwInteger},
       {"wordlist", NULL, rwWordList},
       {"fontindex", NULL, rwFontIndex},
       {"palettetable", NULL, rwPaletteTable},
       {"graffitiinputarea", NULL, rwGraffitiInputArea},        /* 'silk' */
       {"creator", NULL, rwCreator},
       {"language", NULL, rwLanguage},
       {"country", NULL, rwCountry},
       {"area", NULL, rwArea},
       {"screen", NULL, rwScreen},
       {"graffiti", NULL, rwGraffiti},
       {"index", NULL, rwAreaIndex},
       {"keydownchr", NULL, rwKeyDownChr},
       {"keydownkeycode", NULL, rwKeyDownKeyCode},
       {"keydownmodifiers", NULL, rwKeyDownModifiers},
       {"countrylocalisation", NULL, rwCountryLocalisation},    /* 'cnty' */
       {"number", NULL, rwNumber},
       {"name", NULL, rwName},
       {"dateformat", NULL, rwDateFormat},
       {"longdateformat", NULL, rwLongDateFormat},
       {"weekstartday", NULL, rwWeekStartDay},
       {"timeformat", NULL, rwTimeFormat},
       {"numberformat", NULL, rwNumberFormat},
       {"currencyname", NULL, rwCurrencyName},
       {"currencysymbol", NULL, rwCurrencySymbol},
       {"currencyuniquesymbol", NULL, rwCurrencyUniqueSymbol},
       {"currencydecimalplaces", NULL, rwCurrencyDecimalPlaces},
       {"daylightsavings", NULL, rwDaylightSavings},
       {"minuteswestofgmt", NULL, rwMinutesWestOfGmt},
       {"measurementsystem", NULL, rwMeasurementSystem},
       {"feature", NULL, rwFeature},             /* 'feat' */
       {"entry", NULL, rwEntry},
       {"keyboard", NULL, rwKeyboard},           /* 'tkbd' */
       {"longwordlist", NULL, rwLongWordList},   /* 'DLST' */
       {"bytelist", NULL, rwByteList},           /* 'BLST' */
       {"defaultitem", NULL, rwDefaultItem},     /* 'DLST' & 'BLST' */
       {"midi", NULL, rwMidi},                   /* 'MIDI' */
       {"bootscreenfamily", NULL, rwBootScreenFamily},  /* 'tbsb' it look like as a bitmapfamily with an header (size & crc) */

       {"prevleft", NULL, rwPrevLeft},
       {"prevright", NULL, rwPrevRight},
       {"prevwidth", NULL, rwPrevWidth},

       {"prevtop", NULL, rwPrevTop},
       {"prevbottom", NULL, rwPrevBottom},
       {"prevheight", NULL, rwPrevHeight},

       {"menu", "MBAR", rwMenu},
       {"pulldown", NULL, rwPullDown},
       {"menuitem", NULL, rwMenuItem},
       {"separator", NULL, rwSeparator},

       {"alert", "tALT", rwAlert},
       {"message", NULL, rwMessage},
       {"defaultbutton", NULL, rwDefaultBtn},
       {"buttons", NULL, rwButtons},
       {"information", NULL, rwInformation},
       {"confirmation", NULL, rwConfirmation},
       {"warning", NULL, rwWarning},
       {"error", NULL, rwError},

       {"version", "tVER", rwVersion},

       {"stringtable", "tSTL", rwStringTable},
       {"string", "tSTR", rwString},
       {"file", "tSTR", rwFile},
       {"applicationiconname", NULL, rwApplicationIconName},
       {"application", "APPL", rwApplication},
       {"launchercategory", "taic", rwLauncherCategory},
       {"categories", "tAIS", rwCategories},

       {"translation", NULL, rwTranslation},

       {"center", NULL, rwCenter},
       {"right", NULL, rwRight},
       {"bottom", NULL, rwBottom},
       {"auto", NULL, rwAuto},

       {"at", NULL, rwAt},
       {"id", NULL, rwId},
       {"autoid", NULL, rwAutoId},

       {"columns", "numcolumns", rwNumColumns},
       {"rows", "numrows", rwNumRows},
       {"columnwidths", "widths", rwColumnWidths},

       {"define", NULL, rwDefine},
       {"equ", NULL, rwEqu},
       {"include", NULL, rwInclude},
       {"ifdef", NULL, rwIfdef},
       {"ifndef", NULL, rwIfndef},
       {"else", NULL, rwElse},
       {"endif", NULL, rwEndif},
       {"extern", NULL, rwExtern},

       {"icon", NULL, rwIcon},
       //      {"icongrey",     "icongray",     rwIconGrey},
       //      {"icongrey16",   "icongray16",   rwIconGrey16},
       //      {"iconcolor",    "iconcolour",   rwIconColor},
       // Aaron Ardiri 
       // - removed these due to be purely backward 
       //   compatable in the applications launcher

       {"iconfamily", NULL, rwIconFamily},

       {"smallicon", NULL, rwIconSmall},
       //      {"smallicongrey",     "smallicongray",     rwIconSmallGrey},
       //      {"smallicongrey16",   "smallicongray16",   rwIconSmallGrey16},
       //      {"smalliconcolor",    "smalliconcolour",   rwIconSmallColor},
       // Aaron Ardiri 
       // - removed these due to be purely backward 
       //   compatable in the applications launcher

       {"smalliconfamily", NULL, rwIconSmallFamily},

       {"trap", NULL, rwTrap},
       {"fontid", NULL, rwFontId},

       {"hex", NULL, rwHex},
       {"data", NULL, rwData},

       {"transparency", "transparent", rwTransparency},
       {"transparencyIndex", "transparentindex", rwTransparencyIndex},
       {"colortable", "colourtable", rwColorTable},
       {"nocolortable", "nocolourtable", rwNoColorTable},
       {"nocompress", NULL, rwNoCompress},
       {"autocompress", "compress", rwAutoCompress},
       {"forcecompress", NULL, rwForceCompress},

       /*
        * Java specific 
        */
       {"public", NULL, rwPublic},
       {"short", NULL, rwShort},
       {"int", NULL, rwInt},
       {"static", NULL, rwStatic},
       {"final", NULL, rwFinal},
       {NULL, NULL, rwNil},
     };
#endif                                           /* EMITRWT */

/*
 * TOKen 
 */
typedef struct _tok
{
  RW rw;
  LEX lex;
}
TOK;

extern BOOL vfWinGUI;
extern BOOL vfAutoId;
extern BOOL vfQuiet;
extern BOOL vfAllowEditIDs;
extern BOOL vfPalmRez;
extern BOOL vfVSErrors;
extern BOOL vfCheckDupes;
extern BOOL vfAppend;
extern BOOL vfRTL;
extern BOOL vfLE32;

//LDu : Output a Prc file
extern BOOL vfPrc;
extern const char *vfPrcName;
extern const char *vfPrcCreator;
extern const char *vfPrcType;

extern char *szLanguage;

#define dxScreen 160
#define dyScreen 160
#define maxCategories  16
#define categoryLength 16

void CbInit(void);
int CbEmitStruct(void *pv,
                 char *szPic,
                 char **ppchText,
                 BOOL fEmit);
int CbStruct(char *szPic);
RCPFILE *ParseFile(char *szIn,
                   char *szOutDir,
                   char *szResFile,
                   char *szIncFile,
                   int fontType);
SYM *PsymLookupId(int id);
VOID AddSym(char *sz,
            int wVal);

#endif                                           // _pilrc_h
