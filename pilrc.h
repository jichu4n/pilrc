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
 *     20-Nov20000 Renaud Malaval
 *                 additions PalmOS 3.5 support
 *                 additions to support ARM resouces
 */

#include "std.h"
#include "util.h"
#include "lex.h"
#include "plex.h"
#include "font.h"

#ifndef _pilrc_h
#define _pilrc_h		// RMA : multiples include protection

/*
#ifdef ARM				// RMa : original files
#pragma message ("PilRC for ARM")
#else						// RMa : ARM files
#pragma message ("PilRC for 68k")
#endif
*/

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
|	note:  we make the assumption that sizeof(int) == sizeof(void *) -- the emitter
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

Enum(controlStyles) {buttonCtl, pushButtonCtl, checkboxCtl, popupTriggerCtl,
							 selectorTriggerCtl, repeatingButtonCtl,
							 sliderCtl, feedbackSliderCtl	/* RMa add support sliders */
} EndEnum ControlStyles;

Enum(alertTypes) {
	informationAlert,
	confirmationAlert,
	warningAlert,
	errorAlert
} EndEnum AlertType;

Enum(formObjects) {
	frmFieldObj,
	frmControlObj,
	frmListObj,
	frmTableObj,
	frmBitmapObj,
	frmLineObj,
	frmFrameObj,
	frmRectangleObj,
	frmLabelObj,
	frmTitleObj,
	frmPopupObj,
	frmGraffitiStateObj,
	frmGadgetObj,
	frmScrollbarObj,
	frmSliderObj,					/* RMa add support sliders */
	frmGraphicalControlObj,		/* RMa add support graphical control */
} EndEnum FormObjectKind;

Enum(fontID) { stdFont, boldFont, largeFont, symbolFont, symbol11Font, 
				  symbol7Font, ledFont }
EndEnum FontID;

typedef struct _rcpoint
	{
	int x;
	int y;
	} RCPOINT;

typedef struct _Rcrect
	{
	RCPOINT topLeft;
	RCPOINT extent;
	} RCRECT;

/************************************************\
 * RMa update all structure to be compleant with
 *  PalmOS 3.5
 *  ARM ( 16 and 32 bits memory alignment)
\************************************************/
/* window.h */
typedef struct _rcframebits
	{
	int cornerDiam;		/* u8   */
	/* int reserved;	   zu3 */
	int threeD;			/* u   */
	int shadowWidth;	/* u2  */
	int width;			/* u2  */
	} RCFRAMEBITS;		/* u8zu3uu2u2 */

typedef struct _rcwindowflags
	{
    /* Word format:1;      zu */
    /* Word offscreen:1;   zu */
    int modal;			/* u  */
    int focusable;	    /* u  */
    /*	Word enabled:1;    zu */
    /* Word visible:1;     zu  */
    int dialog;    		/* u */
    /* Word freeBitmap:1   zu */
    /* Word reserved :8;   zu8 */
	} RCWINDOWFLAGS;	/* zuzuuu zuzuuzu zu8 */

typedef struct _rcwindowBA16Type
	{
	int displayWidth;					/* w */
	int displayHeight;					/* w */
	/* int displayAddr;					   zl */
	RCWINDOWFLAGS windowFlags;			/* zuzuuu zuzuuzu zu8 */
	RCRECT windowBounds;  				/* w4 */
	/* RCRECT clippingBounds;			   zw4 */
	/* BitmapPtr bitmapP;				   zl */
	RCFRAMEBITS frameType;				/* u8zu3uu2u2 */
	/* GraphicStatePtr drawStateP;		   zl */
	/* struct RCWindowBA16Type *nextWindow; zl */
	} RCWindowBA16Type;
#define szRCWindowBA16 "w,w,zl,zuzuuuzuzuuzuzu8,w4,zw4,zl,u8zu3uu2u2,zl,zl"
typedef struct _rcwindowBA32Type
	{
	int displayWidth;					/* w */
	int displayHeight;					/* w */
	/* int displayAddr;					   zl */
	RCRECT windowBounds;  				/* w4 */
	/* RCRECT clippingBounds;			   zw4 */
	/* BitmapPtr bitmapP;				   zl */
	RCWINDOWFLAGS windowFlags;			/* zuzuuu zuzuuzu zu8 */
	RCFRAMEBITS frameType;				/* u8zu3uu2u2 */
	/* GraphicStatePtr drawStateP;		   zl */
	/* struct RCWindow32Type *nextWindow;  zl */
	} RCWindowBA32Type;
#define szRCWindowBA32 "w,w,zl,w4,zw4,zl,zuzuuuzuzuuzuzu8,u8zu3uu2u2,zl,zl"

#ifdef ARM
	typedef RCWindowBA32Type	RCWINDOW;
	#define szRCWINDOW			szRCWindowBA32 
#else
	typedef RCWindowBA16Type	RCWINDOW;
	#define szRCWINDOW			szRCWindowBA16 
#endif


/* list.h */
typedef struct _rclistattr
	{
	int usable;			/* u */
	int enabled;		/* u */
	int visible;		/* u */
	int poppedUp;		/* u */
	int hasScrollBar;	/* u */
	int search;			/* u */
	/* int reserved;	   zu10 */
	} RCLISTATTR;		/* uuuuuuzu10 */

typedef struct _rcListBA16Type
	{
	int id;					/* w */
	RCRECT bounds;			/* w4 */
	RCLISTATTR attr;		/* uuuuuuzu10 */
	char *itemsText;		/* p */
	int numItems;			/* w */
	/* Word currentItem;	   zw */
	/* Word topItem;		   zw */
	int	font;				/* b  */
	/* UInt8 reserved;	       zb */
	/* WinHandle popupWin;	   zl */
	/* ListDrawDataFuncPtr	drawItemsCallback;  zl */
	/* private, not stored into file */
	int cbListItems;
	} RCListBA16Type;
