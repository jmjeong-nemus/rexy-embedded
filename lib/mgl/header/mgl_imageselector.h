/**
 * $ID$
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 *
 * @file mgl_imageselector.h
 *
 * @author tombraid <tombraid@mococo.com>
 *
 * @brief
 *  단순한 Sprite Animation을 지원하기 위한 Subsystem
 *
 * @date 2005-11-25
 *
 * Notes :
 *
 * Open Issues :
 *
 *
 */

#ifndef MGL_IMAGESELECTOR_H
#define MGL_IMAGESELECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* define {{{ */
/** 
 * @defgroup api_imageselector_option Image Selector Option
 * 
 * Image Selector 설정 옵션
 *
 * @addtogroup api_imageselector_option
 * @{
 * */

/** 랜덤한 위치에서 시작 */
#define MGL_IMGSEL_START_RANDOM				(1 << 0)
/** 시퀀스를 랜덤하게 선택하여 시작 */
#define MGL_IMGSEL_START_RANDOM_SEQUENCE	(1 << 1)

/** 주어진 프래임 정보에 기반. 프레임 정보가 없다면 1 timing 에 하나의 이미지가 사용됨 */
#define MGL_IMGSEL_KEEP_ORDER				(1 << 2)
/** 다음 이미지로 바꾸는 타이밍을 랜덤하게 선택 */
#define MGL_IMGSEL_IGNORE_TIMING			(1 << 3)

/** 시퀀스가 종료되는 경우에 랜덤한 시퀀스를 선택. 이 옵션 선택시 ANIMATION END는 없음 */
#define MGL_IMGSEL_SELECT_SEQUENCE			(1 << 4)

/** 시작은 완전 랜덤, 시퀀스가 끝나면 랜덤 시퀀스로, 타이밍도 맘대로 */
#define MGL_IMGSEL_AUTO						(MGL_IMGSEL_RANDOM_START  		\
												| MGL_IMGSEL_SELECT_SEQUENCE	\
												| MGL_IMGSEL_KEEP_ORDER)

/** ImageSelector 옵션의 개수 */
#define MGL_IMGSEL_COUNT					5
/** ImageSelector 옵션마스크의 마지막 값 */
#define MGL_IMGSEL_LAST						(1 << MGL_IMGSEL_COUNT)
/** 
 * @}
 * }}} */

/* data structure {{{ */
/** 
 * @defgroup api_imageselector_data_structure Image Selector Data Structure
 *
 * Image Selector의 자료구조
 * 
 * @addtogroup api_imageselector_data_structure
 * @{
 * */
/** 
 * @brief MGL에서 image 관리를 ID 로 하기 때문에 MGLint 타입을 MGLimgselitem 타입으로 사용한다.
 */
typedef MGLint MGLimgselitem;
/** 
 * @brief 프레임에 대한 정보는 프레임 순서 번호만을 다룬다.
 */
typedef MGLint MGLimgselframeinfo;
/** 
 * @brief 시퀀스에 대한 정보는 이미지 배열의 인덱스를 의미한다.
 */
typedef MGLint MGLimgselseqinfo;

/** 
 * @brief 이미지 선택을 위해 아이템 별로 가져야 하는 데이터
 *
 * 다수의 아이템들에 대해 같은 이미지 집합을 사용하지만 
 * 같은 방식으로 선택되고자 하지 않을때 사용할 수 있다.
 */
typedef struct MGLimgselarg {
	MGLint nTiming;		/**< 타이밍 값 */
	MGLint nCurImgIdx;	/**< 현재 이미지 인덱스 */
	MGLint nCurSeqIdx;	/**< 현재 시퀀스 인덱스 - 이 값은 option에 따라서 쓰이지 않을 수 있다. */
} MGLimgselarg;

