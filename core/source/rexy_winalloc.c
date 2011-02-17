/*
 * $Id: rexy_winalloc.c 435 2005-02-16 08:25:12Z longdal $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */

/** @file */

/**
 *  @defgroup winalloc Core Window Allocating Service
 *  Window Management API Set
 *  @ingroup RexyECoreGroup
 *  @{
 */

#include "rexy.h"

/*======================================================================*/
/* Declarations								*/
/*======================================================================*/
/*======================================================================*/
/* Public APIs								*/
/*======================================================================*/
void init_w_obj( WND wnd );
void InitWNDFreeStructures(void);
void DelWNDFreeStructures(void);
WND malloc_w( int size );
bool isfreeslot(int magic);
void free_w( WND wnd );
/*======================================================================*/
/* Private								*/
/*======================================================================*/
/*======================================================================*/
/* Module Variables							*/
/*======================================================================*/
int g_window_alloc_count = 0;
static WINDOW 	g_wnd_pool[MAX_WND_STORAGE];
static WINDOW	* g_free_wnd = NULL;
#define FREE_MAGIC_NUMBER (343036)


/*======================================================================*/
/* Implementation							*/
/*======================================================================*/
void init_w_obj( WND wnd )
{
    MemSet((void *)wnd, 0, sizeof(WINDOW));
    return;
}

void InitWNDFreeStructures(void)
{
    int i;

    MemSet((void *)g_wnd_pool, 0, sizeof(g_wnd_pool));

    for ( i = 0; i < (MAX_WND_STORAGE - 1); ++i )
        g_wnd_pool[i].nextfree = &g_wnd_pool[i+1];

    g_wnd_pool[i].nextfree = NULL;
    g_free_wnd = g_wnd_pool;
    return;
}

void DelWNDFreeStructures(void)
{
    return;
}

WND malloc_w( int size )
{
    WND new_wnd;

    if ( g_free_wnd == NULL )
    {
        return NULL;
    }
    else
    {
        new_wnd = g_free_wnd;
        g_free_wnd = g_free_wnd->nextfree;
    }
    TRACE(TR_RexyEtc, TR_LEVEL4, ("malloc_w counter [%d]", ++g_window_alloc_count));

    init_w_obj(new_wnd);
    return new_wnd;
}


bool isfreeslot(int magic)
{
    return (bool)(magic == FREE_MAGIC_NUMBER);
}

void free_w( WND wnd )
{
    // guard code
    //
    if (isfreeslot(wnd->freeMagicNumber)) {
        TRACE(TR_RexyEtc, TR_LEVEL1, ("Already free slot. Do Nothing"));
        return;
    }
    TRACE(TR_RexyEtc, TR_LEVEL4, ("malloc_w counter [%d]", --g_window_alloc_count));

    wnd->nextfree = g_free_wnd;
    g_free_wnd = wnd;

    wnd->freeMagicNumber = FREE_MAGIC_NUMBER;

    return;
}

/** @} */ // End of Window Allocating Service
