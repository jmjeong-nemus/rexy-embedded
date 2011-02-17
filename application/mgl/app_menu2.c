#include "rexy.h"
#include "rexy_haldevice.h"

#include "mgl.h"
#include "mgl_ip.h"
#include "mgl_animator.h"
#include "mgl_itemmanager.h"
#include "mgl_keyframe.h"
#include "mgl_imageselector.h"
#include "mgl_particlepile.h"
#include "application.h"

/**
 * @brief Private Macros
 */
#define JOSHUA_VFACTOR 50
#define TIMER_MGL_SAMPLE_2			0x7777
#define TIMER_VAL_SAMPLE_2			50

#define NUM_LAYERS					7
#define ID_LAYER_BACK				0				// Back Layer ID
#define ID_LAYER_BG_PILE			1
#define ID_LAYER_MENU_SLIDE			2
#define ID_LAYER_FI_PILE			3
#define ID_LAYER_PARTICLE			4				//
#define ID_LAYER_MENU_FOCUS			5				// Selected Menu Item Layer ID
#define ID_LAYER_MENU_ICON			6				// Menu Item Layer ID



#define NUM_MENU_ITEM				12					// 메뉴 아이템의 갯수 
#define MENU_ICON_WIDTH				72					// 메뉴 아이템의 가로 길이
#define MENU_ICON_HEIGHT			70					// 메뉴 아이템의 세로 길이 

#define MENU_FOCUS_WIDTH			82					// 선택된 메뉴 아이템의 가로 길이.
#define MENU_FOCUS_HEIGHT			82					// 선택된 메뉴 아이템의 세로 길이.

#define NUM_SLIDE_ITEMS				4
#define NUM_SLIDE_FRAMES			8

#define NUM_READY_FRAMES			8

#define NUM_MENU_ITEM_IMAGES	 	12
#define NUM_MENU_ITEM_SEQS		 	2

/* particle pile */
#define PA_PILE_BG_START_X		0
#define PA_PILE_BG_START_Y		10
#define PA_PILE_BG_WIDTH		LCD_WIDTH
#define PA_PILE_BG_HEIGHT		LCD_HEIGHT - PA_PILE_BG_START_Y

#define MGL_MAX_PARTICLE		50	// 파티클의 갯수

#define MGL_X_MIN				0
#define MGL_X_MAX				LCD_WIDTH+(LCD_WIDTH/2)
#define MGL_Y_MIN				0
#define MGL_Y_MAX				0
#define MGL_VX_MIN				-(LCD_WIDTH/8)
#define MGL_VX_MAX				(LCD_WIDTH/8)
#define MGL_VY_MIN				LCD_HEIGHT/32
#define MGL_VY_MAX				LCD_HEIGHT/8
#define MGL_MASS_MIN			2
#define MGL_MASS_MAX			2
#define MGL_SPAN_MIN			0
#define MGL_SPAN_MAX			LCD_HEIGHT/2

#define MGL_DELTA_TIME_FACTOR	0.05f
#define MGL_AFC_FACTOR			0.90f
#define MGL_GFX_FACTOR			0
#define MGL_GFY_FACTOR			50

typedef struct MGL_MENU {
	int x;				//< icon의 중심 X좌표
	int y;				//< icon의 중심 Y좌표

	char szTitle[20];	//< 메뉴 아이템의 타이틀
} MGL_MENU;

/**
 * @brief Private Variables
 */
static MGLrc					g_tRC;
static MGLlayer					g_aLayers[NUM_LAYERS];

/* slide */
static MGLalphablendingparams 	g_tSABParams;
static MGLip					g_tSIP;

static MGLanimator				g_tSAnimator;
static MGLsubsystem				g_tSSubsystem;

static MGLitemmanager tSItemManager;
static MGLanidata pSItemInitValues[NUM_SLIDE_ITEMS * 3];
static MGLinitval pSInitVal[3];

static MGLkeyframe tSKeyFrame;
static MGLinterpolator pSInterpolator[2];
static MGLanidata pSKeyData[8];
static MGLdistent pSDistEnt[4];
static MGLanidata pSResults[NUM_SLIDE_FRAMES * 2];         

/* focus */
static MGLscalingparams 		g_tFScalingParams;
static MGLalphablendingparams 	g_tFABParams;
static MGLip					g_tFIP[2];
static MGLanimator				g_tFAnimator;
static MGLsubsystem				g_tFSubsystem;

