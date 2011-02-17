/*
 * $Id: rexy_sysmsg.c 221 2005-01-21 01:27:30Z jmjeong $
 * Copyright(c) 2000-5, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy 
 *
 */
// Windows Header Files:

#pragma warning ( disable : 4115 )          // warning C4115: '_RPC_ASYNC_STATE' : named type definition in parentheses

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "resource.h"
#include "rexy_integrated.h"

#include "rexy_type.h"

extern HWND	g_hSYSWnd ;
extern HINSTANCE hInst;
// static char *GetComment(int kind, int init, int idx);
static char g_NoComment[20] = "No comment";


LRESULT CALLBACK	SysWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND CreateSysWindow(HWND hWnd)
{
    WNDCLASSEX wcex;
    HWND hSysWnd;
    RECT rt;
    HMENU hMenu;

    hMenu = GetMenu(hWnd);
    CheckMenuItem(hMenu, IDM_SYSMSG, MF_BYCOMMAND | MF_CHECKED);
    EnableMenuItem(hMenu, IDM_SYSMSG, MF_BYCOMMAND | MF_GRAYED);

    // Register Class
    wcex.cbSize			= sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)SysWndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInst;
    wcex.hIcon			= LoadIcon(hInst, (LPCTSTR)IDI_REXY_INTEGRATED);
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)COLOR_WINDOW;
    //	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);

    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= "Syswindow";
    wcex.hIconSm		= NULL;
    RegisterClassEx(&wcex);

    // Create New Window.
    GetWindowRect(hWnd, &rt);

    hSysWnd = CreateWindowEx(WS_EX_TOOLWINDOW, "Syswindow", "sys window",  WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION,
                             rt.right, rt.top, 280, 612, hWnd, NULL, hInst, NULL);

    ShowWindow(hSysWnd, SW_SHOWNOACTIVATE);
    UpdateWindow(hSysWnd);

    return hSysWnd;
}

#define ID_BTN_SEND			0
#define ID_RBTN_FOLDER_OPEN		1
#define ID_RBTN_FOLDER_CLOSE		2
#define ID_BTN_EXTL_SHORT			3
#define ID_BTN_EXTS_SHORT			4
#define ID_BTN_EXTL_LONG			5
#define ID_BTN_EXTS_LONG			6

extern HWND g_hMainWnd;
#include "common.h"

LRESULT CALLBACK	SysWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HMENU hMenu;
    static HWND hTmpWnd, hFolderOpen, hFolderClose, hExtBtnLS, hExtBtnSS, hExtBtnLL, hExtBtnSL;

    switch (message)
    {
    case WM_CREATE:
        // folder status
        hTmpWnd = CreateWindow("button"," EVENTS && HS EVENTS ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                               5, 5, 260, 115, hWnd, (HMENU)0, hInst, NULL);
        hFolderOpen=CreateWindow("button","FOLDER OPEN",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                                 10,40,130,30,hWnd,(HMENU)ID_RBTN_FOLDER_OPEN,hInst,NULL);
        hFolderClose=CreateWindow("button","FOLDER CLOSE",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                                  10,70,130,30,hWnd,(HMENU)ID_RBTN_FOLDER_CLOSE,hInst,NULL);
        SendMessage(hFolderOpen, BM_SETCHECK, fOpen, 0 );
        SendMessage(hFolderClose, BM_SETCHECK, !fOpen, 0 );
        // external buttons
        hExtBtnLS = CreateWindow("button","External L(1):Short", WS_CHILD | WS_VISIBLE,
                                 5, 110, 130, 30, hWnd, (HMENU)ID_BTN_EXTL_SHORT, hInst, NULL);
        hExtBtnLL = CreateWindow("button","External L(1):Long", WS_CHILD | WS_VISIBLE,
                                 135, 110, 130, 30, hWnd, (HMENU)ID_BTN_EXTL_LONG, hInst, NULL);
        hExtBtnSS = CreateWindow("button","External S(2):Short", WS_CHILD | WS_VISIBLE,
                                 5, 140, 130, 30, hWnd, (HMENU)ID_BTN_EXTS_SHORT, hInst, NULL);
        hExtBtnSL = CreateWindow("button","External S(2):Long", WS_CHILD | WS_VISIBLE,
                                 135, 140, 130, 30, hWnd, (HMENU)ID_BTN_EXTS_LONG, hInst, NULL);
        break;

    case WM_DESTROY:
        hMenu = GetMenu(g_hMainWnd);
        CheckMenuItem(hMenu, IDM_SYSMSG, MF_BYCOMMAND | MF_UNCHECKED);
        EnableMenuItem(hMenu, IDM_SYSMSG, MF_BYCOMMAND | MF_ENABLED);
        g_hSYSWnd = NULL;
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            switch (LOWORD(wParam))
            {
            case ID_RBTN_FOLDER_OPEN:
                AS_SetFolderStatus( TRUE );
                break;

            case ID_RBTN_FOLDER_CLOSE:
                AS_SetFolderStatus( FALSE );
                break;

            case ID_BTN_EXTL_SHORT:
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_L, AWM_KEYDOWN);
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_L, AWM_RAW_SKEY_UP);
                break;
            case ID_BTN_EXTL_LONG:
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_L, AWM_KEYDOWN);
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_L, AWM_KEYLONG);
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_L, AWM_RAW_LKEY_UP);
                break;
            case ID_BTN_EXTS_SHORT:
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_S, AWM_KEYDOWN);
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_S, AWM_RAW_SKEY_UP);
                break;
            case ID_BTN_EXTS_LONG:
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_S, AWM_KEYDOWN);
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_S, AWM_KEYLONG);
                PostMessage(hWnd, AWM_KEYDEVICE, VK_USER_EXT_S, AWM_RAW_LKEY_UP);
                break;
            }
        }
        break;

    default :
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
/*
static char *GetComment(int kind, int init, int idx)
{
	switch(kind)
	{
	default:
		break;
	}

	return g_NoComment;
}
*/
