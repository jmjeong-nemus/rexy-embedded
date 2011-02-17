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
 *  �ܼ��� Sprite Animation�� �����ϱ� ���� Subsystem
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
 * Image Selector ���� �ɼ�
 *
 * @addtogroup api_imageselector_option
 * @{
 * */

/** ������ ��ġ���� ���� */
#define MGL_IMGSEL_START_RANDOM				(1 << 0)
/** �������� �����ϰ� �����Ͽ� ���� */
#define MGL_IMGSEL_START_RANDOM_SEQUENCE	(1 << 1)

/** �־��� ������ ������ ���. ������ ������ ���ٸ� 1 timing �� �ϳ��� �̹����� ���� */
#define MGL_IMGSEL_KEEP_ORDER				(1 << 2)
/** ���� �̹����� �ٲٴ� Ÿ�̹��� �����ϰ� ���� */
#define MGL_IMGSEL_IGNORE_TIMING			(1 << 3)

/** �������� ����Ǵ� ��쿡 ������ �������� ����. �� �ɼ� ���ý� ANIMATION END�� ���� */
#define MGL_IMGSEL_SELECT_SEQUENCE			(1 << 4)

/** ������ ���� ����, �������� ������ ���� ��������, Ÿ�ֵ̹� ����� */
#define MGL_IMGSEL_AUTO						(MGL_IMGSEL_RANDOM_START  		\
												| MGL_IMGSEL_SELECT_SEQUENCE	\
												| MGL_IMGSEL_KEEP_ORDER)

/** ImageSelector �ɼ��� ���� */
#define MGL_IMGSEL_COUNT					5
/** ImageSelector �ɼǸ���ũ�� ������ �� */
#define MGL_IMGSEL_LAST						(1 << MGL_IMGSEL_COUNT)
/** 
 * @}
 * }}} */

/* data structure {{{ */
/** 
 * @defgroup api_imageselector_data_structure Image Selector Data Structure
 *
 * Image Selector�� �ڷᱸ��
 * 
 * @addtogroup api_imageselector_data_structure
 * @{
 * */
/** 
 * @brief MGL���� image ������ ID �� �ϱ� ������ MGLint Ÿ���� MGLimgselitem Ÿ������ ����Ѵ�.
 */
typedef MGLint MGLimgselitem;
/** 
 * @brief �����ӿ� ���� ������ ������ ���� ��ȣ���� �ٷ��.
 */
typedef MGLint MGLimgselframeinfo;
/** 
 * @brief �������� ���� ������ �̹��� �迭�� �ε����� �ǹ��Ѵ�.
 */
typedef MGLint MGLimgselseqinfo;

/** 
 * @brief �̹��� ������ ���� ������ ���� ������ �ϴ� ������
 *
 * �ټ��� �����۵鿡 ���� ���� �̹��� ������ ��������� 
 * ���� ������� ���õǰ��� ���� ������ ����� �� �ִ�.
 */
typedef struct MGLimgselarg {
	MGLint nTiming;		/**< Ÿ�̹� �� */
	MGLint nCurImgIdx;	/**< ���� �̹��� �ε��� */
	MGLint nCurSeqIdx;	/**< ���� ������ �ε��� - �� ���� option�� ���� ������ ���� �� �ִ�. */
} MGLimgselarg;

/** 
 * @brief �̹��� ������ ���� Ÿ�� ^^
 *
 * ��쿡 ���� ������ ������ �迭�̳� ������ ������ �迭�� ������ ���� �� �ִ�.
 * ������ ������ �迭�� ����ִ� ���� ������ ���� ���� �ε��� �̴�. (�߿�)
 * ������ ������ �迭�� ����ִ� ���� ������ ������ ��ȣ�̴�. (�߿�)
 * ������ ������ ��ȣ��� �ǹ̴� �ϳ��� �̹����� 3��° �����ӿ��� 7��° �����ӱ���
 * �����ȴٰ� ������ 7�̶�� ���� �ǹ��Ѵ�.
 */
typedef struct MGLimgselector {
	MGLint option;				/**< �ɼǰ� - ������ �� �ִ� ���� Define ���� */
	MGLimgselarg tImgSelArg;	/**< �̹��� ������ ���� ���� */

	MGLimgselitem *pImageIDs;	/**< �̹��� �迭 */
	MGLimgselframeinfo *pFrameInfos;	/**< ������ ������ �迭 - �̹��� �迭�� ũ�� �� ���ƾ� �� */
	MGLint nImageCount;				/**< �̹��� �迭�� ũ�� */

	MGLimgselseqinfo *pSeqInfos;	/**< ������ ������ �迭 */
	MGLint nSeqInfoCount;			/**< ������ ���� �迭�� ũ�� */
} MGLimgselector;
/** 
 * @}
 * }}} */

/* API for user {{{ */
/** 
 * @defgroup api_imageselector_api_for_user Image Selector API for User
 *
 * ����ڸ� ���� Image Selector API
 * 
 * @addtogroup api_imageselector_api_for_user
 * @{
 * */
