/*
 * $Id: rexy_util.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Device dependent�� �Լ� �� ����� ����� ���ǵǾ� �ִ�.
 *
 */

#ifndef __REXY_UTIL_H__
#define __REXY_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */

    int		UtlBitmapCopy(RxBITMAP* pBitmapDst, RxBITMAP* pBitmapSrc, int nDstX, int nDstY);
    int		UtlBitmapCopyExt(RxBITMAP* pBitmapDst, RxBITMAP* pBitmapSrc, int nDstX, int nDstY, int gap);
    //int		UtlSetVal(WND window, int val);

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_UTIL_H__
