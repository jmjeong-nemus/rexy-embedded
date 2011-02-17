/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @brief Blur,Rotate,Scale등의 Engine의 Layer 상의 Image Processing을 다루기 위한 API
 *
 * @file mgl_ip.h
 * @author <a href="mailto:jibong79@mococo.com">Hong JiHyun</a>
 * @author <a href="mailto:hrbae@mococo.com">HyangRan Bae</a>
 */

#ifndef __mgl_ip_h_
#define __mgl_ip_h_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Nemustech Premium Graphic Library
 *
 */

// alpha blending params
#define MGL_PARAM_ALPHA					0x0001	/**<  degree of transparency */

// rotation
#define MGL_PARAM_ROTATE_ANGLE			0x0011	/**<  Angle to rotate */
#define MGL_PARAM_ROTATE_DIRECTION		0x0012	/**<  rotation Direction */
#define MGL_PARAM_ROTATE_DATUM_X			0x0013	/**< Image will be rotated using this x position */
#define MGL_PARAM_ROTATE_DATUM_Y			0x0014	/**< Image will be rotated using this y position */
#define MGL_PARAM_ROTATE_DATUM_FLAG		0x0015	/**<  TRUE : user have to set datum.tUserVal- FALSE : user have to set datum.nDefaultVal */
#define MGL_PARAM_ROTATE_DATUM_DEFUALTVAL 0x0016	/**< MGL_ROT_DATUM_LEFT_TOP ~ MGL_ROT_DATUM_RIGHT_BOTTOM */

// scaling
#define MGL_PARAM_SCALE_RATIO_FLAG		0x0021	/**< TRUE : Scale as Ratio Value  / FALSE : Scale as Pixel Value */ 
#define MGL_PARAM_SCALE_WIDTH			0x0022	/**<  new width ( as Pixel Or Percentage Scale )  */
#define MGL_PARAM_SCALE_HEIGHT			0x0023	/**<  new height ( as Pixel Or Percentage Scale ) */
#define MGL_PARAM_SCALE_SAME_RATIO		0x0024	/**<  width, height 를 같은 Ratio로 적용할 때 사용하는 Param ID */

// blur (0x0031 ~ 0x003f)
#define MGL_PARAM_BLUR_MATRIX_TYPE		0x0031	/**< Blur matrix enum value */
#define MGL_PARAM_BLUR_OBJECT_X			0x0032	/**< Object's X position */
#define MGL_PARAM_BLUR_OBJECT_Y			0x0033	/**< Object's Y position */

// drop shadow (0x0041 ~ 0x004f)
#define MGL_PARAM_DROPSHADOW_SHADOWCOLOR	0x0041	/**< Shadow color */
#define MGL_PARAM_DROPSHADOW_ANGLE			0x0042	/**< Angle between Object and Shadow(degree) */
#define MGL_PARAM_DROPSHADOW_DISTANCE		0x0043	/**< Distance from Object to Shadow(pixel) */
#define MGL_PARAM_DROPSHADOW_SIZE			0x0044	/**< Shadow size(pixel)-it also makes blur density */
#define MGL_PARAM_DROPSHADOW_OPACITY		0x0045	/**< Shadow opacity(%)*/
#define MGL_PARAM_DROPSHADOW_OBJECT_X		0x0046	/**< Object's X position */
#define MGL_PARAM_DROPSHADOW_OBJECT_Y		0x0047	/**< Object's Y position */

// motion blur + Alpha  (0x0051 ~ 0x005f)
#define MGL_PARAM_MBLUR_ALPHA_MATRIX_TYPE		0x0051	/**< Blur matrix enum value */
#define MGL_PARAM_MBLUR_ALPHA_OBJECT_X			0x0052	/**< Object's X position */
#define MGL_PARAM_MBLUR_ALPHA_OBJECT_Y			0x0053	/**< Object's Y position */
#define MGL_PARAM_MBLUR_ALPHA_VAL				0x0054	/**< Degree of transparency */

