/*
 * @(#)pilrc.h
 *
 * Copyright 1997-1999, Wes Cherry   (mailto:wesc@technosis.com)
 *                2000, Aaron Ardiri (mailto:aaron@ardiri.com)
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
|		t : bit (1 bit)
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
							 selectorTriggerCtl, repeatingButtonCtl 
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


/* window.h */
typedef struct _rcframebits
	{
	int cornerDiam;  /* b */
	/* int reserved;  zt4 */
	int shadowWidth; /* t2 */
	int width;	     /* t2 */
	} RCFRAMEBITS;

typedef struct _rcwindowflags
	{
    /* Word format:1;    zt */
    /* Word offscreen:1; zt */
    int modal;		    /* t */
    int focusable;	    /* t */
    /*	Word enabled:1;  zt */
    /* Word visible:1;   zt  */
    int dialog;    		/* t */
    /* Word compressed:1 zt */
    /* Word reserved :8; zb */
	} RCWINDOWFLAGS;

typedef struct _rcwindow
	{
	int displayWidth;	       /* w */
	int displayHeight;	       /* w */
	/* int displayAddr;	        zl */
	RCWINDOWFLAGS windowFlags; /* ztztttztzttztzb */
	RCRECT windowBounds;  	   /* w4 */
	/* RCRECT clippingBounds;   zw4 */
	/* PointType viewOrigin;    zw2 */
	RCFRAMEBITS frameType;	   /* bzt4t2t2 */
	/* GraphicStatePtr gstate;  zl */
	/* struct WinTygpeStruct *nextWindow;  zl */
	} RCWINDOW;
#define szRCWINDOW "w,w,zl,ztzttt,ztzttzt,zb,w4,zw4,zw2,bzt4t2t2,zl,zl"

typedef struct _rclistattr
	{
	int usable;	    /* t */
	int enabled;    /* t */
	int visible;    /* t */
	int poppedUp;	/* t */
	/* int reserved; zt4,zb */
	} RCLISTATTR;


/* list.h */
typedef struct _rclist
	{
	int id;	             /* w */
	RCRECT bounds;       /* w4 */
	RCLISTATTR attr;     /* ttttzt4zb */
	char *itemsText;     /* p */
	int numItems;        /* w */
	/* Word currentItem;  zw */
	/* Word topItem;      zw */
	int	font;		     /* b,zb */
	/* WinHandle popupWin; zl */
	/* ListDrawDataFuncPtr	drawItemsCallback;  zl */

	/* private, not stored into file */
	int cbListItems;
	} RCLIST;
#define szRCLIST "w,w4,ttttzt4zb,p,w,zw,zw,bzb,zl,zl"

typedef struct _rcfieldattr
	{
	int usable;		  /* t   */
	int visible;	  /* t   */
	int editable;	  /* t   */
	int singleLine;	  /* t   */

	int hasFocus;     /* t   */
	int dynamicSize;  /* t   */
	int insPtVisible; /* t  */
	int dirty;		  /* t  */

	int underlined;	  /* t2    */
	int justification;/* t2	 */
	int autoShift;
	int hasScrollBar;
	int numeric;
					  /* zt1 */
	} RCFIELDATTR;

typedef struct _rcfield
	{
	int id;	              /* w */
	RCRECT rect;          /* w4 */
	RCFIELDATTR attr;     /* ttttttttt2t2tttzt1 */
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
	} RCFIELD;
#define szRCFIELD "w,w4,tttt,tttt,t2t2tttzt1,p,zl,zl,zw,zw,w,zw,zw,zw,zw,b,zb"


typedef struct RCTableColumnAttrType {
	int width; /* w */
	int usable; /* b,zb */
	int spacing; /* w */
   /*TableDrawItemFuncPtr    drawCallback;  zl */
   /* TableLoadDataFuncPtr    loadDataCallback;	 zl */
   /* TableSaveDataFuncPtr    saveDataCallback;	 zl */
} RCTABLECOLUMNATTR;
#define szRCTABLECOLUMNATTR "w,bzb,w,zl,zl,zl"

