/*
 * @(#)pilrc.c
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
 *     23-Jun-2000 Mark Garlanger
 *                 Additions to support #ifdef/#ifndef/#else/#endif in 
 *                 both *.rcp files and *.h files. 
 *     24-Jun-2000 Kurt Spaugh
 *                 Additions for tSTL resource support
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define EMITRWT	
#include "pilrc.h"
#include "bitmap.h"
#include "font.h"

#define idAutoInit 9999
#define idPalmOSReservedMin 10000

/*-----------------------------------------------------------------------------
|	 Globals
-------------------------------------------------------------WESC------------*/
/* Are we PilRCUI -- this was an attempt to merge pilrc and pilrcui */
BOOL vfWinGUI;

/* Variables to support #ifdef/#else/#endif */
int ifdefSkipping = 0;
int ifdefLevel    = 0;

/* Quiet output */
BOOL vfQuiet;
/* M$ (VS-type) error/warning output */
BOOL vfVSErrors;
/* Translations */
char *szLanguage;
TE *pteFirst;
/* RTL For Hebrew*/
BOOL vfRTL = fFalse;

/* next auto id */
int idAutoMac = idAutoInit;
BOOL vfAutoId;

/* Warning about duplicates in forms and menus */
/* if == 2 then we warn about duplicate labels too */
BOOL vfCheckDupes;

/* Form globals */
FRM *vpfrm;
//RCFORM form;	/* current form being parsed */
//#define cobjMax 256
//RCFORMOBJLIST rglt[cobjMax];


/* Menu globals */
RCMENUBAR menu;
#define imiMax 128	
RCMENUITEM rgmi[imiMax];
#define impdMax 32
RCMENUPULLDOWN rgmpd[impdMax];
int imiMac;	
int idMenu;
#define iidMenuMax 128
int iidMenuMac;
int rgidMenu[iidMenuMax];

#define iidStringMax 512
int iidStringMac;
int rgidString[iidStringMax];
#define iidAlertMax 512
int iidAlertMac;
int rgidAlert[iidAlertMax];
#define iidAISMax 128
int iidAISMac;
int rgidAIS[iidAISMax];



/* Symbol table */
SYM *psymFirst;

/* Parse globals */
BOOL fTokUngotten;
TOK tokPrev;
TOK tok;
int iline;
char szLine[4096];


/* Rect for Prev* keywords */
RCRECT rcPrev;

/*-----------------------------------------------------------------------------
|	AddSym
|	
|		Add symbol with value wVal to symbol table
|
|		Note! no check is made for previous existence
-------------------------------------------------------------WESC------------*/
VOID AddSym(char *sz, int wVal)
	{
	SYM *psym;

	psym = calloc(1, sizeof(SYM));
	psym->sz = strdup(sz);
	psym->wVal = wVal;
	psym->psymNext = psymFirst;
	psymFirst = psym;
	}



/*-----------------------------------------------------------------------------
|	PsymLookup
|	
|		Lookup symbol based on sz -- case sensitive
-------------------------------------------------------------WESC------------*/
SYM *PsymLookup(char *sz)
	{
	SYM *psym;

	for (psym = psymFirst; psym != NULL; psym = psym->psymNext)
		if (strcmp(psym->sz, sz) == 0)
			return psym;
	return NULL;
	}


int IdGetAutoId()
	{
	return idAutoMac--;
	}

SYM *PsymAddSymAutoId(char *sz)
	{
	SYM *psym;
	
	AddSym(sz, IdGetAutoId());
	psym = PsymLookup(tok.lex.szId);
	psym->fAutoId = fTrue;
	return psym;
	}

/*-----------------------------------------------------------------------------
|	FreeSymTable
|	
|	Free up all memory allocated for the symbol table
-------------------------------------------------------------WESC------------*/
VOID FreeSymTable()
	{
	SYM *psym;
	SYM *psymNext;

	for (psym = psymFirst; psym != NULL; psym = psymNext)
		{
		psymNext = psym->psymNext;
		free(psym->sz);
		free(psym);
		}
	psymFirst = NULL;
	}

/*-----------------------------------------------------------------------------
|	PchFromRw
|	
|		Maps a reserved word into a string
-------------------------------------------------------------WESC------------*/
char *PchFromRw(int rw, BOOL fTrySecondName)
	{
	RWT *prwt;
	
	prwt = rgrwt;
	while (prwt->sz1 != NULL)
		{
		if (prwt->rw == rw)
			{
			if (fTrySecondName && prwt->sz2 != NULL)
				return prwt->sz2;
			return prwt->sz1;
			}
		prwt++;
		}
	return NULL;
	}

	

/*-----------------------------------------------------------------------------
|	RwFromLex
|	
|		Looks up lex.id in reserved word table.  returns rwNil if not found
-------------------------------------------------------------WESC------------*/
RW RwFromLex(LEX *plex)
	{
	RWT *prwt;
	
	if (plex->lt != ltId)
		return rwNil;
	prwt = rgrwt;
	while (prwt->sz1 != NULL)
		{
		if (FSzEqI(plex->szId, prwt->sz1) || (prwt->sz2 != NULL && FSzEqI(plex->szId, prwt->sz2)))
			return prwt->rw;	
		prwt++;
		}
	return rwNil;
	}	


/*-----------------------------------------------------------------------------
|	PteFromSz
|	
|		Look up a language tranlation entry. All strings in the .rcp file
|	potentially get translated
-------------------------------------------------------------WESC------------*/
TE *PteFromSz(char *sz)
	{
	TE *pte;

	for (pte = pteFirst; pte != NULL; pte = pte->pteNext)
		if (strcmp(pte->szOrig, sz) == 0)
			return pte;
	return NULL;
	}

/*-----------------------------------------------------------------------------
|	FreeTranslations
|	
|		Free up the translation table
-------------------------------------------------------------WESC------------*/
VOID FreeTranslations()
	{
	TE *pte;
	TE *pteNext;

	for (pte = pteFirst; pte != NULL; pte = pteNext)
		{
		pteNext = pte->pteNext;
		free(pte->szOrig);
		free(pte->szTrans);
		free(pte);
		}
	pteFirst = NULL;
	}

/*-----------------------------------------------------------------------------
|	NextLine
|
|		Skip lexer ahead to the next line
-------------------------------------------------------------WESC------------*/
static BOOL NextLine(void)
	{
	BOOL    retval;

	retval = fFalse;

	szLine[0] = '\0';		/* just in case there is nothing to be gotten   */
	if (fgets(szLine, sizeof(szLine), vfhIn) != NULL)
		{
		iline++;
		retval = fTrue;
		}
	FInitLexer(szLine, fTrue);	/* so program can shut down gracefully      */

	return(retval);
	}

/*-----------------------------------------------------------------------------
|	FGetTok
|
|		Get the next token.  returns fFalse on EOF
|
|	Consistency issue -- takes a ptok, but some other other routines don't.
|	only one global tok...
-------------------------------------------------------------WESC------------*/
static BOOL FGetTok(TOK *ptok)
	{
	BOOL    fInComment;


	if (fTokUngotten)
		{
		*ptok = tokPrev;
		fTokUngotten = fFalse;
		return fTrue;
		}

	ptok->rw = rwNil;
	fInComment = fFalse;
	for (;;)
		{
		while (!FGetLex(&ptok->lex, fInComment))
			{
			if (!NextLine())
				{
				if (fInComment)
					ErrorLine("unexpected end of file during C-style comment");
				return fFalse;
				}
			}

		if (ptok->lex.lt == ltCComment)
			fInComment = fTrue;
		else if (ptok->lex.lt != ltEndCComment)
			break;
		else
			fInComment	= fFalse;
		}

	if (ptok->lex.lt == ltId)
		{
		/* check if it is a reserved word */
		ptok->rw = RwFromLex(&ptok->lex);
		}
	else if (ptok->lex.lt == ltStr)
		{
		TE *pte;
		/* attempt translation */
		pte = PteFromSz(ptok->lex.szId);
		if (pte != NULL)
			strcpy(ptok->lex.szId, pte->szTrans);
		}

	tokPrev = *ptok;
	return fTrue;
	}

/*-----------------------------------------------------------------------------
|	UngetTok
|	
|		Pushback one token.  Note! that this is 1 level only!
-------------------------------------------------------------WESC------------*/
VOID UngetTok()
	{
	tok = tokPrev;
	fTokUngotten = fTrue;
	}	


	
	
/*-----------------------------------------------------------------------------
|	GetExpectLt
|	
|		Get a token and expect a particular lex type.  Emit szErr if it isn't
|	what's expected
-------------------------------------------------------------WESC------------*/
VOID GetExpectLt(TOK *ptok, LT lt, char *szErr)
	{
	FGetTok(ptok);
	if (ptok->lex.lt != lt)
		{
		if (szErr == NULL)
			{
			if (lt == ltId)
				ErrorLine2("Syntax error : expecting identifier, got", ptok->lex.szId);
			else if (lt == ltStr)
				ErrorLine2("Syntax error : expecting string, got", ptok->lex.szId);			
			else if (lt == ltConst)
				ErrorLine2("Syntax error : expecting constant, got", ptok->lex.szId);
			else
				ErrorLine2("syntax error: ", ptok->lex.szId);
			}
		else
			{
			char szT[128];
			sprintf(szT, "expecting: %s, got", szErr);
			ErrorLine2(szT, ptok->lex.szId);
			}
		}
	}
	
/*-----------------------------------------------------------------------------
|	GetExpectRw
|	
|		Get and expect a particular  reserved word.  Emit "Expecting..." if next
|	token isn't rw
-------------------------------------------------------------WESC------------*/
VOID GetExpectRw(RW rw)
	{
	TOK tok;
	
	FGetTok(&tok);
	if (tok.rw != rw)
		{
		char szErr[64];
		sprintf(szErr, "%s expected, got", PchFromRw(rw, fTrue));
		ErrorLine2(szErr, tok.lex.szId);
		}
	}

/*-----------------------------------------------------------------------------
|	PchGetSz
|	
|		Get a quoted string.  return dup'ed string.  (remember to free!)
-------------------------------------------------------------WESC------------*/
char *PchGetSz(char *szErr)
	{
	GetExpectLt(&tok, ltStr, szErr);
	return strdup(tok.lex.szId);
	}

