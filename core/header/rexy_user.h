/*
 * $Id: rexy_user.h 8115 2005-09-26 06:46:44Z joshua $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	User level에서 사용하는 REXY API prototype이 선언되어 있다.
 *
 */

#ifndef __REXY_USER_H__
#define __REXY_USER_H__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */

    //====================================================================
    //	[[ TimerIDs ]]
    //--------------------------------------------------------------------

    // for window emulator
    
#define RESERVED_TIMER_NUM						0	// for later usage

#define TIMER_ID						        0x01
#define KEY_TIMER_ID					        0x02
#define UPDISP_TIMER_ID					        0x03
#define ENV_TIMER_ID					        0x04
#define FONT_ANIMATION_TIMER_ID                 0x05
#define MISC_TIMER_ID							0x06

    // for low battery
#define BEEP_TIMER_ID					        0x07
#define HAL_TIMER_ID							0x08	//added by Joshua on 9-Jan-2006
#define CARET_TIMER						        0x11

    //#define REXY_UPDISP_TIMER						0x1F

#define REXY_ANIICON_TIMER						0x21
#define REXY_POPUPDLG_TIMER						0x22
#define REXY_LIST_SCROLL_TIMER					0x23
#define REXY_LIST_TIME_SQUARE_SCROLLING_TIMER	0x24

    // Standby 관련 : idle화면과 기타화면에서 standby 들어가는 모드가 다름
    // 삭제
#define REXY_MISC_TIMER							0x25

#define REXY_TABLE_TIMER						0x27
#define REXY_ANIMATE_WALLPAPER_TIMER			0x28
#define REXY_MONITOR_MODE_TIMER					0x29
#define REXY_VOLUME_TIMER						0x2A
#define REXY_LOCKDIALOGUE_CLOSE_TIMER			0x2B	//added by Joshua on 20-Oct-2005

#define REXY_APPLICATION_TIMER					0x30
#define REXY_IDLE_STANDBY_TIMER					0x31

// Control Related Timer ID
#define REXY_CTL_SLIDETEXT_TIMER				0x32
#define REXY_CTL_ANIBMP_TIMER					0x33
#define REXY_CTL_EDIT_TIMER						0x34
#define REXY_CTL_EDIT_CARET_TIMER				0x35

#define REXY_MAINMENU_TIMER						0x36


#define	MAX_TIMER_NUM					        20
#define ALL_TIMER						        -1

    //====================================================================
    //	[[ TimerIntervals ]]
    //====================================================================
#define MISC_INTERVAL	                        250
#define UPDISP_TIMER_ELAPSE				        1000
#define ENV_TIMER_ELAPSE				        10
#define FONT_ANIMATION_TIMER_ELAPSE             700
#define MISC_TIMER_ELAPSE						1000
#define BEEP_TIMER_ELAPSE				        1000
#define BALARM_TIME			                    3000

#define BLINK_TIME						        500
#define TIME_SQUARE_SCROLLING_INTERVAL			100

#define LIST_SCROLL_TIMER_INTERVAL              100
#define TABLE_TIMER_INTERVAL                    100

    /** Creates an long type by concatenating two given short integer values.
     *
     * - a	Low short integer.
     * - b	High short integer.
     * - Returns long value.
     */
#define MAKEPARAM(a, b)		((long) (((unsigned short) (a)) | ((ruint32) ((unsigned short) (b))) << 16)) 
    /**	Creates an unsigned short type by concatenating two given short byte values.
     *
     * @param a		Low byte.
     * @param b		High byte.
     *
     * @return		Returns unsigned short value.
    */
#define MAKEWORD_VALUE(a, b)		((WORD) (((byte) (a)) | ((WORD) ((byte) (b))) << 8)) 

#define LOINT16(x)			((unsigned short) x)
#define HIINT16(x)			(x >> 16)

    /** Get Abs Value Macro */
#ifndef ABS
#define ABS(a)				((a) < 0 ? (a) * -1 : (a))          
#endif

    /** Max Macro */
#define max(a, b)			(((a) > (b)) ? (a) : (b))          

    /** Min Macro */
#define min(a, b)			(((a) < (b)) ? (a) : (b))         

    /** SWAP Macro */
#define SWAP(a,b)			{temp = (a); a = (b); b = temp;}     

    /**	Make appropriate COLOR Data with RGB value for BPP_16.
     *
     * @param r		Red color.
     * @param g		Green color.
     * @param b		Blue color.
     * 
     * @return		Returns COLOR Data. Color Data format is dependent on the LCD.
     */
#define MAKERGB(r, g, b) (COLORVAL)((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))


#define RGBCOMPONENTTODEV(red, green, blue) ((UNITCL)(MAKERGB(red, green, blue)))
#define RGBVALTODEV(rgb) ((UNITCL)(rgb))
#define DEVTORGB(dev) ((COLORVAL)(dev))

    // alpha blending을 위한 RGB component mask
    // for BPP_16
    //#define SRC_COLOR		RGBCOMPONENTTODEV(0xFF, 0xFF, 0xFF)		// white -> moved to rexygdi.c

#define UNITCL_R_MASK	0xf800
#define UNITCL_G_MASK	0x07e0
#define UNITCL_B_MASK	0x001f


     rint32	UsrGetCurrentTime(struct tm * current_time);
     ruint32 	UsrGetCurrentTicks(void);
     void	UsrChangeIdleTimer(rint32 new_period);
     void	UsrInsertTimerMsg(long time_msg);
     void	UsrDisplayCurrentTimerEntry(void);

     void	UsrEnableTimerInternal(rint32 nIDEvent);
     void	UsrDisableTimerInternal(rint32 nIDEvent);

     rint32	UsrSetTimer(WND window, rint32 nIDEvent, rint32 uElapse );
     rint32	UsrKillTimer(WND window, rint32 uIDEvent);

     rint32	UsrSetTimerInternal(WND window, rint32 nIDEvent, rint32 uElapse);
     rint32	UsrKillTimerInternal(WND window, rint32 uIDEvent);

     void	UsrPauseStandbyTimer(void);
     rint32	UsrIsStandbyTimerEnabled(void);
     void	UsrResumeStandbyTimer(void);
     void	UsrHandleKernelTimerMessage(void);	//System use only
     rint32   	UsrRand(void);

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_USER_H__