#define szRCListBA16 "w,w4,uuuuuuzu10,p,w,zw,zw,bzb,zl,zl"
typedef struct _rclistBA32Type
	{
	int id;					/* w */
	RCLISTATTR attr;		/* uuuuuuzu10 */
	RCRECT bounds;			/* w4 */
	char *itemsText;		/* p */
	int numItems;			/* w */
	/* Word currentItem;	   zw */
	/* Word topItem;		   zw */
	int	font;				/* b  */
	/* UInt8 reserved;	       zb */
	/* WinHandle popupWin;	   zl */
	/* ListDrawDataFuncPtr	drawItemsCallback;  zl */
	/* private, not stored into file */
	int cbListItems;
	} RCListBA32Type;
#define szRCListBA32 "w,uuuuuuzu10,w4,p,w,zw,zw,bzb,zl,zl"

#ifdef ARM
	typedef RCListBA32Type	RCLIST;
	#define szRCLIST		szRCListBA32
#else
	typedef RCListBA16Type	RCLIST;
	#define szRCLIST		szRCListBA16
#endif


/* Field.h */
typedef struct _rcfieldattr
	{
	int usable;			/* u  */
	int visible;		/* u  */
	int editable;		/* u  */
	int singleLine;	/* u  */

	int hasFocus;     /* u  */
	int dynamicSize;  /* u  */
	int insPtVisible; /* u  */
	int dirty;			/* u  */

	int underlined;	/* u2 */
	int justification;/* u2	*/
	int autoShift;		/* u  */
	int hasScrollBar; /* u  */
	int numeric;		/* u  */
							/* zu */
	} RCFIELDATTR;		/* uuuuuuuu u2u2uuuzu */

typedef struct _rcFieldBA16Type
	{
	int id;	              /* w */
	RCRECT rect;          /* w4 */
	RCFIELDATTR attr;     /* uuuuuuuu u2u2uuuzu */
	char *text;		      /* p */
	/* VoidHand		       zl	textHandle;			 block the contains the text string */
	/* LineInfoPtr	       zl	lines; */
	/* Word			       zw	textLen; */
	/* Word			       zw	textBlockSize; */
	int maxChars;	      /* w */
	/* int selFirstPos;    zw  */
	/* int selLastPos;     zw */
	/* int insPtXPos;      zw */
	/* int insPtYPos;      zw */
	int	fontID;		      /* b */
	/* int reserved;	   zb */
	} RCFieldBA16Type;
#define szRCFieldBA16 "w,w4,uuuuuuuu,u2u2uuuzu,p,zl,zl,zw,zw,w,zw,zw,zw,zw,b,zb"
typedef struct _rcFieldBA32Type
	{
	RCRECT rect;          /* w4 */
	int id;	              /* w */
	RCFIELDATTR attr;     /* uuuuuuuu u2u2uuuzu */
	char *text;		      /* p */
	/* VoidHand		       zl	textHandle;			 block the contains the text string */
	/* LineInfoPtr	       zl	lines; */
	/* Word			       zw	textLen; */
	/* Word			       zw	textBlockSize; */
	int maxChars;	      /* w */
	/* int selFirstPos;    zw  */
	/* int selLastPos;     zw */
	/* int insPtXPos;      zw */
	/* int insPtYPos;      zw */
	int	fontID;		      /* b */
	/* int reserved;	   zb */
	} RCFieldBA32Type;
#define szRCFieldBA32 "w4,w,uuuuuuuu,u2u2uuuzu,p,zl,zl,zw,zw,w,zw,zw,zw,zw,b,zb"

#ifdef ARM
	typedef RCFieldBA32Type	RCFIELD;
	#define szRCFIELD		szRCFieldBA32
#else
	typedef RCFieldBA16Type	RCFIELD;
	#define szRCFIELD		szRCFieldBA16
#endif


/* Table.h */
typedef struct _rcTableColumnAttrBA16Type
	{
	int width;								/* w  */
	/* int reserved1;				  	       zt5 */
	int masked;								/* t  */
	int editIndicator;						/* t  */
	int usable;								/* t */
	/* int reserved2;						   zb */
	int spacing;							/* w  */
   /* TableDrawItemFuncPtr drawCallback;	   zl */
   /* TableLoadDataFuncPtr loadDataCallback;   zl */
   /* TableSaveDataFuncPtr saveDataCallback;   zl */
	} RCTableColumnAttrBA16Type;
#define szRCTableColumnAttrBA16 "w,zt5tttzb,w,zl,zl,zl"
typedef struct _rcTableColumnAttrBA32Type {
	int width;								/* w  */
	int spacing;							/* w  */
	/* int reserved1;				  	       zt5 */
	int masked;								/* t  */
	int editIndicator;						/* t  */
	int usable;								/* t */
	/* int reserved2;						   zb */
	/* int reserved3;						   zw */
   /*TableDrawItemFuncPtr drawCallback;		   zl */
   /* TableLoadDataFuncPtr loadDataCallback;   zl */
   /* TableSaveDataFuncPtr saveDataCallback;   zl */
} RCTableColumnAttrBA32Type;
#define szRCTableColumnAttrBA32 "w,w,zt5tttzb,zw,zl,zl,zl"

#ifdef ARM
	typedef RCTableColumnAttrBA32Type	RCTABLECOLUMNATTR;
	#define szRCTABLECOLUMNATTR			szRCTableColumnAttrBA32
#else
	typedef RCTableColumnAttrBA16Type	RCTABLECOLUMNATTR;
	#define szRCTABLECOLUMNATTR			szRCTableColumnAttrBA16
#endif

typedef struct _rcTableRowAttrType 
	{
	int id;					/* w  */
	int height;				/* w  */
	/*   DWord data;		   zl */
	/* int reserved1;		   zt7 */
	int usable;				/* t  */
	/* int reserved2;		   zt4 */
	int masked;				/* t  */
	int invalid;			/* t  */
	int staticHeight;		/* t  */
	int selectable;			/* t  */
	/* int reserved3;		   zw */
	} RCTABLEROWATTR;
#define szRCTABLEROWATTR "w,w,zl,zt7t,zt4tttt,zw"


/* this is bogus...don't know why */
#define szRCTABLEPADDING "zl,zl"

typedef struct RCTableAttrType 
	{
	int visible;		/* u */
	int editable;		/* u */
	int editing;		/* u */
	int selected;		/* u */
	int hasScrollBar;	/* u */
	/*int reserved;		   zu11 */
	} RCTABLEATTR;		/* uuuu uzu11 */

