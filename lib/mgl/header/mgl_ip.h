/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @brief Blur,Rotate,Scale���� Engine�� Layer ���� Image Processing�� �ٷ�� ���� API
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
#define MGL_PARAM_SCALE_SAME_RATIO		0x0024	/**<  width, height �� ���� Ratio�� ������ �� ����ϴ� Param ID */

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

// Image ������ Motionblur�� ó���ϱ� ���� ���Ǵ� matrix type
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
#define MGL_ROT_DATUM_LEFT_TOP		0			/**< Rotation ������ - �̹����� �»� */
#define MGL_ROT_DATUM_CENTER_TOP	1			/**< Rotation ������ - �̹����� �߻� */
#define MGL_ROT_DATUM_RIGHT_TOP		2			/**< Rotation ������ - �̹����� ��� */
#define MGL_ROT_DATUM_LEFT_CENTER	3			/**< Rotation ������ - �̹����� ���� */
#define MGL_ROT_DATUM_CENTER_CENTER	4			/**< Rotation ������ - �̹����� �߽� */
#define MGL_ROT_DATUM_RIGHT_CENTER	5			/**< Rotation ������ - �̹����� ���� */
#define MGL_ROT_DATUM_LEFT_BOTTOM	6			/**< Rotation ������ - �̹����� ���� */
#define MGL_ROT_DATUM_CENTER_BOTTOM	7			/**< Rotation ������ - �̹����� ���� */
#define MGL_ROT_DATUM_RIGHT_BOTTOM	8			/**< Rotation ������ - �̹����� ���� */

/**
 * @brief Alphablending�� �� �� �Ʒ��� ����ü�� ���� ä��� �Ѵ�. 
 */
typedef struct MGLalphablendingparams{	
	MGLint nAlpha;							/**< Foreground Image�� ����� ������ ��ġ ( 0 ~ MGL_MAX_ALPHA_VAL ) */
} MGLalphablendingparams;

/**
 * @brief Scale�� �� �� �Ʒ��� ����ü�� ���� ä��� �Ѵ�. 
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
 * @brief Rotation�� �� �� �Ʒ��� ����ü�� ���� ä��� �Ѵ�. 
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
 * @brief Motionblur�� �� �� �Ʒ��� ����ü�� ���� ä�� �� �Ѵ�.
 */
typedef struct MGLblurparams{
	MGLint nMatrixType;			/**< Matrix enum value */
	MGLint nOBJ_X;				/**< object position */
	MGLint nOBJ_Y;				/**< object position */
} MGLblurparams;
 
/**
 * Motionblur structure(applied Alpha blending)
 * @brief Motionblur�� �� �� �Ʒ��� ����ü�� ���� ä�� �� �Ѵ�.
 */
typedef struct MGLbluralphablendedparams{
	MGLint nMatrixType;			/**< Matrix enum value */
	MGLint nOBJ_X;				/**< object position */
	MGLint nOBJ_Y;				/**< object position */
	MGLint nOpacity;			/**< degree of transparency */
} MGLbluralphablendedparams;

/**
 * DropShadow structure
 * @brief DropShadow �� �� �Ʒ��� ����ü�� ���� ä�� �� �Ѵ�.
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
 * @param[in] pIP Image Processing ����
 * @param[in] fpImageProcess Image Processing type
 * @param[in] pParams Image process ���� �ʿ��� �߰����� parameter
 */
MGLvoid MGL_SetIP(MGLip * pIP, MGL_IPFuncPtr fpImageProcess, MGLvoid * pParams);

/**
 * MGL_Alphablending
 *
 * @param[in] type : type�� ���� ���������� Alphablending Function�� Action�� ��������. 
 */
MGLvoid * MGL_Alphablending(MGLint type);

