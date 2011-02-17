/**************************************************************************
*                                                                         *
*   PROJECT     : ARM port for UCOS-II                                    *
*                                                                         *
*   MODULE      : FRMWRK.c                                                *
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
*   This is the Framework module. Creates an operating infrastructure.    *
*                                                                         *
**************************************************************************/


#include "includes.h"
#include "frmwrk.h"
#include "consol.h"

#include <stdarg.h>
#include <stdio.h>


/* ********************************************************************* */
/* Global definitions */


/* ********************************************************************* */
/* File local definitions */

#define __nSVCStkBase (STK_SADDR+_SVC_STKSIZE)
#define __nSVCStkEnd  STK_SADDR
#define __nSVCStkSize _SVC_STKSIZE

#define __nIRQStkBase (STK_SADDR+_SVC_STKSIZE+_UND_STKSIZE+_ABT_STKSIZE+_IRQ_STKSIZE)
#define __nIRQStkEnd  (STK_SADDR+_SVC_STKSIZE+_UND_STKSIZE+_ABT_STKSIZE)
#define __nIRQStkSize _IRQ_STKSIZE


/* ********************************************************************* */
/* Local functions */

/*
*********************************************************************************************
*                                       __Haltx
*
* Description: These exception handlers are used to report a particular exception on the
*              CONSOL & halt.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

static void __HaltUndef(void)
{
    CONSOL_Printf("Undefined instruction exception!!!\n");
    while(1);
}

static void __HaltSwi(void)
{
    CONSOL_Printf("SWI exception!!!\n");

    while(1);
}

static void __HaltPabort()
{
    CONSOL_Printf("Pabort exception!!!\n");
    while(1);
}

static void __HaltDabort(void)
{
	unsigned long sp_r;
	unsigned long lr_r;
	__asm
	{
		mov sp_r, sp
		mov lr_r, lr
	}
    CONSOL_Printf("Dabort exception !!! %08X, %08X\n", sp_r, lr_r);
    while(1);
}

static void __HaltFiq(void)
{
    CONSOL_Printf("FIQ exception!!!\n");
    while(1);
}

static void __HaltIrq(void)
{
    CONSOL_Printf("IRQ exception!!!\n");
    while(1);
}

/*
*********************************************************************************************
*                                       __vInitHaltHandlers
*
* Description: This routine hooks all the Halt exception handlers. Exceptions which are to
*              be handled by the application can be overwritten with the new handler at the
*              appropriate Software hook location.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

static void __vInitHaltHandlers(void)
{
    pISR_UNDEF = (unsigned)__HaltUndef;
    pISR_SWI   = (unsigned)__HaltSwi;
    pISR_PABORT= (unsigned)__HaltPabort;
    pISR_DABORT= (unsigned)__HaltDabort;
    pISR_FIQ   = (unsigned)__HaltFiq;
    pISR_IRQ = (unsigned)__HaltIrq;
}

/*
*********************************************************************************************
*                                       __vLogo
*
* Description: This routine display the Logo on the CONSOL port.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

static void __vLogo(void)
{
    CONSOL_Printf("\n\nUCOS_FRAMEWORK Ver %s for %s :%s\n",
                  __mVer,__mProcessor,__mDate);
    CONSOL_Printf("Built using %s on %s <%s>\n",
#ifdef ADS
                  "ADS"
#elif SDT
                  "SDT"
#else
                  "Unknown IDE"
#endif
                  ,__DATE__,__TIME__);
    CONSOL_Printf("Endian: %s\n",
#ifdef __BIG_ENDIAN
                  "BIG"
#else
                  "LITTLE"
#endif
                 );
    CONSOL_Printf("CONSOL: COM%d, %dbps, 8Bit, NP\n",__nConsolPort,__nConsolBaud);
    CONSOL_Printf("CPU Clk: %dMHz   MMU: %s   Cache: %s   Write Buf: %s\n",FCLK/1000000,
#ifdef MMU_enabled
                  "ON","ON","ON");
#else
                  "OFF","OFF","OFF");
#endif
    CONSOL_Printf("FLASH_SADDR:%08xh SRAM_SADDR:%08xh SRAM_EADDR:%08xh\nSDRAM_SADDR:\
                  %08xh SFR_BADDR :%08xh ISR_BADDR :%08xh\n",FLASH_SADDR,SRAM_SADDR,SRAM_EADDR,SDRAM_SADDR,
                  SFR_BADDR,ISR_BADDR);
    CONSOL_Printf("Free RAM: %08xh ~ %08xh\n", C_wRTMem, C_wRTMemLimit);
    CONSOL_Printf("Developed by %s\n\n",__mAuthor);
}

/*
*********************************************************************************************
*                                       __vWDTInterrupt
*
* Description: This routine clears the pending WDT interrupt & calls the OS tick handler.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

static void __vWDTInterrupt()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;

    INTBaseP->rSRCPND = BIT_WDT;	// Clear pending bit -- Watchdog timer
    INTBaseP->rINTPND = BIT_WDT;

    OSTimeTick();
}

/*
*********************************************************************************************
*                                       __vPortInit
*
* Description: This routine sets the default GPIO condition.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

static void __vPortInit(void)
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    //CAUTION:Follow the configuration order for setting the ports.
    // 1) setting value(GPnDAT)
    // 2) setting control register  (GPnCON)
    // 3) configure pull-up resistor(GPnUP)

    //32bit data bus configuration
    //*** PORT A GROUP
    //Ports  : GPA22 GPA21  GPA20 GPA19 GPA18 GPA17 GPA16 GPA15 GPA14 GPA13 GPA12
    //Signal : nFCE nRSTOUT nFRE   nFWE  ALE   CLE  nGCS5 nGCS4 nGCS3 nGCS2 nGCS1
    //Binary :  1     1      1  , 1   1   1    1   ,  1     1     1     1
    //Ports  : GPA11   GPA10  GPA9   GPA8   GPA7   GPA6   GPA5   GPA4   GPA3   GPA2   GPA1  GPA0
    //Signal : ADDR26 ADDR25 ADDR24 ADDR23 ADDR22 ADDR21 ADDR20 ADDR19 ADDR18 ADDR17 ADDR16 ADDR0
    //Binary :  1       1      1      1   , 1       1      1      1   ,  1       1     1      1
    IOPBaseP->rGPACON = 0x7fffff;

    //**** PORT B GROUP
    //Ports  : GPB10    GPB9    GPB8    GPB7    GPB6     GPB5    GPB4   GPB3   GPB2     GPB1      GPB0
    //Signal : nXDREQ0 nXDACK0 nXDREQ1 nXDACK1 nSS_KBD nDIS_OFF L3CLOCK L3DATA L3MODE nIrDATXDEN Keyboard
    //Setting: INPUT  OUTPUT   INPUT  OUTPUT   INPUT   OUTPUT   OUTPUT OUTPUT OUTPUT   OUTPUT    OUTPUT
    //Binary :   00  ,  01       00  ,   01      00   ,  01       01  ,   01     01   ,  01        01
    IOPBaseP->rGPBCON = 0x044555;
    IOPBaseP->rGPBUP  = 0x7ff;     // The pull up function is disabled GPB[10:0]

    //*** PORT C GROUP
    //Ports  : GPC15 GPC14 GPC13 GPC12 GPC11 GPC10 GPC9 GPC8  GPC7   GPC6   GPC5 GPC4 GPC3  GPC2  GPC1 GPC0
    //Signal : VD7   VD6   VD5   VD4   VD3   VD2   VD1  VD0 LCDVF2 LCDVF1 LCDVF0 VM VFRAME VLINE VCLK LEND
    //Binary :  10   10  , 10    10  , 10    10  , 10   10  , 10     10  ,  10   10 , 10     10 , 10   10
    IOPBaseP->rGPCCON = 0xaaaaaaaa;
    IOPBaseP->rGPCUP  = 0xffff;     // The pull up function is disabled GPC[15:0]

    //*** PORT D GROUP
    //Ports  : GPD15 GPD14 GPD13 GPD12 GPD11 GPD10 GPD9 GPD8 GPD7 GPD6 GPD5 GPD4 GPD3 GPD2 GPD1 GPD0
    //Signal : VD23  VD22  VD21  VD20  VD19  VD18  VD17 VD16 VD15 VD14 VD13 VD12 VD11 VD10 VD9  VD8
    //Binary : 10    10  , 10    10  , 10    10  , 10   10 , 10   10 , 10   10 , 10   10 ,10   10
    IOPBaseP->rGPDCON = 0xaaaaaaaa;
    IOPBaseP->rGPDUP  = 0xffff;         // The pull up function is disabled GPD[15:0]

    //*** PORT E GROUP
    //Ports  : GPE15  GPE14 GPE13   GPE12   GPE11   GPE10   GPE9    GPE8     GPE7  GPE6  GPE5   GPE4
    //Signal : IICSDA IICSCL SPICLK SPIMOSI SPIMISO SDDATA3 SDDATA2 SDDATA1 SDDATA0 SDCMD SDCLK I2SSDO
    //Binary :  10     10  ,  10      10  ,  10      10   ,  10      10   ,   10    10  , 10     10  ,
    //-------------------------------------------------------------------------------------------------------
    //Ports  :  GPE3   GPE2  GPE1    GPE0
    //Signal : I2SSDI CDCLK I2SSCLK I2SLRCK
    //Binary :  10     10  ,  10      10
    IOPBaseP->rGPECON = 0xaaaaaaaa;
    IOPBaseP->rGPEUP  = 0xffff;     // The pull up function is disabled GPE[15:0]

    //*** PORT F GROUP
    //Ports  : GPF7   GPF6   GPF5   GPF4      GPF3     GPF2  GPF1   GPF0
    //Signal : nLED_8 nLED_4 nLED_2 nLED_1 nIRQ_PCMCIA EINT2 KBDINT EINT0
    //Setting: Output Output Output Output    EINT3    EINT2 EINT1  EINT0
    //Binary :  01      01 ,  01     01  ,     10       10  , 10     10
    IOPBaseP->rGPFCON = 0x55aa;
    IOPBaseP->rGPFUP  = 0xff;     // The pull up function is disabled GPF[7:0]

    //*** PORT G GROUP
    //Ports  : GPG15 GPG14 GPG13 GPG12 GPG11    GPG10    GPG9     GPG8     GPG7      GPG6
    //Signal : nYPON  YMON nXPON XMON  EINT19 DMAMODE1 DMAMODE0 DMASTART KBDSPICLK KBDSPIMOSI
    //Setting: nYPON  YMON nXPON XMON  EINT19  Output   Output   Output   SPICLK1    SPIMOSI1
    //Binary :   11    11 , 11    11  , 10      01    ,   01       01   ,    11         11
    //-----------------------------------------------------------------------------------------
    //Ports  :    GPG5       GPG4    GPG3    GPG2    GPG1    GPG0
    //Signal : KBDSPIMISO LCD_PWREN EINT11 nSS_SPI IRQ_LAN IRQ_PCMCIA
    //Setting:  SPIMISO1  LCD_PWRDN EINT11   nSS0   EINT9    EINT8
    //Binary :     11         11   ,  10      11  ,  10        10
    IOPBaseP->rGPGCON = 0xff95ffba;
    IOPBaseP->rGPGUP  = 0xffff;    // The pull up function is disabled GPG[15:0]

    //*** PORT H GROUP
    //Ports  :  GPH10    GPH9  GPH8 GPH7  GPH6  GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal : CLKOUT1 CLKOUT0 UCLK nCTS1 nRTS1 RXD1 TXD1 RXD0 TXD0 nRTS0 nCTS0
    //Binary :   10   ,  10     10 , 11    11  , 10   10 , 10   10 , 10    10
    IOPBaseP->rGPHCON = 0x2afaaa;
    IOPBaseP->rGPHUP  = 0x7ff;    // The pull up function is disabled GPH[10:0]

    //External interrupt will be falling edge triggered.
    IOPBaseP->rEXTINT0 = 0x22222222;    // EINT[7:0]
    IOPBaseP->rEXTINT1 = 0x22222222;    // EINT[15:8]
    IOPBaseP->rEXTINT2 = 0x22222222;    // EINT[23:16]
}

/*
*********************************************************************************************
*                                       __vChangeMPllValue
*
* Description: This routine sets the MPLL value.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

static void __vChangeMPllValue(int wMdiv, int wPdiv, int wSdiv)
{
    HwrS3C2410CLKRegPtr CLKBaseP = (HwrS3C2410CLKRegPtr)HWRS3C2410_CLK_BASE;

    CLKBaseP->rMPLLCON = (wMdiv<<12) | (wPdiv<<4) | wSdiv;
}

/*
*********************************************************************************************
*                                       __vChangeClockDivider
*
* Description: This routine sets the HCLK and PCLK.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

static void __vChangeClockDivider(int wHdivn, int wPdivn)
{
    HwrS3C2410CLKRegPtr CLKBaseP = (HwrS3C2410CLKRegPtr)HWRS3C2410_CLK_BASE;

    //        wHdivn,wPdivn FCLK:HCLK:PCLK
    //            0,0                1:1:1
    //            0,1                1:1:2
    //            1,0                1:2:2
    //            1,1                1:2:4
    CLKBaseP->rCLKDIV = (wHdivn<<1) | wPdivn;

    if(wHdivn)
        MMU_SetAsyncBusMode();
    else
        MMU_SetFastBusMode();
}


/* ********************************************************************* */
/* Global functions */