// Image 필터중 Motionblur를 처리하기 위해 사용되는 matrix type
#define MGL_MATRIX_DUMMY			0	/**< dummy value */
#define MGL_BLUR_MOTION_M45_03		1	/**< angle == 45, matrix size == 3 */
#define MGL_BLUR_MOTION_M45_05		2	/**< angle == 45, matrix size == 5 */
#define MGL_BLUR_MOTION_M45_07		3	/**< angle == 45, matrix size == 7 */
#define MGL_BLUR_MOTION_M45_09		4	/**< angle == 45, matrix size == 9 */
#define MGL_BLUR_MOTION_M45_11		5	/**< angle == 45, matrix size == 11 */
#define MGL_BLUR_MOTION_M135_03		6	/**< angle == 135, matrix size == 3 */
#define MGL_BLUR_MOTION_M135_05		7	/**< angle == 135, matrix size == 5 */
#define MGL_BLUR_MOTION_M135_07		8	/**< angle == 135, matrix size == 7 */
#define MGL_BLUR_MOTION_M135_09		9	/**< angle == 135, matrix size == 9 */
#define MGL_BLUR_MOTION_M135_11		10	/**< angle == 135, matrix size == 11 */
#define MGL_BLUR_MOTION_M0_03		11	/**< angle == 0, matrix size == 3 */
#define MGL_BLUR_MOTION_M0_05		12	/**< angle == 0, matrix size == 5 */
#define MGL_BLUR_MOTION_M0_09		13	/**< angle == 0, matrix size == 9 */
#define MGL_BLUR_MOTION_M9_03		14	/**< angle == 90, matrix size == 3 */
#define MGL_BLUR_MOTION_M90_05		15	/**< angle == 90, matrix size == 5 */
#define MGL_BLUR_MOTION_M90_09		16	/**< angle == 90, matrix size == 9 */
#define MGL_MATRIX_MAX				17	/**< max value for array(filterHeight, filterWidth, filterSum) size */ 

// Bitmap RGB(565)
#define MGL_RGB565_R_VAL	0xf800			/**< R value in 565 bitmap type */
#define MGL_RGB565_G_VAL	0x07e0			/**< G value in 565 bitmap type */
#define MGL_RGB565_B_VAL	0x001f			/**< B value in 565 bitmap type */

#define MGL_GET_R_Value(x)	((((x) & MGL_RGB565_R_VAL) >> 11) & 0x001f)		/**< Get R value */
#define MGL_GET_G_Value(x)	((((x) & MGL_RGB565_G_VAL) >> 5) & 0x003f)		/**< Get G value */
#define MGL_GET_B_Value(x)	((x) & MGL_RGB565_B_VAL)						/**< Get B value */

#define MGL_RGB565(r, g, b)	((((r)<<11) & MGL_RGB565_R_VAL) | (((g)<<5) & MGL_RGB565_G_VAL) | ((b) & MGL_RGB565_B_VAL))	/**< Get color from 565 RGB*/

#define MGL_MAX(a, b)			(((a) > (b)) ? (a) : (b))			/**< Get Maximum value */
#define MGL_MIN(a, b)			(((a) < (b)) ? (a) : (b))			/**< Get Minimum value */

#define MGL_TRNASPARENTCOLOR_NONE	-1		/**< Transparent Color*/

// Max Alpha Value 
#define MGL_MAX_ALPHA_VAL		100			/**< Maximum Alphablending Value*/

// Rotation Direction value 
#define	MGL_ROT_DIR_CW		0				/**< Clock wise direction Rotation*/
#define MGL_ROT_DIR_CCW		1				/**< Anti-Clock wise direction Rotation*/

// Rotation Datum info value
#define MGL_ROT_DATUM_LEFT_TOP		0			/**< Rotation 기준점 - 이미지의 좌상 */
#define MGL_ROT_DATUM_CENTER_TOP	1			/**< Rotation 기준점 - 이미지의 중상 */
#define MGL_ROT_DATUM_RIGHT_TOP		2			/**< Rotation 기준점 - 이미지의 우상 */
#define MGL_ROT_DATUM_LEFT_CENTER	3			/**< Rotation 기준점 - 이미지의 좌중 */
#define MGL_ROT_DATUM_CENTER_CENTER	4			/**< Rotation 기준점 - 이미지의 중심 */
#define MGL_ROT_DATUM_RIGHT_CENTER	5			/**< Rotation 기준점 - 이미지의 우중 */
#define MGL_ROT_DATUM_LEFT_BOTTOM	6			/**< Rotation 기준점 - 이미지의 좌하 */
#define MGL_ROT_DATUM_CENTER_BOTTOM	7			/**< Rotation 기준점 - 이미지의 중하 */
#define MGL_ROT_DATUM_RIGHT_BOTTOM	8			/**< Rotation 기준점 - 이미지의 우하 */

/**
 * @brief Alphablending을 할 때 아래의 구조체의 값을 채운뒤 한다. 
 */
