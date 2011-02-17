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
#define TRUE (1)		/**< 참 값 */
#endif

#ifndef FALSE
#define FALSE (0)		/**< 거짓 값 */
#endif

/**
 * @defgroup api_types Types
 *
 * 기본 type에 대한 정의
 *
 * @addtogroup api_types 
 */

/**
 * @brief emueration값
 */
typedef unsigned int MGLenum;		
/**
 * @brief TRUE, FALSE 중 하나의 값을 갖는 타입
 */
typedef unsigned char MGLboolean;
/**
 * @brief
 */
typedef unsigned int MGLbitfield;	/**< bit로 된 필드 타입 */
/**
 * @brief
 */
typedef char MGLbyte;		/**< byte의 크기의 타입 */
/**
 * @brief long 크기의 타입
 */
typedef short MGLlong;
/**
 * @brief
 */
typedef short MGLshort;			/**< short 크기의 타입 */
/**
 * @brief
 */
typedef int MGLint;				/**< int 크기의 타입 */
/**
 * @brief
 */
typedef int MGLsizei;				/**< size를 나타내는 타입 */
/**
 * @brief
 */
typedef unsigned char MGLubyte;	/**< unsigned byte 크기의 타입 */
/**
 * @brief unsigned long 크기의 타입
 */
typedef unsigned long MGLulong;
/**
 * @brief
 */
typedef unsigned short MGLushort;	/**< unsigned short 크기의 타입 */
/**
 * @brief
 */
typedef unsigned int MGLuint;		/**< unsigned int 크기의 타입 */
/**
 * @brief
 */
typedef float MGLfloat;			/**< float 크기의 타입 */
/**
 * @brief
 */
typedef double MGLdouble;			/**< double 크기의 타입 */
/**
 * @brief
 */
typedef void MGLvoid;				/**< void 타입 */

/**
 * @}
 */
 
/**
 * @defgroup api_errhandling Error Handling
 *
 * MGL에서 사용되는 error code 및 error 관련 API
 * @see MGLerrmsg, MGL_SetError, MGL_GetError
 *
 * @addtogroup api_errhandling
 * @{
 */
 
typedef int MGLerror;				/**< error 값의 타입 */
	
/**
 * @brief 에러가 발생되지 않음
 */
#define MGL_ERROR_NONE					0
/**
 * @brief 일반적인 에러코드 영역 시작
 */
#define MGL_ERRORRANGE_GENERIC_START		0x0001
/**
 * @brief 범위를 벗어남
 */
#define MGL_ERROR_OUT_OF_RANGE				0x0001
/**
 * @brief parameter를 찾지 못하는 경우 발생함
 */
#define MGL_ERROR_PARAMETER_NOT_FOUND		0x0003
/**
 * @brief 일반적인 에러코드 영역 끝
 */
#define MGL_ERRORRANGE_GENERIC_END			(MGL_ERRORRANGE_GENERIC_START + 0x0fff)

/**
 * @brief Core Module용 에러코드 영역 시작
 */
#define MGL_ERRORRANGE_CORE_START			0x1001
/**
 * @brief engine을 초기화하는 도중에 문제가 생기는 경우에 발생
 */
#define MGL_ERROR_ENGINE_INIT_FAILURE		0x1002
/**
 * @brief bitmap을 읽어오지(loading) 못하는 경우에 발생
 */
#define MGL_ERROR_BITMAP_LOAD_FAILURE		0x1003
/**
 * @brief 호출한 API가 layer의 속성에 맞지 않는 경우에 발생
 */
#define MGL_ERROR_INVALID_LAYER				0x1004
/**
 * @brief 알 수 없는 속성 ID인 경우에 발생
 */
#define MGL_ERROR_UNKNOWN_PROP_ID			0x1005
/**
 * Core Module용 에러코드 영역 끝
 */
#define MGL_ERRORRANGE_CORE_END				(MGL_ERRORRANGE_CORE_START + 0x0fff)

/**
 * Image Process Module용 에러코드 영역 시작
 */
#define MGL_ERRORRANGE_IP_START				0x2001
/**
 * @brief 알수 없는 ip 함수를 호출하는 경우에 발생
 */
#define MGL_ERROR_UNKNOWN_IP_FUNC			0x2002
/**
 * @brief 알수 없는 ip의 인자값(parameter)의 값을 사용하려는 경우에 발생
 */
#define MGL_ERROR_UNKNOWN_IP_PARAM			0x2003
/**
 * @brief 알수 없는 ip의 속성값(property)을 사용하려는 경우에 발생
 */
#define MGL_ERROR_UNKNOWN_IP_PROP			0x2004
/**
 * @brief 정해진 범위를 넘어가서 값을 사용한 경우에 발생
 */
#define MGL_ERROR_OUT_OF_BOUND				0x2005
/**
 * @brief Image Process Module용 에러코드 영역 끝
 */
#define MGL_ERRORRANGE_IP_END				(MGL_ERRORRANGE_IP_START + 0x0fff)

/**
 * @brief Animator Module용 에러코드 영역 시작
 */
#define MGL_ERRORRANGE_ANIMATOR_START		0x3000
/**
 * @brief Animation이 종료됨
 */
#define MGL_ERROR_ANIMATION_END				0x3001
/**
 * @brief 설정값을 찾을 수 없음
 */
#define MGL_ERROR_NOT_FOUND					0x3002
/**
 * @brief 지원되지않는 PARAM TYPE
 */
#define MGL_ERROR_NOT_SUPPORTED_PARAM_TYPE	0x3003
/**
 * @brief 지원되지않는 PARAM ID
 */
#define MGL_ERROR_NOT_SUPPORTED_PARAM_ID	0x3004
/**
 * @brief 지원되지않는 옵션  
 */
#define MGL_ERROR_NOT_SUPPORTED_OPTION		0x3005
/**
 * @brief Animator Module용 에러코드 영역 끝
 */
#define MGL_ERRORRANGE_ANIMATOR_END			(MGL_ERRORRANGE_ANIMATOR_START + 0x0fff)

/**
 * @brief 사용자 지정 에러코드 영역 시작
 */
#define MGL_ERRORRANGE_USER_DEFINED_START	0x8000
/**
 * @brief 사용자 지정 에러코드 영역 끝
 */
