/*
 * $Id: 
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */

#ifndef __REXY_HALDEVICE_H__
#define __REXY_HALDEVICE_H__

    //====================================================================
    //	[ MainLCD Display Area ]
    //--------------------------------------------------------------------
#define LCD_WIDTH				480
#define LCD_HEIGHT				272

#define LCD_LEFT				0
#define LCD_TOP					0
#define LCD_RIGHT				(LCD_WIDTH - 1)
#define LCD_BOTTOM				(LCD_HEIGHT - 1)

    //====================================================================
    //	[ MainLCD Display Area Layout ]
    //--------------------------------------------------------------------

    //	PICT Line Area
#define	PICTLINE_HEIGHT			24
#define PICTLINE_LEFT			LCD_LEFT
#define PICTLINE_RIGHT			LCD_RIGHT
#define	PICTLINE_TOP			LCD_TOP
#define	PICTLINE_BOTTOM			(PICTLINE_HEIGHT - 1)

    //	SoftKey Area
#define	SOFTKEY_HEIGHT			28
#define SOFTKEY_WIDTH			240

#define SOFTKEY_LEFT			LCD_LEFT
#define SOFTKEY_RIGHT			LCD_RIGHT
#define	SOFTKEY_TOP				(LCD_HEIGHT - SOFTKEY_HEIGHT)
#define	SOFTKEY_BOTTOM			LCD_BOTTOM

    //	TitleBar Area
#define	TITLEBAR_HEIGHT			28
#define TITLEBAR_LEFT			LCD_LEFT
#define TITLEBAR_RIGHT			LCD_RIGHT
#define	TITLEBAR_TOP			(PICTLINE_BOTTOM + 1)
#define	TITLEBAR_BOTTOM			(TITLEBAR_TOP + TITLEBAR_HEIGHT - 1)
#define TITLEBAR_WIDTH			(TITLEBAR_RIGHT - TITLEBAR_LEFT + 1)

    //	Client Area
#define CLIENT_LEFT				LCD_LEFT
#define CLIENT_RIGHT			LCD_RIGHT
#define CLIENT_TOP				(PICTLINE_BOTTOM + 1)
#define CLIENT_BOTTOM			(SOFTKEY_TOP - 1)

#define CLIENT_WIDTH			(CLIENT_RIGHT - CLIENT_LEFT + 1)
#define CLIENT_HEIGHT			(CLIENT_BOTTOM - CLIENT_TOP + 1)

    //	Client Area without TitleBar
#define CLIENT2_TOP				(TITLEBAR_BOTTOM + 1)
#define CLIENT2_HEIGHT			(CLIENT_BOTTOM - CLIENT2_TOP + 1)


//
// Message
//
#define AWM_SETTIMER					(AWM_APP + 100)
#define AWM_KILLTIMER					(AWM_APP + 101)
#define AWM_KEYDEVICE					(AWM_APP + 102)


//
// Timer
//
    // Application에서 Timer가 필요한 경우 여기에 추가해서 사용하세요

#define REXY_CALCULATOR_TIMER					(REXY_APPLICATION_TIMER+1)		// by pugi29 (2005-04-04)
#define REXY_ALARM_UI_TIMER						(REXY_APPLICATION_TIMER+2)		// by pugi29 (2005-04-12)
#define REXY_ALARM_TIMER						(REXY_APPLICATION_TIMER+3)  	// by pugi29 (2005-04-12)
#define REXY_ANSWER_LIST_TIMER					(REXY_APPLICATION_TIMER+4)		// by pugi29 (2005-04-22)
#define REXY_CALD_ALARM_TIMER					(REXY_APPLICATION_TIMER+5)		// by namgung (2005-05-06)
#define REXY_CALD_ALARM_ANI_TIMER				(REXY_APPLICATION_TIMER+6)		// by namgung (2005-05-06)
#define REXY_ANSWER_PLAY_TIMER					(REXY_APPLICATION_TIMER+7)		// by pugi29 (2005-05-12)
#define REXY_ALARM_DETAIL_TIMER					(REXY_APPLICATION_TIMER+8)		// by pugi29 (2005-05-19)

    /* email timer ID */
#define REXY_EMAIL_NIL_TIMER					(REXY_APPLICATION_TIMER+9)
#define REXY_EMAIL_NBLOCK_TIMER				    (REXY_APPLICATION_TIMER+10)
#define REXY_EMAIL_TIME_COMDLG				    (REXY_APPLICATION_TIMER+11)
#define EMAIL_TIME_COMDLG						(REXY_APPLICATION_TIMER+12)
#define REXY_NEWS_SCROLL_TIMER		 			(REXY_APPLICATION_TIMER+13)

#define REXY_ADDR_PHN_TIMER                     (REXY_APPLICATION_TIMER+14)        // address. phone blinking timer

    // kill timer가 없음
#define REXY_IDLE_WATCH_TIMER					(REXY_APPLICATION_TIMER+16)

#define BALARM_TIMER_ID	                        (REXY_APPLICATION_TIMER+17)

    // DATA FOLDER TIMER ID	 allocated by madholic
#define REXY_DATAFOLDER_TIMER_ID				(REXY_APPLICATION_TIMER+18)
    //3 seconds timer when KeyGuard mode , Joshua 29-Jul-2005
#define REXY_IDLE_KEYGUARD_BLIGHT_TIMER			(REXY_APPLICATION_TIMER+19)

    // Nextreaming, timer 정의. 08-15-2005
    //