typedef struct MGLalphablendingparams{	
	MGLint nAlpha;							/**< Foreground Image의 배경대비 불투명 수치 ( 0 ~ MGL_MAX_ALPHA_VAL ) */
} MGLalphablendingparams;

/**
 * @brief Scale을 할 때 아래의 구조체의 값을 채운뒤 한다. 
 */
typedef struct MGLscalingparams{
	MGLboolean bRatioFlag;		/**< TRUE : Scale as Ratio Value  / FALSE : Scale as Pixel Value */ 
	MGLint nNewWidth;		/**<  new width ( as Pixel Or Percentage Scale )  */
	MGLint nNewHeight;		/**<  new height ( as Pixel Or Percentage Scale ) */
} MGLscalingparams;

/**
 * @brief pixel Position Information
 */
typedef struct MGLposinfo{
	MGLint x;				/**<  X Position */
	MGLint y;				/**<  Y Position */
}MGLposinfo;

/**
 * @brief Rotation을 할 때 아래의 구조체의 값을 채운뒤 한다. 
 */
typedef struct MGLrotationparams{
	MGLboolean bUserDatum;		/**<  TRUE : user have to set datum.tUserVal- FALSE : user have to set datum.nDefaultVal */
	union{
		MGLposinfo	tUserVal;		/**< MGLposinfo x, y Position */
		MGLint		nDefaultVal;	/**< MGL_ROT_DATUM_LEFT_TOP ~ MGL_ROT_DATUM_RIGHT_BOTTOM */
	}datum;	/**< datum structure */
	
	MGLint nAngle;			/**<  Angle to rotate */
	MGLint nDir;			/**<  Rotation Direction */	
} MGLrotationparams;

/**
 * Motionblur structure
 * @brief Motionblur를 할 때 아래의 구조체의 값을 채운 뒤 한다.
 */
typedef struct MGLblurparams{
	MGLint nMatrixType;			/**< Matrix enum value */
	MGLint nOBJ_X;				/**< object position */
	MGLint nOBJ_Y;				/**< object position */
} MGLblurparams;
 
/**
 * Motionblur structure(applied Alpha blending)
 * @brief Motionblur를 할 때 아래의 구조체의 값을 채운 뒤 한다.
 */
typedef struct MGLbluralphablendedparams{
	MGLint nMatrixType;			/**< Matrix enum value */
	MGLint nOBJ_X;				/**< object position */
	MGLint nOBJ_Y;				/**< object position */
	MGLint nOpacity;			/**< degree of transparency */
} MGLbluralphablendedparams;

/**
 * DropShadow structure
 * @brief DropShadow 할 때 아래의 구조체의 값을 채운 뒤 한다.
 */
typedef struct MGLdropshadowparams{
	MGLcolor shadowColor;		/**< Shadow color */
	MGLint nAngle;				/**< Angle of Light source(Angle between Object and Shadow)*/
	MGLint nDistance;			/**< Distance from Object to Shadow */
	MGLint nSize;				/**< Shadow Size(also Blur weight) */
	MGLint nOpacity;			/**< Shadow Opacity(between shadow and BG(not Object)) */
	MGLint nOBJ_X;				/**< Object Position */
	MGLint nOBJ_Y;				/**< Object Position */
} MGLdropshadowparams; 

						
/**
 * Image Processing API
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] fpImageProcess Image Processing type
 * @param[in] pParams Image process 별로 필요한 추가적인 parameter
 */
MGLvoid MGL_SetIP(MGLip * pIP, MGL_IPFuncPtr fpImageProcess, MGLvoid * pParams);

/**
 * MGL_Alphablending
 *
 * @param[in] type : type에 따라 내부적으로 Alphablending Function의 Action이 정해진다. 
 */
MGLvoid * MGL_Alphablending(MGLint type);

/**
 * MGL_DoAlphablending
 *
 * @param pSrcBuffer Image Proccess 원본 버퍼
 * @param pDestBuffer Image Proccess 출력 버퍼
 * @param nX Image process 영역의 pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param nY Image process 영역의 pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param pWidth Image process 영역의 pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param pHeight Image process 영역의 pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param pParams Image process 별로 필요한 추가적인 parameter. MGLalphablendingparams 구조체의 pointer를 넘긴다.
 */
MGLvoid MGL_DoAlphablending( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * nX, MGLint * nY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams);

