#ifndef __REXY_EMUL_TEST_WINDOW_H__
#define __REXY_EMUL_TEST_WINDOW_H__

// Windows Header Files:
#include "rexy.h"
#include <windows.h>

bool InitTestWindow(HWND hParent, HINSTANCE hInstance);
void test_send_msg(const char* pstr);
HWND GetTestWindow(void);


#endif
