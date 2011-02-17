/*
 * $Id: rexy_error.h 5441 2005-07-21 04:31:49Z joshua $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded
 *
 */

#ifndef __REXY_ERROR_H__
#define __REXY_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */

    // Created by Joshua on 20-Dec-2005

#define RxErrNone		0x0000
#define RxErrAniClass 	0x0100
#define RxErrFntClass	0x0200
#define RxErrGdiClass	0x0300
#define RxErrMemClass	0x0400
#define RxErrMsgClass	0x0500
#define RxErrMgqClass	0x0600
#define RxErrRscClass	0x0700
#define RxErrStrClass	0x0800
#define RxErrTrcClass	0x0900
#define RxErrUsrClass	0x0A00
#define RxErrUtlClass	0x0B00
#define RxErrWinClass	0x0C00
#define RxErrWlcClass	0x0D00
#define RxErrStaClass	0x0E00

    // You can use 0x00~0xFF as error code.
#define enInvalidParam		0x01
#define enNullPointer		0x02
#define enNotEnoughMem		0x03
#define enOutOfRange		0x04
#define enFileNotFound		0x05
#define enOverLockCount		0x06
#define enUnderLockCount	0x07
#define enSystemError		0x08
#define enInvisibleWindow	0x09
#define enNoIntersectRegion	0x0A
#define enAlreadyExist		0x0B
// Add your error list here

    // rexy control error base value
#define CTLERR_BASE			0x1000

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_ERROR_H__
