							
#ifndef __lex__
#define __lex__


typedef int LT;

#define ltNil     -1
/* must be same as nt's */
#define ltConst    0
#define ltId       1
#define ltPlus     2
#define ltMinus    3
#define ltMult     4
#define ltDiv      5
#define ltMod      6

#define ltLT       7
#define ltGT       8
#define ltEQ       9
#define ltLTE      10
#define ltGTE      11
#define ltNE       12
#define ltCaret	   13
#define ltAssign   14
#define ltBSlash   15
#define ltPound    16
#define ltDoubleSlash 17
#define ltSemi     18
#define ltAt       19
#define ltPipe     20

#define ltLParen   42
#define ltRParen   43
#define ltLBracket 44
#define ltRBracket 45
#define ltLBrace   46
#define ltRBrace   47
#define ltComma    48
#define ltQuestion 49
#define ltColon    50
#define ltStr      51
#define ltCComment 52
#define ltEndCComment 53


typedef int VAL;

#define cchIdMax 4096
/* LEXeme */
typedef struct _lex
	{
	LT lt;
	char szId[cchIdMax];
	VAL val;
	} LEX;



/* Lex function prototypes */
BOOL FInitLexer(char *pch, BOOL fReportErrors);
char *PchLexer(void);
char *PchLexerPrev(void);
BOOL FGetLex(LEX *plex, BOOL fInComment);
VOID PrintLex(LEX *plex);
char *PchParseError();
VOID ParseError(char *, char *);


#endif /* __lex__ */