#define  MDIA_STIV_GIF_TIMER                    (REXY_APPLICATION_TIMER + 21)
#define  MDIA_VODP_AUDP_IMG_TIMER               (REXY_APPLICATION_TIMER + 22)
#define  MDIA_VODP_AUDP_TEXT_TIMER              (REXY_APPLICATION_TIMER + 23)
#define  MDIA_VODP_CLOSE_TIMER                  (REXY_APPLICATION_TIMER + 24)
#define  MDIA_STIV_CLOSE_TIMER                  (REXY_APPLICATION_TIMER + 25)
#define  MDIA_VODP_RA_TIMER                     (REXY_APPLICATION_TIMER + 26)
#define  MDIA_GUIDE_CLOSE_TIMER                 (REXY_APPLICATION_TIMER + 27)
#define  MDIA_MENU_CLOSE_TIMER                  (REXY_APPLICATION_TIMER + 28)

#define  TIMER_SMAF_ANI_TIMER                   (REXY_APPLICATION_TIMER + 29)
#define  TIMER_SMAF_TEXT_SLIDE                  (REXY_APPLICATION_TIMER + 30)
#define  SMAF_END_TIMER                         (REXY_APPLICATION_TIMER + 31)

    // requested by shkim. linkzen. 05-09-13
#define  BTUI_TIMER                             (REXY_APPLICATION_TIMER + 32)

#define MISSEDNOTI_TIMER						(REXY_APPLICATION_TIMER + 33)

    // BROWSER TIMER
#define BROWSER_TIMER							(REXY_APPLICATION_TIMER + 40)
#define KEY_HOLD_TIMER							(REXY_APPLICATION_TIMER + 41)
#define EZ_WAIT_TIMER							(REXY_APPLICATION_TIMER + 42)
#define EZ_LOCK_TIMER							(REXY_APPLICATION_TIMER + 43)


// TEST TIMER
#define	TEST01_TIMER							(REXY_APPLICATION_TIMER + 100)
#define	TEST02_TIMER							(REXY_APPLICATION_TIMER + 100)
#define	TEST03_TIMER							(REXY_APPLICATION_TIMER + 100)
#define	TEST04_TIMER							(REXY_APPLICATION_TIMER + 100)
#define	TEST05_TIMER							(REXY_APPLICATION_TIMER + 100)


//
// Key Value
//
#ifdef S3C6400

#define VK_PAD_0	0x90			/* '0' key, ASCII '0'                      */
#define VK_PAD_1	0x91			/* '1' key, ASCII '1'                      */
#define VK_PAD_2	0x92			/* '2' key, ASCII '2'                      */
#define VK_PAD_3	0x93			/* '3' key, ASCII '3'                      */
#define VK_PAD_4	0x94			/* '4' key, ASCII '4'                      */
#define VK_PAD_5	0x95			/* '5' key, ASCII '5'                      */
#define VK_PAD_6	0x96			/* '6' key, ASCII '6'                      */
#define VK_PAD_7	0x97			/* '7' key, ASCII '7'                      */
#define VK_PAD_8	0x98			/* '8' key, ASCII '8'                      */
#define VK_PAD_9	0x99			/* '9' key, ASCII '9'                      */
#define VK_PAD_STAR	'*'			/* '*' key, ASCII '*'                      */
#define VK_PAD_POUND	'#'			/* '#' key, ASCII '#'                      */

#define VK_USER_OK	13			/* OK key, ASCII RET                       */
#define VK_USER_CLR	27			/* Clear key, ASCII ESC                    */

#define VK_USER_LEFT		0x39
#define VK_USER_RIGHT		0x3B
#define VK_USER_UP		0x32
#define VK_USER_DOWN		0x3A
#define VK_USER_OK		13

#define VK_USER_BACKSPACE	5
#define VK_USER_DELETE		6
#define VK_USER_TAB			9

#define VK_USER_NEXT		'n'
#define VK_USER_PRV			'p'
#define VK_USER_BW			'b'
#define VK_USER_FW			'f'
#define VK_USER_EUC			'e'
#define VK_USER_UTF8		'u'	

#else

// Normal qwerty keyboard

#define VK_PAD_0	0x0B
#define VK_PAD_1	0x02
#define VK_PAD_2	0x03
#define VK_PAD_3	0x04
#define VK_PAD_4	0x05
#define VK_PAD_5	0x06
#define VK_PAD_6	0x07
#define VK_PAD_7	0x08
#define VK_PAD_8	0x09
#define VK_PAD_9	0x0A
#define VK_PAD_STAR	'*'			/* '*' key, ASCII '*'                      */
#define VK_PAD_POUND	'#'			/* '#' key, ASCII '#'                      */

#define VK_USER_OK	13			/* OK key, ASCII RET                       */
#define VK_USER_CLR	27			/* Clear key, ASCII ESC                    */

#define VK_USER_LEFT		0x69			/* left key                                */
#define VK_USER_RIGHT		0x6A			/* right key                               */
#define VK_USER_UP		0x67			/* up key                                  */
#define VK_USER_DOWN		0x6C			/* down key                                */
#define VK_USER_OK		0x1C

#define VK_USER_BACKSPACE	5
#define VK_USER_DELETE		6
#define VK_USER_TAB			9

#define VK_USER_NEXT		'n'
#define VK_USER_PRV			'p'
#define VK_USER_BW			'b'
#define VK_USER_FW			'f'
#define VK_USER_EUC			'e'
#define VK_USER_UTF8		'u'	

#endif


#endif	//__REXY_HALDEVICE_H__
