
#if !defined(AFX_REXY_INTEGRATED_H__8FB03029_8694_4321_8DC9_518FE30B9FD6__INCLUDED_)
#define AFX_REXY_INTEGRATED_H__8FB03029_8694_4321_8DC9_518FE30B9FD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

/*
 *	application of new skin set
 *	chemistz, 2005-03-29 ~ 2005-03-30
 *
 *	* expansion key press area : 2005-04-01
 *	- '+10 means key press area expansion
 */
//
// background
#define		SKIN_BACKGROUND_WIDTH			254
#define		SKIN_BACKGROUND_HEIGHT			720
#define		SKIN_DATUM_BACKGROUND			"J_skin.BMP",	0,	0, SKIN_BACKGROUND_WIDTH, SKIN_BACKGROUND_HEIGHT
// soft keys
#define		SKIN_DATUM_VK_USER_SOFT1		"skin16.bmp",	26,		336,	45,		44
#define		SKIN_DATUM_VK_USER_SOFT2		"skin18.bmp",	183,	336,	45,		44
// OK, navigation
#define		SKIN_DATUM_VK_USER_OK			"skin20.bmp",	101,	379,	52,		52
#define		SKIN_DATUM_VK_USER_LEFT			"skin22.bmp",	71,		374,	30,		62
#define		SKIN_DATUM_VK_USER_RIGHT		"skin23.bmp",	153,	374,	30,		62
#define		SKIN_DATUM_VK_USER_UP			"skin21.bmp",	96,		349,	62,		30
#define		SKIN_DATUM_VK_USER_DOWN			"skin24.bmp",	96,		431,	62,		30
// mail, ezweb, end, end, clear
#define		SKIN_DATUM_VK_USER_MAIL			"skin17.bmp",	26,		428,	45,		44
#define		SKIN_DATUM_VK_USER_EZWEB		"skin19.bmp",	183,	428,	45,		44
#define		SKIN_DATUM_VK_USER_SEND			"skin13.bmp",	20,		493,	69,		29 +10
#define		SKIN_DATUM_VK_USER_END			"skin15.bmp",	164,	493,	69,		29 +10
#define		SKIN_DATUM_VK_USER_CLR			"skin14.bmp",	92,		493,	69,		29 +10
// digits, sharp, asterisk
#define		SKIN_DATUM_VK_PAD_1				"skin01.bmp",	20,		541,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_2				"skin02.bmp",	92,		541,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_3				"skin03.bmp",	164,	541,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_4				"skin04.bmp",	20,		587,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_5				"skin05.bmp",	92,		587,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_6				"skin06.bmp",	164,	587,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_7				"skin07.bmp",	20,		632,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_8				"skin08.bmp",	92,		632,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_9				"skin09.bmp",	164,	632,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_STAR			"skin11.bmp",	20,		678,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_0				"skin10.bmp",	92,		678,	69,		22 +10
#define		SKIN_DATUM_VK_PAD_POUND			"skin12.bmp",	164,	678,	69,		22 +10

//////////////////////////////////////////////////////////////////////////
// various numerical value

#define		MAX_LOADSTRING			100
#define		MAX_SKINNAME			256
#define		SKIN_NUMBER				(24+1)

#define		MAINLCD_WIDTH			240
#define		MAINLCD_HEIGHT			320

#define		LCD_X_POSITION			7
#define		LCD_Y_POSITION			5

#define		REXY_REPEATKEY_ELAPSE		250
#define		REXY_LONGKEY_ELAPSE			500
#define		REXY_LONGKEY_CLOSE_ELAPSE	2000

//////////////////////////////////////////////////////////////////////////
// message value

#define WM_DRAWLCD				(WM_APP + 1)
#define WM_DRAWKEYPAD			(WM_APP + 2)

//////////////////////////////////////////////////////////////////////////
// virtual keycode value

// VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39)
#define VK_0					0x30
#define VK_1					0x31
#define VK_2					0x32
#define VK_3					0x33
#define VK_4					0x34
#define VK_5					0x35
#define VK_6					0x36
#define VK_7					0x37
#define VK_8					0x38
#define VK_9					0x39
#define VK_PAGEUP				0x21	//VK_PRIOR
#define VK_PAGEDN				0x22	//VK_NEXT

//////////////////////////////////////////////////////////////////////////
// function definition

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	DLCDWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void				DrawLCD(HWND hWnd, HDC hdc, LCD_INFO * scr);
void				LoadSkin(void);
void				LoadDefaultSkin(void);
void				DrawSkin(HWND hWnd, HDC hdc, SKIN_BASE_INFO * skin_base);
void				DestroySkin();
void				InvertButtons(int img_clicked);
void				InvertAllButtons(HWND hWnd);
void				RestoreButtons(int img_clicked);
int					PtInButtons(int x, int y);
HWND				CreateDoubleSizeLCD(HWND hWnd);
HWND				CreateSysWindow(HWND hWnd);

void				SetTraceMenu(int eArea, int eTraceLvl);
void				UpdateTracemenu(void);

BITMAPFILEHEADER * LoadBitmapFile(LPCTSTR pszFileName);

//////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_REXY_INTEGRATED_H__8FB03029_8694_4321_8DC9_518FE30B9FD6__INCLUDED_)