/**
 * MGL_DoAlphablending
 *
 * @param pSrcBuffer Image Proccess ���� ����
 * @param pDestBuffer Image Proccess ��� ����
 * @param nX Image process ������ pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param nY Image process ������ pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param pWidth Image process ������ pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param pHeight Image process ������ pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param pParams Image process ���� �ʿ��� �߰����� parameter. MGLalphablendingparams ����ü�� pointer�� �ѱ��.
 */
MGLvoid MGL_DoAlphablending( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * nX, MGLint * nY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams);

/**
 * MGL_SetAlphablendingParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Scaling ����ü �� Setting��  Param ID
 * @param[in] pParam nParamID�� ���� Rotation Param ����ü ������ ������ �� 
 */
MGLvoid MGL_SetAlphablendingParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetAlphablendingAlpha
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nAlpha Alphablending ����ü ������ ���� �� ����� ���� ���� ���� ( 0~ 100 )
 */
MGLvoid MGL_SetAlphablendingAlpha(MGLip * pIP, MGLint nAlpha);

/**
 * MGL_GetAlphablendingBooleanProp
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nPropID Ȯ�� �� Property �� ID
 * @return TRUE or FALSE
 */
MGLboolean MGL_GetAlphablendingBooleanProp(MGLip * pIP, MGLint nPropID);

/**
 * MGL_GetAlphablendingParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Ȯ�� �� Property �� ID
 * @param[out] pnParamType nParamID�� Parameter ���� Type
 * @param[out] pParam nParamID�� Parameter ���� Pointer
 */
MGLvoid MGL_GetAlphablendingParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * MGL_GetAlphablendingAlpha
 *
 * @param[in] pIP Image Processing ����
 * @return Alphablending ��� ��� ���� ���� �� ( 0 ~ 100 )
 */
MGLint MGL_GetAlphablendingAlpha( MGLip * pIP );

/**
 * MGL_GetAlphablendingPixel
 *
 * @param[in] nBkColor ����
 * @param[in] nFgColor �����
 * @param[in] nAlpha Alphablending ��� ��� ���� ���� �� ( 0 ~ 100 )
 * @return ������� nAlpha ������ ������ ȥ�յ� ����
 */
MGLcolor MGL_GetAlphablendingPixel( MGLcolor nBkColor, MGLcolor nFgColor, MGLint nAlpha );


// Blur
/**
 * MGL_Motionblur
 *
 * @param[in] type : type�� ���� ���������� MotionBlur Function�� Action�� ��������. 
 */
MGLvoid * MGL_Motionblur( MGLint type );

/**
 * MGL_Motionblur
 *
 * @param[in,out] pSrcBuffer Image Proccess ���� ����
 * @param[in,out] pDestBuffer Image Proccess ��� ����
 * @param[in,out] pX Image process ������ x Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pY Image process ������ y Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pWidth Image process ������ width value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pHeight Image process ������ height value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in] pParams Image process ���� �ʿ��� �߰����� parameter. MGLblurparams ����ü�� pointer�� �ѱ��.
 */
MGLvoid MGL_DoMotionblur( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams );	/** @brief ���� : Motion blur, Gaussian blur, normal blur */

/**
 * MGL_SetMotionblurParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID MotionBlur ����ü �� Setting��  Param ID
 * @param[in] pParam nParamID�� ���� MotionBlur Param ����ü ������ ������ �� 
 */
MGLvoid MGL_SetMotionblurParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetMotionblurMatrixType
 *
 * @param[in] pIP Image Processing ����
 * @param[in] type Matrix's enum value
 */
MGLvoid MGL_SetMotionblurMatrixType(MGLip * pIP, MGLint type);

/**
 * MGL_SetMotionblurObjectX
 *
 * @param[in] pIP Image Processing ����
 * @param[in] x object's x position
 */
MGLvoid MGL_SetMotionblurObjectX(MGLip * pIP, MGLint x);

/**
 * MGL_SetMotionblurObjectY
 *
 * @param[in] pIP Image Processing ����
 * @param[in] y object's y position
 */
