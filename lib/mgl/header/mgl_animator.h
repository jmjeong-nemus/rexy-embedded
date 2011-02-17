/**
 * $Id$
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 *
 * @file mgl_animator.h
 *
 * @author tombraid <tombraid@mococo.com>
 *
 * @brief
 *  MGL에서 Animation을 지원하기 위한 시스템 
 *
 * @date 2005-11-23
 *
 * Notes :
 *
 * Open Issues :
 *
 */

#ifndef MGL_ANIMATOR_H
#define MGL_ANIMATOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Timing Option {{{ */
/**
 * @defgroup api_animator_option_timing Timing Option
 *
 * Animator 설정시 사용되는 타이밍 옵션에 대한 정의
 *
 * @see MGL_AnimatorSetTiming
 *
 * @addtogroup api_animator_option_timing
 * @{
 */

#define MGL_TIMING_TYPE_TIME	0	/**< 속도를 유지할 수 있도록 타이밍을 계산 */
#define MGL_TIMING_TYPE_FRAME	1	/**< 프레임을 유지하도록 타이밍 계산 */

/**
 * @}
 * }}} */

/* Param Define {{{ */
/** 
 * @defgroup api_animator_option_params Animator Params
 *
 * Animator 를 위한 Params 
 * IP 에서 다루는 Params 과 유사
 * 
 * @addtogroup api_animator_option_params
 * @{
 * */
// layer params
#define MGL_ANI_PARAM_POSITION              1	/**< layer 의 위치에 대한 Param Type */
#define MGL_ANI_PARAM_POSITION_X            2	/**< layer 의 X 좌표에 대한 Param Type */
#define MGL_ANI_PARAM_POSITION_Y            3	/**< layer 의 Y 좌표에 대한 Param Type */
#define MGL_ANI_PARAM_IMAGE					4	/**< layer 의 이미지에 대한 Param Type */

// interval params
#define MGL_ANI_PARAM_INTERVAL              5	/**< 아이템의 간격에 대한 Param Type */

/** 
 * @}
 * }}} */

/* Animation Subsystem Interface {{{ */
/** 
 * @defgroup api_animator_subsystem_interface Animator Subsystem Interface
 *
 * Animator 와 Subsystem 간의 interface
 * 
 * @see page_subsystem
 * @addtogroup api_animator_subsystem_interface
 * @{
 * */
/** 
 * @brief Animation Subsystem Interface 로 서브 시스템에 시간의 흐름을 전달한다.
 * 
 * @param pSubsystem 서브 시스템 
 * @param timing 시간의 흐름으로 상대적인 값이다. 1초가 단위시간이고 updatescene 이 호출된뒤 5초가 흘렸다면 timing 값은 5이다.
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLupdatescene) (MGLvoid *pSubsystem, MGLint timing);

/** 
 * @brief Animation Subsystem Interface 로 Layer의 정보를 얻어온다.
 *
 * 이 타입은 MGLlayer 의 MGL_UpdateImageLayerFuncPtr 과 같은 타입이다. 
 * 이 함수의 호출은 순차호출을 보장받는다.
 * 
 * @param index 얻어올 아이템의 인덱스
 * @param pLayer 얻어올 아이템의 정보를 기록할 메모리
 * @param pSubsystem 서브 시스템
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLgetimagelayer) (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief Animation Subsystem Interface 로 서브 시스템의 정보를 최초의 상태로 돌린다. 
 *
 * 애니메이션을 중간에 멈추고 처음으로 돌리는 효과가 있다.
 * 
 * @param pSubsystem  서브 시스템
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLclear) (MGLvoid *pSubsystem);

/** 
 * @brief Animation Subsystem Interface 로 인자에 주어진 프레임 넘버를 이용하여 layer의 정보를 얻어온다.
 *
 * ItemManger와 같이 서브시스템이 하위 서브시스템을 이용하는 경우 사용할 수 있다.
 * 모든 서브 시스템이 다 구현해야 하는 것은 아니다. 
 * 이 함수가 구현되지 않는 경우 ItemManager 등과 같이 이 interface 를 사용하는 서브 시스템과는 연결되지 않는다.
 * 
 * @param index 얻어올 아이템의 인덱스
 * @param pLayer 얻어올 아이템의 정보를 기록할 메모리
 * @param nSeq 프레임 넘버 
 * @param pSubsystem 서브 시스템
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLgetlayerwithframe) (MGLint index, MGLlayer *pLayer, MGLint nSeq, MGLvoid *pSubsystem);

/** 
 * @}
 * }}} */

