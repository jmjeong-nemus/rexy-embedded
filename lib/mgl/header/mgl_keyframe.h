/**
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 *
 * @file mgl_keyframe.h
 *
 * @brief library for keyframe animation
 *
 * @author tombraid
 *
 * @date 2005-11-22
 *
 * Notes :
 *
 * Open Issues : 
 *
 */

#ifndef MGL_KEYFRAME_H
#define MGL_KEYFRAME_H

#ifdef __cplusplus
extern "C" {
#endif

/* define {{{ */ 
/** 
 * @brief START INDEX 는 2이다. 바운더리 체크를 회피하기 위해서 결정된 값이다.
 */
#define MGL_KEYFRAME_START_INDEX		2

/** 
 * @defgroup api_keyframe_option_interpolation Keyframe Interpolation Option
 *
 * Keyframe 보간 방식에 대한 옵션
 * 
 * @addtogroup api_keyframe_option_interpolation 
 * @{
 * */
#define MGL_KEYFRAME_IPF_FIXED_LINEAR	0		/**< 선형 보간 함수 fixed 형 */
#define MGL_KEYFRAME_IPF_INT_LINEAR		1		/**< 선형 보간 함수 int 형 */
#define MGL_KEYFRAME_IPF_POINT_LINEAR	2		/**< 선형 보간 함수 point_t 형*/
#define MGL_KEYFRAME_IPF_POINT_SPLINE	3		/**< spline 보간 함수 point_t 형 */
#define MGL_KEYFRAME_IPF_POINT_HERMITE	4		/**< hermite 보간 함수 point_t 형 */
#define MGL_KEYFRAME_IPF_POINT_ELLIPSE	5		/**< ellipse 보간 함수 point_t 형 */
#define MGL_KEYFRAME_IPF_USER			6		/**< 사용자 보간 함수 */
#define MGL_KEYFRAME_IPF_LAST			7		/**< 보간함수 마지막 인덱스 - 사용하지 못함 */
/** 
 * @}
 * */
/** 
 * @defgroup api_keyframe_option_ease Keyframe Ease Option
 *
 * Keyframe 속도 제어 방식에 대한 옵션
 * 
 * @addtogroup api_keyframe_option_ease
 * @{
 * */
#define MGL_KEYFRAME_ESF_LINEAR			0 		/**< 선형 ease 함수 */
#define MGL_KEYFRAME_ESF_SIN 			1		/**< sin ease 함수 */
#define MGL_KEYFRAME_ESF_USER 			2		/**< 사용자 ease 함수 */
#define MGL_KEYFRAME_ESF_LAST			3		/**< ease 함수 마지막 인덱스 - 사용하지 못함 */
/** 
 * @}
 * */

/* }}} */

/* data structure {{{ */ 
/** 
 * @defgroup api_keyframe_data_structure Keyframe Data Structure
 * 
 * Keyframe 자료 구조
 *
 * @addtogroup api_keyframe_data_structure
 * @{
 * */

/** 
 * @brief 보간 함수의 종류
 */
typedef MGLint MGLipftype;

/** 
 * @brief ease 함수의 종류
 */
typedef MGLint MGLesftype;

/** 
 * @brief 매개변수와 호길이를 가지고 있는 테이블
 *
 *  -# 테이블은 연속적이지 않다. i 번째 Keyframe에 대한 정보는 i * INTERFRAME_RESOLUTION에서 부터 시작한다.
 *  -# 각 프레임에 대한 엔트리 개수는 cnt[i] 에 있다.
 */
typedef struct MGLdistent {
	MGLfixed u;						/**< 매개변수 값 */
	MGLfixed distance; 				/**< 출발점부터의 거리 값 */
} MGLdistent;


struct MGLinterpolator;

/** 
 * @brief 보간을 수행하는 함수 타입이다.
 * 
 * @param pInterpolator 보간을 처리하기 위한 interpolator (보간자??)의 포인터
 * @param u 매개변수
 * @param parg 위치 보간의 경우 결과 위치가 저장될 point의 포인터, 그외의 경우 NULL
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLipfunc) (struct MGLinterpolator *pInterpolator, MGLfixed u, MGLvoid *parg);

/** 
 * @brief 매개변수 u를 생성해 내기 위한 함수 타입이다. seq를 이용하여 매개변수 u를 생성해 낸다.
 * 
 * @param pInterpolator 보간을 처리하기 위한 보간자의 포인터 
 * @param seq 보간할 프레임 넘버
 * 
 * @return 생성된 매개변수 u
 */
typedef MGLfixed (*MGLeasefunc) (struct MGLinterpolator *pInterpolator, MGLint seq);

/** 
 * @brief 거리를 계산하기 위한 함수 타입이다. 보간대상에 따라 달라질 수 있기 때문에 이런 식으로 한다.
 * 
 * @param arg1 첫번째 인자
 * @param arg2 두번째 인자 
 * 
 * @return 거리 
 */
typedef MGLfixed (*MGLgetdistfunc) (MGLvoid *arg1, MGLvoid *arg2);

/** 
 * @brief 보간자에 대한 자로구조
 */
typedef struct MGLinterpolator {
	MGLint nParamID;				/**< nParamID 보간될 종류 (위치, 각, 배율) */

	MGLipftype ipftype;				/**< 보간 함수 타입 */
	MGLipfunc fpInterpolateFunc;	/**< 보간 함수 */

	MGLesftype esftype;				/**< ease 함수 타입 */
	MGLeasefunc fpEaseFunc;			/**< ease 함수 */

	MGLgetdistfunc fpGetDistFunc;	/**< 거리 계산 함수 */

	MGLint nEndSeq;					/**< 전체 프레임 수 */

	MGLint nKeyLastIndex;			/**< 키데이터의 마지막 값이 들어있는 인덱스 */
	MGLint nKeyDataCount;			/**< 키데이터의 크기 */
	MGLanidata *pKeyData;			/**< 각 키데이터의 정보 */

	MGLvoid *pIP;					/**< IP에 대한 포인터 */

	MGLint nDistCurIdx;				/**< Distance Table 검색시 사용되는 index */
	MGLint nDistEntCount;			/**< Distance Table 크기 */
	MGLdistent *pDistEnt;			/**< 보간을 위한 Distance Table*/
	MGLfixed dTotalDistance;		/**< 전체 거리 */
} MGLinterpolator;

/** 
 * @brief keyframe main data type
 */
typedef struct MGLkeyframe {
	MGLint nSeq;					/**< Current Sequence */
	MGLint nResultFrameCount;		/**< 전체 프레임 수 */
	MGLinterpolator *pInterpolators; 	/**< 보간자들에 대한 포인터 */
	MGLint nInterpolatorCount;			/**< keyframe 이 관리하는 보간자들의 수 */
	MGLanidata *pResults;				/**< 결과를 저장하는 배열. 개수는 nResultFrameCount * nInterpolatorCount */
} MGLkeyframe;

/** 
 * @}
 * }}} */

/* API for User */
/** 
 * @defgroup api_keyframe_api_for_user Keyframe API for User
 * 
 * Keyframe 사용자 API
 *
 * @addtogroup api_keyframe_api_for_user
 * @{
 * */
/* keyframe function {{{ */

/** 
 * @brief Keyframe 을 초기화 하는 함수이다. 이 함수를 통해 사용자는 사용할 메모리등을 할당해 주어야 한다.
 * 
 * @param pKeyFrame Keyframe 메인 데이터
 * @param nResultFrameCount 전체 프레임 수 
 * @param pInterpolators 보간자들의 포인터 - 사용할 효과 만큼의 보간자를 할당하여야 한다.
 * @param nInterpolatorCount 보간자들의 개수
 * 
 * @return MGLerror
 */
MGLerror
MGL_SetKeyFrame (MGLkeyframe *pKeyFrame, MGLint nResultFrameCount, 
		MGLinterpolator *pInterpolators, MGLint nInterpolatorCount);

/** 
 * @brief Keyframe 이 사용될 수 있도록 미리 계산되어야 할 부분들을 처리해 준다.
 *
 * @param pKeyFrame Keyframe에 대한 포인터 
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameStandby (MGLkeyframe *pKeyFrame);

/** 
 * @brief Keyframe 계산을 미리 수행한다.
 * 
 * Keyframe 정보를 실시간으로 얻어오지 않는 경우에 사용한다.
 * 메모리 사용량이나 성능등의 이유로 사용여부를 결정할 수 있다.
 * 반복적인 에니메이션을 필요로 하는 경우나 키 데이터, 거리 테이블 등에 비해 낮은
 * 결과 프레임 개수를 사용하는 경우에는 이 함수를 사용하는 것이 좋다.
 * 이 함수는 호출되는 시점이 매우 중요하다.
 * 제대로 사용되기 위해서는 MGL_KeyFrameSetInSubsystem 전에 호출되어야 한다.
 *
 * @param pKeyFrame Keyframe
 * @param pResults 결과가 저장될 공간의 배열
 * @param nResultCount 결과의 크기 (프레임수 * 인터폴레이터수)
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFramePreCalculate (MGLkeyframe *pKeyFrame, 
		MGLanidata *pResults, MGLint nResultCount);

/** 
 * @brief PreCalcaulate를 사용하는 경우 필요없는 공간을 해제한다.
 * 
 * PreCalculate가 처리된 시점부터 pInterpolators 관련 데이터는 메모리 해제될 수 있다.
 * 실제로 free 함수를 콜한다는 의미가 아니라 명시적으로 사용하지 않음을 표현한다는 의미이다.
 * Realtime 으로 사용하는 경우에 이 함수를 호출하면 에러가 발생한다.
 *
 * @param pKeyFrame 키 프레임
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameFreeInterpolators (MGLkeyframe *pKeyFrame);

/** 
 * @brief Keyframe과 Animator의 Subsystem을 연결한다.
 * 
 * @param pKeyFrame 연결할 Keyframe
 * @param pSubsystem 연결될 Subsystem
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetInSubsystem (MGLkeyframe *pKeyFrame, MGLsubsystem *pSubsystem);

/* }}} */

/* interpolator function {{{ */
/** 
 * @brief 보간자로 부터 보간의 종류를 구함
 */
#define MGL_KEYFRAME_GETPARAMID(pInterpolator)	((pInterpolator)->nParamID)

/** 
 * @brief 보간자로 부터 보간 함수의 종류를 구함
 */
#define MGL_KEYFRAME_GETIPFTYPE(pInterpolator)	((pInterpolator)->ipftype)

/** 
 * @brief 보간자로 부터 ease 함수의 종류를 구함
 */
#define MGL_KEYFRAME_GETESFTYPE(pInterpolator)	((pInterpolator)->esftype)

/** 
 * @brief 마지막 프레임의 seq를 구함. 이는 보간자가 만들어내는 에니메이션의 총 프레임이다.
 */
#define MGL_KEYFRAME_GETLASTFRAME(pInterpolator)		((pInterpolator)->nEndSeq)

/** 
 * @brief interpolator 를 세팅한다.
 *
 * 이 보간자가 사용할 키 데이터들이 저장될 공간과 거리 테이블이 저장될 공간이 세팅되어야한다.
 * 키 데이터가 저장될 공간은 바운더리 체크를 피하기 위해 (키 데이터 개수 + 2)의 크기여야 한다.
 * 거리 테이블이 저장될 공간은 (키 데이터의 개수 * 해상도 + 1)의 크기여야 한다.
 * 해상도가 높을수록 정확한 속도가 보장되나 너무 높게 잡을 필요는 없다.
 * 
 * @param pInterpolator 세팅될 interpolator 의 포인터
 * @param pIP 사용할 IP에 대한 포인터
 * @param nParamID 보간대상 타입
 * @param pKeyData keydata 의 배열 - 전체 키 데이터 + 2 개의 공간이 필요
 * @param nKeyDataCount keydata 의 개수
 * @param pDistEnt 거리 테이블 - (전체 키 데이터 - 1) * 해상도 + 1 의 공간이 필요
 * @param nDistEndCount 거리테이블의 entry 수
 * @param ipftype 선택할 보간함수의 타입 
 * @param fpUserIPFunc 사용자 지정 보간 함수 (평소의 경우 NULL)
 * @param fpUserGetDistFunc 사용자 지정 거리 계산 함수 (평소의 경우 NULL)
 * @param esftype 선택할 매개변수 생성함수의 타입
 * @param fpUserEaseFunc 사용자 지정 매개변수 생성함수 (평소의 경우 NULL)
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetInterpolator (MGLinterpolator *pInterpolator, 
		MGLvoid *pIP, MGLint nParamID,
		MGLanidata *pKeyData, MGLint nKeyDataCount,
		MGLdistent *pDistEnt, MGLint nDistEndCount,
		MGLipftype ipftype, MGLipfunc fpUserIPFunc, MGLgetdistfunc fpUserGetDistFunc,
		MGLesftype esftype, MGLeasefunc fpUserEaseFunc);

/** 
 * @brief 보간자의 세팅을 지운다.
 * 
 * @param pInterpolator 보간자
 * 
 * @return MGLerror
 */
MGLerror
MGL_ClearInterpolator (MGLinterpolator *pInterpolator);

/* }}} */

/* keydata function {{{ */ 

/** 
 * @brief 보간자에 키데이터를 추가한다.
 *
 * 추가시 특이한게 있는데, 양쪽 바운더리에는 바운더리 값이 하나씩 더 들어있다. 이는 바운더리 체크를 피하기 위한 방안이다.
 * 
 * @param pInterpolator 보간자 
 * @param pKeyData Keyframe으로 추가할 프레임에 대한 정보
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetKeyData (MGLinterpolator *pInterpolator, MGLanidata *pKeyData);

/** 
 * @brief 경로에 대한 키데이터를 쉽게 추가할 수 있도록 돕는 함수로 내부적으로 MGL_KeyFrameSetKeyData을 호출한다.
 * 
 * @param pInterpolator 보간자
 * @param x x's position
 * @param y y's position
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetPointKeyData (MGLinterpolator *pInterpolator, MGLint x, MGLint y);

/** 
 * @brief int 값에 대한 키데이터을 쉽게 추가할 수 있도록 돕는 함수로 내부적으로 MGL_KeyFrameSetKeyData 을 호출한다.
 * 
 * @param pInterpolator 보간자
 * @param integer int value
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetIntKeyData (MGLinterpolator *pInterpolator, MGLint integer);

/** 
 * @brief fixedpoint 에 대한 키데이터을 쉽게 추가할 수 있도록 돕는 함수로 내부적으로 MGL_KeyFrameSetKeyData 을 호출한다.
 * 
 * @param pInterpolator 보간자
 * @param fixed fixedpoint value
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetFixedKeyData (MGLinterpolator *pInterpolator, MGLfixed fixed);

/** 
 * @brief 타원형의 움직임을 보간함 
 * 
 * @param pInterpolator 보간자 
 * @param nCenterX 타원의 중심 X 좌표 
 * @param nCenterY 타원의 중심 Y 좌표 
 * @param nRadiusX 타원의 X 방향 반지름 
 * @param nRadiusY 타원의 Y 방향 반지름 
 * @param nFromDeg 시작각 
 * @param nToDeg 끝각
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetEllipseKeyData (MGLinterpolator *pInterpolator, MGLint nCenterX, MGLint nCenterY,
							MGLint nRadiusX, MGLint nRadiusY, MGLint nFromDeg, MGLint nToDeg);

/** 
 * @brief 키데이터의 개수를 리턴함
 */
#define MGL_KEYFRAME_GETKEYDATACOUNT(pInterpolator)		((pInterpolator)->nLastIndex - 1)

/* }}} */
/** 
 * @}
 * */

/* Animation support interface implementation {{{ */
/** 
 * @defgroup api_keyframe_asi Keyframe Animation Support Interface 
 *
 * Keyframe의 Animation Support Interface 
 * 
 * @addtogroup api_keyframe_asi
 * @{
 * */
/** 
 * @brief Keyframe에 시간의 흐름을 전달한다.
 * 
 * @param pSubsystem  Keyframe
 * @param nTiming 타이밍
 * 
 * @return MGLerror
 */
MGLerror 
MGL_KeyFrameUpdateScene (MGLvoid *pSubsystem, MGLint nTiming);

/** 
 * @brief 실시간으로 보간된 결과값을 구한다.
 * 
 * @param index 아이템 인덱스 - 여기서는 항상 0
 * @param pLayer 보간될 결과가 저장될 자료구조
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror 
MGL_KeyFrameGetImageLayerInRealTime (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief 이미 보간된 결과값으로 부터 결과를 가져온다.
 * 
 * @param index 아이템 인덱스 - 여기서는 항상 0
 * @param pLayer 보간될 결과가 저장될 자료구조
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror 
MGL_KeyFrameGetImageLayerFromResult (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief 실시간으로 주어진 프레임 넘버에 의해 보간된 결과값을 구한다.
 * 
 * @param index 아이템 인덱스 - 여기서는 항상 0
 * @param pLayer 보간될 결과가 저장될 자료구조
 * @param nSeq 프레임 넘버 
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameGetLayerWithFrameFromResult (MGLint index, MGLlayer *pLayer, MGLint nSeq, MGLvoid *pSubsystem);

/** 
 * @brief 이미 보간된 결과값으로부터 주어진 프레임 넘버에 해당하는 결과값을 구한다.
 * 
 * @param index 아이템 인덱스 - 여기서는 항상 0
 * @param pLayer 보간될 결과가 저장될 자료구조
 * @param nSeq 프레임 넘버 
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameGetLayerWithFrameInRealTime (MGLint index, MGLlayer *pLayer, MGLint nSeq, MGLvoid *pSubsystem);

/** 
 * @brief 현재 진행중인 키 프레임 애니메이션을 리와인드(?) 한다. ^^
 * 
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameClear (MGLvoid *pSubsystem);

/** 
 * @}
 * }}} */

#ifdef __cplusplus
}
#endif

#endif /* MGL_KEYFRAME_H */

/*
" vim: fileencoding=euc-kr :
 */
