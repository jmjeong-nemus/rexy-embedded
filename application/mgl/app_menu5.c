#include "rexy.h"
#include "rexy_haldevice.h"

#include "mgl.h"
#include "mgl_ip.h"
#include "mgl_animator.h"
#include "mgl_itemmanager.h"
#include "mgl_keyframe.h"
#include "mgl_imageselector.h"
#include "application.h"

/**
 * @brief Private Macros
 */
/**
 * @brief	0x00 : Background layer
 *			0x01 : Slide Menu layer 
 *			0x02 : Focus Menu layer
 *			0x02 : Icon Menu layer
 */
#define TIMER_MGL_SAMPLE_5			0x5555
#define TIMER_VAL_SAMPLE_5			1
#define TIMER_VAL_ICON_ANI			1


#define NUM_LAYERS					4
#define ID_LAYER_BACK				0				// Back Layer ID
#define ID_LAYER_MENU_SLIDE			1
#define ID_LAYER_MENU_FOCUS			2				// Selected Menu Item Layer ID
#define ID_LAYER_MENU_ICON			3				// Menu Item Layer ID

#define NUM_MENU_ITEM				12					// 메뉴 아이템의 갯수 
#define MENU_ICON_WIDTH				72					// 메뉴 아이템의 가로 길이
#define MENU_ICON_HEIGHT			70					// 메뉴 아이템의 세로 길이 

#define MENU_FOCUS_WIDTH			82					// 선택된 메뉴 아이템의 가로 길이.
#define MENU_FOCUS_HEIGHT			82					// 선택된 메뉴 아이템의 세로 길이.

#define FOCUS_RADIUS				45
#define NUM_FOCUS_FRAMES			55
#define NUM_FOCUS_GROUPS			5
#define NUM_FOCUS_ITEMS_PER_GROUP	5
#define NUM_FOCUS_ITEMS				(NUM_FOCUS_GROUPS * NUM_FOCUS_ITEMS_PER_GROUP)

#define NUM_SLIDE_ITEMS				4
#define NUM_SLIDE_FRAMES			8

#define NUM_READY_FRAMES			8

#define NUM_MENU_ITEM_IMAGES	 	12
#define NUM_MENU_ITEM_SEQS		 	2

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

static MGLanimator				tSAnimator;
static MGLsubsystem				tSSubsystem;

static MGLitemmanager tSItemManager;
static MGLanidata pSItemInitValues[NUM_SLIDE_ITEMS * 3];
static MGLinitval pSInitVal[3];          /* y, interval, image */

static MGLkeyframe tSKeyFrame;
static MGLinterpolator pSInterpolator[2];    /* y, alpha */
static MGLanidata pSKeyData[8];         /* (2 + 2) * 2 */
static MGLdistent pSDistEnt[4];         /* ((2 - 1) * 1 + 1) * 2 */
static MGLanidata pSResults[NUM_SLIDE_FRAMES * 2];         

/* focus */
static MGLrotationparams 		g_tFRotationParams;
static MGLscalingparams 		g_tFScalingParams;
static MGLalphablendingparams 	g_tFABParams;
static MGLip					g_tFIP[3];
static MGLanimator				tFAnimator;
static MGLsubsystem				tFSubsystem[NUM_FOCUS_GROUPS];

static MGLitemmanager tFItemManager[NUM_FOCUS_GROUPS];
static MGLanidata pFItemInitValues[NUM_FOCUS_GROUPS * NUM_FOCUS_ITEMS_PER_GROUP * 6];
static MGLinitval pFInitVal[NUM_FOCUS_GROUPS * 6];          /* x, y, interval, rotate, scale, alpha */

