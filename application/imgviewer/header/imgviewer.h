#ifndef __IMGVIEWER_H__
#define __IMGVIEWER_H__

#include "rexy.h"


#ifdef __cplusplus
extern "C" {
#endif

	typedef enum 
{
	IMAGEVIEW_STATE_FILE_LIST = 1,
	IMAGEVIEW_STATE_FILE_SHOW,
	IMAGEVIEW_STATE_SETTING,

} ImgStateList;

typedef struct
{
	ImgStateList eStateId;
	void (*pfnFormHandler)(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
} ImgFormHandle;


int ImgViewerProc(WND hWnd, int nMsg, int wParam, int lParam);

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __IMGVIEWER_H__