#define MGL_ERRORRANGE_USER_DEFINED_END		(MGL_ERRORRANGE_USER_DEFINED_START + 0x0fff)

/**
 * @brief 알수 없는 에러
 */
#define MGL_ERROR_UNKNWON					0xffff

/**
 * @brief 발생한 error 정보를 설정한다. 
 *
 * @attention 이 함수는 내부적으로 사용되므로 MGL_SetError 를 사용한다.
 * @param[in] error error code
 * @param[in] pszFile error가 발생한 소스의 파일명
 * @param[in] pszFunc error가 발생한 소스의 함수
 * @param[in] nLine error가 발생한 소스의 줄번호
 * @see MGL_SetError, MGL_GetError, MGLerrmsg
 */
MGLvoid  MGL_SetErrorInfo( MGLerror error, MGLbyte * pszFile, MGLbyte * pszFunc, MGLint nLine );

/**
 * @brief 최근 발생한 error code를 구한다.
 *
 * @return error code
 * @see MGL_SetError
 */
MGLerror MGL_GetError( MGLvoid );

/**
 * @brief 발생한 error code를 설정한다.
 *
 * error 설정을 좀 더 편하기 위해 사용된다. 
 * @param[in] error error code
 * @see MGL_GetError
 * @note MGL_SetErrorInfo의 매개 변수에 __MGL_FILE__, __MGL_FUNC__, __MGL_LINE__ 을 사용한 매크로이다.
 *		FILE, FUNC, LINE의 값은 MGL_SetError(..) 함수가 있는 곳으로 무의미 할 수 있다. 
 */
#define MGL_SetError(error) MGL_SetErrorInfo((MGLerror)error, ( MGLbyte * )__MGL_FILE__, (MGLbyte *)__MGL_FUNC__, (MGLint)__MGL_LINE__)

/**
 * @}
 */

/**
 * @defgroup apilayertype Layer Type ID
 *
 * MGLlayer::type값으로 사용된다.
 * @see MGLlayer
 *
 * @addtogroup apilayertype
 * @{
 */
/**
 * @brief image layer의 type값
 */
#define MGL_IMAGE_LAYER				0x1
/**
 * @brief external image layer의 type값
 */
#define MGL_EXTERNAL_IMAGE_LAYER	0x2
/**
 * @brief custom layer의 type값
 */
#define MGL_CUSTOM_LAYER			0x3
/**
 * @}
 */ 
 
/**
 * @defgroup globpropids Global Property IDs
 *
 * MGL_GetStringProperty, MGL_GetIntProperty, MGL_GetBooleanProperty 의 nPropID 값으로 사용된다.
 *
 * @addtogroup globpropids
 * @{
 */

/**
 * @brief 시스템 이름
 */
#define MGL_SYS_PROP_STR_NAME				0x0001
/**
 * @brief 시스템 작성자
 */
#define MGL_SYS_PROP_STR_AUTHOR				0x0002
/**
 * @brief 시스템의 major version number
 */
#define MGL_SYS_PROP_INT_MAJOR_VERSION		0x0011
/**
 * @brief 시스템의 middle version number
 */
#define MGL_SYS_PROP_INT_MIDDLE_VERSION		0x0012
/**
 * @brief 시스템의 minor version number
 */
#define MGL_SYS_PROP_INT_MINOR_VERSION		0x0013
/**
 * @brief 시스템의 build number
 */
#define MGL_SYS_PROP_INT_BUILD_NUMBER		0x0014
/**
 * @brief 시스템의 build 날짜 및 시간
 */
#define MGL_SYS_PROP_STR_BUILD_DATE			0x0015
/**
 * @brief 타겟 속성 플래그
 */
#define MGL_TARGET_PROP_FLAG				0x1000
/**
 * @brief 타겟의 H/W 모델 설명
 */
#define MGL_TARGET_PROP_STR_HW_MODEL		0x1001
/**
 * @brief 타겟의 H/W 중 CPU 설명
 */
#define MGL_TARGET_PROP_STR_HW_CPU			0x1002
/**
 * @brief 타겟의 H/W 중 Memory 설명
 */
#define MGL_TARGET_PROP_STR_HW_MEMORY		0x1003
/**
 * @brief 타겟의 H/W 중 GPU 설명
 */
#define MGL_TARGET_PROP_STR_HW_GPU			0x1004
/**
 * @brief 타겟의 S/W 중 OS 설명
 */
#define MGL_TARGET_PROP_STR_SW_OS			0x1101
/**
 * @brief 타겟의 S/W 중 GUI Platform 설명
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
 * @brief error 정보 구조체
 *
 * 이 구조체는 device context에 포함되며 MGL_GetError 를 호출하여 에러정보를 가져올 수 있다.
 * @see MGL_SetError, MGL_GetError, MGLdc
 */
typedef struct MGLerrmsg
{
	MGLerror	error;			/**< error code */
	MGLbyte		szFile[128];	/**< error가 발생한 파일 이름 */
	MGLbyte		szFunc[128];	/**< error가 발생한 함수 이름 */
	MGLint		nLine;			/**< error가 발생한 라인 */
} MGLerrmsg;

#if MGL_COLOR_RGB565	
	typedef unsigned short MGLcolor;	/**< 색상값을 나타내기 위한 타입  */
	#define MGL_R_MASK	0xf800			/**< 색상값에서 빨간색 mask 값  */
	#define MGL_G_MASK	0x07e0			/**< 색상값에서 녹색 mask 값  */
	#define MGL_B_MASK	0x001f			/**< 색상값에서 파란색 mask 값  */
	#define MGL_MakeColor( r, g, b )	(MGLcolor)((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))	/**< 색상값 만들기 매크로  */
#else
	typedef unsigned long MGLcolor;
#endif	// MGL_COLOR_RGB565

#define MGL_GetColorR(col) (col) & MGL_R_MASK	/**< 주어진 색상에서 빨간색의 값만 가져옴 */
#define MGL_GetColorG(col) (col) & MGL_G_MASK	/**< 주어진 색상에서 녹색의 값만 가져옴 */
#define MGL_GetColorB(col) (col) & MGL_B_MASK	/**< 주어진 색상에서 파란색의 값만 가져옴 */

/**
 * @brief bitmap 구조체
 */