static MGLkeyframe tFKeyFrame[NUM_FOCUS_GROUPS];
static MGLinterpolator pFInterpolator[NUM_FOCUS_GROUPS * 4];    /* position, rotate, scale, alpha */
static MGLanidata pFKeyData[NUM_FOCUS_GROUPS * 20 * 4];         /* (3 + 2) * 4 */
static MGLdistent pFDistEnt[NUM_FOCUS_GROUPS * 12 * 4];         /* ((3 - 1) * 1 + 1) * 4 */
static MGLanidata pFResults[NUM_FOCUS_GROUPS * NUM_FOCUS_FRAMES * 4];         /* */

/* icon */
static MGLanimator				tIAnimator;
static MGLsubsystem				tISubsystem;
static MGLimgselector			tItemImageSelector[NUM_MENU_ITEM];
static MGLimgselitem			tItemImages[NUM_MENU_ITEM][NUM_MENU_ITEM_IMAGES];

static MGL_MENU		g_tMainMenu[NUM_MENU_ITEM] = {
		{  41, 101, "To DO" },
		{ 120, 101, "Contacts" },
		{ 199, 101, "Multimedia" },

		{  41, 146, "Internet" },
		{ 120, 146, "Mailbox" },
		{ 199, 146, "Download" },
	
		{  41, 201, "Schedule" },
		{ 120, 201, "Camera" },
		{ 199, 201, "Settings" },

		{  41, 247, "PDA" },
		{ 120, 247, "Time" },
		{ 199, 247, "Alarm" }
};

static int nStatus;

static int g_nCurMenuIdx = 0;	// 현재 메뉴 Index
static int g_nPrevMenuIdx = 0;	// 이전 메뉴 Index

/**
 * @brief Private Function Prototypes
 */
static int MsgCreate( WND wnd, int msg, int wparam, int lparam );
static int MsgDestroy( WND wnd, int msg, int wparam, int lparam );
static int MsgKeyDown( WND wnd, int key, int option );

static MGLvoid PrvDrawBackground( MGLvoid * pBuffer, MGLvoid * pParams );
static MGLvoid PrvInitRC( WND wnd );
static void SetSlideAnimation (MGLvoid);
static void SetReadyAnimation (MGLvoid);
static void SetIconAnimation (MGLvoid);
static void SetFocusAnimation (MGLvoid);
static void SetFocusPosition (MGLint x, MGLint y);
static void MoveFocus (MGLvoid);

/* PrvDrawBackground {{{ */
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
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Sample5", StrLen("Sample5"), ALIGN_CENTER);
	GdiDrawTextOut((WND)pBuffer, 10, 295, 220, "Cancel", StrLen("Cancel"), ALIGN_RIGHT);

	return;
}
/* }}} */ 

/* PrvInitRC {{{ */
/** 
 * @brief Layer들을 초기화한다. RC와 Layer를 설정한다. 
 */
