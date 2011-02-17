/*
 * $Id: rexy_type.h 6110 2005-08-03 07:40:59Z jmjeong $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * In this file, types and data structures for Rexy are defined.
 *
 */

#ifndef __REXY_TYPE_H__
#define __REXY_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

    /** @file */
#include "rexy_haltype.h"

#ifdef BOOL
#undef BOOL
#endif

typedef ruint8	bool;
typedef ruint32 dword;
typedef ruint8  byte;
typedef ruint16 UNITCL;
typedef ruint16 WORD;
typedef ruint16 COLORVAL;



#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

    //	[ TRUE, FALSE ]
    //	TRUE, FALSE macro
#ifndef TRUE
#define TRUE		1
#endif

#ifndef FALSE
#define FALSE		0
#endif

    struct _window;
    typedef int (*WINPROC) (struct _window * hwnd, int msg, int wparam, int lparam);

    /** Rectangle Structure */
    typedef struct _rect {
        /** left */
        int left;

        /** top */
        int top;

        /** right */
        int right;

        /** bottom */
        int bottom;
    } rect;

    typedef struct _scr_dev {
        int width;
        int height;
        int mem_size;
        UNITCL * mem_addr;

    } SCR_DEV;

    typedef struct _hardware_setting {
        int lcd_brightness;
        int lcd_on_off;
        int keypad_on_off;
    } HARDWARE_SETTING;

    /** Message Structure
     *
     */
    typedef struct _msg {
        /** kind of message */
        int     wmsg;

        /** target window */
        int     window;

        /** param 1 */
        int     wparam;

        /** param 2 */
        int     lparam;
    } msg, *pmsg;

    /** Window structure definition */

    typedef struct _window {
        struct _window * parentwin;		///< parent window
        struct _window * prevwin;		///< previous window
        struct _window * nextwin;		///< next window
        struct _window * childwin;		///< child window ( mainly control )
        int id;							///< control id

        int style;						///< cnotrol style
        //int value;

        int window_type;				///< window type :
        int window_visible;				///< window visible

        WINPROC WndProc;				///< window main message handler

        rect frame;						///< window real frame rectangle
        rect clip;						///< window clipping area

        //char * text;

        UNITCL fg_color;				///< foreground color
        UNITCL bg_color;				///< background color

        SCR_DEV * dev;					///< frame buffer to window
        struct _window * nextfree;
        void*  pData;					///< control data structure
        void*  pfnProc;					///< user callback function for control

        int  freeMagicNumber;      // to indicate this window is not used
    } WINDOW, *WND;

    typedef struct _tm_msg {
        WND window;
        int wparam_id;
        long interval;
        long due;
    } tm_msg;

    /** Point Structure */
    typedef struct __point {

        /** x coordination */
        int x;

        /** y coordination */
        int y;
    } Point;


    /** Bitmap Information Structure.
     * This structure is used in various Bitmap loading, drawing functions. 
     */
    typedef struct {
        int width;				///< bitmap width
        int height;				///< bitmap height
        int bTransparent;		///< flag to indicate whether this bitmap has transparent pixels.
        ///< if this flag is set as BITMAP_TRANSPARENT, the pixels whose color value is "transparentColor"
        ///< will be treated as the transparent pixel.
        int bpp;				///< bits per pixel in this bitmap
        COLORVAL transparentColor;	///< the color of the pixel to treat as a transparent pixel.
        void * palette;				///< palette information. not used.
        void * data;				///< field holding all bitmap pixels.
    } RxBITMAP, * RxHBITMAP;

	typedef unsigned short RxErr;

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_TYPE_H__