/** 
 * @brief �̹��� �����͸� �ʱ�ȭ �ϴ� �Լ��̴�.
 * 
 * @param pImageSelector �̹��� ������
 * @param pImages �̹����鿡 ���� ������ ���� �迭 - �̹����� ������ŭ �־����� ��
 * @param pFraemInfos ������ ������ ���� �迭 - �̹����� ������ŭ �־����� ��
 * @param nImageCount �̹��� ����
 * @param pSeqInfos ������ ������ ���� �迭 - ������ ������ŭ �־����� ��
 * @param nSeqInfoCount ������ ����
 * @param option �ɼ��� ���ǵ� ������ �������� ������� MGL_IMGSEL_XXX define ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_SetImageSelector (MGLimgselector *pImageSelector, MGLimgselitem *pImages, 
		MGLimgselframeinfo *pFraemInfos, MGLint nImageCount, 
		MGLimgselseqinfo *pSeqInfos, MGLint nSeqInfoCount, MGLint option);


/** 
 * @brief �̹��� �����Ϳ� �̹��� �ϳ��� �����Ѵ�.
 * 
 * @param pImageSelector �̹��� ������ 
 * @param nImageID �̹��� ���̵�
 * @param index �̹��� �������� �̹��� �迭�� ������ �ε���
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetImage (MGLimgselector *pImageSelector, MGLint nImageID, MGLint index);

/** 
 * @brief �̹��� �����Ϳ� �̹��� �迭�� �����Ѵ�.
 * 
 * @param pImageSelector �̹��� ������
 * @param pImageIDs �̹��� ���̵���� �迭
 * @param count �̹��� ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetImages (MGLimgselector *pImageSelector, MGLint *pImageIDs, MGLint count);

/** 
 * @brief �̹��� �����Ϳ� �̹��� �������� �����Ѵ�.
 * 
 * @param pImageSelector �̹��� ������
 * @param pImageIDs �̹��� ���������� �̹��� ���̵���� �迭
 * @param nImageIdx �̹��� �����Ϳ� �ִ� �̹��� �迭�� �ε����� �̹��� �������� ����� ��ġ
 * @param count �̹��� ����
 * @param nSeqIdx �̹��� �������� ������ ������ ����� �ε���
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetImageSeq (MGLimgselector *pImageSelector, MGLint *pImageIDs, MGLint nImageIdx, MGLint count, MGLint nSeqIdx);

/** 
 * @brief �̹��� �����Ϳ� ������ ������ �����Ѵ�.
 * 
 * �� �Լ����� �߿��� ����Ʈ�� nOrder ���� '������' ������ ��ȣ��� ���̴�.
 * ���� ���, 3���� �̹����� 3�����Ӿ� ���ӵǴ� �� 9������¥�� ���ϸ��̼��� �ִٸ�,
 * �� �Լ��� ���� ȣ��Ǵµ� �׶� ������ ���ڴ� ������ ����.
 * MGL_ImageSelectorSetFrameInfo (pImageSelector, 2, 0);
 * MGL_ImageSelectorSetFrameInfo (pImageSelector, 5, 1);
 * MGL_ImageSelectorSetFrameInfo (pImageSelector, 8, 2);
 *
 * @param pImageSelector �̹��� ������
 * @param nOrder �ε����� �ش��ϴ� �̹����� '������' ������ ��ȣ
 * @param index �̹����� �ε���
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetFrameInfo (MGLimgselector *pImageSelector, MGLint nOrder, MGLint index);

/** 
 * @brief �̹��� �����Ϳ� ������ �������� �����Ѵ�.
 *
 * MGL_ImageSelectorSetFrameInfo �� ���� ����
 * 
 * @param pImageSelector �̹��� ������
 * @param pOrders �ε����� �ش��ϴ� �̹����� '������'������ ��ȣ��
 * @param count �̹��� ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetFrameInfos (MGLimgselector *pImageSelector, MGLint *pOrders, MGLint count);

/** 
 * @brief �̹��� �����Ϳ� ������ ������ �����Ѵ�.
 * 
 * @param pImageSelector �̹��� ������
 * @param nStart �̹��� �迭���� �������� ���� ��ġ
 * @param nCount �������� ���� �̹����� ����
 * @param index ������ ������ �ε���
 * 
 * @return MGLerror
 */
MGLerror
MGL_ImageSelectorSetSequenceInfo (MGLimgselector *pImageSelector, MGLint nStart, MGLint nCount, MGLint index);

/** 
 * @brief �ִϸ����Ϳ��� �̹��� �����͸� ����� �� �ֵ��� �����Ѵ�.
 * 
 * @param pImageSelector �̹��� ������ 
 * @param pSubsystem ���� �ý��� 
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
 * Image Selector�� Animation Support Interface
 * 
 * @addtogroup api_imageselector_asi
 * @{
 * */
/** 
 * @brief �ð��� �帧�� ������ �ش�.
 * 
 * @param pSubsystem ����ý��� (���⼭�� �̹��� ������)
 * @param nTiming �帥 �ð��� ���� Ÿ�̹� ��
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ImageSelectorUpdateScene (MGLvoid *pSubsystem, MGLint nTiming);

/** 
 * @brief �̹��� ���̾ ������ ä���ش�.
 * 
 * @param index �ε���
 * @param pLayer ������� �� ���̾��� ������
 * @param pSubsystem ����ý��� (���⼭�� �̹��� ������)
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ImageSelectorGetImageLayer (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief �̹��� ������ ������ �ʱ�ȭ �Ѵ�. �����ε��� ����
 * 
 * @param pSubsystem ����ý��� (���⼭�� �̹��� ������)
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
