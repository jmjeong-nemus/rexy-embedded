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
 * @brief START INDEX �� 2�̴�. �ٿ���� üũ�� ȸ���ϱ� ���ؼ� ������ ���̴�.
 */
#define MGL_KEYFRAME_START_INDEX		2

/** 
 * @defgroup api_keyframe_option_interpolation Keyframe Interpolation Option
 *
 * Keyframe ���� ��Ŀ� ���� �ɼ�
 * 
 * @addtogroup api_keyframe_option_interpolation 
 * @{
 * */
#define MGL_KEYFRAME_IPF_FIXED_LINEAR	0		/**< ���� ���� �Լ� fixed �� */
#define MGL_KEYFRAME_IPF_INT_LINEAR		1		/**< ���� ���� �Լ� int �� */
#define MGL_KEYFRAME_IPF_POINT_LINEAR	2		/**< ���� ���� �Լ� point_t ��*/
#define MGL_KEYFRAME_IPF_POINT_SPLINE	3		/**< spline ���� �Լ� point_t �� */
#define MGL_KEYFRAME_IPF_POINT_HERMITE	4		/**< hermite ���� �Լ� point_t �� */
#define MGL_KEYFRAME_IPF_POINT_ELLIPSE	5		/**< ellipse ���� �Լ� point_t �� */
#define MGL_KEYFRAME_IPF_USER			6		/**< ����� ���� �Լ� */
#define MGL_KEYFRAME_IPF_LAST			7		/**< �����Լ� ������ �ε��� - ������� ���� */
/** 
 * @}
 * */
/** 
 * @defgroup api_keyframe_option_ease Keyframe Ease Option
 *
 * Keyframe �ӵ� ���� ��Ŀ� ���� �ɼ�
 * 
 * @addtogroup api_keyframe_option_ease
 * @{
 * */
#define MGL_KEYFRAME_ESF_LINEAR			0 		/**< ���� ease �Լ� */
#define MGL_KEYFRAME_ESF_SIN 			1		/**< sin ease �Լ� */
#define MGL_KEYFRAME_ESF_USER 			2		/**< ����� ease �Լ� */
#define MGL_KEYFRAME_ESF_LAST			3		/**< ease �Լ� ������ �ε��� - ������� ���� */
/** 
 * @}
 * */

/* }}} */

/* data structure {{{ */ 
/** 
 * @defgroup api_keyframe_data_structure Keyframe Data Structure
 * 
 * Keyframe �ڷ� ����
 *
 * @addtogroup api_keyframe_data_structure
 * @{
 * */

/** 
 * @brief ���� �Լ��� ����
 */
typedef MGLint MGLipftype;

/** 
 * @brief ease �Լ��� ����
 */
typedef MGLint MGLesftype;

/** 
 * @brief �Ű������� ȣ���̸� ������ �ִ� ���̺�
 *
 *  -# ���̺��� ���������� �ʴ�. i ��° Keyframe�� ���� ������ i * INTERFRAME_RESOLUTION���� ���� �����Ѵ�.
 *  -# �� �����ӿ� ���� ��Ʈ�� ������ cnt[i] �� �ִ�.
 */
typedef struct MGLdistent {
	MGLfixed u;						/**< �Ű����� �� */
	MGLfixed distance; 				/**< ����������� �Ÿ� �� */
} MGLdistent;


struct MGLinterpolator;

/** 
 * @brief ������ �����ϴ� �Լ� Ÿ���̴�.
 * 
 * @param pInterpolator ������ ó���ϱ� ���� interpolator (������??)�� ������
 * @param u �Ű�����
 * @param parg ��ġ ������ ��� ��� ��ġ�� ����� point�� ������, �׿��� ��� NULL
 * 
 * @return MGLerror
 */
typedef MGLerror (*MGLipfunc) (struct MGLinterpolator *pInterpolator, MGLfixed u, MGLvoid *parg);

/** 
 * @brief �Ű����� u�� ������ ���� ���� �Լ� Ÿ���̴�. seq�� �̿��Ͽ� �Ű����� u�� ������ ����.
 * 
 * @param pInterpolator ������ ó���ϱ� ���� �������� ������ 
 * @param seq ������ ������ �ѹ�
 * 
 * @return ������ �Ű����� u
 */
typedef MGLfixed (*MGLeasefunc) (struct MGLinterpolator *pInterpolator, MGLint seq);