/* icon */
static MGLanimator				g_tIAnimator;
static MGLsubsystem				g_tISubsystem;
static MGLimgselector			g_tItemImageSelector[NUM_MENU_ITEM];
static MGLimgselitem			g_tItemImages[NUM_MENU_ITEM][NUM_MENU_ITEM_IMAGES];

/* particle pile */
static MGLscalingparams			g_tPScaleParams;
static MGLalphablendingparams	g_tPABParams;
static MGLip					g_tPIP[2];

static MGLanimator				g_tPAnimator;
static MGLsubsystem				g_tPSubsystem;

static MGLpapileunityinfo		g_tPapileunityinfo;
static MGLpapileimageinfo		g_tPapileImageInfo;
static MGLpapileipinfo			g_tPapileIpInfo[2];

static MGLpapileipinfo			g_tPapileSingleIpInfo;

static MGLint g_pPapileABData[2]	=	{ 100, 100 };
static MGLint g_pPapileScaleData[2]	=	{ 100, 100 };
static MGLint g_pPapileImage[2]		=	{ MGL_snow_I, MGL_snow_I };

static MGLbyte g_tPapileBgCoordiInfo[LCD_WIDTH * LCD_HEIGHT] = {{0},};
static MGLbyte g_tPapileFiCoordiInfo[LCD_WIDTH * LCD_HEIGHT] = {{0},};

static MGLpapilebgrenderinfo g_tPapileBgRenderInfo[MGL_MAX_PARTICLE];
static MGLpapilefirenderinfo g_tPapileFiRenderInfo[MGL_MAX_PARTICLE];

static MGLushort g_pPileBgWndMemBuf[LCD_WIDTH * LCD_HEIGHT * 2 / sizeof(MGLushort)];
static MGLushort g_pPileFiWndMemBuf[LCD_WIDTH * LCD_HEIGHT * 2 / sizeof(MGLushort)];

static MGLpapilebgareainfo g_tPapileBgAreaInfo;
static MGLpapilefiareainfo g_tPapileFiAreaInfo;

static MGLpapilestdinfo g_tPapileStdInfoArray[MGL_MAX_PARTICLE];
static MGLpapilestdattrib g_tPapileStdPpt = {	MGL_X_MIN,    MGL_X_MAX,
												MGL_Y_MIN,    MGL_Y_MAX,
												MGL_VX_MIN,   MGL_VX_MAX,
												MGL_VY_MIN,   MGL_VY_MAX,
												MGL_MASS_MIN, MGL_MASS_MAX,
												MGL_SPAN_MIN, MGL_SPAN_MAX
};
static MGLpapilefactorattrib g_tPapileFactorPpt = { MGL_AFC_FACTOR, MGL_DELTA_TIME_FACTOR,
													MGL_GFX_FACTOR, MGL_GFY_FACTOR };
static WND g_tBgWnd;
static WND g_tPileBgWnd;
static WND g_tPileFiWnd;

/* menu text */
static MGL_MENU		g_tMainMenu[NUM_MENU_ITEM] = {
		{  41, 101-JOSHUA_VFACTOR, "To DO" },
		{ 120, 101-JOSHUA_VFACTOR, "Contacts" },
		{ 199, 101-JOSHUA_VFACTOR, "Multimedia" },

		{  41, 146-JOSHUA_VFACTOR, "Internet" },
		{ 120, 146-JOSHUA_VFACTOR, "Mailbox" },
		{ 199, 146-JOSHUA_VFACTOR, "Download" },
	
		{  41, 201-JOSHUA_VFACTOR, "Schedule" },
		{ 120, 201-JOSHUA_VFACTOR, "Camera" },
		{ 199, 201-JOSHUA_VFACTOR, "Settings" },

		{  41, 247-JOSHUA_VFACTOR, "PDA" },
		{ 120, 247-JOSHUA_VFACTOR, "Time" },
		{ 199, 247-JOSHUA_VFACTOR, "Alarm" }
};

static int nStatus;

static int g_nCurMenuIdx = 0;	// 현재 메뉴 Index
static int g_nPrevMenuIdx = 0;	// 이전 메뉴 Index

static int g_nFocusAlpha;
static int g_nFocusScale;
static int g_nFocusCount;

static int MsgCreate( WND wnd, int msg, int wparam, int lparam );
static int MsgDestroy( WND wnd, int msg, int wparam, int lparam );
static int MsgKeyDown( WND wnd, int key, int option );

