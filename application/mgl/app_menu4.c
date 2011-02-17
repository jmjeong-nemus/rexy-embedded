#include "rexy.h"
#include "rexy_haldevice.h"

#include "mgl.h"
#include "mgl_ip.h"
#include "mgl_animator.h"
#include "mgl_imageselector.h"
#include "application.h"

/**
 * @brief Private Macros
 */
/**
 * @brief	0x00 : Background layer
 *			0x01 - 0x04 : Slide Menu layer 
 *			0x05 - 0x10 : Menu item layer
 */
#ifndef F_REXY_ENV_PHONE
	#define TIMER_MGL_SLIDE_MENU	0x1
	#define TIMER_VAL_SLIDE_MENU	1					//###
	#define TIMER_MGL_MENU_FOCUS	0x2
	#define TIMER_VAL_MENU_FOCUS	1
	#define TIMER_MGL_ITEM_ANI1		0x3				//### 이미지 IP 값 변경
	#define TIMER_VAL_ITEM_ANI1		1
	#define TIMER_MGL_ITEM_ANI2		0x4				//### 소스 이미지 변경
	#define TIMER_VAL_ITEM_ANI2		1
	#define TIMER_MGL_EXPLOSION_ANI	0x5
	#define TIMER_VAL_EXPLOSION_ANI	1
#else
	#define TIMER_MGL_SLIDE_MENU	0x1
	#define TIMER_VAL_SLIDE_MENU	1				//###
	#define TIMER_MGL_MENU_FOCUS	0x2
	#define TIMER_VAL_MENU_FOCUS	1
	#define TIMER_MGL_ITEM_ANI1		0x3				//### 이미지 IP 값 변경
	#define TIMER_VAL_ITEM_ANI1		1
	#define TIMER_MGL_ITEM_ANI2		0x4				//### 소스 이미지 변경
	#define TIMER_VAL_ITEM_ANI2		1
	#define TIMER_MGL_EXPLOSION_ANI	0x5
	#define TIMER_VAL_EXPLOSION_ANI	1
#endif	//#ifdef REXY_ENV_PHONE

#define ID_LAYER_BACK			0					// Back Layer ID
#define ID_LAYER_MENU_SLIDE1	1					// Menu Slide Layer ID
#define ID_LAYER_MENU_SLIDE2	2					// ...
#define ID_LAYER_MENU_SLIDE3	3					// ...
#define ID_LAYER_MENU_SLIDE4	4					// ...
#define ID_LAYER_MENU_FOCUS		5					// Selected Menu Item Layer ID
#define ID_LAYER_MENU_ITEM		6					// Menu Item Layer ID
#define ID_LAYER_EXPLOSION		7					// Explosion effect Layer ID
#define ID_LAYER_END			ID_LAYER_EXPLOSION
#define NUM_LAYERS				ID_LAYER_END+1

#define MENU_ITEM_WIDTH			72					// 메뉴 아이템의 가로 길이
#define MENU_ITEM_HEIGHT		70					// 메뉴 아이템의 세로 길이 

#define MENU_FOCUS_WIDTH		82					// 선택된 메뉴 아이템의 가로 길이.
#define MENU_FOCUS_HEIGHT		82					// 선택된 메뉴 아이템의 세로 길이.

#define EXPLOSION_WIDTH			120					// 폭발 효과 이미지의 가로 길이.
#define	EXPLOSION_HEIGHT		110					// 폭발 효과 이미지의 세로 길이.

typedef struct MGL_MENU {
	int x;					//< icon의 중심 X좌표
	int y;					//< icon의 중심 Y좌표
	char szTitle[20];		//< 메뉴 아이템의 타이틀
} MGL_MENU;

/**
 * @brief Private Variables
 */
static MGLrc					g_tRC;
static MGLlayer					g_aLayers[NUM_LAYERS];
static MGLalphablendingparams	g_tBackABParams;	// 배경 Alphablending 정보 구조체이다.
static MGLip					g_tBackIP;			// 배경 Image Processing 구조체이다.