/** 
 * @brief �Ÿ��� ����ϱ� ���� �Լ� Ÿ���̴�. ������� ���� �޶��� �� �ֱ� ������ �̷� ������ �Ѵ�.
 * 
 * @param arg1 ù��° ����
 * @param arg2 �ι�° ���� 
 * 
 * @return �Ÿ� 
 */
typedef MGLfixed (*MGLgetdistfunc) (MGLvoid *arg1, MGLvoid *arg2);

/** 
 * @brief �����ڿ� ���� �ڷα���
 */
typedef struct MGLinterpolator {
	MGLint nParamID;				/**< nParamID ������ ���� (��ġ, ��, ����) */

	MGLipftype ipftype;				/**< ���� �Լ� Ÿ�� */
	MGLipfunc fpInterpolateFunc;	/**< ���� �Լ� */

	MGLesftype esftype;				/**< ease �Լ� Ÿ�� */
	MGLeasefunc fpEaseFunc;			/**< ease �Լ� */

	MGLgetdistfunc fpGetDistFunc;	/**< �Ÿ� ��� �Լ� */

	MGLint nEndSeq;					/**< ��ü ������ �� */

	MGLint nKeyLastIndex;			/**< Ű�������� ������ ���� ����ִ� �ε��� */
	MGLint nKeyDataCount;			/**< Ű�������� ũ�� */
	MGLanidata *pKeyData;			/**< �� Ű�������� ���� */

	MGLvoid *pIP;					/**< IP�� ���� ������ */

	MGLint nDistCurIdx;				/**< Distance Table �˻��� ���Ǵ� index */
	MGLint nDistEntCount;			/**< Distance Table ũ�� */
	MGLdistent *pDistEnt;			/**< ������ ���� Distance Table*/
	MGLfixed dTotalDistance;		/**< ��ü �Ÿ� */
} MGLinterpolator;

/** 
 * @brief keyframe main data type
 */
typedef struct MGLkeyframe {
	MGLint nSeq;					/**< Current Sequence */
	MGLint nResultFrameCount;		/**< ��ü ������ �� */
	MGLinterpolator *pInterpolators; 	/**< �����ڵ鿡 ���� ������ */
	MGLint nInterpolatorCount;			/**< keyframe �� �����ϴ� �����ڵ��� �� */
	MGLanidata *pResults;				/**< ����� �����ϴ� �迭. ������ nResultFrameCount * nInterpolatorCount */
} MGLkeyframe;

/** 
 * @}
 * }}} */

/* API for User */
/** 
 * @defgroup api_keyframe_api_for_user Keyframe API for User
 * 
 * Keyframe ����� API
 *
 * @addtogroup api_keyframe_api_for_user
 * @{
 * */
/* keyframe function {{{ */

/** 
 * @brief Keyframe �� �ʱ�ȭ �ϴ� �Լ��̴�. �� �Լ��� ���� ����ڴ� ����� �޸𸮵��� �Ҵ��� �־�� �Ѵ�.
 * 
 * @param pKeyFrame Keyframe ���� ������
 * @param nResultFrameCount ��ü ������ �� 
 * @param pInterpolators �����ڵ��� ������ - ����� ȿ�� ��ŭ�� �����ڸ� �Ҵ��Ͽ��� �Ѵ�.
 * @param nInterpolatorCount �����ڵ��� ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_SetKeyFrame (MGLkeyframe *pKeyFrame, MGLint nResultFrameCount, 
		MGLinterpolator *pInterpolators, MGLint nInterpolatorCount);

/** 
 * @brief Keyframe �� ���� �� �ֵ��� �̸� ���Ǿ�� �� �κе��� ó���� �ش�.
 *
 * @param pKeyFrame Keyframe�� ���� ������ 
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameStandby (MGLkeyframe *pKeyFrame);

/** 
 * @brief Keyframe ����� �̸� �����Ѵ�.
 * 
 * Keyframe ������ �ǽð����� ������ �ʴ� ��쿡 ����Ѵ�.
 * �޸� ��뷮�̳� ���ɵ��� ������ ��뿩�θ� ������ �� �ִ�.
 * �ݺ����� ���ϸ��̼��� �ʿ�� �ϴ� ��쳪 Ű ������, �Ÿ� ���̺� � ���� ����
 * ��� ������ ������ ����ϴ� ��쿡�� �� �Լ��� ����ϴ� ���� ����.
 * �� �Լ��� ȣ��Ǵ� ������ �ſ� �߿��ϴ�.
 * ����� ���Ǳ� ���ؼ��� MGL_KeyFrameSetInSubsystem ���� ȣ��Ǿ�� �Ѵ�.
 *
 * @param pKeyFrame Keyframe
 * @param pResults ����� ����� ������ �迭
 * @param nResultCount ����� ũ�� (�����Ӽ� * �����������ͼ�)
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFramePreCalculate (MGLkeyframe *pKeyFrame, 
		MGLanidata *pResults, MGLint nResultCount);

/** 
 * @brief PreCalcaulate�� ����ϴ� ��� �ʿ���� ������ �����Ѵ�.
 * 
 * PreCalculate�� ó���� �������� pInterpolators ���� �����ʹ� �޸� ������ �� �ִ�.
 * ������ free �Լ��� ���Ѵٴ� �ǹ̰� �ƴ϶� ��������� ������� ������ ǥ���Ѵٴ� �ǹ��̴�.
 * Realtime ���� ����ϴ� ��쿡 �� �Լ��� ȣ���ϸ� ������ �߻��Ѵ�.
 *
 * @param pKeyFrame Ű ������
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameFreeInterpolators (MGLkeyframe *pKeyFrame);

/** 
 * @brief Keyframe�� Animator�� Subsystem�� �����Ѵ�.
 * 
 * @param pKeyFrame ������ Keyframe
 * @param pSubsystem ����� Subsystem
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetInSubsystem (MGLkeyframe *pKeyFrame, MGLsubsystem *pSubsystem);

/* }}} */