typedef struct MGLbitmap
{
	MGLint		width;				/**< bitmap width */
	MGLint		height;				/**< bitmap height */
	MGLboolean	bTransparent;		/**< 투명색 사용여부 */
	MGLcolor	nTrColor;			/**< 투명색깔값 */
	MGLint		bpp;				/**< bit per pixel값 */
	MGLvoid *	data;				/**< bitmap raw 데이터 영역의 포인터 */
} MGLbitmap;

/**
 * @brief image processor의 대표(main) 함수 포인터
 *
 * @param[in] type image processor의 하위(sub) 함수의 타입
 * @return 하위 함수의 포인터 
 * @see MGLip
 */
typedef MGLvoid *(*MGL_IPFuncPtr)(MGLint type);

/**
 * @brief image processor 구조체
 *
 * image processor를 대표하는 함수의 포인터와 인자값 구조체의 포인터 정보를 가지고 있다.
 * @see @ref page_ip, MGL_IPFuncPtr, MGL_DoIPFuncPtr, MGL_SetIPParamFuncPtr, MGL_GetIPIntPropFuncPtr, MGL_GetIPBooleanPropFuncPtr
 */
typedef struct MGLip
{
	MGL_IPFuncPtr				fpIP;		/**< image processor를 대표하는 함수의 포인터 */
	MGLvoid *					pParams;	/**< image processor에 필요한 인자값 구조체의 포인터 */
} MGLip;

/**
 * @brief image processor의 MGLint 형태의 속성값을 구하는 함수 포인터
 *
 * @param[in] pIP image processor 구조체의 포인터
 * @param[in] nPropID 속성의 ID값
 * @return MGLint형태의 속성값
 * @see MGLip, MGL_IPFuncPtr
 */
typedef MGLint (*MGL_GetIPIntPropFuncPtr)(MGLip * pIP, MGLint nPropID);
/**
 * @brief image processor의 MGLboolean 형태의 속성값을 구하는 함수 포인터
 *
 * @param[in]  pIP image processor 구조체의 포인터
 * @param[in] nPropID 속성의 ID값
 * @return MGLboolean형태의 속성값
  * @see MGLip, MGL_IPFuncPtr
*/
typedef MGLboolean (*MGL_GetIPBooleanPropFuncPtr)(MGLip * pIP, MGLint nPropID);
/**
 * @brief image processor의 image processing 하는 함수 포인터 타입
 *
 * @param[in] pSrcBuffer image processing에 필요한 원본 버퍼
 * @param[in,out] pDestBuffer image processing에 필요한 출력 버퍼
 * @param[in,out]  nX image processing이 행해지는 영역값의 포인터. 영역이 변경될 수 있기 때문에 포인터를 넘긴다.
 * @param[in,out] nY image processing이 행해지는 영역값의 포인터. 영역이 변경될 수 있기 때문에 포인터를 넘긴다.
 * @param[in,out] pWidth image processing이 행해지는 영역값의 포인터. 영역이 변경될 수 있기 때문에 포인터를 넘긴다.
 * @param[in,out] pHeight image processing이 행해지는 영역값의 포인터. 영역이 변경될 수 있기 때문에 포인터를 넘긴다.
 * @param[in] pParam image processor별로 필요한 추가적인 인자값 구조체의 포인터
 * @see MGLip, MGL_IPFuncPtr
 */
typedef MGLvoid (*MGL_DoIPFuncPtr)(MGLvoid * pSrcBuffer, MGLvoid * pDestBuffer, MGLint * pX, MGLint * pY, MGLint * pWidth ,MGLint * pHeight, MGLvoid * pParams);

/**
 * @brief image processor의 paramter를 구하는 함수의 포인터 타입
 *
 * @param[in] pIP image processor의 포인터
 * @param[in] nParamID parameter의 ID
 * @param[out] pnParamType paramter의 타입에 대한 포인터
 * @param[out] pParam paramter 값
 */
typedef MGLvoid (*MGL_GetIPParamFuncPtr)(MGLip * pIP, MGLint nParamID, MGLint * pnParamType, MGLvoid * pParam);

/**
 * @brief image processor의 인자값을 설정한다.
 *
 * @param[in] pIP image processor의 포인터
 * @param[in] nParamID 인자값의 ID
 * @param[in] pParam 인자값
 * @see MGLip, MGL_IPFuncPtr
 */
typedef MGLvoid (*MGL_SetIPParamFuncPtr)(MGLip * pIP, MGLint nParamID, MGLvoid * pParam);

/**
 * @brief bitmap 정보를 가진 layer
 *
 * @see MGLlayer
 */
typedef MGLbitmap		MGLimagelayer;

struct MGLlayer;
/**
 * @brief image layer 정보를 업데이트하는 함수 포인터
 *
 * @param layer MGLlayer pointer
 * @param pParams 업데이트 함수에서 사용할 인자 구조체의 포인터
 * @return FALSE이면 업데이트 함수를 지속적으로 호출한다. TRUE이면 더 이상 업데이트 함수를 호출하지 않는다.
 * @see MGLlayer
 */
typedef MGLboolean (* MGL_UpdateImageLayerFuncPtr)(MGLint index, struct MGLlayer * layer, MGLvoid * pParams);

/**
 * @brief 외부의 함수를 통해 bitmap 정보를 업데이트하게 하는 layer
 *
 * @see MGLlayer, MGLimagelayer. MGL_UpdateImageLayerFuncPtr
 */
typedef struct MGLextimagelayer
{
	MGLint						count;					/**< 총 업데이트 함수 호출 회수 */
	MGL_UpdateImageLayerFuncPtr	fpUpdateImageLayer;		/**< 업데이트 함수의 포인터 */
	MGLvoid *					pParams;				/**< 업데이트 함수 호출시 필요한 parameter 구조체의 포인터 */
} MGLextimagelayer;

/**
 * @brief layer를 그리는 함수의 포인터
 *
 * @see MGLlayer, MGLcustomlayer
 */
typedef MGLvoid (*MGL_DrawLayerFuncPtr)(MGLvoid * pBuffer, MGLvoid * pParams);

/**
 * @brief 외부의 함수를 통해 그려지게 하는 layer
 *
 * @see MGLlayer, MGL_DrawLayerFuncPtr
 */
