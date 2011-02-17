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
 *  ����� �ִϸ��̼� �����͸� ���� �ټ��� �������� �����ϱ� ���� Subsystem
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
 * Item Manager �ʱ� ������ �� �ɼ� 
 * 
 * @addtogroup api_itemmanager_initval_option
 * @{
 * */
#define MGL_ITEMMANAGER_INITVAL_OPTION_NONE				0	/**< �ʱ� �������� ���� �ɼ��� ���� */
#define MGL_ITEMMANAGER_INITVAL_OPTION_ASCENDING		1	/**< �ʱⰪ�� ���Ҷ� ���� ������ ���� ���Ͽ� ���� ������ ���� ���� */
#define MGL_ITEMMANAGER_INITVAL_OPTION_DESCENDING		2	/**< �ʱⰪ�� ���Ҷ� ���� ������ ��>�� ���� ���� ������ ���� ���� */
#define MGL_ITEMMANAGER_INITVAL_OPTION_STATIC_VALUE		3	/**< ������ ���� �ʱⰪ���� ��� */
/** 
 * @}
 * }}} */ 

/* Option Define {{{ */
/** 
 * @defgroup api_itemmanager_option Item Manager Option
 *
 * Item Manager �� �ɼ� 
 * 
 * @addtogroup api_itemmanager_option
 * @{
 * */
#define MGL_ITEMMANAGER_OPTION_NONE			0				/**< Item Manager �ɼ��� ���� */
#define MGL_ITEMMANAGER_OPTION_KEEP_LAST	(1 << 0)		/**< ������ ���� �����ϴ� �ɼ� */
/** 
 * @}
 * }}} */ 

/* Data Structure {{{ */
/** 
 * @defgroup api_itemmanager_data_structure Item Manager Data Structure
 * 
 * Item Manager �ڷᱸ��
 *
 * @addtogroup api_itemmanager_data_structure
 * @{
 * */
/** 
 * @brief �ʱⰪ�� ���ϱ� ���� ������ �����ϴ� ����ü
 *
 * from, to ������ ���� step ������ ����. 
 * ��а� ���� �����ϱ� ���� step�� 1�θ� ó���Ǵ� ������ �Ѵ�.
 */
typedef struct MGLinitval {
	MGLint from;		/**< ���۰� */
	MGLint to;			/**< ���� */
	MGLint step;		/**< ���� */
	MGLint nParamID;	/**< Param ID */
	MGLint nOption;		/**< Option */
	MGLvoid *pIP;		/**< IP�� ���� pointer */
} MGLinitval;

/** 
 * @brief �������� �׷��� �������� ó���ϱ� ���� ����ü
 */
typedef struct MGLitemmanager {
	MGLint nSeq;				/**< ���� ������ - Ÿ�ֿ̹� ���� ����Ǵ� �� */
	MGLint nOption;				/**< Option */

	/* �ʱ� ���� ��� ���� ���� */
	MGLinitval *pInitValue;		/**< �ʱⰪ ���� �迭 */
	MGLint nInitValueCount;		/**< ������ ȿ���� ���� */

	/* ���ϴ� �ʱⰪ */
	MGLanidata *pItemInitValue;	/**< �ʱⰪ ������ ���� ������ �ʱⰪ */
	MGLanidata *pInterval;		/**< �ʱⰪ ������ ���� ������ ���͹� ������ pItemInitValue�� ������ �κ�
								  (������ nItemCount��ŭ�� ���͹� ���� �������� �Ѵ�.)�� ���� �������̴�. */
	MGLint nItemCount;			/**< �������� ���� */
	
	/* ���ϴ� �ټ��� ������ ó���� ���� ��� */
	MGLint nCurItemIdx;			/**< ���� ������ �ε��� - GetImageLayer �Լ� ȣ��� ó�� */
	MGLint nCurInterval;		/**< ���� �׷��� �༮�� ���͹��� - GetImageLayer �Լ� ȣ�� �� ó�� */

	MGLsubsystem tSubsystem;	/**< ����ý��� - Ű�������̶� ����Ǿ��ٸ� Ű������ */
} MGLitemmanager;
/** 
 * @}
 * }}} */

/* API for User {{{ */
/** 
 * @defgroup api_itemmanager_api_for_user Item Manager API for User
 *
 * ����ڸ� ���� Item Manager API
 * 
 * @addtogroup api_itemmanager_api_for_user
 * @{
 * */
/** 
 * @brief ItemManager�� �ʱ�ȭ �ϴ� �Լ�
 * 
 * @param pItemManager ItemManager 
 * @param pInitValue �ʱⰪ�� ���ϱ� ���� �������� ������ �迭  
 * @param nInitValueCount �������� ���� 
 * @param nItemCount �������� ����
 * @param pItemInitValue �ʱⰪ�� ����� ������ �迭
 * @param nOption ItemManager�� �ɼ�
 * 
 * @return MGLerror
 */
MGLerror
MGL_SetItemManager (MGLitemmanager *pItemManager, MGLinitval *pInitValue, MGLint nInitValueCount,
		MGLint nItemCount, MGLanidata *pItemInitValue, MGLint nOption);

/** 
 * @brief Animator�� ItemManager�� ����� �� �ֵ��� ������ �ִ� �Լ�
 * 
 * @param pItemManager ItemManager
 * @param pSubsystem Animator�� ������ �ִ� ���� �ý���
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetInSubsystem (MGLitemmanager *pItemManager, MGLsubsystem *pSubsystem);

/** 
 * @brief ItemManager�� �ʱⰪ ������ ���� �������� �����Ѵ�.
 * 
 * �� �Լ��� ����ϴ� ��쿡 nOption�� MGL_ITEMMANAGER_INITVAL_OPTION_STATIC_VALUE�� �����ϸ�, 
 * MGL_ERROR_NOT_SUPPORTED_OPTION ������ ����.
 *
 * @param pItemManager ItemManager
 * @param index �ε��� 
 * @param from ���� ������
 * @param to �� ������
 * @param step ���� 
 * @param pIP IP�� ���� ������
 * @param nParamID Param ID
 * @param nOption Option value
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetInitValue (MGLitemmanager *pItemManager, MGLint index, MGLint from, MGLint to, MGLint step, MGLvoid *pIP, MGLint nParamID, MGLint nOption);

/** 
 * @brief static �� �ʱⰪ�� �����ϴ� �Լ� 
 * 
 * �� �Լ��� ����ϴ� ��쿡 nOption�� MGL_ITEMMANAGER_INITVAL_OPTION_STATIC_VALUE�� �ڵ� ���õȴ�.
 * @param pItemManager ItemManager
 * @param index �ε��� 
 * @param pValues ������ �迭
 * @param nCount ����
 * @param pIP IP�� ���� ������
 * @param nParamID Param ID
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetStaticInitValue (MGLitemmanager *pItemManager, MGLint index, MGLint *pValues, MGLint nCount, MGLvoid *pIP, MGLint nParamID);

/** 
 * @brief ItemManager�� �ʱ� ���͹� ������ ���� �������� �����Ѵ�. 
 *
 * MGL_ItemManagerSetInitValue�� ���еǾ� �ִ� ������ MGLint drawarg �� �ϴ� INTERVAL�� ������ �� ���� �����̴�.
 * 
 * @param pItemManager ItemManager
 * @param index �ε��� 
 * @param from ���� ������
 * @param to �� ������
 * 
 * @return MGLerror
 */
MGLerror
MGL_ItemManagerSetInitInterval (MGLitemmanager *pItemManager, MGLint index, MGLint from, MGLint to);

/** 
 * @brief static�� ���͹� �����ϴ� �Լ� 
 * 
 * @param pItemManager ItemManager
 * @param index �ε��� 
 * @param pIntervals ������ �迭
 * @param nCount ����
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
 * Item Manager�� Animation Subsystem Interface
 * 
 * @addtogroup api_itemmanager_asi
 * @{
 * */
/** 
 * @brief �������� ������Ʈ �Ѵ�. 
 * 
 * @param pSubsystem ItemManager
 * @param timing Ÿ�̹� ��
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ItemManagerUpdateScene (MGLvoid *pSubsystem, MGLint timing);

/** 
 * @brief �̹��� ���̾� ������ ä��� �Լ� 
 *
 * �� �Լ��� ���������� �ڽ��� �����ϴ� ���� �ý��ۿ��� ������ �����´�.
 * 
 * @param index �������� �ε���
 * @param pLayer ������ ä�� ���̾�
 * @param pSubsystem ItemManager
 * 
 * @return MGLerror
 */
MGLerror 
MGL_ItemManagerGetImageLayer (MGLint index, MGLlayer *pLayer, MGLvoid *pSubsystem);

/** 
 * @brief ItemManager�� �����ϴ� �ִϸ��̼��� �ʱ�ȭ ��
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