static MGLvoid PrvInitRC( WND wnd )
{
	g_nCurMenuIdx = 4;

	MGL_SetDCRCs(1, &g_tRC); 
	MGL_SetDCCurRC( &g_tRC );

	MGL_SetRCBounds( 0, 0, LCD_WIDTH, LCD_HEIGHT );
	MGL_SetRCLayers( NUM_LAYERS, g_aLayers );

	// 배경 이미지 : Title, center, softkey 이미지 Drawing Layer
	MGL_SetCurLayerIndex( ID_LAYER_BACK );
	MGL_SetLayerBounds( 0, 0, LCD_WIDTH, LCD_HEIGHT );
	MGL_SetLayerVisible (TRUE) ;
	MGL_SetLayerType( MGL_CUSTOM_LAYER );
	MGL_SetLayerDrawFuncPtr( PrvDrawBackground );

	// 메뉴 슬라이딩 애니메이션 Layer
	MGL_SetCurLayerIndex (ID_LAYER_MENU_SLIDE);
	MGL_SetLayerBounds (0, 54, LCD_WIDTH, LCD_HEIGHT);
	MGL_SetLayerVisible (TRUE) ;
	MGL_SetLayerType (MGL_EXTERNAL_IMAGE_LAYER);
	MGL_SetIP (&g_tSIP, MGL_Alphablending, &g_tSABParams);
	MGL_SetLayerIPs (1, &g_tSIP);

	SetSlideAnimation ();

	// Selected menu item layer
	MGL_SetCurLayerIndex (ID_LAYER_MENU_FOCUS);
	MGL_SetLayerBounds (0, 0, MENU_FOCUS_WIDTH, MENU_FOCUS_HEIGHT);
	MGL_SetLayerVisible (TRUE) ;
	MGL_SetLayerType (MGL_EXTERNAL_IMAGE_LAYER);

	//*
	MGL_SetIP (&g_tFIP[0], MGL_Rotation, &g_tFRotationParams);
	MGL_SetRotationAngle (&g_tFIP[0], 0);
	MGL_SetRotationDirection (&g_tFIP[0], MGL_ROT_DIR_CW);
	MGL_SetRotationDatumFlag (&g_tFIP[0], FALSE);
	MGL_SetRotationDatumDefaultValue (&g_tFIP[0], MGL_ROT_DATUM_CENTER_CENTER);

	MGL_SetIP (&g_tFIP[1], MGL_Scaling, &g_tFScalingParams);
	MGL_SetScalingRatioFlag (&g_tFIP[1], TRUE);
	MGL_SetScalingWidth (&g_tFIP[1], 100);
	MGL_SetScalingHeight (&g_tFIP[1], 100);

	MGL_SetIP (&g_tFIP[2], MGL_Alphablending, &g_tFABParams);
	MGL_SetAlphablendingAlpha (&g_tFIP[2], 80);
	MGL_SetLayerIPs (3, g_tFIP);
	// */
	SetFocusAnimation ();

	// 메뉴 아이템 Layer
	MGL_SetCurLayerIndex (ID_LAYER_MENU_ICON);
	MGL_SetLayerBounds (0, 0, MENU_ICON_WIDTH, MENU_ICON_HEIGHT);
	MGL_SetLayerVisible (TRUE);
	MGL_SetLayerType (MGL_EXTERNAL_IMAGE_LAYER);

	SetIconAnimation ();

	MoveFocus ();
	UsrSetTimer( wnd, TIMER_MGL_SAMPLE_5, TIMER_VAL_SAMPLE_5);
}
/* }}} */ 

/* SetIconAnimation {{{ */
static void 
SetIconAnimation (MGLvoid)
{
	MGLint nImageIdx, nCount;

	/* Setting Animator*/
	MGL_SetAnimator (&tIAnimator, &tISubsystem, 1);
	MGL_AnimatorSetTiming (&tIAnimator, MGL_TIMING_TYPE_FRAME, 0);
	MGL_AnimatorSetRepeatCount (&tIAnimator, 0);

	/* Linking Animator & External Layer */
	MGL_AnimatorSetExternalImageLayer (&tIAnimator, 1);

	nImageIdx = MGL_icon_01_01_I;
	for (nCount = 0; nCount < NUM_MENU_ITEM; nCount++ ) {
		/* Setting ImageSelector */
		MGL_SetImageSelector (&tItemImageSelector[nCount], tItemImages[nCount], 
				NULL, NUM_MENU_ITEM_IMAGES, NULL, 0, MGL_IMGSEL_KEEP_ORDER );

		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx,     0);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx + 1, 1);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx + 2, 2);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx + 3, 3);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx + 3, 4);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx + 3, 5);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx + 3, 6);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx + 2, 7);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx + 1, 8);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx,     9);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx,    10);
		MGL_ImageSelectorSetImage (&tItemImageSelector[nCount], nImageIdx,    11);
		nImageIdx += 4;
	}
}
/* }}} */ 

