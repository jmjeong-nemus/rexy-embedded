/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @brief MGL Core Module
 *
 * @file mgl.h
 * @author <a href="mailto:hylee@mococo.com">Hwayoung Lee</a>
 * @author <a href="mailto:mio@mococo.com">Jinwoo Min</a>
 *
 */
 
#ifndef __mgl_h_
#define __mgl_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "mgl_config.h"

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef NULL
#define NULL (0)		/**< NULL value */
#endif

#ifndef TRUE
#define TRUE (1)		/**< �� �� */
#endif

#ifndef FALSE
#define FALSE (0)		/**< ���� �� */
#endif

/**
 * @defgroup api_types Types
 *
 * �⺻ type�� ���� ����
 *
 * @addtogroup api_types 
 */

/**
 * @brief emueration��
 */
typedef unsigned int MGLenum;		
/**
 * @brief TRUE, FALSE �� �ϳ��� ���� ���� Ÿ��
 */
typedef unsigned char MGLboolean;
/**
 * @brief
 */
typedef unsigned int MGLbitfield;	/**< bit�� �� �ʵ� Ÿ�� */
/**
 * @brief
 */
typedef char MGLbyte;		/**< byte�� ũ���� Ÿ�� */
/**
 * @brief long ũ���� Ÿ��
 */
typedef short MGLlong;
/**
 * @brief
 */
typedef short MGLshort;			/**< short ũ���� Ÿ�� */
/**
 * @brief
 */
typedef int MGLint;				/**< int ũ���� Ÿ�� */
/**
 * @brief
 */
typedef int MGLsizei;				/**< size�� ��Ÿ���� Ÿ�� */
/**
 * @brief
 */
typedef unsigned char MGLubyte;	/**< unsigned byte ũ���� Ÿ�� */
/**
 * @brief unsigned long ũ���� Ÿ��
 */
typedef unsigned long MGLulong;
/**
 * @brief
 */
typedef unsigned short MGLushort;	/**< unsigned short ũ���� Ÿ�� */
/**
 * @brief
 */
typedef unsigned int MGLuint;		/**< unsigned int ũ���� Ÿ�� */
/**
 * @brief
 */
typedef float MGLfloat;			/**< float ũ���� Ÿ�� */
/**
 * @brief
 */
typedef double MGLdouble;			/**< double ũ���� Ÿ�� */
/**
 * @brief
 */
typedef void MGLvoid;				/**< void Ÿ�� */

/**
 * @}
 */
 
/**
 * @defgroup api_errhandling Error Handling
 *
 * MGL���� ���Ǵ� error code �� error ���� API
 * @see MGLerrmsg, MGL_SetError, MGL_GetError
 *
 * @addtogroup api_errhandling
 * @{
 */
 
typedef int MGLerror;				/**< error ���� Ÿ�� */
	
/**
 * @brief ������ �߻����� ����
 */
#define MGL_ERROR_NONE					0
/**
 * @brief �Ϲ����� �����ڵ� ���� ����
 */
#define MGL_ERRORRANGE_GENERIC_START		0x0001
/**
 * @brief ������ ���
 */
#define MGL_ERROR_OUT_OF_RANGE				0x0001
/**
 * @brief parameter�� ã�� ���ϴ� ��� �߻���
 */
#define MGL_ERROR_PARAMETER_NOT_FOUND		0x0003
/**
 * @brief �Ϲ����� �����ڵ� ���� ��
 */
#define MGL_ERRORRANGE_GENERIC_END			(MGL_ERRORRANGE_GENERIC_START + 0x0fff)

/**
 * @brief Core Module�� �����ڵ� ���� ����
 */
#define MGL_ERRORRANGE_CORE_START			0x1001
/**
 * @brief engine�� �ʱ�ȭ�ϴ� ���߿� ������ ����� ��쿡 �߻�
 */
#define MGL_ERROR_ENGINE_INIT_FAILURE		0x1002
/**
 * @brief bitmap�� �о����(loading) ���ϴ� ��쿡 �߻�
 */
#define MGL_ERROR_BITMAP_LOAD_FAILURE		0x1003
/**
 * @brief ȣ���� API�� layer�� �Ӽ��� ���� �ʴ� ��쿡 �߻�
 */
#define MGL_ERROR_INVALID_LAYER				0x1004
/**
 * @brief �� �� ���� �Ӽ� ID�� ��쿡 �߻�
 */
#define MGL_ERROR_UNKNOWN_PROP_ID			0x1005
/**
 * Core Module�� �����ڵ� ���� ��
 */
#define MGL_ERRORRANGE_CORE_END				(MGL_ERRORRANGE_CORE_START + 0x0fff)

/**
 * Image Process Module�� �����ڵ� ���� ����
 */
#define MGL_ERRORRANGE_IP_START				0x2001
/**
 * @brief �˼� ���� ip �Լ��� ȣ���ϴ� ��쿡 �߻�
 */
#define MGL_ERROR_UNKNOWN_IP_FUNC			0x2002
/**
 * @brief �˼� ���� ip�� ���ڰ�(parameter)�� ���� ����Ϸ��� ��쿡 �߻�
 */
#define MGL_ERROR_UNKNOWN_IP_PARAM			0x2003
/**
 * @brief �˼� ���� ip�� �Ӽ���(property)�� ����Ϸ��� ��쿡 �߻�
 */
#define MGL_ERROR_UNKNOWN_IP_PROP			0x2004
/**
 * @brief ������ ������ �Ѿ�� ���� ����� ��쿡 �߻�
 */
#define MGL_ERROR_OUT_OF_BOUND				0x2005
/**
 * @brief Image Process Module�� �����ڵ� ���� ��
 */
#define MGL_ERRORRANGE_IP_END				(MGL_ERRORRANGE_IP_START + 0x0fff)

/**
 * @brief Animator Module�� �����ڵ� ���� ����
 */
#define MGL_ERRORRANGE_ANIMATOR_START		0x3000
/**
 * @brief Animation�� �����
 */
#define MGL_ERROR_ANIMATION_END				0x3001
/**
 * @brief �������� ã�� �� ����
 */
#define MGL_ERROR_NOT_FOUND					0x3002
/**
 * @brief ���������ʴ� PARAM TYPE
 */
#define MGL_ERROR_NOT_SUPPORTED_PARAM_TYPE	0x3003
/**
 * @brief ���������ʴ� PARAM ID
 */
#define MGL_ERROR_NOT_SUPPORTED_PARAM_ID	0x3004
/**
 * @brief ���������ʴ� �ɼ�  
 */
#define MGL_ERROR_NOT_SUPPORTED_OPTION		0x3005
/**
 * @brief Animator Module�� �����ڵ� ���� ��
 */
