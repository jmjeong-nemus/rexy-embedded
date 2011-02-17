/*
 * $Id: rexy_integrated.c 221 2005-01-21 01:27:30Z jmjeong $
 * Copyright(c) 2000-5, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy 
 *
 */
// Windows Header Files:

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <aygshell.h>

#include "resource.h"

#include "rexy.h"


//////////////////////////////////////////////////////////////////////////
// global variables

HINSTANCE	g_hInstance = NULL;
HINSTANCE	hInst = NULL;			// current instance
HACCEL		g_hAccelTable;
HWND		g_hMainWnd = NULL;

LCD_INFO	g_lcd;

bool		g_bInverted = FALSE;
int			g_nImgClicked = 0;
int			g_bpp = 16;		// for BPP_16
int			nPrevKey = -1;
TCHAR		szWindowClass[] = L"RexyWndClass";
TCHAR		szTitle[] = L"Rexy Simulator";

//////////////////////////////////////////////////////////////////////////
// local methods
static ATOM MyRegisterClass(HINSTANCE hInstance);
static bool InitInstance(HINSTANCE hInstance, int nCmdShow);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern void CreateDIB(void);
extern void DestroyDIB(void);

//////////////////////////////////////////////////////////////////////////
// structures
//////////////////////////////////////////////////////////////////////////
// extern function

//////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
    // TODO: Place code here.
    g_hInstance = hInstance;

    g_hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_REXY_INTEGRATED);

    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	CreateDIB();
    StartRexy();
	DestroyDIB();
    //
    // EndRexy();

    return 0;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASS wcex;

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_REXY_INTEGRATED);
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= szWindowClass;
   
    return RegisterClass(&wcex);
}
//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
static bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    RECT windowRect;
    DWORD style;
    hInst = hInstance; // Store instance handle in our global variable

    windowRect.left = 0;
    windowRect.right = 240;
    windowRect.top = 0;
	windowRect.bottom = 320;

    style = WS_VISIBLE;
	
    // Create emulator window except for LCD window
    // LCD window : real screen that shows REXY
	g_hMainWnd = CreateWindow((LPCTSTR)szWindowClass, (LPCTSTR)szTitle, style,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              NULL, NULL, hInstance, NULL);
    if (!g_hMainWnd)
    {
        return FALSE;
    }

	SHFullScreen(g_hMainWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON |  SHFS_HIDESTARTICON);
	MoveWindow( g_hMainWnd, 0, 0, windowRect.right, windowRect.bottom, TRUE);
    ShowWindow( g_hMainWnd, nCmdShow );
    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//  
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    UINT elapse;
    static int timer_set = -1;
    static int img_clicked = 0;
    static enum {SHORTKEY, LONGKEY, REPEATKEY} eLongKey = SHORTKEY;

    switch (message)
    {
    case WM_CREATE:
        g_lcd.bpp_mode = g_bpp;
        InvalidateRect(g_hMainWnd, NULL, TRUE);
        break;

    case WM_COMMAND:
        break;

    case WM_INITMENUPOPUP:
        break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        RalScreenRepaint(hWnd, &ps);
		//SHFullScreen(g_hMainWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON |  SHFS_HIDESTARTICON);
        EndPaint(hWnd, &ps);
        break;

    case WM_TIMER:
        break;

    case WM_DESTROY:
        if (timer_set == 1)
        {
            timer_set = -1;
            KillTimer(hWnd, TIMER_ID);
        }
        KillTimer(hWnd, ENV_TIMER_ID);

        //EndRexy();
        ExitProcess(0);
        break;

    case AWM_SETTIMER:
        elapse = wParam * 1000 + lParam / 1000;
        SetTimer(hWnd, TIMER_ID, elapse, NULL);
        timer_set = 1;
        break;

    case AWM_KILLTIMER:
        if (timer_set == 1)
            KillTimer(hWnd, TIMER_ID);
        timer_set = -1;
        break;

    case WM_LBUTTONDOWN:
        break;

    case WM_LBUTTONUP:
        break;

        // To use keyboard device
    case WM_KEYDOWN:
    case WM_KEYUP:
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }

    return FALSE;
}

BITMAPFILEHEADER *LoadBitmapFile(LPCTSTR pszFileName)
{
    bool bSuccess;
    BITMAPFILEHEADER *pHeader;
    DWORD dwFileSize, dwHighSize, dwBytesRead;
    HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return NULL;

    dwFileSize = GetFileSize(hFile, &dwHighSize);

    if (dwHighSize)
    {
        CloseHandle(hFile);
        return NULL;
    }

    pHeader = (BITMAPFILEHEADER *) malloc(dwFileSize);
    if (!pHeader)
    {
        CloseHandle(hFile);
        return NULL;
    }

    bSuccess = ReadFile(hFile, pHeader, dwFileSize, &dwBytesRead, NULL);

    CloseHandle(hFile);

    if (!bSuccess || (dwBytesRead != dwFileSize)
            || (pHeader->bfType != *(WORD *) "BM")
            || (pHeader->bfSize != dwFileSize))
    {
        free(pHeader);
        return NULL;
    }

    return pHeader;
}


static UNITCL g_converted_bitmap[320 * 240];
static UNITCL * ReflectBrightness(int brightness, UNITCL * source)
{
    int i;

    if ( brightness == 3 )
    {
        return source;
    }

    for ( i = 0; i < 320 * 240; ++i )
    {
        unsigned short pixel;
        int R, G, B, mixing_pixel;
        double blending_factor;

        pixel = source[i];

        R = (pixel & 0xF800) >> 11;
        G = (pixel & 0x07E0) >> 5;
        B = (pixel & 0x001F);

        mixing_pixel = 0;
        blending_factor = (double)brightness / 3;
        R = (int)(mixing_pixel * (1 - blending_factor) + R * blending_factor);
        G = (int)(mixing_pixel * (1 - blending_factor) + G * blending_factor);
        B = (int)(mixing_pixel * (1 - blending_factor) + B * blending_factor);

        if ( R >= 0x1F )
        {
            R = 0x1F;
        }

        if ( G >= 0x3F )
        {
            G = 0x3F;
        }

        if ( B >= 0x1F )
        {
            B = 0x1F;
        }

        pixel = (UNITCL)(B | (G << 5) | (R << 11));
        g_converted_bitmap[i] = pixel;
    }

    return g_converted_bitmap;
}