typedef struct MGLcustomlayer
{
	MGL_DrawLayerFuncPtr		fpDrawLayer;	/**< 사용자로 하여금 layer를 그리게 하는 함수의 포인터. */
	MGLvoid *					pBuffer;		/**< 사용자가 정의하는 버퍼. NULL이면 사용하지 않는다. @attention 만약 사용하는 경우, 이 layer에는 image processing이 되지 않는다. */
	MGLvoid *					pParams;		/**< custom layer에 필요한 사용자 정의 paramter */
} MGLcustomlayer;

/**
 * @brief layer 구조체
 *
 * layer에는 타입, 좌표, 범위 등의 기본 정보와 타입에 따른 부가 정보를 가지고 있다.
 * @see MGLdc, MGLimagelayer, MGLextimagelayer, MGLcustomlayer, MGLip
 */
typedef struct MGLlayer
{
    MGLenum		type;				/**< layer의 종류. @ref apilayertype 를 참고한다. */
	MGLint		x;					/**< layer의 영역 */
	MGLint		y;					/**< layer의 영역 */
	MGLint		width;				/**< layer의 영역 */
	MGLint		height;				/**< layer의 영역 */
	MGLint		nNumIPs;			/**< image processor들의 총 개수 */
	MGLip *		pIPs;				/**< image processor 배열의 포인터 */
	MGLboolean	bVisible;			/**< 표시(rendering) 여부. false이면 표시하지 않는다. */
	union
	{
		MGLimagelayer			tImage;			/**< image layer 정보 */
		MGLextimagelayer		tExtImage;		/**< external image layer 정보 */
		MGLcustomlayer			tCustom;		/**< custom layer 정보 */
	} data;							/**< layer 종류별 데이터 */
} MGLlayer;


/**
 * @brief rendering context 구조체. RC라고 줄여서 사용한다.
 *
 * rendering context는 여러개의 layer 들에 대한 정보를 가지고 있으며, RC를 사용하려면 MGL_SetDCCurRC 를 호출하여 device context에  현재 사용중인 rendering context 를 등록해야 한다.
 * @see MGLdc, MGL_SetDCCurRC, MGLlayer
 */
typedef struct MGLrc
{
	MGLint		x;					/**< rendering buffer 안의 그려지는 영역 (topleft 좌표)  */
	MGLint		y;					/**< rendering buffer 안의 그려지는 영역 (topleft 좌표)  */
    MGLsizei	width;				/**< rendering buffer 안의 그려지는 영역  */
    MGLsizei	height;				/**< rendering buffer 안의 그려지는 영역  */

	MGLlayer *	pCurLayer;			/**< 현재 사용하는 layer의 포인터 */
	MGLint		nNumLayer;			/**< RC가 포함하는 layer의 개수. */
	MGLlayer *	pLayers;			/**< RC가 관리하는 layer 배열의 포인터 */
} MGLrc;

/**
 * @brief buffer 들 구조체 
 *
 * rendering/working buffer 들에 대한 정보를 담고 있다.
 */
typedef struct MGLbuffers
{
	MGLvoid *	pRenderBuffer;		/**< rendering buffer. platform에 출력되는 버퍼 */
	MGLint		nNumWorkBuffers;	/**< working buffer의 총 갯수. 최소 2개 이상있어야 image processing이 가능하다. */
	MGLvoid **	pWorkBuffers;		/**< working buffer 배열의 포인터. layer 하나를 그리기 위해 임시로 사용하는 버퍼 배열. image processing이 있는 경우 사용하며 2개를 서로 번갈아 사용된다. */
	MGLvoid *	pTempBuffer;		/**< MGL_RenderBuffer()를 위해 임시로 사용하는 버퍼 */
} MGLbuffers;

/**
 * @brief device context 구조체. 줄여서 DC로 사용한다.
 *
 * device context에는 rendering buffer에 대한  정보, 현재 사용중인 rendering context, 에러 정보 등을 담고 있다.
 * @see MGLrc, MGLbuffers, MGLerrmsg
 */
typedef struct MGLdc
{
    MGLsizei	width;				/**< rendering buffer width */
    MGLsizei	height;				/**< rendering buffer height */
	MGLcolor	transparent;		/**< transparent color */
	MGLint		count;				/**< rendering context의 총 개수 */
	MGLrc *		pRCs;				/**< rendering context 배열의 포인터 */
	MGLrc *		pCurRC;				/**< 현재 사용하는 rendering context의 포인터 */
	MGLbuffers	tBuffers;			/**< buffer 정보 */
	MGLerrmsg	tError;				/**< 에러 정보 */
	MGLboolean	bFPSEnabled;		/**< true이면 frames per second를 조사하게 하고 출력하게 한다. */
	MGLulong	nFPSTicks;			/**< frames per second를 조사하기 위해 필요한 값으로 어느 동안의 시간(tick) */
	MGLint		nFPSCount;			/**< frames per second를 조사하기 위해 필요한 값으로 정해진 시간 동안에 그려지는 frame 회수 */
} MGLdc;

/*
 * Math API
 */
#define MGL_FIXEDPOINT_PRECISION	10 /**< fixedpoint 형에서 사용하는 정밀도 값 */


#ifdef MGL_CONFIG_USE_FIXED_POINT
	typedef int							MGLfixed; /**< fixed point 타입을 int 로 한다. */
	#define MGL_Int2Fixed(x)			((x) << MGL_FIXEDPOINT_PRECISION) /**< 정수형을 fixed point 형으로 변환한다. */
	#define MGL_Fixed2Int(x)			((x) >> MGL_FIXEDPOINT_PRECISION) /**< fixedpoint 형을 정수형으로 변환한다. */
	#define MGL_Float2Fixed(x)			((MGLfixed)((x) * (1 << MGL_FIXEDPOINT_PRECISION))) /**< 부동소수형을 fixed point 형으로 변환한다. */
	#define MGL_Fixed2Float(x)			(((double)(x)) / (1 << MGL_FIXEDPOINT_PRECISION)) /**< fixed point 형을 부동소수점형으로 변환한다. */
	#define MGL_Fraction(x)				((x) & ((1 << MGL_FIXEDPOINT_PRECISION) - 1)) /**< 소수부를 구함  */
	#define MGL_Mul(x, y)				(((x) * (y)) >> MGL_FIXEDPOINT_PRECISION) /**< fixed point 형 곱하기 연산 */
	#define MGL_Div(x, y)				(((x) << MGL_FIXEDPOINT_PRECISION) / (y)) /**< fixed point 형 나누기 연산 */
	#define MGL_Div2(x, y)				(((x) / (y)) << MGL_FIXEDPOINT_PRECISION) /**< fixed point 형 나누기 연산 */
	//#define MGL_Div(x, y)				(((x) * (1 << MGL_FIXEDPOINT_PRECISION)) / (y)) /**< fixed point 형 나누기 연산 */
	//#define MGL_Div(x, y)				((MGLfixed)((((long long)x) << MGL_FIXEDPOINT_PRECISION) / (y))) /**< fixed point 형 나누기 연산 */
	#define MGL_PI						MGL_Float2Fixed(3.14159265358979323846) /**< 파이값 */
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
 * @brief fixedpoint 형 더하기 연산
 */