static MGLvoid PrvDrawBackground( MGLvoid * pBuffer, MGLvoid * pParams );
static MGLvoid PrvInitRC( WND wnd );
static MGLvoid SetSlideAnimation (MGLvoid);
static MGLvoid SetIconAnimation (MGLvoid);
static MGLvoid SetParticleAnimation (MGLvoid);
static MGLvoid MoveFocus (MGLvoid);
static MGLvoid MoveIcon (MGLvoid);
static MGLvoid PrvInitFocusAnimation(MGLvoid);
static MGLvoid ClearParticle (MGLvoid);

MGLint 
MGLSample2Proc(WND wnd, int msg, int wparam, int lparam);

static MGLvoid PrvDrawBackground(MGLvoid * pBuffer, MGLvoid * pParams)
{
	MGLbitmap bitmap;

	// MGL_LoadBitmap( MGL_title_I, &bitmap );
	// MGL_DrawBitmap( &bitmap, 0, 0 );

	MGL_LoadBitmap( MGL_back_I, &bitmap );
	MGL_DrawBitmap( &bitmap, 0, 0 );

	/*
	MGL_LoadBitmap( MGL_softkey_I, &bitmap );
	MGL_DrawBitmap( &bitmap, 0, 282 );

	// draw texts
	MGL_SetBgColor(MGL_GetDCTrColor());
	MGL_SetFgColor(MGL_MakeColor(255, 255, 255));
	GdiDrawTextOut((WND)pBuffer, 0, 20, 240, g_tMainMenu[g_nCurMenuIdx].szTitle, strlen(g_tMainMenu[g_nCurMenuIdx].szTitle), ALIGN_CENTER);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Option", StrLen("Option"), ALIGN_LEFT);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Sample2", StrLen("Sample2"), ALIGN_CENTER);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Cancel", StrLen("Cancel"), ALIGN_RIGHT);
	*/
	return;
}

