/*
*
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
*
* This file is a part of Rexy Embedded
*
*/

/** @file */


/**
 * @defgroup RexyELock Core Lock
 * @ingroup RexyECoreGroup
 * @{
 */

#include "rexy.h"

/**
 * @brief		RexyE���� ����� ���� �ʱ�ȭ
 * @return		�ʱ�ȭ ���� ����
 * @remarks		
 * @see			
 * @if HISTORY
 *				06-Jan-2006 madholic Initiated 
 *				06-Jul-2006 yich Move the function body to HAL
 * @endif
 */
bool LckInit(void)
{
    return RalLckInit();
}

/**
 * @brief		Ư�� ���� �۵���Ų��
 * @param[in]	lockID	�۵� ��ų �� ID�� ����	
 * @return		�� �۵� ����
 * @remarks		
 * @see			
 * @if HISTORY
 *				06-Jan-2006 madholic Initiated 
 *				06-Jul-2006 yich Move the function body to HAL
 * @endif
 */
bool LckLock(LockIDType lockID)
{
    return RalLckLock(lockID);
}

/** 
 * @brief		Ư�� ���� ������Ų��
 * @param[in]	lockID	���� ��ų �� ID�� ����	
 * @return		�� ���� ����
 * @remarks
 * @see
 * @if HISTORY
 *				06-Jan-2006 madholic Initiated 
 *				06-Jul-2006 yich Move the function body to HAL
 * @endif
 */
bool LckUnlock(LockIDType lockID)
{
    return RalLckUnlock(lockID);
}

/** @} */ // end lock
