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


////////////// Base Application Definition

#define NUM_LAYERS				20		// Added by 2 Layer by jibong
#define TIMER_MGL_SLIDE_MENU	0x0001
#define TIMER_VAL_SLIDE_MENU	1					//### 0.02초
#define TIMER_MGL_MENU_FOCUS	0x0002
#define TIMER_VAL_MENU_FOCUS	1
#define TIMER_MGL_ITEM_ANI		0x0003
#define TIMER_VAL_ITEM_ANI		1

//#define ID_SAMPLE_MAIN			ID_MGL_SAMPLE2		//### Menu Slide 애니메이션이 끝난뒤 이동할 State ID

#define ID_LAYER_BACK			0							// Back Layer ID
#define ID_LAYER_MENU_SLIDE1	ID_LAYER_BACK + 1			// Menu Slide Layer ID
#define ID_LAYER_MENU_SLIDE2	ID_LAYER_MENU_SLIDE1 + 1	// ...
#define ID_LAYER_MENU_SLIDE3	ID_LAYER_MENU_SLIDE2 + 1	// ...
#define ID_LAYER_MENU_SLIDE4	ID_LAYER_MENU_SLIDE3 + 1	// ...
#define ID_LAYER_FOCUSEFFECT_1	ID_LAYER_MENU_SLIDE4 + 1	// FocusEffect Layer
#define ID_LAYER_FOCUSEFFECT_2	ID_LAYER_FOCUSEFFECT_1 + 1	// FocusEffect Layer
#define ID_LAYER_MENU_FOCUS		ID_LAYER_FOCUSEFFECT_2 + 1	// Selected Menu Item Layer ID
#define ID_LAYER_MENU_R1C1		ID_LAYER_MENU_FOCUS + 1		// Menu Item Layer ID
#define ID_LAYER_MENU_R1C2		ID_LAYER_MENU_R1C1 + 1		// ...
#define ID_LAYER_MENU_R1C3		ID_LAYER_MENU_R1C2 + 1		// ...
#define ID_LAYER_MENU_R2C1		ID_LAYER_MENU_R1C3 + 1		// ...
#define ID_LAYER_MENU_R2C2		ID_LAYER_MENU_R2C1 + 1		// ...
#define ID_LAYER_MENU_R2C3		ID_LAYER_MENU_R2C2 + 1		// ...
#define ID_LAYER_MENU_R3C1		ID_LAYER_MENU_R2C3 + 1		// ...
#define ID_LAYER_MENU_R3C2		ID_LAYER_MENU_R3C1 + 1		// ...
#define ID_LAYER_MENU_R3C3		ID_LAYER_MENU_R3C2 + 1		// ...
#define ID_LAYER_MENU_R4C1		ID_LAYER_MENU_R3C3 + 1		// ...
#define ID_LAYER_MENU_R4C2		ID_LAYER_MENU_R4C1 + 1		// ...
#define ID_LAYER_MENU_R4C3		ID_LAYER_MENU_R4C2 + 1		// ...


#define NUM_MENU_ITEM			12					// 메뉴 아이템의 갯수 
#define MENU_ICON_WIDTH			72					// 메뉴 아이템의 가로 길이
#define MENU_ICON_HEIGHT		70					// 메뉴 아이템의 세로 길이 

#define MENU_FOCUS_WIDTH		82					// 선택된 메뉴 아이템의 가로 길이.
#define MENU_FOCUS_HEIGHT		82					// 선택된 메뉴 아이템의 세로 길이.

#define NUM_MENU_ITEM_IMAGES	 10
#define NUM_MENU_ITEM_SEQS		 2

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

static int g_nSlideCount;		// Slide menu 애니메이션.
static int g_nFocusCount;		// Focus 애니메이션 
static int g_nFocusAlpha;
static int g_nFocusScale;


///////////// Focus Effect Definition

//#define TEST_REXY_COMPARISON
#define ADD_FOCUSEFFECTLAYER_IP

#define NUM_FOCUSEFFECT_LAYERS	2
#define NUM_FOCUSEFFECT_IP		2
//#define NUM_FOCUSEFFECT_FRAME	9
#define NUM_FOCUSEFFECT_FRAME	12

#define TIMER_ID_FOCUSEFFECT1	0x0004
#define TIMER_ID_FOCUSEFFECT2	0x0005
#define TIMER_GAP_FOCUSEFFECT	600//300

#define MENU_FOCUSBG_WIDTH		98					// 선택된 메뉴 아이템의 가로 길이.
#define MENU_FOCUSBG_HEIGHT		98					// 선택된 메뉴 아이템의 세로 길이.


//////////// Focus Effect Variables 
static int						g_nFocusEffectIdx1;
static int						g_nFocusEffectIdx2;