#define		MGL_Add(x, y)				((x) + (y))

/**
 * @brief fixedpoint 형 빼기 연산
 */
#define		MGL_Sub(x, y)				((x) - (y))

/**
 * @brief Fixed point library를 사용할 때 정수로 나누는 함수 
 */
#define		MGL_DivByInt(x,y)			((x) / (y))

/** 
 * @brief 지수승 연산 함수 (x ^ n).
 * 
 * @param x 베이스
 * @param n 지수
 * @return x ^ n
 */
MGLfixed	MGL_Pow (MGLfixed x, MGLint n);

/** 
 * @brief 제곱근을 구하는 함수
 * 
 * @param x 제곱근을 구할 수 
 * @return x의 제곱근
 */	
MGLfixed	MGL_Sqrt (MGLfixed x);

/** 
 * @brief sin을 계산하는 함수.
 * 
 * @param deg 각도
 * @return deg 각에서의 sin 값
 */
MGLfixed	MGL_Sin (MGLfixed deg);

/** 
 * @brief cos을 계산하는 함수. 
 * 
 * @param deg 각도
 * @return deg 각에서의 cos 값
 */
MGLfixed	MGL_Cos (MGLfixed deg);

/** 
 * @brief 임의의 수를 얻기위해 사용하는 Seed 값을 설정한다. 
 * 
 * @param nSeed 설정 값
 */
MGLvoid		MGL_SetRandomSeed( MGLint nSeed );

/** 
 * @brief 주어진 Seed 값을 해당 범위 안의 사용하여 임의의 값을 얻어오는 함수 . 
 * 
 * @param nHigh 범위 중 가장 높은 수
 * @param nLow  범위 중 가장 낮은 수
 * @return 범위 안의 임의의 값
 */
MGLint		MGL_Random( MGLint nHigh, MGLint nLow );

/**
 * @brief engine을 초기화한다.
 * 
 * @param[in,out] data engine 내부에서 필요로 하는 data. porting시에 platform에 따라 정의가 다르다.
 */
MGLvoid MGL_InitEngine(MGLvoid * data);
/**
 * @brief engine을 종료한다.
 */
MGLvoid MGL_DeinitEngine( MGLvoid );

//  device context related functions
/**
 * @brief device context의 width를 구한다.
 *
 * @return device context의 width
 */
MGLint MGL_GetDCWidth(MGLvoid);
/**
 * @brief device context의 height를 구한다.
 *
 * @return device context의 height
 */
MGLint MGL_GetDCHeight(MGLvoid);
/**
 * @brief device context의 투명색값을 구한다.
 * 
 * @return 투명색값
 */
MGLcolor MGL_GetDCTrColor( MGLvoid );

// rendering context related functions
/**
 * @brief 사용할 rendering context 배열을 설정한다.
 *
 * @param[in] count rendering context의 총 개수
 * @param[in] pRCs rendering context 배열의 포인터
 */
MGLvoid MGL_SetDCRCs( MGLint count, MGLrc *pRCs );
/**
 * @brief 현재 사용중인 rendering context를 index를 기준으로 설정한다.
 *
 * @param[in] index rendering context의 index
 */
MGLvoid MGL_SetDCCurRCIndex( MGLint index );
/**
 * @brief 현재 사용중인 rendering context를 설정한다.
 *
 * @param[in] pRC rendering context의 포인터
 */
MGLvoid MGL_SetDCCurRC( MGLrc *pRC );
/**
 * @brief 현재 사용중인 rendering context의 포인터를 구한다.
 * 
 * @return rendering context의 포인터
 */
MGLrc * MGL_GetDCCurRC( MGLvoid );
/**
 * @brief 현재 사용중인 rendering context에 layer를 설정한다.
 * 
 * @param[in] count layer의 총 개수
 * @param[in] pLayers layer의 배열의 포인터
 */
MGLvoid MGL_SetRCLayers(MGLint count, MGLlayer * pLayers);
/**
 * @brief rendering context의 영역을 설정한다.
 * 
 * @param[in] x 영역의 좌표값
 * @param[in] y 영역의 좌표값
 * @param[in] width 영역의 x 크기
 * @param[in] height 영역의 y 크기
 */
MGLvoid MGL_SetRCBounds(MGLint x, MGLint y, MGLint width, MGLint height);
/**
 * @brief rendering context의 영역의 x 좌표값을 구한다.
 * 
 * @return x 좌표값
 */
MGLint MGL_GetRCX(MGLvoid);
/**
 * @brief rendering context의 영역의 y 좌표값을 구한다.
 * 
 * @return y 좌표값
 */
MGLint MGL_GetRCY(MGLvoid);
/**
 * @brief rendering context의 영역의 x 크기(width)를 구한다.
 * 
 * @return x 크기 
 */
MGLint MGL_GetRCWidth(MGLvoid);
/**
 * @brief rendering context의 영역의 y 크기(height)을 구한다.
 * 
 * @return y 크기
 */
MGLint MGL_GetRCHeight(MGLvoid);

// layer property related functions
/**
 * @brief 현재 사용중인 layer를 index값으로 설정한다.
 * 
 * @param[in] index layer 배열의 index
 */
MGLvoid MGL_SetCurLayerIndex( MGLint index );
/**
 * @brief 현재 사용중인 layer를 layer의 포인터로 설정한다.
 * 
 * @param[in] layer layer 구조체의 포인터
 */