static MGLalphablendingparams	g_tMenuABParams1;	// 메뉴1 Alphablending 정보 구조체이다.
static MGLalphablendingparams	g_tMenuABParams2;	// 메뉴2 Alphablending 정보 구조체이다.
static MGLalphablendingparams	g_tMenuABParams3;	// 메뉴3 Alphablending 정보 구조체이다.
static MGLalphablendingparams	g_tMenuABParams4;	// 메뉴4 Alphablending 정보 구조체이다.
static MGLip					g_tMenuIP1;			// 메뉴1 Image Processing 구조체이다.
static MGLip					g_tMenuIP2;			// 메뉴2 Image Processing 구조체이다.
static MGLip					g_tMenuIP3;			// 메뉴3 Image Processing 구조체이다.
static MGLip					g_tMenuIP4;			// 메뉴4 Image Processing 구조체이다.

static MGLalphablendingparams	g_tFocusABParams;	// Focus Item Alphablending 정보 구조체이다.
static MGLscalingparams			g_tFocusScaleParams;// Focus Item Scaling 정보 구조체이다.
static MGLip					g_tFocusIP[2];		// Focus item Image Processing 구조체이다.

static MGLalphablendingparams	g_tItemABParams;	// Menu Item Alphablending 정보 구조체이다.
static MGLscalingparams			g_tItemScaleParams;	// Menu Item Scaling 정보 구조체이다.
static MGLip					g_tItemIP[2];		// Menu Item Image Processing 구조체이다.

#define NUM_MENU_ITEM			12					// 메뉴 아이템의 갯수 

static MGL_MENU		g_tMainMenu[NUM_MENU_ITEM] = {
		{  41, 101, "To DO" },		{ 120, 101, "Contacts" },	{ 199, 101, "Multimedia" },
		{  41, 146, "Internet" },	{ 120, 146, "Mailbox" },	{ 199, 146, "Download" },
		{  41, 201, "Schedule" },	{ 120, 201, "Camera" },		{ 199, 201, "Settings" },
		{  41, 247, "PDA" },		{ 120, 247, "Time" },		{ 199, 247, "Alarm" }
};
static int g_nCurMenuIdx = 0;	// 현재 메뉴 Index
static int g_nPrevMenuIdx = 0;	// 이전 메뉴 Index

static int g_nSlideCount;		// Slide menu 애니메이션.
static int g_nFocusCount;		// Focus 애니메이션.
static int g_nItemCount;		// Menu Item 애니메이션.
static int g_nItemImage;		// Menu Item 애니메이션 시작 이미
static int g_nExplosionCount;	// Explosion 애니메이션.

static int g_nFocusAlpha;
static int g_nFocusScale;
static int g_nItemAlpha;
static int g_nItemScale;

static bool g_bEnableKey;		// 키 입력을 처리할 것인지에 대한 모드, FALSE-disable, TRUE-enable

/**
 * @brief Private Function Prototypes
 */
static int MsgCreate( WND wnd, int msg, int wparam, int lparam );
static int MsgDestroy( WND wnd, int msg, int wparam, int lparam );
static int MsgKeyDown( WND wnd, int key, int option );

static MGLvoid PrvDrawBackground( MGLvoid * pBuffer, MGLvoid * pParams );
static MGLvoid PrvInitRC( WND wnd );
static void PrvFocusAnimation( WND wnd );
static void PrvSlideAnimation( WND wnd );
static void PrvInitFocusAnimation( void );
static void PrvStartFocusAnimation( WND wnd );
static void PrvStopFocusAnimation( WND wnd );
static void PrvExplosionAnimation( WND wnd );
static void PrvGetMenuItemPos( int nIdx, int width, int height, int *x, int *y );
static void PrvInitItemAnimation( void );
static void PrvStartItemAnimation_2( WND wnd, int nStartImg );
static void PrvStartItemAnimation_1( WND wnd );
static void PrvItemAnimation_2( WND wnd );
static void PrvItemAnimation_1( WND wnd );