static MGLvoid PrvInitRC( WND wnd )
{
	g_nCurMenuIdx = 4;
	
	printf("prvInitRC");

	MGL_SetDCRCs(1, &g_tRC); 
	MGL_SetDCCurRC( &g_tRC );

	MGL_SetRCBounds( 0, 0, LCD_WIDTH, LCD_HEIGHT );
	MGL_SetRCLayers( NUM_LAYERS, g_aLayers );

	/* custom bg image layer */

	MGL_SetCurLayerIndex( ID_LAYER_BACK );
	MGL_SetLayerBounds( 0, 0, LCD_WIDTH, LCD_HEIGHT );
	MGL_SetLayerVisible (TRUE) ;
	MGL_SetLayerType( MGL_CUSTOM_LAYER );
	MGL_SetLayerDrawFuncPtr( PrvDrawBackground );

	/* custom particle bg layer*/

	g_tPileBgWnd = WinCreateWindow ( NULL, 0, 0, LCD_WIDTH, LCD_HEIGHT, WND_CUSTOM_MEM ) ;
	g_tPileBgWnd->dev->mem_addr = g_pPileBgWndMemBuf;
	
	MGL_SetCurLayerIndex( ID_LAYER_BG_PILE );
	MGL_SetLayerBounds( 0, 0, LCD_WIDTH, LCD_HEIGHT );
	MGL_SetLayerVisible( TRUE );
	MGL_SetLayerType( MGL_CUSTOM_LAYER );
	MGL_SetLayerDrawFuncPtr( MGL_DrawParticlePileBgArea );	

	MGL_SetCustomLayerBuffer(g_tPileBgWnd);
	MGL_SetCustomLayerParams(&g_tPapileunityinfo);
	MGL_ClearBuffer(g_tPileBgWnd);

	/* slide image layer */
	MGL_SetCurLayerIndex (ID_LAYER_MENU_SLIDE);
	MGL_SetLayerBounds (0, 54, LCD_WIDTH, LCD_HEIGHT);
	MGL_SetLayerVisible (TRUE) ;
	MGL_SetLayerType (MGL_EXTERNAL_IMAGE_LAYER);
	MGL_SetIP (&g_tSIP, MGL_Alphablending, &g_tSABParams);
	MGL_SetLayerIPs (1, &g_tSIP);

	SetSlideAnimation ();

	// custom particle fi layer */

	g_tPileFiWnd = WinCreateWindow ( NULL, 0, 0, LCD_WIDTH, LCD_HEIGHT, WND_MEM ) ;
	g_tPileFiWnd->dev->mem_addr = g_pPileFiWndMemBuf;

	MGL_SetCurLayerIndex(ID_LAYER_FI_PILE);
	MGL_SetLayerBounds( 0, 0, MENU_ICON_WIDTH, MENU_ICON_HEIGHT );
	MGL_SetLayerVisible(TRUE);
	MGL_SetLayerType( MGL_CUSTOM_LAYER );
	MGL_SetLayerDrawFuncPtr( MGL_DrawParticlePileFiArea );
	
	MGL_SetCustomLayerBuffer(g_tPileFiWnd);
	MGL_SetCustomLayerParams(&g_tPapileunityinfo);
	MGL_ClearBuffer(g_tPileFiWnd);

	/* particle image layer */
	MGL_SetCurLayerIndex(ID_LAYER_PARTICLE);
	MGL_SetLayerBounds(0, 0, LCD_WIDTH, LCD_HEIGHT);
	MGL_SetLayerVisible(TRUE);
	MGL_SetLayerType(MGL_EXTERNAL_IMAGE_LAYER);	

	MGL_SetIP (&g_tPIP[0], MGL_Scaling, &g_tPScaleParams);
	MGL_SetScalingSameRatio (&g_tPIP[0], TRUE);

	MGL_SetIP (&g_tPIP[1], MGL_Alphablending, &g_tPABParams);

	MGL_SetLayerIPs( 2, g_tPIP);	
	
	SetParticleAnimation();

	/* menu icon Layer*/
	MGL_SetCurLayerIndex (ID_LAYER_MENU_ICON);
	MGL_SetLayerBounds (0, 0, MENU_ICON_WIDTH, MENU_ICON_HEIGHT);
	MGL_SetLayerVisible (TRUE);
	MGL_SetLayerType (MGL_EXTERNAL_IMAGE_LAYER);

	SetIconAnimation ();
	/* focus icon layer */
	MGL_SetCurLayerIndex( ID_LAYER_MENU_FOCUS );
	MGL_SetLayerBounds( 0, 0, MENU_FOCUS_WIDTH, MENU_FOCUS_HEIGHT );
	MGL_SetLayerVisible( FALSE );
	MGL_SetLayerType( MGL_IMAGE_LAYER );
	MGL_SetLayerBitmap( MGL_focus_I );

	MGL_SetIP( &g_tFIP[0], MGL_Scaling, &g_tFScalingParams );
	MGL_SetIP( &g_tFIP[1], MGL_Alphablending, &g_tFABParams );
	MGL_SetLayerIPs( 2, g_tFIP);

	PrvInitFocusAnimation();

	/* Create Particle */
	MGL_CreateParticlePile(&g_tPapileunityinfo);

	MoveIcon();
	MoveFocus();


	UsrSetTimer( wnd, TIMER_MGL_SAMPLE_2, TIMER_VAL_SAMPLE_2);
}