/**
 * MGL_SetAlphablendingParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID Scaling 구조체 중 Setting할  Param ID
 * @param[in] pParam nParamID를 가진 Rotation Param 구조체 변수에 세팅할 값 
 */
MGLvoid MGL_SetAlphablendingParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetAlphablendingAlpha
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nAlpha Alphablending 구조체 변수에 저장 할 배경대비 전경 색의 비율 ( 0~ 100 )
 */
MGLvoid MGL_SetAlphablendingAlpha(MGLip * pIP, MGLint nAlpha);

/**
 * MGL_GetAlphablendingBooleanProp
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nPropID 확인 할 Property 의 ID
 * @return TRUE or FALSE
 */
MGLboolean MGL_GetAlphablendingBooleanProp(MGLip * pIP, MGLint nPropID);

/**
 * MGL_GetAlphablendingParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID 확인 할 Property 의 ID
 * @param[out] pnParamType nParamID의 Parameter 변수 Type
 * @param[out] pParam nParamID의 Parameter 변수 Pointer
 */
MGLvoid MGL_GetAlphablendingParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * MGL_GetAlphablendingAlpha
 *
 * @param[in] pIP Image Processing 정보
 * @return Alphablending 배경 대비 전경 비율 값 ( 0 ~ 100 )
 */
MGLint MGL_GetAlphablendingAlpha( MGLip * pIP );

/**
 * MGL_GetAlphablendingPixel
 *
 * @param[in] nBkColor 배경색
 * @param[in] nFgColor 전경색
 * @param[in] nAlpha Alphablending 배경 대비 전경 비율 값 ( 0 ~ 100 )
 * @return 전경색이 nAlpha 비율로 배경색과 혼합된 색상값
 */
MGLcolor MGL_GetAlphablendingPixel( MGLcolor nBkColor, MGLcolor nFgColor, MGLint nAlpha );


// Blur
/**
 * MGL_Motionblur
 *
 * @param[in] type : type에 따라 내부적으로 MotionBlur Function의 Action이 정해진다. 
 */
MGLvoid * MGL_Motionblur( MGLint type );

/**
 * MGL_Motionblur
 *
 * @param[in,out] pSrcBuffer Image Proccess 원본 버퍼
 * @param[in,out] pDestBuffer Image Proccess 출력 버퍼
 * @param[in,out] pX Image process 영역의 x Position value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pY Image process 영역의 y Position value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pWidth Image process 영역의 width value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pHeight Image process 영역의 height value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in] pParams Image process 별로 필요한 추가적인 parameter. MGLblurparams 구조체의 pointer를 넘긴다.
 */
MGLvoid MGL_DoMotionblur( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams );	/** @brief 종류 : Motion blur, Gaussian blur, normal blur */

/**
 * MGL_SetMotionblurParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID MotionBlur 구조체 중 Setting할  Param ID
 * @param[in] pParam nParamID를 가진 MotionBlur Param 구조체 변수에 세팅할 값 
 */
MGLvoid MGL_SetMotionblurParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetMotionblurMatrixType
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] type Matrix's enum value
 */
MGLvoid MGL_SetMotionblurMatrixType(MGLip * pIP, MGLint type);

/**
 * MGL_SetMotionblurObjectX
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] x object's x position
 */
MGLvoid MGL_SetMotionblurObjectX(MGLip * pIP, MGLint x);

/**
 * MGL_SetMotionblurObjectY
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] y object's y position
 */
MGLvoid MGL_SetMotionblurObjectY(MGLip * pIP, MGLint y);

/**
 * MGL_GetMotionblurParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID 확인하고자 하는 Property ID
 * @param[out] pnParamType nParamID의 Parameter 변수 타입
 * @param[out] pParam MotionBlur 구조체 변수 중 nParamID의 Parameter 변수의 포인터
 */
MGLvoid MGL_GetMotionblurParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam); 

/**
 * MGL_GetMotionblurMatrixType
 *
 * @param[in] pIP Image Processing 정보
 * @return Matrix's enum value 
 */
MGLint MGL_GetMotionblurMatrixType(MGLip * pIP);

/**
 * MGL_GetMotionblurObjectX
 *
 * @param[in] pIP Image Processing 정보
 * @return Object's x position 
 */
MGLint MGL_GetMotionblurObjectX(MGLip * pIP);

/**
 * MGL_GetMotionblurObjectY
 *
 * @param[in] pIP Image Processing 정보
 * @return Object's y position 
 */