MGLvoid MGL_SetMotionblurObjectY(MGLip * pIP, MGLint y);

/**
 * MGL_GetMotionblurParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Ȯ���ϰ��� �ϴ� Property ID
 * @param[out] pnParamType nParamID�� Parameter ���� Ÿ��
 * @param[out] pParam MotionBlur ����ü ���� �� nParamID�� Parameter ������ ������
 */
MGLvoid MGL_GetMotionblurParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam); 

/**
 * MGL_GetMotionblurMatrixType
 *
 * @param[in] pIP Image Processing ����
 * @return Matrix's enum value 
 */
MGLint MGL_GetMotionblurMatrixType(MGLip * pIP);

/**
 * MGL_GetMotionblurObjectX
 *
 * @param[in] pIP Image Processing ����
 * @return Object's x position 
 */
MGLint MGL_GetMotionblurObjectX(MGLip * pIP);

/**
 * MGL_GetMotionblurObjectY
 *
 * @param[in] pIP Image Processing ����
 * @return Object's y position 
 */
MGLint MGL_GetMotionblurObjectY(MGLip * pIP);

/**
 * MGL_GetMotionblurBooleanProp
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nPropID Ȯ���ϰ��� �ϴ� Property ID
 */
MGLboolean MGL_GetMotionblurBooleanProp(MGLip * pIP, MGLint nPropID);




// MotionBlur Alphablended
/**
 * MGL_MotionblurAlpha
 *
 * @param[in] type : type�� ���� ���������� MotionBlur Function�� Action�� ��������. 
 */
MGLvoid * MGL_MotionblurAlpha( MGLint type );

/**
 * MGL_MotionblurAlpha
 *
 * @param[in,out] pSrcBuffer Image Proccess ���� ����
 * @param[in,out] pDestBuffer Image Proccess ��� ����
 * @param[in,out] pX Image process ������ x Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pY Image process ������ y Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pWidth Image process ������ width value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pHeight Image process ������ height value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in] pParams Image process ���� �ʿ��� �߰����� parameter. MGLblurparams ����ü�� pointer�� �ѱ��.
 */
MGLvoid MGL_DoMotionblurAlpha( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams );

/**
 * MGL_SetMotionblurAlphaParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID MotionBlur ����ü �� Setting��  Param ID
 * @param[in] pParam nParamID�� ���� MotionBlur Param ����ü ������ ������ �� 
 */
MGLvoid MGL_SetMotionblurAlphaParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetMotionblurAlphaMatrixType
 *
 * @param[in] pIP Image Processing ����
 * @param[in] type Matrix's enum value
 */
MGLvoid MGL_SetMotionblurAlphaMatrixType(MGLip * pIP, MGLint type);

/**
 * MGL_SetMotionblurAlphaObjectX
 *
 * @param[in] pIP Image Processing ����
 * @param[in] x object's x position
 */
MGLvoid MGL_SetMotionblurAlphaObjectX(MGLip * pIP, MGLint x);

/**
 * MGL_SetMotionblurAlphaObjectY
 *
 * @param[in] pIP Image Processing ����
 * @param[in] y object's y position
 */
MGLvoid MGL_SetMotionblurAlphaObjectY(MGLip * pIP, MGLint y);

/**
 * MGL_SetMotionblurAlphaOpacity
 *
 * @param[in] pIP Image Processing ����
 * @param[in] opacity degree of transparency
 */
MGLvoid MGL_SetMotionblurAlphaOpacity(MGLip * pIP, MGLint opacity);

/**
 * MGL_GetMotionblurAlphaParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Ȯ���ϰ��� �ϴ� Property ID
 * @param[out] pnParamType nParamID�� Parameter ���� Ÿ��
 * @param[out] pParam MotionBlur ����ü ���� �� nParamID�� Parameter ������ ������
 */
MGLvoid MGL_GetMotionblurAlphaParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam); 