typedef struct _rcTableBA16Type 
	{
	int id;							/* w  */
	RCRECT bounds;					/* w4 */
	RCTABLEATTR attr;				/* uuuu uzu11 */
	int numColumns;					/* w */
	int numRows;					/* w */
	int currentRow;					/* w */
	int currentColumn;				/* w */
	int topRow;						/* w */
	/*   TableColumnAttrType * columnAttrs;		zl */
	/* TableRowAttrType *      rowAttrs;		zl */
    /* TableItemPtr            items;			zl */
    RCFieldBA16Type currentField;
	/* not emitted */
	int *rgdxcol; 
	} RCTableBA16Type;
#define szRCTableBA16 "w,w4,uuuuuzu11,w,w,w,w,w,w,zl,zl,zl"##szRCFieldBA16
typedef struct _rcTableBA32Type 
	{
	int id;							/* w  */
	RCTABLEATTR attr;				/* uuuu uzu11 */
	RCRECT bounds;					/* w4 */
	int numColumns;					/* w */
	int numRows;					/* w */
	int currentRow;					/* w */
	int currentColumn;				/* w */
	int topRow;						/* w */
	/* int reserved					   zw */
	/*   TableColumnAttrType *   columnAttrs;	zl */
	/* TableRowAttrType *      rowAttrs;		zl */
   /* TableItemPtr            items;			zl */
    RCFieldBA32Type currentField;
	/* not emitted */
	int *rgdxcol; 
	} RCTableBA32Type;
#define szRCTableBA32 "w,uuuuuzu11,w4,w,w,w,w,w,zw,zl,zl,zl"##szRCFieldBA32

#ifdef ARM
	typedef RCTableBA32Type		RCTABLE;
	#define szRCTABLE			szRCTableBA32
#else
	typedef RCTableBA16Type		RCTABLE;
	#define szRCTABLE			szRCTableBA16
#endif


/* form.h */
typedef struct _rcformobjattr
	{
	int usable;			/* t,zt7 (opt) */
	/* int reserved		   zb */
	} RCFORMOBJATTR;	/* tzt7,zb */

typedef struct _rcFormLabelBA16Type
	{
	int id;	            /* w */
	RCPOINT pos;        /* w2 */
	RCFORMOBJATTR attr; /* uzu15 */
	int fontID;	        /* b */
	/* int reserved        zb */
	char *text;	        /* p */
	} RCFormLabelBA16Type;
#define szRCFormLabelBA16 "w,w2,uzu15,b,zb,p"
typedef struct _rcFormLabelBA32Type
	{
	RCPOINT pos;        /* w2 */
	char *text;	        /* p */
	int id;	            /* w */
	RCFORMOBJATTR attr; /* uzu15 */
	int fontID;	        /* b */
	/* int reserved        zb */
	/* int padding;		   zw */
	} RCFormLabelBA32Type;
#define szRCFormLabelBA32 "w2,p,w,uzu15,b,zb,zw"

#ifdef ARM
	typedef RCFormLabelBA32Type RCFORMLABEL;
	#define szRCFORMLABEL		szRCFormLabelBA32
#else
	typedef RCFormLabelBA16Type RCFORMLABEL;
	#define szRCFORMLABEL		szRCFormLabelBA16
#endif

// &&& RMa: define if it use
typedef struct _rclabel
	{
	int id;	            /* w */
	RCPOINT pos;        /* w2 */
	RCFORMOBJATTR attr; /* tzt7,zb */
	int fontID;	        /* b */
	/* int reserved        zb */
	char *text;	        /* p */
	} RCLABEL;
#define szRCLABEL "w,w2,tzt7,zb,b,zb,p"


typedef struct _rcformtitle
	{
	RCRECT rect;      /* w4 */
	char *text;       /* p */
	} RCFORMTITLE;
#define szRCFORMTITLE "w4,p"


typedef struct _rcformpopup
	{
	int	controlID;    /* w */
	int	listID;       /* w */
	} RCFORMPOPUP;
#define szRCFORMPOPUP "ww"

typedef struct _rcgsi
	{
	RCPOINT pos;     /* w2 */
	} RCFORMGRAFFITISTATE;
#define szRCFORMGRAFFITISTATE "w2"

typedef struct _rcFormGadgetAttr
	{
	int usable;			/* u */
	int extended;		/* u */
	int visible;		/* u */
	/* int reserved		   zu13 */
	} RCFormGadgetAttr;	/* uuuzu13 */

typedef struct _rcformgadget
	{
	int id;							/* w */
	RCFormGadgetAttr attr;			/* uuuzu13 */
	RCRECT rect;					/* w4 */
	/* VoidPtr data;				   zl */
	/*FormGadgetHandlerType	*handler   zl */
	} RCFORMGADGET;
#define szRCFORMGADGET "w,uuuzu13,w4,zl,zl"

typedef struct _rcFormBitMapBA16Type
	{
	RCFORMOBJATTR attr;	/* uzu15 */
	RCPOINT pos;	    /* w2 */
	int rscID;          /* w */
	} RCFormBitMapBA16Type;
#define szRCFormBitMapBA16 "uzu15,w2,w"
typedef struct _rcFormBitMapBA32Type
	{
	RCFORMOBJATTR attr;	/* uzu15 */
	int rscID;          /* w */
	RCPOINT pos;	    /* w2 */
	} RCFormBitMapBA32Type;
#define szRCFormBitMapBA32 "uzu15,w,w2"

#ifdef ARM
	typedef RCFormBitMapBA32Type	RCFORMBITMAP;
	#define szRCFORMBITMAP			szRCFormBitMapBA32
#else
	typedef RCFormBitMapBA16Type	RCFORMBITMAP;
	#define szRCFORMBITMAP			szRCFormBitMapBA16
#endif


/* control.h */
typedef struct _rccontrolattr
	{
	int usable;			/* u  */
	int enabled;		/* u  */
	int visible;		/* u  */ /* OS use internal */
	int on;				/* u  */
	int leftAnchor;		/* u  */
	int frame;			/* u3 */

	int drawnAsSelected;/* u  */
	int graphical;		/* u  */
	int vertical;		/* u  */
	/*int reserved;		   zu5 */
} RCCONTROLATTR;		/* uuuuuu3 uuuzu5 */