/* interpolator function {{{ */
/** 
 * @brief �����ڷ� ���� ������ ������ ����
 */
#define MGL_KEYFRAME_GETPARAMID(pInterpolator)	((pInterpolator)->nParamID)

/** 
 * @brief �����ڷ� ���� ���� �Լ��� ������ ����
 */
#define MGL_KEYFRAME_GETIPFTYPE(pInterpolator)	((pInterpolator)->ipftype)

/** 
 * @brief �����ڷ� ���� ease �Լ��� ������ ����
 */
#define MGL_KEYFRAME_GETESFTYPE(pInterpolator)	((pInterpolator)->esftype)

/** 
 * @brief ������ �������� seq�� ����. �̴� �����ڰ� ������ ���ϸ��̼��� �� �������̴�.
 */
#define MGL_KEYFRAME_GETLASTFRAME(pInterpolator)		((pInterpolator)->nEndSeq)

/** 
 * @brief interpolator �� �����Ѵ�.
 *
 * �� �����ڰ� ����� Ű �����͵��� ����� ������ �Ÿ� ���̺��� ����� ������ ���õǾ���Ѵ�.
 * Ű �����Ͱ� ����� ������ �ٿ���� üũ�� ���ϱ� ���� (Ű ������ ���� + 2)�� ũ�⿩�� �Ѵ�.
 * �Ÿ� ���̺��� ����� ������ (Ű �������� ���� * �ػ� + 1)�� ũ�⿩�� �Ѵ�.
 * �ػ󵵰� �������� ��Ȯ�� �ӵ��� ����ǳ� �ʹ� ���� ���� �ʿ�� ����.
 * 
 * @param pInterpolator ���õ� interpolator �� ������
 * @param pIP ����� IP�� ���� ������
 * @param nParamID ������� Ÿ��
 * @param pKeyData keydata �� �迭 - ��ü Ű ������ + 2 ���� ������ �ʿ�
 * @param nKeyDataCount keydata �� ����
 * @param pDistEnt �Ÿ� ���̺� - (��ü Ű ������ - 1) * �ػ� + 1 �� ������ �ʿ�
 * @param nDistEndCount �Ÿ����̺��� entry ��
 * @param ipftype ������ �����Լ��� Ÿ�� 
 * @param fpUserIPFunc ����� ���� ���� �Լ� (����� ��� NULL)
 * @param fpUserGetDistFunc ����� ���� �Ÿ� ��� �Լ� (����� ��� NULL)
 * @param esftype ������ �Ű����� �����Լ��� Ÿ��
 * @param fpUserEaseFunc ����� ���� �Ű����� �����Լ� (����� ��� NULL)
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
 * @brief �������� ������ �����.
 * 
 * @param pInterpolator ������
 * 
 * @return MGLerror
 */
MGLerror
MGL_ClearInterpolator (MGLinterpolator *pInterpolator);

/* }}} */

/* keydata function {{{ */ 

