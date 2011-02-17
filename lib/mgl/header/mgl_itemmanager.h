/**
 * $Id$
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 *
 * @file mgl_itemmanager.h
 *
 * @author tombraid <tombraid@mococo.com>
 *
 * @brief
 *  비슷한 애니메이션 데이터를 갖는 다수의 아이템을 지원하기 위한 Subsystem
 *
 * @date 2005-11-23
 *
 * Notes :
 *
 * Open Issues :
 *
 *
 */

#ifndef MGL_ITEMMANAGER_H
#define MGL_ITEMMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Init Value Option Define {{{ */
/** 
 * @defgroup api_itemmanager_initval_option Item Manager Init Value Option
 *
 * Item Manager 초기 설정값 의 옵션 
 * 
 * @addtogroup api_itemmanager_initval_option
 * @{
 * */
#define MGL_ITEMMANAGER_INITVAL_OPTION_NONE				0	/**< 초기 설정갑에 대한 옵션이 없음 */
#define MGL_ITEMMANAGER_INITVAL_OPTION_ASCENDING		1	/**< 초기값을 구할때 이전 아이템 값과 더하여 현재 아이템 값을 구함 */
#define MGL_ITEMMANAGER_INITVAL_OPTION_DESCENDING		2	/**< 초기값을 구할때 이전 아이템 값>과 빼서 현재 아이템 값을 구함 */
#define MGL_ITEMMANAGER_INITVAL_OPTION_STATIC_VALUE		3	/**< 고정된 값을 초기값으로 사용 */
/** 
 * @}
 * }}} */ 

/* Option Define {{{ */
/** 
 * @defgroup api_itemmanager_option Item Manager Option
 *
 * Item Manager 의 옵션 
 * 
 * @addtogroup api_itemmanager_option
 * @{
 * */
#define MGL_ITEMMANAGER_OPTION_NONE			0				/**< Item Manager 옵션이 없음 */
#define MGL_ITEMMANAGER_OPTION_KEEP_LAST	(1 << 0)		/**< 마지막 값을 유지하는 옵션 */
/** 
 * @}
 * }}} */ 

/* Data Structure {{{ */
/** 
 * @defgroup api_itemmanager_data_structure Item Manager Data Structure
 * 
 * Item Manager 자료구조
 *
 * @addtogroup api_itemmanager_data_structure
 * @{
 * */
/** 
 * @brief 초기값을 구하기 위한 설정을 저장하는 구조체
 *
 * from, to 사이의 값을 step 단위로 구함. 
 * 당분간 쉽게 구현하기 위해 step은 1로만 처리되는 것으로 한다.
 */
typedef struct MGLinitval {
	MGLint from;		/**< 시작값 */
	MGLint to;			/**< 끝값 */
	MGLint step;		/**< 스텝 */
	MGLint nParamID;	/**< Param ID */
	MGLint nOption;		/**< Option */
	MGLvoid *pIP;		/**< IP에 대한 pointer */
} MGLinitval;

/** 
 * @brief 여러개의 그려질 아이템을 처리하기 위한 구조체
 */
typedef struct MGLitemmanager {
	MGLint nSeq;				/**< 현재 시퀀스 - 타이밍에 의해 변경되는 값 */
	MGLint nOption;				/**< Option */

	/* 초기 값을 얻기 위한 설정 */
	MGLinitval *pInitValue;		/**< 초기값 설정 배열 */
	MGLint nInitValueCount;		/**< 수정할 효과의 개수 */

	/* 이하는 초기값 */
	MGLanidata *pItemInitValue;	/**< 초기값 설정에 의해 구해진 초기값 */
	MGLanidata *pInterval;		/**< 초기값 설정에 의해 구해진 인터벌 값으로 pItemInitValue의 마지막 부분
								  (마지막 nItemCount만큼을 인터벌 값의 영역으로 한다.)에 대한 포인터이다. */
	MGLint nItemCount;			/**< 아이템의 개수 */
	
	/* 이하는 다수의 아이템 처리를 위해 사용 */
	MGLint nCurItemIdx;			/**< 현재 아이템 인덱스 - GetImageLayer 함수 호출시 처리 */
	MGLint nCurInterval;		/**< 현재 그려질 녀석의 인터벌값 - GetImageLayer 함수 호출 시 처리 */

	MGLsubsystem tSubsystem;	/**< 서브시스템 - 키프레임이랑 연결되었다면 키프레임 */
} MGLitemmanager;
/** 
 * @}
 * }}} */

/* API for User {{{ */
/** 
 * @defgroup api_itemmanager_api_for_user Item Manager API for User
 *
 * 사용자를 위한 Item Manager API
 * 
 * @addtogroup api_itemmanager_api_for_user
 * @{
 * */