static MGLvoid
SetParticleAnimation (MGLvoid)
{
	
	/* Setting Animator*/
	MGL_SetAnimator (&g_tPAnimator, &g_tPSubsystem, 1);
	MGL_AnimatorSetTiming (&g_tPAnimator, MGL_TIMING_TYPE_FRAME, TIMER_VAL_SAMPLE_2);
	MGL_AnimatorSetRepeatCount (&g_tPAnimator, 1);

	/* Linking Animator & External Layer */
	MGL_AnimatorSetExternalImageLayer(&g_tPAnimator, MGL_MAX_PARTICLE);
	
	/* Clear Animator */
	MGL_AnimatorClear (&g_tPAnimator);
	
	/* Linking Particle & Animator */
	MGL_ParticlePileSetAnimator(&g_tPapileunityinfo, &g_tPSubsystem);
	
	/* Setting Image & Processing */
	MGL_ParticlePileSetImageProcSystem(&g_tPapileunityinfo, &g_tPapileImageInfo, g_tPapileIpInfo);
	MGL_ParticlePileSetImagesInit ( &g_tPapileunityinfo, g_pPapileImage, 2 );
	MGL_ParticlePileSetIpInfo ( &g_tPapileunityinfo, 2, 2 );

	MGL_ParticlePileSetIpInit ( &g_tPapileunityinfo, g_pPapileScaleData, &g_tPIP[0],  MGL_PARAM_SCALE_SAME_RATIO,  0 );
	MGL_ParticlePileSetIpInit ( &g_tPapileunityinfo, g_pPapileABData, &g_tPIP[1], MGL_PARAM_ALPHA, 1 );
	MGL_ParticlePileSetImageProcInit (&g_tPapileunityinfo, MGL_PA_PILE_SET_IMG_PROC_KEEP_UP, NULL, 0 );

	/* Setting particle bg & fi pile render & coordinate info */
	MGL_SetParticlePileRenderInfo (&g_tPapileunityinfo, g_tPapileBgRenderInfo, MGL_MAX_PARTICLE, g_tPapileFiRenderInfo, MGL_MAX_PARTICLE);
	MGL_SetParticlePileCoordiInfo (&g_tPapileunityinfo, g_tPapileBgCoordiInfo,LCD_WIDTH * LCD_HEIGHT, g_tPapileFiCoordiInfo,LCD_WIDTH * LCD_HEIGHT );	
	
	/* Setting particle pile bg & fi pile area info */
	MGL_SetParticlePileAreaInfo ( &g_tPapileunityinfo, &g_tPapileBgAreaInfo, &g_tPapileFiAreaInfo);
	MGL_SetParticlePileBgInfo ( &g_tPapileunityinfo, PA_PILE_BG_START_X, PA_PILE_BG_START_Y, PA_PILE_BG_WIDTH, PA_PILE_BG_HEIGHT );
	
	/* Setting particle pile system */ 
	MGL_SetParticlePileSystem(&g_tPapileunityinfo, g_tPapileStdInfoArray, &g_tPapileStdPpt, &g_tPapileFactorPpt, MGL_MAX_PARTICLE);

	/* Animator start */
	MGL_AnimatorStart(&g_tPAnimator,0);
	
	return;
}

static MGLvoid 
SetIconAnimation (MGLvoid)
{
	MGLint nImageIdx, nCount;

	/* Setting Animator*/
	MGL_SetAnimator (&g_tIAnimator, &g_tISubsystem, 1);
	MGL_AnimatorSetTiming (&g_tIAnimator, MGL_TIMING_TYPE_FRAME, 0);
	MGL_AnimatorSetRepeatCount (&g_tIAnimator, 0);

	/* Linking Animator & External Layer */
	MGL_AnimatorSetExternalImageLayer (&g_tIAnimator, 1);

	nImageIdx = MGL_icon_01_01_I;
	for (nCount = 0; nCount < NUM_MENU_ITEM; nCount++ ) {
		/* Setting ImageSelector */
		MGL_SetImageSelector (&g_tItemImageSelector[nCount], g_tItemImages[nCount], 
				NULL, NUM_MENU_ITEM_IMAGES, NULL, 0, MGL_IMGSEL_KEEP_ORDER );

		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx,     0);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx + 1, 1);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx + 2, 2);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx + 3, 3);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx + 3, 4);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx + 3, 5);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx + 3, 6);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx + 2, 7);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx + 1, 8);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx,     9);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx,    10);
		MGL_ImageSelectorSetImage (&g_tItemImageSelector[nCount], nImageIdx,    11);
		nImageIdx += 4;

		// Linking ImageSelector & Animator
		MGL_ImageSelectorSetInSubsystem (&g_tItemImageSelector[nCount], g_tIAnimator.pSubsystems);

		MGL_AnimatorClear (&g_tIAnimator);
	}
}

static MGLvoid 
PrvInitFocusAnimation(MGLvoid)
{
	g_nFocusAlpha = 0;
	g_nFocusScale = 90;
	g_nFocusCount = 0;

	return;
}