static MGLip					g_tFocusEffectIP1[NUM_FOCUSEFFECT_IP];
static MGLip					g_tFocusEffectIP2[NUM_FOCUSEFFECT_IP];

static MGLalphablendingparams	g_tFocusEffectABPrm1;
static MGLalphablendingparams	g_tFocusEffectABPrm2;

static MGLscalingparams			g_tFocusEffectScalePrm1;
static MGLscalingparams			g_tFocusEffectScalePrm2;

typedef struct tFocusEffect{
	int nTimerDelay;
	int nAlpha;
	int nScale;
}tFocusEffect;

static tFocusEffect	g_nFocusEffectA[NUM_FOCUSEFFECT_FRAME] = 
{

	{  120,  20, 101 },
	{  120,  30, 102 },
	{  120,  35, 105 },
	{  120,  45, 110 },

	{  120,  50, 112 },	
	{  120,  30, 114 },	
	{  120,  25, 116 },
	{  120,  15, 118 },
	{  120,   5, 120 },
	{  120,   3, 120 },
	
	{  100,  0, 100 },	
	{  1000,  0, 100 }	// Delay

};

/**
 * @brief Private Function Prototypes
 */
static int MsgCreate( WND wnd, int msg, int wparam, int lparam );
static int MsgDestroy( WND wnd, int msg, int wparam, int lparam );
static int MsgKeyDown( WND wnd, int key, int option );

static MGLvoid PrvDrawBackground( MGLvoid * pBuffer, MGLvoid * pParams);
static MGLvoid PrvInitRC( WND wnd );
static void PrvFocusAnimation( WND wnd );
static void PrvSlideAnimation( WND wnd );
static void PrvInitFocusAnimation( void );
static void PrvStartFocusAnimation( WND wnd );
static void PrvStopFocusAnimation( WND wnd );

/////////////////  Focus Effect Function

static MGLvoid PrvInitFocusEffectLayerPos( void )
{
	MGL_SetCurLayerIndex( ID_LAYER_FOCUSEFFECT_1 );
	MGL_SetLayerX( g_tMainMenu[g_nCurMenuIdx].x - (MENU_FOCUSBG_WIDTH/2));
	MGL_SetLayerY( g_tMainMenu[g_nCurMenuIdx].y - (MENU_FOCUSBG_HEIGHT/2));	

	MGL_SetCurLayerIndex( ID_LAYER_FOCUSEFFECT_2 );
	MGL_SetLayerX( g_tMainMenu[g_nCurMenuIdx].x - (MENU_FOCUSBG_WIDTH/2));
	MGL_SetLayerY( g_tMainMenu[g_nCurMenuIdx].y - (MENU_FOCUSBG_HEIGHT/2));	
}

static MGLvoid PrvInitFocusEffectLayerIP( void )
{
	MGL_SetScalingSameRatio( &g_tFocusEffectIP1[0], 100 );	// Ratio 100
	MGL_SetAlphablendingAlpha( &g_tFocusEffectIP1[1], 0 );	// Alpha 0

	MGL_SetScalingSameRatio( &g_tFocusEffectIP2[0], 100 );	// Ratio 100
	MGL_SetAlphablendingAlpha( &g_tFocusEffectIP2[1], 0 );	// Alpha 0
}

/** 
 * @brief Layer들을 초기화한다. RC와 Layer를 설정한다. 
 */
static MGLvoid PrvInitFocusEffectLayer( MGLvoid )
{		
	// Focus Effect 1
	MGL_SetCurLayerIndex( ID_LAYER_FOCUSEFFECT_1 );
	MGL_SetLayerX( g_tMainMenu[g_nCurMenuIdx].x - (MENU_FOCUSBG_WIDTH/2));
	MGL_SetLayerY( g_tMainMenu[g_nCurMenuIdx].y - (MENU_FOCUSBG_HEIGHT/2));		
	MGL_SetLayerVisible( TRUE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_focusBg_I );//MGL_focus_I );	

#ifdef ADD_FOCUSEFFECTLAYER_IP	
	MGL_SetIP( &g_tFocusEffectIP1[0], MGL_Scaling, &g_tFocusEffectScalePrm1 );	
	MGL_SetIP( &g_tFocusEffectIP1[1], MGL_Alphablending, &g_tFocusEffectABPrm1 );
	MGL_SetScalingSameRatio( &g_tFocusEffectIP1[0], 100 );
	MGL_SetAlphablendingAlpha( &g_tFocusEffectIP1[1], 0 );
	MGL_SetLayerIPs( NUM_FOCUSEFFECT_IP, g_tFocusEffectIP1);
#endif	//ADD_FOCUSEFFECTLAYER_IP

	// Focus Effect 2
	MGL_SetCurLayerIndex( ID_LAYER_FOCUSEFFECT_2 );
	MGL_SetLayerX( g_tMainMenu[g_nCurMenuIdx].x - (MENU_FOCUSBG_WIDTH/2));
	MGL_SetLayerY( g_tMainMenu[g_nCurMenuIdx].y - (MENU_FOCUSBG_HEIGHT/2));
	MGL_SetLayerVisible( TRUE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_focusBg_I );//MGL_focus_I );

#ifdef ADD_FOCUSEFFECTLAYER_IP	
	// Add IP on Focus Effect 2 Layer
	MGL_SetIP( &g_tFocusEffectIP2[0], MGL_Scaling, &g_tFocusEffectScalePrm2 );	
	MGL_SetIP( &g_tFocusEffectIP2[1], MGL_Alphablending, &g_tFocusEffectABPrm2 );
	MGL_SetScalingSameRatio( &g_tFocusEffectIP2[0], 100 );
	MGL_SetAlphablendingAlpha( &g_tFocusEffectIP2[1], 0 );
	MGL_SetLayerIPs( NUM_FOCUSEFFECT_IP, g_tFocusEffectIP2);
#endif
	
}