/** 
 * @brief ItemManager를 초기화 하는 함수
 * 
 * @param pItemManager ItemManager 
 * @param pInitValue 초기값을 구하기 위한 설정값을 저장할 배열  
 * @param nInitValueCount 설정값의 개수 
 * @param nItemCount 아이템의 개수
 * @param pItemInitValue 초기값이 저장될 공간의 배열
 * @param nOption ItemManager의 옵션
 * 
 * @return MGLerror
 */
MGLerror
MGL_SetItemManager (MGLitemmanager *pItemManager, MGLinitval *pInitValue, MGLint nInitValueCount,
		MGLint nItemCount, MGLanidata *pItemInitValue, MGLint nOption);

/** 
 * @brief Animator가 ItemManager를 사용할 수 있도록 세팅해 주는 함수
 * 
 * @param pItemManager ItemManager
 * @param pSubsystem Animator가 가지고 있는 서브 시스템
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetInSubsystem (MGLitemmanager *pItemManager, MGLsubsystem *pSubsystem);

/** 
 * @brief ItemManager의 초기값 설정을 위한 범위값을 설정한다.
 * 
 * 이 함수를 사용하는 경우에 nOption을 MGL_ITEMMANAGER_INITVAL_OPTION_STATIC_VALUE로 세팅하면, 
 * MGL_ERROR_NOT_SUPPORTED_OPTION 에러를 낸다.
 *
 * @param pItemManager ItemManager
 * @param index 인덱스 
 * @param from 시작 범위값
 * @param to 끝 범위값
 * @param step 간격 
 * @param pIP IP에 대한 포인터
 * @param nParamID Param ID
 * @param nOption Option value
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetInitValue (MGLitemmanager *pItemManager, MGLint index, MGLint from, MGLint to, MGLint step, MGLvoid *pIP, MGLint nParamID, MGLint nOption);

/** 
 * @brief static 한 초기값을 설정하는 함수 
 * 
 * 이 함수를 사용하는 경우에 nOption은 MGL_ITEMMANAGER_INITVAL_OPTION_STATIC_VALUE로 자동 세팅된다.
 * @param pItemManager ItemManager
 * @param index 인덱스 
 * @param pValues 값들의 배열
 * @param nCount 개수
 * @param pIP IP에 대한 포인터
 * @param nParamID Param ID
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetStaticInitValue (MGLitemmanager *pItemManager, MGLint index, MGLint *pValues, MGLint nCount, MGLvoid *pIP, MGLint nParamID);

/** 
 * @brief ItemManager의 초기 인터벌 설정을 위한 범위값을 설정한다. 
 *
 * MGL_ItemManagerSetInitValue와 구분되어 있는 이유는 MGLint drawarg 가 일단 INTERVAL을 지정할 수 없기 때문이다.
 * 
 * @param pItemManager ItemManager
 * @param index 인덱스 
 * @param from 시작 범위값
 * @param to 끝 범위값
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetInitInterval (MGLitemmanager *pItemManager, MGLint index, MGLint from, MGLint to);

/** 
 * @brief static한 인터벌 세팅하는 함수 
 * 
 * @param pItemManager ItemManager
 * @param index 인덱스 
 * @param pIntervals 값들의 배열
 * @param nCount 개수
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetStaticInterval (MGLitemmanager *pItemManager, MGLint index, MGLint *pIntervals, MGLint nCount);

/** 
 * @}
 * }}} */

/* Animation Subsystem Interface {{{ */
/** 
 * @defgroup api_itemmanager_asi Item Manager Animation Subsystem Interface
 *
 * Item Manager의 Animation Subsystem Interface
 * 
 * @addtogroup api_itemmanager_asi
 * @{
 * */
/** 
 * @brief 프레임을 업데이트 한다. 
 * 
 * @param pSubsystem ItemManager
 * @param timing 타이밍 값
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ItemManagerUpdateScene (MGLvoid *pSubsystem, MGLint timing);

/** 
 * @brief 이미지 레이어 정보를 채우는 함수 
 *
 * 이 함수는 내부적으로 자신이 관리하는 서브 시스템에서 정보를 가져온다.
 * 
 * @param index 아이템의 인덱스
 * @param pLayer 정보를 채울 레이어
 * @param pSubsystem ItemManager
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ItemManagerGetImageLayer (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief ItemManager가 관리하는 애니메이션을 초기화 함
 * 
 * @param pSubsystem ItemManager
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerClear (MGLvoid *pSubsystem);
/** 
 * @}
 * }}} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MGL_ITEMMANAGER_H */

/*
" vim: fileencoding=euc-kr :
*/