/* Animator Data Structure  {{{ */
/** 
 * @defgroup api_animator_data_structure Animator Data Structure
 *
 * Animator 를 위한 자료구조
 * 
 * @addtogroup api_animator_data_structure
 * @{
 * */
/** 
 * @brief 위치 정보를 저장하는 타입
 */
typedef struct MGLpoint {
	MGLint x;	/**< x 좌표 값 */
	MGLint y;	/**< y 좌표 값 */
} MGLpoint;

/** 
 * @brief 프레임정보 타입
 */
typedef union MGLanidata {
	MGLfixed fixedpoint;	/**< 프레임의 배율값 및 각도값 등등 */
	MGLint integer;			/**< 프레임의 알파값 */
	MGLpoint point;			/**< 프레임의 위치값 */
} MGLanidata;

/** 
 * @brief 애니메이터가 다루는 서브 시스템에 대한 정보를 담는 타입
 */
typedef struct MGLsubsystem {
	MGLint bStart;			/**< 서브 시스템의 동작 상태를 나타냄 */
	MGLvoid *pSubsystem; 	/**< 서브 시스템 */

	/* 이하는 Animation Subsystem Interface에 관련한 부분이다. */ 
	MGLupdatescene fpUpdateScene; 		/**< ASI MGLupdatescene에 대한 Function Pointer */
	MGLgetimagelayer fpGetImageLayer;	/**< ASI MGLgetimagelayer 에 대한 Function Pointer */
	MGLclear fpClear;					/**< ASI MGLclear 에 대한 Function Pointer */
	MGLgetlayerwithframe fpGetLayerWithFrame;	/**< ASI MGLgetlayerwithframe 에 대한 Function Pointer */
} MGLsubsystem;

/** 
 * @brief 애니메이터 타입
 */
typedef struct MGLanimator {
	/* 이하는 타이밍 계산을 위한 부분이다. */
	MGLint nLastTiming;	/**< 여지껏 총 계산한 Timing 의 합으로 Tick 과는 다르다. */
	MGLint nTimingType; /**< 타이밍 계산을 하는 타입 */
	MGLint nInterval;	/**< 애니메이션 인터벌 이값을 이용하여 Framerate 를 조정할 수 있다. */ 
	MGLuint nStartTick;	/**< 애니메이션이 시작한 틱 */

	/* 이하는 Subsystem과의 연동을 위한 부분이다. */ 
	MGLint nSubsystemCount;	/**< 애니메이터와 연동될 서브 시스템의 개수 */
	MGLsubsystem *pSubsystems; /**< 애니메이터와 연동될 서브 시스템의 배열 */
	MGLuint nEndMask;		/**< 서브시스템별 애니메이션 구동 마스크, n번째 애니메이션이 끝난다면 n번째 비트가 0 */

	/* 이하는 애니메이션 실행에 관련한 부분이다. */ 
	MGLint nRepeatCount; 	/**< 애니메이션 반복 회수 */
	MGLint nCurCount;		/**< 현재 반복 수 */

	/* 이하는 Get Image Layer */
	MGLint nUpdateLayerSubsystemCount;	/**< GetImageLayer 를 구현하는중 index 를 적절히 넘기기 위해 사용하는 변수 */
	MGLint nUpdateLayerSubsystemIdx;	/**< GetImageLayer 를 구현하는중 필요한 현재 정보를 가져와야 할 서브 시스템의 인덱스 */

	/* 이하는 image 관련 */
	MGLint nImageID;	/**< 디폴트 이미지 ID */
} MGLanimator;
/** 
 * @}
 * }}} */

/* API for User {{{ */
/** 
 * @defgroup api_animator_for_user Animator API for User
 *
 * Animator 를 사용하기 위한 사용자 API
 *
 * @see api_animator_option_timing
 *
 * @addtogroup api_animator_for_user
 * @{
 */

