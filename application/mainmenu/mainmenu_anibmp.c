#include "rexy.h"
#include "rexy_haldevice.h"
#include "rexy_ctl_anibmp.h"

#include "application.h"
#include "mainmenu_anibmp.h"

#define MAINMENU_ITEMS_COUNT	8
#define NORMAL_IMAGE_WIDTH		223
#define NORMAL_IMAGE_HEIGHT		43
#define ACTIVE_IMAGE_WIDTH		223
#define ACTIVE_IMAGE_HEIGHT		92
#define NUM_OF_IMG				3

static int nLocation = 0; // 비트맵 리스트의 처음 위치
static int nCurrLoc = 0; //비트맵 현재 활성화위치
WND g_wndMenuItem;

MNGlobalType g_mnAniBmpGlobalType[MAINMENU_ITEMS_COUNT] = 
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

void MainMenuAniBmp_Load(WND wnd)
{
	RxBITMAP bitmap;
	rect Rect;
	static int aBmpIDs[NUM_OF_IMG];
	int nErr;
	int i, j, menu_index;
	int srcX = 0, srcY = 0;
	int dstX = 0, dstY = 0;

	menu_index = g_mnAniBmpGlobalType[nCurrLoc].nMenuIndex;

	GdiLockScreen(wnd);
	GdiClrScreen(wnd, MAKERGB(75,75,75));

	for(i=nLocation; i<nLocation + 6; i++)
	{
		if(i == menu_index)
		{
			//animation bmp load
			for(j = 0; j < NUM_OF_IMG; j++)
				aBmpIDs[j] = g_mnAniBmpGlobalType[i].szNormal + j;

			SET_RECT(&Rect, dstX, dstY, dstX + ACTIVE_IMAGE_WIDTH, dstY + ACTIVE_IMAGE_HEIGHT);
	
			g_wndMenuItem = CtlCreate(wnd, CTL_TYPE_ANIBMP, &Rect, NULL, &nErr);

			CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_SET_LEFT, dstX, &nErr);
			CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_SET_TOP, dstY, &nErr);
			CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_SET_WIDTH, ACTIVE_IMAGE_WIDTH, &nErr);
			CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_SET_HEIGHT, ACTIVE_IMAGE_HEIGHT, &nErr);

			CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_SET_BGCOLOR, MAKERGB(75,75,75), &nErr);
			CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_SET_FRAME_NO, NUM_OF_IMG, &nErr);
			CtlSetPtr(g_wndMenuItem, CTL_ANIBMP_CMD_SET_P_IDS, aBmpIDs, &nErr);
			CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_SET_PLAYTYPE, CTL_ANIBMP_PLAY_NORMAL, &nErr);
			CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_SET_TIMEOUT, 500, &nErr);

			dstY += ACTIVE_IMAGE_HEIGHT;
		}
		else
		{
			//normal bmp load
			RscLoadBitmap(g_mnAniBmpGlobalType[i].szActive3, &bitmap);
			GdiDrawBitmap(wnd, &bitmap, srcX, srcY, dstX, dstY, bitmap.width, bitmap.height);

			dstY += NORMAL_IMAGE_HEIGHT;
		}
	}

	GdiUnlockScreen(wnd);

	CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_START_STOP, TRUE, &nErr);
}

void MainMenuAniBmp_Key(WND wnd, int wparam, int lparam)
{
	int nErr;
	
	switch(wparam)
	{
	case VK_USER_DOWN:
		CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_START_STOP, FALSE, &nErr);

		if(g_wndMenuItem)
		{
			CtlDestroy(g_wndMenuItem, &nErr);
			g_wndMenuItem = NULL;
		}

		if(g_mnAniBmpGlobalType[nCurrLoc].nMenuIndex == 7)
		{
			nLocation = 0;
			nCurrLoc = 0;
		}
		else
			nCurrLoc++;

		if(nCurrLoc == nLocation +6)
		{
			nLocation++;			
		}

		MainMenuAniBmp_Load(wnd);
		break;
	case VK_USER_UP:
		CtlSeti(g_wndMenuItem, CTL_ANIBMP_CMD_START_STOP, FALSE, &nErr);

		if(g_wndMenuItem)
		{
			CtlDestroy(g_wndMenuItem, &nErr);
			g_wndMenuItem = NULL;
		}

		if(g_mnAniBmpGlobalType[nCurrLoc].nMenuIndex == 0)
		{
			nLocation = 2;
			nCurrLoc = 7;
		}
		else
			nCurrLoc--;

		if(nCurrLoc == nLocation -1 )
		{
			nLocation--;			
		}

		MainMenuAniBmp_Load(wnd);
		break;

	}
}

rint32 PrvMainMenuAniBmpProc(WND wnd, int msg, int wparam, int lparam)
{
	int nErr = CTLERR_NONE;

	switch (msg)
	{
	case AWM_CREATE:
		MainMenuAniBmp_Load(wnd);
		break;

	case AWM_DESTROY:
		break;

	case AWM_PAINT:
		//MainMenuAniBmp_Load(wnd);
		return 0;

	case AWM_KEYDOWN:
	//	MsgKeyDown( wnd, wparam, lparam );
		MainMenuAniBmp_Key(wnd, wparam, lparam);
		return 0;

	case AWM_TIMER:
		return 0;
	}

	return 0;
}

rint32 MainMenuAniBmpProc(WND wnd, int msg, int wparam, int lparam)
{
	int	ScreenWidth, ScreenHeight;
	WND SamCtrlWnd;
	int bHandled = FALSE;

    switch(msg)
    {
    case AWM_CREATE:
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
		SamCtrlWnd = WinCreateWindow(PrvMainMenuAniBmpProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL);
		WinSetSelection( SamCtrlWnd );
		GdiSetBkColor( wnd, COLOR_TRANSPARENT );
		GdiSetFgColor( wnd, COLOR_WHITE );
		nLocation = g_mnAniBmpGlobalType[0].nMenuIndex;

		bHandled = TRUE;
        break;

    case AWM_DESTROY:
		WinDestroyWindow(wnd);
		bHandled = TRUE;
        break;

    default:
        break;
    }

    return bHandled;
}
