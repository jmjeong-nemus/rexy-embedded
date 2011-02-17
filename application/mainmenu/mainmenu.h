#include "rexy.h"

typedef struct _MNGlobalType
{
	int nMenuIndex;
	char* cName;
	BmpID szNormal;
	BmpID szActive1;
	BmpID szActive2;
	BmpID szActive3;
} MNGlobalType ;

rint32 MainMenuProc(WND wnd, int msg, int wparam, int lparam);