/*-----------------------------------------------------------------------------
|	lex.c
|	
|	Pilrc lexer
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define NOPILOTINC
#include "pilrc.h"


int wBaseCur = 10;
              
char *pchLexBuf;
char *pchLexPrev;
char *pchLex;

char *_pchParseError;
BOOL _fReportErrors;

BOOL FInitLexer(char *pch, BOOL fMarkErrors)
	{
	pchLexBuf = pch;
	pchLex = pch;
	pchLexPrev = pch;
	_pchParseError = NULL;
	_fReportErrors = fMarkErrors;
	return fTrue;
	}

char *PchLexer()
	{
	return pchLex;
	}

char *PchLexerPrev()
	{
	return pchLexPrev;
	}

VOID ParseError(char *sz1, char *sz2)
	{
	_pchParseError = PchLexerPrev();
/*	if (_fReportErrors) */
	ErrorLine2(sz1, sz2);
	}

char *PchParseError()
	{
	return _pchParseError;
	}
	
	
	


BOOL FSkipWhite()	
	{
	if (pchLex == NULL)
		return fFalse;
	while (*pchLex == ' ' || *pchLex == '\t' || *pchLex == '\n' || *pchLex == '\r')
		pchLex++;
	return (*pchLex != '\000');
	}

BOOL FParseHex(LEX *plex, int ch)
	{
	LEX lex;

	ch = tolower(ch);
	if (ch == '0' && *(pchLex) == 'x' || *(pchLex) == 'X')
		{
		pchLex++;
		ch = *pchLex++;
		}
	if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f'))
		{
		lex.lt = ltConst;
		lex.val = 0;
		while ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f'))
			{
			lex.val *= 16;

			if ((ch >= '0' && ch <= '9'))
				lex.val += ch-'0';
			else
				lex.val += ch-'a'+10;
			ch = *pchLex++;
			ch = tolower(ch);
			}
		*plex = lex;
		return fTrue;
		}
	return fFalse;
	}

BOOL FParseConst(LEX *plex, int ch)
	{
	char *pchSav;
	extern int wBaseCur;
	char *pchStore;


	pchStore = plex->szId;
//	*pchStore++ = ch;
	pchSav = pchLex;
/*	if (wBaseCur == 16) */
		/*return FParseHex(plex, ch); */
#ifdef LATER
	if (wBaseCur == 2)
		return FParseBin(plex, ch);
#endif
	Assert(wBaseCur == 10);
	if ((ch >= '0' && ch <= '9') || ch == '.')
		{
		plex->lt = ltConst;
		plex->val = 0;
		if (ch == '0' && *(pchLex) == 'x' || *(pchLex) == 'X')
			{
			*pchStore++ = *pchLex++;
			*pchStore++ = ch;
			ch = *pchLex++;
			return FParseHex(plex, ch);
			}
		while (ch >= '0' && ch <= '9')
			{
			plex->val *= 10;
			plex->val += ch-'0';
			*pchStore++ = ch;
			ch = *pchLex++;
			}
		*pchStore = 0;
		return fTrue;
		}
	*pchStore = 0;
	return fFalse;
	}

int ChParseOctal(int ch)
	{
	int chVal;

	chVal = 0;
	while (ch >= '0' && ch <= '7')
		{
		chVal *= 8;
		chVal += ch-'0';
		ch = *pchLex++;
		}
	pchLex--;
	return chVal;
	}

BOOL FParseId(LEX *plex, int ch)
	{
	LEX lex;

	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
	   	{
	   	/* Identifier */
	   	int cch;

	   	lex.lt = ltId;
	   	cch = 0;
	   	do
	   		{
/*			if (ch != '"') */
				{
				lex.szId[cch] = ch;
				cch++;
				}
	   		ch = *pchLex++;
	   		if (cch == cchIdMax-1)
	   			{    
				ParseError("Identifier too long", NULL);
	   			break;
	   			}
	   		}
	   	while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_');
	   	lex.szId[cch] = '\000';
		*plex = lex;
		return fTrue;
	   	}				
	return fFalse;
	}

#define SLT(ch, ltArg) case ch: lex.lt = ltArg; break;