MGLvoid MGL_SetCurLayer( MGLlayer * layer );
/**
 * @brief 현재 사용중인 layer의 포인터를 구한다.
 * 
 * @return layer의 포인터
 */
MGLlayer * MGL_GetCurLayer( MGLvoid );
/**
 * @brief index값으로 layer의 포인터를 구한다.
 * 
 * @return layer의 포인터
 */
MGLlayer * MGL_GetLayer( MGLint index );
/**
 * @brief layer의 타입을 설정한다.
 * 
 * @param[in] type layer의 타입값
 */
MGLvoid MGL_SetLayerType( MGLint type );
/**
 * @brief layer를 보이게 하거나 숨긴다.
 * 
 * @param[in] bVisible TRUE이면 보이게 한다.
 */
MGLvoid MGL_SetLayerVisible( MGLboolean bVisible );
/**
 * @brief layer의 visible 속성을 구한다.
 * 
 * @return layer의 visible 속성
 */
MGLboolean MGL_GetLayerVisible(MGLvoid);
/**
 * @brief layer의 영역을 설정한다.
 *
 * @attention 이 값은 MGL_SetLayerBitmap 이나 image processing 과정을 거치면 engine에 의해 변경될 수 있습니다.
 * @param[in] x 영역의 x 좌표값
 * @param[in] y 영역의 y 좌표값
 * @param[in] width 영역의 x 크기값 (width)
 * @param[in] height 영역의 y 크기값 (height)
 */
MGLvoid MGL_SetLayerBounds(MGLint x, MGLint y, MGLint width, MGLint height);
/**
 * @brief layer의 x 좌표값을 설정한다.
 * 
 * @param[in] x 영역의 x 좌표값
 */
MGLvoid MGL_SetLayerX(MGLint x);
/**
 * @brief layer의 x 좌표값을 구한다.
 * 
 * @return 영역의 x 좌표값
 */
MGLint MGL_GetLayerX(MGLvoid);
/**
 * @brief layer의 y 좌표값을 설정한다.
 * 
 * @param[in] y 영역의 y 좌표값
 */
MGLvoid MGL_SetLayerY(MGLint y);
/**
 * @brief layer의 y 좌표값을 구한다.
 * 
 * @return 영역의 y 좌표값
 */
MGLint MGL_GetLayerY(MGLvoid);
/**
 * @brief layer의 x 크기값을 설정한다.
 * 
 * @param[in] width 영역의 x 크기값
 */
MGLvoid MGL_SetLayerWidth(MGLint width);
/**
 * @brief layer의 x 크기값을 구한다.
 * 
 * @return 영역의 x 크기값
 */
MGLint MGL_GetLayerWidth(MGLvoid);
/**
 * @brief layer의 y 크기값을 설정한다.
 * 
 * @param[in] height 영역의 y 크기값
 */
MGLvoid MGL_SetLayerHeight(MGLint height);
/**
 * @brief layer의 y 크기값을 구한다.
 * 
 * @return 영역의 y 크기값
 */
MGLint MGL_GetLayerHeight(MGLvoid);
/**
 * @brief 현재의 layer에 image processor들을 설정한다.
 * 
 * @param[in] count image processor들의 총 개수
 * @param[in] pIPs image processor들의 배열의 포인터
 */
MGLvoid MGL_SetLayerIPs(MGLint count, MGLip * pIPs);
/**
 * @brief layer를 이동한다.
 * 
 * @param[in] nXOffset x 좌표의 상대 이동값
 * @param[in] nYOffset y 좌표의 상대 이동값
 */
MGLvoid MGL_MoveLayer(MGLint nXOffset, MGLint nYOffset);

// only image layer related functions
/**
 * @brief layer에 bitmap을 설정한다.
 *
 * 내부적으로 MGL_LoadBitmap 을 호출한다.
 * 
 * @param[in] nBitmapID bitmap ID
 */
MGLvoid MGL_SetLayerBitmap(MGLint nBitmapID);

// only external image layer related functions
/**
 * @brief external image layer의 image layer 정보를 업데이트하게 해주는 함수를 설정한다.
 * 
 * @param[in] fpUpdateImageLayer image layer 정보를 업데이트하게 해주는 함수의 포인터
 * @param[in] count 업데이트 함수 총 호출 회수
 * @param[in] pParams 업데이트 함수 호출시에 필요한 parameter 구조체의 포인터
 */
MGLvoid MGL_SetUpdateImageLayerFuncPtr(MGL_UpdateImageLayerFuncPtr fpUpdateImageLayer, MGLint count, MGLvoid * pParams);

// only custom layer related functions
/**
 * @brief custom layer의 layer를 그리는 함수를 설정한다.
 * 
 * @param[in] fpDrawLayer layer를 그리는 함수의 포인터
 */
MGLvoid MGL_SetLayerDrawFuncPtr(MGL_DrawLayerFuncPtr fpDrawLayer);
/**
 * @brief custom layer의 사용자 버퍼를 설정한다.
 * 
 * @param[in] pBuffer 사용자 버퍼. 버퍼의 타입은 porting시에 platform에 따라 정해진다.
 */
MGLvoid MGL_SetCustomLayerBuffer(MGLvoid * pBuffer);
/**
 * @brief custom layer의 layer를 그리는 함수에 필요한 parameter 구조체를 설정한다.
 * 
 * @param[in] pParams 구조체의 포인터
 */
MGLvoid MGL_SetCustomLayerParams(MGLvoid * pParams);
/**
 * @brief 버퍼를 device context의 영역만큼 투명색으로 지운다. 
 *
 * @param[in] pBuffer 버퍼의 포인터
 */
MGLvoid MGL_ClearBuffer(MGLvoid * pBuffer);
/**
 * @brief 버퍼를 rendering context의 영역만큼 투명색으로 지운다. 
 *
 * @param[in] pBuffer 버퍼의 포인터
 * @param[in] pRC rendering context의 포인터
 */
MGLvoid MGL_ClearBufferRC(MGLvoid *pBuffer, MGLrc * pRC);
/**
 * @brief 버퍼를 layer의 영역만큼 투명색으로 지운다. 
 *
 * @param[in] pBuffer 버퍼의 포인터
 * @param[in] pLayer layer의 포인터
 */
MGLvoid MGL_ClearBufferLayer(MGLvoid *pBuffer, MGLlayer * pLayer);

