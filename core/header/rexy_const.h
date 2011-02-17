/*
 * $Id: rexy_const.h 8841 2005-10-21 03:02:15Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *  Defines most of constants used in Rexy Library
 *
 */

#ifndef __REXY_CONST_H__
#define __REXY_CONST_H__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */

#define AWS_VISIBLE						0x10000000L        
#define AWS_TABSTOP						0x00010000L			

#define CTRL						    0x1000

#define WND_BUTTON						0x1001
#define WND_EDITBOX						0x1002
#define WND_LISTBOX						0x1003
#define WND_POPUP						0x1004
#define WND_SLIDETEXT					0x1005
#define WND_ICONVIEW					0x1006
#define WND_ANIBITMAP					0x1007
#define WND_SCROLLBAR					0x1008
#define WND_PROGRESSBAR					0x1009
#define WND_COMBOBOX					0x1010
#define WND_TEXTLABEL					0x1011
#define WND_IMAGE						0x1012
#define WND_USERCONTROL					0x1040

#define PANEL							0x2000
#define WND_PANEL						0x2001
#define WND_MEM_PANEL					0x2004	

#define WND_MEM							1
#define WND_SCREEN						2
#define WND_CUSTOM_MEM					3	// User have to link user's own allocated memory
											// to created window manually. 
											// added by Joshua on 19-Jan-2006

#define ERASE_BKGND						0x01
#define LEAVE_BKGND						0x02
#define HOLD_FOCUS						0x04
#define HIDE_PANEL						0x10
#define SHOW_PANEL						0x20
#define NO_REDRAW						0x40
#define WND_VISIBLE						0x10
#define WND_INVISIBLE					0x20
#define SYSTEM_PANEL					((WND)1)
#define WND_CARET						((WND)2)
#define WND_SUBLCD						-1

#define SHOW_CARET						1
#define HIDE_CARET						0

    /** COLOR : WHITE
     *
     *	MAKERGB(255, 255, 255)
     */
#define COLOR_WHITE						MAKERGB(255, 255, 255)
    /** COLOR : BLACK
     *
     *	MAKERGB(0, 0, 0)
     */
#define COLOR_BLACK						MAKERGB(0, 0, 0)
    /** COLOR : RED
     *
     *	MAKERGB(255, 0, 0)
     */
#define COLOR_RED						MAKERGB(255, 0, 0)

    //#define COLOR_GREEN						MAKERGB(0, 255, 0)


    /** COLOR : BLUE
     *
     *	MAKERGB(0, 0, 255)
     */
#define COLOR_BLUE						MAKERGB(0, 0, 255)

    /** COLOR : BLUE LIKE
     *
     *  MAKERGB(0, 0, 240)		
     */
#define COLOR_BLUE_FOR_PICT				MAKERGB(0, 0, 240)

    /** COLOR : TRANSPARENT
     *
     *	MAKERGB(255, 0, 255)
     */
#define COLOR_TRANSPARENT				MAKERGB(255, 0, 255)


    /** COLOR_MAGENTA */
#define COLOR_MAGENTA					MAKERGB(220,20,124)

    /** COLOR_GREEN */
#define COLOR_GREEN						MAKERGB(0,255,0)

    /** COLOR_YELLOW */
#define COLOR_YELLOW						MAKERGB(0xFF,0xE4,0x04)

    /** COLOR_ORANGE */
#define COLOR_ORANGE						MAKERGB(255,128,0)

    /** COLOR_CYAN */
#define COLOR_CYAN						MAKERGB(0x00,0xFF,0xFF)

    /** COLOR_LIGHTGREEN */
#define COLOR_LIGHTGREEN				MAKERGB(184,224,20)

    /** COLOR_DARKGREEN */
#define COLOR_DARKGREEN					MAKERGB(120,120,92)

    /** COLOR_DARKGREEN_TEXT */
#define COLOR_DARKGREEN_TEXT			MAKERGB(148,148,148)		// ADDED BY MADHOLIC

    /** COLOR_BACKGROUNDGRAY */
#define COLOR_BACKGROUNDGRAY			MAKERGB(224,224,224)	

    /** COLOR_GRAYFONT */
#define COLOR_GRAYFONT					MAKERGB(148,148,148)

    /** COLOR_GRAY */
#define COLOR_GRAY						MAKERGB(192,192,192)

    /** COLOR_CURSOR */
#define COLOR_CURSOR					MAKERGB(184,224,20)

    /** COLOR_LIGHTBLUE (TextView Active Font Color) */
#define COLOR_LIGHTBLUE					MAKERGB(0,0,230)



#define TITLE_HEIGHT					28

    //============================================================


    // for BPP_16
#define COLOR_INVALID					0xFFFF