MGLint MGL_GetMotionblurObjectY(MGLip * pIP);

/**
 * MGL_GetMotionblurBooleanProp
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nPropID 확인하고자 하는 Property ID
 */
MGLboolean MGL_GetMotionblurBooleanProp(MGLip * pIP, MGLint nPropID);




// MotionBlur Alphablended
/**
 * MGL_MotionblurAlpha
 *
 * @param[in] type : type에 따라 내부적으로 MotionBlur Function의 Action이 정해진다. 
 */
MGLvoid * MGL_MotionblurAlpha( MGLint type );

/**
 * MGL_MotionblurAlpha
 *
 * @param[in,out] pSrcBuffer Image Proccess 원본 버퍼
 * @param[in,out] pDestBuffer Image Proccess 출력 버퍼
 * @param[in,out] pX Image process 영역의 x Position value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pY Image process 영역의 y Position value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pWidth Image process 영역의 width value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pHeight Image process 영역의 height value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in] pParams Image process 별로 필요한 추가적인 parameter. MGLblurparams 구조체의 pointer를 넘긴다.
 */
MGLvoid MGL_DoMotionblurAlpha( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams );

/**
 * MGL_SetMotionblurAlphaParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID MotionBlur 구조체 중 Setting할  Param ID
 * @param[in] pParam nParamID를 가진 MotionBlur Param 구조체 변수에 세팅할 값 
 */
MGLvoid MGL_SetMotionblurAlphaParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetMotionblurAlphaMatrixType
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] type Matrix's enum value
 */
MGLvoid MGL_SetMotionblurAlphaMatrixType(MGLip * pIP, MGLint type);

/**
 * MGL_SetMotionblurAlphaObjectX
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] x object's x position
 */
MGLvoid MGL_SetMotionblurAlphaObjectX(MGLip * pIP, MGLint x);

/**
 * MGL_SetMotionblurAlphaObjectY
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] y object's y position
 */
MGLvoid MGL_SetMotionblurAlphaObjectY(MGLip * pIP, MGLint y);

/**
 * MGL_SetMotionblurAlphaOpacity
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] opacity degree of transparency
 */
MGLvoid MGL_SetMotionblurAlphaOpacity(MGLip * pIP, MGLint opacity);

/**
 * MGL_GetMotionblurAlphaParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID 확인하고자 하는 Property ID
 * @param[out] pnParamType nParamID의 Parameter 변수 타입
 * @param[out] pParam MotionBlur 구조체 변수 중 nParamID의 Parameter 변수의 포인터
 */
MGLvoid MGL_GetMotionblurAlphaParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam); 

/**
 * MGL_GetMotionblurAlphaMatrixType
 *
 * @param[in] pIP Image Processing 정보
 * @return Matrix's enum value 
 */
MGLint MGL_GetMotionblurAlphaMatrixType(MGLip * pIP);

/**
 * MGL_GetMotionblurAlphaObjectX
 *
 * @param[in] pIP Image Processing 정보
 * @return Object's x position 
 */
MGLint MGL_GetMotionblurAlphaObjectX(MGLip * pIP);

/**
 * MGL_GetMotionblurAlphaObjectY
 *
 * @param[in] pIP Image Processing 정보
 * @return Object's y position 
 */
MGLint MGL_GetMotionblurAlphaObjectY(MGLip * pIP);

/**
 * MGL_GetMotionblurAlphaOpacity
 *
 * @param[in] pIP Image Processing 정보
 * @return opacity degree of transparency
 */
MGLint MGL_GetMotionblurAlphaOpacity(MGLip * pIP);

/**
 * MGL_GetMotionblurAlphaBooleanProp
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nPropID 확인하고자 하는 Property ID
 */
MGLboolean MGL_GetMotionblurAlphaBooleanProp(MGLip * pIP, MGLint nPropID);



// DropShadow
/**
 * MGL_Dropshadow
 *
 * @param[in] type : type에 따라 내부적으로 Dropshadow Function의 Action이 정해진다. 
 */
MGLvoid * MGL_Dropshadow( MGLint type );

/**
 * MGL_DoDropshadow
 *
 * @param[in,out] pSrcBuffer Image Proccess 원본 버퍼
 * @param[in,out] pDestBuffer Image Proccess 출력 버퍼
 * @param[in,out] pX Image process 영역의 x Position value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pY Image process 영역의 y Position value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pWidth Image process 영역의 width value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pHeight Image process 영역의 height value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in] pParams Image process 별로 필요한 추가적인 parameter. MGLdropshadowparams 구조체의 pointer를 넘긴다.
 */