typedef struct _rcontrolheader
	{
	int id;					/* w */
	RCRECT bounds;			/* w4 */
	short backgroundid;	/* l/2 switch order on intel */
	short thumbid;			/* l/2 */
/*	char *text;             p */
	RCCONTROLATTR attr;	/* tttttt3 tttzt5 */
	int style;				/* b */
	} RCCONTROLHEADER;        
#define szRCCONTROLHEADER "w,w4,l,tttttt3,tttzt5,b"

typedef struct _rcontroltail
	{
	int font;	         /* b */
	int group;				/* b */
	} RCCONTROLTAIL;        
#define szRCCONTROLTAIL "b,b,zb"

typedef struct _rcControlBA16Type
	{
	int id;					/* w */
	RCRECT bounds;      /* w4 */
	union {
		struct {
			short backgroundid; /* l/2 switch order on intel */
			short thumbid;	     /* l/2 */
		} ids;
		char *text;			/* p */
	} u;
	RCCONTROLATTR attr; /* uuuuuu3 uuuzu5 */
	int style;          /* b */
	int font;           /* b */
	int group;          /* b */
	} RCControlBA16Type;        
#define szRCCONTROLBA16 "w,w4,p,uuuuuu3,uuuzu5,b,b,b,zb"
#define szRCGRAPHICALCONTROLBA16 "w,w4,l,uuuuuu3,uuuzu5,b,b,b,zb"
typedef struct _rcControlBA32Type
	{
	int id;					/* w */
	RCCONTROLATTR attr;		/* uuuuuu3 uuuzu5 */
	RCRECT bounds;			/* w4 */
	union {
		struct {
			short backgroundid; /* l/2 switch order on intel */
			short thumbid;	    /* l/2 */
		} ids;
		char *text;				/* p */
	} u;
	ControlStyles style;	/* b */
	int font;				/* b */
	int group;				/* b */
	/*int reserved;			   zb */
	} RCControlBA32Type;
#define szRCControlBA32 "w,uuuuuu3,uuuzu5,w4,p,b,b,b,zb"
#define szRCGRAPHICALCONTROLBA32 "w,uuuuuu3,uuuzu5,w4,l,b,b,b,zb"

#ifdef ARM
	typedef RCControlBA32Type	RCCONTROL;
	#define szRCCONTROL			szRCControlBA32
#else
	typedef RCControlBA16Type	RCCONTROL;
	#define szRCCONTROL			szRCControlBA16
#endif


typedef struct _rslidercontroltail
	{
	int minValue;       /* w */
	int maxValue;       /* w */
	int pageSize;       /* w */
	int value;          /* w */
				/* zl */
	} RCSLIDERCONTROLTAIL;        
#define szRCSLIDERCONTROLTAIL "zb,w,w,w,w,zl"

typedef struct _SliderControlBA16Type
	{
	int					id;				/* w  */
	RCRECT				bounds;			/* w4 */
/* reversing this 2 param to manage word ordering */
	short				backgroundid;	/* l  */				// overlays text in ControlBA16Type
	short				thumbid;		/*     */				// overlays text in ControlBA16Type
	RCCONTROLATTR		attr;			/* uuuuuu3,uuuzu5 */	// graphical *is* set
	ControlStyles		style;			/* b */					// must be sliderCtl or repeatingSliderCtl
	/* UInt8			reserved;		   zb */
	int					minValue;		/* w */
	int					maxValue;		/* w */
	int					pageSize;		/* w */	/* pageJump == pageSize */
	int					value;			/* w */
	/*MemPtr			activeSliderP;	   zl */
	} RCSliderControlBA16Type;
#define szRCSliderControlBA16 "w,w4,l,uuuuuu3,uuuzu5,b,zb,w,w,w,w,zl"
typedef struct _SliderControlBA32Type
	{
	int					id;				/* w */
	RCCONTROLATTR		attr;			/* uuuuuu3,uuuzu5 */	// graphical *is* set
	RCRECT				bounds;			/* w4 */
	short				thumbid;		/*    */				// overlays text in ControlBA16Type
	short				backgroundid;	/* l  */				// overlays text in ControlBA16Type
	ControlStyles		style;			/* b  */				// must be sliderCtl or repeatingSliderCtl
	/* UInt8			reserved;		   zb */
	int					minValue;		/* w */
	int					maxValue;		/* w */
	int					pageSize;		/* w */
	int					value;			/* w */
	/*int				reserved2;		   zw */
	/*MemPtr			activeSliderP;	   zl */
	} RCSliderControlBA32Type;
#define szRCSliderControlBA32 "w,uuuuuu3,uuuzu5,w4,l,b,zb,w,w,w,w,zw,zl"

#ifdef ARM
	typedef RCSliderControlBA32Type		RCSLIDERCONTROL;
	#define szRCSLIDERCONTROL			szRCSliderControlBA16
#else
	typedef RCSliderControlBA16Type		RCSLIDERCONTROL;
	#define szRCSLIDERCONTROL			szRCSliderControlBA32
#endif

/* scrollbar.h */
typedef struct _rcscrollbarattr
	{
	int usable;			/* t */
	int visible;		/* t */
	int hilighted;		/* t */
	int shown;			/* t */
	int activeRegion;	/* t4 */
	/*int reserved;		   zb */
	} RCSCROLLBARATTR;

typedef struct _rcscrollbar
	{
	RCRECT bounds;          /* w4 */
	int id;                 /* w */
	RCSCROLLBARATTR attr;   /* ttttt4,zb */
	int value;				/* w */
	int minValue;           /* w */
	int maxValue;           /* w */
	int pageSize;           /* w */
	/*Short penPosInCar;*/	/* zw */
	/*Short savePos;*/      /* zw */ 
	} RCSCROLLBAR;
#define szRCSCROLLBAR "w4,w,ttttt4,zb,w,w,w,w,zw,zw"

