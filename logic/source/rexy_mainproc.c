/*
 * $Id: rexy_mainproc.c 5436 2005-07-21 02:29:10Z madholic $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */

/** @file */

/**
 * @defgroup logic_rexy_mainproc Logic MainProc
 * These APIs are associated with MainProc of the Logic
 * @ingroup LogicGroup
 * @{
 */

#include "rexy.h"

/** MainProc
*
* @param hWnd	
* @param msg	
* @param wParam	
* @param lParam	
* 
* @return Return 0
*/
int MainProc(WND hWnd, int msg, int wParam, int lParam)
{
    WINPROC proc;

    TRACE(TR_RexyCore, TR_LEVEL1, ("MainProc( %08X, %d, %d, %d )", hWnd, msg, wParam, lParam) );

    proc = StaGetCurrentProc();
    if ( proc && proc(hWnd, msg, wParam, lParam) < 0 )
    {
        return MsgDefWindowProc(hWnd, msg, wParam, lParam);
    }
    else
    {
        return 0;
    }
}

/** @} */ // End of Logic MainProc
