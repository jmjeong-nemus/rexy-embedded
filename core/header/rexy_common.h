/*
 * $Id: common.h 8339 2005-10-03 11:54:23Z jmjeong $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded
 *
 */

#ifndef __REXY_COMMON_H__
#define __REXY_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rexy_gdi.h"


    /** @file */
#define WNDSTYLE_MASK					0x0000001FL		// mask

#define WNDSTYLE_BKGND_MASK				0x00000007L		// bkgnd mask
#define WNDSTYLE_CUSTOM_BKGND			0x00000000L		// custom bkgnd
#define WNDSTYLE_TEXTURE_BKGND			0x00000001L		// texture bkgnd
#define WNDSTYLE_WHITE_BKGND			0x00000002L		// white bkgnd
#define WNDSTYLE_WALLPAPER_BKGND0		0x00000003L		// wallpaper level 0
#define WNDSTYLE_WALLPAPER_BKGND1		0x00000004L		// wallpaper level 1
#define WNDSTYLE_WALLPAPER_BKGND2		0x00000005L		// wallpaper level 2

#define WNDSTYLE_DLG					0x00000008L		// dialog style
#define WNDSTYLE_WITH_SCROLLBAR			0x00000010L		// wnd with scrollbar style


    //====================================================================
    //	[[ Global Variables ]]
    //--------------------------------------------------------------------

#ifdef __cplusplus
    extern "C" {
#endif  // __cplusplus

        //====================================================================
        //	[[ Common Functions ]]
        //--------------------------------------------------------------------


        //====================================================================
        //	[[ Util Functions ]]
        //--------------------------------------------------------------------

        /** @addtogroup gdi
         *
         * @{
         */

        /** sets the coordinates of the specified rectangle. */
#define SET_RECT(P, L, T, R, B)	if((P)) { (P)->left=L; (P)->top=T; (P)->right=R; (P)->bottom=B; }
        /*
        * - P		structure with rectangle to set
        * - L		left coordinate
        * - T		top coordinate
        * - R		right coordinate
        * - B		bottom coordinate
        */

        /** @} */

        //====================================================================
        //	[ Draw Text ]
        //--------------------------------------------------------------------

#define ALIGN_LEFT		0x00
#define ALIGN_RIGHT		0x10
#define ALIGN_CENTER	0x20


        //====================================================================
        //	[ Control Functions ]
        //--------------------------------------------------------------------

#ifdef __cplusplus
    }
#endif  // __cplusplus

#define case_NUMBER		case VK_PAD_0: case VK_PAD_1: case VK_PAD_2: \
						case VK_PAD_3: case VK_PAD_4: case VK_PAD_5: \
						case VK_PAD_6: case VK_PAD_7: case VK_PAD_8: case VK_PAD_9


#ifdef __cplusplus
}
#endif	// __cplusplus

#endif  // __REXY_COMMON_H__