/**
 * MGL_GetMotionblurAlphaMatrixType
 *
 * @param[in] pIP Image Processing ����
 * @return Matrix's enum value 
 */
MGLint MGL_GetMotionblurAlphaMatrixType(MGLip * pIP);

/**
 * MGL_GetMotionblurAlphaObjectX
 *
 * @param[in] pIP Image Processing ����
 * @return Object's x position 
 */
MGLint MGL_GetMotionblurAlphaObjectX(MGLip * pIP);

/**
 * MGL_GetMotionblurAlphaObjectY
 *
 * @param[in] pIP Image Processing ����
 * @return Object's y position 
 */
MGLint MGL_GetMotionblurAlphaObjectY(MGLip * pIP);

/**
 * MGL_GetMotionblurAlphaOpacity
 *
 * @param[in] pIP Image Processing ����
 * @return opacity degree of transparency
 */
MGLint MGL_GetMotionblurAlphaOpacity(MGLip * pIP);

/**
 * MGL_GetMotionblurAlphaBooleanProp
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nPropID Ȯ���ϰ��� �ϴ� Property ID
 */
MGLboolean MGL_GetMotionblurAlphaBooleanProp(MGLip * pIP, MGLint nPropID);



// DropShadow
/**
 * MGL_Dropshadow
 *
 * @param[in] type : type�� ���� ���������� Dropshadow Function�� Action�� ��������. 
 */
MGLvoid * MGL_Dropshadow( MGLint type );

/**
 * MGL_DoDropshadow
 *
 * @param[in,out] pSrcBuffer Image Proccess ���� ����
 * @param[in,out] pDestBuffer Image Proccess ��� ����
 * @param[in,out] pX Image process ������ x Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pY Image process ������ y Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pWidth Image process ������ width value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pHeight Image process ������ height value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in] pParams Image process ���� �ʿ��� �߰����� parameter. MGLdropshadowparams ����ü�� pointer�� �ѱ��.
 */
MGLvoid MGL_DoDropshadow( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth, MGLint * pHeight, MGLvoid * pParams );

/**
 * MGL_SetDropshadowParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Dropshadow ����ü �� Setting��  Param ID
 * @param[in] pParam nParamID�� ���� Dropshadow Param ����ü ������ ������ �� 
 */
MGLvoid MGL_SetDropshadowParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetDropshadowShadowColor
 *
 * @param[in] pIP Image Processing ����
 * @param[in] color Shadow�� color 
 */
MGLvoid MGL_SetDropshadowShadowColor(MGLip * pIP, MGLcolor color);

/**
 * MGL_SetDropshadowAngle
 *
 * @param[in] pIP Image Processing ����
 * @param[in] angle Shadow angle(���� : 0~360 (����) 180�� �̻��� ��쿡�� ������ ǥ���Ѵ�. ex. 315�� = -45��)
 */
MGLvoid MGL_SetDropshadowAngle(MGLip * pIP, MGLint angle);

/**
 * MGL_SetDropshadowDistance
 *
 * @param[in] pIP Image Processing ����
 * @param[in] distance Shadow distance( Object�� Shadow������ �Ÿ�(pixel)) 
 */
MGLvoid MGL_SetDropshadowDistance(MGLip * pIP, MGLint distance);

/**
 * MGL_SetDropshadowSize
 *
 * @param[in] pIP Image Processing ����
 * @param[in] size Shadow size (pixel)
 */
MGLvoid MGL_SetDropshadowSize(MGLip * pIP, MGLint size );

/**
 * MGL_SetDropshadowOpacity
 *
 * @param[in] pIP Image Processing ����
 * @param[in] opacity Shadow opacity(%). (100%�� ����� ���� �̹��� ������ ������.) 
 */
MGLvoid MGL_SetDropshadowOpacity(MGLip * pIP, MGLint opacity );