/**
 * @brief Menu 아이템의 중심좌표와 width, height 정보를 이용하여 위치 정보를 계산한다. 
 */
static void PrvGetMenuItemPos( int nIdx, int width, int height, int *x, int *y )
{
	*x = g_tMainMenu[nIdx].x - (width/2);
	*y = g_tMainMenu[nIdx].y - (height/2);

	return;
}

/** 
 * @brief 배경 이미지를 그린다. 
 */
static MGLvoid PrvDrawBackground(MGLvoid * pBuffer, MGLvoid * pParams)
{
	MGLbitmap bitmap;

	MGL_LoadBitmap( MGL_title_I, &bitmap );
	MGL_DrawBitmap( &bitmap, 0, 0 );

	MGL_LoadBitmap( MGL_back_I, &bitmap );
	MGL_DrawBitmap( &bitmap, 0, 66 );

	MGL_LoadBitmap( MGL_softkey_I, &bitmap );
	MGL_DrawBitmap( &bitmap, 0, 282 );

	// draw texts
	MGL_SetBgColor(MGL_GetDCTrColor());
	MGL_SetFgColor(MGL_MakeColor(255, 255, 255));
	GdiDrawTextOut((WND)pBuffer, 0, 20, 240, g_tMainMenu[g_nCurMenuIdx].szTitle, strlen(g_tMainMenu[g_nCurMenuIdx].szTitle), ALIGN_CENTER);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Option", StrLen("Option"), ALIGN_LEFT);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "SAMPLE4", StrLen("SAMPLE4"), ALIGN_CENTER);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Cancel", StrLen("Cancel"), ALIGN_RIGHT);

	return;
}

/** 
 * @brief Layer들을 초기화한다. RC와 Layer를 설정한다. 
 */
static MGLvoid PrvInitRC( WND wnd )
{
	MGLbitmap * pBitmap = NULL;

	g_nSlideCount = 1;
	g_nCurMenuIdx = 10;

	//MGL_SetRCs( 1, &g_tRC );

	MGL_SetDCRCs(1, &g_tRC); 
	MGL_SetDCCurRC( &g_tRC );

	MGL_SetRCBounds( 0, 0, LCD_WIDTH, LCD_HEIGHT );
	MGL_SetRCLayers( NUM_LAYERS, g_aLayers );

	// 배경 이미지 : Title, center, softkey 이미지 Drawing Layer
	MGL_SetCurLayerIndex( ID_LAYER_BACK );
	MGL_SetLayerBounds( 0, 0, LCD_WIDTH, LCD_HEIGHT );
	MGL_SetLayerVisible( TRUE );
	MGL_SetLayerType( MGL_CUSTOM_LAYER );
	MGL_SetLayerDrawFuncPtr( PrvDrawBackground );

	// 메뉴 슬라이딩 애니메이션 Layer
	MGL_SetCurLayerIndex( ID_LAYER_MENU_SLIDE1 );
	MGL_SetLayerBounds( 0, 50, LCD_WIDTH, 61 );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_menus_01_I );
	MGL_SetIP( &g_tMenuIP1, MGL_Alphablending, &g_tMenuABParams1 );
	MGL_SetLayerIPs( 1, &g_tMenuIP1);

	MGL_SetCurLayerIndex( ID_LAYER_MENU_SLIDE2 );
	MGL_SetLayerBounds( 0, 111, LCD_WIDTH, 53 );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_menus_02_I );
	MGL_SetIP( &g_tMenuIP2, MGL_Alphablending, &g_tMenuABParams2 );
	MGL_SetLayerIPs( 1, &g_tMenuIP2);

	MGL_SetCurLayerIndex( ID_LAYER_MENU_SLIDE3 );
	MGL_SetLayerBounds( 0, 164, LCD_WIDTH, 52 );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_menus_03_I );
	MGL_SetIP( &g_tMenuIP3, MGL_Alphablending, &g_tMenuABParams3 );
	MGL_SetLayerIPs( 1, &g_tMenuIP3);

	MGL_SetCurLayerIndex( ID_LAYER_MENU_SLIDE4 );
	MGL_SetLayerBounds( 0, 216, LCD_WIDTH, 50 );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_menus_04_I );
	MGL_SetIP( &g_tMenuIP4, MGL_Alphablending, &g_tMenuABParams4 );
	MGL_SetLayerIPs( 1, &g_tMenuIP4);

	// Selected menu item layer
	MGL_SetCurLayerIndex( ID_LAYER_MENU_FOCUS );
	MGL_SetLayerBounds( 0, 0, MENU_FOCUS_WIDTH, MENU_FOCUS_HEIGHT );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_focus_I );
	MGL_SetIP( &g_tFocusIP[0], MGL_Scaling, &g_tFocusScaleParams );
	MGL_SetIP( &g_tFocusIP[1], MGL_Alphablending, &g_tFocusABParams );
	MGL_SetLayerIPs( 2, g_tFocusIP);

	// Menu item layer
	MGL_SetCurLayerIndex( ID_LAYER_MENU_ITEM );
	MGL_SetLayerBounds( 0, 0, MENU_FOCUS_WIDTH, MENU_FOCUS_HEIGHT );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetIP( &g_tItemIP[0], MGL_Scaling, &g_tItemScaleParams );
	MGL_SetIP( &g_tItemIP[1], MGL_Alphablending, &g_tItemABParams );
	MGL_SetLayerIPs( 2, g_tItemIP);
	PrvInitItemAnimation();

	// Selected menu item layer
	MGL_SetCurLayerIndex( ID_LAYER_EXPLOSION );
	MGL_SetLayerBounds( 0, 0, MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_ex1_I );
	

	UsrSetTimer( wnd, TIMER_MGL_SLIDE_MENU, TIMER_VAL_SLIDE_MENU );
}

