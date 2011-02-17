/**************************************************************************
*                                                                         *
*   PROJECT     : ARM port for UCOS-II                                    *
*                                                                         *
*   MODULE      : FRMWRK.h                                                *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : S3c2410x (32 bit ARM920T RISC core from Samsung)        *
*                                                                         *
*   IDE         : SDT 2.51 & ADS 1.2                                      *
*                                                                         *
*   DESCRIPTION :                                                         *
*   External interface & Module configuration header for FRMWRK           *
*   Infrastructure module.                                                *
*                                                                         *
**************************************************************************/


#ifndef __FRMWRK_H__
#define __FRMWRK_H__


#include "def.h"


#ifdef __cplusplus
extern "C" {
#endif


    /* ********************************************************************* */
    /* Module configuration */

#define __mDate "04/01/03"             /* Revision date */
#define __mVer  "1.10"                 /* Revision number */

#define __mAuthor "Michael Anburaj, http://geocities.com/michaelanburaj/"
#define __mProcessor "S3c2410 (ARM920T from Samsung)"

#define __nConsolPort         0        /* Consol UART port number */
#define __nConsolBaud         115200   /* Consol Baud rate */
#define __nConsolFifoEn       True     /* FIFO Enable flag */

#define MMU_enabled                    /* This switch is included with the compiler flags */

#define FCLK (184000000)               /* 184Mhz */
#define HCLK (FCLK/2)
#define PCLK (FCLK/4)
#define UCLK 48000000

    /* note: Reflect the changes in init.s */
#define FLASH_SADDR  0x00000000 /* Flash starting address */
#define SRAM_SADDR   0x40000000 /* SRAM starting address */
#define SRAM_SIZE    (4*1024)   /* 4K internal SRAM */
#define SFR_BADDR    0x48000000 /* SFR base address */

#define SDRAM_SADDR  0x30000000 /* SDRAM starting address */
#define SDRAM_SIZE   (64*1024*1024) /* 64M SDRAM */
#define ISR_BADDR    0x33ffff00 /* ISR vector table start address */

#define SRAM_EADDR   (SRAM_SADDR+SRAM_SIZE-1) /* SRAM end address */
#define SDRAM_EADDR  (SDRAM_SADDR+SDRAM_SIZE-1) /* SRAM end address */

#define MMUTT_SIZE   (16*1024) /* It has to be in multiples of 16K */
#define MMUTT_SADDR  (SDRAM_SADDR) /* MMU Table start address */
#define MMUTT_EADDR  (MMUTT_SADDR+MMUTT_SIZE)

    /* These are from init.s, for stack analysis */
#define _SVC_STKSIZE  1024*20
#define _UND_STKSIZE  256
#define _ABT_STKSIZE  256
#define _IRQ_STKSIZE  1024*1
#define _FIQ_STKSIZE  256

#define STK_SIZE      (_SVC_STKSIZE+_UND_STKSIZE+_ABT_STKSIZE+_IRQ_STKSIZE+_FIQ_STKSIZE)

#define STK_SADDR     (ISR_BADDR-STK_SIZE)
    /* These are from init.s, for stack analysis */


    /* ********************************************************************* */
    /* Interface macro & data definition */

    extern char Image$$ZI$$Limit[];

#define C_wRTMem      ((U32)Image$$ZI$$Limit)
#define C_wRTMemLimit (STK_SADDR-1)

#ifdef FRMWRK_VERBOSE_EN
#define VERBOSE(x) { x }
#else
#define VERBOSE(x) { }
#endif


    /* ********************************************************************* */
    /* Interface function definition */

    /*
    *********************************************************************************************
    *                                       FRMWRK_vRamTest
    *
    * Description: This routine is used do RAM test.
    *
    * Arguments  : pbID   - Test identification string.
    *              wSAddr - RAM starting address.
    *              wEAddr - RAM end address.
    *
    * Return     : none.
    *
    * Note(s)    : Do not test the following areas using this routine:
    *                       1. Stack
    *                       2. Software vector table
    *                       3. C Variables (RW & ZI)
    *********************************************************************************************
    */
    void FRMWRK_vRamTest(U8 *pbID,U32 wSAddr,U32 wEAddr);

    /*
    *********************************************************************************************
    *                                       FRMWRK_vStartTicker
    *
    * Description: This routine starts Timer1 (TC1) in the Prescale mode for OS Tick.
    *
    * Arguments  : wTicksPerSec - Time ticks per second.
    *
    * Return     : none.
    *
    * Note(s)    : 
    *********************************************************************************************
    */
    void FRMWRK_vStartTicker(U32 wTicksPerSec); /* wTicksPerSec = 10...10000 ticks per sec */

    /*
    *********************************************************************************************
    *                                       APP_vMain
    *
    * Description: This is the Application main entry function.
    *
    * Arguments  : none.
    *
    * Return     : none.
    *
    * Note(s)    : This function needs to be implemented in the Application.
    *********************************************************************************************
    */
    void APP_vMain(void);


    /* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /*__FRMWRK_H__*/
