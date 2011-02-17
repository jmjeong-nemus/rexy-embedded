/*
 * $Id: rexy_util.c 9002 2005-11-03 03:32:48Z madholic $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */

/** @file */

/**
 * @defgroup util Core Utility Service
 * Utility Service API
 * @ingroup RexyECoreGroup
 * @{
 */

#include "rexy.h"
#include "rexy_util.h"

/*======================================================================*/
/* Module Variables							*/
/*======================================================================*/
#define MASK_ADDR(addr)	(void*)((unsigned long)(addr)|0xA0000000)

/*======================================================================*/
/* Implementation							*/
/*======================================================================*/

/**
* @brief		
* @param	pBitmapDst
* @param	pBitmapSrc
* @param	nDstX
* @param	nDstY
* @param	gap
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int UtlBitmapCopyExt(RxBITMAP* pBitmapDst, RxBITMAP* pBitmapSrc, int nDstX, int nDstY, int gap)
{
    int i;
    COLORVAL*	pBitmapDataSrc = (COLORVAL*)pBitmapSrc->data;
    COLORVAL* 	pBitmapDataDst = (COLORVAL*)pBitmapDst->data;
    int width = 0;
    int height = 0;


    if( pBitmapDst->width <= nDstX )
    {
        return 1;
    }
    if( pBitmapDst->height <= nDstY )
    {
        return 2;
    }

    // starting point
    pBitmapDataDst = pBitmapDataDst + nDstY*(pBitmapDst->width) + nDstX;
    pBitmapDataSrc = pBitmapDataSrc + (pBitmapSrc->width*gap);

    width = pBitmapSrc->width;
    if(width > RalScreenGetWidth())
        width = RalScreenGetWidth();

    height = pBitmapSrc->height;
    if(height > RalScreenGetHeight())
        height = RalScreenGetHeight();

    for(i=0; i<(width)*(height); i++)
    {
        *pBitmapDataDst	= *(pBitmapDataSrc+i);

        if( (i+1)%(pBitmapSrc->width) == 0 )
        {
            /*
             *	if (TRUE) : till width
             */

            pBitmapDataDst = pBitmapDataDst +1  +((pBitmapDst->width) - (pBitmapSrc->width)) ;
        }
        else
        {
            /*
             *	if (FALSE) : reached at width position
             */

            pBitmapDataDst = pBitmapDataDst +1;
        }
    }

    return 0;
}

/**
* @brief		
* @param	pBitmapDst
* @param	pBitmapSrc
* @param	nDstX
* @param	nDstY
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int UtlBitmapCopy(RxBITMAP* pBitmapDst, RxBITMAP* pBitmapSrc, int nDstX, int nDstY)
{
    int i;
    COLORVAL*	pBitmapDataSrc = (COLORVAL*)pBitmapSrc->data;
    COLORVAL* 	pBitmapDataDst = (COLORVAL*)pBitmapDst->data;
    int width = 0;
    int height = 0;


    if( pBitmapDst->width <= nDstX )
    {
        return 1;
    }
    if( pBitmapDst->height <= nDstY )
    {
        return 2;
    }

    // starting point
    pBitmapDataDst = pBitmapDataDst + nDstY*(pBitmapDst->width) + nDstX;

    width = pBitmapSrc->width;
    if(width > RalScreenGetWidth())
        width = RalScreenGetWidth();

    height = pBitmapSrc->height;
    if(height > RalScreenGetHeight())
        height = RalScreenGetHeight();

    for(i=0; i<(width)*(height); i++)
    {
        *pBitmapDataDst	= *(pBitmapDataSrc+i);

        if( (i+1)%(pBitmapSrc->width) == 0 )
        {
            /*
             *	if (TRUE) : till width
             */

            pBitmapDataDst = pBitmapDataDst +1  +((pBitmapDst->width) - (pBitmapSrc->width)) ;
        }
        else
        {
            /*
             *	if (FALSE) : reached at width position
             */

            pBitmapDataDst = pBitmapDataDst +1;
        }
    }

    return 0;
}

/**
* @brief		Set 'value' variable of the previous window structure.
* @param[in]	window		Handle of the window.
* @param[in]	int			New 'value' variable of the window parameter.
* @return		'value' variable of the previous window structure
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
/*
int UtlSetVal(WND window, int val)
{
    int old_val;

    old_val = window->value;
    window->value = val;

    return old_val;
}
*/
/** @} */ // End of Util Service