/** 
 * @brief Focus 애니메이션의 사용 값들을 초기화 한다. 
 */
static void PrvInitFocusAnimation( void )
{
	g_nFocusAlpha = 0;
	g_nFocusScale = 90;
	g_nFocusCount = 0;
}

/** 
 * @brief Focus 애니메이션을 시작한다.  
 */
static void PrvStartFocusAnimation( WND wnd )
{
	g_bEnableKey = TRUE;
	PrvInitFocusAnimation();
	UsrSetTimer( wnd, TIMER_MGL_MENU_FOCUS, TIMER_VAL_MENU_FOCUS );
}

/** 
 * @brief Focus 애니메이션을 멈춘다.
 */
static void PrvStopFocusAnimation( WND wnd )
{
//	PrvInitFocusAnimation();
	PrvInitItemAnimation();

	UsrKillTimer( wnd, TIMER_MGL_MENU_FOCUS );
	UsrKillTimer( wnd, TIMER_MGL_ITEM_ANI1 );
	UsrKillTimer( wnd, TIMER_MGL_ITEM_ANI2 );

	MGL_SetCurLayerIndex( ID_LAYER_MENU_FOCUS );
	MGL_SetLayerVisible( FALSE );
	MGL_SetCurLayerIndex( ID_LAYER_MENU_ITEM );
	MGL_SetLayerVisible( FALSE );
}

/** 
 * @brief Focus 애니메이션이 동작하는 동안 실행되는 함수.
 */