/*
*********************************************************************************************
*                                       C_IRQHandler
*
* Description: This routine is the IRQ handler. Handles all peripheral interrupts.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void C_IRQHandler(void)
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;
	void(*func_ptr)(void);

    U32 wTemp;

    wTemp = INTBaseP->rINTOFFSET<<2;

    func_ptr = ((void(*)(void))(*((U32 *)(aISR_EINT0+wTemp))));
	if ( func_ptr ) func_ptr();
}

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

void FRMWRK_vStartTicker(U32 wTicksPerSec)
{
    HwrS3C2410WTCRegPtr WTCBaseP = (HwrS3C2410WTCRegPtr)HWRS3C2410_WTC_BASE;
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;

#if OS_CRITICAL_METHOD == 3                         /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif

    OS_ENTER_CRITICAL();

    WTCBaseP->rWTCON = 0;                                 // Disable watchdog

    INTBaseP->rSRCPND &= BIT_WDT;                         // Clear any pending WD int
    INTBaseP->rINTPND &= BIT_WDT;

    pISR_WDT = (U32)__vWDTInterrupt;

    WTCBaseP->rWTDAT = WTCBaseP->rWTCNT = PCLK / 16 / wTicksPerSec; //set up the WDT for wTicksPerSec

    WTCBaseP->rWTCON = (0<<8)|(1<<5)|(0<<3)|(1<<2)|(0<<0);// Enable: WD, WD interrupts

    BIT_CLR(INTBaseP->rINTMSK, BIT_WDT);                  // Enable WatchDog interrupts

    OS_EXIT_CRITICAL();
}

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

void FRMWRK_vRamTest(U8 *pbID,U32 wSAddr,U32 wEAddr)
{
    U32 wI,wJ;
    U8 bError=0;

    CONSOL_Printf("\n%s(%08xh-%08xh):WR",pbID,wSAddr,wEAddr);
    for(wI=wSAddr;wI<wEAddr;wI+=4)
    {
        *((volatile unsigned *)wI)=wI;
    }
    CONSOL_Printf("\b\bRD");
    for(wI=wSAddr;wI<wEAddr;wI+=4)
    {
        wJ=*((volatile unsigned *)wI);
        if(wJ!=wI)bError=1;
    }

    if(bError==0)CONSOL_Printf("\b\bO.K.\n");
    else CONSOL_Printf("\b\bFAIL\n");
}

void LEDDisplay(unsigned int data)
{
	HwrS3C2410IOPRegPtr ptr;
	ptr = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

	ptr->rGPFDAT = (ptr->rGPFDAT & ~(0xf<<4)) | ((~data & 0xf)<<4);
}
/*
*********************************************************************************************
*                                       C_vMain
*
* Description: This is the main C entry function.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void C_vMain(void)
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;
	unsigned int* ptr;

    __vChangeClockDivider(1,1);            // 1:2:4
    __vChangeMPllValue(84,1,1);            // FCLK=184 MHz

    INTBaseP->rINTMOD     = 0x0;                     //All=IRQ mode
    INTBaseP->rINTMSK     = BIT_ALLMSK;              //All interrupt is masked.
    INTBaseP->rINTSUBMSK  = BIT_SUB_ALLMSK;          //All sub-interrupt is masked. <- April 01, 2002 SOP

	// klenui. clear
	for ( ptr = (unsigned int*)ISR_BADDR ; ptr < (unsigned int*)(ISR_BADDR+0x100) ; ptr++ ) *ptr = 0;

    __vInitHaltHandlers();

    pISR_IRQ = (U32)UCOS_IRQHandler;

    __vPortInit();

    CONSOL_Select(__nConsolPort);
    CONSOL_Init(__nConsolBaud);

    MMU_Init();

    __vLogo();

    APP_vMain();

    CONSOL_Printf("APP has ended...\n");
}

/*
*********************************************************************************************
*                                       printf
*
* Description: This is a stdio.h function.
*
* Arguments  : .
*
* Return     : Returns 0.
*
* Note(s)    : 
*********************************************************************************************
*/

int printf(const char *pbFmt,...)
{
    va_list pArg;
    char abString[1024];

    va_start(pArg,pbFmt);
    vsprintf(abString,pbFmt,pArg);
    CONSOL_SendString(abString);
    va_end(pArg);

    return 0;
}


/* ********************************************************************* */
