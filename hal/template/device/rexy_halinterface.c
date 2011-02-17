/*
* $Id: rexy_halinterface.c 435 2005-02-16 08:25:12Z klenui $
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
*
* This file is a part of Rexy Embedded 
*
*/

/** @file */

/**
 * @defgroup halifc Hal Interface
 * These APIs are associated with Hal Interface
 * @ingroup HalGroup
 * @{
 */

#include "rexy.h"
#include "rexy_haldevice.h"

#define DEVICE_HEIGHT	LCD_HEIGHT
#define DEVICE_WIDTH	LCD_WIDTH

static UNITCL frame_buffer[DEVICE_HEIGHT][DEVICE_WIDTH] = {0,};

/** Get screen width.
*
* @return Return screen width
*/
int  RalScreenGetWidth(void)
{
    return DEVICE_WIDTH;
}

/** Get screen height.
*
* @return Return screen height
*/
int  RalScreenGetHeight(void)
{
    return DEVICE_HEIGHT;
}

/** Get screen buffer.
*
* @return Return screen buffer
*/
UNITCL* RalScreenGetBuffer(void)
{
    return (UNITCL*)frame_buffer;
}

/** Get screen memory size.
*
* @return Return screen memory size
*/
int RalScreenGetMemSize(void)
{
    return sizeof(frame_buffer);
}

/** Get current time.
*
* @param outCurrentTime 
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalTimeGetCurrent(struct tm* outCurrentTime)
{
    return TRUE;
}
// Created by Joshua on 10-Jan-2006
ruint32 RalTimeGetTicks(void)
{
	return 0;
}

bool RalTimeSetTimer(ruint32 inMilliseconds, int id)
{
    return TRUE;
}

bool RalTimeKillTimer(int id)
{
	return TRUE;
}


/** Get event.
*
* @param outMsg 
* @param inTimeoutMs 
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalEventGet(pmsg outMsg, ruint32 inTimeoutMs)
{
    return FALSE;
}

/** Convert host to dev.
*
* @param src 
* @param dev 
*
* @return converted value
*/
void HostToDev(unsigned int src, unsigned char * dev)
{

}

/** Convert dev to host.
*
* @param dev 
*
* @return converted value
*/
unsigned int DevToHost(unsigned char * dev)
{
    unsigned int ret_val;

    ret_val = 0;

    return ret_val;
}

/** Convert dev to host short.
*
* @param dev 
*
* @return converted value
*/
unsigned short DevToHostShort(unsigned char * dev)
{
    unsigned short ret_val;

    ret_val = 0;

    return ret_val;
}

/** Flush screen,
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalScreenFlush(void)
{

    return TRUE;
}

bool RalScreenFlushPart(rect* inBoundary)
{
    return TRUE;
}

/**
 * @brief		타겟에 사용하는 락 시스템을 초기화
 * @return		초기화 여부
 * @remarks		
 * @see			
 * @if HISTORY
 *				05-Jul-2006 yich Initiated 
 * @endif
 */
bool RalLckInit(void)
{
    return TRUE;
}

/**
 * @brief		락을 작동시킴
 * @param[in]	lockID	작동시킬 락 ID
 * @return		락 작동 여부
 * @remarks		
 * @see			
 * @if HISTORY
 *				05-Jul-2006 yich Initiated 
 * @endif
 */
bool RalLckLock(LockIDType lockID)
{
    return TRUE;
}

/** 
 * @brief		타겟에서 작동된 락을 해제
 * @param[in]	lockID 해제할 락 ID
 * @return 		락 해제 여부
 * @remarks
 * @see
 * @if HISTORY
 *				05-Jul-2006 yich Initiated 
 * @endif
 */
bool RalLckUnlock(LockIDType lockID)
{
    return TRUE;
}


/** 
 * @brief		타겟에서 제공하는 기능에 따른 값을 반환
 * @param[in]	type	타겟에서 사용하는 값을 알고자하는 기능 ID
 * @return 		타겟에서 사용하는 값을 반환
 * @remarks
 * @see
 * @if HISTORY
 *				05-Jul-2006 wijang Initiated 
 * @endif
 */

int RalGetSystemFeature (FeatureType type)
{
	return 0;
}

/** @} */ // End of Hal Interface