#ifdef DOESNTWORK
/* attempt at allowing GCC preprocessed string files */
/*-----------------------------------------------------------------------------
|	PchGetSzMultiLine
|	
|   gets strings on multiple lines w/ \ continuation character.
-------------------------------------------------------------WESC------------*/
char *PchGetSzMultiLine(char *szErr)
	{
	char sz[16384];

	GetExpectLt(&tok, ltStr, szErr);
	strcpy(sz, tok.lex.szId);
	while (FGetTok(&tok))
		{
		if (tok.lex.lt == ltStr)
			{
			strcat(sz, tok.lex.szId);
			}
		else if (tok.lex.lt != ltBSlash)
			{
			UngetTok();
			break;
			}
		else
			{
			GetExpectLt(&tok, ltStr, szErr);
			strcat(sz, tok.lex.szId);
			}
		w
	return strdup(sz);
	}
#else
/*-----------------------------------------------------------------------------
|	PchGetSzMultiLine
|	
|   gets strings on multiple lines w/ \ continuation character.
-------------------------------------------------------------WESC------------*/
char *PchGetSzMultiLine(char *szErr)
	{
	char sz[8192];

	GetExpectLt(&tok, ltStr, szErr);
	strcpy(sz, tok.lex.szId);
	while (FGetTok(&tok))
		{
		if (tok.lex.lt != ltBSlash)
			{
			UngetTok();
			break;
			}
		GetExpectLt(&tok, ltStr, szErr);
		strcat(sz, tok.lex.szId);
		}
	return strdup(sz);
	}
#endif

/*-----------------------------------------------------------------------------
|	PchGetId
|	
|		Epect and get an ident. (ltId)
-------------------------------------------------------------WESC------------*/
char *PchGetId(char *szErr)
	{
	GetExpectLt(&tok, ltId, szErr);
	return strdup(tok.lex.szId);
	}


/*-----------------------------------------------------------------------------
|	WGetConst
|	
|		Get an integer constant or one of the Prev* reserved words, returning
|	valu	e.
|		
-------------------------------------------------------------WESC------------*/
int WGetConst(char *szErr)
	{
	char sz[256];

	if (!FGetTok(&tok))
		ErrorLine("unexpected end of file");
	switch (tok.rw)
		{
	default:
		if (tok.lex.lt == ltId)
			{
			SYM *psym;

			psym = PsymLookup(tok.lex.szId);
			if (psym == NULL)
				{
				if (vfAutoId)
					{
					psym = PsymAddSymAutoId(tok.lex.szId);
					}
				else
					{
					sprintf(sz, "Expecting %s, got unknown symbol:", szErr);
					ErrorLine2(sz, tok.lex.szId);
					}
				}
			return psym->wVal;
			}
		if (tok.lex.lt != ltConst)
			{
			sprintf(sz, "%s expected, got", szErr);
			ErrorLine2(sz, tok.lex.szId);
			}
		return tok.lex.val;
	case rwPrevLeft:
		return  rcPrev.topLeft.x;
	case rwPrevRight:
		return  rcPrev.topLeft.x+rcPrev.extent.x;
	case rwPrevWidth:
		return rcPrev.extent.x;
	case rwPrevTop:
		return  rcPrev.topLeft.y;
	case rwPrevBottom:
		return  rcPrev.topLeft.y+rcPrev.extent.y;
	case rwPrevHeight:
		return rcPrev.extent.y;
		}
	}


	
int WGetConstEx(char *szErr);
	
	
/*-----------------------------------------------------------------------------
| WGetConstExFactor
| 
| Get a constant expression -- parens allowed left to right associativity
-------------------------------------------------------------WESC------------*/
int WGetConstExFactor(char *szErr) 
	{
	int wVal;

	if (FGetTok(&tok)) 
		{
		if (tok.lex.lt == ltLParen) 
			{
			wVal = WGetConstEx(szErr);
			if (!FGetTok(&tok) || tok.lex.lt != ltRParen)
				ErrorLine("Expected but didn't get ')'!");
			} 
		else 
			{
			UngetTok();
			wVal = WGetConst(szErr);
			}
		}
	else
		{
		wVal = WGetConst(szErr); /* aka hack */
		}

	return wVal;
	}
		
		
		
/*-----------------------------------------------------------------------------
| WGetConstEx
| 
| Get a constant expression -- parens allowed left to right associativity
-------------------------------------------------------------WESC------------*/
int WGetConstEx(char *szErr)
	{
	int wValT;
	int wVal;
	LT ltOp;
	
	wVal = WGetConstExFactor(szErr);


	for (;;)
		{
		if(!FGetTok(&tok)) 
			return wVal;
			
		switch (tok.lex.lt) 
			{
		default:
			UngetTok();
			return wVal;
    
		case ltPlus:
		case ltMinus:
		case ltMult:
		case ltDiv:
		case ltPipe:
			ltOp = tok.lex.lt;
			wValT = WGetConstExFactor(szErr);
			switch (ltOp) 
				{
			case ltPlus:
				wVal += wValT;
				break;
			case ltMinus:
				wVal -= wValT;
				break;
			case ltMult:
				wVal *= wValT;
				break;
			case ltDiv:
				if (wValT == 0)
					ErrorLine("Divide By Zero!");
				wVal /= wValT;
				break;
			case ltPipe:
				if (vfRTL)
					wVal = wValT;
				break;
				}
			}
	}
	return wVal;
	}
	

/*-----------------------------------------------------------------------------
|	Various Konstant types -- basically deferred evaluation of constants
|	mainly for AUTO and CENTER because we can't evaluate them until we know
|	the font for the particular item.
-------------------------------------------------------------WESC------------*/

/* Konstant Type */
typedef enum _kt
	{
	ktConst,
	ktCenter,
	ktAuto,
	ktCenterAt,
	ktRightAt,
	ktBottomAt,
	} KT;

/* Konstant */
typedef struct _k
	{
	KT kt;
	int wVal;
	} K;

/* Konstant Point */
typedef struct _kpt
	{
	K kX;
	K kY;
	} KPT;

/* Konstant Rect */
typedef struct _krc
	{
	KPT kptUpperLeft;
	KPT kptExtent;
	} KRC;

	

/*-----------------------------------------------------------------------------
|	KtGetK
|	
|		Get a Konstant, returning the type.
-------------------------------------------------------------WESC------------*/
KT KtGetK(K *pk, char *szErr)
	{
	if(!FGetTok(&tok))
		ErrorLine("Unexpected end of file");
	switch (tok.rw)
		{
	default:
		UngetTok();
		pk->kt = ktConst;
		pk->wVal = WGetConstEx(szErr);
		break;
	case rwCenter:
		pk->kt = ktCenter;
		if (FGetTok(&tok))
			{
			if (tok.lex.lt == ltAt)
				{
				pk->wVal = WGetConstEx("CENTER@ position")*2;	/* we store extent */
				pk->kt = ktCenterAt;
				}
			else
				UngetTok();
			}
			
		break;
	case rwAuto:
		pk->kt = ktAuto;
		break;
	case rwRight:
		pk->kt = ktRightAt;
		GetExpectLt(&tok, ltAt, "@");
		pk->wVal = WGetConstEx("RIGHT@ position");	
		break;				
	case rwBottom:
		pk->kt = ktBottomAt;
		GetExpectLt(&tok, ltAt, "@");
		pk->wVal = WGetConstEx("BOTTOM@ position");	
		break;				
		}
	return pk->kt;
	}


/*-----------------------------------------------------------------------------
|	ParseKpt
|	
|		Parse a point (not the AT and () parts).
-------------------------------------------------------------WESC------------*/
VOID ParseKpt(KPT *pkpt)
	{
	KtGetK(&pkpt->kX, "x pos");
	KtGetK(&pkpt->kY, "y pos");
	}

/*-----------------------------------------------------------------------------
|	ParseKrc
|	
|		Parse a rect (not the AT and () parts).
-------------------------------------------------------------WESC------------*/
VOID ParseKrc(KRC *pkrc)
	{
	KtGetK(&pkrc->kptUpperLeft.kX, "rect left");
	KtGetK(&pkrc->kptUpperLeft.kY, "rect top");
	KtGetK(&pkrc->kptExtent.kX, "rect width");
	KtGetK(&pkrc->kptExtent.kY, "rect height");
	}


/*-----------------------------------------------------------------------------
|	ITM
|		an item in a form -- grif and grif2 define the syntax of the item
|	and what to expect.
-------------------------------------------------------------WESC------------*/
typedef struct _itm
	{
	int grif;
	int grifOut;
	int grif2;
	int grif2Out;
	char *text;
	int cbText;		/* length of text including nul terminator */
	int id;
	int listid;
	KRC krc;
	/* RectangleType rc; */
	RCRECT rc;
	KPT kpt;
	/*PointType pt; */
	RCPOINT pt;
	BOOL usable;
	BOOL leftAnchor;
	int frame;
	BOOL enabled;
	BOOL on;	/* checked */
	BOOL editable;
	BOOL underlined;
	BOOL singleLine;
	BOOL dynamicSize;
	int justification;
	int maxChars;
	int autoShift;
	BOOL hasScrollBar;
	BOOL numeric;
	int numItems;
	int cvis;
	int group;
	int font;
	int rscID;
	BOOL modal;
	BOOL saveBehind;
	int helpId;
	int defaultBtnId;
	int menuId;
	int numRows;
	int numColumns;
	int rgdxcol[64];
	int value;	/* scrollbar */
	int minValue; /* scrollbar */
	int maxValue; /* scrollbar */
	int pageSize; /* scrollbar */
	} ITM;

/* Item Flags */
#define ifText         0x00000001
#define ifMultText     0x00000002
#define ifId           0x00000004
#define ifRc           0x00000008
#define ifPt           0x00000010
#define ifUsable       0x00000020
#define ifAnchor       0x00000040
#define ifFrame        0x00000080
#define ifEnabled      0x00000100
#define ifOn           0x00000200
#define ifEditable     0x00000400
#define ifSingleLine   0x00000800
#define ifDynamicSize  0x00001000
#define ifMaxChars     0x00002000
#define ifCvis         0x00004000
#define ifGroup        0x00008000
#define ifFont         0x00010000
#define ifAlign        0x00020000
#define ifUnderlined   0x00040000
#define ifListId       0x00080000
#define ifBitmap       0x00100000

/* Form ifs */
#define ifModal        0x00200000
#define ifSaveBehind   0x00400000
#define ifHelpId       0x00800000
#define ifDefaultBtnId 0x01000000
#define ifMenuId       0x02000000

/* Ifs defining margins -- extra width to add to an item in addition to it's string width */
#define ifSmallMargin  0x80000000
#define ifBigMargin    0x40000000

/* if2s -- ran out of bits in if! */
#define if2Null        0x00000000
#define if2NumColumns  0x00000001
#define if2NumRows     0x00000002
#define if2ColumnWidths 0x00000004
#define if2Value        0x00000008
#define if2MinValue     0x00000010
#define if2MaxValue     0x00000020
#define if2PageSize     0x00000040
#define if2AutoShift    0x00000080
#define if2Scrollbar    0x00000100
#define if2Numeric      0x00000200


/* Semi-arbitrary margins */
#define dxObjSmallMargin 3
#define dxObjBigMargin 6


/*-----------------------------------------------------------------------------
|	WResolveK
|	
|		Resolve a Konstant to it's real value, returning it.
-------------------------------------------------------------WESC------------*/
int WResolveK(K *pk, ITM *pitm, int dxyExtent, BOOL fHoriz)
	{
	int wVal;
	int dxyCenterAcross;

	switch (pk->kt)
		{
	default:
		Assert(fFalse);
		return 0;
	case ktConst:
		return pk->wVal;
	case ktAuto:
		if (fHoriz)
			{
			wVal = DxCalcExtent((unsigned char *)pitm->text, pitm->font);
			if (pitm->grif & ifSmallMargin)
				wVal += 2*dxObjSmallMargin; 
			if (pitm->grif & ifBigMargin)
				wVal += 2*dxObjBigMargin; 
			}
		else
			wVal = DyFont(pitm->font)+1;	
		return wVal;
	case ktCenter:
		if (fHoriz)
			dxyCenterAcross = vpfrm->form.window.windowBounds.extent.x;
		else
			dxyCenterAcross = vpfrm->form.window.windowBounds.extent.y;
		wVal = (dxyCenterAcross-dxyExtent)/2;
		return wVal;
	case ktCenterAt:
		dxyCenterAcross = pk->wVal;
		wVal = (dxyCenterAcross-dxyExtent)/2;
		return wVal;
	case ktRightAt:
	case ktBottomAt:
		wVal = pk->wVal-dxyExtent;
		return wVal;
		}
	Assert(fFalse);
	}

/*-----------------------------------------------------------------------------
|	ResolveKrcKpt
|	
|		Resolve a Krc and/or Kpt, setting rcPrev
-------------------------------------------------------------WESC------------*/
VOID ResolveKrcKpt(ITM *pitm)
	{
	if (pitm->grif & ifRc)
		{
		pitm->rc.extent.x = WResolveK(&pitm->krc.kptExtent.kX, pitm, 0, fTrue);
		pitm->rc.extent.y = WResolveK(&pitm->krc.kptExtent.kY, pitm, 0, fFalse);
		
		pitm->rc.topLeft.x = WResolveK(&pitm->krc.kptUpperLeft.kX, pitm, pitm->rc.extent.x, fTrue);
		pitm->rc.topLeft.y = WResolveK(&pitm->krc.kptUpperLeft.kY, pitm, pitm->rc.extent.y, fFalse);
		rcPrev = pitm->rc;
		}

	if (pitm->grif & ifPt)
		{
		K kT;
		int xT;
		int yT;

		kT.kt = ktAuto;
		xT = WResolveK(&kT, pitm, 0, fTrue);
		yT = WResolveK(&kT, pitm, 0, fFalse);

		pitm->pt.x = WResolveK(&pitm->kpt.kX, pitm, xT, fTrue);
		pitm->pt.y = WResolveK(&pitm->kpt.kY, pitm, yT, fFalse);
		rcPrev.topLeft = pitm->pt;
		rcPrev.extent.x = xT;
		rcPrev.extent.y = yT;
		}
	}


/*-----------------------------------------------------------------------------
|	DoCheckGrif
|	
| Check if ifP is set in grif -- error if not
-------------------------------------------------------------WESC------------*/
VOID DoCheckGrif(int grif, int ifP)
	{
	if (!(grif & ifP))
		{
		ErrorLine2(tok.lex.szId, "unexpected");
		}
	}

/*-----------------------------------------------------------------------------
|	WGetId
|	
|		Parse ID <const>
-------------------------------------------------------------WESC------------*/
int WGetId(char *szErr, BOOL fAutoIDOk)
	{
	int w;

	fAutoIDOk = fAutoIDOk; /* shut up whiney compilers */
	if (!FGetTok(&tok))
		return 0;
	if (tok.rw == rwAutoId)
		w = IdGetAutoId();
	else
		{
		if (tok.rw != rwId)
			UngetTok();
		w = WGetConstEx(szErr);
		}
	if (w >= idPalmOSReservedMin)
		WarningLine("ID conflicts with PalmOS reserved ID range (valid values: 0-9999, Ok for predefined IDs for Edit menu)");
	return w;
	}

#define CheckGrif(ifP) do {DoCheckGrif(pitm->grif, ifP); pitm->grifOut |= ifP;} while (0);
#define CheckGrif2(ifP) do {DoCheckGrif(pitm->grif2, ifP); pitm->grif2Out |= ifP;} while (0);
/*-----------------------------------------------------------------------------
|	ParseItm
|	
|		Parse an item in a form
|	Arguments:
|		ITM *pitm
|		int grif : valid attributes for this item
|		int grif2: more valid attributes for this item
-------------------------------------------------------------WESC------------*/
void ParseItm(ITM *pitm, int grif, int grif2)
	{
	BOOL fAt;
	int icol;

	memset(pitm, 0, sizeof(ITM));
	
	pitm->grif = grif;
	pitm->grif2 = grif2;
	/* defaults */
	pitm->usable = 1;
	pitm->frame = 1;
	pitm->enabled = 1;
	pitm->justification = leftAlign;
	pitm->singleLine = 1;
	pitm->editable = 1;
	pitm->cvis = -1;
	pitm->saveBehind = 1;
	
	if (grif & ifText)
		{
		pitm->grifOut |= ifText;
		pitm->text = PchGetSz("item string");
		pitm->cbText = strlen(pitm->text)+1;
		}
	if (grif & ifMultText)
		{
		char *pch;
		char rgb[16384];

		pitm->grifOut |= ifMultText;
		GetExpectLt(&tok, ltStr, "text");
		pch = rgb;
		for(;;)
			{
			strcpy(pch, tok.lex.szId);
			pch = pch+strlen(tok.lex.szId)+1;
			pitm->numItems++;
			if (!FGetTok(&tok))
				return;
			if (tok.lex.lt != ltStr)
				break;
			}
		pitm->text = malloc(pch-rgb);
		pitm->cbText = pch-rgb;
		memcpy(pitm->text, rgb, pch-rgb);
		UngetTok();
		}

	if (grif & ifId)
		{
		pitm->grifOut |= ifId;
		pitm->id = WGetId("ItemId", fTrue);
		}
	if (grif & ifListId)
		{
		pitm->grifOut |= ifListId;
		pitm->listid = WGetConstEx("ListId");
		}

	fAt = fFalse;
	if (grif & (ifRc|ifPt))
		{
		if (!FGetTok(&tok))
			return;
		if (tok.rw == rwAt)
			{
			fAt = fTrue;
			GetExpectLt(&tok, ltLParen, "(");
			}
		else
			UngetTok();
		}
	if (grif & ifRc)
		{
		pitm->grifOut |= ifRc;
		ParseKrc(&pitm->krc);
		}
	if (grif & ifPt)
		{
		pitm->grifOut |= ifPt;
		ParseKpt(&pitm->kpt);
		}
	if (fAt)
		GetExpectLt(&tok, ltRParen, ")");
	
	while (FGetTok(&tok))
		{
		switch (tok.rw)
			{
		default:
			ResolveKrcKpt(pitm);
			UngetTok();
			return;
		case rwUsable:								
		case rwNonUsable:
			CheckGrif(ifUsable);
			pitm->usable = tok.rw == rwUsable;
			break;
		case rwLeftAnchor:	/* default? */
		case rwRightAnchor:
			CheckGrif(ifAnchor);
			pitm->leftAnchor = tok.rw == rwLeftAnchor;
			break;
		case rwGroup:
			CheckGrif(ifGroup);
			pitm->group = WGetConstEx("GroupId"); 
			break;
		case rwFont:	/* Add rwBoldFont, rwLargeFont? */
			CheckGrif(ifFont);
			pitm->font = WGetConstEx("FontId");
			break;
		case rwFrame:					/* BUG! other frame types */
		case rwNoFrame:
			CheckGrif(ifFrame);
			pitm->frame = tok.rw == rwFrame;
			break;
		case rwBoldFrame:
			CheckGrif(ifFrame);
			pitm->frame = boldButtonFrame;
			break;
		case rwDisabled:
		case rwEnabled:	
			CheckGrif(ifEnabled);
			pitm->enabled = tok.rw == rwEnabled;
			break;
		case rwChecked:
			CheckGrif(ifOn);
			pitm->on = fTrue;
			break;
		case rwEditable:
		case rwNonEditable:
			CheckGrif(ifEditable);
			pitm->editable = tok.rw == rwEditable;
			break;
		case rwSingleLine:
		case rwMultipleLines:
			CheckGrif(ifSingleLine);
			pitm->singleLine = tok.rw == rwSingleLine;
			break;
		case rwDynamicSize:
			CheckGrif(ifDynamicSize);
			pitm->dynamicSize = 1;
			break;
		case rwMaxChars:
			CheckGrif(ifMaxChars);
			pitm->maxChars = WGetConstEx("MaxChars");
			break;
		case rwLeftAlign:	/* default? */
			CheckGrif(ifAlign);
			pitm->justification = leftAlign;
			break;
		case rwRightAlign:
			CheckGrif(ifAlign);
			pitm->justification = rightAlign;
			break;
		case rwUnderlined:
			CheckGrif(ifUnderlined);
			pitm->underlined = 1;	/* WES! try 2 & 3! */
			break;
		case rwVisibleItems:
			CheckGrif(ifCvis);
			pitm->cvis = WGetConstEx("VisibleItems");
			break;
		case rwValue:
			CheckGrif2(if2Value);
			pitm->value = WGetConstEx("Value");
			break;
		case rwMinValue:
			CheckGrif2(if2MinValue);
			pitm->minValue = WGetConstEx("MinValue");
			break;
		case rwMaxValue:
			CheckGrif2(if2MaxValue);
			pitm->maxValue = WGetConstEx("MaxValue");
			break;
		case rwPageSize:
			CheckGrif2(if2PageSize);
			pitm->pageSize = WGetConstEx("PageSize");
			break;

		case rwBitmap:
			CheckGrif(ifBitmap);
			pitm->rscID = WGetConstEx("BitmapId");
			break;
		case rwModal:
			CheckGrif(ifModal);
			pitm->modal = 1;
			break;
		case rwSaveBehind:
			CheckGrif(ifSaveBehind);
			pitm->saveBehind = 1;
			break;
		case rwNoSaveBehind:
			CheckGrif(ifSaveBehind);
			pitm->saveBehind = 0;
			break;
		case rwHelpId:
			CheckGrif(ifHelpId);
			pitm->helpId = WGetConstEx("HelpId");
			break;
		case rwDefaultBtnId:
			CheckGrif(ifDefaultBtnId);
			pitm->defaultBtnId = WGetConstEx("DefaultButtonId");
			break;
		case rwMenuId:
			CheckGrif(ifDefaultBtnId);
			pitm->menuId = WGetConstEx("MenuId");
			break;
		case rwNumColumns:
			CheckGrif2(if2NumColumns);
			pitm->numColumns = WGetConstEx("NumColumns");
			break;
		case rwNumRows:
			CheckGrif2(if2NumRows);
			pitm->numRows = WGetConstEx("NumRows");
			break;
		case rwColumnWidths:
			CheckGrif2(if2ColumnWidths);
			for (icol = 0; icol < pitm->numColumns; icol++)
				pitm->rgdxcol[icol] = WGetConstEx("ColumnWidth"); 
			break;
		case rwAutoShift:
			CheckGrif2(if2AutoShift);
			pitm->autoShift = 1;
			break;
		case rwHasScrollBar:
			CheckGrif2(if2Scrollbar);
			pitm->hasScrollBar = 1;
			break;
		case rwNumeric:
			CheckGrif2(if2Numeric);
			pitm->numeric = 1;
			break;
			}
		}			
	}
		

#define CondEmitB(b) do {if (fEmit) EmitB(b);} while (0)
#define CondEmitW(w) do {if (fEmit) EmitW(w);} while (0)
#define CondEmitL(l) do {if (fEmit) EmitL(l);} while (0)

/*-----------------------------------------------------------------------------
|	CbEmitStruct
|	
|		Emit a struct (RC*).  It is assumed that the members of the struct
|	are integers or pointers only.  Further it is assumed that pointers and
|	integers are the same size.
|	
|	Arguments:
|		void *pv : struct to emit
|		char *szPic: format picture -- see pilrc.h for format
|		char **ppchText: if szPic contains a 'p' then return that pointer here
|		BOOL fEmit: Actually emit the bytes
|	
|	Returns:
|		size of the structure
-------------------------------------------------------------WESC------------*/
int CbEmitStruct(void *pv, char *szPic, char **ppchText, BOOL fEmit)
	{
	char *pch;
	int *pi;
	int ibit;
	unsigned char byte;
	int cb;

	if (ppchText != NULL)
		*ppchText = NULL;
	cb = 0;
	ibit = 0;
	byte = 0;
	Assert(sizeof(char *) == sizeof(int));
	pi = (int *)pv;
	for (pch = szPic; *pch != 0;)
		{
		int ch;
		int c;
		BOOL fZero;

		if (*pch == ',')
			{
			pch++;
			continue;
			}
		fZero = *pch == 'z';
		if (fZero)
			pch++;
		ch = *pch++;
		c = 0;
		while (isdigit(*pch))
			{
			c = 10*c + (*pch-'0');
			pch++;
			}
		if (c == 0)
			c = 1;
		switch (ch)
			{
		default:
			Assert(fFalse);
			break;
		case 'b':	/* byte (8 bit) */
			while(c--)
				{
				if (fZero)
					CondEmitB(0);
				else
					{
					CondEmitB((unsigned char) *pi);
					pi++;
					}
				cb += 1;
				}
			break;
		case 'w':	/* word (16 bit) */
			while(c--)
				{
				if (fZero)
					CondEmitW(0);
				else
					{
					CondEmitW((unsigned short) *pi);
					pi++;
					}
				cb += 2;
				}
			break;
		case 'p':	/* text (pointer) */
			Assert(c == 1);
			if (ppchText != NULL)
				*ppchText = *(char **)pi;
			if (!fZero)
				pi++;
			fZero = fTrue;
			/* fall thru */
		case 'l':	/* long (32bit) */
			while(c--)
				{
				if (fZero)
					CondEmitL(0);
				else
					{
					CondEmitL(*pi);
					pi++;
					}
				cb += 4;
				}
			break;
		case 't':	/* bit */
			ibit += c;
			byte = (unsigned char)(byte << c);
			if (!fZero && pi != NULL)
				byte |= *pi++;
			if (ibit == 8)
				{
				CondEmitB(byte);
				cb++;
				byte = 0;
				ibit = 0;
				}
			break;
			}
		}
	Assert(ibit == 0);
	return cb;
	}

/*-----------------------------------------------------------------------------
|	CbStruct
|		
|		returns the size of a pilot struct given it's format pic
-------------------------------------------------------------WESC------------*/
int CbStruct(char *szPic)
	{
	return CbEmitStruct(NULL, szPic, NULL, fFalse); 
	}



/* mapping of form object types to resource ids */
char * mpotszEmit[] =
	{
	szRCFIELD,	/* 'tFLD', */
	szRCCONTROL,	/* 'tCTL',	 need to special case this one! */
	szRCLIST,	/* 'tLST', */
	szRCTABLE, /*	szRCTBL'tTBL', */
	szRCFORMBITMAP, /* 'tFBM', */
	NULL, /* 'tLIN',		 bogus */
	NULL, /* 'tFRA',		 bogus */
	NULL, /* 'tREC',      bogus */
	szRCFORMLABEL, /* 'tLBL', */
	szRCFORMTITLE, /* 'tTTL', */
	szRCFORMPOPUP, /* 'tPUL', */
	szRCFORMGRAFFITISTATE, /* 'tGSI', */
	szRCFORMGADGET, /* 'tGDT' */
	szRCSCROLLBAR,  /* 'tSCL' */
	};




/*-----------------------------------------------------------------------------
|	CbFromLt
|	
|		Return emitted size of the form item.  if fText then include the
|	length of the associated item text
-------------------------------------------------------------WESC------------*/
int CbFromLt(RCFORMOBJLIST *plt, int fText)
	{
	int cb;
	RCFORMOBJECT *pobj;
	char *pchText;
	
	cb = 0;
	pobj = &plt->u.object;
	pchText = NULL;
	cb = CbEmitStruct(pobj->ptr, mpotszEmit[plt->objectType], &pchText, fFalse);
	switch (plt->objectType)
		{
	case frmListObj:
		pchText = NULL;
		cb += pobj->list->numItems * 4 + pobj->list->cbListItems;
		break;
	case frmTableObj:
		pchText = NULL;
		cb += pobj->table->numColumns*CbStruct(szRCTABLECOLUMNATTR)+pobj->table->numRows*CbStruct(szRCTABLEROWATTR)+
			pobj->table->numColumns*pobj->table->numRows*CbStruct(szRCTABLEPADDING);
			;
		break;
		}

	if (fText && pchText != NULL)
		cb += strlen(pchText)+1;	
	if (cb & 1)
		cb++;
	return cb;
	}
	


	
/*-----------------------------------------------------------------------------
|	DumpForm
-------------------------------------------------------------WESC------------*/
void DumpForm(FRM *pfrm)
	{
	int cbDirectory;
	int clt;
	int ilt;
	RCFORMOBJLIST lt;
	int ib;
	int il;


	OpenOutput("tFRM", pfrm->form.formId);
	clt = pfrm->form.numObjects;	
	Assert(PlexGetCount(&pfrm->pllt) == clt);

	CbEmitStruct(&pfrm->form, szRCFORM, NULL, fTrue);
    
	cbDirectory = CbEmitStruct(&lt, szRCFORMOBJLIST, NULL, fFalse)*clt;
	ib = IbOut()+cbDirectory;
	for (ilt = 0; ilt < clt; ilt++)
		{
		int cb;
		
		lt = *(RCFORMOBJLIST *)PlexGetElementAt(&pfrm->pllt, ilt);
		cb = CbFromLt(&lt, 1);
		lt.u.ibobj = ib;
		ib += cb;
		CbEmitStruct(&lt, szRCFORMOBJLIST, NULL, fTrue);
		}
	/* now dump the controls themselves		 */
	for (ilt = 0; ilt < clt; ilt++)
		{
		int cbLt;
		char *pchText;						  
		RCFORMOBJECT *pobj;
		RCFORMOBJLIST *plt;
		
		PadWordBoundary();					
		plt = PlexGetElementAt(&pfrm->pllt, ilt);
		
		pobj = &plt->u.object;
		cbLt = CbFromLt(plt, 0);
		pchText = NULL;
		CbEmitStruct(pobj->ptr, mpotszEmit[plt->objectType], &pchText, fTrue);
		switch (plt->objectType)
			{
		case frmListObj:
			pchText = NULL;	/* we dump it ourselves */
			for (il = 0; il < pobj->list->numItems; il++)
				DumpBytes(rgbZero, sizeof(unsigned long));
			DumpBytes(pobj->list->itemsText, pobj->list->cbListItems);
			break;
		case frmTableObj:
			{
			RCTABLECOLUMNATTR colattr;
			RCTABLEROWATTR rwattr;
			int irw;
			int icol;

			memset(&colattr, 0, sizeof(colattr));
			colattr.spacing = 1;	/* ?? */
			for (icol = 0; icol < pobj->table->numColumns; icol++)
				{
				colattr.width = pobj->table->rgdxcol[icol];
				CbEmitStruct(&colattr, szRCTABLECOLUMNATTR, NULL, fTrue);
				}
			memset(&rwattr, 0, sizeof(rwattr));
			rwattr.height = 0x0b;	/* ?? */
			rwattr.usable = 1;
			rwattr.selectable = 1;
			for (irw = 0; irw < pobj->table->numRows; irw++)
				{
				CbEmitStruct(&rwattr, szRCTABLEROWATTR, NULL, fTrue);
				}
			/* emit bogus padding */
			for (irw = 0; irw < pobj->table->numRows*pobj->table->numColumns; irw++)
				CbEmitStruct(NULL, szRCTABLEPADDING, NULL, fTrue);

			break;
			}
			}
		if (pchText != NULL)
			{
			PadWordBoundary();					
			DumpBytes(pchText, strlen(pchText)+1);
			}
		}
	CloseOutput();
	}

BOOL FIdFormObject(FormObjectKind kind, BOOL fIncludeLabels)
	{	
	switch (kind)
		{
	case frmFieldObj:
	case frmControlObj:
	case frmListObj:
	case frmTableObj:
	
	case frmScrollbarObj:
	case frmGadgetObj:
		return fTrue;
	case frmLabelObj:
		return fIncludeLabels;
	case frmTitleObj:
	case frmPopupObj:
	case frmLineObj:
	case frmFrameObj:
	case frmRectangleObj:
	case frmGraffitiStateObj:	
	case frmBitmapObj:
		return fFalse;
		}
	Assert(fFalse);
	return fFalse;
	}		

/* Grrr, the PalmOS developers could have been intelligent created a common header
 for all form objects */
int IdFromObj(RCFORMOBJECT *pobj, FormObjectKind kind)
	{
	switch (kind)
		{
	case frmFieldObj:
		return pobj->field->id;
		
	case frmControlObj:
		return pobj->control->id;
	case frmListObj:
		return pobj->list->id;
	case frmTableObj:
		return pobj->table->id;
	case frmScrollbarObj:
		return pobj->scrollbar->id;
	case frmGadgetObj:
		return pobj->gadget->id;
	case frmLabelObj:
		return pobj->label->id;
		
	case frmTitleObj:
	case frmPopupObj:
	case frmLineObj:
	case frmFrameObj:
	case frmRectangleObj:
	case frmGraffitiStateObj:	
	case frmBitmapObj:
		Assert(fFalse);
		return -1;
		}
	Assert(fFalse);
	return fFalse;
	}
	
/*-----------------------------------------------------------------------------
|	AddObject
|	
|		Add an object (item) to the current form
-------------------------------------------------------------WESC------------*/
VOID AddObject(RCFORMOBJECT *pobj, FormObjectKind kind)
	{
	RCFORMOBJLIST lt;
	int iobj;

//	if (vpfrm->form.numObjects > cobjMax)
		//ErrorLine("Too many objects in current form");
	if (vfCheckDupes && FIdFormObject(kind, fTrue))
		{
		for (iobj = 0; iobj < vpfrm->form.numObjects; iobj++)
			{
			RCFORMOBJLIST *plt;
			plt = PlexGetElementAt(&vpfrm->pllt, iobj);
			if (FIdFormObject(plt->objectType, fTrue) && 
				IdFromObj(pobj, kind) == IdFromObj(&plt->u.object, plt->objectType))
				{
				/* dupe labels are ok */
				if (!(kind == frmLabelObj && plt->objectType == frmLabelObj))
					WarningLine("Duplicate form object id (error is on the previous line)");
				break;
				}
			}
		}
	lt.objectType = kind;	
	lt.u.object = *pobj;
	PlexAddElement(&vpfrm->pllt, &lt);
	vpfrm->form.numObjects++;
	}


/*-----------------------------------------------------------------------------
|	FreeLt
|	
|		Frees stuff pointed to by lt (Doesn't free lt itself)
-------------------------------------------------------------WESC------------*/
VOID FreeLt(RCFORMOBJLIST *plt)
	{
	char *pchText;

	CbEmitStruct(plt->u.object.ptr, mpotszEmit[plt->objectType], &pchText, fFalse);
	if (pchText != NULL)
		free(pchText);
	if (plt->objectType == frmTableObj)
		free(plt->u.object.table->rgdxcol);
	free(plt->u.object.ptr);
	}

/*-----------------------------------------------------------------------------
|	FreeFrm
-------------------------------------------------------------WESC------------*/
void FreeFrm(FRM *pfrm)
	{
	int ilt;

	for (ilt = 0; ilt < PlexGetCount(&pfrm->pllt); ilt++)
		FreeLt(PlexGetElementAt(&pfrm->pllt, ilt));
	PlexFree(&pfrm->pllt);	
	}


void FreeRcpfile(RCPFILE *prcpf)
	{
	int ifrm;

	if (prcpf != NULL)
		{
		for (ifrm = 0; ifrm < PlexGetCount(&prcpf->plfrm); ifrm++)
			FreeFrm((FRM *)PlexGetElementAt(&prcpf->plfrm, ifrm));
		PlexFree(&prcpf->plfrm);
		}

	iidMenuMac   = 0;
	iidAlertMac  = 0;
	iidStringMac = 0;
	iidAISMac    = 0;
	idAutoMac    = idAutoInit;

	FreeFontMem();
	}

/*-----------------------------------------------------------------------------
|	FParseObjects
|	
|		Parse the objects (items) in a form
-------------------------------------------------------------WESC------------*/
BOOL FParseObjects()
	{
	RCFORMOBJECT obj;
	RW rwSav;
	ITM itm;
	int icol;
	FormObjectKind fok;
	
	/* add objects to object table until we see a rwEnd */
	while (FGetTok(&tok))
		{
		fok = (FormObjectKind) -1;
		switch(rwSav = tok.rw)
			{
			case rwEnd:
				return fTrue;
			case rwTTL: 	/* Yuck, should just do this in FORM line! */
			  	ParseItm(&itm, ifText, if2Null);
				obj.title = calloc(1, sizeof(RCFORMTITLE));
				obj.title->text = itm.text;
				fok = frmTitleObj;
				break;
			
		 	case rwBTN: /* */
		  		ParseItm(&itm, ifText|ifId|ifRc|ifUsable|ifEnabled|ifFont|ifAnchor|ifFrame|ifBigMargin, if2Null);
				goto Control;
			case rwPBN:	/* pushbutton */
		  		ParseItm(&itm, ifText|ifId|ifRc|ifUsable|ifEnabled|ifFont|ifAnchor|ifGroup|ifSmallMargin, if2Null);
				goto Control;
			case rwCBX:
		  		ParseItm(&itm, ifText|ifId|ifRc|ifUsable|ifEnabled|ifFont|ifAnchor|ifGroup|ifOn|ifBigMargin|ifSmallMargin, if2Null);	/* BUG! need to add checkbox extra! */
				itm.frame = 0;
				goto Control;
			case rwPUT: 
		  		ParseItm(&itm, ifText|ifId|ifRc|ifUsable|ifEnabled|ifFont|ifAnchor|ifBigMargin|ifSmallMargin, if2Null); /* SAME! */
				goto Control;
			case rwSLT:
		  		ParseItm(&itm, ifText|ifId|ifRc|ifUsable|ifEnabled|ifFont|ifAnchor|ifSmallMargin, if2Null);
				goto Control;
			case rwREP: /* repeating control */
		  		ParseItm(&itm, ifText|ifId|ifRc|ifUsable|ifEnabled|ifFrame|ifFont|ifAnchor|ifBigMargin, if2Null);
Control:
				obj.control = calloc(1, sizeof(RCCONTROL));
				obj.control->style = rwSav-rwBTN;
				obj.control->text = itm.text;
				obj.control->bounds = itm.rc;
				obj.control->id = itm.id;
				obj.control->attr.enabled = itm.enabled;
				obj.control->attr.usable = itm.usable;
				obj.control->attr.frame = itm.frame;
				obj.control->attr.leftAnchor = itm.leftAnchor;
				obj.control->group = itm.group; 
				obj.control->font = itm.font;
				obj.control->attr.frame = itm.frame;
				obj.control->attr.on = itm.on;
				fok = frmControlObj;
				break;
			case rwLBL: 
				ParseItm(&itm, ifText|ifId|ifPt|ifFont|ifUsable, if2Null);
				obj.label = calloc(1, sizeof(RCFORMLABEL));
				obj.label->text = itm.text;
				obj.label->id = itm.id;
				obj.label->pos = itm.pt;
				obj.label->attr.usable = itm.usable;
				obj.label->fontID = itm.font;
				fok = frmLabelObj;
				break;
						
			case rwFLD:
				ParseItm(&itm, ifId|ifRc|ifUsable|ifAlign|ifFont|ifEnabled|
					ifUnderlined|ifSingleLine|ifEditable|ifDynamicSize|ifMaxChars, if2AutoShift|if2Scrollbar|if2Numeric);
				obj.field = calloc(1, sizeof(RCFIELD));
				obj.field->id = itm.id;
				obj.field->rect = itm.rc;
				obj.field->attr.editable = itm.editable;
				obj.field->attr.usable = itm.usable;
				
				obj.field->fontID = itm.font;
				obj.field->attr.singleLine = itm.singleLine;
				obj.field->attr.underlined = itm.underlined;
				obj.field->attr.insPtVisible = 1;
				obj.field->attr.justification = itm.justification;
				obj.field->attr.dynamicSize = itm.dynamicSize;
				obj.field->attr.autoShift = itm.autoShift;
				obj.field->attr.hasScrollBar = itm.hasScrollBar;
				obj.field->attr.numeric = itm.numeric;
				obj.field->maxChars = itm.maxChars;
				fok = frmFieldObj;
				break;
			
			case rwPUL:
				ParseItm(&itm, ifId|ifListId, if2Null);
				obj.popup = calloc(1, sizeof(RCFORMPOPUP));
				obj.popup->controlID = itm.id;
				obj.popup->listID = itm.listid;
				fok = frmPopupObj;
				break;
			case rwLST: 
				ParseItm(&itm, ifMultText|ifId|ifRc|ifUsable|ifFont|ifEnabled|ifCvis|ifSmallMargin, if2Null);

				obj.list = calloc(1, sizeof(RCLIST));				
				obj.list->itemsText = itm.text;
				obj.list->cbListItems = itm.cbText;
				obj.list->numItems = itm.numItems;				
				obj.list->id = itm.id;
				obj.list->bounds = itm.rc;
				obj.list->attr.usable = itm.usable;
				obj.list->font = itm.font;
				obj.list->attr.enabled = itm.enabled;	   
				if (itm.cvis != -1)
					{
					obj.list->bounds.extent.y = (obj.list->font == largeFont ? 14 : 11)*itm.cvis;
					rcPrev.extent.y = obj.list->bounds.extent.y;
					}
				fok = frmListObj;
				break;
				
			case rwGSI:
 				ParseItm(&itm, ifPt, if2Null);
				obj.grfState = calloc(1, sizeof(RCFORMGRAFFITISTATE));
				obj.grfState->pos = itm.pt;
				fok = frmGraffitiStateObj;
				break;

			case rwGDT:
				ParseItm(&itm, ifId|ifRc|ifUsable, if2Null);
				obj.gadget = calloc(1, sizeof(RCFORMGADGET));
				obj.gadget->id = itm.id;
				obj.gadget->rect = itm.rc;
				obj.gadget->attr.usable = itm.usable;
				fok = frmGadgetObj;
				break;				

			case rwFBM: 
				ParseItm(&itm, ifPt|ifUsable|ifBitmap, if2Null);
				obj.bitmap = calloc(1, sizeof(RCFORMBITMAP));
				/*obj.bitmap->id = itm.id; */
				obj.bitmap->pos = itm.pt;
				obj.bitmap->attr.usable = itm.usable;
				obj.bitmap->rscID = itm.rscID;
				fok = frmBitmapObj;
				break;
			case rwTBL:
				ParseItm(&itm, ifId|ifRc|ifEditable, if2NumColumns|if2NumRows|if2ColumnWidths);
				obj.table = calloc(1, sizeof(RCTABLE));
				obj.table->id = itm.id;
				obj.table->bounds = itm.rc;
				obj.table->attr.editable = itm.editable;
				obj.table->numColumns = itm.numColumns;
				obj.table->numRows = itm.numRows;
				obj.table->rgdxcol = calloc(obj.table->numColumns, sizeof(int));
				for (icol = 0; icol < obj.table->numColumns; icol++)
					obj.table->rgdxcol[icol] = itm.rgdxcol[icol];
				fok = frmTableObj;
				break;
			case rwSCL:
				ParseItm(&itm, ifId|ifRc|ifUsable, if2Value|if2MinValue|if2MaxValue|if2PageSize);
				obj.scrollbar = calloc(1, sizeof(RCSCROLLBAR));
				obj.scrollbar->id = itm.id;
				obj.scrollbar->bounds = itm.rc;
				if (itm.rc.extent.x != 7)
					WarningLine("Scrollbar width not the recommended 7");
				obj.scrollbar->attr.usable = itm.usable;
				obj.scrollbar->value = itm.value;
				obj.scrollbar->minValue = itm.minValue;
				obj.scrollbar->maxValue = itm.maxValue;
				obj.scrollbar->pageSize = itm.pageSize;
				fok = frmScrollbarObj;
				break;
			default:
				 ErrorLine2("Unknown token:", tok.lex.szId);
				 break;
			}
		if (fok != (FormObjectKind) -1)
			AddObject(&obj, fok);			
		}
	return fFalse;	
	}	


void InitFrm(FRM *pfrm)
	{
	PlexInit(&pfrm->pllt, sizeof(RCFORMOBJLIST), 10, 10);
	}
/*-----------------------------------------------------------------------------
|	FParseForm
-------------------------------------------------------------WESC------------*/
BOOL FParseForm(RCPFILE *prcpf)
	{
	ITM itm;
	BOOL f;
	FRM frm;

	memset(&frm, 0, sizeof(FRM));
	InitFrm(&frm);
	vpfrm = &frm;
//	memset(&form, 0, sizeof(RCFORM));
//	memset(rglt, 0, cobjMax*sizeof(RCFORMOBJLIST));

	vpfrm->form.window.windowFlags.focusable = 1;
	ParseItm(&itm, ifId|ifRc|ifUsable|ifFrame|ifModal|ifSaveBehind|ifHelpId|ifDefaultBtnId|ifMenuId, if2Null);
	vpfrm->form.formId = itm.id;
	vpfrm->form.window.windowBounds = itm.rc;
	vpfrm->form.window.frameType.width = itm.frame;
	if (itm.modal)
		{
		vpfrm->form.window.windowFlags.modal = fTrue;
		vpfrm->form.window.windowFlags.dialog = fTrue;
		/* vpfrm->form.window.frameType.word = dialogFrame;		 Need to parse this!!! */
		vpfrm->form.window.frameType.cornerDiam = 3;
		vpfrm->form.window.frameType.width = 2;
		}
	vpfrm->form.attr.saveBehind = itm.saveBehind;
	/*vpfrm->form.attr.saveBehind = 1;*/
	vpfrm->form.attr.usable = itm.usable;
	vpfrm->form.attr.enabled = itm.enabled;
	vpfrm->form.helpRscId = itm.helpId;
	vpfrm->form.defaultButton = itm.defaultBtnId;
	vpfrm->form.menuRscId = itm.menuId;

	GetExpectRw(rwBegin);
	f = FParseObjects();
//	if (ifrmMac > ifrmMax)
//		Error("Too many forms!");
	if (vfCheckDupes)
		{
		int ifrm;

		for (ifrm = 0; ifrm < PlexGetCount(&prcpf->plfrm); ifrm++)
			{
			FRM *pfrm;
			pfrm = (FRM *)PlexGetElementAt(&prcpf->plfrm, ifrm);
			if (pfrm->form.formId == vpfrm->form.formId)
				{
				ErrorLine("Duplicate Form Resource IDs");
				break;
				}
			}
		}
//	frm.rglt = calloc(sizeof(RCFORMOBJLIST), vpfrm->form.numObjects);
	//memcpy(frm.rglt, rglt, sizeof(RCFORMOBJLIST) * vpfrm->form.numObjects);
	
	return PlexAddElement(&prcpf->plfrm, &frm);
//	rgfrm[ifrmMac].form = form;
//	rgfrm[ifrmMac].rglt = calloc(sizeof(RCFORMOBJLIST), vpfrm->form.numObjects);
//	memcpy(rgfrm[ifrmMac].rglt, rglt, sizeof(RCFORMOBJLIST) * vpfrm->form.numObjects);
	/* clone pointers?  yuck */
//	ifrmMac++;
	return f;
	}
										



/*-----------------------------------------------------------------------------
|	DumpMenu
|	
| This could be done more efficiently by emitting the strings differently, but I want to make the
| emitted bytes the same as the USR tools.
-------------------------------------------------------------WESC------------*/
VOID DumpMenu()
	{
	int cmpd;
	int impd;
	int imi;
	int ibCommands;
	int ibStrings;
	
	OpenOutput("MBAR", idMenu);
	cmpd = menu.numMenus;
	CbEmitStruct(&menu, szRCMENUBAR, NULL, fTrue);
	imi = 0;
	ibCommands = CbStruct(szRCMENUBAR)+cmpd*CbStruct(szRCMENUPULLDOWN);
	Assert(!(ibCommands & 1));
	ibStrings = ibCommands + imiMac*CbStruct(szRCMENUITEM);
	Assert(!(ibStrings & 1));
	for (impd = 0; impd < cmpd; impd++)
		{
		int imiT;
		RCMENUPULLDOWN mpd;
		
		mpd = rgmpd[impd];
		mpd.title = (char *) ibStrings;
		mpd.items = (RCMENUITEM *) ibCommands;
		CbEmitStruct(&mpd, szRCMENUPULLDOWN, NULL, fTrue);

		ibCommands += CbStruct(szRCMENUITEM)*rgmpd[impd].numItems;
		ibStrings += strlen(rgmpd[impd].title)+1;
		/* word align? */
		for (imiT = 0; imiT < rgmpd[impd].numItems; imiT++)
			{
			ibStrings += strlen(rgmi[imi].itemStr)+1;
			imi++;
			}
		}
	Assert(imi == imiMac);
	/* emit menuitems */
	ibCommands = CbStruct(szRCMENUBAR)+cmpd*CbStruct(szRCMENUPULLDOWN);
	Assert(!(ibCommands & 1));
	ibStrings = ibCommands + imiMac*CbStruct(szRCMENUITEM);
	Assert(!(ibStrings & 1));
	imi = 0;	
	for (impd = 0; impd < cmpd; impd++)
		{
		int imiT;
		
		ibStrings += strlen(rgmpd[impd].title)+1;
		/* word align? */
		for (imiT = 0; imiT < rgmpd[impd].numItems; imiT++)
			{
			RCMENUITEM mi;
			
			mi = rgmi[imi];
			mi.itemStr = (char *) ibStrings;
			CbEmitStruct(&mi, szRCMENUITEM, NULL, fTrue);
			ibStrings += strlen(rgmi[imi].itemStr)+1;
			imi++;
			}
		}
	/* now finally emit the dang strings		 */
	imi = 0;		
	for (impd = 0; impd < cmpd; impd++)
		{
		int imiT;

		DumpBytes(rgmpd[impd].title, strlen(rgmpd[impd].title)+1);
		/* word align? */
		for (imiT = 0; imiT < rgmpd[impd].numItems; imiT++)
			{
			DumpBytes(rgmi[imi].itemStr, strlen(rgmi[imi].itemStr)+1);
			imi++;
			}
		}
	CloseOutput();
	
	}

	
/*-----------------------------------------------------------------------------
|	AssignMenuRects
|	
|		Compute the menu screen rect
-------------------------------------------------------------WESC------------*/
VOID AssignMenuRects()
	{
	int cmpd;
	int impd;
	int imi;
	int xTitle;
	
	cmpd = menu.numMenus;
	imi = 0;
	xTitle = 4;
	for (impd = 0; impd < cmpd; impd++)
		{
		int imiT;
		int dxMac;
		int dx;
		RCMENUPULLDOWN *pmpd;
		
		pmpd = &rgmpd[impd];
		dx = DxCalcExtent((unsigned char *)pmpd->title, fTrue)+7;
		pmpd->titleBounds.topLeft.x = xTitle;
		pmpd->titleBounds.topLeft.y = 0;
		pmpd->titleBounds.extent.y = 12;
		xTitle += pmpd->titleBounds.extent.x = dx;

		pmpd->bounds.topLeft.y = 14;
		pmpd->bounds.extent.y = 0;
		dxMac = 0;
		for (imiT = 0; imiT < rgmpd[impd].numItems; imiT++)
			{
			int dxT;

			dxT = DxCalcExtent((unsigned char *)rgmi[imi].itemStr, fTrue)+6;
			if (rgmi[imi].command != 0)
				{
				char szT[2];
				dxT += 6 + 12;
				szT[0] = (char) rgmi[imi].command;
				szT[1] = 0;
				dxT += DxCalcExtent((unsigned char *)szT, fTrue);
				}
			dxMac = WMax(dxMac, dxT);
			if (strcmp(rgmi[imi].itemStr, "-") == 0)
				pmpd->bounds.extent.y += 5;
			else
				pmpd->bounds.extent.y += 11;
			imi++;
			}
   		pmpd->bounds.extent.x = dxMac;
		pmpd->bounds.topLeft.x = WMin(pmpd->titleBounds.topLeft.x + 2, dxScreen-dxMac-2);		
		}
	
	}	

/*-----------------------------------------------------------------------------
|	FParsePullDown
-------------------------------------------------------------WESC------------*/
BOOL FParsePullDown()
	{
	RCMENUPULLDOWN mpd;
	RCMENUITEM mi;
	
	memset(&mpd, 0, sizeof(RCMENUPULLDOWN));
	mpd.title = PchGetSz("Popup title");
	GetExpectRw(rwBegin);
	while (FGetTok(&tok))
		{
	 	switch (tok.rw)
			{
		default:
			ErrorLine("END or MENUITEM expected");
			break;
		case rwMenuItem:
			memset(&mi, 0, sizeof(RCMENUITEM));
			if (!FGetTok(&tok))
				return fFalse;
			if (tok.rw == rwSeparator)
				{
				mi.itemStr = strdup("-");
				/*mi.id = 999; */
				}
			else
				{
				int cch;

				UngetTok();
				mi.itemStr = PchGetSz("Item Text");
				cch = strlen(mi.itemStr);
				/* pilot has a special ... character */
				if (cch >= 3 && strcmp(&mi.itemStr[cch-3], "...") == 0)
					{
					mi.itemStr[cch-3] = (char) 0x85;
					mi.itemStr[cch-2] = 0;
					}
				mi.id = WGetId("CommandId", fFalse);			
				if (FGetTok(&tok))
					{
					if (tok.rw == rwNil && tok.lex.lt == ltStr)
						{
						if (strlen(tok.lex.szId) != 1)
							ErrorLine("CommandKey must be 1 character");
						mi.command = toupper(tok.lex.szId[0]);
						}				
					else
						UngetTok();
					}
				if (vfCheckDupes)
					{
					int imi;

					for (imi = 0; imi < imiMac; imi++)
						{
						if (strcmp(rgmi[imi].itemStr, "-") != 0)
							{
							if (mi.id == rgmi[imi].id)
								{
								WarningLine("Duplicate menu item id");
								break;
								}
							}
						}
					}
				}				

			if (imiMac > imiMax)
				ErrorLine("Too many menu items");
			rgmi[imiMac++] = mi;
			mpd.numItems++;				
			break;
		case rwEnd:
			if (menu.numMenus > impdMax)
				ErrorLine("Too many menus");
			rgmpd[menu.numMenus++] = mpd;
			return fTrue; 		
			}
		}
	return fTrue;
	}
	
	
/*-----------------------------------------------------------------------------
|	FParseMenu
-------------------------------------------------------------WESC------------*/
BOOL FParseMenu()
	{
	/* init menu globals */
	memset(&menu, 0, sizeof(RCMENUBAR));
	imiMac = 0;

	idMenu = WGetId("MenuId", fFalse);

	if (vfCheckDupes)
		{
		int iid;

		for (iid = 0; iid < iidMenuMac; iid++)
			{
			if (rgidMenu[iid] == idMenu)
				{
				ErrorLine("Duplicate Menu Resource ID");
				}
			}
		}

	rgidMenu[iidMenuMac++] = idMenu;

	menu.curItem = -1;	/* all resource I've seen have this set */
	GetExpectRw(rwBegin);
	while (FGetTok(&tok))
		{
		switch (tok.rw)
			{
		default:
			ErrorLine2("Unknown identifier:", tok.lex.szId);
			break;			
		case rwPullDown:
			FParsePullDown();
			break;
		case rwEnd:
			return fTrue;
			break;			
			}			
		}
	return fFalse;		
	}				

/*-----------------------------------------------------------------------------
|	FreeMenu
-------------------------------------------------------------WESC------------*/
void FreeMenu()
	{
	int impd;
	int imi;

	for (impd = 0; impd < menu.numMenus; impd++)
		{
		RCMENUPULLDOWN *pmpd;

		pmpd = &rgmpd[impd];
		free(pmpd->title);
		}
	for (imi = 0; imi < imiMac; imi++)
		{
		free(rgmi[imi].itemStr);
		}
	memset(&menu, 0, sizeof(RCMENUBAR));
	imiMac = 0;
	}

/*-----------------------------------------------------------------------------
|	ParseDumpAlert
-------------------------------------------------------------WESC------------*/
void ParseDumpAlert()
	{
	ITM itm;
	RCALERTTEMPLATE at;
	int idAlert;
	char *pchTitle;
	char *pchMessage;

	memset(&at, 0, sizeof(at));
	memset(&itm, 0, sizeof(itm));
	pchTitle = rgbZero;
	pchMessage = rgbZero;
	idAlert = WGetId("AlertId", fFalse);

	if (vfCheckDupes)
		{
		int iid;

		for (iid = 0; iid < iidAlertMac; iid++)
			{
			if (rgidAlert[iid] == idAlert)
				{
				ErrorLine("Duplicate Alert Resource ID");
				}
			}
		}
	if (iidAlertMac < iidAlertMax)
		rgidAlert[iidAlertMac++] = idAlert;

	
	while (FGetTok(&tok))
		{
		switch (tok.rw)
			{
		default:
			ErrorLine("Unexpected end of file");
			break;
		case rwInformation:
		case rwConfirmation:
		case rwWarning:
		case rwError:
			at.alertType = tok.rw-rwInformation;
			break;
		case rwHelpId:
			at.helpRscID = WGetConstEx("HelpId");
			break;
 		case rwDefaultBtn:
 			at.defaultButton = WGetConstEx("DefaultButton");
 			break;
		case rwBegin:
			while (FGetTok(&tok))
				{
				switch(tok.rw)
					{
				default:
					ErrorLine("END expected");
				case rwEnd:
					goto WriteAlert;
				case rwTTL:
					pchTitle = PchGetSz("Title Text");
					break;
				case rwMessage:
					pchMessage = PchGetSzMultiLine("Message Text");
					break;
				case rwBTN:
				case rwButtons:	/* button */
					ParseItm(&itm, ifMultText, if2Null);
					break;
					}
				}
			break;
			}
		}
WriteAlert:
	at.numButtons = itm.numItems;
 	if (at.numButtons > 0 && at.defaultButton >= at.numButtons)
 		ErrorLine("Invalid default button number");

	OpenOutput("Talt", idAlert);
	CbEmitStruct(&at, szRCALERTTEMPLATE, NULL, fTrue);
	DumpBytes(pchTitle, strlen(pchTitle)+1);
	DumpBytes(pchMessage, strlen(pchMessage)+1);
	if (itm.text != NULL)
		DumpBytes(itm.text, itm.cbText);
	CloseOutput();
	if (pchTitle != rgbZero)
		free(pchTitle);
	if (pchMessage != rgbZero)
		free(pchMessage);
	}	

/*-----------------------------------------------------------------------------
|	ParseDumpVersion
-------------------------------------------------------------WESC------------*/
void ParseDumpVersion()
	{
	int id;
	char *pchVersion;

	// default version ID is 1 (it dont work otherwise) :P
	id = 1;
	if (FGetTok(&tok)) {
		UngetTok();
		if (tok.rw == rwId) 
			id = WGetId("Version ResourceId", fFalse);
	}

	pchVersion = PchGetSz("Version Text");
	OpenOutput("tver", id);
	DumpBytes(pchVersion, strlen(pchVersion)+1);
	CloseOutput();
	free(pchVersion);
	}

/*-----------------------------------------------------------------------------
|	ParseDumpStringTable
-------------------------------------------------------------KAS-------------*/
void ParseDumpStringTable()
	{
	int id;
	int tot = 0;
	int numStrings = 0;
	char *buf;
	char *prefixString;

	buf = malloc(32768);
	id = WGetId("StringTable ResourceId", fFalse);
	if (vfCheckDupes)
		{
		int iid;
		for (iid = 0; iid < iidStringMac; iid++)
			if (rgidString[iid] == id)
				ErrorLine("Duplicate StringTable Resource ID");
		}
	if (iidStringMac < iidStringMax)
		rgidString[iidStringMac++] = id;

	GetExpectLt(&tok, ltStr, "String Text");
	prefixString = strdup(tok.lex.szId);

	GetExpectLt(&tok, ltStr, "String Text");
	strcpy(buf, tok.lex.szId);
	tot = strlen(tok.lex.szId)+1;
	numStrings++;

	while (FGetTok(&tok)) 
		{
		int l;
		if (tok.lex.lt != ltStr)
			{
			UngetTok();
			break;
			}

		l = strlen(tok.lex.szId) + 1;
		if (tot+l > 32768)
			ErrorLine("Sum of string lengths must be less than 32768");

		strcpy(buf+tot, tok.lex.szId);
		tot += l;
		numStrings++;
		if (numStrings >= 384)
			ErrorLine("Number of strings in table must be less than 384");
		}
	OpenOutput("tSTL", id);
	DumpBytes(prefixString, strlen(prefixString)+1);
	EmitW((unsigned short)numStrings);
	DumpBytes(buf, tot);
	CloseOutput();

	free(prefixString);
	free(buf);
	}

/*-----------------------------------------------------------------------------
|	ParseDumpString
-------------------------------------------------------------WESC------------*/
void ParseDumpString()
	{
	int id;
	char *pchString;

	id = WGetId("String ResourceId", fFalse);

	if (vfCheckDupes)
		{
		int iid;

		for (iid = 0; iid < iidStringMac; iid++)
			{
			if (rgidString[iid] == id)
				{
				ErrorLine("Duplicate String Resource ID");
				}
			}
		}
	if (iidStringMac < iidStringMax)
		rgidString[iidStringMac++] = id;

	if (!FGetTok(&tok))
		{
		ErrorLine("String Text or FILE Expected");
		}
		
	if (tok.rw == rwFile)
		{
		FILE *fh;
		int cch;
		
		pchString = malloc(16384);
		GetExpectLt(&tok, ltStr, "String filename");
		fh = fopen(tok.lex.szId, "rt");
		if (fh == NULL)
			ErrorLine2("Unable to open String file ", tok.lex.szId);
		cch = fread(pchString, 1, 16384, fh);				
		if (cch == 16384)
			ErrorLine("String too long!");
		pchString[cch] = 0;
		fclose(fh);		
		}
	else
		{
		UngetTok();
		pchString = PchGetSzMultiLine("String Text");
		}
	OpenOutput("tSTR", id);
	DumpBytes(pchString, strlen(pchString)+1);
	CloseOutput();
	free(pchString);
	}
/*-----------------------------------------------------------------------------
|       ParseDumpCategories
-------------------------------------------------------------JOHNL-----------*/
void ParseDumpCategories()
	{
	int id, len, count;
	char *string;

	id = WGetId("Categories ResourceId", fFalse);

	if (vfCheckDupes)
		{
		int iid;

		for (iid = 0; iid < iidAISMac; iid++)
			{
			if (rgidAIS[iid] == id)
				{
				ErrorLine("Duplicate Categories Resource ID");
				}
			}
		}
	if (iidAISMac < iidAISMax)
		rgidString[iidAISMac++] = id;

	OpenOutput("tAIS", id);

	count = 0;
	GetExpectLt(&tok, ltStr, "String Text");
	do
		{
		/* Only read up to maxCategories strings */
		if (count < maxCategories) 
			{
			string = tok.lex.szId;
			len = strlen(tok.lex.szId);
			/* Check the size of the string and only write 15 character max */
			if (len >= categoryLength)
				len = categoryLength - 1;
			DumpBytes(string, len);
			EmitB(0);
			}
		else
			{
			if (count == maxCategories)
				WarningLine("Too many strings in Categories. Extra strings will be ignored");
			}
		count++;

		if (!FGetTok(&tok))
			break;
		} 
	while(tok.lex.lt == ltStr);

	if (tok.lex.lt != ltNil)
	    UngetTok();

	/* The AppInfo category structure expects exactly maxCategories 
         * strings, so write a null byte for any unspecified strings */
	for (; count < maxCategories; count++) 
		EmitB(0);

	CloseOutput();

	}


/*-----------------------------------------------------------------------------
|	ParseDumpBitmapFile
-------------------------------------------------------------DAVE------------*/
void ParseDumpBitmapFile(int bitmapType)
	{
	int compress;
	BOOL colortable;
	int id;
	char *pchFileName[4];
	int  transparencyData[4];

	id = WGetId("Bitmap ResourceId", fFalse);
	pchFileName[0] = PchGetSz("Bitmap Filename");

	// family? need to get 1bpp, 2bpp, 4bpp and 8bpp!
        if (bitmapType == rwBitmapFamily) {
	  pchFileName[1] = PchGetSz("Bitmap Filename");
	  pchFileName[2] = PchGetSz("Bitmap Filename");
	  pchFileName[3] = PchGetSz("Bitmap Filename");
        }

	compress            = rwNoCompress;
	colortable          = fFalse;
	transparencyData[0] = 0;
	while (FGetTok(&tok)) 
		{
		if (tok.rw == rwNoCompress || tok.rw == rwAutoCompress || tok.rw == rwForceCompress) 
			{
			compress = tok.rw;
			}
		else 
		if (tok.rw == rwNoColorTable || tok.rw == rwColorTable) 
			{
			colortable = (tok.rw == rwColorTable);
			}
		else 
		if (tok.rw == rwTransparency) 
			{
			transparencyData[0] = rwTransparency;
			transparencyData[1] = WGetConst("red value");
			transparencyData[2] = WGetConst("green value");
			transparencyData[3] = WGetConst("blue value");
			}
		else 
		if (tok.rw == rwTransparencyIndex) 
			{
			transparencyData[0] = rwTransparencyIndex;
			transparencyData[1] = WGetConst("transparency index");
			}
		else 
			{
			UngetTok();
			break;
			}
		}

	OpenOutput("Tbmp", id);
        if (bitmapType == rwBitmapFamily) {

          int i, flag = 0x00;
        
          if (strcmp(pchFileName[0], "") != 0) flag |= 0x01;
          if (strcmp(pchFileName[1], "") != 0) flag |= 0x02;
          if (strcmp(pchFileName[2], "") != 0) flag |= 0x04;
          if (strcmp(pchFileName[3], "") != 0) flag |= 0x08;

          // only process the bitmaps that have been supplied!
          i=0;
          while (flag != 0x00) {

            if ((flag & 0x01) == 0x01)
              DumpBitmap(pchFileName[i], 0, compress, 
                         rwBitmap+i, colortable, transparencyData, ((flag & 0xfe) != 0x00));
            flag = flag >> 1;
            i++;
          }
        }
        else {
	  DumpBitmap(pchFileName[0], fFalse, compress, bitmapType, colortable, transparencyData, fFalse);
        }
	CloseOutput();

	free(pchFileName[0]);
        if (bitmapType == rwBitmapFamily) {
	  free(pchFileName[1]);
	  free(pchFileName[2]);
	  free(pchFileName[3]);
	}
	}

/*-----------------------------------------------------------------------------
|	ParseDumpIcon
-------------------------------------------------------------DAVE------------*/
void ParseDumpIcon(BOOL fSmall, int bitmapType)
	{
	char *pchFileName[4];
	int  transparencyData[4];
	BOOL colortable;

	pchFileName[0] = PchGetSz("Icon Filename");

	// family? need to get 1bpp, 2bpp, 4bpp and 8bpp!
        if (bitmapType == rwBitmapFamily) {
	  pchFileName[1] = PchGetSz("Icon Filename");
	  pchFileName[2] = PchGetSz("Icon Filename");
	  pchFileName[3] = PchGetSz("Icon Filename");
        }

        transparencyData[0] = 0;
	colortable          = fFalse;
	while (FGetTok(&tok)) 
		{
		if (tok.rw == rwNoColorTable || tok.rw == rwColorTable) 
			{
			colortable = (tok.rw == rwColorTable);
			}
		else 
		if (tok.rw == rwTransparency) 
			{
			transparencyData[0] = rwTransparency;
			transparencyData[1] = WGetConst("red value");
			transparencyData[2] = WGetConst("green value");
			transparencyData[3] = WGetConst("blue value");
			}
		else 
		if (tok.rw == rwTransparencyIndex) 
			{
			transparencyData[0] = rwTransparencyIndex;
			transparencyData[1] = WGetConst("transparency index");
			}
		else 
			{
			UngetTok();
			break;
			}
		}

	OpenOutput("tAIB", fSmall ? 1001 : 1000);
	if (bitmapType == rwBitmapFamily) {

          int i, flag = 0x00;

          if (strcmp(pchFileName[0], "") != 0) flag |= 0x01;
          else ErrorLine("ICONFAMILY/SMALLICONFAMILY must have a 1bpp bitmap");
          if (strcmp(pchFileName[1], "") != 0) flag |= 0x02;
          if (strcmp(pchFileName[2], "") != 0) flag |= 0x04;
          if (strcmp(pchFileName[3], "") != 0) flag |= 0x08;

          // only process the bitmaps that have been supplied!
          i=0;
          while (flag != 0x00) {

            if ((flag & 0x01) == 0x01)
              DumpBitmap(pchFileName[i], fSmall ? 2 : 1, rwNoCompress, 
                         rwBitmap+i, colortable, transparencyData, ((flag & 0xfe) != 0x00));
            flag = flag >> 1;
            i++;
          }

	  free(pchFileName[0]);
	  free(pchFileName[1]);
	  free(pchFileName[2]);
	  free(pchFileName[3]);
	}
	else {
 	  DumpBitmap(pchFileName[0], fSmall ? 2 : 1, rwNoCompress, bitmapType, fFalse, transparencyData, fFalse);
	  free(pchFileName[0]);
	}
	CloseOutput();
	}


/*-----------------------------------------------------------------------------
|	ParseDumpApplicationIconName
-------------------------------------------------------------WESC------------*/
void ParseDumpApplicationIconName()
	{
	int id;
	char *pchString;

	id = WGetId("Application IconName ResourceId", fFalse);
	pchString = PchGetSz("Icon Name Text");
	OpenOutput("tAIN", id);
	DumpBytes(pchString, strlen(pchString)+1);
	CloseOutput();
	free(pchString);
	}


/*-----------------------------------------------------------------------------
|	ParseDumpApplication
-------------------------------------------------------------WESC------------*/
void ParseDumpApplication()
	{
	int id;
	char *pchString;

	id = WGetId("APPL ResourceId", fFalse);
	pchString = PchGetSz("APPL");
	if (strlen(pchString) != 4)
		ErrorLine("APPL resource must be 4 chars");
	OpenOutput("APPL", id);
	DumpBytes(pchString, 4);
	CloseOutput();
	free(pchString);
	}

/*-----------------------------------------------------------------------------
|	ParseDumpTrap
-------------------------------------------------------------WESC------------*/
void ParseDumpTrap()
	{
	int id;
	int wTrap;

	id = WGetId("TRAP ResourceId", fFalse);
	wTrap = WGetConst("Trap number");
	if (id < 1000)
		ErrorLine("TRAP resource id must be >= 1000, see HackMaster documentation");
	OpenOutput("TRAP", id);
	EmitW((unsigned short)wTrap);
	CloseOutput();
	}


/*-----------------------------------------------------------------------------
|     ParseDumpFont
-------------------------------------------------------------DPT-------------*/
void ParseDumpFont()
	{
	int id;
	int fontid;
	char *pchFileName;
	
	id = WGetId("Font ResourceId", fFalse);
	GetExpectRw(rwFontId);
	fontid = WGetId("FontId", fFalse);
	if (fontid < 128 || fontid > 255)
		ErrorLine("FontID invalid.  valid values: 128<=FontID<=255");
	pchFileName = PchGetSz("Font Filename");
	
	OpenOutput("NFNT", id);
	DumpFont(pchFileName, fontid);
	CloseOutput();
	
	free(pchFileName);
	}

// 2.5b7 additions:
/*-----------------------------------------------------------------------------
|     ParseDumpHex
--------------------------------------------------------------AA-------------*/
void ParseDumpHex()
{
	char *pchResType;
	int id;

        // get the information from the .rcp entry
	pchResType = PchGetSz("Resource Type");
	id = WGetId("Data ResourceId", fFalse);

        // write the data to file
	OpenOutput(pchResType, id);
	while (FGetTok(&tok))
	{
		// we have a constant?
		if (tok.lex.lt == ltConst)
		{
			if (tok.lex.val > 0xff)
				ErrorLine("HEX data must be BYTE or less");

			EmitB((unsigned char)tok.lex.val);
		}
		
		// we have a string?
		else
		if (tok.lex.lt == ltStr)
		{
			char *pchString;

			UngetTok();
			pchString = PchGetSzMultiLine("String Text");
			DumpBytes(pchString, strlen(pchString));
			free(pchString);
		}

		// we dunno, assume "end" of resource
		else
		{
			UngetTok();
			break;
		}
	}
	CloseOutput();

	free(pchResType);
}

/*-----------------------------------------------------------------------------
|     ParseDumpData
--------------------------------------------------------------AA-------------*/
void ParseDumpData()
{
	char *pchResType;
	int id;
	char *pchFileName;

        // get the information from the .rcp entry
	pchResType = PchGetSz("Resource Type");
	id = WGetId("Data ResourceId", fFalse);
	pchFileName = PchGetSz("Data Filename");

        // write the data to file
	OpenOutput(pchResType, id);
	{
		int  cch;
		char *data;
		FILE *fh = fopen(pchFileName, "rb");

		data = malloc(4096);
		cch = fread(data, 1, 4096, fh);
		while (cch != 0) {
			DumpBytes(data, cch);
			cch = fread(data, 1, 4096, fh);
		}
		fclose(fh);
	}
	CloseOutput();

	free(pchResType);
	free(pchFileName);
}
// end

/*-----------------------------------------------------------------------------
|	ParseTranslation
-------------------------------------------------------------WESC------------*/
void ParseTranslation()
	{
	BOOL fAddTranslation;
	char *pch;
	TE *pte = NULL;

	GetExpectLt(&tok, ltStr, "Language");
	fAddTranslation = szLanguage != NULL && FSzEqI(tok.lex.szId, szLanguage);
	GetExpectRw(rwBegin);
	while (FGetTok(&tok))
		{
		switch (tok.rw)
			{
		case rwEnd:
			return;
		case rwNil:
			if (tok.lex.lt != ltStr)
				ErrorLine("String Expected");
			if (fAddTranslation)
				{
				pte = malloc(sizeof(TE));
				pte->szOrig = strdup(tok.lex.szId);
				}
			GetExpectLt(&tok, ltAssign, "=");
			pch = PchGetSzMultiLine("Translation");
			if (fAddTranslation)
				{
				pte->szTrans = pch;
				pte->pteNext = pteFirst;
				pteFirst = pte;
				}
			else
				free(pch);

		default:
			break;
			}
		}
	}

/*-----------------------------------------------------------------------------
|	OpenInputFile
-------------------------------------------------------------WESC------------*/
VOID OpenInputFile(char *szIn)
	{
	extern char szInFile[];

	strcpy( szInFile, FindAndOpenFile( szIn, "rt", &vfhIn ));
	iline = 0;
	}

/*-----------------------------------------------------------------------------
|	ParseCInclude
|	
|		Supports #define foo const and foo equ const
/      Added Support for #ifdef/#else/#endif  (nested #includes still needed).
-------------------------------------------------------------WESC------------*/
void ParseCInclude( char *szIncludeFile )
	{
	FILE *fhInSav;
	int ilineSav;
	char szInFileSav[256];
	char szId[256];
	int wIdVal;
	extern char szInFile[];
        /* Variables to support #ifdef/#else/#endif */
        /* needed at this scope to not interfer with the globals */
        int ifdefSkipping = 0;
        int ifdefLevel    = 0;

	/* tok contains filename */

	fhInSav = vfhIn;
	ilineSav = iline;
	strcpy(szInFileSav, szInFile);

	OpenInputFile(szIncludeFile);

	while (FGetTok(&tok))
		{
		switch (tok.lex.lt)
			{
		case ltPound:
                     {
			if (!FGetTok(&tok))
				ErrorLine("preprocessor directive expected");
			switch (tok.rw)
                        {
                        case rwDefine:
                            {
                                if (ifdefSkipping)
                                {
                		    NextLine();
                
                                }
                                else
                                {
			            GetExpectLt(&tok, ltId, "identifier");
			            strcpy(szId, tok.lex.szId);
 		                    wIdVal = WGetConstEx("Constant");
		                    AddSym(szId, wIdVal);
                                }
                                break;
                            }
                            
// 2.5b8 additions
                        case rwIfdef:
                            {
                                ifdefLevel++;
 
                                if( ifdefSkipping)
                                {
 
                                    ifdefSkipping++;
                                }
                                else
                                {
                                    SYM *psym;
 
                                    GetExpectLt(&tok, ltId, "identifier");
                                    strcpy(szId, tok.lex.szId);
 
                                    psym = PsymLookup(szId);
                                    if (psym == NULL)
                                    {
                                        ifdefSkipping = 1;
                                    }
                                }
                                break;
                            }
 
                        case rwIfndef:
                            {
                                ifdefLevel++;
 
                                if( ifdefSkipping)
                                {
 
                                    ifdefSkipping++;
                                }
                                else
                                {
                                    SYM *psym;
 
	                            GetExpectLt(&tok, ltId, "identifier");
	                            strcpy(szId, tok.lex.szId);
 
                                    psym = PsymLookup(szId);
                                    if (psym != NULL) 
                                    {
                                        ifdefSkipping = 1;
                                    }
                                }
                                break;
                            }
                            
                        case rwElse:
                            {
                                /* Verify Else is valid */
                                if (ifdefLevel)
                                {
                                    /* only do something if ifdefSkipping == 0 or 1 */
                                    if (ifdefSkipping == 1)
                                    {
                                         ifdefSkipping = 0;
                                    }
                                    else
                                    {
                                        if (ifdefSkipping == 0)
                                        {
                                             ifdefSkipping = 1;
                                        }
                                    }
                                }
                                else
                                {
                                  ErrorLine2("no matching #ifdef for :", tok.lex.szId);
                                }
                                break;
                            }
 
                        case rwEndif:
                            {
                                if (ifdefLevel)
                                {
                                    --ifdefLevel;
                                    if (ifdefSkipping)
                                    {
                                        --ifdefSkipping;
                                    }
                                }
                                else
                                {
                                  ErrorLine2("no matching #ifdef for :", tok.lex.szId);
                                }
 
                                break;
                            }
// end
        
                        default:
				{
				NextLine();
				break;
				}
                        }
                        break;
                    }
		case ltId:
                        if (ifdefSkipping)
                        {
                	    NextLine();
                
                        }
                        else
                        {
			    strcpy(szId, tok.lex.szId);
			    GetExpectRw(rwEqu);
			    wIdVal = WGetConstEx("Constant");
			    AddSym(szId, wIdVal);
                        }
			break;
		case ltSemi:
		case ltDoubleSlash:
			NextLine();
			break;			
		default:
			ErrorLine("Identifier expected");
			break;
			}
		}	
	fclose(vfhIn);
	strcpy(szInFile, szInFileSav);
	iline = ilineSav;
	vfhIn = fhInSav;	
	}


/*-----------------------------------------------------------------------------
|	ParseJavaInclude
|	
|		Supports a file of the EXACT form:
|
    package <pkgname>;
	
	public class <classname>
	{
		public static final short <idname> = <idnumber>;
	}
-------------------------------------------------------------DAVE------------*/
void ParseJavaInclude( char *szIncludeFile )
	{
	FILE *fhInSav;
	int ilineSav;
	char szInFileSav[256];
	char szId[256];
	int wIdVal;
	extern char szInFile[];

	/* tok contains filename */

	fhInSav = vfhIn;
	ilineSav = iline;
	strcpy(szInFileSav, szInFile);

	OpenInputFile(szIncludeFile);
	
	/* Skip to class xxx { */
	while (FGetTok(&tok) && 
		   (tok.lex.lt != ltLBrace))
		{
		}

	if ( tok.lex.lt != ltLBrace ) 
		{
		ErrorLine("expected class definition in java file");
		}

	while (FGetTok(&tok))
		{
		switch (tok.rw)
			{
			case rwPublic:
			case rwShort:			
			case rwInt:
			case rwStatic:		
			case rwFinal:
				continue;
			default:
				break;
			}
			
		switch (tok.lex.lt)
			{
		case ltId:
			strcpy(szId, tok.lex.szId);
			GetExpectLt(&tok, ltAssign, "[public] [static] [final] [short|int] Name = number;");

			wIdVal = WGetConstEx("Constant");
			AddSym(szId, wIdVal);
			break;
		case ltSemi:
		case ltDoubleSlash:
			NextLine();
			break;
		case ltRBrace:
			goto endOfClass;
		default:
			ErrorLine("Identifier definition expected");
			break;
			}
		}	

endOfClass:

	fclose(vfhIn);
	strcpy(szInFile, szInFileSav);
	iline = ilineSav;
	vfhIn = fhInSav;	
	}


void WriteIncFile(char *szFile)
	{
	FILE *fh;
	SYM *psym;
	
	fh = fopen(szFile, "wt");
	if (fh == NULL)
		Error3("Unable to open:", szFile, strerror(errno));
	if (!vfQuiet)
		printf("writing include file: %s\n", szFile);
	fprintf(fh, "/* pilrc generated file.  Do not edit!*/\n");
	for (psym = psymFirst; psym != NULL; psym = psym->psymNext)
		{
		if (psym->fAutoId)
			fprintf(fh, "#define %s %d\n", psym->sz, psym->wVal);
		}			
	fclose(fh);
	}

/*-----------------------------------------------------------------------------
|	ParseDirectives
-------------------------------------------------------------DAVE------------*/

void ParseDirectives()
	{
	TOK tok;
	char szId[256];
	
	FGetTok(&tok);
	switch (tok.rw) 
		{
	case rwInclude:
		{
		char *szFileName;
		char *pchExt;

		GetExpectLt(&tok, ltStr, "include filename");
		szFileName = tok.lex.szId;

		pchExt = strrchr(szFileName,'.');
		if (!pchExt) 
			{
			/* Assume it's a .h file */
			ParseCInclude(szFileName);
			break;
			}

		pchExt++;
		if (FSzEqI(pchExt, "java") || FSzEqI(pchExt, "jav"))
			{
			ParseJavaInclude(szFileName);
			}
		else 
			{
			ParseCInclude(szFileName);
			}
		break;
		}
        case rwDefine:
            {
            
		GetExpectLt(&tok, ltId, "identifier");
		strcpy(szId, tok.lex.szId);
                if (PsymLookup(szId) == NULL)
                break;
            }
        
// 2.5b8 additions
        case rwIfdef:
            {
                ifdefLevel++;
                
                if( ifdefSkipping)
                {
                    
                    ifdefSkipping++;
                }
                else
                {
                    SYM *psym;
                    
		    GetExpectLt(&tok, ltId, "identifier");
		    strcpy(szId, tok.lex.szId);
                    
                    psym = PsymLookup(szId);
                    if (psym == NULL)
                    {
                        ifdefSkipping = 1;
                    }
                }
                break;
            }
            
         case rwIfndef:
            {
                ifdefLevel++;
                
                if( ifdefSkipping)
                {
                    
                    ifdefSkipping++;
                }
                else
                {
                    SYM *psym;
                    
		    GetExpectLt(&tok, ltId, "identifier");
		    strcpy(szId, tok.lex.szId);
                    
                    psym = PsymLookup(szId);
                    if (psym != NULL)
                    {
                        ifdefSkipping = 1;
                    }
                }
                break;
            }
       
        case rwElse:
            {
                /* Verify Else is valid */
                if (ifdefLevel)
                {
                    /* only do something if ifdefSkipping == 0 or 1 */
                    if (ifdefSkipping == 1)
                    {
                         ifdefSkipping = 0;
                    }
                    else
                    {
                        if (ifdefSkipping == 0)
                        {
                             ifdefSkipping = 1;
                        }
                    }
                }
                else
                {
		    ErrorLine2("no matching #ifdef for :", tok.lex.szId);
                }
                break;
            }
        
        case rwEndif:
            {
                if (ifdefLevel)
                {
                    --ifdefLevel;
                    if (ifdefSkipping)
                    {
                        --ifdefSkipping;
                    } 
                }
                else
                {
		    ErrorLine2("no matching #ifdef for :", tok.lex.szId);
                }
            
                break;
            }
// end

	/* Add new directives here	*/

	default:
		ErrorLine2("directive not supported (try #include):", tok.lex.szId);
		break;
		}

	}

VOID InitRcpfile(RCPFILE *prcpfile, int fontType)
	{
	PlexInit(&prcpfile->plfrm, sizeof(FRM), 10, 10);
	InitFontMem(fontType);
	}

/*-----------------------------------------------------------------------------
|	ParseFile
-------------------------------------------------------------WESC------------*/
RCPFILE *ParseFile(char *szIn, char *szOutDir, char *szResFile, char *szIncFile, int fontType)
	{
	RCPFILE *prcpfile;
	
	prcpfile = calloc(1, sizeof(RCPFILE));
	InitRcpfile(prcpfile, fontType);
	
	
	SetOutFileDir(szOutDir);
	OpenInputFile(szIn);
	OpenResFile(szResFile);
	FInitLexer(NULL, fTrue);

	if (!FGetTok(&tok))
		Error("bogus source file");
	do
		{
                if (ifdefSkipping)
                {
		     if (tok.lex.lt == ltPound)
		     {
		          ParseDirectives();
		     }
		     else {
			  NextLine();
                     }
                     continue;
                }
		switch(tok.rw)
			{
		case rwForm:
			FParseForm(prcpfile);
			DumpForm(PlexGetElementAt(&prcpfile->plfrm, PlexGetCount(&prcpfile->plfrm)-1));
			break;
		case rwMenu:
			FParseMenu();
			AssignMenuRects();
			DumpMenu();
			FreeMenu();
			break;
		case rwAlert:
			ParseDumpAlert();
			break;
		case rwVersion:
			ParseDumpVersion();
			break;
		case rwStringTable:
			ParseDumpStringTable();
			break;
		case rwString:
			ParseDumpString();
			break;
		case rwCategories:
			ParseDumpCategories();
			break;
		case rwApplicationIconName:
			ParseDumpApplicationIconName();
			break;
		case rwApplication:
			ParseDumpApplication();
			break;
		case rwTranslation:
			ParseTranslation();
			break;
		case rwBitmap:
		case rwBitmapGrey:
		case rwBitmapGrey16:
		case rwBitmapColor:
		case rwBitmapFamily:
			ParseDumpBitmapFile(tok.rw);
			break;
		case rwIcon:
			ParseDumpIcon(fFalse, rwBitmap);
			break;
		case rwIconFamily:
			ParseDumpIcon(fFalse, rwBitmapFamily);
			break;
		case rwIconSmall:
			ParseDumpIcon(fTrue, rwBitmap);
			break;
		case rwIconSmallFamily:
			ParseDumpIcon(fTrue, rwBitmapFamily);
			break;
		case rwTrap:
			ParseDumpTrap();
			break;
		case rwFont:
			ParseDumpFont();
			break;
// 2.5b7 additions:
		case rwHex:
			ParseDumpHex();
			break;
		case rwData:
			ParseDumpData();
			break;
// end

		/* Add a rw here, remember to add to error message below */
		default:
			if (tok.lex.lt == ltPound)
				{
				ParseDirectives();
				}
			else if (tok.lex.lt == ltDoubleSlash)
				NextLine();
			else
				ErrorLine("FORM, MENU, ALERT, VERSION, STRINGTABLE, STRING, CATEGORIES, APPLICATIONICONNAME, APPLICATION, BITMAP, BITMAPGREY, BITMAPGREY16, BITMAPCOLOR, BITMAPFAMILY, ICON, ICONFAMILY, SMALLICON, SMALLICONFAMILY, TRAP, FONT, HEX, DATA or TRANSLATION expected");
			}
		}
	while (FGetTok(&tok));
	fclose(vfhIn);
	if (szIncFile != NULL)
		{
		WriteIncFile(szIncFile);
		}

	FreeSymTable();
	FreeTranslations();
	CloseResFile();
	return prcpfile;
	}


