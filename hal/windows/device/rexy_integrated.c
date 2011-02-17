/*
 * $Id: rexy_integrated.c 221 2005-01-21 01:27:30Z jmjeong $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */
// Windows Header Files:

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <malloc.h>

#include "resource.h"

#include "rexy.h"
#include "rexy_haldevice.h"

#include "rexy_debug_win.h"
#include "rexy_test_win.h"

//OS dependant device layer
extern void RalScreenRepaint(HWND hWnd, LPPAINTSTRUCT ps);

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
const char szWindowClass[] = "RexyWndClass";
const char szTitle[] = "Rexy Simulator";

//////////////////////////////////////////////////////////////////////////
// local methods
static ATOM MyRegisterClass(HINSTANCE hInstance);
static bool InitInstance(HINSTANCE hInstance, int nCmdShow);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


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
	
	InitDebugWindow(g_hMainWnd, hInstance);

    StartRexy();
    //
    //  EndRexy();

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
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_REXY_INTEGRATED);
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU_MAIN);
    wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

    return RegisterClassEx(&wcex);
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
	UINT menuHeight;
    DWORD style;
    hInst = hInstance; // Store instance handle in our global variable

    windowRect.left = 0;
    windowRect.right = LCD_WIDTH;
    windowRect.top = 0;
    windowRect.bottom = LCD_HEIGHT;
	menuHeight = 20;

    style = WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX;
    AdjustWindowRect( &windowRect, style, FALSE );

    // Create emulator window except for LCD window
    // LCD window : real screen that shows REXY
    g_hMainWnd = CreateWindow(szWindowClass, szTitle, style,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              windowRect.right - windowRect.left,
                              windowRect.bottom - windowRect.top + menuHeight,
                              NULL, NULL, hInstance, NULL);

    if (!g_hMainWnd)
    {
        return FALSE;
    }

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
		if(wParam == ID_TEST_MONITOR)
			InitTestWindow(hWnd, g_hInstance);
        break;

    case WM_INITMENUPOPUP:
        break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        RalScreenRepaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        if (timer_set == 1)
        {
            timer_set = -1;
            KillTimer(hWnd, TIMER_ID);
        }

        //EndRexy();
        ExitProcess(0);
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


static UNITCL g_converted_bitmap[LCD_HEIGHT * LCD_WIDTH];
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

