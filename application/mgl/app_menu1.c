/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @brief ...
 *
 * @file app_menu1.c
 * @author <a href="mailto:hrbae@mococo.com">HyangRan Bae</a>
 * @todo comments here
 */
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
 *			0x05 - 0x17 : Menu item layer
 */
#define NUM_LAYERS				19
#define TIMER_MGL_APP			0x7771

#ifdef REXY_EMULATOR
	#define TIMER_VAL				100
	#define TIMER_VAL_SLIDE_MENU	100				//### 0.02초
	#define TIMER_VAL_MENU_FOCUS	100
	#define TIMER_VAL_ITEM_ANI		100				//200
	#define TIMER_VAL_MBLUR_ANI		10				//### 0.4 s
#else
	#define TIMER_VAL				1
	#define TIMER_VAL_SLIDE_MENU	1				
	#define TIMER_VAL_MENU_FOCUS	1
	#define TIMER_VAL_ITEM_ANI		1				
	#define TIMER_VAL_MBLUR_ANI		1
#endif		// #ifdef REXY_EMULATOR

#define TIMER_MGL_SLIDE_MENU_MASK	(1 << 0)
#define TIMER_MGL_MENU_FOCUS_MASK	(1 << 1)
#define TIMER_MGL_ITEM_ANI_MASK		(1 << 2)
#define TIMER_MGL_MBLUR_ANI_MASK	(1 << 3)

static MGLuint timermask;

#define ID_LAYER_BACK			0				// Back Layer ID
#define ID_LAYER_MENU_SLIDE1	1				// Menu Slide Layer ID
#define ID_LAYER_MENU_SLIDE2	2				// ...
#define ID_LAYER_MENU_SLIDE3	3				// ...
#define ID_LAYER_MENU_SLIDE4	4				// ...

#define ID_LAYER_MENU_MBLUR		5				// Motionblur layer(right bottom of "ID_LAYER_MENU_FOCUS")
#define ID_LAYER_MENU_FOCUS		6				// Selected Menu Item Layer ID
#define ID_LAYER_MENU_R1C1		7				// Menu Item Layer ID
#define ID_LAYER_MENU_R1C2		8				// ...
#define ID_LAYER_MENU_R1C3		9				// ...
#define ID_LAYER_MENU_R2C1		10				// ...
#define ID_LAYER_MENU_R2C2		11				// ...
#define ID_LAYER_MENU_R2C3		12				// ...
#define ID_LAYER_MENU_R3C1		13				// ...
#define ID_LAYER_MENU_R3C2		14				// ...
#define ID_LAYER_MENU_R3C3		15				// ...
#define ID_LAYER_MENU_R4C1		16				// ...
#define ID_LAYER_MENU_R4C2		17				// ...
#define ID_LAYER_MENU_R4C3		18				// ...


#define NUM_MENU_ITEM			12					// 메뉴 아이템의 갯수 
#define MENU_ICON_WIDTH			72					// 메뉴 아이템의 가로 길이
#define MENU_ICON_HEIGHT		70					// 메뉴 아이템의 세로 길이 

#define MENU_FOCUS_WIDTH		82					// 선택된 메뉴 아이템의 가로 길이.
#define MENU_FOCUS_HEIGHT		82					// 선택된 메뉴 아이템의 세로 길이.

#define NUM_MENU_ITEM_IMAGES	 10
#define NUM_MENU_ITEM_SEQS		 2

#define MBLUR_KEY_LEFT				0
#define MBLUR_KEY_RIGHT				1
#define MBLUR_KEY_UP				2
#define MBLUR_KEY_DOWN				3

#define MBLUR_MOVEMENT_DISTANCE		20

