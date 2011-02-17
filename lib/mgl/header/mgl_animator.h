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
 *  MGL���� Animation�� �����ϱ� ���� �ý��� 
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
 * Animator ������ ���Ǵ� Ÿ�̹� �ɼǿ� ���� ����
 *
 * @see MGL_AnimatorSetTiming
 *
 * @addtogroup api_animator_option_timing
 * @{
 */

#define MGL_TIMING_TYPE_TIME	0	/**< �ӵ��� ������ �� �ֵ��� Ÿ�̹��� ��� */
#define MGL_TIMING_TYPE_FRAME	1	/**< �������� �����ϵ��� Ÿ�̹� ��� */

/**
 * @}
 * }}} */

/* Param Define {{{ */
/** 
 * @defgroup api_animator_option_params Animator Params
 *
 * Animator �� ���� Params 
 * IP ���� �ٷ�� Params �� ����
 * 
 * @addtogroup api_animator_option_params
 * @{
 * */
// layer params
#define MGL_ANI_PARAM_POSITION              1	/**< layer �� ��ġ�� ���� Param Type */
#define MGL_ANI_PARAM_POSITION_X            2	/**< layer �� X ��ǥ�� ���� Param Type */
#define MGL_ANI_PARAM_POSITION_Y            3	/**< layer �� Y ��ǥ�� ���� Param Type */
#define MGL_ANI_PARAM_IMAGE					4	/**< layer �� �̹����� ���� Param Type */

// interval params
#define MGL_ANI_PARAM_INTERVAL              5	/**< �������� ���ݿ� ���� Param Type */

/** 
 * @}
 * }}} */

/* Animation Subsystem Interface {{{ */
/** 
 * @defgroup api_animator_subsystem_interface Animator Subsystem Interface
 *
 * Animator �� Subsystem ���� interface
 * 
 * @see page_subsystem
 * @addtogroup api_animator_subsystem_interface
 * @{
 * */
/** 
 * @brief Animation Subsystem Interface �� ���� �ý��ۿ� �ð��� �帧�� �����Ѵ�.
 * 
 * @param pSubsystem ���� �ý��� 
 * @param timing �ð��� �帧���� ������� ���̴�. 1�ʰ� �����ð��̰� updatescene �� ȣ��ȵ� 5�ʰ� ��ȴٸ� timing ���� 5�̴�.
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLupdatescene) (MGLvoid *pSubsystem, MGLint timing);

/** 
 * @brief Animation Subsystem Interface �� Layer�� ������ ���´�.
 *
 * �� Ÿ���� MGLlayer �� MGL_UpdateImageLayerFuncPtr �� ���� Ÿ���̴�. 
 * �� �Լ��� ȣ���� ����ȣ���� ����޴´�.
 * 
 * @param index ���� �������� �ε���
 * @param pLayer ���� �������� ������ ����� �޸�
 * @param pSubsystem ���� �ý���
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLgetimagelayer) (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief Animation Subsystem Interface �� ���� �ý����� ������ ������ ���·� ������. 
 *
 * �ִϸ��̼��� �߰��� ���߰� ó������ ������ ȿ���� �ִ�.
 * 
 * @param pSubsystem  ���� �ý���
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLclear) (MGLvoid *pSubsystem);

/** 
 * @brief Animation Subsystem Interface �� ���ڿ� �־��� ������ �ѹ��� �̿��Ͽ� layer�� ������ ���´�.
 *
 * ItemManger�� ���� ����ý����� ���� ����ý����� �̿��ϴ� ��� ����� �� �ִ�.
 * ��� ���� �ý����� �� �����ؾ� �ϴ� ���� �ƴϴ�. 
 * �� �Լ��� �������� �ʴ� ��� ItemManager ��� ���� �� interface �� ����ϴ� ���� �ý��۰��� ������� �ʴ´�.
 * 
 * @param index ���� �������� �ε���
 * @param pLayer ���� �������� ������ ����� �޸�
 * @param nSeq ������ �ѹ� 
 * @param pSubsystem ���� �ý���
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
 * Animator �� ���� �ڷᱸ��
 * 
 * @addtogroup api_animator_data_structure
 * @{
 * */
/** 
 * @brief ��ġ ������ �����ϴ� Ÿ��
 */
typedef struct MGLpoint {
	MGLint x;	/**< x ��ǥ �� */
	MGLint y;	/**< y ��ǥ �� */
} MGLpoint;

/** 
 * @brief ���������� Ÿ��
 */
typedef union MGLanidata {
	MGLfixed fixedpoint;	/**< �������� ������ �� ������ ��� */
	MGLint integer;			/**< �������� ���İ� */
	MGLpoint point;			/**< �������� ��ġ�� */
} MGLanidata;

/** 
 * @brief �ִϸ����Ͱ� �ٷ�� ���� �ý��ۿ� ���� ������ ��� Ÿ��
 */
typedef struct MGLsubsystem {
	MGLint bStart;			/**< ���� �ý����� ���� ���¸� ��Ÿ�� */
	MGLvoid *pSubsystem; 	/**< ���� �ý��� */

	/* ���ϴ� Animation Subsystem Interface�� ������ �κ��̴�. */ 
	MGLupdatescene fpUpdateScene; 		/**< ASI MGLupdatescene�� ���� Function Pointer */
	MGLgetimagelayer fpGetImageLayer;	/**< ASI MGLgetimagelayer �� ���� Function Pointer */
	MGLclear fpClear;					/**< ASI MGLclear �� ���� Function Pointer */
	MGLgetlayerwithframe fpGetLayerWithFrame;	/**< ASI MGLgetlayerwithframe �� ���� Function Pointer */
} MGLsubsystem;

/** 
 * @brief �ִϸ����� Ÿ��
 */
typedef struct MGLanimator {
	/* ���ϴ� Ÿ�̹� ����� ���� �κ��̴�. */
	MGLint nLastTiming;	/**< ������ �� ����� Timing �� ������ Tick ���� �ٸ���. */
	MGLint nTimingType; /**< Ÿ�̹� ����� �ϴ� Ÿ�� */
	MGLint nInterval;	/**< �ִϸ��̼� ���͹� �̰��� �̿��Ͽ� Framerate �� ������ �� �ִ�. */ 
	MGLuint nStartTick;	/**< �ִϸ��̼��� ������ ƽ */

	/* ���ϴ� Subsystem���� ������ ���� �κ��̴�. */ 
	MGLint nSubsystemCount;	/**< �ִϸ����Ϳ� ������ ���� �ý����� ���� */
	MGLsubsystem *pSubsystems; /**< �ִϸ����Ϳ� ������ ���� �ý����� �迭 */
	MGLuint nEndMask;		/**< ����ý��ۺ� �ִϸ��̼� ���� ����ũ, n��° �ִϸ��̼��� �����ٸ� n��° ��Ʈ�� 0 */

	/* ���ϴ� �ִϸ��̼� ���࿡ ������ �κ��̴�. */ 
	MGLint nRepeatCount; 	/**< �ִϸ��̼� �ݺ� ȸ�� */
	MGLint nCurCount;		/**< ���� �ݺ� �� */

	/* ���ϴ� Get Image Layer */
	MGLint nUpdateLayerSubsystemCount;	/**< GetImageLayer �� �����ϴ��� index �� ������ �ѱ�� ���� ����ϴ� ���� */
	MGLint nUpdateLayerSubsystemIdx;	/**< GetImageLayer �� �����ϴ��� �ʿ��� ���� ������ �����;� �� ���� �ý����� �ε��� */

	/* ���ϴ� image ���� */
	MGLint nImageID;	/**< ����Ʈ �̹��� ID */
} MGLanimator;
/** 
 * @}
 * }}} */

/* API for User {{{ */
/** 
 * @defgroup api_animator_for_user Animator API for User
 *
 * Animator �� ����ϱ� ���� ����� API
 *
 * @see api_animator_option_timing
 *
 * @addtogroup api_animator_for_user
 * @{
 */

/** 
 * @brief �ִϸ����͸� �ʱ�ȭ �ϴ� �Լ�
 * 
 * @param pAnimator �ִϸ�����
 * @param pSubsystem �ִϸ����Ϳ� �Բ� �����ؼ� ����� ���� �ý����� ������ ���� �迭
 * @param nSubsystemCount �ִϸ����Ϳ� �Բ� �����ؼ� ����� ���� �ý����� ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_SetAnimator (MGLanimator *pAnimator, MGLsubsystem *pSubsystem, MGLint nSubsystemCount);

/** 
 * @brief �ִϸ����Ͱ� Ÿ�̹��� ����ϱ� ���� �ʿ��� ������ �����ϴ� �Լ�
 * 
 * @param pAnimator  �ִϸ�����
 * @param nTimingType Ÿ�̹� ��� ��� @ref api_animator_option_timing
 * @param nInterval �ִϸ��̼ǿ��� ���� ������ ������ ���͹� - �� ���� �����Ͽ� FrameRate �� ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorSetTiming (MGLanimator *pAnimator, MGLint nTimingType, MGLint nInterval);

/** 
 * @brief �ִϸ��̼� �ݺ�ȸ���� �����ϴ� �Լ�. 0�� ���� �ݺ�
 * 
 * @param pAnimator  �ִϸ�����
 * @param nRepeatCount �ݺ� ȸ�� - 0�� ���� �ݺ�
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorSetRepeatCount (MGLanimator *pAnimator, MGLuint nRepeatCount);

/** 
 * @brief External Image Layer�� Animator �� ����� �� �ֵ��� ����.
 *
 * �� �Լ��� ���ϴ� �Լ��� Core ���� �ۼ��� ���� �����Ƿ� �� �Լ��� ������ ���� ���� ����.
 * 
 * @param pAnimator �ִϸ�����
 * @param nCount Item ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorSetExternalImageLayer (MGLanimator *pAnimator, MGLint nCount);

/** 
 * @brief ������ ����ý����� ������Ʈ ��Ű�� �Լ� 
 * 
 * @param pAnimator �ִϸ����� 
 * @param index ���� �ý��ۿ� ���� �ε��� 
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorUpdate (MGLanimator *pAnimator, MGLint index);

/** 
 * @brief ������ ���� �ý����� �ִϸ��̼��� �Ͻ������� ���� ��Ŵ
 * 
 * @param pAnimator �ִϸ�����
 * @param index ���� �ý��ۿ� ���� �ε���
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorStop (MGLanimator *pAnimator, MGLint index);

/** 
 * @brief ������ ���� �ý����� �ִϸ��̼��� ����� ��Ŵ
 * 
 * @param pAnimator �ִϸ�����
 * @param index ���� �ý��ۿ� ���� �ε���
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorStart (MGLanimator *pAnimator, MGLint index);

/** 
 * @brief �ִϸ����Ͱ� �����ϴ� ��ü �ִϸ��̼��� �Ͻ������� ���� ��Ŵ
 * 
 * @param pAnimator �ִϸ�����
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorStopAll (MGLanimator *pAnimator);

/** 
 * @brief �ִϸ����Ͱ� �����ϴ� ��ü �ִϸ��̼��� ����� ��Ŵ
 * 
 * @param pAnimator  �ִϸ�����
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorStartAll (MGLanimator *pAnimator);

/** 
 * @brief �ִϸ����Ͱ� �����ϴ� ��ü ����ý����� ������Ʈ ��Ŵ
 * 
 * @param pAnimator �ִϸ�����
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorUpdateAll (MGLanimator *pAnimator);

/** 
 * @brief �ִϸ����͸� �ʱ�ȭ �Ѵ�. �ִϸ����� �������� ȣ���� �ִ� ���� ����.
 * 
 * @param pAnimator �ִϸ�����
 * 
 * @return MGLerror
 */
MGLerror
MGL_AnimatorClear (MGLanimator *pAnimator);

/** 
 * @brief ����Ʈ �̹����� �����ϴ� �Լ� 
 *
 * �� �Լ��� ���̾ ����Ʈ �̹����� ������ ����� ���� ������ �����ȴ�.
 * ����Ʈ �̹����� ���õǾ� �ִٸ� GetImageLayer �Լ��� ȣ��ɶ�, 
 * ���� �ý������� ���� ���� ���̾ �� �̹����� �����Ѵ�.
 * 
 * @param pAnimator �ִϸ�����
 * @param nID �̹��� ���̵�
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
