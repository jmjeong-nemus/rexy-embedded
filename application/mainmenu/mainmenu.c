#include "rexy.h"
#include "rexy_haldevice.h"
#include "rexy_ctl_anibmp.h"

#include "application.h"
#include "mainmenu.h"

#define MAINMENU_ITEMS_COUNT	8
#define NORMAL_IMAGE_WIDTH		223
#define NORMAL_IMAGE_HEIGHT		43
#define ACTIVE_IMAGE_WIDTH		223
#define ACTIVE_IMAGE_HEIGHT		92

static int nLocation = 0; // 비트맵 리스트의 처음 위치
static int nCurrLoc = 0; //비트맵 현재 활성화위치
static int nDstY = 0;

MNGlobalType g_mnGlobalType[MAINMENU_ITEMS_COUNT] = 
{
	{0, "Call",			MGL_mainmenu_call_n_I,	MGL_mainmenu_call_a1_I,		MGL_mainmenu_call_a2_I,		MGL_mainmenu_call_a3_I},
	{1, "Mail",			MGL_mainmenu_mail_n_I,	MGL_mainmenu_mail_a1_I,		MGL_mainmenu_mail_a2_I,		MGL_mainmenu_mail_a3_I},
	{2, "Camera",		MGL_mainmenu_camera_n_I,MGL_mainmenu_camera_a1_I,	MGL_mainmenu_camera_a2_I,	MGL_mainmenu_camera_a3_I},
	{3, "Ezweb",		MGL_mainmenu_ezweb_n_I,	MGL_mainmenu_ezweb_a1_I,		MGL_mainmenu_ezweb_a2_I,		MGL_mainmenu_ezweb_a3_I},
	{4, "DataFolder",	MGL_mainmenu_datafolder_n_I,	MGL_mainmenu_datafolder_a1_I,		MGL_mainmenu_datafolder_a2_I,		MGL_mainmenu_datafolder_a3_I},
	{5, "UserFunction",	MGL_mainmenu_tools_n_I,	MGL_mainmenu_tools_a1_I,		MGL_mainmenu_tools_a2_I,		MGL_mainmenu_tools_a3_I},
	{6, "Setting",		MGL_mainmenu_setting_n_I,	MGL_mainmenu_setting_a1_I,		MGL_mainmenu_setting_a2_I,		MGL_mainmenu_setting_a3_I},
	{7, "Profile",		MGL_mainmenu_mynumber_n_I,	MGL_mainmenu_mynumber_a1_I,		MGL_mainmenu_mynumber_a2_I,		MGL_mainmenu_mynumber_a3_I},
};

void MainMenu_Load(WND wnd)
{
		RxBITMAP bitmap;
		rect rt = { 0, 0, LCD_WIDTH, LCD_HEIGHT };
		int nMenuIndex = 0;
		int srcX = 0;
		int srcY = 0;
		int dstX = 0;
		int dstY = 0;
		int i;

		nMenuIndex = g_mnGlobalType[nLocation].nMenuIndex;
		
	
		GdiFillRect(wnd, &rt, MAKERGB(75,75,75));

		for(i = 0; i < 6; i++)
		{
			if(i == nMenuIndex)
			{
				nDstY = dstY;
				RscLoadBitmap(g_mnGlobalType[i].szActive1, &bitmap);
				GdiDrawBitmap(wnd, &bitmap, srcX, srcY, dstX, dstY, bitmap.width, bitmap.height);
				dstY = dstY + ACTIVE_IMAGE_HEIGHT;
			}
			else
			{
				RscLoadBitmap(g_mnGlobalType[i].szNormal, &bitmap);
				GdiDrawBitmap(wnd, &bitmap, srcX, srcY, dstX, dstY, bitmap.width, bitmap.height);
				dstY = dstY + NORMAL_IMAGE_HEIGHT;
				
			}

		}

		UsrSetTimer(wnd, REXY_MAINMENU_TIMER, 200);


}

void MainMenu_Timer(WND wnd, int msg, int wparam, int lparam)
{
	RxBITMAP bitmap;
	static int i=0;
//	int x;

//	if(nCurrLoc > 5)
//		x = nCurrLoc - (nCurrLoc -5);
//	else
//		x = nCurrLoc;

	if(wparam == REXY_MAINMENU_TIMER)
	{
		if(i == 0)
		{
			RscLoadBitmap(g_mnGlobalType[nCurrLoc].szActive2, &bitmap);
			GdiDrawBitmap( wnd, &bitmap, 0, 0, 0, 	nDstY , bitmap.width, bitmap.height );
			i++;
		}
		else if (i == 1)
		{
			RscLoadBitmap(g_mnGlobalType[nCurrLoc].szActive2, &bitmap);
			GdiDrawBitmap( wnd, &bitmap, 0, 0, 0, nDstY, bitmap.width, bitmap.height );

			i = 0;
			UsrKillTimer(wnd, REXY_MAINMENU_TIMER);
		}
	}
}