static void PrvStartFocusEffectAnimation( WND wnd )
{
#ifdef ADD_FOCUSEFFECTLAYER_IP
	g_nFocusEffectIdx1 = 0;
	g_nFocusEffectIdx2 = 0;
	
	UsrSetTimer( wnd, TIMER_ID_FOCUSEFFECT1, g_nFocusEffectA[0].nTimerDelay );
	UsrSetTimer( wnd, TIMER_ID_FOCUSEFFECT2, g_nFocusEffectA[0].nTimerDelay + TIMER_GAP_FOCUSEFFECT );
#endif
}

static void PrvStopFocusEffectAnimation( WND wnd )
{
#ifdef ADD_FOCUSEFFECTLAYER_IP	
	PrvInitFocusEffectLayerPos();	
	PrvInitFocusEffectLayerIP();
	//MGL_Render();	// Refresh

	UsrKillTimer( wnd, TIMER_ID_FOCUSEFFECT1 );
	UsrKillTimer( wnd, TIMER_ID_FOCUSEFFECT2 );
#endif
}

static void PrvDoFocusEffectAnimation( WND wnd, int nTimerID, int nLayerID, int * nCurEffectIdxP, MGLip * pIP  )
{	
#ifdef ADD_FOCUSEFFECTLAYER_IP
	int nExtW, nExtH;
	
	// Kill Previous Timer
	UsrKillTimer( wnd, nTimerID );

	nExtW = (g_nFocusEffectA[*nCurEffectIdxP].nScale * MENU_FOCUSBG_WIDTH )/100;
	nExtH = (g_nFocusEffectA[*nCurEffectIdxP].nScale * MENU_FOCUSBG_HEIGHT )/100;
	
	// Adjust Layer Position X, Y
	MGL_SetCurLayerIndex( nLayerID );
	MGL_SetLayerX( g_tMainMenu[g_nCurMenuIdx].x - (nExtW/2));
	MGL_SetLayerY( g_tMainMenu[g_nCurMenuIdx].y - (nExtH/2));

	MGL_SetScalingSameRatio( &pIP[0], g_nFocusEffectA[*nCurEffectIdxP].nScale );
	MGL_SetAlphablendingAlpha( &pIP[1], g_nFocusEffectA[*nCurEffectIdxP].nAlpha );
	
	// Accumulate Handling Index	
	if( *nCurEffectIdxP >= NUM_FOCUSEFFECT_FRAME - 1 )
	{
		*nCurEffectIdxP = 0;

		// Set New Timer
		if( nLayerID == ID_LAYER_FOCUSEFFECT_2 )
		{
			// Restart Again After Layer 2 effect was done
			PrvStartFocusEffectAnimation(wnd);
		}
	}
	else
	{
		*nCurEffectIdxP = *nCurEffectIdxP + 1;
	
		// Set New Timer
		UsrSetTimer( wnd, nTimerID, g_nFocusEffectA[*nCurEffectIdxP].nTimerDelay );
	}
#endif
}

//////////////////////////////// Base

/** 
 * @brief 배경 이미지를 그린다. 
 */
static MGLvoid PrvDrawBackground(MGLvoid * pBuffer, MGLvoid * pParams )
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
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Sample3", StrLen("Sample3"), ALIGN_CENTER);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Cancel", StrLen("Cancel"), ALIGN_RIGHT);

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

	g_nSlideCount = 1;
	g_nCurMenuIdx = 10;
	
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

	// Add Focus Effect Layer.. Added by jibong
	PrvInitFocusEffectLayer();

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
				//MGL_IMGSEL_KEEP_ORDER );

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
	UsrSetTimer( wnd, TIMER_MGL_MENU_FOCUS, TIMER_VAL_MENU_FOCUS );
	UsrSetTimer( wnd, TIMER_MGL_ITEM_ANI,   TIMER_VAL_ITEM_ANI );

	// Added by jibong
	PrvStartFocusEffectAnimation( wnd );
}