/* SetSlideAnimation {{{ */
static MGLvoid 
SetSlideAnimation (MGLvoid)
{
	MGLint pPosYs[4];
	MGLint pIntervals[4];
	MGLint pImages[4];

	// set keyframe
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

	/* Item Manager Setting */
	MGL_SetItemManager (&tSItemManager, pSInitVal, 3, NUM_SLIDE_ITEMS, 
						pSItemInitValues, MGL_ITEMMANAGER_OPTION_KEEP_LAST);

	pIntervals[0] = 0;
	pIntervals[1] = 2;
	pIntervals[2] = 2;
	pIntervals[3] = 2;
	MGL_ItemManagerSetStaticInterval (&tSItemManager, 0, pIntervals, NUM_SLIDE_ITEMS);

	/* 선언시에 써놓는게 좋을듯함 ^^ */
	pPosYs[0] = 53;
	pPosYs[1] = 114;
	pPosYs[2] = 167;
	pPosYs[3] = 219;
	MGL_ItemManagerSetStaticInitValue (&tSItemManager, 1, pPosYs, 
						NUM_SLIDE_ITEMS, NULL, MGL_ANI_PARAM_POSITION_Y);

	pImages[0] = MGL_menus_01_I;
	pImages[1] = MGL_menus_02_I;
	pImages[2] = MGL_menus_03_I;
	pImages[3] = MGL_menus_04_I;
	MGL_ItemManagerSetStaticInitValue (&tSItemManager, 2, pImages, 
						NUM_SLIDE_ITEMS, NULL, MGL_ANI_PARAM_IMAGE);

	/* Link KeyFrame & ItemManager */
	MGL_KeyFrameSetInSubsystem (&tSKeyFrame, &(tSItemManager.tSubsystem));

	/* Animator Setting */
	MGL_SetAnimator (&tSAnimator, &tSSubsystem, 1);
	MGL_AnimatorSetTiming (&tSAnimator, MGL_TIMING_TYPE_FRAME, 0);
	MGL_AnimatorSetRepeatCount (&tSAnimator, 1);

	/* Link ItemManager & Animator */
	MGL_ItemManagerSetInSubsystem (&tSItemManager, tSAnimator.pSubsystems);

	/* Link Animator & External Layer */
	MGL_AnimatorSetExternalImageLayer (&tSAnimator, NUM_SLIDE_ITEMS);

	MGL_AnimatorClear (&tSAnimator);

	nStatus = 1;

	return;
}
/* }}} */ 