static void PrvFocusAnimation( WND wnd )
{
	int x, y, width, height;

	MGL_SetCurLayerIndex( ID_LAYER_MENU_FOCUS );
	MGL_SetLayerVisible( TRUE );

	g_nFocusCount++;
	if( g_nFocusCount >= 11 )
	{
//		PrvInitFocusAnimation();
		UsrKillTimer( wnd, TIMER_MGL_MENU_FOCUS );
	}
	else 
	{
		if( g_nFocusCount >=6 && g_nFocusCount < 11 )
		{
			g_nFocusScale -= 2;		//### 110% -> 100%
		}
		else
		{
			g_nFocusAlpha += 20;	//###  0% -> 100%
			g_nFocusScale += 4;		//### 90% -> 110%
		}
		//### 위치 변경 및 보정. 
		width = MENU_FOCUS_WIDTH*g_nFocusScale/100;
		height = MENU_FOCUS_HEIGHT*g_nFocusScale/100;
		PrvGetMenuItemPos( g_nCurMenuIdx, width, height, &x, &y );
		MGL_SetLayerBounds( x, y, width, height );
		MGL_SetScalingSameRatio( &g_tFocusIP[0], g_nFocusScale );
		MGL_SetAlphablendingAlpha( &g_tFocusIP[1], g_nFocusAlpha );
	}

	if( g_nFocusCount == 5)
		PrvStartItemAnimation_1( wnd );

	return;
}


/** 
 * @brief Slide 애니메이션 실행 부분.
 */
static void PrvSlideAnimation( WND wnd )
{
	int i;
	int nAlpha[8] = { 12, 26, 38, 50, 62, 76, 88, 100 };

	//### Layer menu 1의 alpha 값과 위치를 수정한다. 
	if( g_nSlideCount >=1 && g_nSlideCount <=8 )
	{
		MGL_SetCurLayerIndex( ID_LAYER_MENU_SLIDE1 );
		MGL_SetLayerVisible( TRUE );
		MGL_SetAlphablendingAlpha( &g_tMenuIP1, nAlpha[g_nSlideCount - 1] );
		MGL_MoveLayer( 0, 2 );
	}
	//### Layer menu 2의 alpha 값과 위치를 수정한다. 
	if( g_nSlideCount >=3 && g_nSlideCount <=10 )
	{
		MGL_SetCurLayerIndex( ID_LAYER_MENU_SLIDE2 );
		MGL_SetLayerVisible( TRUE );
		MGL_SetAlphablendingAlpha( &g_tMenuIP2, nAlpha[g_nSlideCount - 3] );
		MGL_MoveLayer( 0, 2 );
	}
	//### Layer menu 3의 alpha 값과 위치를 수정한다. 
	if( g_nSlideCount >=5 && g_nSlideCount <=12 )
	{
		MGL_SetCurLayerIndex( ID_LAYER_MENU_SLIDE3 );
		MGL_SetLayerVisible( TRUE );
		MGL_SetAlphablendingAlpha( &g_tMenuIP3, nAlpha[g_nSlideCount - 5] );
		MGL_MoveLayer( 0, 2 );
	}
	//### Layer menu 4의 alpha 값과 위치를 수정한다. 
	if( g_nSlideCount >=7 && g_nSlideCount <=14 )
	{
		MGL_SetCurLayerIndex( ID_LAYER_MENU_SLIDE4 );
		MGL_SetLayerVisible( TRUE );
		MGL_SetAlphablendingAlpha( &g_tMenuIP4, nAlpha[g_nSlideCount - 7] );
		MGL_MoveLayer( 0, 2 );
	}

	//### Alpha 값이 100 -> 60 으로 줄어든다. 
	if( g_nSlideCount >= 15 )
	{
		i = 100 - ( 5 * (g_nSlideCount - 15) );

		MGL_SetAlphablendingAlpha( &g_tMenuIP1, i );
		MGL_SetAlphablendingAlpha( &g_tMenuIP2, i );
		MGL_SetAlphablendingAlpha( &g_tMenuIP3, i );
		MGL_SetAlphablendingAlpha( &g_tMenuIP4, i );
	}
	if( g_nSlideCount > 22 )
	{
		g_nSlideCount = 0;
		UsrKillTimer( wnd, TIMER_MGL_SLIDE_MENU );
		// focus 애니메이션을 동작한다. 
		PrvStartFocusAnimation( wnd );
	}
	g_nSlideCount++;
	
	return;
}

/** 
 * @brief Explosion 효과 애니메이션을 시작한다.  
 */
static void PrvStartExplosionAnimation( WND wnd )
{
	g_nExplosionCount = 0;
	UsrSetTimer( wnd, TIMER_MGL_EXPLOSION_ANI, TIMER_VAL_EXPLOSION_ANI );
	return;
}