#define MGL_ERRORRANGE_ANIMATOR_END			(MGL_ERRORRANGE_ANIMATOR_START + 0x0fff)

/**
 * @brief ����� ���� �����ڵ� ���� ����
 */
#define MGL_ERRORRANGE_USER_DEFINED_START	0x8000
/**
 * @brief ����� ���� �����ڵ� ���� ��
 */
#define MGL_ERRORRANGE_USER_DEFINED_END		(MGL_ERRORRANGE_USER_DEFINED_START + 0x0fff)

/**
 * @brief �˼� ���� ����
 */
#define MGL_ERROR_UNKNWON					0xffff

/**
 * @brief �߻��� error ������ �����Ѵ�. 
 *
 * @attention �� �Լ��� ���������� ���ǹǷ� MGL_SetError �� ����Ѵ�.
 * @param[in] error error code
 * @param[in] pszFile error�� �߻��� �ҽ��� ���ϸ�
 * @param[in] pszFunc error�� �߻��� �ҽ��� �Լ�
 * @param[in] nLine error�� �߻��� �ҽ��� �ٹ�ȣ
 * @see MGL_SetError, MGL_GetError, MGLerrmsg
 */
MGLvoid  MGL_SetErrorInfo( MGLerror error, MGLbyte * pszFile, MGLbyte * pszFunc, MGLint nLine );

/**
 * @brief �ֱ� �߻��� error code�� ���Ѵ�.
 *
 * @return error code
 * @see MGL_SetError
 */
MGLerror MGL_GetError( MGLvoid );

/**
 * @brief �߻��� error code�� �����Ѵ�.
 *
 * error ������ �� �� ���ϱ� ���� ���ȴ�. 
 * @param[in] error error code
 * @see MGL_GetError
 * @note MGL_SetErrorInfo�� �Ű� ������ __MGL_FILE__, __MGL_FUNC__, __MGL_LINE__ �� ����� ��ũ���̴�.
 *		FILE, FUNC, LINE�� ���� MGL_SetError(..) �Լ��� �ִ� ������ ���ǹ� �� �� �ִ�. 
 */
#define MGL_SetError(error) MGL_SetErrorInfo((MGLerror)error, ( MGLbyte * )__MGL_FILE__, (MGLbyte *)__MGL_FUNC__, (MGLint)__MGL_LINE__)

/**
 * @}
 */

/**
 * @defgroup apilayertype Layer Type ID
 *
 * MGLlayer::type������ ���ȴ�.
 * @see MGLlayer
 *
 * @addtogroup apilayertype
 * @{
 */
/**
 * @brief image layer�� type��
 */
#define MGL_IMAGE_LAYER				0x1
/**
 * @brief external image layer�� type��
 */
#define MGL_EXTERNAL_IMAGE_LAYER	0x2
/**
 * @brief custom layer�� type��
 */
#define MGL_CUSTOM_LAYER			0x3
/**
 * @}
 */ 
 
/**
 * @defgroup globpropids Global Property IDs
 *
 * MGL_GetStringProperty, MGL_GetIntProperty, MGL_GetBooleanProperty �� nPropID ������ ���ȴ�.
 *
 * @addtogroup globpropids
 * @{
 */

/**
 * @brief �ý��� �̸�
 */
#define MGL_SYS_PROP_STR_NAME				0x0001
/**
 * @brief �ý��� �ۼ���
 */
#define MGL_SYS_PROP_STR_AUTHOR				0x0002
/**
 * @brief �ý����� major version number
 */
#define MGL_SYS_PROP_INT_MAJOR_VERSION		0x0011
/**
 * @brief �ý����� middle version number
 */
#define MGL_SYS_PROP_INT_MIDDLE_VERSION		0x0012
/**
 * @brief �ý����� minor version number
 */
#define MGL_SYS_PROP_INT_MINOR_VERSION		0x0013
/**
 * @brief �ý����� build number
 */
#define MGL_SYS_PROP_INT_BUILD_NUMBER		0x0014
/**
 * @brief �ý����� build ��¥ �� �ð�
 */
#define MGL_SYS_PROP_STR_BUILD_DATE			0x0015
/**
 * @brief Ÿ�� �Ӽ� �÷���
 */
#define MGL_TARGET_PROP_FLAG				0x1000
/**
 * @brief Ÿ���� H/W �� ����
 */
#define MGL_TARGET_PROP_STR_HW_MODEL		0x1001
/**
 * @brief Ÿ���� H/W �� CPU ����
 */
#define MGL_TARGET_PROP_STR_HW_CPU			0x1002
/**
 * @brief Ÿ���� H/W �� Memory ����
 */
#define MGL_TARGET_PROP_STR_HW_MEMORY		0x1003
/**
 * @brief Ÿ���� H/W �� GPU ����
 */
#define MGL_TARGET_PROP_STR_HW_GPU			0x1004
/**
 * @brief Ÿ���� S/W �� OS ����
 */
#define MGL_TARGET_PROP_STR_SW_OS			0x1101
/**
 * @brief Ÿ���� S/W �� GUI Platform ����
 */
#define MGL_TARGET_PROP_STR_SW_GUI			0x1102
/**
 * @}
 */ 


/**
 * @defgroup core Core API
 *
 * Engine Core API
 * 
 * @addtogroup core
 * @{
 */

/**
 * @brief error ���� ����ü
 *
 * �� ����ü�� device context�� ���ԵǸ� MGL_GetError �� ȣ���Ͽ� ���������� ������ �� �ִ�.
 * @see MGL_SetError, MGL_GetError, MGLdc
 */
typedef struct MGLerrmsg
{
	MGLerror	error;			/**< error code */
	MGLbyte		szFile[128];	/**< error�� �߻��� ���� �̸� */
	MGLbyte		szFunc[128];	/**< error�� �߻��� �Լ� �̸� */
	MGLint		nLine;			/**< error�� �߻��� ���� */
} MGLerrmsg;

#if MGL_COLOR_RGB565	
	typedef unsigned short MGLcolor;	/**< ������ ��Ÿ���� ���� Ÿ��  */
	#define MGL_R_MASK	0xf800			/**< ���󰪿��� ������ mask ��  */
	#define MGL_G_MASK	0x07e0			/**< ���󰪿��� ��� mask ��  */
	#define MGL_B_MASK	0x001f			/**< ���󰪿��� �Ķ��� mask ��  */
	#define MGL_MakeColor( r, g, b )	(MGLcolor)((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))	/**< ���� ����� ��ũ��  */
#else
	typedef unsigned long MGLcolor;
#endif	// MGL_COLOR_RGB565