typedef union _rcFormObjectBA16Type
	{                  
	void *						ptr;
	RCFieldBA16Type *    		field;
	RCControlBA16Type *    		control;
	RCSliderControlBA16Type *	slider;
	RCListBA16Type	*		 	list;
	RCTableBA16Type *			table;
	RCFormBitMapBA16Type *    	bitmap;
/*	RCFORMLINE *				line; */
/*	RCFORMFRAME *	    		frame; */
/*	RCFORMRECTANGLE *   		rectangle; */
	RCFormLabelBA16Type *		label;
	RCFORMTITLE *	    		title;	
	RCFORMPOPUP *	    		popup;
	RCFORMGRAFFITISTATE *		grfState;
	RCFORMGADGET *	    		gadget;
	RCSCROLLBAR	*				scrollbar;
	} RCFormObjectBA16Type;
typedef union _rcFormObjectBA32Type
	{                  
	void *						ptr;
	RCFieldBA32Type *    		field;
	RCControlBA32Type *    		control;
	RCSliderControlBA32Type *	slider;
	RCListBA32Type	*		 	list;
	RCTableBA32Type *			table;
	RCFormBitMapBA32Type *    	bitmap;
/*	RCFORMLINE *				line; */
/*	RCFORMFRAME *	    		frame; */
/*	RCFORMRECTANGLE *   		rectangle; */
	RCFormLabelBA32Type *		label;
	RCFORMTITLE *	    		title;	
	RCFORMPOPUP *	    		popup;
	RCFORMGRAFFITISTATE *		grfState;
	RCFORMGADGET *	    		gadget;
	RCSCROLLBAR	*				scrollbar;
	} RCFormObjectBA32Type;    

#ifdef ARM
	typedef RCFormObjectBA32Type	RCFORMOBJECT;
#else
	typedef RCFormObjectBA16Type	RCFORMOBJECT;
#endif


typedef struct _rcFormObjListBA16Type
	{
	int objectType;				/* b  */
	/*int reserved;				   zb */
	union
		{
		RCFORMOBJECT object;	/* l */
		int ibobj;
		} u;
	} RCFormObjListBA16Type;
#define szRCFormObjListBA16 "b,zb,l"
typedef struct _rcFormObjListBA32Type
	{
	union
		{
		RCFORMOBJECT object;	/* l */
		int ibobj;
		} u;
	int objectType;				/* b */
	/*int reserved;				   zb */
	/*int padding;				   zw */
	} RCFormObjListBA32Type;
#define szRCFormObjListBA32 "l,b,zb,zw"

#ifdef ARM
	typedef RCFormObjListBA32Type	RCFORMOBJLIST;
	#define szRCFORMOBJLIST			szRCFormObjListBA32
#else
	typedef RCFormObjListBA16Type	RCFORMOBJLIST;
	#define szRCFORMOBJLIST			szRCFormObjListBA16
#endif

typedef struct _rcformattr
	{
	int usable;				/* u */
	int enabled;			/* u */
	int visible;			/* u */
	int dirty;				/* u */
	int saveBehind;			/* u */
	int graffitiShift;		/* u */
	int globalsAvailable;	/* u */
	int doingDialog;		/* u */
	int exitDialog;			/* u */
	/*int reserved;			   zu7 */
	/*int reserved2;		   zw */
	} RCFORMATTR;	/* uuuuuuuu,uzu7,zw*/

typedef struct _rcFormBA16Type
	{
	RCWindowBA16Type window;		
	int formId;							/* w  */
	RCFORMATTR attr;					/* uuuuuuuu uzu7 zw */
	/* WinHandle bitsBehindForm;		   zl */
	/* FormEventHandlerPtr handler;		   zl */
	/* Word	focus;						   zw */
	int defaultButton;					/* w  */
	int helpRscId;						/* w  */
	int menuRscId;						/* w  */
	int numObjects;						/* w  */
	/* RCFormObjListBA16Type *objects;     zl */
	} RCFormBA16Type;
#define szRCFormBA16 szRCWindowBA16##",w,uuuuuuuuuzu7,zw,zl,zl,zw,w,w,w,w,zl"
typedef struct _rcFormBA32Type
	{
	RCWindowBA32Type window;		
	RCFORMATTR attr;					/* uuuuuuuu uzu7 zw */
	/* WinHandle bitsBehindForm;		   zl */
	/* FormEventHandlerPtr handler;		   zl */
	int formId;							/* w  */
	/* Word	focus;						   zw */
	int defaultButton;					/* w  */
	int helpRscId;						/* w  */
	int menuRscId;						/* w  */
	int numObjects;						/* w  */
	/* RCFormObjListBA32Type *objects;	   zl */
	} RCFormBA32Type;
#define szRCFormBA32 szRCWindowBA32##",uuuuuuuuuzu7,zw,zl,zl,w,zw,w,w,w,w,zl"

#ifdef ARM
	typedef RCFormBA32Type	RCFORM;
	#define szRCFORM		szRCFormBA32
#else
	typedef RCFormBA16Type	RCFORM;
	#define szRCFORM		szRCFormBA16
#endif


/*-----------------------------------------------------------------------------
|	MENUS
-------------------------------------------------------------WESC------------*/

typedef struct _rcmenuitem
	{
	int id;	        /* w */
	int command;    /* b */
	int hidden;		/* t */
	//int reserved;	/* zt7 */
	char *itemStr;  /* l */
	} RCMENUITEM;
#define szRCMENUITEM "w,b,tzt7,l"

typedef struct _rcMenuPullDownBA16Type
	{
	/* WinHandle menuWin;       zl */
	RCRECT bounds;           /* w4 */
	/* WinHandle bitsBehind;    zl */
	RCRECT titleBounds;      /* w4 */
	char *title;	         /* l */
	int hidden;				 /* u */
	int numItems;            /* u15 */
	RCMENUITEM * items;      /* l */
	} RCMenuPullDownBA16Type;
#define szRCMenuPullDownBA16 "zl,w4,zl,w4,l,uu15,l"
typedef struct _rcMenuPullDownBA32Type
	{
	/* WinHandle menuWin;       zl */
	RCRECT bounds;           /* w4 */
	/* WinHandle bitsBehind;    zl */
	RCRECT titleBounds;      /* w4 */
	char *title;	         /* l  */
	int hidden;				 /* u  */
	/* int reserved				zu15 */
	int numItems;			 /*	w  */
	RCMENUITEM * items;      /* l  */
	} RCMenuPullDownBA32Type;
#define szRCMenuPullDownBA32 "zl,w4,zl,w4,l,uzu15,w,l"