#define SCALE							100
#define BLACK_ON_WHITE					1
#define WHITE_ON_BLACK					2
#define COLOR_ON_COLOR					3
#define HALF_TONE						4

#define SRC_COPY						1
#define SRC_AND							2
#define SRC_INVERT						3
#define DST_INVERT						4
#define ALPHA_MAX						255

#define BITMAP_TRANSPARENT				0x00
#define BITMAP_OPAQUE					0x01

#define BITMAP_SIZE_OFFSET				2
#define BITMAP_DATA_OFFSET				54
#define BITMAP_WIDTH_OFFSET				(14 + 4)
#define BITMAP_HEIGHT_OFFSET			(14 + 8)	

#define NORMALFONT						0x00
#define UNDERLINEFONT					0x01
#define BOLDFONT						0x02
#define OUTLINEFONT						0x04

    // AWM Message Define
#define AWM_NONE						0

#define AWM_SYS                         ((unsigned short)0x1000)

    /** Crate Message */
#define AWM_CREATE                      (AWM_SYS + 1)

    /** Destroy Message */
#define AWM_DESTROY                     (AWM_SYS + 2)

    /** Load Message */
#define AWM_LOAD                        (AWM_SYS + 3)

    /** Save Message
     * @if hangule
     * 일반적으로 종료시 메시지로 많이 사용 한다 
     * @endif
     */

#define AWM_SAVE                        (AWM_SYS + 4)
#define AWM_ALARM                       (AWM_SYS + 5)
#define	AWM_SHORTCUT					(AWM_SYS + 6)

    /** Erase Background Message
     * @if hangule
     * AS_InvalidateWindow를 호출하는 경우 메시지가 생성된다 
     * @endif
     */
#define AWM_ERASEBKGND                  (AWM_SYS + 7)
#define AWM_SHOWWINDOW                  (AWM_SYS + 8)

    /** Paint Message
     * @if hangule
     * AS_UpdateWindow 를 호출하는 경우 해당 윈도우에 메시지가 생성된다 
     * @endif
     */
#define AWM_PAINT                       (AWM_SYS + 9)

    /** Set Focus Message
     * @if hangule
     * AS_SetSelection 를 호출하는 경우 해당 윈도우에 메시지가 생성된다 
     * @endif
     */
#define AWM_SETFOCUS                    (AWM_SYS + 10)

    /** Kill Focus Message
     * @if hangule
     * AS_SetSelection 를 호출하는 경우 이전 포커스 윈도우에 메시지가 생성된다 
     * @else
     * 
     * @endif
     */
#define AWM_KILLFOCUS                   (AWM_SYS + 11)

#define AWM_COMMAND                     (AWM_SYS + 12)
#define AWM_CHAR                        (AWM_SYS + 13)
#define AWM_KEYDOWN                     (AWM_SYS + 14)
#define AWM_KEYUP                       (AWM_SYS + 15)
#define AWM_KEYLONG						(AWM_SYS + 16)
#define AWM_KEYREPEAT					(AWM_SYS + 17)

    /** Quit Message
     * @if hangule
     * AS_PostQuitMessage 를 호출하는 경우 메시지가 생성된다 
     * @endif
     */

#define AWM_QUIT                        (AWM_SYS + 18)
#define AWM_TIMER                       (AWM_SYS + 19)	// 4115
#define AWM_DRAWITEM                    (AWM_SYS + 20)
    //for Shutdown State
#define AWM_SHUT_S                      (AWM_SYS + 21)
#define AWM_RETURN						(AWM_SYS + 22)
#define AWM_STATUSBAR					(AWM_SYS + 23)

//#define AWM_WINCREATE					(AWM_SYS + 24)
//#define AWM_WINDESTROY					(AWM_SYS + 25)
#define AWM_CHILDMSG					(AWM_SYS + 26)	// 4122

#define AWM_UPDATE_PICT					(AWM_SYS + 106)
#define AWM_BATTERY_STAT				(AWM_SYS + 116)
#define AWM_CLOSE_POPUP					(AWM_SYS + 117)

#define AWM_UPDATECONTROL				(AWM_SYS + 120)
#define AWM_DISPUPDATE					(AWM_SYS + 121)
#define AWM_SETHILITE					(AWM_SYS + 122)
#define AWM_KILLHILITE					(AWM_SYS + 123)

#define AWM_SYS_LAST_CODE               (0x1500-1)

#define AWM_USER_CONTROL  			    (0x1500)   ///> Nemustech에서만 사용
#define AWM_USER_CONTROL_LAST_CODE		(0x2000-1)

#define AWM_APP							(0x2000)
#define AWM_APP_LAST_CODE               (0x3000-1)


#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_CONST_H__
