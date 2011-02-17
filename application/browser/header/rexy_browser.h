#ifndef __REXY_BROWSER_H__
#define __REXY_BROWSER_H__

#include "rexy.h"

enum 
{
	LAUNCH_FILELIST = 0,
	LAUNCH_DIRECT
};


#ifdef __cplusplus
extern "C" {
#endif

    int Rexy_Browser(WND wnd, int msg, int wparam, int lparam);

#ifdef __cplusplus
}

#endif	// __cplusplus
#endif	// __REXY_BROWSER_H__