#ifdef ARM
	typedef RCMenuPullDownBA32Type	RCMENUPULLDOWN;
	#define szRCMENUPULLDOWN		szRCMenuPullDownBA32
#else
	typedef RCMenuPullDownBA16Type	RCMENUPULLDOWN;
	#define szRCMENUPULLDOWN		szRCMenuPullDownBA16
#endif

typedef struct _RCMENUBARATTR
	{
	int visible;			/* u  */
	/*WORD commandPending;	   zu */
	/*WORD insPtEnabled;	   zu */
	/*int needsRecalc;		   zu */
	/*int reserved;			   zu12 */
	} RCMENUBARATTR;

typedef struct _rcMenuBarBA16Type
	{
	/*WinHandle barWin;				   zl */
	/*WinHandle bitsBehind;			   zl */
	/*WinHandle savedActiveWin;		   zl */
	/*WinHandle bitsBehindStatus;	   zl */
	RCMENUBARATTR attr;				/* uzu3zu12 */
	/*SWord curMenu;				   zw */
	int curItem;					/* w  */
	/* SDWord commandTick;			   zl */
	int numMenus;					/* w   number of menus */
	/* MenuPullDownPtr menus;		   zl  array of menus */
	} RCMenuBarBA16Type;
#define szRCMenuBarBA16 "zl,zl,zl,zl,uzu3zu12,zw,w,zl,w,zl"
typedef struct _rcMenuBarBA32Type
	{
	/*WinHandle barWin;				   zl */
	/*WinHandle bitsBehind;			   zl */
	/*WinHandle savedActiveWin;		   zl */
	/*WinHandle bitsBehindStatus;	   zl */
	RCMENUBARATTR attr;				/* uzu3zu12 */
	/*SWord curMenu;				   zw */
	int curItem;					/* w  */
	int numMenus;					/* w   number of menus */
	/* SDWord commandTick;			   zl */
	/* MenuPullDownPtr menus;		   zl  array of menus */
	} RCMenuBarBA32Type;
#define szRCMenuBarBA32 "zl,zl,zl,zl,uzu3zu12,zw,w,w,zl,zl"

#ifdef ARM
	typedef RCMenuBarBA32Type	RCMENUBAR;
	#define szRCMENUBAR			szRCMenuBarBA32
#else
	typedef RCMenuBarBA16Type	RCMENUBAR;
	#define szRCMENUBAR			szRCMenuBarBA16
#endif


/*-----------------------------------------------------------------------------
|	ALERTS
-------------------------------------------------------------WESC------------*/

typedef struct _rcALERTTEMPLATE
	{
	int alertType;			/* w */
	int helpRscID; 	   /* w */
	int numButtons;		/* w */
	int defaultButton;	/* w */
	} RCALERTTEMPLATE;
#define szRCALERTTEMPLATE "w,w,w,w"


/*-----------------------------------------------------------------------------
|	BITMAP
-------------------------------------------------------------WESC------------*/
typedef struct _rcBitmapFlagsType
	{
	int compressed;			/* u */ /* Data format:  0=raw; 1=compressed */
	int hasColorTable;		/* u */ /* if true, color table stored before bits[] */
	int hasTransparency;		/* u */ /* true if transparency is used */
	int indirect;			/* u */ /* true if bits are stored indirectly */
									/*           Never set this flag. Only the display (screen) bitmap has the indirect bit set. */
	int forScreen;			/* u */ /* system use only */
	int directColor;		/* u  */ /* direct color bitmap */
	//int reserved;			/* zu10 */ 
	} RCBitmapFlagsType;

typedef struct _rcBITMAP
	{ /* bm */
    int cx;						/* w */
	int cy;						/* w */
	int cbRow;					/* w */
	RCBitmapFlagsType flags;	/* uuuuuuzu10 */	/* RMa struct updated */
    int pixelsize;				/* b */
    int version;				/* b */
    int nextDepthOffset;		/* w */
    int transparentIndex;		/* b */
    int compressionType;		/* b */
	/* ushort and_reserved_and_colorTable[3] */	/* z1w */
	unsigned char *pbBits;

	/* private, not stored into file */
	int cbDst;
	} RCBITMAP;
#define szRCBITMAP "w,w,w,uuuuuuzu10,b,b,w,b,b,zw"


/*-----------------------------------------------------------------------------
|	Other PILRC types and such
-------------------------------------------------------------WESC------------*/

//#define ifrmMax 32
DEFPL(PLEXFORMOBJLIST);
typedef struct _FRMBA16
	{
	RCFormBA16Type form;
	PLEXFORMOBJLIST pllt;
//	RCFORMOBJLIST *rglt;
	} FRMBA16Type;
typedef struct _FRMBA32
	{
	RCFormBA32Type form;
	PLEXFORMOBJLIST pllt;
//	RCFORMOBJLIST *rglt;
	} FRMBA32Type;

#ifdef ARM
	typedef FRMBA32Type		FRM;
#else
	typedef FRMBA16Type		FRM;
#endif

//extern FRM rgfrm[];
//extern int ifrmMac;


void ErrorLine(char *sz);
				

/* Translation Entry -- used to map to foreign languages */
typedef struct _te
	{
	char *szOrig;
	char *szTrans;
	struct _te *pteNext;
	} TE;


/* Symbol Table */
typedef struct _sym
	{
	char *sz;
	int wVal;
	BOOL fAutoId;
	struct _sym *psymNext;
	} SYM;


/* RCPFILE -- contains everything in a .rcp file */
DEFPL(PLEXFRM)
typedef struct _rcpfile
	{
	PLEXFRM plfrm;
	} RCPFILE;	
void FreeRcpfile(RCPFILE *prcpfile);	