#define MGL_GetColorR(col) (col) & MGL_R_MASK	/**< �־��� ���󿡼� �������� ���� ������ */
#define MGL_GetColorG(col) (col) & MGL_G_MASK	/**< �־��� ���󿡼� ����� ���� ������ */
#define MGL_GetColorB(col) (col) & MGL_B_MASK	/**< �־��� ���󿡼� �Ķ����� ���� ������ */

/**
 * @brief bitmap ����ü
 */
typedef struct MGLbitmap
{
	MGLint		width;				/**< bitmap width */
	MGLint		height;				/**< bitmap height */
	MGLboolean	bTransparent;		/**< ����� ��뿩�� */
	MGLcolor	nTrColor;			/**< ������� */
	MGLint		bpp;				/**< bit per pixel�� */
	MGLvoid *	data;				/**< bitmap raw ������ ������ ������ */
} MGLbitmap;

/**
 * @brief image processor�� ��ǥ(main) �Լ� ������
 *
 * @param[in] type image processor�� ����(sub) �Լ��� Ÿ��
 * @return ���� �Լ��� ������ 
 * @see MGLip
 */
typedef MGLvoid *(*MGL_IPFuncPtr)(MGLint type);

/**
 * @brief image processor ����ü
 *
 * image processor�� ��ǥ�ϴ� �Լ��� �����Ϳ� ���ڰ� ����ü�� ������ ������ ������ �ִ�.
 * @see @ref page_ip, MGL_IPFuncPtr, MGL_DoIPFuncPtr, MGL_SetIPParamFuncPtr, MGL_GetIPIntPropFuncPtr, MGL_GetIPBooleanPropFuncPtr
 */
typedef struct MGLip
{
	MGL_IPFuncPtr				fpIP;		/**< image processor�� ��ǥ�ϴ� �Լ��� ������ */
	MGLvoid *					pParams;	/**< image processor�� �ʿ��� ���ڰ� ����ü�� ������ */
} MGLip;

/**
 * @brief image processor�� MGLint ������ �Ӽ����� ���ϴ� �Լ� ������
 *
 * @param[in] pIP image processor ����ü�� ������
 * @param[in] nPropID �Ӽ��� ID��
 * @return MGLint������ �Ӽ���
 * @see MGLip, MGL_IPFuncPtr
 */
typedef MGLint (*MGL_GetIPIntPropFuncPtr)(MGLip * pIP, MGLint nPropID);
/**
 * @brief image processor�� MGLboolean ������ �Ӽ����� ���ϴ� �Լ� ������
 *
 * @param[in]  pIP image processor ����ü�� ������
 * @param[in] nPropID �Ӽ��� ID��
 * @return MGLboolean������ �Ӽ���
  * @see MGLip, MGL_IPFuncPtr
*/
typedef MGLboolean (*MGL_GetIPBooleanPropFuncPtr)(MGLip * pIP, MGLint nPropID);
/**
 * @brief image processor�� image processing �ϴ� �Լ� ������ Ÿ��
 *
 * @param[in] pSrcBuffer image processing�� �ʿ��� ���� ����
 * @param[in,out] pDestBuffer image processing�� �ʿ��� ��� ����
 * @param[in,out]  nX image processing�� �������� �������� ������. ������ ����� �� �ֱ� ������ �����͸� �ѱ��.
 * @param[in,out] nY image processing�� �������� �������� ������. ������ ����� �� �ֱ� ������ �����͸� �ѱ��.
 * @param[in,out] pWidth image processing�� �������� �������� ������. ������ ����� �� �ֱ� ������ �����͸� �ѱ��.
 * @param[in,out] pHeight image processing�� �������� �������� ������. ������ ����� �� �ֱ� ������ �����͸� �ѱ��.
 * @param[in] pParam image processor���� �ʿ��� �߰����� ���ڰ� ����ü�� ������
 * @see MGLip, MGL_IPFuncPtr
 */
typedef MGLvoid (*MGL_DoIPFuncPtr)(MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams);

/**
 * @brief image processor�� paramter�� ���ϴ� �Լ��� ������ Ÿ��
 *
 * @param[in] pIP image processor�� ������
 * @param[in] nParamID parameter�� ID
 * @param[out] pnParamType paramter�� Ÿ�Կ� ���� ������
 * @param[out] pParam paramter ��
 */