/** 
 * @brief �����ڿ� Ű�����͸� �߰��Ѵ�.
 *
 * �߰��� Ư���Ѱ� �ִµ�, ���� �ٿ�������� �ٿ���� ���� �ϳ��� �� ����ִ�. �̴� �ٿ���� üũ�� ���ϱ� ���� ����̴�.
 * 
 * @param pInterpolator ������ 
 * @param pKeyData Keyframe���� �߰��� �����ӿ� ���� ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetKeyData (MGLinterpolator *pInterpolator, MGLanidata *pKeyData);

/** 
 * @brief ��ο� ���� Ű�����͸� ���� �߰��� �� �ֵ��� ���� �Լ��� ���������� MGL_KeyFrameSetKeyData�� ȣ���Ѵ�.
 * 
 * @param pInterpolator ������
 * @param x x's position
 * @param y y's position
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetPointKeyData (MGLinterpolator *pInterpolator, MGLint x, MGLint y);

/** 
 * @brief int ���� ���� Ű�������� ���� �߰��� �� �ֵ��� ���� �Լ��� ���������� MGL_KeyFrameSetKeyData �� ȣ���Ѵ�.
 * 
 * @param pInterpolator ������
 * @param integer int value
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetIntKeyData (MGLinterpolator *pInterpolator, MGLint integer);

/** 
 * @brief fixedpoint �� ���� Ű�������� ���� �߰��� �� �ֵ��� ���� �Լ��� ���������� MGL_KeyFrameSetKeyData �� ȣ���Ѵ�.
 * 
 * @param pInterpolator ������
 * @param fixed fixedpoint value
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetFixedKeyData (MGLinterpolator *pInterpolator, MGLfixed fixed);

/** 
 * @brief Ÿ������ �������� ������ 
 * 
 * @param pInterpolator ������ 
 * @param nCenterX Ÿ���� �߽� X ��ǥ 
 * @param nCenterY Ÿ���� �߽� Y ��ǥ 
 * @param nRadiusX Ÿ���� X ���� ������ 
 * @param nRadiusY Ÿ���� Y ���� ������ 
 * @param nFromDeg ���۰� 
 * @param nToDeg ����
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameSetEllipseKeyData (MGLinterpolator *pInterpolator, MGLint nCenterX, MGLint nCenterY,
							MGLint nRadiusX, MGLint nRadiusY, MGLint nFromDeg, MGLint nToDeg);

/** 
 * @brief Ű�������� ������ ������
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
 * Keyframe�� Animation Support Interface 
 * 
 * @addtogroup api_keyframe_asi
 * @{
 * */
/** 
 * @brief Keyframe�� �ð��� �帧�� �����Ѵ�.
 * 
 * @param pSubsystem  Keyframe
 * @param nTiming Ÿ�̹�
 * 
 * @return MGLerror
 */
MGLerror 
MGL_KeyFrameUpdateScene (MGLvoid *pSubsystem, MGLint nTiming);

/** 
 * @brief �ǽð����� ������ ������� ���Ѵ�.
 * 
 * @param index ������ �ε��� - ���⼭�� �׻� 0
 * @param pLayer ������ ����� ����� �ڷᱸ��
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror 
MGL_KeyFrameGetImageLayerInRealTime (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief �̹� ������ ��������� ���� ����� �����´�.
 * 
 * @param index ������ �ε��� - ���⼭�� �׻� 0
 * @param pLayer ������ ����� ����� �ڷᱸ��
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror 
MGL_KeyFrameGetImageLayerFromResult (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief �ǽð����� �־��� ������ �ѹ��� ���� ������ ������� ���Ѵ�.
 * 
 * @param index ������ �ε��� - ���⼭�� �׻� 0
 * @param pLayer ������ ����� ����� �ڷᱸ��
 * @param nSeq ������ �ѹ� 
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameGetLayerWithFrameFromResult (MGLint index, MGLlayer *pLayer, MGLint nSeq, MGLvoid *pSubsystem);

/** 
 * @brief �̹� ������ ��������κ��� �־��� ������ �ѹ��� �ش��ϴ� ������� ���Ѵ�.
 * 
 * @param index ������ �ε��� - ���⼭�� �׻� 0
 * @param pLayer ������ ����� ����� �ڷᱸ��
 * @param nSeq ������ �ѹ� 
 * @param pSubsystem Keyframe
 * 
 * @return MGLerror
 */
MGLerror
MGL_KeyFrameGetLayerWithFrameInRealTime (MGLint index, MGLlayer *pLayer, MGLint nSeq, MGLvoid *pSubsystem);

/** 
 * @brief ���� �������� Ű ������ �ִϸ��̼��� �����ε�(?) �Ѵ�. ^^
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