/** 
 * @brief 이미지 셀렉터 메인 타입 ^^
 *
 * 경우에 따라서 시퀀스 정보의 배열이나 프레임 정보의 배열은 사용되지 않을 수 있다.
 * 시퀀스 정보의 배열에 들어있는 값은 시퀀스 끝에 대한 인덱스 이다. (중요)
 * 프레임 정보의 배열에 들어있는 값은 마지막 프레임 번호이다. (중요)
 * 마지막 프레임 번호라는 의미는 하나의 이미지가 3번째 프레임에서 7번째 프레임까지
 * 유지된다고 했을때 7이라는 값을 의미한다.
 */
typedef struct MGLimgselector {
	MGLint option;				/**< 옵션값 - 설정할 수 있는 값은 Define 참조 */
	MGLimgselarg tImgSelArg;	/**< 이미지 선택을 위한 인자 */

	MGLimgselitem *pImageIDs;	/**< 이미지 배열 */
	MGLimgselframeinfo *pFrameInfos;	/**< 프레임 정보의 배열 - 이미지 배열의 크기 와 같아야 함 */
	MGLint nImageCount;				/**< 이미지 배열의 크기 */

	MGLimgselseqinfo *pSeqInfos;	/**< 시퀀스 정보의 배열 */
	MGLint nSeqInfoCount;			/**< 시퀀스 정보 배열의 크기 */
} MGLimgselector;
/** 
 * @}
 * }}} */

/* API for user {{{ */
/** 
 * @defgroup api_imageselector_api_for_user Image Selector API for User
 *
 * 사용자를 위한 Image Selector API
 * 
 * @addtogroup api_imageselector_api_for_user
 * @{
 * */
/** 
 * @brief 이미지 셀렉터를 초기화 하는 함수이다.
 * 
 * @param pImageSelector 이미지 셀렉터
 * @param pImages 이미지들에 대한 정보를 담을 배열 - 이미지의 개수만큼 주어져야 함
 * @param pFraemInfos 프레임 정보를 담을 배열 - 이미지의 개수만큼 주어져야 함
 * @param nImageCount 이미지 개수
 * @param pSeqInfos 시퀀스 정보를 담을 배열 - 시퀀스 개수만큼 주어져야 함
 * @param nSeqInfoCount 시퀀스 개수
 * @param option 옵션은 정의된 값들의 조합으로 만들어짐 MGL_IMGSEL_XXX define 참조
 * 
 * @return MGLerror
 */
MGLerror
MGL_SetImageSelector (MGLimgselector *pImageSelector, MGLimgselitem *pImages, 
		MGLimgselframeinfo *pFraemInfos, MGLint nImageCount, 
		MGLimgselseqinfo *pSeqInfos, MGLint nSeqInfoCount, MGLint option);