MGLvoid MGL_DoDropshadow( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth, MGLint * pHeight, MGLvoid * pParams );

/**
 * MGL_SetDropshadowParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID Dropshadow 구조체 중 Setting할  Param ID
 * @param[in] pParam nParamID를 가진 Dropshadow Param 구조체 변수에 세팅할 값 
 */
MGLvoid MGL_SetDropshadowParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetDropshadowShadowColor
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] color Shadow의 color 
 */
MGLvoid MGL_SetDropshadowShadowColor(MGLip * pIP, MGLcolor color);

/**
 * MGL_SetDropshadowAngle
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] angle Shadow angle(범위 : 0~360 (주의) 180도 이상인 경우에는 음수로 표현한다. ex. 315도 = -45도)
 */
MGLvoid MGL_SetDropshadowAngle(MGLip * pIP, MGLint angle);

/**
 * MGL_SetDropshadowDistance
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] distance Shadow distance( Object와 Shadow사이의 거리(pixel)) 
 */
MGLvoid MGL_SetDropshadowDistance(MGLip * pIP, MGLint distance);

/**
 * MGL_SetDropshadowSize
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] size Shadow size (pixel)
 */
MGLvoid MGL_SetDropshadowSize(MGLip * pIP, MGLint size );

/**
 * MGL_SetDropshadowOpacity
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] opacity Shadow opacity(%). (100%에 가까울 수록 이미지 원본에 가깝다.) 
 */
MGLvoid MGL_SetDropshadowOpacity(MGLip * pIP, MGLint opacity );

/**
 * MGL_SetDropshadowObjectX
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] x Object x position 
 */
MGLvoid MGL_SetDropshadowObjectX(MGLip * pIP, MGLint x);

/**
 * MGL_SetDropshadowObjectY
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] y Object y position 
 */
MGLvoid MGL_SetDropshadowObjectY(MGLip * pIP, MGLint y);

/**
 * MGL_GetDropshadowParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID 확인하고자 하는 Property ID
 * @param[out] pnParamType nParamID의 Parameter 변수 타입
 * @param[out] pParam Dropshadow 구조체 변수 중 nParamID의 Parameter 변수의 포인터
 */
MGLvoid MGL_GetDropshadowParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * MGL_GetDropshadowShadowColor
 *
 * @param[in] pIP Image Processing 정보
 * @return Shadow color 
 */
MGLcolor MGL_GetDropshadowShadowColor(MGLip * pIP);

/**
 * MGL_GetDropshadowAngle
 *
 * @param[in] pIP Image Processing 정보
 * @return angle between object and shadow 
 */
MGLint MGL_GetDropshadowAngle(MGLip * pIP);

/**
 * MGL_GetDropshadowDistance
 *
 * @param[in] pIP Image Processing 정보
 * @return distance from object to shadow 
 */
MGLint MGL_GetDropshadowDistance(MGLip * pIP);

/**
 * MGL_GetDropshadowSize
 *
 * @param[in] pIP Image Processing 정보
 * @return Shadow size 
 */
MGLint MGL_GetDropshadowSize(MGLip * pIP);

/**
 * MGL_GetDropshadowOpacity
 *
 * @param[in] pIP Image Processing 정보
 * @return Shadow opacity 
 */
MGLint MGL_GetDropshadowOpacity(MGLip * pIP);

/**
 * MGL_GetDropshadowObjectX
 *
 * @param[in] pIP Image Processing 정보
 * @return object's x position 
 */
MGLint MGL_GetDropshadowObjectX(MGLip * pIP);

/**
 * MGL_GetDropshadowObjectY
 *
 * @param[in] pIP Image Processing 정보
 * @return object's y position 
 */
MGLint MGL_GetDropshadowObjectY(MGLip * pIP);

/**
 * MGL_GetDropshadowBooleanProp
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nPropID 확인하고자 하는 Property ID
 */
MGLboolean MGL_GetDropshadowBooleanProp(MGLip * pIP, MGLint nPropID);



// Rotation
/**
 * MGL_Rotation
 *
 * @param[in] type : type에 따라 내부적으로 Rotation Function의 Action이 정해진다. 
 */
MGLvoid * MGL_Rotation(MGLint type);