typedef struct MGL_MENU {
	int x;				//< icon의 중심 X좌표
	int y;				//< icon의 중심 Y좌표

	MGLlayer * pLayer;	//< 메뉴 아이템에 연결된 LAYER
	char szTitle[20];	//< 메뉴 아이템의 타이틀
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

static MGLbluralphablendedparams g_tFocusMBlurAlphaParams;	// Focus Item MotionBlur 정보 구조체이다.
static MGLip					 g_tFocusMBlurAlphaIP[2];		// Focus Motionblur 

static MGLanimator				g_tItemAnimator[NUM_MENU_ITEM];
static MGLsubsystem				g_tItemSubsystem[NUM_MENU_ITEM];
static MGLimgselector			g_tItemImageSelector[NUM_MENU_ITEM];
static MGLimgselitem			g_tItemImages[NUM_MENU_ITEM][NUM_MENU_ITEM_IMAGES];
static MGLimgselseqinfo			g_tSeqInfos[NUM_MENU_ITEM][NUM_MENU_ITEM_SEQS];


static MGL_MENU		g_tMainMenu[NUM_MENU_ITEM] = {
		{  41, 101, &g_aLayers[ID_LAYER_MENU_R1C1], "To DO" },
		{ 120, 101, &g_aLayers[ID_LAYER_MENU_R1C2], "Contacts" },
		{ 199, 101, &g_aLayers[ID_LAYER_MENU_R1C3], "Multimedia" },

		{  41, 146, &g_aLayers[ID_LAYER_MENU_R2C1], "Internet" },
		{ 120, 146, &g_aLayers[ID_LAYER_MENU_R2C2], "Mailbox" },
		{ 199, 146, &g_aLayers[ID_LAYER_MENU_R2C3], "Download" },
	
		{  41, 201, &g_aLayers[ID_LAYER_MENU_R3C1], "Schedule" },
		{ 120, 201, &g_aLayers[ID_LAYER_MENU_R3C2], "Camera" },
		{ 199, 201, &g_aLayers[ID_LAYER_MENU_R3C3], "Settings" },

		{  41, 247, &g_aLayers[ID_LAYER_MENU_R4C1], "PDA" },
		{ 120, 247, &g_aLayers[ID_LAYER_MENU_R4C2], "Time" },
		{ 199, 247, &g_aLayers[ID_LAYER_MENU_R4C3], "Alarm" }
};

static int g_nCurMenuIdx = 0;	// 현재 메뉴 Index
static int g_nPrevMenuIdx = 0;	// 이전 메뉴 Index

static int g_nSlideCount;		// Slide menu Animation.
static int g_nFocusCount;		// Focus Animation
static int g_nFocusMBlurCount;	// Focus Motionblur Animation
 
static int g_nFocusAlpha;
static int g_nFocusScale;
static int g_nMBlurAlpha;
static int g_nMBlurScale;
static int g_nDirection;
static bool g_bMBlurDone;		// True - MotionBlur is done absolutely.(not interrupted)

/**
 * @brief Private Function Prototypes
 */
static int MsgCreate( WND wnd, int msg, int wparam, int lparam );
static int MsgDestroy( WND wnd, int msg, int wparam, int lparam );
static int MsgKeyDown( WND wnd, int key, int option );

static MGLvoid PrvDrawBackground( MGLvoid * pBuffer, MGLvoid * pParams );
static MGLvoid PrvInitRC( WND wnd );
static void PrvFocusAnimation( WND wnd );
static void PrvFocusMBlurAnimation( WND wnd );
static void PrvSlideAnimation( WND wnd );
static void PrvInitFocusAnimation( void );
static void PrvInitFocusMBlurAnimation( void );
static void PrvStartFocusAnimation( WND wnd );
static void PrvStartFocusMBlurAnimation( WND wnd );
static void PrvStopFocusAnimation( WND wnd );
static void PrvStopFocusMBlurAnimation( WND wnd );

/** 
 * @brief 배경 이미지를 그린다. 
 */
static MGLvoid PrvDrawBackground( MGLvoid * pBuffer, MGLvoid * pParams )
{
	MGLbitmap bitmap;

	MGL_LoadBitmap( MGL_title_I, &bitmap );
	MGL_DrawBitmap( &bitmap, 0, 0 );

	MGL_LoadBitmap( MGL_back_I, &bitmap );
	MGL_DrawBitmap( &bitmap, 0, 66 );

	MGL_LoadBitmap( MGL_softkey_I, &bitmap );
	MGL_DrawBitmap( &bitmap, 0, 282 );

	// Draw texts
	MGL_SetBgColor(MGL_GetDCTrColor());
	MGL_SetFgColor(MGL_MakeColor(255, 255, 255));
	GdiDrawTextOut((WND)pBuffer, 0, 20, 240, g_tMainMenu[g_nCurMenuIdx].szTitle, strlen(g_tMainMenu[g_nCurMenuIdx].szTitle), ALIGN_CENTER);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Option", StrLen("Option"), ALIGN_LEFT);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Cancel", StrLen("Cancel"), ALIGN_RIGHT);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Sample1", StrLen("Sample1"), ALIGN_CENTER);

	return;
}