/* SetReadyAnimation {{{ */
static MGLvoid 
SetReadyAnimation (MGLvoid)
{
	MGLint pPosYs[4];
	MGLint pIntervals[4];
	MGLint pImages[4];

	// set keyframe
	MGL_SetKeyFrame (&tSKeyFrame, NUM_READY_FRAMES, pSInterpolator, 2);

	MGL_KeyFrameSetInterpolator (pSInterpolator + 0, NULL, MGL_ANI_PARAM_POSITION_Y, pSKeyData, 4,
			pSDistEnt, 2, MGL_KEYFRAME_IPF_INT_LINEAR, NULL, NULL, MGL_KEYFRAME_ESF_LINEAR,NULL);

	MGL_KeyFrameSetIntKeyData (pSInterpolator + 0, 0);
	MGL_KeyFrameSetIntKeyData (pSInterpolator + 0, 0);

	MGL_KeyFrameSetInterpolator (pSInterpolator + 1, &g_tSIP, MGL_PARAM_ALPHA, pSKeyData + 4, 4,
			pSDistEnt + 2, 2, MGL_KEYFRAME_IPF_INT_LINEAR, NULL, NULL, MGL_KEYFRAME_ESF_LINEAR,NULL);
	MGL_KeyFrameSetIntKeyData (pSInterpolator + 1, 100);
	MGL_KeyFrameSetIntKeyData (pSInterpolator + 1, 60);

	MGL_KeyFrameStandby (&tSKeyFrame);

	MGL_KeyFramePreCalculate (&tSKeyFrame, pSResults, NUM_READY_FRAMES * 2);

	/* Item Manager Setting */
	MGL_SetItemManager (&tSItemManager, pSInitVal, 3, NUM_SLIDE_ITEMS, 
						pSItemInitValues, MGL_ITEMMANAGER_OPTION_KEEP_LAST);

	pIntervals[0] = 0;
	pIntervals[1] = 0;
	pIntervals[2] = 0;
	pIntervals[3] = 0;
	MGL_ItemManagerSetStaticInterval (&tSItemManager, 0, pIntervals, NUM_SLIDE_ITEMS);

	/* 선언시에 써놓는게 좋을듯함 ^^ */
	pPosYs[0] = 66;
	pPosYs[1] = 127;
	pPosYs[2] = 180;
	pPosYs[3] = 232;
	MGL_ItemManagerSetStaticInitValue (&tSItemManager, 1, pPosYs, 
						NUM_SLIDE_ITEMS, NULL, MGL_ANI_PARAM_POSITION_Y);

	pImages[0] = MGL_menus_01_I;
	pImages[1] = MGL_menus_02_I;
	pImages[2] = MGL_menus_03_I;
	pImages[3] = MGL_menus_04_I;
	MGL_ItemManagerSetStaticInitValue (&tSItemManager, 2, pImages, 
						NUM_SLIDE_ITEMS, NULL, MGL_ANI_PARAM_IMAGE);

	/* Link KeyFrame & ItemManager */
	MGL_KeyFrameSetInSubsystem (&tSKeyFrame, &(tSItemManager.tSubsystem));

	/* Animator Setting */
	MGL_SetAnimator (&tSAnimator, &tSSubsystem, 1);
	MGL_AnimatorSetTiming (&tSAnimator, MGL_TIMING_TYPE_FRAME, 0);
	MGL_AnimatorSetRepeatCount (&tSAnimator, 1);

	/* Link ItemManager & Animator */
	MGL_ItemManagerSetInSubsystem (&tSItemManager, tSAnimator.pSubsystems);

	/* Link Animator & External Layer */
	MGL_AnimatorSetExternalImageLayer (&tSAnimator, NUM_SLIDE_ITEMS);

	MGL_AnimatorClear (&tSAnimator);

	nStatus = 2;

	return;
}
/* }}} */ 

/* SetFocusAnimation {{{ */
#define FOCUS_CENTER_X	10
#define FOCUS_CENTER_Y	10	

static MGLvoid
SetFocusPosition (MGLint x, MGLint y)
{
	MGLint i;
	MGLint pPosXs[5];
	MGLint pPosYs[5];

	for (i = 0; i < NUM_FOCUS_GROUPS; i++) {
		/* Set Position {{{ */ 
		/* 선언시에 써놓는게 좋을듯함 ^^ */
		pPosXs[0] = -10 - FOCUS_CENTER_X + x;
		pPosXs[1] = -5 - FOCUS_CENTER_X + x;
		pPosXs[2] = 0 - FOCUS_CENTER_X + x;
		pPosXs[3] = 7 - FOCUS_CENTER_X + x;
		pPosXs[4] = 14 - FOCUS_CENTER_X + x;
		MGL_ItemManagerSetStaticInitValue (tFItemManager + i, 1, pPosXs, 
				NUM_FOCUS_ITEMS_PER_GROUP, NULL, MGL_ANI_PARAM_POSITION_X);

		pPosYs[0] = -14 - FOCUS_CENTER_Y + y;
		pPosYs[1] = -4 - FOCUS_CENTER_Y + y;
		pPosYs[2] = 0 - FOCUS_CENTER_Y + y;
		pPosYs[3] = 1 - FOCUS_CENTER_Y + y;
		pPosYs[4] = 6 - FOCUS_CENTER_Y + y;
		MGL_ItemManagerSetStaticInitValue (tFItemManager + i, 2, pPosYs, 
				NUM_FOCUS_ITEMS_PER_GROUP, NULL, MGL_ANI_PARAM_POSITION_Y);
		/* }}} */ 
	}
}

