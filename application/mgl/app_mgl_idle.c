/**
 * @brief Private Macros
 */

#include "rexy.h"
#include "rexy_haldevice.h"
#include "application.h"

#include "mgl.h"

/**
 * @brief Private Function Prototypes
 */
static int MsgCreate( WND wnd, int msg, int wparam, int lparam );
static int MsgDestroy( WND wnd, int msg, int wparam, int lparam );
static int MsgKeyDown( WND wnd, int key, int option );

static void  PrvDrawBackground( WND wnd );
static int PrvMGLMenuWindowProc(WND wnd, int msg, int wparam, int lparam);
/** 
 * @brief 배경 이미지를 그린다. 
 */
static void PrvDrawBackground( WND wnd)
{
	RxBITMAP bitmap;
	rint16 nY, nOffset = 20;
	rect rt = { 0, 65, 239, 282 };

	RscLoadBitmap( MGL_title_I, &bitmap );
	GdiDrawBitmap( wnd, &bitmap, 0, 0, 0, 0, bitmap.width, bitmap.height );

	GdiFillRect( wnd, &rt, COLOR_WHITE );

	RscLoadBitmap( MGL_softkey_I, &bitmap );
	GdiDrawBitmap( wnd, &bitmap, 0, 0, 0, 282, bitmap.width, bitmap.height );

	GdiSetBkColor( wnd, COLOR_TRANSPARENT );
	GdiSetFgColor( wnd, COLOR_WHITE );
	GdiDrawTextOut( wnd, 0, 20, 240, "NeoGraphic Library V1.0", 27, ALIGN_CENTER );

	GdiSetFgColor( wnd, COLOR_BLACK );
	nY = 90;
	
	GdiDrawTextOut( wnd, 50, nY, 240, "1. Motion Blur", 14, ALIGN_LEFT );
	nY += nOffset;
	GdiDrawTextOut( wnd, 50, nY, 240, "2. Particle + Pile", 18, ALIGN_LEFT );
	nY += nOffset;
	GdiDrawTextOut( wnd, 50, nY, 240, "3. Scale", 8, ALIGN_LEFT );
	nY += nOffset;
	GdiDrawTextOut( wnd, 50, nY, 240, "4. Animation", 12, ALIGN_LEFT );
	nY += nOffset;
	GdiDrawTextOut( wnd, 50, nY, 240, "5. Key frame", 12, ALIGN_LEFT );
	nY += nOffset;
	GdiDrawTextOut( wnd, 50, nY, 240, "6. Back to Title", 16, ALIGN_LEFT );


	GdiSetFgColor( wnd, COLOR_WHITE );
	GdiDrawTextOut( wnd, 0, 295, 240, "Press Keypad !", 14, ALIGN_CENTER );
	return;
}

static int MsgCreate(WND wnd, int msg, int wparam, int lparam)
{
	int ScreenWidth, ScreenHeight;
	WND	hMGLWindow;
	
	// Create Window for this application
	GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
	hMGLWindow= WinCreateWindow( PrvMGLMenuWindowProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );
	MGL_InitEngine( hMGLWindow );
	
	StaGotoState( wnd, STATE_ID_MGL_SAMPLE2, MSG_NULL, MSG_CREATE);
	//PrvDrawBackground( hMGLWindow );
	return TRUE;
}

static int MsgDestroy(WND wnd, int msg, int wparam, int lparam)
{
	MGL_DeinitEngine();
	WinDestroyWindow(wnd);
	return 0;
}

static int MsgKeyDown( WND wnd, int key, int option )
{	
	switch( key )
	{
	case VK_PAD_1:
	case VK_PAD_STAR:
		StaGotoState( wnd, STATE_ID_MGL_SAMPLE1, MSG_NULL, MSG_CREATE );
		break;
		
	case VK_PAD_2:
	case VK_PAD_POUND:
		StaGotoState( wnd, STATE_ID_MGL_SAMPLE2, MSG_NULL, MSG_CREATE);
		break;
	case VK_PAD_3:
	case VK_USER_CLR:
		StaGotoState( wnd, STATE_ID_MGL_SAMPLE3, MSG_NULL, MSG_CREATE);
		break;
	case VK_PAD_4:
	//case VK_USER_SEND:
		StaGotoState( wnd, STATE_ID_MGL_SAMPLE4, MSG_NULL, MSG_CREATE);
		break;
	case VK_PAD_5:
	//case VK_USER_SOFT1:
		StaGotoState( wnd, STATE_ID_MGL_SAMPLE5, MSG_NULL, MSG_CREATE);
		break;
	case VK_PAD_6:
		StaGotoState( wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );
		break;
	case VK_USER_OK:
		break;
	}

	return 0;
}

int MGLDemoIdleProc(WND wnd, int msg, int wparam, int lparam)
{
	int bHandled=FALSE;

	switch (msg)
	{
	case AWM_CREATE:
		bHandled = TRUE;
		MsgCreate(wnd, msg, wparam, lparam);
		break;

	case AWM_RETURN:
		// Below method is not desirable, but MGL should use same window
		WinSetCurrentProc( wnd, PrvMGLMenuWindowProc );
		StaGotoState( wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );
		//WinUpdateWindow( wnd, ERASE_BKGND );	// we use same window handle
		break;

	case AWM_DESTROY:
		bHandled = TRUE;
		MsgDestroy(wnd, msg, wparam, lparam);
		break;
	}

	return bHandled;
}
static int PrvMGLMenuWindowProc(WND wnd, int msg, int wparam, int lparam)
{
	switch ( msg )
	{
	case AWM_KEYDOWN:
		MsgKeyDown( wnd, wparam, lparam );
		return TRUE;	// handled this event

	case AWM_PAINT:
		PrvDrawBackground( wnd );
		return 0;
	}

	return 0;
}