/** 
 * @brief Layer들을 초기화한다. RC와 Layer를 설정한다. 
 */
static MGLvoid PrvInitRC( WND wnd )
{
	int nIdx, nCount, nX, nY;
	int nImageIdx;
	MGLbitmap * pBitmap = NULL;

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

/****************************************************/
	// Motionblur(Alphablended) layer
	MGL_SetCurLayerIndex( ID_LAYER_MENU_MBLUR );
	MGL_SetLayerBounds( 0, 0, MENU_FOCUS_WIDTH, MENU_FOCUS_HEIGHT );	// TODO - check size
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_focus_I );
	MGL_SetIP(&g_tFocusMBlurAlphaIP[0], MGL_MotionblurAlpha, &g_tFocusMBlurAlphaParams);
	MGL_SetLayerIPs( 1, g_tFocusMBlurAlphaIP);	
	PrvInitFocusMBlurAnimation();
	
	// Selected menu item layer
	MGL_SetCurLayerIndex( ID_LAYER_MENU_FOCUS );
	MGL_SetLayerBounds( 0, 0, MENU_FOCUS_WIDTH, MENU_FOCUS_HEIGHT );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_focus_I );
	MGL_SetIP( &g_tFocusIP[0], MGL_Scaling, &g_tFocusScaleParams );
	MGL_SetIP( &g_tFocusIP[1], MGL_Alphablending, &g_tFocusABParams );
	MGL_SetLayerIPs( 2, g_tFocusIP);
	PrvInitFocusAnimation();
/****************************************************/


	// 메뉴 아이템 Layer
	nImageIdx = MGL_icon_01_01_I;
	for( nIdx=ID_LAYER_MENU_R1C1, nCount=0; nIdx <= ID_LAYER_MENU_R4C3; nIdx++, nCount++ )
	{
		nX = g_tMainMenu[nCount].x - (MENU_ICON_WIDTH/2);
		nY = g_tMainMenu[nCount].y - (MENU_ICON_HEIGHT/2);
		MGL_SetCurLayerIndex( nIdx );
		MGL_SetLayerBounds( nX, nY, MENU_ICON_WIDTH, MENU_ICON_HEIGHT );
		MGL_SetLayerVisible( FALSE );
		MGL_SetLayerType( MGL_EXTERNAL_IMAGE_LAYER );

		// @todo External image layer 설정을 해줘야한다.
		/* Setting Animator*/
		MGL_SetAnimator (&g_tItemAnimator[nCount], &g_tItemSubsystem[nCount], 1);
		MGL_AnimatorSetTiming (&g_tItemAnimator[nCount], MGL_TIMING_TYPE_FRAME, 0);
		MGL_AnimatorSetRepeatCount (&g_tItemAnimator[nCount], 0);

		/* Setting ImageSelector */
		MGL_SetImageSelector (&g_tItemImageSelector[nCount], g_tItemImages[nCount], 
				NULL, NUM_MENU_ITEM_IMAGES, g_tSeqInfos[nCount], NUM_MENU_ITEM_SEQS,
				MGL_IMGSEL_KEEP_ORDER | MGL_IMGSEL_SELECT_SEQUENCE );

		MGL_ImageSelectorSetSequenceInfo (&g_tItemImageSelector[nCount], 0, 4, 0);
		MGL_ImageSelectorSetSequenceInfo (&g_tItemImageSelector[nCount], 3, 7, 1);

		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx,   0);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx+1, 1);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx+2, 2);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx+3, 3);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx+3, 4);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx+3, 5);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx+2, 6);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx+1, 7);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx,   8);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx,   9);
		nImageIdx += 4;

		/* Linking ImageSelector & Animator */
		MGL_ImageSelectorSetInSubsystem (&g_tItemImageSelector[nCount], g_tItemAnimator[nCount].pSubsystems);

		/* Linking Animator & External Layer */
		MGL_AnimatorSetExternalImageLayer (&g_tItemAnimator[nCount], 1);

		MGL_AnimatorClear (&g_tItemAnimator[nCount]);
	}

	g_nSlideCount = 1;
	g_nCurMenuIdx = 10;

	//UsrSetTimer( wnd, TIMER_MGL_SLIDE_MENU, TIMER_VAL_SLIDE_MENU );
	timermask |= TIMER_MGL_SLIDE_MENU_MASK;
}