static MGLvoid 
SetFocusAnimation (MGLvoid)
{
	MGLint i;
	MGLint pPosXs[5];
	MGLint pPosYs[5];
	/*
	MGLint pAlphas[5];
	MGLint pScales[5];
	MGLint pAngles[5];
	MGLint pIntervals[5];
	*/
	MGLinterpolator *pInterpolator;
	MGLanidata *pKeyData;
	MGLdistent *pDistEnt;

	/* Animator Setting */
	MGL_SetAnimator (&tFAnimator, tFSubsystem, NUM_FOCUS_GROUPS);
	MGL_AnimatorSetTiming (&tFAnimator, MGL_TIMING_TYPE_FRAME, 0);
	MGL_AnimatorSetRepeatCount (&tFAnimator, 0);
	MGL_AnimatorSetDefaultImage (&tFAnimator, MGL_particle_small_I);

	for (i = 0; i < NUM_FOCUS_GROUPS; i++) {
		/* Set Keyframe {{{ */
		pInterpolator = pFInterpolator + i * 4;
		pKeyData = pFKeyData + i * 20;
		pDistEnt = pFDistEnt + i * 12;

		MGL_SetKeyFrame (tFKeyFrame + i, NUM_FOCUS_FRAMES, pInterpolator, 4);

		MGL_KeyFrameSetInterpolator (pInterpolator + 0, NULL, MGL_ANI_PARAM_POSITION, pKeyData, 4,
				pDistEnt, 2, MGL_KEYFRAME_IPF_POINT_ELLIPSE, NULL, NULL, MGL_KEYFRAME_ESF_LINEAR,NULL);

		MGL_KeyFrameSetEllipseKeyData (pInterpolator + 0, 
				MENU_FOCUS_WIDTH / 2, MENU_FOCUS_HEIGHT / 2, FOCUS_RADIUS, FOCUS_RADIUS, 120 + i * 30, 330 + i * 30);

		MGL_KeyFrameSetInterpolator (pInterpolator + 1, g_tFIP + 0, MGL_PARAM_ROTATE_ANGLE, pKeyData + 4, 5,
				pDistEnt + 2, 3, MGL_KEYFRAME_IPF_INT_LINEAR, NULL, NULL, MGL_KEYFRAME_ESF_LINEAR,NULL);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 1, -30);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 1, 30);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 1, -30);

		MGL_KeyFrameSetInterpolator (pInterpolator + 2, g_tFIP + 1, MGL_PARAM_SCALE_SAME_RATIO, pKeyData + 9, 5,
				pDistEnt + 5, 3, MGL_KEYFRAME_IPF_INT_LINEAR, NULL, NULL, MGL_KEYFRAME_ESF_LINEAR,NULL);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 2, 85);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 2, 110);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 2, 90);

		MGL_KeyFrameSetInterpolator (pInterpolator + 3, g_tFIP + 2, MGL_PARAM_ALPHA, pKeyData + 14, 5,
				pDistEnt + 8, 3, MGL_KEYFRAME_IPF_INT_LINEAR, NULL, NULL, MGL_KEYFRAME_ESF_LINEAR,NULL);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 3, 50);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 3, 90);
		MGL_KeyFrameSetIntKeyData (pInterpolator + 3, 10);

		MGL_KeyFrameStandby (tFKeyFrame + i);

		MGL_KeyFramePreCalculate (tFKeyFrame + i, pFResults + i * NUM_FOCUS_FRAMES * 4, NUM_FOCUS_FRAMES * 4);
		/* }}} */ 

		/* Item Manager Setting {{{ */
		MGL_SetItemManager (tFItemManager + i, pFInitVal + i * 6, 6, NUM_FOCUS_ITEMS_PER_GROUP, 
				pFItemInitValues + i * NUM_FOCUS_ITEMS_PER_GROUP * 6, MGL_ITEMMANAGER_OPTION_NONE);

		/*
		pIntervals[0] = 0;
		pIntervals[1] = 0;
		pIntervals[2] = 0;
		pIntervals[3] = 0;
		pIntervals[4] = 0;
		MGL_ItemManagerSetStaticInterval (tFItemManager + i, 0, pIntervals, NUM_FOCUS_ITEMS);
		/*/
		MGL_ItemManagerSetInitInterval (tFItemManager + i, 0, 5, 7);
		//*/

		/* 선언시에 써놓는게 좋을듯함 ^^ */
		pPosXs[0] = 0;
		pPosXs[1] = 0;
		pPosXs[2] = 0;
		pPosXs[3] = 0;
		pPosXs[4] = 0;
		MGL_ItemManagerSetStaticInitValue (tFItemManager + i, 1, pPosXs, 
				NUM_FOCUS_ITEMS_PER_GROUP, NULL, MGL_ANI_PARAM_POSITION_X);

		pPosYs[0] = 0;
		pPosYs[1] = 0;
		pPosYs[2] = 0;
		pPosYs[3] = 0;
		pPosYs[4] = 0;
		MGL_ItemManagerSetStaticInitValue (tFItemManager + i, 2, pPosYs, 
				NUM_FOCUS_ITEMS_PER_GROUP, NULL, MGL_ANI_PARAM_POSITION_Y);

		//*
		MGL_ItemManagerSetInitValue (tFItemManager + i, 3, 0, 360, 1, g_tFIP + 0, 
						MGL_PARAM_ROTATE_ANGLE, MGL_ITEMMANAGER_INITVAL_OPTION_NONE);
		MGL_ItemManagerSetInitValue (tFItemManager + i, 4, -10, 10, 1, g_tFIP + 1, 
						MGL_PARAM_SCALE_SAME_RATIO, MGL_ITEMMANAGER_INITVAL_OPTION_NONE);
		MGL_ItemManagerSetInitValue (tFItemManager + i, 5, -10, 10, 1, g_tFIP + 2, 
						MGL_PARAM_ALPHA, MGL_ITEMMANAGER_INITVAL_OPTION_NONE);
		/*/
		pAngles[0] = 50;
		pAngles[1] = 90;
		pAngles[2] = 120;
		pAngles[3] = -45;
		pAngles[4] = -100;
		MGL_ItemManagerSetStaticInitValue (tFItemManager + i, 3, pAngles, 
				NUM_FOCUS_ITEMS_PER_GROUP, g_tFIP + 0, MGL_PARAM_ROTATE_ANGLE);

		pScales[0] = 5;
		pScales[1] = 3;
		pScales[2] = 0;
		pScales[3] = -5;
		pScales[4] = -3;
		MGL_ItemManagerSetStaticInitValue (tFItemManager + i, 4, pScales, 
				NUM_FOCUS_ITEMS_PER_GROUP, g_tFIP + 1, MGL_PARAM_SCALE_SAME_RATIO);

		pAlphas[0] = 0;
		pAlphas[1] = 0;
		pAlphas[2] = 0;
		pAlphas[3] = 0;
		pAlphas[4] = 0;
		MGL_ItemManagerSetStaticInitValue (tFItemManager + i, 5, pAlphas, 
				NUM_FOCUS_ITEMS_PER_GROUP, g_tFIP + 2, MGL_PARAM_ALPHA);
		//*/

		/* Link KeyFrame & ItemManager */
		MGL_KeyFrameSetInSubsystem (tFKeyFrame + i, &((tFItemManager[i]).tSubsystem));

		/* Link ItemManager & Animator */
		MGL_ItemManagerSetInSubsystem (tFItemManager + i, tFAnimator.pSubsystems + i);
		/* }}} */ 
	}

	/* Link Animator & External Layer */
	MGL_AnimatorSetExternalImageLayer (&tFAnimator, NUM_FOCUS_ITEMS);

	MGL_AnimatorClear (&tFAnimator);

	return;
}
/* }}} */ 

