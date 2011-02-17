#ifndef __TEXTVIEWER_H__
#define __TEXTVIEWER_H__

#include "rexy.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	TEXT_UNICODE_LITTLE	= 0,
	TEXT_UNICODE_BIG,
	TEXT_ANSI_CODE,
	TEXT_UTF8,
} TxvCode;

typedef enum 
{
	TEXTVIEW_STATE_FILE_LIST = 1,
	TEXTVIEW_STATE_FILE_SHOW,
	TEXTVIEW_STATE_SETTING,

} TxvStateList;

typedef struct
{
	TxvStateList eStateId;
	int (*pfnFormHandler)(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
} TxvFormHandle;

int TxvTextFileListProc(WND wnd, int msg, int wparam, int lparam);
int TxvTextViewProc(WND wnd, int msg, int wparam, int lparam);


#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __TEXTVIEWER_H__