typedef struct RCTableRowAttrType 
	{
	int id;	/* w */
	int height; /* w */
	/*   DWord data;  zl */
   int usable; /* b */
   int selectable; /* b */
   int invalid; /* b,zb */
	} RCTABLEROWATTR;
#define szRCTABLEROWATTR "w,w,zl,b,b,b,zb"

/* this is bogus...don't know why */
#define szRCTABLEPADDING "zl,zl"

typedef struct RCTableAttrType 
	{
	int visible;  /* t */
	int editable; /* t */
	int editing;  /* t */
	int selected; /* t */
	} RCTABLEATTR;


typedef struct RCTableType 
	{
	int id;
	RCRECT bounds;
	RCTABLEATTR attr;
	int numColumns;
	int numRows;
	int currentRow;
	int currentColumn;
	int topRow;
	/*   TableColumnAttrType *   columnAttrs; */
	/* TableRowAttrType *      rowAttrs; */
   /* TableItemPtr            items; */
    RCFIELD currentField;
	/* not emitted */
	int *rgdxcol;
	} RCTABLE;
#define szRCTABLE "w,w4,ttttzt4,zb,w,w,w,w,w,zl,zl,zl"##szRCFIELD

typedef struct _rcformobjattr
	{
	int usable; /* t,zt7,zb (opt) */
	} RCFORMOBJATTR;

typedef struct _rcformlabel
	{
	int id;	            /* w */
	RCPOINT pos;        /* w2 */
	RCFORMOBJATTR attr; /*t,zt7 */
	int fontID;	        /* b */
	char *text;	        /* p */
	} RCFORMLABEL;
#define szRCFORMLABEL "w,w2,tzt7,zb,b,zb,p"

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


typedef struct _rcformgadget
	{
	int id;		        /* w */
	RCFORMOBJATTR attr; /* t,zt7,zb */
	RCRECT rect;	    /* w4 */
	/* VoidPtr data;     zl */
	} RCFORMGADGET;
#define szRCFORMGADGET "w,tzt7zb,w4,zl"

typedef struct _rcformbitmap
	{
	RCFORMOBJATTR attr;	/* t,zt7,zb */
	RCPOINT pos;	    /* w2 */
	int rscID;          /* w */
	} RCFORMBITMAP;
#define szRCFORMBITMAP "t,zt7,zb,w2,w"

typedef struct _rccontrolattr
	{
	int usable;	    /* t */
	int enabled;	/* t  */
	int visible;	/* t */
	int on;			/* t  */
	int leftAnchor;	/* t */
	int frame;		/* t3,zb */
	} RCCONTROLATTR;

typedef struct _rcontrol
	{
	int id;				/* w */
	RCRECT bounds;      /* w4 */
	char *text;         /* p */
	RCCONTROLATTR attr; /* tttttt3zb */
	int style;          /* b */
	int font  ;         /* b */
	int group;          /* b */
	} RCCONTROL;        
#define szRCCONTROL "w,w4,p,tttt,tt3,zb,b,b,b,zb"

typedef struct _rcscrollbarattr
	{
	int usable;
	int visible;
	int hilighted;
	int shown;
	int activeRegion;
	} RCSCROLLBARATTR;

typedef struct _rcscrollbar
	{
	RCRECT bounds;          /* w4 */
	int id;                 /* w */
	RCSCROLLBARATTR attr;   /* ttttt4zb */
	int value;				/* w */
	int minValue;           /* w */
	int maxValue;           /* w */
	int pageSize;           /* w */
	/*Short penPosInCar;*/	/* zw */
	/*Short savePos;*/      /* zw */ 
	} RCSCROLLBAR;
#define szRCSCROLLBAR "w4,w,ttttt4zb,w,w,w,w,zw,zw"