/* MoveFocus {{{ */
static void
MoveFocus (MGLvoid)
{
	MGLint nX, nY;

	//g_nCurMenuIdx 를 이용 
	nX = g_tMainMenu[g_nCurMenuIdx].x - (MENU_ICON_WIDTH / 2);
	nY = g_tMainMenu[g_nCurMenuIdx].y - (MENU_ICON_HEIGHT / 2);

	MGL_SetCurLayerIndex (ID_LAYER_MENU_FOCUS);
	MGL_SetLayerBounds (nX, nY, MENU_ICON_WIDTH, MENU_ICON_HEIGHT);

	MGL_SetCurLayerIndex (ID_LAYER_MENU_ICON);
	MGL_SetLayerBounds (nX, nY, MENU_ICON_WIDTH, MENU_ICON_HEIGHT);

	/* Linking ImageSelector & Animator */
	MGL_ImageSelectorSetInSubsystem (&tItemImageSelector[g_nCurMenuIdx], tIAnimator.pSubsystems);

	MGL_AnimatorClear (&tIAnimator);

	SetFocusPosition (nX, nY); // Item Manager
	MGL_AnimatorClear (&tFAnimator);
}
/* }}} */ 

static int MsgCreate(WND wnd, int msg, int wparam, int lparam)
{
	//CONSOL_Printf("\n+++++++++ MGL Base App MsgCreate() Start+++++++++ \n");	
	
	PrvInitRC( wnd );
	
	//CONSOL_Printf("\nMGL Base APP RC was initialized \n");	
	//CONSOL_Printf("\n+++++++++ MGL Base App MsgCreate() End+++++++++ \n");	

	return 0;
}