/** 
 * @brief Focus 애니메이션의 사용 값들을 초기화 한다. 
 */
static void PrvInitFocusAnimation( void )
{
	g_nFocusAlpha = 100;	// 100% (Original img)
	g_nFocusScale = 100;	// 100%
	g_nFocusCount = 0;
}

static void PrvInitFocusMBlurAnimation( void )
{
	g_nMBlurAlpha = 0;		// 0%	(absolutely Transparent)
	g_nMBlurScale = 100;	// 100%
	g_nFocusMBlurCount = 0;
//	g_bMBlurDone = 0;
//	g_nDirection = 0;
}

static void PrvStartFocusAnimation( WND wnd )
{
	timermask |= TIMER_MGL_MENU_FOCUS_MASK;
	timermask |= TIMER_MGL_ITEM_ANI_MASK;
}

static void PrvStartFocusMBlurAnimation( WND wnd )
{
	if(g_bMBlurDone == 0)		// Interrupted in Motionblur ani
	{
		timermask &= ~TIMER_MGL_MBLUR_ANI_MASK;		// for Key Repeat event
		PrvStopFocusMBlurAnimation(wnd);
	}
	timermask |= TIMER_MGL_MBLUR_ANI_MASK;	//UsrSetTimer( wnd, TIMER_MGL_MBLUR_ANI, TIMER_VAL_MBLUR_ANI);

	MGL_SetCurLayerIndex( ID_LAYER_MENU_FOCUS );
	MGL_SetLayerVisible( FALSE );
}

/** 
 * @brief Focus 애니메이션을 멈춘다.
 */
static void PrvStopFocusAnimation( WND wnd )
{
	PrvInitFocusAnimation();
	timermask &= ~TIMER_MGL_MENU_FOCUS_MASK;	//UsrKillTimer( wnd, TIMER_MGL_MENU_FOCUS );
	timermask &= ~TIMER_MGL_ITEM_ANI_MASK;

	MGL_SetCurLayerIndex( ID_LAYER_MENU_R1C1 + g_nPrevMenuIdx );
	MGL_SetLayerVisible( FALSE );	//### 이전 layer 안보이게 하기.
}

static void PrvStopFocusMBlurAnimation( WND wnd )
{
	PrvInitFocusMBlurAnimation();
}

/****************** START ********************/
/** 
 * @brief Focus 애니메이션 실행 함수 
 */
static void PrvFocusAnimation( WND wnd )
{
	int x, y, width, height;

	MGL_SetCurLayerIndex( ID_LAYER_MENU_FOCUS );
	MGL_SetLayerVisible( TRUE );

	g_nFocusCount++;
	if( g_nFocusCount >= 11 )
	{
		PrvInitFocusAnimation();
		timermask &= ~TIMER_MGL_MENU_FOCUS_MASK;
	}
	else 
	{
		width = MENU_FOCUS_WIDTH;
		height = MENU_FOCUS_HEIGHT;
		x = g_tMainMenu[g_nCurMenuIdx].x - width/2;
		y = g_tMainMenu[g_nCurMenuIdx].y - height/2;
		MGL_SetLayerBounds( x, y, width, height );
		MGL_SetScalingSameRatio( &g_tFocusIP[0], g_nFocusScale );
		MGL_SetAlphablendingAlpha( &g_tFocusIP[1], g_nFocusAlpha );
	}

	return;
}

