
typedef struct _fobj
	{
	Word idObj;
	DWord rid;
	} RFOBJ;

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
	} RFORM;


typedef struct _rbtn
	{
	Word btnId;
	RectangleType rc;
	Word rfUsable;
	Word rfLeftAnchor;
	Word rfFrame;
	Word rfNonBoldFrame;
	Word rfFont;
	} RBTN;
/*#define CbBtn(pbtn) (sizeof(RBTN)-256+strlen(pbtn->szLabel)+1) */


#define rfTrue 0x100
#define rfFalse 0

#define RfBool(f) ((f) ? rfTrue : rfFalse)
#defien RfByte(b) ((b) << 8)