static int MsgDestroy(WND wnd, int msg, int wparam, int lparam)
{
	return 0;
}

static int MsgKeyDown( WND wnd, int key, int option )
{
	switch( key ) {
		case VK_USER_RIGHT:
			g_nPrevMenuIdx = g_nCurMenuIdx;
			if( g_nCurMenuIdx == 11 )
				g_nCurMenuIdx = 0;
			else
				g_nCurMenuIdx++;
			MoveFocus ();
			break;

		case VK_USER_LEFT:

			g_nPrevMenuIdx = g_nCurMenuIdx;
			if( g_nCurMenuIdx == 0 )
				g_nCurMenuIdx = 11;
			else
				g_nCurMenuIdx--;
			MoveFocus ();
			break;

		case VK_USER_UP:
			g_nPrevMenuIdx = g_nCurMenuIdx;
			if( g_nCurMenuIdx <= 2 )
				g_nCurMenuIdx += NUM_MENU_ITEM;
			g_nCurMenuIdx -=3;
			MoveFocus ();
			break;

		case VK_USER_DOWN:
			g_nPrevMenuIdx = g_nCurMenuIdx;
			if( g_nCurMenuIdx >= 9 )
				g_nCurMenuIdx -= NUM_MENU_ITEM;
			g_nCurMenuIdx += 3;
			MoveFocus ();
			break;

		case VK_USER_OK:
			StaGotoState( wnd, STATE_ID_MGL_DEMO_IDLE , MSG_DESTROY, MSG_RETURN );		
			break;
	}

	return 0;
}

static int MsgTimer(WND wnd, int msg, int id, int lparam)
{
	static int n = 0;
	if( id == TIMER_MGL_SAMPLE_5) {
		MGL_Render();
		MGL_AnimatorUpdateAll (&tFAnimator);

		if (nStatus != 3) {
			if (MGL_AnimatorUpdateAll (&tSAnimator) == MGL_ERROR_ANIMATION_END) {
				if (nStatus == 1) {
					MGL_SetCurLayerIndex (ID_LAYER_MENU_SLIDE);
					SetReadyAnimation ();
				} else if (nStatus == 2) {
					nStatus = 3;
				}
			}
		}

		if (++n % 5 == 0) {
			n = 0;
			MGL_AnimatorUpdateAll (&tIAnimator);
		}
	}

	return 0;
}

int MGLSample5Proc(WND wnd, int msg, int wparam, int lparam)
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