// rendering function
/**
 * @brief 현재의 rendering context를 그린다.
 */
MGLvoid MGL_Render(MGLvoid);

/**
 * @brief 현재의 rendering context를 지정한 버퍼에 그린다.
 *
 * @param[in] pBuffer 버퍼의 포인터
 */
MGLvoid MGL_RenderToBuffer(MGLvoid * pBuffer);

// timing related function
/** 
 * @brief 초당 Tick수를 구한다.
 *
 * @return Tick수
 */
MGLulong MGL_TicksForSecond( MGLvoid );

/** 
 * @brief 현재의 Tick값을 구한다.
 *
 * @return Tick값
 */
MGLulong MGL_GetTicks( MGLvoid );

/*
 * memory managing function
 */
/** 
 * @brief	dest 메모리 영역을 임의의 값(value)으로 주어진 크기만큼 채운다. 
 *
 * @param dest 채울 메모리 주소
 * @param value 채울 값 
 * @param size 크기
 */
MGLvoid MGL_MemSet( MGLvoid * dest, MGLint value, MGLint size);

/** 
 * @brief	dest 메모리 영역의 값을 src 메모리 영역의 값으로 채운다. 
 *
 * @param dest 채울 메모리 주소
 * @param src 원본 데이터의 메모리 주소
 * @param size 크기
 */
MGLvoid MGL_MemCopy( MGLvoid * dest, MGLvoid * src, MGLint size);

/*
 * 2D Primitive API
 */
/** 
 * @brief	Layer의 배경색을 바꾼다. (한 프레임을 그릴때에만 유효함.)
 *
 * @param color	바꾸려는 색상값.
 * @return	이전 색상값.
 */
MGLcolor MGL_SetBgColor( MGLcolor color );

/** 
 * @brief	현재 Layer의 전경색을 바꾼다. 
 *
 * @param color	바꾸려는 색상값.
 * @return	이전 색상값.
 */
MGLcolor MGL_SetFgColor( MGLcolor color );

/** 
 * @brief	현재 Layer의 배경색을 얻어온다.  
 *
 * @param	없음.
 * @return	현재 색상값.
 */
MGLcolor MGL_GetBgColor( MGLvoid );

/** 
 * @brief	현재 Layer의 전경색을 얻어온다.  
 * 
 * @param	없음.
 * @return	현재 색상값.
 */
MGLcolor MGL_GetFgColor( MGLvoid );

/** 
 * @brief	주어진 좌표의 픽셀 색상 값을 바꾼다. 
 *
 * @param x		X 좌표 
 * @param y		Y 좌표 
 * @param color	변경할 색
 * @return	주어진 좌표의 이전 색상 
 */
MGLcolor MGL_SetPixel( MGLint x, MGLint y, MGLcolor color );

/** 
 * @brief	주어진 좌표의 픽셀 색상 값을 얻어온다. 
 *
 * @param x		X 좌표 
 * @param y		Y 좌표 
 * @return	주어진 좌표의 색상 값.
 */
MGLcolor MGL_GetPixel( MGLint x, MGLint y );

/** 
 * @brief	두 점을 잇는 선을 그린다. 
 *
 * @param x1	선의 시작 X 좌표
 * @param x2	선의 시작 Y 좌표
 * @param y1	선의 끝 X 좌표
 * @param y2	선의 끝 Y 좌표
 */
MGLvoid MGL_DrawLine( MGLint x1, MGLint y1, MGLint x2, MGLint y2 );

/** 
 * @brief	사각형을 그린다. 
 *
 * @param left		영역 좌표 : left
 * @param top		영역 좌표 : top
 * @param width		X축 너비
 * @param height	Y축 높이 
 */
MGLvoid	MGL_DrawRect( MGLint left, MGLint top, MGLint width, MGLint height );

/** 
 * @brief	원하는 색으로 채워져 있는 사각형을 그린다.
 *
 * @param left		영역 좌표 : left
 * @param top		영역 좌표 : top
 * @param width		X축 너비
 * @param height	Y축 높이 
 * @param color		채울 색상 값. 
 */
MGLvoid	MGL_FillRect( MGLint left, MGLint top, MGLint width, MGLint height, MGLcolor color );

/** 
 * @brief	원을 그린다. 
 *
 * @param x	중심점 X 좌표 
 * @param y 중심점 Y 좌표 
 * @param r 원의 반지름
 */
MGLvoid	MGL_DrawCircle( MGLint x, MGLint y, MGLint r );

/** 
 * @brief	타원을 그린다. 
 *
 * @param x		중심점 X 좌표 
 * @param y		중심점 Y 좌표 
 * @param xr	X축 반지름
 * @param yr	Y축 반지름
 */
MGLvoid	MGL_DrawEllipse( MGLint x, MGLint y, MGLint xr, MGLint yr );


/*
 * Bitmap API
 */

/**
 * @brief	현재 선택된 layer에 bitmap을 그린다. 
 *
 * @param pBitmap	bitmap data
 * @param x			bitmap을 그릴 X 좌표
 * @param y			bitmap을 그릴 Y 좌표
  */
MGLvoid MGL_DrawBitmap( MGLbitmap * pBitmap, MGLint x, MGLint y );

/** 
 * @brief	소스 영역의 데이터를 그릴려고 하는 곳에 그려준다. 
 *
 * @param pDst		그려져야하는 영역에 대한 포인터
 * @param dst_x		그려져야하는 영역 : X 좌표
 * @param dst_y		그려져야하는 영역 : Y 좌표
 * @param width		그려져야하는 영역 : width
 * @param height	그려져야하는 영역 : height
 * @param pSrc		소스 영역에 대한 포인터
 * @param src_x		소스 영역 : X 좌표
 * @param src_y		소스 영역 : Y 좌표
 */
MGLvoid MGL_BitBlt( MGLuint * pDst, MGLint dst_x, MGLint dst_y, MGLint width, MGLint height, 
				    MGLuint * pSrc, MGLint src_x, MGLint src_y );

/** 
 * @brief	Bitmap 데이터를 가져온다. 
 *
 * @param id		데이터를 얻으려는 Bitmap ID
 * @param pBitmap	Bitmap 데이터 저장 공간
 */
MGLvoid MGL_LoadBitmap( MGLint id, MGLbitmap *  pBitmap );

/**
 * @brief image processor의 image processing을 구현한 함수의 포인터
 */