/** 
 * @brief 애니메이터를 초기화 하는 함수
 * 
 * @param pAnimator 애니메이터
 * @param pSubsystem 애니메이터와 함께 연동해서 사용할 서브 시스템의 정보를 담을 배열
 * @param nSubsystemCount 애니메이터와 함께 연동해서 사용할 서브 시스템의 개수
 * 
 * @return MGLerror
 */
MGLerror
MGL_SetAnimator (MGLanimator *pAnimator, MGLsubsystem *pSubsystem, MGLint nSubsystemCount);

/** 
 * @brief 애니메이터가 타이밍을 계산하기 위해 필요한 변수를 세팅하는 함수
 * 
 * @param pAnimator  애니메이터
 * @param nTimingType 타이밍 계산 방법 @ref api_animator_option_timing
 * @param nInterval 애니메이션에서 다음 프레임 까지의 인터벌 - 이 값을 조절하여 FrameRate 를 조절
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorSetTiming (MGLanimator *pAnimator, MGLint nTimingType, MGLint nInterval);

/** 
 * @brief 애니메이션 반복회수를 설정하는 함수. 0은 무한 반복
 * 
 * @param pAnimator  애니메이터
 * @param nRepeatCount 반복 회수 - 0은 무한 반복
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorSetRepeatCount (MGLanimator *pAnimator, MGLuint nRepeatCount);

/** 
 * @brief External Image Layer에 Animator 를 사용할 수 있도록 설정.
 *
 * 이 함수에 준하는 함수가 Core 에서 작성될 수도 있으므로 이 함수는 사용되지 않을 수도 있음.
 * 
 * @param pAnimator 애니메이터
 * @param nCount Item 개수
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorSetExternalImageLayer (MGLanimator *pAnimator, MGLint nCount);

/** 
 * @brief 지정된 서브시스템을 업데이트 시키는 함수 
 * 
 * @param pAnimator 애니메이터 
 * @param index 서브 시스템에 대한 인덱스 
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorUpdate (MGLanimator *pAnimator, MGLint index);

/** 
 * @brief 지정된 서브 시스템의 애니메이션을 일시적으로 중지 시킴
 * 
 * @param pAnimator 애니메이터
 * @param index 서브 시스템에 대한 인덱스
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorStop (MGLanimator *pAnimator, MGLint index);

/** 
 * @brief 지정된 서브 시스템의 애니메이션을 재시작 시킴
 * 
 * @param pAnimator 애니메이터
 * @param index 서브 시스템에 대한 인덱스
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorStart (MGLanimator *pAnimator, MGLint index);

/** 
 * @brief 애니메이터가 관장하는 전체 애니메이션을 일시적으로 중지 시킴
 * 
 * @param pAnimator 애니메이터
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorStopAll (MGLanimator *pAnimator);

/** 
 * @brief 애니메이터가 관장하는 전체 애니메이션을 재시작 시킴
 * 
 * @param pAnimator  애니메이터
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorStartAll (MGLanimator *pAnimator);

/** 
 * @brief 애니메이터가 관장하는 전체 서브시스템을 업데이트 시킴
 * 
 * @param pAnimator 애니메이터
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorUpdateAll (MGLanimator *pAnimator);

/** 
 * @brief 애니메이터를 초기화 한다. 애니메이터 구동전에 호출해 주는 것이 좋다.
 * 
 * @param pAnimator 애니메이터
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorClear (MGLanimator *pAnimator);

/** 
 * @brief 디폴트 이미지를 세팅하는 함수 
 *
 * 이 함수는 레이어에 디폴트 이미지를 세팅할 방법이 없기 때문에 제공된다.
 * 디폴트 이미지가 세팅되어 있다면 GetImageLayer 함수가 호출될때, 
 * 서브 시스템으로 부터 얻어온 레이어에 이 이미지를 세팅한다.
 * 
 * @param pAnimator 애니메이터
 * @param nID 이미지 아이디
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorSetDefaultImage (MGLanimator *pAnimator, MGLint nID);

/**
 * @}
 * }}} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MGL_ANIMATOR_H */

/*
" vim: fileencoding=euc-kr :
*/