/**
 * MGL_SetDropshadowObjectX
 *
 * @param[in] pIP Image Processing ����
 * @param[in] x Object x position 
 */
MGLvoid MGL_SetDropshadowObjectX(MGLip * pIP, MGLint x);

/**
 * MGL_SetDropshadowObjectY
 *
 * @param[in] pIP Image Processing ����
 * @param[in] y Object y position 
 */
MGLvoid MGL_SetDropshadowObjectY(MGLip * pIP, MGLint y);

/**
 * MGL_GetDropshadowParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Ȯ���ϰ��� �ϴ� Property ID
 * @param[out] pnParamType nParamID�� Parameter ���� Ÿ��
 * @param[out] pParam Dropshadow ����ü ���� �� nParamID�� Parameter ������ ������
 */
MGLvoid MGL_GetDropshadowParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * MGL_GetDropshadowShadowColor
 *
 * @param[in] pIP Image Processing ����
 * @return Shadow color 
 */
MGLcolor MGL_GetDropshadowShadowColor(MGLip * pIP);

/**
 * MGL_GetDropshadowAngle
 *
 * @param[in] pIP Image Processing ����
 * @return angle between object and shadow 
 */
MGLint MGL_GetDropshadowAngle(MGLip * pIP);

/**
 * MGL_GetDropshadowDistance
 *
 * @param[in] pIP Image Processing ����
 * @return distance from object to shadow 
 */
MGLint MGL_GetDropshadowDistance(MGLip * pIP);

/**
 * MGL_GetDropshadowSize
 *
 * @param[in] pIP Image Processing ����
 * @return Shadow size 
 */
MGLint MGL_GetDropshadowSize(MGLip * pIP);

/**
 * MGL_GetDropshadowOpacity
 *
 * @param[in] pIP Image Processing ����
 * @return Shadow opacity 
 */
MGLint MGL_GetDropshadowOpacity(MGLip * pIP);

/**
 * MGL_GetDropshadowObjectX
 *
 * @param[in] pIP Image Processing ����
 * @return object's x position 
 */
MGLint MGL_GetDropshadowObjectX(MGLip * pIP);

/**
 * MGL_GetDropshadowObjectY
 *
 * @param[in] pIP Image Processing ����
 * @return object's y position 
 */
MGLint MGL_GetDropshadowObjectY(MGLip * pIP);

/**
 * MGL_GetDropshadowBooleanProp
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nPropID Ȯ���ϰ��� �ϴ� Property ID
 */
MGLboolean MGL_GetDropshadowBooleanProp(MGLip * pIP, MGLint nPropID);



// Rotation
/**
 * MGL_Rotation
 *
 * @param[in] type : type�� ���� ���������� Rotation Function�� Action�� ��������. 
 */
MGLvoid * MGL_Rotation(MGLint type);

/**
 * MGL_DoRotation
 *
 * @param[in,out] pSrcBuffer Image Proccess ���� ����
 * @param[in,out] pDestBuffer Image Proccess ��� ����
 * @param[in,out] nDestX Image process ������ x Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] nDestY Image process ������ y Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pWidth Image process ������ width value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pHeight Image process ������ height value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in] pParams Image process ���� �ʿ��� �߰����� parameter. MGLrotationparams ����ü�� pointer�� �ѱ��.
 */
MGLvoid MGL_DoRotation( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * nDestX, MGLint * nDestY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams);

/**
 * MGL_SetRotationParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Rotation ����ü �� Setting��  Param ID
 * @param[in] pParam nParamID�� ���� Rotation Param ����ü ������ ������ �� 
 */
MGLvoid MGL_SetRotationParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetRotationAngle
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nAngle Rotation ���� 
 */
MGLvoid MGL_SetRotationAngle(MGLip * pIP, MGLint nAngle );

/**
 * MGL_SetRotationDirection
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nDir Rotation ���� MGL_ROT_DIR_CW | MGL_ROT_DIR_CCW
 */