static void PrvFocusMBlurAnimation( WND wnd )
{
	int x, y, width, height;

	MGL_SetCurLayerIndex( ID_LAYER_MENU_MBLUR );
	MGL_SetLayerVisible( TRUE );

	g_nFocusMBlurCount++;
	if( g_nFocusMBlurCount >=  3 )	// Last scene
	{
		g_bMBlurDone = 1;
		PrvInitFocusMBlurAnimation();
		timermask &= ~TIMER_MGL_MBLUR_ANI_MASK;

		MGL_SetCurLayerIndex( ID_LAYER_MENU_FOCUS );
		MGL_SetLayerVisible( TRUE );

		width = MENU_FOCUS_WIDTH;
		height = MENU_FOCUS_HEIGHT;
		x = g_tMainMenu[g_nCurMenuIdx].x - width/2;
		y = g_tMainMenu[g_nCurMenuIdx].y - height/2;
		MGL_SetLayerBounds( x, y, width, height );
	}
	else
	{
		g_bMBlurDone = 0;
		width = MENU_FOCUS_WIDTH;
		height = MENU_FOCUS_HEIGHT;

		// Alpha
		g_nMBlurAlpha = 40*(3-g_nFocusMBlurCount);
		MGL_SetMotionblurAlphaOpacity(&g_tFocusMBlurAlphaIP[0], g_nMBlurAlpha);

		MGL_SetMotionblurAlphaObjectX(&g_tFocusMBlurAlphaIP[0], 0);
		MGL_SetMotionblurAlphaObjectY(&g_tFocusMBlurAlphaIP[0], 0);

		// Movement ( Vertical or Horizontal)		
		switch(g_nDirection)
		{
		case MBLUR_KEY_LEFT:
			x = g_tMainMenu[g_nCurMenuIdx].x - (MBLUR_MOVEMENT_DISTANCE*g_nFocusMBlurCount);
			y = g_tMainMenu[g_nCurMenuIdx].y - height/2;

			MGL_SetMotionblurAlphaMatrixType(&g_tFocusMBlurAlphaIP[0], MGL_BLUR_MOTION_M0_09);
			break;

		case MBLUR_KEY_RIGHT:
			x = g_tMainMenu[g_nCurMenuIdx].x + (MBLUR_MOVEMENT_DISTANCE*g_nFocusMBlurCount) - width;
			y = g_tMainMenu[g_nCurMenuIdx].y - height/2;

			MGL_SetMotionblurAlphaMatrixType(&g_tFocusMBlurAlphaIP[0], MGL_BLUR_MOTION_M0_09);
			break;

		case MBLUR_KEY_UP:
			x = g_tMainMenu[g_nCurMenuIdx].x - width/2;
			y = g_tMainMenu[g_nCurMenuIdx].y - (MBLUR_MOVEMENT_DISTANCE*g_nFocusMBlurCount);
			
			MGL_SetMotionblurAlphaMatrixType(&g_tFocusMBlurAlphaIP[0], MGL_BLUR_MOTION_M90_09);
			break;

		case MBLUR_KEY_DOWN:
			x = g_tMainMenu[g_nCurMenuIdx].x - width/2;
			y = g_tMainMenu[g_nCurMenuIdx].y + (MBLUR_MOVEMENT_DISTANCE*g_nFocusMBlurCount) - height;

			MGL_SetMotionblurAlphaMatrixType(&g_tFocusMBlurAlphaIP[0], MGL_BLUR_MOTION_M90_09);
			break;
		}

		MGL_SetLayerX(x);
		MGL_SetLayerY(y);
	}

	return;
}
/******************  END  ********************/


/** 
 * @brief Slide 애니메이션 실행 부분.
 */
static void PrvSlideAnimation( WND wnd )
{
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

	//### 현재 선택된 행이 나타날때 focus 애니메이션 동작한다. 
	//###   g_nSlideCount/2 -> 현재 행이 나옴.
	if( (g_nSlideCount/2) == (g_nCurMenuIdx/3) )
	{
		PrvStartFocusAnimation( wnd );
		g_bMBlurDone = 1;
	}

	if( g_nSlideCount > 11 )
	{
		g_nSlideCount = 0;
		timermask &= ~TIMER_MGL_SLIDE_MENU_MASK;
	}
	g_nSlideCount++;
	
	return;
}

static int MsgCreate(WND wnd, int msg, int wparam, int lparam)
{
	PrvInitRC( wnd );
	MGL_Render();
	UsrSetTimer (wnd, TIMER_MGL_APP, TIMER_VAL);

	return 0;
}

static int MsgDestroy(WND wnd, int msg, int wparam, int lparam)
{
	UsrKillTimer (wnd, TIMER_MGL_APP);
	return 0;
}