/** 
 * @brief 이미지 셀렉터에 이미지 하나를 세팅한다.
 * 
 * @param pImageSelector 이미지 셀렉터 
 * @param nImageID 이미지 아이디
 * @param index 이미지 셀렉터의 이미지 배열에 저장할 인덱스
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetImage (MGLimgselector *pImageSelector, MGLint nImageID, MGLint index);

/** 
 * @brief 이미지 셀렉터에 이미지 배열을 세팅한다.
 * 
 * @param pImageSelector 이미지 셀렉터
 * @param pImageIDs 이미지 아이디들의 배열
 * @param count 이미지 개수
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetImages (MGLimgselector *pImageSelector, MGLint *pImageIDs, MGLint count);

/** 
 * @brief 이미지 셀렉터에 이미지 시퀀스를 세팅한다.
 * 
 * @param pImageSelector 이미지 셀렉터
 * @param pImageIDs 이미지 시퀀스에서 이미지 아이디들의 배열
 * @param nImageIdx 이미지 셀렉터에 있는 이미지 배열의 인덱스로 이미지 시퀀스가 저장될 위치
 * @param count 이미지 개수
 * @param nSeqIdx 이미지 셀렉터의 시퀀스 정보가 저장될 인덱스
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetImageSeq (MGLimgselector *pImageSelector, MGLint *pImageIDs, MGLint nImageIdx, MGLint count, MGLint nSeqIdx);

/** 
 * @brief 이미지 셀렉터에 프레임 정보를 세팅한다.
 * 
 * 이 함수에서 중요한 포인트는 nOrder 값이 '마지막' 프레임 번호라는 것이다.
 * 예를 들어, 3장의 이미지가 3프레임씩 지속되는 총 9프레임짜리 에니메이션이 있다면,
 * 이 함수는 세번 호출되는데 그때 각각의 인자는 다음과 같다.
 * MGL_ImageSelectorSetFrameInfo (pImageSelector, 2, 0);
 * MGL_ImageSelectorSetFrameInfo (pImageSelector, 5, 1);
 * MGL_ImageSelectorSetFrameInfo (pImageSelector, 8, 2);
 *
 * @param pImageSelector 이미지 셀렉터
 * @param nOrder 인덱스에 해당하는 이미지의 '마지막' 프레임 번호
 * @param index 이미지의 인덱스
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetFrameInfo (MGLimgselector *pImageSelector, MGLint nOrder, MGLint index);

/** 
 * @brief 이미지 셀렉터에 프레임 정보들을 세팅한다.
 *
 * MGL_ImageSelectorSetFrameInfo 의 설명 참조
 * 
 * @param pImageSelector 이미지 셀렉터
 * @param pOrders 인덱스에 해당하는 이미지의 '마지막'프레임 번호들
 * @param count 이미지 개수
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetFrameInfos (MGLimgselector *pImageSelector, MGLint *pOrders, MGLint count);

/** 
 * @brief 이미지 셀렉터에 시퀀스 정보를 세팅한다.
 * 
 * @param pImageSelector 이미지 셀렉터
 * @param nStart 이미지 배열에서 시퀀스의 시작 위치
 * @param nCount 시퀀스에 속한 이미지의 개수
 * @param index 시퀀스 정보의 인덱스
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetSequenceInfo (MGLimgselector *pImageSelector, MGLint nStart, MGLint nCount, MGLint index);

/** 
 * @brief 애니메이터에서 이미지 셀렉터를 사용할 수 있도록 세팅한다.
 * 
 * @param pImageSelector 이미지 셀렉터 
 * @param pSubsystem 서브 시스템 
 * 
 * @return 
 */
MGLerror
MGL_ImageSelectorSetInSubsystem (MGLimgselector *pImageSelector, MGLsubsystem *pSubsystem);
/** 
 * @}
 * }}} */

/* Animation Support Interface Implementation {{{ */
/** 
 * @defgroup api_imageselector_asi Image Selector Animation Support Interface
 *
 * Image Selector의 Animation Support Interface
 * 
 * @addtogroup api_imageselector_asi
 * @{
 * */
/** 
 * @brief 시간의 흐름을 가르쳐 준다.
 * 
 * @param pSubsystem 서브시스템 (여기서는 이미지 셀렉터)
 * @param nTiming 흐른 시간에 대한 타이밍 값
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ImageSelectorUpdateScene (MGLvoid *pSubsystem, MGLint nTiming);

/** 
 * @brief 이미지 레이어에 정보를 채워준다.
 * 
 * @param index 인덱스
 * @param pLayer 결과물이 들어갈 레이어의 포인터
 * @param pSubsystem 서브시스템 (여기서는 이미지 셀렉터)
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ImageSelectorGetImageLayer (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief 이미지 셀렉터 정보를 초기화 한다. 리와인드의 역할
 * 
 * @param pSubsystem 서브시스템 (여기서는 이미지 셀렉터)
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ImageSelectorClear (MGLvoid *pSubsystem);
/** 
 * @}
 * }}} */ 

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MGL_IMAGESELECTOR_H */

/*
" vim: fileencoding=euc-kr :
*/