/** 
 * @brief Focus 애니메이션을 멈춘다.
 */
static void PrvStopFocusAnimation( WND wnd )
{
	PrvInitFocusAnimation();
	UsrKillTimer( wnd, TIMER_MGL_MENU_FOCUS );
	UsrKillTimer( wnd, TIMER_MGL_ITEM_ANI );

	MGL_SetCurLayerIndex( ID_LAYER_MENU_R1C1 + g_nPrevMenuIdx );
	MGL_SetLayerVisible( FALSE );	//### 이전 layer 안보이게 하기.

	// Added by jibong
	PrvStopFocusEffectAnimation( wnd );
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
		PrvInitFocusAnimation();
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
		x = g_tMainMenu[g_nCurMenuIdx].x - width/2;
		y = g_tMainMenu[g_nCurMenuIdx].y - height/2;
		MGL_SetLayerBounds( x, y, width, height );
		MGL_SetScalingSameRatio( &g_tFocusIP[0], g_nFocusScale );
		MGL_SetAlphablendingAlpha( &g_tFocusIP[1], g_nFocusAlpha );
	}

	return;
}


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
	}

	if( g_nSlideCount >= 15 )
	{
		g_nSlideCount = 0;
		UsrKillTimer( wnd, TIMER_MGL_SLIDE_MENU );		
	}

	g_nSlideCount++;
	
	return;
}

static int MsgCreate(WND wnd, int msg, int wparam, int lparam)
{
	PrvInitRC( wnd );
	MGL_Render();

	return 0;
}

static int MsgDestroy(WND wnd, int msg, int wparam, int lparam)
{
	UsrKillTimer( wnd, TIMER_MGL_MENU_FOCUS );
	UsrKillTimer( wnd, TIMER_MGL_ITEM_ANI );
	UsrKillTimer( wnd, TIMER_MGL_SLIDE_MENU );
	UsrKillTimer( wnd, TIMER_ID_FOCUSEFFECT1 );
	UsrKillTimer( wnd, TIMER_ID_FOCUSEFFECT2 );

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
		PrvStartFocusAnimation( wnd );
		break;

	case VK_USER_LEFT:

		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx == 0 )
			g_nCurMenuIdx = 11;
		else
			g_nCurMenuIdx--;
		PrvStartFocusAnimation( wnd );
		break;

	case VK_USER_UP:
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx <= 2 )
			g_nCurMenuIdx += NUM_MENU_ITEM;
		g_nCurMenuIdx -=3;
		PrvStartFocusAnimation( wnd );
		break;

	case VK_USER_DOWN:
		g_nPrevMenuIdx = g_nCurMenuIdx;
		PrvStopFocusAnimation( wnd );
		if( g_nCurMenuIdx >= 9 )
			g_nCurMenuIdx -= NUM_MENU_ITEM;
		g_nCurMenuIdx += 3;
		PrvStartFocusAnimation( wnd );
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
	else if( id == TIMER_MGL_ITEM_ANI )
	{
		if( g_nPrevMenuIdx != g_nCurMenuIdx )
		{
			MGL_SetCurLayerIndex( ID_LAYER_MENU_R1C1 + g_nPrevMenuIdx );
			MGL_SetLayerVisible( FALSE );	//### 이전 layer 안보이게 하기.
		}

		MGL_SetCurLayerIndex( ID_LAYER_MENU_R1C1 + g_nCurMenuIdx );
		MGL_SetLayerVisible( TRUE );
		MGL_AnimatorUpdateAll (&g_tItemAnimator[g_nCurMenuIdx]);
 
		MGL_Render();
	}	
	else if( id == TIMER_ID_FOCUSEFFECT1 )
	{
		PrvDoFocusEffectAnimation( wnd, TIMER_ID_FOCUSEFFECT1, 
			ID_LAYER_FOCUSEFFECT_1, &g_nFocusEffectIdx1, g_tFocusEffectIP1  );
		MGL_Render();	
	}
	else if( id == TIMER_ID_FOCUSEFFECT2 )
	{
		PrvDoFocusEffectAnimation( wnd, TIMER_ID_FOCUSEFFECT2, 
			ID_LAYER_FOCUSEFFECT_2, &g_nFocusEffectIdx2, g_tFocusEffectIP2  );
		MGL_Render();
	}

	return 0;
}

int MGLSample3Proc(WND wnd, int msg, int wparam, int lparam)
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