/** 
 * @brief Explosion 효과 애니메이션 실행 부분.
 */
static void PrvExplosionAnimation( WND wnd )
{
	int x, y;

	if( g_nExplosionCount < 4 )
	{
		MGL_SetCurLayerIndex( ID_LAYER_EXPLOSION );
		PrvGetMenuItemPos( g_nPrevMenuIdx, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, &x, &y );
		MGL_SetLayerBounds( x, y, EXPLOSION_WIDTH, EXPLOSION_HEIGHT );
		MGL_SetLayerVisible( TRUE );
		MGL_SetLayerBitmap( MGL_ex1_I + g_nExplosionCount );
		g_nExplosionCount++;
	}
	else
	{
		MGL_SetCurLayerIndex( ID_LAYER_EXPLOSION );
		MGL_SetLayerVisible( FALSE );
		UsrKillTimer( wnd, TIMER_MGL_EXPLOSION_ANI );
		PrvStartFocusAnimation( wnd );
	}
}

/** 
 * @brief Menu Item 애니메이션의 사용 값들을 초기화 한다. 
 */
static void PrvInitItemAnimation( void )
{
	g_nItemAlpha = 20;
	g_nItemScale = 150;
	g_nItemCount = 0;
}

/** 
 * @brief 첫번째 Menu Item 애니메이션 시작한다. 
 */
static void PrvStartItemAnimation_1( WND wnd )
{
	UsrSetTimer( wnd, TIMER_MGL_ITEM_ANI1,   TIMER_VAL_ITEM_ANI1 );
}

/** 
 * @brief 두번째 Menu Item 애니메이션 시작한다. 
 */
static void PrvStartItemAnimation_2( WND wnd, int nStartImg )
{
	g_nItemCount = 0;
	g_nItemImage = nStartImg;
	UsrSetTimer( wnd, TIMER_MGL_ITEM_ANI2, TIMER_VAL_ITEM_ANI2 );
}

/** 
 * @brief 두번째 Menu Item 애니메이션 실행 부분.
 */
static void PrvItemAnimation_2( WND wnd )
{
	if( g_nItemCount < 3 )
		g_nItemCount++;
	else 
		g_nItemCount = 0;

	MGL_SetCurLayerIndex( ID_LAYER_MENU_ITEM );
	MGL_SetLayerBitmap( g_nItemImage + g_nItemCount );

	return;	
}

/** 
 * @brief 첫번째 Menu Item 애니메이션 실행 부분.
 */
static void PrvItemAnimation_1( WND wnd )
{
	int x, y, width, height;
	int nImgIdx = MGL_icon_01_01_I + (g_nCurMenuIdx*4);

	MGL_SetCurLayerIndex( ID_LAYER_MENU_ITEM );
	MGL_SetLayerVisible( TRUE );

	if( g_nItemCount >= 12 )
	{
		UsrKillTimer( wnd, TIMER_MGL_ITEM_ANI1 );
		PrvStartItemAnimation_2( wnd, nImgIdx );
	}
	else 
	{
		if( g_nItemCount >=0 && g_nItemCount < 5 )
		{
			g_nItemAlpha += 16;	//###  20% -> 100% : 16(단위변화값) * 5 = 80(전체변화값)
			g_nItemScale -= 10;	//### 150% -> 100% : 10(단위변화값) * 5 = 50(전체변화값)
		}
		else if( g_nItemCount >= 5 && g_nItemCount < 8 )
		{
			g_nItemScale -= 4;		//### 100% -> 88% (실은 90%으로해야함.)
		}
		else
		{
			g_nItemScale += 4;		//### 88% -> 100%
		}

		//### 위치 변경 및 보정. 
		width = MENU_ITEM_WIDTH*g_nItemScale/100;
		height = MENU_ITEM_HEIGHT*g_nItemScale/100;
		PrvGetMenuItemPos( g_nCurMenuIdx, width, height, &x, &y );
		MGL_SetLayerBounds( x, y, width, height );
		MGL_SetScalingSameRatio( &g_tItemIP[0], g_nItemScale );
		MGL_SetAlphablendingAlpha( &g_tItemIP[1], g_nItemAlpha );
		MGL_SetLayerBitmap( nImgIdx );
		g_nItemCount++;
	}

	return;	
}

