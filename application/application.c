#include "application.h"

// If you want to add your own application, add here (this file and header file), 
// Sample code by Joshua on 15-May-2006

extern int IdleEntryProc(WND wnd, int msg, int wparam, int lparam);
extern int MGLDemoIdleProc(WND wnd, int msg, int wparam, int lparam);
extern int MGLSample1Proc(WND wnd, int msg, int wparam, int lparam);
extern int MGLSample2Proc(WND wnd, int msg, int wparam, int lparam);
extern int MGLSample3Proc(WND wnd, int msg, int wparam, int lparam);
extern int MGLSample4Proc(WND wnd, int msg, int wparam, int lparam);
extern int MGLSample5Proc(WND wnd, int msg, int wparam, int lparam);
#ifdef WIN32
extern int ImgViewerProc(WND wnd, int msg, int wparam, int lparam);
extern int BrowserProc(WND wnd, int msg, int wparam, int lparam);
#endif

extern int TetrisProc(WND wnd, int msg, int wparam, int lparam);
extern int BiorhythmProc(WND hWnd, int msg, int wParam, int lParam);

extern int WinDemoProc(WND wnd, int msg, int wparam, int lparam);

extern int SampleControlProc(WND wnd, int msg, int wparam, int lparam);
extern int SamplePopupProc(WND wnd, int msg, int wparam, int lparam);
extern int SampleListProc(WND wnd, int msg, int wparam, int lparam);
extern int SampleSlideTextProc(WND wnd, int msg, int wparam, int lparam);
extern int SampleAniBmpGateProc(WND wnd, int msg, int wparam, int lparam);
extern int SampleEditProc(WND wnd, int msg, int wparam, int lparam);
extern int SampleDBTestProc(WND wnd, int msg, int wparam, int lparam);

extern int MainMenuProc(WND wnd, int msg, int wparam, int lparam);
extern int MainMenuAniBmpProc(WND wnd, int msg, int wparam, int lparam);


extern const FntType FntDataAlpha16;
extern const FntType FntDataNumber10;
extern const FntType FntDataNumber16;
extern const FntType FntDataUnicode10Han;
extern const FntType FntDataUnicode10BL;
extern const FntType FntDataUnicode10SL;
extern const FntType FntDataUnicode12Han;
extern const FntType FntDataUnicode12BL;
extern const FntType FntDataUnicode12SL;
extern const FntType FntDataUnicode14Han;
extern const FntType FntDataUnicode14BL;
extern const FntType FntDataUnicode14SL;
extern const FntType FntDataUnicode16Han;
extern const FntType FntDataUnicode16BL;
extern const FntType FntDataUnicode16SL;

MsgType MSG_NULL = { AWM_NONE, 0, 0 };
MsgType MSG_CREATE = { AWM_CREATE, 0, 0 };
MsgType MSG_DESTROY = { AWM_DESTROY, 0, 0 };
MsgType MSG_LOAD = { AWM_LOAD, 0, 0 };
MsgType MSG_SAVE = { AWM_SAVE, 0, 0 };
MsgType MSG_RETURN = { AWM_RETURN, 0, 0 };

static void AppRegister(void);
static void FontRegister(void);

static void AppRegister(void)
{
  	//StaSetEventHandler( STATE_ID_ENTRY,		        MGLDemoIdleProc );
	StaSetEventHandler( STATE_ID_ENTRY,		        IdleEntryProc );
	StaSetEventHandler( STATE_ID_MGL_DEMO_IDLE,		MGLDemoIdleProc );
	StaSetEventHandler( STATE_ID_MGL_SAMPLE1,		MGLSample1Proc );
	StaSetEventHandler( STATE_ID_MGL_SAMPLE2,		MGLSample2Proc );
	StaSetEventHandler( STATE_ID_MGL_SAMPLE3,		MGLSample3Proc );
	StaSetEventHandler( STATE_ID_MGL_SAMPLE4,		MGLSample4Proc );
	StaSetEventHandler( STATE_ID_MGL_SAMPLE5,		MGLSample5Proc );

	//StaSetEventHandler( STATE_ID_BIORHYTHM,			BiorhythmProc );
	StaSetEventHandler( STATE_ID_TETRIS,			TetrisProc );

	//StaSetEventHandler( STATE_ID_WINDEMO,			WinDemoProc );
	//StaSetEventHandler( STATE_ID_IMGVIEWER,			ImgViewerProc );

	//StaSetEventHandler( STATE_ID_CTL_TEST,			SampleControlProc );
	//StaSetEventHandler( STATE_ID_CTL_POPUP_TEST,	SamplePopupProc );
	//StaSetEventHandler( STATE_ID_CTL_LIST_TEST,		SampleListProc );
	//StaSetEventHandler( STATE_ID_CTL_EDIT_TEST,		SampleEditProc );
	//StaSetEventHandler( STATE_ID_CTL_SLIDETEXT_TEST,SampleSlideTextProc );
	//StaSetEventHandler( STATE_ID_CTL_ANIBMP_TEST,	SampleAniBmpGateProc );

	//StaSetEventHandler( STATE_ID_MAINMENU, MainMenuProc );
	//StaSetEventHandler( STATE_ID_MAINMENU_ANIBMP, MainMenuAniBmpProc );

	//StaSetEventHandler( STATE_ID_BROWSER,	BrowserProc );
	//StaSetEventHandler( STATE_ID_DB_TEST,	SampleDBTestProc );
}

static void FontRegister(void)
{
	RscSetFont( FntIDAlpha,			(FntType*)&FntDataAlpha16  );
	RscSetFont( FntIDNumber10,		(FntType*)&FntDataNumber10 );
	RscSetFont( FntIDNumber16,		(FntType*)&FntDataNumber16 );
	RscSetFont( FntIDUnicode10Han,	(FntType*)&FntDataUnicode10Han );
	RscSetFont( FntIDUnicode10BL,	(FntType*)&FntDataUnicode10BL );
	RscSetFont( FntIDUnicode10SL,  	(FntType*)&FntDataUnicode10SL );
	RscSetFont( FntIDUnicode12Han, 	(FntType*)&FntDataUnicode12Han );
	RscSetFont( FntIDUnicode12BL,  	(FntType*)&FntDataUnicode12BL );
	RscSetFont( FntIDUnicode12SL,  	(FntType*)&FntDataUnicode12SL );
	RscSetFont( FntIDUnicode14Han, 	(FntType*)&FntDataUnicode14Han );
	RscSetFont( FntIDUnicode14BL,  	(FntType*)&FntDataUnicode14BL );
	RscSetFont( FntIDUnicode14SL,  	(FntType*)&FntDataUnicode14SL );
	RscSetFont( FntIDUnicode16Han, 	(FntType*)&FntDataUnicode16Han );
	RscSetFont( FntIDUnicode16BL,  	(FntType*)&FntDataUnicode16BL );
	RscSetFont( FntIDUnicode16SL,  	(FntType*)&FntDataUnicode16SL );
}

// Rexy Embedded will call this function to initialize application
// at startup
void AppInit(void)
{
	AppRegister();
	FontRegister();
	RscSetBitmapGroup((BmpType*)AppBmpData);
}