MGLvoid MGL_SetRotationDirection(MGLip * pIP, MGLint nDir );

/**
 * MGL_SetRotationDatumX
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nDatumX Rotation ������ x ��ǥ
 */
MGLvoid MGL_SetRotationDatumX(MGLip * pIP, MGLint nDatumX );

/**
 * MGL_SetRotationDatumY
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nDatumY Rotation ������ y ��ǥ
 */
MGLvoid MGL_SetRotationDatumY(MGLip * pIP, MGLint nDatumY );

/**
 * MGL_SetRotationDatumFlag
 *
 * @param[in] pIP Image Processing ����
 * @param[in] bUserDatum User�� ���� �������� ����� ��� TRUE otherwise FALSE
 */
MGLvoid MGL_SetRotationDatumFlag(MGLip * pIP, MGLboolean bUserDatum );

/**
 * MGL_SetRotationDatumDefaultValue
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nDfaultVal Rotation �߽��� ��ǥ�� Rotation �� ���������� ������ش�. MGL_ROT_DATUM_LEFT_TOP ~ MGL_ROT_DATUM_RIGHT_BOTTOM
 */
MGLvoid MGL_SetRotationDatumDefaultValue(MGLip * pIP, MGLint nDfaultVal );

/**
 * MGL_GetRotationBooleanProp
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nPropID Ȯ���ϰ��� �ϴ� Property ID
 */
MGLboolean MGL_GetRotationBooleanProp(MGLip * pIP, MGLint nPropID);

/**
 * MGL_GetRotationParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Ȯ���ϰ��� �ϴ� Property ID
 * @param[out] pnParamType nParamID�� Parameter ���� Ÿ��
 * @param[out] pParam Rotation ����ü ���� �� nParamID�� Parameter ������ ������
 */
MGLvoid MGL_GetRotationParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * MGL_GetRotationAngle
 *
 * @param[in] pIP Image Processing ���� 
 * @return Rotation ȸ�� ���� ( 0 ~ 360 )
 */
MGLint MGL_GetRotationAngle(MGLip * pIP );

/**
 * MGL_GetRotationDirection
 *
 * @param[in] pIP Image Processing ����
 * @return Rotation ���� MGL_ROT_DIR_CW | MGL_ROT_DIR_CCW
 */
MGLint MGL_GetRotationDirection(MGLip * pIP );

/**
 * MGL_GetRotationDatumX
 *
 * @param[in] pIP Image Processing ����
 * @return Rotation ������ x Position
 */
MGLint MGL_GetRotationDatumX( MGLip * pIP );

/**
 * MGL_GetRotationDatumY
 *
 * @param[in] pIP Image Processing ����
 * @return Rotation ������ y Position
 */
MGLint MGL_GetRotationDatumY( MGLip * pIP );

/**
 * MGL_GetRotationDatumFlag
 *
 * @param[in] pIP Image Processing ����
 * @return TRUE Rotation ������ x,y�� User�� �� ��� / FALSE Rotation �������� Rotation ���������� ����ϵ��� �� ��� 
 */
MGLboolean MGL_GetRotationDatumFlag( MGLip * pIP );

/**
 * MGL_GetRotationDatumDefaultValue
 *
 * @param[in] pIP Image Processing ����
 * @return Rotation ������ ���� MGL_ROT_DATUM_LEFT_TOP ~ MGL_ROT_DATUM_RIGHT_BOTTOM
 */
MGLint MGL_GetRotationDatumDefaultValue( MGLip * pIP );


// Scaling
/**
 * MGL_Rotation
 *
 * @param[in] type : type�� ���� ���������� Scaling Function�� Action�� ��������. 
 */
MGLvoid * MGL_Scaling(MGLint type);