BOOL FGetLex(LEX *plex)
	{
	int ch;
	char szT[2];
	LEX lex;
	char *pchSav;
	char *pchStore;


	lex.lt = plex->lt = ltNil;
	pchLexPrev = pchLex;
	if (!FSkipWhite())
		return fFalse;
	pchSav = pchLex;
	pchStore = lex.szId;
	*pchStore++ = ch = *pchLex++;
	*pchStore = 0;
	switch (ch)
		{
	/* BUG! could use a lookup table... */
	/*TODO logical operators */
	SLT('+', ltPlus)
	SLT('-', ltMinus)
	SLT('*',	ltMult)
	SLT('%', ltMod)
	SLT('(', ltLParen)
	SLT(')', ltRParen)
	SLT('[', ltLBracket)
	SLT(']', ltRBracket)
	SLT('{', ltLBrace)
	SLT('}', ltRBrace)
	SLT(',', ltComma)
	SLT('?', ltQuestion)
	SLT(':', ltColon)
	SLT('^', ltCaret)
	SLT('\\', ltBSlash)
	SLT('#', ltPound)
	SLT(';', ltSemi)
	case '/':
		if (*pchLex == '/')
			{
			*pchStore++ = *pchLex++;
			*pchStore = 0;
			lex.lt = ltDoubleSlash;
			}
		else
			lex.lt = ltDiv;
		break;
	case '<':
		if (*pchLex == '=')
			{
			*pchStore++ = *pchLex++;
			*pchStore = 0;
			lex.lt = ltLTE;
			}
		else if (*pchLex == '>')
			{
			*pchStore++ = *pchLex++;
			*pchStore = 0;
			lex.lt = ltNE;
			}
		else
			lex.lt = ltLT;
		break;
	case '>':
		if (*pchLex++ == '=')
			{
			*pchStore++ = *pchLex++;
			*pchStore = 0;
			lex.lt = ltGTE;
			}
		else
			lex.lt = ltGT;
		break;
	case '=':
		if (*pchLex == '=')
			{
			*pchStore++ = *pchLex++;
			*pchStore = 0;
			lex.lt = ltEQ;
			}
		else
			lex.lt = ltAssign;
		break;
	case '"':
		lex.lt = ltStr;
		pchStore = lex.szId;
		while (*pchLex != '"')
			{
			if (*pchLex == '\\')
				{
				int ch;

				pchLex++;
				ch = *pchLex++;
				if (ch == 'n')
					ch = 0x0a;
				else if (ch == 't')
					ch = '\t';
				else if (ch >= '0' && ch <= '7')
					ch = ChParseOctal(ch);
				*pchStore++ = ch;
				}
			else
				*pchStore++ = *pchLex++;
	   		if (pchStore-lex.szId == cchIdMax-1)
	   			{    
				ParseError("String too long", NULL);
	   			break;
	   			}
			if (*pchLex == 0)
				{
				ParseError("Unterminated string", NULL);
				break;
				}
			}
		pchLex++;
		*pchStore = 0;
		break;
	default:
		if (FParseConst(&lex, ch) || FParseId(&lex, ch))
			{
			/* do nuthin...code is easier to read this way */
			}
		else
			{
			szT[0] = ch;
			szT[1] = '\000';
			ParseError("Unknown character: ", szT);
			}
		pchLex--;
		break;
		}
	
	*plex = lex;
	return lex.lt != ltNil;
	}

#define SPLT(lt, sz) case lt: printf(sz); break;

VOID PrintLex(LEX *plex)
	{
#ifdef FOO
	switch(plex->lt)
		{
	case ltConst:
 		printf("%d ", plex->val);
		break;
	case ltId:
		printf("%s ", plex->szId);
		break;
	SPLT(ltPlus, "+");
	SPLT(ltMinus, "-");
	SPLT(ltMult, "*");
	SPLT(ltDiv, "/");
	SPLT(ltMod, "%");
	SPLT(ltLParen, "(");
	SPLT(ltRParen, ")");
	SPLT(ltLBracket, "[");
	SPLT(ltRBracket, "]");
	SPLT(ltLBrace, "{");
	SPLT(ltRBrace, "}");
	SPLT(ltComma, ",");
	SPLT(ltLT, "<");
	SPLT(ltGT, ">");
	SPLT(ltLTE, "<=");
	SPLT(ltGTE, ">=");
	SPLT(ltNE, "<>");
	SPLT(ltEQ, "==");
	SPLT(ltAssign, "=");
	SPLT(ltQuestion, "?");
	SPLT(ltColon, ":");
	SPLT(ltCaret, "^");
		}
#endif
	}