static MGLvoid 
SetSlideAnimation (MGLvoid)
{
#define JOSHUA_HFACTOR	30
	MGLint pPosXs[4] = { JOSHUA_HFACTOR, JOSHUA_HFACTOR, JOSHUA_HFACTOR, JOSHUA_HFACTOR};
	MGLint pPosYs[4];
	MGLint pIntervals[4];
	MGLint pImages[4];

	MGL_SetKeyFrame (&tSKeyFrame, NUM_SLIDE_FRAMES, pSInterpolator, 2);

	MGL_KeyFrameSetInterpolator (pSInterpolator + 0, NULL, MGL_ANI_PARAM_POSITION_Y, pSKeyData, 4,
			pSDistEnt, 2, MGL_KEYFRAME_IPF_INT_LINEAR, NULL, NULL, MGL_KEYFRAME_ESF_LINEAR,NULL);

	MGL_KeyFrameSetIntKeyData (pSInterpolator + 0, 0);
	MGL_KeyFrameSetIntKeyData (pSInterpolator + 0, 13);
	
	MGL_KeyFrameSetInterpolator (pSInterpolator + 1, &g_tSIP, MGL_PARAM_ALPHA, pSKeyData + 4, 4,
			pSDistEnt + 2, 2, MGL_KEYFRAME_IPF_INT_LINEAR, NULL, NULL, MGL_KEYFRAME_ESF_LINEAR,NULL);
	MGL_KeyFrameSetIntKeyData (pSInterpolator + 1, 0);
	MGL_KeyFrameSetIntKeyData (pSInterpolator + 1, 100);

	MGL_KeyFrameStandby (&tSKeyFrame);

	MGL_KeyFramePreCalculate (&tSKeyFrame, pSResults, NUM_SLIDE_FRAMES * 2);

	MGL_SetItemManager (&tSItemManager, pSInitVal, 3, NUM_SLIDE_ITEMS, 
						pSItemInitValues, MGL_ITEMMANAGER_OPTION_KEEP_LAST);

	pIntervals[0] = 0;
	pIntervals[1] = 2;
	pIntervals[2] = 2;
	pIntervals[3] = 2;
	MGL_ItemManagerSetStaticInterval (&tSItemManager, 0, pIntervals, NUM_SLIDE_ITEMS);

	

	pPosYs[0] = 53 - JOSHUA_VFACTOR;
	pPosYs[1] = 114 - JOSHUA_VFACTOR;
	pPosYs[2] = 167 - JOSHUA_VFACTOR;
	pPosYs[3] = 219 - JOSHUA_VFACTOR;
	MGL_ItemManagerSetStaticInitValue (&tSItemManager, 1, pPosYs, 
						NUM_SLIDE_ITEMS, NULL, MGL_ANI_PARAM_POSITION_Y);



	pImages[0] = MGL_menus_01_I;
	pImages[1] = MGL_menus_02_I;
	pImages[2] = MGL_menus_03_I;
	pImages[3] = MGL_menus_04_I;
	MGL_ItemManagerSetStaticInitValue (&tSItemManager, 2, pImages, 
						NUM_SLIDE_ITEMS, NULL, MGL_ANI_PARAM_IMAGE);

	MGL_KeyFrameSetInSubsystem (&tSKeyFrame, &(tSItemManager.tSubsystem));

	MGL_SetAnimator (&g_tSAnimator, &g_tSSubsystem, 1);
	MGL_AnimatorSetTiming (&g_tSAnimator, MGL_TIMING_TYPE_FRAME, 0);
	MGL_AnimatorSetRepeatCount (&g_tSAnimator, 1);

	MGL_ItemManagerSetInSubsystem (&tSItemManager, g_tSAnimator.pSubsystems);
	MGL_AnimatorSetExternalImageLayer (&g_tSAnimator, NUM_SLIDE_ITEMS);
	MGL_AnimatorClear (&g_tSAnimator);

	nStatus = 1;

	return;
}

static MGLvoid
MoveFocus (MGLvoid)
{
	MGLint nX, nY, width, height;

	MGL_SetCurLayerIndex (ID_LAYER_MENU_FOCUS);
	MGL_SetLayerVisible( TRUE );
	
	g_nFocusCount++;
	if( !(g_nFocusCount >= 11) )
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
	}

	width = MENU_FOCUS_WIDTH*g_nFocusScale/100;
	height = MENU_FOCUS_HEIGHT*g_nFocusScale/100;
	nX = g_tMainMenu[g_nCurMenuIdx].x - (width / 2);
	nY = g_tMainMenu[g_nCurMenuIdx].y - (height / 2);

	MGL_SetLayerBounds (nX, nY, width, height);
	MGL_SetScalingSameRatio( &g_tFIP[0], g_nFocusScale );
	MGL_SetAlphablendingAlpha( &g_tFIP[1], g_nFocusAlpha );

	// for particle pile
	MGL_SetParticlePileFiInfo (&g_tPapileunityinfo, nX, nY-4, width-8, height);

	return;
}

