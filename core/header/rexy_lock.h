/*
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded
 *
 */


#ifndef __REXY_LOCK_H__
#define __REXY_LOCK_H__

/** @file */

typedef enum {
    LOCK_ID_MSGQ,
    LOCK_ID_ALLOC_M,
    LOCK_ID_VIDEO,
    LOCK_ID_MAX
} LockIDType;

bool LckInit(void);
bool LckLock(LockIDType lockID);
bool LckUnlock(LockIDType lockID);

/** @} */

#endif//__REXY_LOCK_H__

