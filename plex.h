

typedef struct _plex
	{
	int m_iMac;
	int m_iMax;
	int m_dGrow;
	int m_cbItem;
	void *m_rgb;
	} PLEX;
	
	
BOOL PlexInit(PLEX *pplex, int cbItem, int iMaxInit, int dGrow);
VOID PlexFree(PLEX *pplex);
int PlexGetCount(PLEX *pplex);
int PlexGetMax(PLEX *pplex);
void *PlexGetElementAt(PLEX *pplex, int i);
BOOL PlexAddElement(PLEX *pplex, void *pb);
BOOL PlexIsValid(PLEX *pplex);


#define DEFPL(NAME) typedef PLEX NAME;