typedef MGLvoid (*MGL_GetIPParamFuncPtr)(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * @brief image processor�� ���ڰ��� �����Ѵ�.
 *
 * @param[in] pIP image processor�� ������
 * @param[in] nParamID ���ڰ��� ID
 * @param[in] pParam ���ڰ�
 * @see MGLip, MGL_IPFuncPtr
 */
typedef MGLvoid (*MGL_SetIPParamFuncPtr)(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * @brief bitmap ������ ���� layer
 *
 * @see MGLlayer
 */
typedef MGLbitmap		MGLimagelayer;

struct MGLlayer;
/**
 * @brief image layer ������ ������Ʈ�ϴ� �Լ� ������
 *
 * @param layer MGLlayer pointer
 * @param pParams ������Ʈ �Լ����� ����� ���� ����ü�� ������
 * @return FALSE�̸� ������Ʈ �Լ��� ���������� ȣ���Ѵ�. TRUE�̸� �� �̻� ������Ʈ �Լ��� ȣ������ �ʴ´�.
 * @see MGLlayer
 */
typedef MGLboolean (* MGL_UpdateImageLayerFuncPtr)(MGLint index, struct MGLlayer * layer, MGLvoid * pParams);

/**
 * @brief �ܺ��� �Լ��� ���� bitmap ������ ������Ʈ�ϰ� �ϴ� layer
 *
 * @see MGLlayer, MGLimagelayer. MGL_UpdateImageLayerFuncPtr
 */
typedef struct MGLextimagelayer
{
	MGLint						count;					/**< �� ������Ʈ �Լ� ȣ�� ȸ�� */
	MGL_UpdateImageLayerFuncPtr	fpUpdateImageLayer;		/**< ������Ʈ �Լ��� ������ */
	MGLvoid *					pParams;				/**< ������Ʈ �Լ� ȣ��� �ʿ��� parameter ����ü�� ������ */
} MGLextimagelayer;

/**
 * @brief layer�� �׸��� �Լ��� ������
 *
 * @see MGLlayer, MGLcustomlayer
 */
typedef MGLvoid (*MGL_DrawLayerFuncPtr)(MGLvoid * pBuffer, MGLvoid * pParams);

/**
 * @brief �ܺ��� �Լ��� ���� �׷����� �ϴ� layer
 *
 * @see MGLlayer, MGL_DrawLayerFuncPtr
 */
typedef struct MGLcustomlayer
{
	MGL_DrawLayerFuncPtr		fpDrawLayer;	/**< ����ڷ� �Ͽ��� layer�� �׸��� �ϴ� �Լ��� ������. */
	MGLvoid *					pBuffer;		/**< ����ڰ� �����ϴ� ����. NULL�̸� ������� �ʴ´�. @attention ���� ����ϴ� ���, �� layer���� image processing�� ���� �ʴ´�. */
	MGLvoid *					pParams;		/**< custom layer�� �ʿ��� ����� ���� paramter */
} MGLcustomlayer;

/**
 * @brief layer ����ü
 *
 * layer���� Ÿ��, ��ǥ, ���� ���� �⺻ ������ Ÿ�Կ� ���� �ΰ� ������ ������ �ִ�.
 * @see MGLdc, MGLimagelayer, MGLextimagelayer, MGLcustomlayer, MGLip
 */
typedef struct MGLlayer
{
    MGLenum		type;				/**< layer�� ����. @ref apilayertype �� �����Ѵ�. */
	MGLint		x;					/**< layer�� ���� */
	MGLint		y;					/**< layer�� ���� */
	MGLint		width;				/**< layer�� ���� */
	MGLint		height;				/**< layer�� ���� */
	MGLint		nNumIPs;			/**< image processor���� �� ���� */
	MGLip *		pIPs;				/**< image processor �迭�� ������ */
	MGLboolean	bVisible;			/**< ǥ��(rendering) ����. false�̸� ǥ������ �ʴ´�. */
	union
	{
		MGLimagelayer			tImage;			/**< image layer ���� */
		MGLextimagelayer		tExtImage;		/**< external image layer ���� */
		MGLcustomlayer			tCustom;		/**< custom layer ���� */
	} data;							/**< layer ������ ������ */
} MGLlayer;


/**
 * @brief rendering context ����ü. RC��� �ٿ��� ����Ѵ�.
 *
 * rendering context�� �������� layer �鿡 ���� ������ ������ ������, RC�� ����Ϸ��� MGL_SetDCCurRC �� ȣ���Ͽ� device context��  ���� ������� rendering context �� ����ؾ� �Ѵ�.
 * @see MGLdc, MGL_SetDCCurRC, MGLlayer
 */
typedef struct MGLrc
{
	MGLint		x;					/**< rendering buffer ���� �׷����� ���� (topleft ��ǥ)  */
	MGLint		y;					/**< rendering buffer ���� �׷����� ���� (topleft ��ǥ)  */
    MGLsizei	width;				/**< rendering buffer ���� �׷����� ����  */
    MGLsizei	height;				/**< rendering buffer ���� �׷����� ����  */

	MGLlayer *	pCurLayer;			/**< ���� ����ϴ� layer�� ������ */
	MGLint		nNumLayer;			/**< RC�� �����ϴ� layer�� ����. */
	MGLlayer *	pLayers;			/**< RC�� �����ϴ� layer �迭�� ������ */
} MGLrc;

/**
 * @brief buffer �� ����ü 
 *
 * rendering/working buffer �鿡 ���� ������ ��� �ִ�.
 */
typedef struct MGLbuffers
{
	MGLvoid *	pRenderBuffer;		/**< rendering buffer. platform�� ��µǴ� ���� */
	MGLint		nNumWorkBuffers;	/**< working buffer�� �� ����. �ּ� 2�� �̻��־�� image processing�� �����ϴ�. */
	MGLvoid **	pWorkBuffers;		/**< working buffer �迭�� ������. layer �ϳ��� �׸��� ���� �ӽ÷� ����ϴ� ���� �迭. image processing�� �ִ� ��� ����ϸ� 2���� ���� ������ ���ȴ�. */
	MGLvoid *	pTempBuffer;		/**< MGL_RenderBuffer()�� ���� �ӽ÷� ����ϴ� ���� */
} MGLbuffers;

/**
 * @brief device context ����ü. �ٿ��� DC�� ����Ѵ�.
 *
 * device context���� rendering buffer�� ����  ����, ���� ������� rendering context, ���� ���� ���� ��� �ִ�.
 * @see MGLrc, MGLbuffers, MGLerrmsg
 */
typedef struct MGLdc
{
    MGLsizei	width;				/**< rendering buffer width */
    MGLsizei	height;				/**< rendering buffer height */
	MGLcolor	transparent;		/**< transparent color */
	MGLint		count;				/**< rendering context�� �� ���� */
	MGLrc *		pRCs;				/**< rendering context �迭�� ������ */
	MGLrc *		pCurRC;				/**< ���� ����ϴ� rendering context�� ������ */
	MGLbuffers	tBuffers;			/**< buffer ���� */
	MGLerrmsg	tError;				/**< ���� ���� */
	MGLboolean	bFPSEnabled;		/**< true�̸� frames per second�� �����ϰ� �ϰ� ����ϰ� �Ѵ�. */
	MGLulong	nFPSTicks;			/**< frames per second�� �����ϱ� ���� �ʿ��� ������ ��� ������ �ð�(tick) */
	MGLint		nFPSCount;			/**< frames per second�� �����ϱ� ���� �ʿ��� ������ ������ �ð� ���ȿ� �׷����� frame ȸ�� */
} MGLdc;

/*
 * Math API
 */
#define MGL_FIXEDPOINT_PRECISION	10 /**< fixedpoint ������ ����ϴ� ���е� �� */


#ifdef MGL_CONFIG_USE_FIXED_POINT
	typedef int							MGLfixed; /**< fixed point Ÿ���� int �� �Ѵ�. */
	#define MGL_Int2Fixed(x)			((x) << MGL_FIXEDPOINT_PRECISION) /**< �������� fixed point ������ ��ȯ�Ѵ�. */
	#define MGL_Fixed2Int(x)			((x) >> MGL_FIXEDPOINT_PRECISION) /**< fixedpoint ���� ���������� ��ȯ�Ѵ�. */
	#define MGL_Float2Fixed(x)			((MGLfixed)((x) * (1 << MGL_FIXEDPOINT_PRECISION))) /**< �ε��Ҽ����� fixed point ������ ��ȯ�Ѵ�. */
	#define MGL_Fixed2Float(x)			(((double)(x)) / (1 << MGL_FIXEDPOINT_PRECISION)) /**< fixed point ���� �ε��Ҽ��������� ��ȯ�Ѵ�. */
	#define MGL_Fraction(x)				((x) & ((1 << MGL_FIXEDPOINT_PRECISION) - 1)) /**< �Ҽ��θ� ����  */
	#define MGL_Mul(x, y)				(((x) * (y)) >> MGL_FIXEDPOINT_PRECISION) /**< fixed point �� ���ϱ� ���� */
	#define MGL_Div(x, y)				(((x) << MGL_FIXEDPOINT_PRECISION) / (y)) /**< fixed point �� ������ ���� */
	#define MGL_Div2(x, y)				(((x) / (y)) << MGL_FIXEDPOINT_PRECISION) /**< fixed point �� ������ ���� */
	//#define MGL_Div(x, y)				(((x) * (1 << MGL_FIXEDPOINT_PRECISION)) / (y)) /**< fixed point �� ������ ���� */
	//#define MGL_Div(x, y)				((MGLfixed)((((long long)x) << MGL_FIXEDPOINT_PRECISION) / (y))) /**< fixed point �� ������ ���� */
	#define MGL_PI						MGL_Float2Fixed(3.14159265358979323846) /**< ���̰� */
#else // #ifdef MGL_CONFIG_USE_FIXED_POINT
	typedef double						MGLfixed;
	#define MGL_Int2Fixed(x)			( (MGLfixed)x )
	#define MGL_Fixed2Int(x)			( (MGLint)x )
	#define MGL_Float2Fixed(x)			( (MGLfixed)x )
	#define MGL_Fixed2Float(x)			( (MGLdouble)x )
	#define MGL_Fraction(x)				( x - (MGLint)x)
	#define MGL_Mul(x, y)				( (x) * (y) )
	#define MGL_Div(x, y)				( (x) / (y) )
	#define MGL_PI						3.14159265358979323846
#endif // #ifdef MGL_CONFIG_USE_FIXED_POINT

/**
 * @brief fixedpoint �� ���ϱ� ����
 */
#define		MGL_Add(x, y)				((x) + (y))

/**
 * @brief fixedpoint �� ���� ����
 */
#define		MGL_Sub(x, y)				((x) - (y))

/**
 * @brief Fixed point library�� ����� �� ������ ������ �Լ� 
 */
#define		MGL_DivByInt(x,y)			((x) / (y))

/** 
 * @brief ������ ���� �Լ� (x ^ n).
 * 
 * @param x ���̽�
 * @param n ����
 * @return x ^ n
 */
MGLfixed	MGL_Pow (MGLfixed x, MGLint n);

/** 
 * @brief �������� ���ϴ� �Լ�
 * 
 * @param x �������� ���� �� 
 * @return x�� ������
 */	
MGLfixed	MGL_Sqrt (MGLfixed x);

/** 
 * @brief sin�� ����ϴ� �Լ�.
 * 
 * @param deg ����
 * @return deg �������� sin ��
 */
MGLfixed	MGL_Sin (MGLfixed deg);

/** 
 * @brief cos�� ����ϴ� �Լ�. 
 * 
 * @param deg ����
 * @return deg �������� cos ��
 */
MGLfixed	MGL_Cos (MGLfixed deg);

/** 
 * @brief ������ ���� ������� ����ϴ� Seed ���� �����Ѵ�. 
 * 
 * @param nSeed ���� ��
 */
MGLvoid		MGL_SetRandomSeed( MGLint nSeed );

/** 
 * @brief �־��� Seed ���� �ش� ���� ���� ����Ͽ� ������ ���� ������ �Լ� . 
 * 
 * @param nHigh ���� �� ���� ���� ��
 * @param nLow  ���� �� ���� ���� ��
 * @return ���� ���� ������ ��
 */
MGLint		MGL_Random( MGLint nHigh, MGLint nLow );

/**
 * @brief engine�� �ʱ�ȭ�Ѵ�.
 * 
 * @param[in,out] data engine ���ο��� �ʿ�� �ϴ� data. porting�ÿ� platform�� ���� ���ǰ� �ٸ���.
 */
MGLvoid MGL_InitEngine(MGLvoid * data);
/**
 * @brief engine�� �����Ѵ�.
 */
MGLvoid MGL_DeinitEngine( MGLvoid );

//  device context related functions
/**
 * @brief device context�� width�� ���Ѵ�.
 *
 * @return device context�� width
 */
MGLint MGL_GetDCWidth(MGLvoid);
/**
 * @brief device context�� height�� ���Ѵ�.
 *
 * @return device context�� height
 */
MGLint MGL_GetDCHeight(MGLvoid);
/**
 * @brief device context�� ��������� ���Ѵ�.
 * 
 * @return �������
 */
MGLcolor MGL_GetDCTrColor( MGLvoid );

// rendering context related functions
/**
 * @brief ����� rendering context �迭�� �����Ѵ�.
 *
 * @param[in] count rendering context�� �� ����
 * @param[in] pRCs rendering context �迭�� ������
 */
MGLvoid MGL_SetDCRCs( MGLint count, MGLrc *pRCs );
/**
 * @brief ���� ������� rendering context�� index�� �������� �����Ѵ�.
 *
 * @param[in] index rendering context�� index
 */
MGLvoid MGL_SetDCCurRCIndex( MGLint index );
/**
 * @brief ���� ������� rendering context�� �����Ѵ�.
 *
 * @param[in] pRC rendering context�� ������
 */
MGLvoid MGL_SetDCCurRC( MGLrc *pRC );
/**
 * @brief ���� ������� rendering context�� �����͸� ���Ѵ�.
 * 
 * @return rendering context�� ������
 */
MGLrc * MGL_GetDCCurRC( MGLvoid );
/**
 * @brief ���� ������� rendering context�� layer�� �����Ѵ�.
 * 
 * @param[in] count layer�� �� ����
 * @param[in] pLayers layer�� �迭�� ������
 */
MGLvoid MGL_SetRCLayers(MGLint count, MGLlayer * pLayers);
/**
 * @brief rendering context�� ������ �����Ѵ�.
 * 
 * @param[in] x ������ ��ǥ��
 * @param[in] y ������ ��ǥ��
 * @param[in] width ������ x ũ��
 * @param[in] height ������ y ũ��
 */
MGLvoid MGL_SetRCBounds(MGLint x, MGLint y, MGLint width, MGLint height);
/**
 * @brief rendering context�� ������ x ��ǥ���� ���Ѵ�.
 * 
 * @return x ��ǥ��
 */
MGLint MGL_GetRCX(MGLvoid);
/**
 * @brief rendering context�� ������ y ��ǥ���� ���Ѵ�.
 * 
 * @return y ��ǥ��
 */
MGLint MGL_GetRCY(MGLvoid);
/**
 * @brief rendering context�� ������ x ũ��(width)�� ���Ѵ�.
 * 
 * @return x ũ�� 
 */
MGLint MGL_GetRCWidth(MGLvoid);
/**
 * @brief rendering context�� ������ y ũ��(height)�� ���Ѵ�.
 * 
 * @return y ũ��
 */
MGLint MGL_GetRCHeight(MGLvoid);

// layer property related functions
/**
 * @brief ���� ������� layer�� index������ �����Ѵ�.
 * 
 * @param[in] index layer �迭�� index
 */
MGLvoid MGL_SetCurLayerIndex( MGLint index );
/**
 * @brief ���� ������� layer�� layer�� �����ͷ� �����Ѵ�.
 * 
 * @param[in] layer layer ����ü�� ������
 */
MGLvoid MGL_SetCurLayer( MGLlayer * layer );
/**
 * @brief ���� ������� layer�� �����͸� ���Ѵ�.
 * 
 * @return layer�� ������
 */
MGLlayer * MGL_GetCurLayer( MGLvoid );
/**
 * @brief index������ layer�� �����͸� ���Ѵ�.
 * 
 * @return layer�� ������
 */
MGLlayer * MGL_GetLayer( MGLint index );
/**
 * @brief layer�� Ÿ���� �����Ѵ�.
 * 
 * @param[in] type layer�� Ÿ�԰�
 */
MGLvoid MGL_SetLayerType( MGLint type );
/**
 * @brief layer�� ���̰� �ϰų� �����.
 * 
 * @param[in] bVisible TRUE�̸� ���̰� �Ѵ�.
 */
MGLvoid MGL_SetLayerVisible( MGLboolean bVisible );
/**
 * @brief layer�� visible �Ӽ��� ���Ѵ�.
 * 
 * @return layer�� visible �Ӽ�
 */
MGLboolean MGL_GetLayerVisible(MGLvoid);
/**
 * @brief layer�� ������ �����Ѵ�.
 *
 * @attention �� ���� MGL_SetLayerBitmap �̳� image processing ������ ��ġ�� engine�� ���� ����� �� �ֽ��ϴ�.
 * @param[in] x ������ x ��ǥ��
 * @param[in] y ������ y ��ǥ��
 * @param[in] width ������ x ũ�Ⱚ (width)
 * @param[in] height ������ y ũ�Ⱚ (height)
 */
MGLvoid MGL_SetLayerBounds(MGLint x, MGLint y, MGLint width, MGLint height);
/**
 * @brief layer�� x ��ǥ���� �����Ѵ�.
 * 
 * @param[in] x ������ x ��ǥ��
 */
MGLvoid MGL_SetLayerX(MGLint x);
/**
 * @brief layer�� x ��ǥ���� ���Ѵ�.
 * 
 * @return ������ x ��ǥ��
 */
MGLint MGL_GetLayerX(MGLvoid);
/**
 * @brief layer�� y ��ǥ���� �����Ѵ�.
 * 
 * @param[in] y ������ y ��ǥ��
 */
MGLvoid MGL_SetLayerY(MGLint y);
/**
 * @brief layer�� y ��ǥ���� ���Ѵ�.
 * 
 * @return ������ y ��ǥ��
 */
MGLint MGL_GetLayerY(MGLvoid);
/**
 * @brief layer�� x ũ�Ⱚ�� �����Ѵ�.
 * 
 * @param[in] width ������ x ũ�Ⱚ
 */
MGLvoid MGL_SetLayerWidth(MGLint width);
/**
 * @brief layer�� x ũ�Ⱚ�� ���Ѵ�.
 * 
 * @return ������ x ũ�Ⱚ
 */
MGLint MGL_GetLayerWidth(MGLvoid);
/**
 * @brief layer�� y ũ�Ⱚ�� �����Ѵ�.
 * 
 * @param[in] height ������ y ũ�Ⱚ
 */
MGLvoid MGL_SetLayerHeight(MGLint height);
/**
 * @brief layer�� y ũ�Ⱚ�� ���Ѵ�.
 * 
 * @return ������ y ũ�Ⱚ
 */
MGLint MGL_GetLayerHeight(MGLvoid);
/**
 * @brief ������ layer�� image processor���� �����Ѵ�.
 * 
 * @param[in] count image processor���� �� ����
 * @param[in] pIPs image processor���� �迭�� ������
 */
MGLvoid MGL_SetLayerIPs(MGLint count, MGLip * pIPs);
/**
 * @brief layer�� �̵��Ѵ�.
 * 
 * @param[in] nXOffset x ��ǥ�� ��� �̵���
 * @param[in] nYOffset y ��ǥ�� ��� �̵���
 */
MGLvoid MGL_MoveLayer(MGLint nXOffset, MGLint nYOffset);

// only image layer related functions
/**
 * @brief layer�� bitmap�� �����Ѵ�.
 *
 * ���������� MGL_LoadBitmap �� ȣ���Ѵ�.
 * 
 * @param[in] nBitmapID bitmap ID
 */
MGLvoid MGL_SetLayerBitmap(MGLint nBitmapID);

// only external image layer related functions
/**
 * @brief external image layer�� image layer ������ ������Ʈ�ϰ� ���ִ� �Լ��� �����Ѵ�.
 * 
 * @param[in] fpUpdateImageLayer image layer ������ ������Ʈ�ϰ� ���ִ� �Լ��� ������
 * @param[in] count ������Ʈ �Լ� �� ȣ�� ȸ��
 * @param[in] pParams ������Ʈ �Լ� ȣ��ÿ� �ʿ��� parameter ����ü�� ������
 */
MGLvoid MGL_SetUpdateImageLayerFuncPtr(MGL_UpdateImageLayerFuncPtr fpUpdateImageLayer, MGLint count, MGLvoid * pParams);

// only custom layer related functions
/**
 * @brief custom layer�� layer�� �׸��� �Լ��� �����Ѵ�.
 * 
 * @param[in] fpDrawLayer layer�� �׸��� �Լ��� ������
 */
MGLvoid MGL_SetLayerDrawFuncPtr(MGL_DrawLayerFuncPtr fpDrawLayer);
/**
 * @brief custom layer�� ����� ���۸� �����Ѵ�.
 * 
 * @param[in] pBuffer ����� ����. ������ Ÿ���� porting�ÿ� platform�� ���� ��������.
 */
MGLvoid MGL_SetCustomLayerBuffer(MGLvoid * pBuffer);
/**
 * @brief custom layer�� layer�� �׸��� �Լ��� �ʿ��� parameter ����ü�� �����Ѵ�.
 * 
 * @param[in] pParams ����ü�� ������
 */
MGLvoid MGL_SetCustomLayerParams(MGLvoid * pParams);
/**
 * @brief ���۸� device context�� ������ŭ ��������� �����. 
 *
 * @param[in] pBuffer ������ ������
 */
MGLvoid MGL_ClearBuffer(MGLvoid * pBuffer);
/**
 * @brief ���۸� rendering context�� ������ŭ ��������� �����. 
 *
 * @param[in] pBuffer ������ ������
 * @param[in] pRC rendering context�� ������
 */
MGLvoid MGL_ClearBufferRC(MGLvoid *pBuffer, MGLrc * pRC);
/**
 * @brief ���۸� layer�� ������ŭ ��������� �����. 
 *
 * @param[in] pBuffer ������ ������
 * @param[in] pLayer layer�� ������
 */
MGLvoid MGL_ClearBufferLayer(MGLvoid *pBuffer, MGLlayer * pLayer);

// rendering function
/**
 * @brief ������ rendering context�� �׸���.
 */
MGLvoid MGL_Render(MGLvoid);

/**
 * @brief ������ rendering context�� ������ ���ۿ� �׸���.
 *
 * @param[in] pBuffer ������ ������
 */
MGLvoid MGL_RenderToBuffer(MGLvoid * pBuffer);

// timing related function
/** 
 * @brief �ʴ� Tick���� ���Ѵ�.
 *
 * @return Tick��
 */
MGLulong MGL_TicksForSecond( MGLvoid );

/** 
 * @brief ������ Tick���� ���Ѵ�.
 *
 * @return Tick��
 */
MGLulong MGL_GetTicks( MGLvoid );

/*
 * memory managing function
 */
/** 
 * @brief	dest �޸� ������ ������ ��(value)���� �־��� ũ�⸸ŭ ä���. 
 *
 * @param dest ä�� �޸� �ּ�
 * @param value ä�� �� 
 * @param size ũ��
 */
MGLvoid MGL_MemSet( MGLvoid * dest, MGLint value, MGLint size);

/** 
 * @brief	dest �޸� ������ ���� src �޸� ������ ������ ä���. 
 *
 * @param dest ä�� �޸� �ּ�
 * @param src ���� �������� �޸� �ּ�
 * @param size ũ��
 */
MGLvoid MGL_MemCopy( MGLvoid * dest, MGLvoid * src, MGLint size);

/*
 * 2D Primitive API
 */
/** 
 * @brief	Layer�� ������ �ٲ۴�. (�� �������� �׸������� ��ȿ��.)
 *
 * @param color	�ٲٷ��� ����.
 * @return	���� ����.
 */
MGLcolor MGL_SetBgColor( MGLcolor color );

/** 
 * @brief	���� Layer�� ������� �ٲ۴�. 
 *
 * @param color	�ٲٷ��� ����.
 * @return	���� ����.
 */
MGLcolor MGL_SetFgColor( MGLcolor color );

/** 
 * @brief	���� Layer�� ������ ���´�.  
 *
 * @param	����.
 * @return	���� ����.
 */
MGLcolor MGL_GetBgColor( MGLvoid );

/** 
 * @brief	���� Layer�� ������� ���´�.  
 * 
 * @param	����.
 * @return	���� ����.
 */
MGLcolor MGL_GetFgColor( MGLvoid );

/** 
 * @brief	�־��� ��ǥ�� �ȼ� ���� ���� �ٲ۴�. 
 *
 * @param x		X ��ǥ 
 * @param y		Y ��ǥ 
 * @param color	������ ��
 * @return	�־��� ��ǥ�� ���� ���� 
 */
MGLcolor MGL_SetPixel( MGLint x, MGLint y, MGLcolor color );

/** 
 * @brief	�־��� ��ǥ�� �ȼ� ���� ���� ���´�. 
 *
 * @param x		X ��ǥ 
 * @param y		Y ��ǥ 
 * @return	�־��� ��ǥ�� ���� ��.
 */
MGLcolor MGL_GetPixel( MGLint x, MGLint y );

/** 
 * @brief	�� ���� �մ� ���� �׸���. 
 *
 * @param x1	���� ���� X ��ǥ
 * @param x2	���� ���� Y ��ǥ
 * @param y1	���� �� X ��ǥ
 * @param y2	���� �� Y ��ǥ
 */
MGLvoid MGL_DrawLine( MGLint x1, MGLint y1, MGLint x2, MGLint y2 );

/** 
 * @brief	�簢���� �׸���. 
 *
 * @param left		���� ��ǥ : left
 * @param top		���� ��ǥ : top
 * @param width		X�� �ʺ�
 * @param height	Y�� ���� 
 */
MGLvoid	MGL_DrawRect( MGLint left, MGLint top, MGLint width, MGLint height );

/** 
 * @brief	���ϴ� ������ ä���� �ִ� �簢���� �׸���.
 *
 * @param left		���� ��ǥ : left
 * @param top		���� ��ǥ : top
 * @param width		X�� �ʺ�
 * @param height	Y�� ���� 
 * @param color		ä�� ���� ��. 
 */
MGLvoid	MGL_FillRect( MGLint left, MGLint top, MGLint width, MGLint height, MGLcolor color );

/** 
 * @brief	���� �׸���. 
 *
 * @param x	�߽��� X ��ǥ 
 * @param y �߽��� Y ��ǥ 
 * @param r ���� ������
 */
MGLvoid	MGL_DrawCircle( MGLint x, MGLint y, MGLint r );

/** 
 * @brief	Ÿ���� �׸���. 
 *
 * @param x		�߽��� X ��ǥ 
 * @param y		�߽��� Y ��ǥ 
 * @param xr	X�� ������
 * @param yr	Y�� ������
 */
MGLvoid	MGL_DrawEllipse( MGLint x, MGLint y, MGLint xr, MGLint yr );


/*
 * Bitmap API
 */

/**
 * @brief	���� ���õ� layer�� bitmap�� �׸���. 
 *
 * @param pBitmap	bitmap data
 * @param x			bitmap�� �׸� X ��ǥ
 * @param y			bitmap�� �׸� Y ��ǥ
  */
MGLvoid MGL_DrawBitmap( MGLbitmap * pBitmap, MGLint x, MGLint y );

/** 
 * @brief	�ҽ� ������ �����͸� �׸����� �ϴ� ���� �׷��ش�. 
 *
 * @param pDst		�׷������ϴ� ������ ���� ������
 * @param dst_x		�׷������ϴ� ���� : X ��ǥ
 * @param dst_y		�׷������ϴ� ���� : Y ��ǥ
 * @param width		�׷������ϴ� ���� : width
 * @param height	�׷������ϴ� ���� : height
 * @param pSrc		�ҽ� ������ ���� ������
 * @param src_x		�ҽ� ���� : X ��ǥ
 * @param src_y		�ҽ� ���� : Y ��ǥ
 */
MGLvoid MGL_BitBlt( MGLuint * pDst, MGLint dst_x, MGLint dst_y, MGLint width, MGLint height, 
				    MGLuint * pSrc, MGLint src_x, MGLint src_y );

/** 
 * @brief	Bitmap �����͸� �����´�. 
 *
 * @param id		�����͸� �������� Bitmap ID
 * @param pBitmap	Bitmap ������ ���� ����
 */
MGLvoid MGL_LoadBitmap( MGLint id, MGLbitmap *  pBitmap );

/**
 * @brief image processor�� image processing�� ������ �Լ��� ������
 */
#define MGL_IP_DO_FUNC						0x0001
/**
 * @brief image processor�� parameter�� ���ϴ� �Լ��� ������
 */
#define MGL_IP_GET_PARAM_FUNC				0x0011
/**
 * @brief image processor�� parameter�� �����ϴ� �Լ��� ������
 */
#define MGL_IP_SET_PARAM_FUNC				0x0012
/**
 * @brief image processor�� boolean ������ �Ӽ����� ���ϴ� �Լ��� ������
 */
#define MGL_IP_GET_BOOLEAN_PROP_FUNC		0x0021
/**
 * @brief image processor�� int ������ �Ӽ����� ���ϴ� �Լ��� ������
 */
#define MGL_IP_GET_INT_PROP_FUNC			0x0022

/**
 * @brief void Ÿ��
 */
#define MGL_TYPE_VOID						0x0000
/**
 * @brief boolean Ÿ��
 */
#define MGL_TYPE_BOOLEAN					0x0001
/**
 * @brief char Ÿ��
 */
#define MGL_TYPE_CHAR						0x0002
/**
 * @brief short Ÿ��
 */
#define MGL_TYPE_SHORT						0x0003
/**
 * @brief int Ÿ��
 */
#define MGL_TYPE_INT						0x0004
/**
 * @brief long Ÿ��
 */
#define MGL_TYPE_LONG						0x0005
/**
 * @brief float Ÿ��
 */
#define MGL_TYPE_FLOAT						0x0006
/**
 * @brief double Ÿ��
 */
#define MGL_TYPE_DOUBLE						0x0007

/**
 * @brief unsigned char Ÿ��
 */
#define MGL_TYPE_UCHAR						0x0102
/**
 * @brief unsigned short Ÿ��
 */
#define MGL_TYPE_USHORT						0x0103
/**
 * @brief unsigned int Ÿ��
 */
#define MGL_TYPE_UINT						0x0104
/**
 * @brief unsigned long Ÿ��
 */
#define MGL_TYPE_ULONG						0x0105

/**
 * @brief void * Ÿ��
 */
#define MGL_TYPE_VOID_PTR					0x1000
/**
 * @brief char * Ÿ��
 */
#define MGL_TYPE_CHAR_PTR					0x1002
/**
 * @brief unsigned char * Ÿ��
 */
#define MGL_TYPE_UCHAR_PTR					0x1102

/**
 * @brief error code���� �����ϴ� Ÿ��
 */
#define MGL_TYPE_ERROR						0x2001
/**
 * @brief fixed point���� �����ϴ� Ÿ��
 */
#define MGL_TYPE_FIXED						0x2002
/**
 * @brief color���� �����ϴ� Ÿ��
 */
#define MGL_TYPE_COLOR						0x2003

/**
 * @brief �˼� ���� Ÿ��
 */
#define MGL_TYPE_UNKNOWN					0xFFFF

/**
 * @brief �ݵ�� dest buffer�� mix�ؾ� �ϴ� image process
 *
 * ��) alpha blending, drop shadow, blur
 */
#define MGL_IP_PROP_DEST_MIX		1

/**
 * @brief FPS �˻翩�θ� ���Ѵ�.
 *
 * @param[in] bEnabled TRUE�̸� FPS�� �˻��ϰ� �޽����� ����Ѵ�.
 */
MGLvoid MGL_SetFPSEnalbed(MGLboolean bEnabled);

/**
 * @brief FPS�� �ʱ�ȭ�Ѵ�.
 */
MGLvoid MGL_ResetFPS(MGLvoid);

/**
 * @brief FPS���� �˻��Ѵ�. 
 *
 * �������ο��� MGL_Render, MGL_RenderToBuffer �� ȣ��� �� �� �Լ��� ȣ��Ǹ� frame count�� �����ϰ�, ������ �ð��� ������ fps���� ����� �޽����� ����Ѵ�.
 */
MGLvoid MGL_CheckFPS(MGLvoid);

/** 
 * @brief Assert ���¸� �˻��Ѵ�. ���ο����� ���ǹǷ� MGL_Assert �� ��� ����Ѵ�.
 *
 * @attention ����׿� ���������� �۵��Ѵ�.
 * @param[in] nCondition 0 �Ǵ� �׿��� ������ ���� ���ǽ��� ������� �־��ش�. 
 * @param[in] pszFile filename
 * @param[in] pszFunc funtion name
 * @param[in] nLine line number
 */
MGLvoid MGL_AssertInternal(MGLint nCondition, MGLbyte * pszFile, MGLbyte * pszFunc, MGLint nLine );

/** 
 * @brief	Assert ���¸� �˻��Ѵ�.  
 *
 * @attention ����׿� ���������� �۵��Ѵ�.
 * @param[in] nCondition 0 �Ǵ� �׿��� ������ ���� ���ǽ��� ������� �־��ش�. 
 */
#ifdef MGL_CONFIG_USE_DEBUG_INFO
	#define MGL_Assert(nCondition) MGL_AssertInternal(nCondition, __MGL_FILE__, __MGL_FUNC__, __MGL_LINE__)
#else
	#define MGL_Assert(nCondition)
#endif

/**
 * @brief ����׿� �޽����� printf ��Ŀ� �°� ����Ѵ�.
 *
 * @attention ����׿� ���������� �۵��Ѵ�.
 * @param[in] format ����� ���. printf�� �����ϴ�.
 * @param[in] ... ���ڰ���
 */
MGLvoid MGL_DebugPrintF(MGLbyte * format, ...);

/**
 * @brief ����׿� �޽����� ����Ѵ�.
 *
 * @attention ����׿� ���������� �۵��Ѵ�.
 * @param[in] msg ����� �޽���
 */
MGLvoid MGL_DebugMsg(MGLbyte * msg);

/**
 * @brief ���ڿ�(MGLbyte *)������ Global �Ӽ����� ���Ѵ�. @ref globpropids �� �����Ѵ�.
 * 
 * @param[in] nPropID �Ӽ� ID
 * @return MGLbyte * ������ �Ӽ���
 */
MGLbyte * MGL_GetStringProperty(MGLint nPropID);

/**
 * @brief ����(MGLint)������ Global �Ӽ����� ���Ѵ�. @ref globpropids �� �����Ѵ�.
 * 
 * @param[in] nPropID �Ӽ� ID
 * @return MGLint ������ �Ӽ���
 */
MGLint MGL_GetIntProperty(MGLint nPropID);

/**
 * @brief �Ҹ�(MGLboolean)������ Global �Ӽ����� ���Ѵ�. @ref globpropids �� �����Ѵ�.
 * 
 * @param[in] nPropID �Ӽ� ID
 * @return MGLboolean ������ �Ӽ���
 */
MGLboolean MGL_GetBooleanProperty(MGLint nPropID);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __mgl_h_
