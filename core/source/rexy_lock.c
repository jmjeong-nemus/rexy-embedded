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
 * @brief		RexyE에서 사용할 락을 초기화
 * @return		초기화 성공 여부
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
 * @brief		특정 락을 작동시킨다
 * @param[in]	lockID	작동 시킬 락 ID를 전달	
 * @return		락 작동 여부
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
 * @brief		특정 락을 해제시킨다
 * @param[in]	lockID	해제 시킬 락 ID를 전달	
 * @return		락 해제 여부
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