#define MGL_IP_DO_FUNC						0x0001
/**
 * @brief image processor의 parameter를 구하는 함수의 포인터
 */
#define MGL_IP_GET_PARAM_FUNC				0x0011
/**
 * @brief image processor의 parameter를 설정하는 함수의 포인터
 */
#define MGL_IP_SET_PARAM_FUNC				0x0012
/**
 * @brief image processor의 boolean 형태의 속성값을 구하는 함수의 포인터
 */
#define MGL_IP_GET_BOOLEAN_PROP_FUNC		0x0021
/**
 * @brief image processor의 int 형태의 속성값을 구하는 함수의 포인터
 */
#define MGL_IP_GET_INT_PROP_FUNC			0x0022

/**
 * @brief void 타입
 */
#define MGL_TYPE_VOID						0x0000
/**
 * @brief boolean 타입
 */
#define MGL_TYPE_BOOLEAN					0x0001
/**
 * @brief char 타입
 */
#define MGL_TYPE_CHAR						0x0002
/**
 * @brief short 타입
 */
#define MGL_TYPE_SHORT						0x0003
/**
 * @brief int 타입
 */
#define MGL_TYPE_INT						0x0004
/**
 * @brief long 타입
 */
#define MGL_TYPE_LONG						0x0005
/**
 * @brief float 타입
 */
#define MGL_TYPE_FLOAT						0x0006
/**
 * @brief double 타입
 */
#define MGL_TYPE_DOUBLE						0x0007

/**
 * @brief unsigned char 타입
 */
#define MGL_TYPE_UCHAR						0x0102
/**
 * @brief unsigned short 타입
 */
#define MGL_TYPE_USHORT						0x0103
/**
 * @brief unsigned int 타입
 */
#define MGL_TYPE_UINT						0x0104
/**
 * @brief unsigned long 타입
 */
#define MGL_TYPE_ULONG						0x0105

/**
 * @brief void * 타입
 */
#define MGL_TYPE_VOID_PTR					0x1000
/**
 * @brief char * 타입
 */
#define MGL_TYPE_CHAR_PTR					0x1002
/**
 * @brief unsigned char * 타입
 */
#define MGL_TYPE_UCHAR_PTR					0x1102

/**
 * @brief error code값을 저장하는 타입
 */
#define MGL_TYPE_ERROR						0x2001
/**
 * @brief fixed point값을 저장하는 타입
 */
#define MGL_TYPE_FIXED						0x2002
/**
 * @brief color값을 저장하는 타입
 */
#define MGL_TYPE_COLOR						0x2003

/**
 * @brief 알수 없는 타입
 */
#define MGL_TYPE_UNKNOWN					0xFFFF

/**
 * @brief 반드시 dest buffer와 mix해야 하는 image process
 *
 * 예) alpha blending, drop shadow, blur
 */
#define MGL_IP_PROP_DEST_MIX		1

/**
 * @brief FPS 검사여부를 정한다.
 *
 * @param[in] bEnabled TRUE이면 FPS를 검사하고 메시지를 출력한다.
 */
MGLvoid MGL_SetFPSEnalbed(MGLboolean bEnabled);

/**
 * @brief FPS를 초기화한다.
 */
MGLvoid MGL_ResetFPS(MGLvoid);

/**
 * @brief FPS값을 검사한다. 
 *
 * 엔진내부에서 MGL_Render, MGL_RenderToBuffer 가 호출될 때 이 함수가 호출되며 frame count가 증가하고, 정해진 시간이 지나면 fps값을 디버그 메시지로 출력한다.
 */
MGLvoid MGL_CheckFPS(MGLvoid);

/** 
 * @brief Assert 상태를 검사한다. 내부용으로 사용되므로 MGL_Assert 를 대신 사용한다.
 *
 * @attention 디버그용 버전에서만 작동한다.
 * @param[in] nCondition 0 또는 그외의 값으로 현재 조건식의 결과값을 넣어준다. 
 * @param[in] pszFile filename
 * @param[in] pszFunc funtion name
 * @param[in] nLine line number
 */
MGLvoid MGL_AssertInternal(MGLint nCondition, MGLbyte * pszFile, MGLbyte * pszFunc, MGLint nLine );

/** 
 * @brief	Assert 상태를 검사한다.  
 *
 * @attention 디버그용 버전에서만 작동한다.
 * @param[in] nCondition 0 또는 그외의 값으로 현재 조건식의 결과값을 넣어준다. 
 */
#ifdef MGL_CONFIG_USE_DEBUG_INFO
	#define MGL_Assert(nCondition) MGL_AssertInternal(nCondition, __MGL_FILE__, __MGL_FUNC__, __MGL_LINE__)
#else
	#define MGL_Assert(nCondition)
#endif

/**
 * @brief 디버그용 메시지를 printf 양식에 맞게 출력한다.
 *
 * @attention 디버그용 버전에서만 작동한다.
 * @param[in] format 출력할 양식. printf과 동일하다.
 * @param[in] ... 인자값들
 */
MGLvoid MGL_DebugPrintF(MGLbyte * format, ...);

/**
 * @brief 디버그용 메시지를 출력한다.
 *
 * @attention 디버그용 버전에서만 작동한다.
 * @param[in] msg 출력할 메시지
 */
MGLvoid MGL_DebugMsg(MGLbyte * msg);

/**
 * @brief 문자열(MGLbyte *)형태의 Global 속성값을 구한다. @ref globpropids 를 참고한다.
 * 
 * @param[in] nPropID 속성 ID
 * @return MGLbyte * 형태의 속성값
 */
MGLbyte * MGL_GetStringProperty(MGLint nPropID);

/**
 * @brief 숫자(MGLint)형태의 Global 속성값을 구한다. @ref globpropids 를 참고한다.
 * 
 * @param[in] nPropID 속성 ID
 * @return MGLint 형태의 속성값
 */
MGLint MGL_GetIntProperty(MGLint nPropID);

/**
 * @brief 불린(MGLboolean)형태의 Global 속성값을 구한다. @ref globpropids 를 참고한다.
 * 
 * @param[in] nPropID 속성 ID
 * @return MGLboolean 형태의 속성값
 */
MGLboolean MGL_GetBooleanProperty(MGLint nPropID);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __mgl_h_