/* ReservedWord */
typedef enum 
	{
	rwFLD, rwLST, rwTBL, rwFBM, rwLBL, rwTTL, rwPUL, rwGSI, rwGDT,
	rwBTN, rwPBN, rwCBX, rwPUT, rwSLT, rwREP, rwSLD, rwSCL,

	rwForm, rwBegin, rwEnd, rwModal, rwSaveBehind, rwNoSaveBehind, rwHelpId, rwDefaultBtnId, rwMenuId, 
	rwEnabled, rwDisabled, rwUsable, rwNonUsable, rwLeftAnchor, rwRightAnchor, rwGroup, rwFont, 
	rwFrame, rwNoFrame, rwBoldFrame, rwVertical, rwGraphical,
	
	rwEditable, rwNonEditable, rwUnderlined, rwSingleLine, rwMultipleLines, rwDynamicSize, rwLeftAlign, rwHasScrollBar, rwRightAlign, rwMaxChars,
	rwVisibleItems, rwAutoShift, rwNumeric,
	rwChecked,

	rwBitmap,
	rwBitmapGrey,
	rwBitmapGrey16,
	rwBitmapColor16,
	rwBitmapColor256,
// 2.7 modifications/additions
	rwBitmapColor16k,
	rwBitmapColor24k,
	rwBitmapColor32k,
// end
	rwBitmapFamily,
	rwBitmapFamily_special,
// 2.7 modifications/additions
	rwInteger,
// end
	rwPrevLeft, rwPrevRight, rwPrevWidth, rwPrevTop, rwPrevBottom, rwPrevHeight,
	rwMenu,
	rwPullDown,
	rwMenuItem,
	rwSeparator,

	rwValue,
	rwMinValue,
	rwMaxValue,
	rwPageSize,

// 2.7 RMa addition support Slider and graphical control
	rwFeedback,
	rwThumbID,
	rwBackgroundID,
	rwBitmapID,
	rwSelectedBitmapID,
// end

	rwAlert,
	rwMessage,
	rwButtons,
	rwDefaultBtn,

	rwInformation, /* order assumed */
	rwConfirmation,
	rwWarning,
	rwError,

	rwVersion,
	rwString,
	rwStringTable,
	rwFile,
	rwApplicationIconName,
	rwApplication,
	rwLauncherCategory,		/* RMa add: application default launcher category */
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

	rwIcon,
//	rwIconGrey,
//	rwIconGrey16,
//	rwIconColor,     
                         // Aaron Ardiri 
                         // - removed these due to be purely backward 
                         //   compatable in the applications launcher
	rwIconFamily,

	rwIconSmall,
//  	rwIconSmallGrey,
//  	rwIconSmallGrey16,
//  	rwIconSmallColor,
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
	} RW;


typedef struct _rwt
	{
	char *sz1;
	char *sz2;
	RW rw;
	} RWT;