/**
 * MGL_DoRotation
 *
 * @param[in,out] pSrcBuffer Image Proccess 원본 버퍼
 * @param[in,out] pDestBuffer Image Proccess 출력 버퍼
 * @param[in,out] nDestX Image process 영역의 x Position value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] nDestY Image process 영역의 y Position value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pWidth Image process 영역의 width value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pHeight Image process 영역의 height value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in] pParams Image process 별로 필요한 추가적인 parameter. MGLrotationparams 구조체의 pointer를 넘긴다.
 */
MGLvoid MGL_DoRotation( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * nDestX, MGLint * nDestY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams);

/**
 * MGL_SetRotationParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID Rotation 구조체 중 Setting할  Param ID
 * @param[in] pParam nParamID를 가진 Rotation Param 구조체 변수에 세팅할 값 
 */
MGLvoid MGL_SetRotationParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetRotationAngle
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nAngle Rotation 각도 
 */
MGLvoid MGL_SetRotationAngle(MGLip * pIP, MGLint nAngle );

/**
 * MGL_SetRotationDirection
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nDir Rotation 방향 MGL_ROT_DIR_CW | MGL_ROT_DIR_CCW
 */
MGLvoid MGL_SetRotationDirection(MGLip * pIP, MGLint nDir );

/**
 * MGL_SetRotationDatumX
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nDatumX Rotation 기준점 x 좌표
 */
MGLvoid MGL_SetRotationDatumX(MGLip * pIP, MGLint nDatumX );

/**
 * MGL_SetRotationDatumY
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nDatumY Rotation 기준점 y 좌표
 */
MGLvoid MGL_SetRotationDatumY(MGLip * pIP, MGLint nDatumY );

/**
 * MGL_SetRotationDatumFlag
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] bUserDatum User가 정한 기준점을 사용할 경우 TRUE otherwise FALSE
 */
MGLvoid MGL_SetRotationDatumFlag(MGLip * pIP, MGLboolean bUserDatum );

/**
 * MGL_SetRotationDatumDefaultValue
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nDfaultVal Rotation 중심점 좌표를 Rotation 시 내부적으로 계산해준다. MGL_ROT_DATUM_LEFT_TOP ~ MGL_ROT_DATUM_RIGHT_BOTTOM
 */
MGLvoid MGL_SetRotationDatumDefaultValue(MGLip * pIP, MGLint nDfaultVal );

/**
 * MGL_GetRotationBooleanProp
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nPropID 확인하고자 하는 Property ID
 */
MGLboolean MGL_GetRotationBooleanProp(MGLip * pIP, MGLint nPropID);

/**
 * MGL_GetRotationParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID 확인하고자 하는 Property ID
 * @param[out] pnParamType nParamID의 Parameter 변수 타입
 * @param[out] pParam Rotation 구조체 변수 중 nParamID의 Parameter 변수의 포인터
 */
MGLvoid MGL_GetRotationParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * MGL_GetRotationAngle
 *
 * @param[in] pIP Image Processing 정보 
 * @return Rotation 회전 각도 ( 0 ~ 360 )
 */
MGLint MGL_GetRotationAngle(MGLip * pIP );

/**
 * MGL_GetRotationDirection
 *
 * @param[in] pIP Image Processing 정보
 * @return Rotation 방향 MGL_ROT_DIR_CW | MGL_ROT_DIR_CCW
 */
MGLint MGL_GetRotationDirection(MGLip * pIP );

/**
 * MGL_GetRotationDatumX
 *
 * @param[in] pIP Image Processing 정보
 * @return Rotation 기준점 x Position
 */
MGLint MGL_GetRotationDatumX( MGLip * pIP );

/**
 * MGL_GetRotationDatumY
 *
 * @param[in] pIP Image Processing 정보
 * @return Rotation 기준점 y Position
 */
MGLint MGL_GetRotationDatumY( MGLip * pIP );

/**
 * MGL_GetRotationDatumFlag
 *
 * @param[in] pIP Image Processing 정보
 * @return TRUE Rotation 기준점 x,y를 User가 준 경우 / FALSE Rotation 기준점을 Rotation 내부적으로 계산하도록 한 경우 
 */
MGLboolean MGL_GetRotationDatumFlag( MGLip * pIP );

/**
 * MGL_GetRotationDatumDefaultValue
 *
 * @param[in] pIP Image Processing 정보
 * @return Rotation 기준점 정보 MGL_ROT_DATUM_LEFT_TOP ~ MGL_ROT_DATUM_RIGHT_BOTTOM
 */