/**
 * MGL_DoScaling
 *
 * @param[in,out] pSrcBuffer Image Proccess ���� ����
 * @param[in,out] pDestBuffer Image Proccess ��� ����
 * @param[in,out] pX Image process ������ x Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pY Image process ������ y Position value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pWidth Image process ������ width value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in,out] pHeight Image process ������ height value pointer. ������ ����� �� �ֱ� ������ pointer�� �ѱ��.
 * @param[in] pParams Image process ���� �ʿ��� �߰����� parameter. MGLscalingparams ����ü�� pointer�� �ѱ��.
 */
MGLvoid MGL_DoScaling( MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams);

/**
 * MGL_SetScalingParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Scaling ����ü �� Setting��  Param ID
 * @param[in] pParam nParamID�� ���� Scaling Param ����ü ������ ������ �� 
 */
MGLvoid MGL_SetScalingParam(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * MGL_SetScalingRatioFlag
 * 
 * @brief Scale�� ������ �� width/height ������ ������ �������� Pixel������ ������ �� �����ϴ� �Լ�
 * @param[in] pIP Image Processing ����
 * @param[in] bRatio TRUE Scale�� width, height ���� ������ ������ ��� / FALSE Scale�� width, height ���� pixel�� ������ ���
 */
MGLvoid MGL_SetScalingRatioFlag(MGLip * pIP, MGLboolean bRatio );

/**
 * MGL_SetScalingWidth
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nNewWidth Scale�� �� ������ width�� ������ ��� 0 ~ 1000 or pixel ���� width ��
 */
MGLvoid MGL_SetScalingWidth(MGLip * pIP, MGLint nNewWidth );

/**
 * MGL_SetScalingHeight
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nNewHeight Scale�� �� ������ Height�� ������ ��� 0 ~ 1000 or pixel ���� width ��
 */
MGLvoid MGL_SetScalingHeight(MGLip * pIP, MGLint nNewHeight );

/**
 * MGL_SetScalingSameRatio
 * @brief ���� ���� ������ �����ϰ� �����ؼ� Scale �� ��� ����ϴ� �Լ�
 * @param[in] pIP Image Processing ����
 * @param[in] nNewRatio width,height Scaling ����
 */
MGLvoid MGL_SetScalingSameRatio( MGLip * pIP, MGLint nNewRatio );

/**
 * MGL_GetScalingBooleanProp
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nPropID Ȯ���ϰ��� �ϴ� Property ID
 */
MGLboolean MGL_GetScalingBooleanProp(MGLip * pIP, MGLint nPropID);

/**
 * MGL_GetScalingParam
 *
 * @param[in] pIP Image Processing ����
 * @param[in] nParamID Ȯ���ϰ��� �ϴ� Property ID
 * @param[out] pnParamType nParamID�� Parameter ���� Ÿ��
 * @param[out] pParam Rotation ����ü ���� �� nParamID�� Parameter ������ ������
 */
MGLvoid MGL_GetScalingParam(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * MGL_GetScalingRatioFlag
 * @brief ���� Scale ������ ������ �Ǿ����� Pixel������ �Ǿ������� ��Ÿ���� Flag ���� ��� �Լ�
 * @param[in] pIP Image Processing ����
 * @return TRUE Scale�� width,height ���� ������ ���õ� ���/ FALSE pixel������ ���õ� ��� 
 */
MGLboolean MGL_GetScalingRatioFlag(MGLip * pIP );

/**
 * MGL_GetScalingWidth
 *
 * @param[in] pIP Image Processing ����
 * @return Scale�� width �� ( ���� Ȥ�� Pixel ) 
 */
MGLint MGL_GetScalingWidth(MGLip * pIP );

/**
 * MGL_GetScalingHeight
 *
 * @param[in] pIP Image Processing ����
 * @return Scale�� Height �� ( ���� Ȥ�� Pixel ) 
 */
MGLint MGL_GetScalingHeight(MGLip * pIP );



#ifdef __cplusplus
}
#endif

#endif // __mgl_ip_h_