static int MsgKeyDown( WND wnd, int key, int option )
{
	switch( key )
	{
	case VK_USER_RIGHT:
		g_nDirection = MBLUR_KEY_RIGHT;
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx == 11 )
			g_nCurMenuIdx = 0;
		else
			g_nCurMenuIdx++;
		PrvStartFocusMBlurAnimation( wnd );
		PrvStopFocusMBlurAnimation( wnd );
		timermask |= TIMER_MGL_ITEM_ANI_MASK;
		break;

	case VK_USER_LEFT:
		g_nDirection = MBLUR_KEY_LEFT;
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx == 0 )
			g_nCurMenuIdx = 11;
		else
			g_nCurMenuIdx--;
		PrvStartFocusMBlurAnimation( wnd );
		PrvStopFocusMBlurAnimation( wnd );
		timermask |= TIMER_MGL_ITEM_ANI_MASK;
		break;

	case VK_USER_UP:
		g_nDirection = MBLUR_KEY_UP;
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx <= 2 )
			g_nCurMenuIdx += NUM_MENU_ITEM;
		g_nCurMenuIdx -=3;
		PrvStartFocusMBlurAnimation( wnd );
		PrvStopFocusMBlurAnimation( wnd );
		timermask |= TIMER_MGL_ITEM_ANI_MASK;
		break;

	case VK_USER_DOWN:
		g_nDirection = MBLUR_KEY_DOWN;
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx >= 9 )
			g_nCurMenuIdx -= NUM_MENU_ITEM;
		g_nCurMenuIdx += 3;
		PrvStartFocusMBlurAnimation( wnd );
		PrvStopFocusMBlurAnimation( wnd );
		timermask |= TIMER_MGL_ITEM_ANI_MASK;
		break;
	
	case VK_USER_OK:
		StaGotoState( wnd, STATE_ID_MGL_DEMO_IDLE, MSG_DESTROY, MSG_RETURN );
		break;
	}

	return 0;
}

static int MsgTimer(WND wnd, int msg, int id, int lparam)
{
	static int itemani;

	//CONSOL_Printf ("MsgTimer id [%x] \n", id);
	if (id == TIMER_MGL_APP) {
		if (timermask & TIMER_MGL_MENU_FOCUS_MASK) {
			//CONSOL_Printf ("Menu Focus [%d] [%d] \n", menufocus, TIMER_VAL_MENU_FOCUS / TIMER_VAL);
			//if (menufocus >= TIMER_VAL_MENU_FOCUS / TIMER_VAL) {
				PrvFocusAnimation (wnd);
			//	menufocus = 0;
			//}
			//menufocus++;

		} 
		
		if (timermask & TIMER_MGL_SLIDE_MENU_MASK) {
			//CONSOL_Printf ("Slide Menu [%d] [%d] \n", slidemenu, TIMER_VAL_SLIDE_MENU / TIMER_VAL);
			//if (slidemenu >= TIMER_VAL_SLIDE_MENU / TIMER_VAL) {
				PrvSlideAnimation (wnd);
			//	slidemenu = 0;
			//}
			//slidemenu++;
		} 
		
		if (timermask & TIMER_MGL_ITEM_ANI_MASK) {
			//CONSOL_Printf ("Item Ani [%d] [%d] \n", itemani, TIMER_VAL_ITEM_ANI / TIMER_VAL);
			//if (itemani >= TIMER_VAL_ITEM_ANI / TIMER_VAL) {
			if (itemani >= 5) {
				if (g_nPrevMenuIdx != g_nCurMenuIdx) {
					MGL_SetCurLayerIndex (ID_LAYER_MENU_R1C1 + g_nPrevMenuIdx);
					MGL_SetLayerVisible (FALSE);	// ### 이전 layer 안보이게 하기.
					g_nPrevMenuIdx = g_nCurMenuIdx;
				}

				MGL_SetCurLayerIndex (ID_LAYER_MENU_R1C1 + g_nCurMenuIdx);
				MGL_SetLayerVisible (TRUE);
				MGL_AnimatorUpdateAll (&g_tItemAnimator[g_nCurMenuIdx]);

				itemani = 0;
			}
			itemani++;
			
		} 
		
		if (timermask & TIMER_MGL_MBLUR_ANI_MASK) {	// Mblur
			//CONSOL_Printf ("Mblur Ani [%d] [%d] \n", mblurani, TIMER_VAL_MBLUR_ANI / TIMER_VAL);
			//if (mblurani >= TIMER_VAL_MBLUR_ANI / TIMER_VAL) {
				PrvFocusMBlurAnimation (wnd);
			//	mblurani = 0;
			//}
			//mblurani++;
		}
		MGL_Render();

	}

	return 0;
}

int MGLSample1Proc(WND wnd, int msg, int wparam, int lparam)
{
	int handled=FALSE;

	switch (msg)
	{
	case AWM_CREATE:
	case AWM_LOAD:
		MsgCreate(wnd, msg, wparam, lparam);
		handled = TRUE;
		break;

	case AWM_KEYDOWN:
		MsgKeyDown( wnd, wparam, lparam );
		handled = TRUE;
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

	return handled;
}