MGLint MGL_GetRotationDatumDefaultValue( MGLip * pIP );


// Scaling
/**
 * MGL_Rotation
 *
 * @param[in] type : type에 따라 내부적으로 Scaling Function의 Action이 정해진다. 
 */
MGLvoid * MGL_Scaling(MGLint type);

/**
 * MGL_DoScaling
 *
 * @param[in,out] pSrcBuffer Image Proccess 원본 버퍼
 * @param[in,out] pDestBuffer Image Proccess 출력 버퍼
 * @param[in,out] pX Image process 영역의 x Position value pointer. 영역이 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pY Image process 영역의 y Position value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pWidth Image process 영역의 width value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in,out] pHeight Image process 영역의 height value pointer. 영역가 변경될 수 있기 때문에 pointer를 넘긴다.
 * @param[in] pParams Image process 별로 필요한 추가적인 parameter. MGLscalingparams 구조체의 pointer를 넘긴다.
 */
MGLvoid MGL_DoScaling( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams);

/**
 * MGL_SetScalingParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID Scaling 구조체 중 Setting할  Param ID
 * @param[in] pParam nParamID를 가진 Scaling Param 구조체 변수에 세팅할 값 
 */
MGLvoid MGL_SetScalingParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetScalingRatioFlag
 * 
 * @brief Scale을 적용할 때 width/height 정보를 비율로 적용할지 Pixel값으로 적용할 지 세팅하는 함수
 * @param[in] pIP Image Processing 정보
 * @param[in] bRatio TRUE Scale할 width, height 값을 비율로 세팅할 경우 / FALSE Scale할 width, height 값을 pixel로 세팅할 경우
 */
MGLvoid MGL_SetScalingRatioFlag(MGLip * pIP, MGLboolean bRatio );

/**
 * MGL_SetScalingWidth
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nNewWidth Scale할 값 비율로 width를 세팅할 경우 0 ~ 1000 or pixel 단위 width 값
 */
MGLvoid MGL_SetScalingWidth(MGLip * pIP, MGLint nNewWidth );

/**
 * MGL_SetScalingHeight
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nNewHeight Scale할 값 비율로 Height을 세팅할 경우 0 ~ 1000 or pixel 단위 width 값
 */
MGLvoid MGL_SetScalingHeight(MGLip * pIP, MGLint nNewHeight );

/**
 * MGL_SetScalingSameRatio
 * @brief 가로 세로 비율을 동일하게 적용해서 Scale 할 경우 사용하는 함수
 * @param[in] pIP Image Processing 정보
 * @param[in] nNewRatio width,height Scaling 비율
 */
MGLvoid MGL_SetScalingSameRatio( MGLip * pIP, MGLint nNewRatio );

/**
 * MGL_GetScalingBooleanProp
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nPropID 확인하고자 하는 Property ID
 */
MGLboolean MGL_GetScalingBooleanProp(MGLip * pIP, MGLint nPropID);

/**
 * MGL_GetScalingParam
 *
 * @param[in] pIP Image Processing 정보
 * @param[in] nParamID 확인하고자 하는 Property ID
 * @param[out] pnParamType nParamID의 Parameter 변수 타입
 * @param[out] pParam Rotation 구조체 변수 중 nParamID의 Parameter 변수의 포인터
 */
MGLvoid MGL_GetScalingParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * MGL_GetScalingRatioFlag
 * @brief 현재 Scale 적용이 비율로 되었는지 Pixel값으로 되었는지를 나타내는 Flag 값을 얻는 함수
 * @param[in] pIP Image Processing 정보
 * @return TRUE Scale할 width,height 값이 비율로 세팅된 경우/ FALSE pixel값을로 세팅된 경우 
 */
MGLboolean MGL_GetScalingRatioFlag(MGLip * pIP );

/**
 * MGL_GetScalingWidth
 *
 * @param[in] pIP Image Processing 정보
 * @return Scale할 width 값 ( 비율 혹은 Pixel ) 
 */
MGLint MGL_GetScalingWidth(MGLip * pIP );

/**
 * MGL_GetScalingHeight
 *
 * @param[in] pIP Image Processing 정보
 * @return Scale할 Height 값 ( 비율 혹은 Pixel ) 
 */
MGLint MGL_GetScalingHeight(MGLip * pIP );



#ifdef __cplusplus
}
#endif

#endif // __mgl_ip_h_
