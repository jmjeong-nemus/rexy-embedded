#ifndef __REXY_EMUL_DEBUG_WINDOW_H__
#define __REXY_EMUL_DEBUG_WINDOW_H__

// Windows Header Files:
#include "rexy.h"
#include <windows.h>

bool InitDebugWindow(HWND hParent, HINSTANCE hInstance);
void mon_send_msg(const char* pstr);
HWND GetDebugWindow(void);


#endif