typedef union _rcformobject
	{                  
	void *                  ptr;
	RCFIELD *		    	field;
	RCCONTROL *		    	control;
	RCLIST	*		    	list;
	RCTABLE *		    	table;
	RCFORMBITMAP *	    	bitmap;
/*	RCFORMLINE *	    	line; */
/*	RCFORMFRAME *	    	frame; */
/*	RCFORMRECTANGLE *   	rectangle; */
	RCFORMLABEL *	    	label;
	RCFORMTITLE *	    	title;	
	RCFORMPOPUP *	    	popup;
	RCFORMGRAFFITISTATE *    grfState;
	RCFORMGADGET *	    	gadget;
	RCSCROLLBAR	*			scrollbar;
	} RCFORMOBJECT;    

typedef struct _rcformobjlist
	{
	int objectType; /* b,zb */
	union
		{
		RCFORMOBJECT object;	/* l */
		int ibobj;
		} u;
	} RCFORMOBJLIST;
#define szRCFORMOBJLIST "b,zb,l"

typedef struct _rcformattr
	{
	int usable;	      /* t */
	int enabled;      /* t */
	int visible;      /* t */
	int dirty;        /* t */
	int saveBehind;   /* t */
	int graffitiShift;/* t */
	/* int reserved			zt2,zt8,zt1,zt7zb; */
	} RCFORMATTR;

typedef struct _rcform
	{
	RCWINDOW window;
	int formId;	       /* w */
	RCFORMATTR attr;
	/* WinHandle bitsBehindForm;     zl */
	/* FormEventHandlerPtr handler;  zl */
	/* Word	focus;      zw */
	int defaultButton; /* w */
	int helpRscId;     /* w */
	int menuRscId;	   /* w */
	int numObjects;    /* w */
	/* FormObjListType *objects;     zl */
	} RCFORM;

#define szRCFORM szRCWINDOW##"w,tttttt,zt2zt8zt1zt7zb,zl,zl,zw,w,w,w,w,zl"


/*-----------------------------------------------------------------------------
|	MENUS
-------------------------------------------------------------WESC------------*/

typedef struct _rcmenuitem
	{
	int id;	        /* w */
	int command;    /* b,zb */
	char *itemStr;  /* l */
	} RCMENUITEM;
#define szRCMENUITEM "w,b,zb,l"

typedef struct _RCMENUPULLDOWN
	{
	/* WinHandle menuWin;     zl */
	RCRECT bounds;           /* w4 */
	/* WinHandle bitsBehind;  zl */
	RCRECT titleBounds;      /* w4 */
	char *title;	         /* l */
	int numItems;            /* w */
	RCMENUITEM * items;      /* l */
	} RCMENUPULLDOWN;
#define szRCMENUPULLDOWN "zl,w4,zl,w4,l,w,l"

typedef struct _RCMENUBARATTR
	{
	int visible;	/* t */
	/*WORD cmdPending	 zt */
	/*WORD insPtEnabled  zt */
	} RCMENUBARATTR;

typedef struct _RCMENUBAR
	{
	/*WinHandle barWin;		      zl    */
	/*WinHandle bitsBehind;       zl    */
	/*WinHandle savedActiveWin;	  zl */
	/*WinHandle bitsBehindStatus; zl */
	RCMENUBARATTR attr;	         /* tzt7zb */
	/*SWord curMenu;              zw */
	int curItem;	             /* w */
	/* SDWord commandTick;	      zl */
	int numMenus;	/* w          number of menus */
	/* MenuPullDownPtr menus; zl  array of menus */
	} RCMENUBAR;
#define szRCMENUBAR "zl,zl,zl,zl,tzt7zb,zw,w,zl,w,zl"


/*-----------------------------------------------------------------------------
|	ALERTS
-------------------------------------------------------------WESC------------*/

typedef struct _rcALERTTEMPLATE
	{
	int alertType;	   /* w */
	int helpRscID; 	   /* w */
	int numButtons;    /* w */
	int defaultButton; /* w */
	} RCALERTTEMPLATE;
#define szRCALERTTEMPLATE "w,w,w,w"