static MGLvoid
MoveIcon (MGLvoid)
{
	MGLint nX, nY;

	//g_nCurMenuIdx 를 이용 
	nX = g_tMainMenu[g_nCurMenuIdx].x - (MENU_ICON_WIDTH / 2);
	nY = g_tMainMenu[g_nCurMenuIdx].y - (MENU_ICON_HEIGHT / 2);
	
	MGL_SetCurLayerIndex (ID_LAYER_MENU_ICON);
	MGL_SetLayerBounds (nX, nY, MENU_ICON_WIDTH, MENU_ICON_HEIGHT);

	/* Linking ImageSelector & Animator */
	MGL_ImageSelectorSetInSubsystem (&g_tItemImageSelector[g_nCurMenuIdx], g_tIAnimator.pSubsystems);
	MGL_AnimatorClear (&g_tIAnimator);
}

static MGLint 
MsgCreate(WND wnd, int msg, int wparam, int lparam)
{
	PrvInitRC( wnd );
	WinSetCurrentProc( wnd, MGLSample2Proc );
	return 0;
}

static MGLint 
MsgDestroy(WND wnd, int msg, int wparam, int lparam)
{
	UsrKillTimer( wnd, TIMER_MGL_SAMPLE_2);
	MGL_AnimatorClear ( &g_tPAnimator);

	MGL_ClearParticlePileFiArea( &g_tPapileunityinfo );

	if (g_tPileBgWnd->dev->mem_addr)
		g_tPileBgWnd->dev->mem_addr = NULL;
	WinDestroyWindow(g_tPileBgWnd);

	if (g_tPileFiWnd->dev->mem_addr)
		g_tPileFiWnd->dev->mem_addr = NULL;
	WinDestroyWindow(g_tPileFiWnd);

  return 0;
}

static MGLvoid
ClearParticle(MGLvoid)
{
	MGL_ClearParticlePileFiArea( &g_tPapileunityinfo );
	MGL_ClearBuffer(g_tPileFiWnd);
}

static MGLint 
MsgKeyDown( WND wnd, int key, int option )
{
	switch( key ) 
	{
		case VK_USER_RIGHT:
			g_nPrevMenuIdx = g_nCurMenuIdx;
			if( g_nCurMenuIdx == 11 )
				g_nCurMenuIdx = 0;
			else
				g_nCurMenuIdx++;
			PrvInitFocusAnimation();
			MoveFocus ();
			MoveIcon();
			ClearParticle();
			break;

		case VK_USER_LEFT:

			g_nPrevMenuIdx = g_nCurMenuIdx;
			if( g_nCurMenuIdx == 0 )
				g_nCurMenuIdx = 11;
			else
				g_nCurMenuIdx--;
			PrvInitFocusAnimation();
			MoveFocus ();
			MoveIcon();
			ClearParticle();
			break;

		case VK_USER_UP:
			g_nPrevMenuIdx = g_nCurMenuIdx;
			if( g_nCurMenuIdx <= 2 )
				g_nCurMenuIdx += NUM_MENU_ITEM;
			g_nCurMenuIdx -=3;
			PrvInitFocusAnimation();
			MoveFocus ();
			MoveIcon();
			ClearParticle();
			break;

		case VK_USER_DOWN:
			g_nPrevMenuIdx = g_nCurMenuIdx;
			if( g_nCurMenuIdx >= 9 )
				g_nCurMenuIdx -= NUM_MENU_ITEM;
			g_nCurMenuIdx += 3;
			PrvInitFocusAnimation();
			MoveFocus ();
			MoveIcon();
			ClearParticle();
			break;

		case VK_USER_OK:
			StaGotoState( wnd, STATE_ID_MGL_DEMO_IDLE, MSG_DESTROY, MSG_RETURN );
			break;
	}
	return 0;
}

static MGLint 
MsgTimer(WND wnd, int msg, int id, int lparam)
{
	static int n = 0;
	if( id == TIMER_MGL_SAMPLE_2) {
		MGL_Render();
		MGL_AnimatorUpdate(&g_tPAnimator,0);
		MGL_AnimatorUpdate(&g_tSAnimator,0);
		MGL_AnimatorUpdate (&g_tIAnimator,0);

		if ( g_nFocusCount <= 11 )
		{
			MoveFocus ();
		}

		if (++n % 5 == 0) {
			n = 0;
			MGL_AnimatorUpdate (&g_tIAnimator,0);
		}
	}

	return 0;
}

MGLint 
MGLSample2Proc(WND wnd, int msg, int wparam, int lparam)
{
	MGLint	handled = FALSE;

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