/* could just use index into this table as rw but that is kinda fragile. */
#ifdef EMITRWT
RWT rgrwt[] =
	{
	{"tFLD",     "field",            rwFLD},
	{"tLST",	 "list",             rwLST}, /* List */
	{"tTBL",	 "table",            rwTBL}, /* Table */
	{"tFBM",	 "formbitmap",       rwFBM}, /* Form Bitmap */
	{"tLBL",	 "label",            rwLBL}, /* Label */
	{"tTTL",     "title",            rwTTL}, /* Title		 */
	{"tPUL",     "popuplist",        rwPUL},
	{"tGSI",     "graffitistateindicator",rwGSI}, /* Graffiti State */
	{"tGDT",      "gadget",          rwGDT}, /* Gadget */
	{"tSCL",      "scrollbar",       rwSCL}, /* Scrollbar */
	                                     
	{"tBTN",			"button",           rwBTN},
	{"tPBN",			"pushbutton",       rwPBN},
	{"tCBX",			"checkbox",         rwCBX},/*  */
	{"tPUT",			"popuptrigger",     rwPUT},/* Popup trigger */
	{"tSLT",			"selectortrigger",  rwSLT},
	{"tREP",			"repeatbutton",     rwREP},
// 2.7 additions
	{"tSLD",			"slider",			rwSLD},		/* RMa add */
// end 
	{"form",			"tFRM",         rwForm},
	{"begin",        NULL,           rwBegin}, 
	{"end",          NULL,           rwEnd}, 
	{"modal",        NULL,           rwModal}, 
	{"savebehind",   NULL,           rwSaveBehind},
	{"nosavebehind",   NULL,           rwNoSaveBehind},
	{"helpid",       NULL,           rwHelpId},  
	{"defaultbtnid", NULL,           rwDefaultBtnId},    
	{"menuid",       NULL,           rwMenuId}, 

	{"enabled",      NULL,           rwEnabled},
	{"disabled",     NULL,           rwDisabled}, 
	{"usable",       NULL,           rwUsable}, 
	{"nonusable",    NULL,           rwNonUsable}, 
	{"leftanchor",   NULL,           rwLeftAnchor}, 
	{"rightanchor",  NULL,           rwRightAnchor}, 
	{"group",        NULL,           rwGroup}, 
	{"font",         NULL,           rwFont}, 
	
	{"frame",        NULL,           rwFrame}, 
	{"noframe",      NULL,           rwNoFrame}, 
	{"boldframe",    NULL,           rwBoldFrame}, 

// 2.7 additions
	{"vertical",			NULL,           rwVertical}, 
	{"graphical",			NULL,           rwGraphical}, 
// end
	{"editable",     NULL,           rwEditable}, 
	{"noneditable",  NULL,           rwNonEditable}, 
	{"underlined",   NULL,           rwUnderlined}, 
	{"singleline",   NULL,           rwSingleLine}, 
	{"multiplelines","multipleline",  rwMultipleLines}, 
	{"dynamicsize",  NULL,           rwDynamicSize}, 
	{"leftalign",    NULL,           rwLeftAlign}, 
	{"hasscrollbar", NULL,           rwHasScrollBar}, 
	{"rightalign",   NULL,           rwRightAlign}, 
	{"maxchars",     NULL,           rwMaxChars}, 
	{"autoshift",    NULL,           rwAutoShift},
	{"numeric",	     NULL,           rwNumeric},
	
	{"visibleitems", NULL,           rwVisibleItems}, 

	{"value",        NULL,           rwValue},
	{"min",          "minvalue",     rwMinValue},
	{"max",          "maxvalue",     rwMaxValue},
	{"pagesize",     NULL,           rwPageSize},
// 2.7 modifications/additions
	{"feedback",			NULL,			rwFeedback},

	{"thumbid",				NULL,       rwThumbID},
	{"backgroundid",		NULL,       rwBackgroundID},

	{"bitmapid",			NULL,       rwBitmapID},
	{"selectedbitmapid",	NULL,			rwSelectedBitmapID},
// end
	{"checked",      "on",           rwChecked}, 
	{"bitmap",       NULL,           rwBitmap},
	{"bitmapgrey",   "bitmapgray",   rwBitmapGrey},
	{"bitmapgrey16", "bitmapgray16", rwBitmapGrey16},
	{"bitmapcolor16",  "bitmapcolour16", rwBitmapColor16},
	{"bitmapcolor",  "bitmapcolour", rwBitmapColor256},
// 2.7 modifications/additions
	{"bitmapcolor16k",  "bitmapcolour16k", rwBitmapColor16k},
	{"bitmapcolor24k",  "bitmapcolour24k", rwBitmapColor24k},
	{"bitmapcolor32k",  "bitmapcolour32k", rwBitmapColor32k},
// end 
	{"bitmapfamily", NULL, 		 rwBitmapFamily},
	{"bitmapfamilyspecial", NULL, 	 rwBitmapFamily_special},
// 2.7 modifications/additions
	{"integer", 	NULL, 	 	rwInteger},
// end
	{"prevleft",     NULL,           rwPrevLeft},
	{"prevright",    NULL,           rwPrevRight},
	{"prevwidth",    NULL,           rwPrevWidth},

	{"prevtop",       NULL,          rwPrevTop},
	{"prevbottom",    NULL,          rwPrevBottom},
	{"prevheight",    NULL,          rwPrevHeight},

	{"menu",         "MBAR",         rwMenu},
	{"pulldown",     NULL,           rwPullDown},
	{"menuitem",     NULL,           rwMenuItem},
	{"separator",     NULL,          rwSeparator},

	{"alert",        "tALT",         rwAlert},
	{"message",      NULL,           rwMessage},
	{"defaultbutton",NULL,           rwDefaultBtn},
	{"buttons",      NULL,           rwButtons},
	{"information",  NULL,           rwInformation},
	{"confirmation", NULL,           rwConfirmation},
	{"warning",      NULL,           rwWarning},
	{"error",        NULL,           rwError},

	{"version",      "tVER",         rwVersion},

	{"stringtable",  "tSTL",         rwStringTable},
	{"string",       "tSTR",         rwString},
	{"file",         "tSTR",         rwFile},
	{"applicationiconname", NULL,    rwApplicationIconName},
	{"application", "APPL",          rwApplication},
// 2.7 modifications/additions
	{"launchercategory",	"taic",		rwLauncherCategory},
// end
	{"categories",   "tAIS",         rwCategories},

	{"translation",  NULL,           rwTranslation},

	{"center",       NULL,           rwCenter},
	{"right",		  NULL,           rwRight},
	{"bottom",       NULL,           rwBottom},
	{"auto",         NULL,           rwAuto},

	{"at",           NULL,           rwAt},
	{"id",           NULL,           rwId},
	{"autoid",       NULL,           rwAutoId},
	
	{"columns",      "numcolumns",   rwNumColumns},
	{"rows",         "numrows",      rwNumRows},
	{"columnwidths", "widths",       rwColumnWidths},
	
	{"define",       NULL,           rwDefine},
	{"equ",          NULL,           rwEqu},
	{"include",      NULL,           rwInclude},
	{"ifdef",        NULL,           rwIfdef},
	{"ifndef",       NULL,           rwIfndef},
	{"else",         NULL,           rwElse},
	{"endif",        NULL,           rwEndif},

	{"icon",         NULL,           rwIcon},
//	{"icongrey",     "icongray",     rwIconGrey},
//	{"icongrey16",   "icongray16",   rwIconGrey16},
//	{"iconcolor",    "iconcolour",   rwIconColor},
                         // Aaron Ardiri 
                         // - removed these due to be purely backward 
                         //   compatable in the applications launcher

	{"iconfamily",   NULL,           rwIconFamily},

	{"smallicon",    NULL,           rwIconSmall},
//	{"smallicongrey",     "smallicongray",     rwIconSmallGrey},
//	{"smallicongrey16",   "smallicongray16",   rwIconSmallGrey16},
//	{"smalliconcolor",    "smalliconcolour",   rwIconSmallColor},
                         // Aaron Ardiri 
                         // - removed these due to be purely backward 
                         //   compatable in the applications launcher

	{"smalliconfamily",   NULL,                rwIconSmallFamily},
	
	{"trap",         NULL,           rwTrap},
	{"fontid",       NULL,         rwFontId},

	{"hex",          NULL,           rwHex},
	{"data",         NULL,           rwData},

	{"transparency", "transparent",  rwTransparency},
	{"transparencyIndex", "transparentindex",  rwTransparencyIndex},
	{"colortable",   "colourtable",  rwColorTable},
	{"nocolortable", "nocolourtable",  rwNoColorTable},
	{"nocompress",   NULL,           rwNoCompress},
	{"autocompress", "compress",     rwAutoCompress},
	{"forcecompress",NULL,           rwForceCompress},

	/* Java specific */
	{"public",       NULL,           rwPublic},
	{"short",        NULL,           rwShort},
	{"int",          NULL,           rwInt},
	{"static",       NULL,           rwStatic},
	{"final",        NULL,           rwFinal},
	{NULL,	         NULL,           rwNil},
	};
#endif /* EMITRWT */

/* TOKen */
typedef struct _tok
	{
	RW rw;
	LEX lex;
	} TOK;


extern BOOL vfWinGUI;
extern BOOL vfAutoId;
extern BOOL vfQuiet;
extern BOOL vfAllowEditIDs;
extern BOOL vfVSErrors;
extern BOOL vfCheckDupes;
extern BOOL vfAppend;
extern BOOL vfRTL;
extern char *szLanguage;


#define dxScreen 160
#define dyScreen 160
#define maxCategories  16
#define categoryLength 16

int CbEmitStruct(void *pv, char *szPic, char **ppchText, BOOL fEmit);
RCPFILE *ParseFile(char *szIn, char *szOutDir, char *szResFile, char *szIncFile, int fontType);
SYM *PsymLookupId(int id);
VOID AddSym(char *sz, int wVal);


#endif // _pilrc_h