/*-----------------------------------------------------------------------------
|	BITMAP
-------------------------------------------------------------WESC------------*/
typedef struct _rcBITMAP
	{ /* bm */
    int cx;					/* w */
	int cy;					/* w */
	int cbRow;				/* w */
	int ff;					/* w */
    int pixelsize;          /* b */
    int version;            /* b */
    int nextDepthOffset;    /* w */
    int transparentIndex;   /* b */
    int compressionType;    /* b */
	/* ushort and_reserved_and_colorTable[3] */	/* z1w */
	unsigned char *pbBits;

	/* private, not stored into file */
	int cbDst;
	} RCBITMAP;
#define szRCBITMAP "w,w,w,w,b,b,w,b,b,zw"



/*-----------------------------------------------------------------------------
|	Other PILRC types and such
-------------------------------------------------------------WESC------------*/

//#define ifrmMax 32
DEFPL(PLEXFORMOBJLIST);
typedef struct _FRM
	{
	RCFORM form;
	PLEXFORMOBJLIST pllt;
//	RCFORMOBJLIST *rglt;
	} FRM;

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
	rwBTN, rwPBN, rwCBX, rwPUT, rwSLT, rwREP, rwSCL,

	rwForm, rwBegin, rwEnd, rwModal, rwSaveBehind, rwNoSaveBehind, rwHelpId, rwDefaultBtnId, rwMenuId, 
	rwEnabled, rwDisabled, rwUsable, rwNonUsable, rwLeftAnchor, rwRightAnchor, rwGroup, rwFont, 
	rwFrame, rwNoFrame, rwBoldFrame,
	
	rwEditable, rwNonEditable, rwUnderlined, rwSingleLine, rwMultipleLines, rwDynamicSize, rwLeftAlign, rwHasScrollBar, rwRightAlign, rwMaxChars,
	rwVisibleItems, rwAutoShift, rwNumeric,
	rwChecked,

	rwBitmap,
	rwBitmapGrey,
	rwBitmapGrey16,
	rwBitmapColor,
	rwBitmapFamily,

	rwPrevLeft, rwPrevRight, rwPrevWidth, rwPrevTop, rwPrevBottom, rwPrevHeight,
	rwMenu,
	rwPullDown,
	rwMenuItem,
	rwSeparator,

	rwValue,
	rwMinValue,
	rwMaxValue,
	rwPageSize,

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
	rwFile,
	rwApplicationIconName,
	rwApplication,
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
	                                     
	{"tBTN",     "button",           rwBTN},
	{"tPBN",     "pushbutton",       rwPBN},
	{"tCBX",     "checkbox",         rwCBX},/*  */
	{"tPUT",     "popuptrigger",     rwPUT},/* Popup trigger */
	{"tSLT",     "selectortrigger",  rwSLT},
	{"tREP",     "repeatbutton",     rwREP},

	{"form",		 "tFRM",         rwForm},
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
  
	{"checked",      "on",           rwChecked}, 
	{"bitmap",       NULL,           rwBitmap},
	{"bitmapgrey",   "bitmapgray",   rwBitmapGrey},
	{"bitmapgrey16", "bitmapgray16", rwBitmapGrey16},
	{"bitmapcolor",  "bitmapcolour", rwBitmapColor},
	{"bitmapfamily", NULL, 		 rwBitmapFamily},

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
	{"string",       "tSTR",         rwString},
	{"file",         "tSTR",         rwFile},
	{"applicationiconname", NULL,    rwApplicationIconName},
	{"application", "APPL",          rwApplication},
	{"categories",   "tAIS",         rwCategories},

	{"translation",  NULL,           rwTranslation},

	{"center",       NULL,           rwCenter},
	{"right",       NULL,           rwRight},
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

// 2.5b7 additions
	{"hex",          NULL,           rwHex},
	{"data",         NULL,           rwData},
// end 

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
extern BOOL vfVSErrors;
extern BOOL vfCheckDupes;
extern BOOL vfRTL;
extern char *szLanguage;


#define dxScreen 160
#define dyScreen 160
#define maxCategories  16
#define categoryLength 16

int CbEmitStruct(void *pv, char *szPic, char **ppchText, BOOL fEmit);
RCPFILE *ParseFile(char *szIn, char *szOutDir, char *szResFile, char *szIncFile, int fontType);
SYM *PsymLookupId(int id);

