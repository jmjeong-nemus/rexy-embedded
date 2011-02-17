/**
 * Application Message Procedure List
 */
#include "rexy.h"
#include "app_bmpdata.h"

/*-- State ID --*/

// Add here if you want to add your own application //////////////////////////
typedef enum {

	STATE_ID_ENTRY=0,
	STATE_ID_MGL_DEMO_IDLE,
	STATE_ID_MGL_SAMPLE1,
	STATE_ID_MGL_SAMPLE2,
	STATE_ID_MGL_SAMPLE3,
	STATE_ID_MGL_SAMPLE4,
	STATE_ID_MGL_SAMPLE5,
	STATE_ID_BIORHYTHM,
	STATE_ID_BROWSER,
	STATE_ID_TETRIS,
	STATE_ID_WINDEMO,
	STATE_ID_IMGVIEWER,
	STATE_ID_MAINMENU,
	STATE_ID_MAINMENU_ANIBMP,

	STATE_ID_CTL_TEST,
	STATE_ID_CTL_POPUP_TEST,
	STATE_ID_CTL_LIST_TEST,
	STATE_ID_CTL_EDIT_TEST,
	STATE_ID_CTL_SLIDETEXT_TEST,
	STATE_ID_CTL_ANIBMP_TEST,

	STATE_ID_DB_TEST,

	STATE_ID_END

} AppStateIDType;

typedef enum
{
	FntIDAlpha = 0,
	FntIDNumber10,
	FntIDNumber16,
	FntIDUnicode10Han,
	FntIDUnicode10BL,
	FntIDUnicode10SL,
	FntIDUnicode12Han,
	FntIDUnicode12BL,
	FntIDUnicode12SL,
	FntIDUnicode14Han,
	FntIDUnicode14BL,
	FntIDUnicode14SL,
	FntIDUnicode16Han,
	FntIDUnicode16BL,
	FntIDUnicode16SL,
	FntIDEND

} AppFontIDType;

// Added by Joshua on 7-Jun-2006
extern MsgType MSG_NULL;
extern MsgType MSG_CREATE;
extern MsgType MSG_DESTROY;
extern MsgType MSG_LOAD;
extern MsgType MSG_SAVE;
extern MsgType MSG_RETURN;