void MainMenu_Key(WND wnd, int wparam, int lparam)
{
	RxBITMAP bitmap;
	rect rt = { 0, 0, LCD_WIDTH, LCD_HEIGHT };
//	static int nCurrLoc = nLocation;
	int nTmpMenuIndex;
	int nMenuIndex;
	int srcX = 0;
	int srcY = 0;
	int dstX = 0;
	int dstY = 0;
	int i;

	switch(wparam)
	{
	case VK_USER_UP:
	case VK_USER_DOWN:
		UsrKillTimer( wnd, REXY_MAINMENU_TIMER );
		break;
	}

	//메세지 구분
	switch(wparam)
	{
	case VK_USER_DOWN:
		nTmpMenuIndex = g_mnGlobalType[nCurrLoc].nMenuIndex;
		
		if(nTmpMenuIndex == nLocation +5)
		{
			nLocation++;			
		}

		if(nTmpMenuIndex == 7)
		{
			nLocation = 0;
			nCurrLoc = 0;
		}
		else
			nCurrLoc++;
		
		nMenuIndex = g_mnGlobalType[nCurrLoc].nMenuIndex;

		GdiFillRect(wnd, &rt, MAKERGB(75,75,75));

		for(i = nLocation; i < nLocation+6; i++)
		{
			if(i == nMenuIndex)
			{
				nDstY = dstY;
				RscLoadBitmap(g_mnGlobalType[i].szActive1, &bitmap);
				GdiDrawBitmap(wnd, &bitmap, srcX, srcY, dstX, dstY, bitmap.width, bitmap.height);
				dstY = dstY + ACTIVE_IMAGE_HEIGHT;
				
			}
			else
			{
				RscLoadBitmap(g_mnGlobalType[i].szNormal, &bitmap);
				GdiDrawBitmap(wnd, &bitmap, srcX, srcY, dstX, dstY, bitmap.width, bitmap.height);
				dstY = dstY + NORMAL_IMAGE_HEIGHT;
				
			}

		}

		break;

	case VK_USER_UP:
		nTmpMenuIndex = g_mnGlobalType[nCurrLoc].nMenuIndex;
		
		if(nTmpMenuIndex == nLocation)
		{
			nLocation--;			
		}

		if(nTmpMenuIndex == 0)
		{
			nLocation = 2;
			nCurrLoc = 7;
		}
		else
			nCurrLoc--;
		
		nMenuIndex = g_mnGlobalType[nCurrLoc].nMenuIndex;

		GdiFillRect(wnd, &rt, MAKERGB(75,75,75));

		for(i = nLocation; i < nLocation+6; i++)
		{
			if(i == nMenuIndex)
			{
				nDstY = dstY;
				RscLoadBitmap(g_mnGlobalType[i].szActive1, &bitmap);
				GdiDrawBitmap(wnd, &bitmap, srcX, srcY, dstX, dstY, bitmap.width, bitmap.height);
				dstY = dstY + ACTIVE_IMAGE_HEIGHT;
			
			}
			else
			{
				RscLoadBitmap(g_mnGlobalType[i].szNormal, &bitmap);
				GdiDrawBitmap(wnd, &bitmap, srcX, srcY, dstX, dstY, bitmap.width, bitmap.height);
				dstY = dstY + NORMAL_IMAGE_HEIGHT;
			}

		}

	}

	UsrSetTimer(wnd, REXY_MAINMENU_TIMER, 200);
}
rint32 PrvMainMenuProc(WND wnd, int msg, int wparam, int lparam)
{
	switch (msg)
	{
	case AWM_PAINT:
		MainMenu_Load(wnd);
		return 0;

	case AWM_KEYDOWN:
	//	MsgKeyDown( wnd, wparam, lparam );
		MainMenu_Key(wnd, wparam, lparam);
		return 0;

	case AWM_TIMER:
		MainMenu_Timer(wnd, msg, wparam, lparam);
		return 0;
	}

	return 0;
}
rint32 MainMenuProc(WND wnd, int msg, int wparam, int lparam)
{
	int	ScreenWidth;
	int ScreenHeight;
	WND SamCtrlWnd;
	int bHandled = FALSE;
    switch(msg)
    {
    case AWM_CREATE:
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
		SamCtrlWnd = WinCreateWindow(PrvMainMenuProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL);
		WinSetSelection( SamCtrlWnd );
		GdiSetBkColor( wnd, COLOR_TRANSPARENT );
		GdiSetFgColor( wnd, COLOR_WHITE );
		MsgPostMessage( SamCtrlWnd, AWM_PAINT, 0, 0 );
		bHandled = TRUE;
		nLocation = g_mnGlobalType[0].nMenuIndex;
        break;

    case AWM_DESTROY:
		WinDestroyWindow(wnd);
		bHandled = TRUE;

		UsrKillTimer(wnd, REXY_MAINMENU_TIMER);
        break;

    default:
        break;
    }

    return bHandled;
}
