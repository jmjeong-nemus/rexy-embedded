#ifndef __TETLESS_H__
#define __TETLESS_H__

#include "rexy.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
	TETRIS_START = 1,
	TETRIS_MILESTONE_NEXT_GAME,
	TETRIS_FAIL_MESSAGE,
	TETRIS_GAMING
} TetrisStateID;

typedef struct
{
	TetrisStateID eStateId;
	void (*pfnStateHandler)(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
} st_TetrisStateHandle;

int TetlessProc(WND wnd, int msg, int wparam, int lparam);

#define TETRIS_TIMER	1000

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __TETLESS_H__