static int MsgCreate(WND wnd, int msg, int wparam, int lparam)
{
	// 키 입력을 받지 않게 한다. 
	g_bEnableKey = FALSE;
	
	PrvInitRC( wnd );
	MGL_Render();

	return 0;
}

static int MsgDestroy(WND wnd, int msg, int wparam, int lparam)
{

	UsrKillTimer( wnd, TIMER_MGL_SLIDE_MENU );
	UsrKillTimer( wnd, TIMER_MGL_MENU_FOCUS );
	UsrKillTimer( wnd, TIMER_MGL_ITEM_ANI1 );
	UsrKillTimer( wnd, TIMER_MGL_ITEM_ANI2 );
	UsrKillTimer( wnd, TIMER_MGL_EXPLOSION_ANI );

	return 0;
}

static int MsgKeyDown( WND wnd, int key, int option )
{
	switch( key )
	{
	case VK_USER_RIGHT:
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx == 11 )
			g_nCurMenuIdx = 0;
		else
			g_nCurMenuIdx++;
		PrvStartExplosionAnimation( wnd );
		break;

	case VK_USER_LEFT:
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		
		if( g_nCurMenuIdx == 0 )
			g_nCurMenuIdx = 11;
		else
			g_nCurMenuIdx--;
		PrvStartExplosionAnimation( wnd );
		break;

	case VK_USER_UP:
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx <= 2 )
			g_nCurMenuIdx += NUM_MENU_ITEM;
		g_nCurMenuIdx -=3;
		PrvStartExplosionAnimation( wnd );
		break;

	case VK_USER_DOWN:
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx >= 9 )
			g_nCurMenuIdx -= NUM_MENU_ITEM;
		g_nCurMenuIdx += 3;
		PrvStartExplosionAnimation( wnd );
		break;
	
	case VK_USER_OK:
		StaGotoState( wnd, STATE_ID_MGL_DEMO_IDLE, MSG_DESTROY, MSG_RETURN );		
		break;
	}

	return 0;
}

static int MsgTimer(WND wnd, int msg, int id, int lparam)
{
	if( id == TIMER_MGL_MENU_FOCUS )
	{
		PrvFocusAnimation( wnd );
		MGL_Render();
	}
	else if( id == TIMER_MGL_SLIDE_MENU )
	{
		PrvSlideAnimation( wnd );
		MGL_Render();
	}
	else if( id == TIMER_MGL_EXPLOSION_ANI )
	{
		PrvExplosionAnimation( wnd );
		MGL_Render();
	}
	else if( id == TIMER_MGL_ITEM_ANI1 )
	{
		PrvItemAnimation_1( wnd );
		MGL_Render();
	}
	else if( id == TIMER_MGL_ITEM_ANI2 )
	{
		PrvItemAnimation_2( wnd );
		MGL_Render();
	}
	return 0;
}

int MGLSample4Proc(WND wnd, int msg, int wparam, int lparam)
{
	int handled = FALSE;

	switch (msg)
	{
	case AWM_CREATE:
	case AWM_LOAD:
		MsgCreate(wnd, msg, wparam, lparam);
		handled = TRUE;
		break;

	case AWM_KEYDOWN:
		if( g_bEnableKey )
		{
			MsgKeyDown( wnd, wparam, lparam );
			handled = TRUE;
		}
		break;

	case AWM_DESTROY:
	case AWM_SAVE:
		MsgDestroy(wnd, msg, wparam, lparam);
		handled = TRUE;
		break;

	case AWM_TIMER:
		MsgTimer(wnd, msg, wparam, lparam);
		handled = TRUE;
		break;
	}

	return 0;
}