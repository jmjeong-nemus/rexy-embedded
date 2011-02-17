/******************************************************************************
 *
 *
 * File: S3C2410A.h
 *
 * Description:
 *      Register Definition for S3C2410A
 *
 * History:
 *      05/13/2005 Created By Jaewoong Sim
 *   	
 *
 *****************************************************************************/

#ifndef	__S3C2410A_H__
#define	__S3C2410A_H__


/*
-------------------------------------------------

special register

	HWRS3C2410_MEM_BASE		0x48000000
	HWRS3C2410_UHC_BASE		0x49000000
	HWRS3C2410_INT_BASE		0x4a000000
	HWRS3C2410_DMA0_BASE	0x4b000000
	HWRS3C2410_DMA1_BASE	0x4b000040
	HWRS3C2410_DMA2_BASE	0x4b000080
	HWRS3C2410_DMA3_BASE	0x4b0000c0
	HWRS3C2410_CLK_BASE		0x4c000000
	HWRS3C2410_LCD_BASE		0x4d000000
	HWRS3C2410_UART0_BASE	0x50000000
	HWRS3C2410_UART1_BASE	0x50004000
	HWRS3C2410_UART2_BASE	0x50008000
	HWRS3C2410_PWM_BASE		0x51000000
	HWRS3C2410_WTC_BASE		0x53000000
	HWRS3C2410_IIS_BASE		0x55000000
	HWRS3C2410_IOP_BASE		0x56000000
	HWRS3C2410_RTC_BASE		0x57000000
	HWRS3C2410_ADC_BASE		0x58000000
	HWRS3C2410_SPI0_BASE	0x59000000
	HWRS3C2410_SPI1_BASE	0x59000020
	HWRS3C2410_SDI_BASE		0x5a000000

-------------------------------------------------
*/


//
// Register : Memory Controller
//
#define HWRS3C2410_MEM_BASE		0x48000000
struct HwrS3C2410MEMReg {
    INT32U rBWSCON;
    INT32U rBANKCON0;
    INT32U rBANKCON1;
    INT32U rBANKCON2;
    INT32U rBANKCON3;
    INT32U rBANKCON4;
    INT32U rBANKCON5;
    INT32U rBANKCON6;
    INT32U rBANKCON7;
    INT32U rREFRESH;
    INT32U rBANKSIZE;
    INT32U rMRSRB6;
    INT32U rMRSRB7;
};
typedef volatile struct HwrS3C2410MEMReg*	HwrS3C2410MEMRegPtr;


//
// Register : USB Host Controller
//
#define HWRS3C2410_UHC_BASE	0x49000000
struct HwrS3C2410UHCReg {
    INT32U rREVISION;
    INT32U rCONTROL;
    INT32U rCOMMANDSTATUS;
    INT32U rINTERRUPTSTATUS;
    INT32U rINTERRUPTENABLE;
    INT32U rINTERRUPTDISABLE;
    INT32U rHCCA;
    INT32U rPERIODCURRENTED;
    INT32U rCONTROLHEADED;
    INT32U rCONTROLCURRENTED;
    INT32U rBULKHEADED;
    INT32U rBULKCURRENTED;
    INT32U rDONEHEAD;
    INT32U rFMINTERVAL;
    INT32U rFMREMAINING;
    INT32U rFMNUMBER;
    INT32U rPERIODICSTART;
    INT32U rLSTHRESHOLD;
    INT32U rRHDESCRIPTORA;
    INT32U rRHDESCRIPTORB;
    INT32U rRHSTATUS;
    INT32U rRHPORTSTATUS1;
    INT32U rRHPORTSTATUS2;
};
typedef volatile struct HwrS3C2410UHCReg *HwrS3C2410UHCRegPtr;


//
// Registers : Interrupt Controller
//
#define HWRS3C2410_INT_BASE      0x4a000000	// 0x4a000000
struct HwrS3C2410INTReg{
    INT32U rSRCPND;
    INT32U rINTMOD;
    INT32U rINTMSK;
    INT32U rPRIORITY;
    INT32U rINTPND;
    INT32U rINTOFFSET;
    INT32U rSUBSRCPND;
    INT32U rINTSUBMSK;
};
typedef volatile struct HwrS3C2410INTReg*	HwrS3C2410INTRegPtr;

#define hwr2410IntStatusKeyboard			0x00000001


//
// Register : DMA
//
#define HWRS3C2410_DMA0_BASE	0x4B000000
#define HWRS3C2410_DMA1_BASE	0x4B000040
#define HWRS3C2410_DMA2_BASE	0x4B000080
#define HWRS3C2410_DMA3_BASE	0x4B0000C0
struct HwrS3C2410DMAReg {
    INT32U DISRC;
    INT32U DISRCC;
    INT32U DIDST;
    INT32U DIDSTC;
    INT32U DCON;
    INT32U DSTAT;
    INT32U DCSRC;
    INT32U DCDST;
    INT32U DMASKTRIG;
};
typedef volatile struct HwrS3C2410DMAReg* HwrS3C2410DMARegPtr;

// DMA Initial Source Control Register
#define hwrS3C2410DMASourceAPB			0x00000002
#define hwrS3C2410DMAAddrFixed			0x00000001

// DMA Initial Destination Control Register
#define hwrS3C2410DMADestAPB			0x00000002
#define hwrS3C2410DMAAddrFixed			0x00000001

// DMA Control Register
#define hwrS3C2410DMACtlHandShake		0x80000000
#define hwrS3C2410DMACtlSyncHClk		0x40000000
#define hwrS3C2410DMACtlTCIntEnable		0x20000000
#define hwrS3C2410DMACtlBurstTrans		0x10000000
#define hwrS3C2410DMACtlWholeService	0x08000000
#define hwrS3C2410DMACtlHardwareMode	0x00800000
#define hwrS3C2410DMACtlReloadOff		0x00400000
#define hwrS3C2410DMACtlDataWord		0x00200000
#define hwrS3C2410DMACtlDataHalfWord	0x00100000
#define hwrS3C2410DMACtlDataByte		0x00000000

// DMA Status Register
#define hwrS3C2410DMAStatusBusy			0x00100000

// DMA Mask Trigger Register
#define hwrS3C2410DMATrgStop			0x00000004
#define hwrS3C2410DMATrgChannelOn		0x00000002
#define hwrS3C2410DMATrgSoftTrgReq		0x00000001


//
// Register : Clock
//
#define HWRS3C2410_CLK_BASE		0x4c000000
struct HwrS3C2410CLKReg {
    INT32U rLOCKTIME;
    INT32U rMPLLCON;
    INT32U rUPLLCON;
    INT32U rCLKCON;
    INT32U rCLKSLOW;
    INT32U rCLKDIV;
};
typedef volatile struct HwrS3C2410CLKReg*		HwrS3C2410CLKRegPtr;

// Clock Control Register (CLKCON)
#define hwrS3C2410ClkCtlSPIEanble		0x00040000
#define hwrS3C2410ClkCtlIISEnable		0x00020000
#define hwrS3C2410ClkCtlIICEnable		0x00010000
#define hwrS3C2410ClkCtlADCEnable		0x00008000
#define hwrS3C2410ClkCtlRTCEnable		0x00004000
#define hwrS3C2410ClkCtlGPIOEnable		0x00002000
#define hwrS3C2410ClkCtlUART2Enable		0x00001000
#define hwrS3C2410ClkCtlUART1Enable		0x00000800
#define hwrS3C2410ClkCtlUART0Enable		0x00000400
#define hwrS3C2410ClkCtlSDIEnable		0x00000200
#define hwrS3C2410ClkCtlPWMTimerEnable	0x00000100
#define hwrS3C2410ClkCtlUSBDevEnable	0x00000080
#define hwrS3C2410ClkCtlUSBHostEnable	0x00000040
#define hwrS3C2410ClkCtlLCDCEnable		0x00000020
#define hwrS3C2410ClkCtlNANDEanble		0x00000010
#define hwrS3C2410ClkCtlPowerOffMode	0x00000008
#define hwrS3C2410ClkCtlIdleMode		0x00000004
#define hwrS3C2410ClkCtlSpecialMode		0x00000001

// Clock Slow Control Register (CLKSLOW)
#define hwrS3C2410ClkSlowCtlUClkOff		0x00000080
#define hwrS3C2410ClkSlowCtlMPLLOff		0x00000020
#define hwrS3C2410ClkSlowCtlSlowModeEn	0x00000010

// Clock Divider Control Register (CLKDIVN)
#define hwrS3C2410ClkDivCtlSpecial		0x00000004
#define hwrS3C2410ClkDivCtlHDiv			0x00000002
#define hwrS3C2410ClkDivCtlPDiv			0x00000001


//
// Registers : LCD Controller
//
#define HWRS3C2410_LCD_BASE      0x4d000000
struct HwrS3C2410LCDReg  {
    INT32U rLCDCON1;		// 00
    INT32U rLCDCON2;		// 04
    INT32U rLCDCON3;		// 08
    INT32U rLCDCON4;		// 0C
    INT32U rLCDCON5;		// 10
    INT32U rLCDSADDR1;		// 14
    INT32U rLCDSADDR2;		// 18
    INT32U rLCDSADDR3;		// 1C
    INT32U rREDLUT;			// 20
    INT32U rGREENLUT;		// 24
    INT32U rBLUELUT;		// 28
    INT32U rPAD[8];			// 2C - 48
    INT32U rDITHMODE;		// 4C
    INT32U rTPAL;			// 50
    INT32U rLCDINTPND;		// 54
    INT32U rLCDSRCPND;		// 58
    INT32U rLCDINTMSK;		// 5C
    INT32U rLPCSEL;			// 60
};
typedef volatile struct HwrS3C2410LCDReg*	HwrS3C2410LCDRegPtr;

#define M5D(n)              ((n) & 0x1fffff)


//
// Registers : NAND Flash Controller
//
/*
// kekim_20050219, Porting SPK540
#define HWRS3C2410_NFC_BASE		0x4e000000
struct HwrS3C2410NFCReg {
	INT32U rNFCONF;
	INT32U rNFCMD;
	INT32U rNFADDR;
	INT32U rNFDATA;
	INT32U rNFSTAT;
	INT32U rNFECC;
};
typedef volatile struct HwrS3C2410NFCReg *HwrS3C2410NFCRegPtr;
*/


//
// Registers : UART
//
#define HWRS3C2410_UART0_BASE		0x50000000
#define HWRS3C2410_UART1_BASE		0x50004000
#define HWRS3C2410_UART2_BASE		0x50008000
struct HwrS3C2410UARTReg {
    INT32U  rULCON;
    INT32U  rUCON;
    INT32U  rUFCON;
    INT32U  rUMCON;
    INT32U  rUTRSTAT;
    INT32U  rUERSTAT;
    INT32U  rUFSTAT;
    INT32U  rUMSTAT;
    INT32U  rUTXH;
    INT32U  rURXH;
    INT32U  rUBRDIV;
};
typedef volatile struct HwrS3C2410UARTReg*	HwrS3C2410UARTRegPtr;

// UART Line Control Register
#define hwrS3C2410UARTIrMode		0x00000040
#define hwrS3C2410UARTParity0		0x00000038
#define hwrS3C2410UARTParity1		0x00000030
#define hwrS3C2410UARTParityEven	0x00000028
#define hwrS3C2410UARTParityOdd		0x00000020
#define hwrS3C2410UART2StopBit		0x00000004
#define hwrS3C2410UART8Bits			0x00000003
#define hwrS3C2410UART7Bits			0x00000002
#define hwrS3C2410UART6Bits			0x00000001
#define hwrS3C2410UART5Bits			0x00000000

// UART Control Register
#define hwrS3C2410UARTClkUCLK			0x00000400
#define hwrS3C2410UARTTxLvlInt			0x00000200
#define hwrS3C2410UARTRxLvlInt			0x00000100
#define hwrS3C2410UARTRxTimeOutEnable	0x00000080
#define hwrS3C2410UARTRxErrIntEnable	0x00000040
#define hwrS3C2410UARTLoopbackMode		0x00000020
#define hwrS3C2410UARTSendBreakSignal	0x00000010
#define hwrS3C2410UARTTxModeDMA1UART1	0x0000000C
#define hwrS3C2410UARTTxModeDMA03UART02 0x00000008
#define hwrS3C2410UARTTxModeIntPoll		0x00000004
#define hwrS3C2410UARTRxModeDMA1UART1	0x00000003
#define hwrS3C2410UARTRxModeDMA03UART02	0x00000002
#define hwrS3C2410UARTRxModeIntPoll		0x00000001

// UART FIFO Control Register
#define hwrS3C2410UARTTxFIFOTrg12Byte	0x000000C0
#define hwrS3C2410UARTTxFIFOTrg8Byte	0x00000080
#define hwrS3C2410UARTTxFIFOTrg4Byte	0x00000040
#define hwrS3C2410UARTRxFIFOTrg16Byte	0x00000030
#define hwrS3C2410UARTRxFIFOTrg12Byte	0x00000020
#define hwrS3C2410UARTRxFIFOTrg8Byte	0x00000010
#define hwrS3C2410UARTTxFIFOReset		0x00000004
#define hwrS3C2410UARTRxFIFOReset		0x00000002
#define hwrS3C2410UARTFIFOEnable		0x00000001

// UART Modem Control Register
#define hwrS3C2410UARTAFCEnalbe			0x00000010
#define hwrS3C2410UARTReqSend			0x00000001

// UART Tx/Rx Status Register
#define hwrS3C2410UARTTransmtEmpty		0x00000004
#define hwrS3C2410UARTTransBufEmpty		0x00000002
#define hwrS3C2410UARTRxBufReady		0x00000001

// UART Error Status Register
#define hwrS3C2410UARTBreakDetect		0x00000008
#define hwrS3C2410UARTFrameError		0x00000004
#define hwrS3C2410UARTParityError		0x00000002
#define hwrS3C2410UARTOverrunError		0x00000001

// UART FIFO Status Register
#define hwrS3C2410UARTTxFIFOFull		0x00000200
#define hwrS3C2410UARTRxFIFOFull		0x00000100

// UART Modem Status Register
#define hwrS3C2410UARTDeltaCTS			0x00000004
#define hwrS3C2410UARTClearSend			0x00000001


#define UART0BaudRate		115200
#define UART1BaudRate		115200

#define hwr2410UERSTATBreakDetect			0x00000008
#define hwr2410UERSTATFrameError			0x00000004
#define hwr2410UERSTATParityError			0x00000002
#define hwr2410UERSTATOverrunError			0x00000001

#if 0
#define FCLK 				202800000
#define HCLKDIV				2
#define PCLKDIV				4
#define HCLK				(FCLK/HCLKDIV)
#define PCLK				(FCLK/PCLKDIV)
#define UCLK				PCLK
#endif

#define TIMER_PRESCALER0	25
#define TIMER_PRESCALER1	25

#define DLY_TIMER_DIVIDER	2
/* tick for 1000ms = 1second */
#define DLY_CLOCK_FREQ	(PCLK / (TIMER_PRESCALER0+1) / DLY_TIMER_DIVIDER)	/* DLY_TIMER_DIVIDER == 2 : 1014000 at 203 Mhz */
// Timer count for 1 ms
#define DLY_COUNT_1MS	(DLY_CLOCK_FREQ / 1000)		/* DLY_TIMER_DIVIDER == 2 : about  1014 at 203 Mhz */

//
// Registers : PWM
//
#define HWRS3C2410_PWM_BASE      0x51000000
struct HwrS3C2410PWMReg  {
    INT32U rTCFG0;
    INT32U rTCFG1;
    INT32U rTCON;
    INT32U rTCNTB0;
    INT32U rTCMPB0;
    INT32U rTCNTO0;
    INT32U rTCNTB1;
    INT32U rTCMPB1;
    INT32U rTCNTO1;
    INT32U rTCNTB2;
    INT32U rTCMPB2;
    INT32U rTCNTO2;
    INT32U rTCNTB3;
    INT32U rTCMPB3;
    INT32U rTCNTO3;
    INT32U rTCNTB4;
    INT32U rTCNTO4;
};
typedef volatile struct HwrS3C2410PWMReg*	HwrS3C2410PWMRegPtr;

// Timer Configuration Register 1 (TCFG1)
#define hwrS3C2410PWMDMANoSelect		0x00000000	
#define hwrS3C2410PWMDMATimer0			0x00100000
#define hwrS3C2410PWMDMATimer1			0x00200000
#define hwrS3C2410PWMDMATimer2			0x00300000
#define hwrS3C2410PWMDMATimer3			0x00400000
#define hwrS3C2410PWMDMATimer4			0x00500000

#define hwrS3C2410PWMTimer4ExtCLK1		0x00040000
#define hwrS3C2410PWMTimer4DivBy16		0x00030000
#define hwrS3C2410PWMTimer4DivBy8		0x00020000
#define hwrS3C2410PWMTimer4DivBy4		0x00010000
#define hwrS3C2410PWMTimer4DivBy2		0x00000000

#define hwrS3C2410PWMTimer3ExtCLK1		0x00004000
#define hwrS3C2410PWMTimer3DivBy16		0x00003000
#define hwrS3C2410PWMTimer3DivBy8		0x00002000
#define hwrS3C2410PWMTimer3DivBy4		0x00001000
#define hwrS3C2410PWMTimer3DivBy2		0x00000000

#define hwrS3C2410PWMTimer2ExtCLK1		0x00000400
#define hwrS3C2410PWMTimer2DivBy16		0x00000300
#define hwrS3C2410PWMTimer2DivBy8		0x00000200
#define hwrS3C2410PWMTimer2DivBy4		0x00000100
#define hwrS3C2410PWMTimer2DivBy2		0x00000000

#define hwrS3C2410PWMTimer1ExtCLK0		0x00000040
#define hwrS3C2410PWMTimer1DivBy16		0x00000030
#define hwrS3C2410PWMTimer1DivBy8		0x00000020
#define hwrS3C2410PWMTimer1DivBy4		0x00000010
#define hwrS3C2410PWMTimer1DivBy2		0x00000000

#define hwrS3C2410PWMTimer0ExtCLK0		0x00000004
#define hwrS3C2410PWMTimer0DivBy16		0x00000003
#define hwrS3C2410PWMTimer0DivBy8		0x00000002
#define hwrS3C2410PWMTimer0DivBy4		0x00000001
#define hwrS3C2410PWMTimer0DivBy2		0x00000000

// Timer Control Register (TCON)
#define hwrS3C2410Timer4AutoReload		0x00400000
#define hwrS3C2410Timer4Update			0x00200000
#define hwrS3C2410Timer4Start			0x00100000

#define hwrS3C2410Timer3AutoReload		0x00080000
#define hwrS3C2410Timer3Invert			0x00040000
#define hwrS3C2410Timer3Update			0x00020000
#define hwrS3C2410Timer3Start			0x00010000

#define hwrS3C2410Timer2AutoReload		0x00008000
#define hwrS3C2410Timer2Invert			0x00004000
#define hwrS3C2410Timer2Update			0x00002000
#define hwrS3C2410Timer2Start			0x00001000

#define hwrS3C2410Timer1AutoReload		0x00000800
#define hwrS3C2410Timer1Invert			0x00000400
#define hwrS3C2410Timer1Update			0x00000200
#define hwrS3C2410Timer1Start			0x00000100

#define hwrS3C2410DeadZoneEnable		0x00000010
#define hwrS3C2410Timer0AutoReload		0x00000008
#define hwrS3C2410Timer0Invert			0x00000004
#define hwrS3C2410Timer0Update			0x00000002
#define hwrS3C2410Timer0Start			0x00000001


//
// Register : Watch Dog Timer
//
#define HWRS3C2410_WTC_BASE		0x53000000
struct HwrS3C2410WTCReg {
    INT32U rWTCON;
    INT32U rWTDAT;
    INT32U rWTCNT;
};
typedef volatile struct HwrS3C2410WTCReg* HwrS3C2410WTCRegPtr;

//
// Register : IIS-Bus Interface
//
// Little Endian / Word
#define HWRS3C2410_IIS_BASE     0x55000000
struct HwrS3C2410IISReg {
    unsigned short int    IISCON;
    unsigned short int	  rPAD1;
    unsigned short int    IISMOD;
    unsigned short int	  rPAD2;
    unsigned short int    IISPSR;
    unsigned short int	  rPAD3;
    unsigned short int    IISFCON;
    unsigned short int	  rPAD4;
    unsigned short int    IISFIFO;
};
typedef volatile struct HwrS3C2410IISReg* HwrS3C2410IISRegPtr;

#define kFramesPerBuffer 				64
struct HwrS3C2410DMABuffer {
    INT16U samples[kFramesPerBuffer * 2];
};

// by micro
//#define HWRS3C2410_PLAYBACK_BUF_BASE	(0x33f00000 + ((1024*1024) - kFramesPerBuffer*8*sizeof(INT16U) - 0x3000))
//#define HWRS3C2410_RECORD_BUF_BASE		(HWRS3C2410_PLAYBACK_BUF_BASE + kFramesPerBuffer*4*sizeof(INT16U))
typedef struct HwrS3C2410DMABuffer* HwrS3C2410DMABufferPtr;


//
// Registers : I/O port
//
#define HWRS3C2410_IOP_BASE      0x56000000
struct HwrS3C2410IOPReg {
    INT32U  rGPACON;		// 00
    INT32U  rGPADAT;
    INT32U  rPAD1[2];

    INT32U  rGPBCON;		// 10
    INT32U  rGPBDAT;
    INT32U  rGPBUP;
    INT32U  rPAD2;

    INT32U  rGPCCON;		// 20
    INT32U  rGPCDAT;
    INT32U  rGPCUP;
    INT32U  rPAD3;

    INT32U  rGPDCON;		// 30
    INT32U  rGPDDAT;
    INT32U  rGPDUP;
    INT32U  rPAD4;

    INT32U  rGPECON;		// 40
    INT32U  rGPEDAT;
    INT32U  rGPEUP;
    INT32U  rPAD5;

    INT32U  rGPFCON;		// 50
    INT32U  rGPFDAT;
    INT32U  rGPFUP;
    INT32U  rPAD6;

    INT32U  rGPGCON;		// 60
    INT32U  rGPGDAT;
    INT32U  rGPGUP;
    INT32U  rPAD7;

    INT32U  rGPHCON;		// 70
    INT32U  rGPHDAT;
    INT32U  rGPHUP;
    INT32U  rPAD8;

    INT32U  rMISCCR;		// 80
    INT32U  rDCLKCON;
    INT32U  rEXTINT0;
    INT32U  rEXTINT1;
    INT32U  rEXTINT2;		// 90
    INT32U  rEINTFLT0;
    INT32U  rEINTFLT1;
    INT32U  rEINTFLT2;
    INT32U  rEINTFLT3;		// A0
    INT32U  rEINTMASK;
    INT32U  rEINTPEND;
    INT32U  rGSTATUS0;
    INT32U  rGSTATUS1;		// B0
    INT32U  rGSTATUS2;
    INT32U  rGSTATUS3;
    INT32U  rGSTATUS4;
};
typedef volatile struct HwrS3C2410IOPReg*	HwrS3C2410IOPRegPtr;

// jangth-20050128 - changing coding convension
// MISCELLANEOUS Control Register (MISCCR)
#define hwrS3C2410IOPMISCCRCtlEnSCKE	0x00080000
#define hwrS3C2410IOPMISCCRCtlEnSCLK1	0x00040000
#define hwrS3C2410IOPMISCCRCtlEnSCLK0	0x00020000
#define hwrS3C2410IOPMISCCRCtlRSTCON	0x00010000

#define hwrS3C2410IOPMISCCRCtlUSBSUSPND1	0x00002000
#define hwrS3C2410IOPMISCCRCtlUSBSUSPND0	0x00001000

#define hwrS3C2410IOPMISCCRCtlCLKSEL1	0x00000700
#define hwrS3C2410IOPMISCCRCtlCLKSEL0	0x00000070

#define hwrS3C2410IOPMISCCRCtlUSBPAD	0x00000008
#define hwrS3C2410IOPMISCCRCtlSPUCR1	0x00000002
#define hwrS3C2410IOPMISCCRCtlSPUCR0	0x00000001

// DCLK Control Register (DCLDCON)
#define hwrS3C2410IOPDCLDCONDCLD1CMP	0x0F000000
#define hwrS3C2410IOPDCLDCONDCLK1DIV	0x00F00000
#define hwrS3C2410IOPDCLDCONDCLD1SelCK	0x00020000
#define hwrS3C2410IOPDCLDCONDCLD1En		0x00010000
#define hwrS3C2410IOPDCLDCONDCLD0CMP	0x00000F00
#define hwrS3C2410IOPDCLDCONDCLD0DIV	0x000000F0
#define hwrS3C2410IOPDCLDCONDCLD0SelCK	0x00000002
#define hwrS3C2410IOPDCLDCONDCLD0En		0x00000001

// General Status Register (GSTATUSn)
// GSTATUS0
#define hwrS3C2410IOPGSTATUS0nWait		0x8
#define hwrS3C2410IOPGSTATUS0NCON1		0x4
#define hwrS3C2410IOPGSTATUS0NCON0		0x2
#define hwrS3C2410IOPGSTATUS0nBATT_FLT	0x1

//
// Register : RTC
//
#define HWRS3C2410_RTC_BASE			0x57000000
struct HwrS3C2410RTCReg{
    INT32U rPAD0[16]; 	// 00 - 3C
    INT8U rRTCCON;		// 40
    INT8U rPAD1[3];
    INT8U rTICINT;
    INT8U rPAD2[3];
    INT32U rPAD3[2];
    INT8U rRTCALM;
    INT8U rPAD4[3];
    INT8U rALMSEC;
    INT8U rPAD5[3];
    INT8U rALMMIN;
    INT8U rPAD6[3];
    INT8U rALMHOUR;
    INT8U rPAD7[3];
    INT8U rALMDAY;
    INT8U rPAD8[3];
    INT8U rALMMON;
    INT8U rPAD9[3];
    INT8U rALMYEAR;
    INT8U rPAD10[3];
    INT8U rRTCRST;
    INT8U rPAD11[3];
    INT8U rBCDSEC;
    INT8U rPAD12[3];
    INT8U rBCDMIN;
    INT8U rPAD13[3];
    INT8U rBCDHOUR;
    INT8U rPAD14[3];
    INT8U rBCDDAY;
    INT8U rPAD15[3];
    INT8U rBCDDATE;
    INT8U rPAD16[3];
    INT8U rBCDMON;
    INT8U rPAD17[3];
    INT8U rBCDYEAR;
};
typedef volatile struct HwrS3C2410RTCReg*	HwrS3C2410RTCRegPtr;

// RTC Control Register (INT8U)
#define hwrS3C2410RTCCtlClkReset		0x08
#define hwrS3C2410RTCCtlCntSel			0x04
#define hwrS3C2410RTCCtlClkSel			0x02
#define hwrS3C2410RTCCtlEnable			0x01

// RTC Tick Time Count Register
#define hwrS3C2410RTCTckIntEnable		0x80

// RTC Alarm Control Register
#define hwrS3C2410RTCAlmEnable			0x40
#define hwrS3C2410RTCYearEnable			0x20
#define hwrS3C2410RTCMonEnable			0x10
#define hwrS3C2410RTCDayEnable			0x08
#define hwrS3C2410RTCHourEnable			0x04
#define hwrS3C2410RTCMinEnable			0x02
#define hwrS3C2410RTCSecEnable			0x01

// RTC Round Reset Register
#define hwrS3C2410RTCRndSecResetEnable	0x08
#define hwrS3C2410RTCRndSecCarrGen50s	0x05
#define hwrS3C2410RTCRndSecCarrGen40s	0x04
#define hwrS3C2410RTCRndSecCarrGen30s	0x03


//
// Register : ADC
//
#define HWRS3C2410_ADC_BASE			0x58000000
struct HwrS3C2410ADCReg {
    INT32U rADCCON;
    INT32U rADCTSC;
    INT32U rADCDLY;
    INT32U rADCDAT0;
    INT32U rADCDAT1;
};
typedef volatile struct HwrS3C2410ADCReg*	HwrS3C2410ADCRegPtr;

// ADC : ADC Control Register
#define hwrS3C2410ADCCtlECFlag		0x00008000
#define hwrS3C2410ADCCtlPrscEnable	0x00004000
#define hwrS3C2410ADCCtlAIN0		0x00000000	
#define hwrS3C2410ADCCtlAIN1		0x00000008
#define hwrS3C2410ADCCtlAIN2		0x00000010
#define hwrS3C2410ADCCtlAIN3		0x00000018
#define hwrS3C2410ADCCtlAIN4		0x00000020
#define hwrS3C2410ADCCtlAIN5		0x00000028
#define hwrS3C2410ADCCtlAIN6		0x00000030
#define hwrS3C2410ADCCtlAIN7		0x00000038
#define hwrS3C2410ADCCtlStdbyMode	0x00000004
#define hwrS3C2410ADCCtlReadEnable	0x00000002
#define hwrS3C2410ADCCtlADCEnable	0x00000001

// ADC : TSP Control Register
#define hwrS3C2410TSPCtlYMEnable	0x00000080
#define hwrS3C2410TSPCtlYPEnable	0x00000040
#define hwrS3C2410TSPCtlXMEnable	0x00000020
#define hwrS3C2410TSPCtlXPEnable	0x00000010
#define hwrS3C2410TSPCtlPullUp		0x00000008
#define hwrS3C2410TSPCtlAutoPST		0x00000004
#define hwrS3C2410TSPCtlWaitInt		0x00000003
#define hwrS3C2410TSPCtlYPPST		0x00000002
#define hwrS3C2410TSPCtlXPPST		0x00000001
#define hwrS3C2410TSPCtlNoOP		0x00000000

// ADC : ADC Conversion Data Register
#define hwrS3C2410DatStylusUp		0x00008000
#define hwrS3C2410DatAutoPST		0x00004000
#define hwrS3C2410DatWaitInt		0x00003000
#define hwrS3C2410DatYPPST			0x00002000
#define hwrS3C2410DatXPPST			0x00001000
#define hwrS3C2410DatNoOp			0x00000000


//
// Reigster : SPI Interface
//
#define HWRS3C2410_SPI0_BASE	0x59000000
#define HWRS3C2410_SPI1_BASE	0x59000020
struct HwrS3C2410SPIReg {
    INT32U rSPCON;
    INT32U rSPSTA;
    INT32U rSPPIN;
    INT32U rSPPRE;
    INT32U rSPTDAT;
    INT32U rSPRDAT;
};
typedef volatile struct HwrS3C2410SPIReg* HwrS3C2410SPIRegPtr;


//
// Register : Secure Digital Interface for SDIO
//
#define HWRS3C2410_SDI_BASE	0x5a000000
struct HwrS3C2410SDIReg {
    INT32U rSDICON;
    INT32U rSDIPRE;
    INT32U rSDICARG;
    INT32U rSDICCON;
    INT32U rSDICSTA;
    INT32U rSDIRSP0;
    INT32U rSDIRSP1;
    INT32U rSDIRSP2;
    INT32U rSDIRSP3;
    INT32U rSDIDTIMER;
    INT32U rSDIBSIZE;
    INT32U rSDIDCON;
    INT32U rSDIDCNT;
    INT32U rSDIDSTA;
    INT32U rSDIFSTA;
    INT32U rSDIDAT;
    INT32U rSDIIMSK;
};
typedef volatile struct HwrS3C2410SDIReg* HwrS3C2410SDIRegPtr;

// ISR
#define pISR_RESET     (*(unsigned *)(ISR_BADDR+0x0))
#define pISR_UNDEF     (*(unsigned *)(ISR_BADDR+0x4))
#define pISR_SWI       (*(unsigned *)(ISR_BADDR+0x8))
#define pISR_PABORT    (*(unsigned *)(ISR_BADDR+0xc))
#define pISR_DABORT    (*(unsigned *)(ISR_BADDR+0x10))
#define pISR_RESERVED  (*(unsigned *)(ISR_BADDR+0x14))
#define pISR_IRQ       (*(unsigned *)(ISR_BADDR+0x18))
#define pISR_FIQ       (*(unsigned *)(ISR_BADDR+0x1c))

#define aISR_EINT0     (ISR_BADDR+0x20)
#define pISR_EINT0     (*(unsigned *)aISR_EINT0)
#define pISR_EINT1     (*(unsigned *)(ISR_BADDR+0x24))
#define pISR_EINT2     (*(unsigned *)(ISR_BADDR+0x28))
#define pISR_EINT3     (*(unsigned *)(ISR_BADDR+0x2c))
#define pISR_EINT4_7   (*(unsigned *)(ISR_BADDR+0x30))
#define pISR_EINT8_23  (*(unsigned *)(ISR_BADDR+0x34))
#define pISR_NOTUSED6  (*(unsigned *)(ISR_BADDR+0x38))
#define pISR_BAT_FLT   (*(unsigned *)(ISR_BADDR+0x3c))
#define pISR_TICK      (*(unsigned *)(ISR_BADDR+0x40))
#define pISR_WDT       (*(unsigned *)(ISR_BADDR+0x44))
#define pISR_TIMER0    (*(unsigned *)(ISR_BADDR+0x48))
#define pISR_TIMER1    (*(unsigned *)(ISR_BADDR+0x4c))
#define pISR_TIMER2    (*(unsigned *)(ISR_BADDR+0x50))
#define pISR_TIMER3    (*(unsigned *)(ISR_BADDR+0x54))
#define pISR_TIMER4    (*(unsigned *)(ISR_BADDR+0x58))
#define pISR_UART2     (*(unsigned *)(ISR_BADDR+0x5c))
#define pISR_LCD       (*(unsigned *)(ISR_BADDR+0x60))
#define pISR_DMA0      (*(unsigned *)(ISR_BADDR+0x64))
#define pISR_DMA1      (*(unsigned *)(ISR_BADDR+0x68))
#define pISR_DMA2      (*(unsigned *)(ISR_BADDR+0x6c))
#define pISR_DMA3      (*(unsigned *)(ISR_BADDR+0x70))
#define pISR_SDI       (*(unsigned *)(ISR_BADDR+0x74))
#define pISR_SPI0      (*(unsigned *)(ISR_BADDR+0x78))
#define pISR_UART1     (*(unsigned *)(ISR_BADDR+0x7c))
#define pISR_NOTUSED24 (*(unsigned *)(ISR_BADDR+0x80))
#define pISR_USBD      (*(unsigned *)(ISR_BADDR+0x84))
#define pISR_USBH      (*(unsigned *)(ISR_BADDR+0x88))
#define pISR_IIC       (*(unsigned *)(ISR_BADDR+0x8c))
#define pISR_UART0     (*(unsigned *)(ISR_BADDR+0x90))
#define pISR_SPI1      (*(unsigned *)(ISR_BADDR+0x94))
#define pISR_RTC       (*(unsigned *)(ISR_BADDR+0x98))
#define pISR_ADC       (*(unsigned *)(ISR_BADDR+0x9c))

// Interrupts.h
//----------------------------------------------------------------------------
// irqNum
//----------------------------------------------------------------------------
#define EINT0				0
#define EINT1				1
#define EINT2				2
#define EINT3				3
#define EINT11				4
#define EINT19				5
#define BAT_FLT				6
#define INT_TICK			7
#define INT_WDT				8
#define INT_TIMER0			9
#define INT_TIMER1			10
#define INT_TIMER2			11
#define INT_TIMER3			12
#define INT_TIMER4			13
#define INT_UART2_RX		14
#define INT_UART2_TX		15
#define INT_LCD				16
#define INT_DMA0			17
#define INT_DMA1			18
#define INT_DMA2			19
#define INT_DMA3			20
#define INT_SDI				21
#define INT_SPI0			22
#define INT_UART1_RX		23
#define INT_UART1_TX		24
#define INT_USBD			25
#define INT_USBH			26
#define INT_IIC				27
#define INT_UART0_RX		28
#define INT_UART0_TX		29
#define INT_SPI1			30
#define INT_RTC				31
#define INT_TC				32
#define EINT18				33
#define EINT8				34

/* The number of interrupts in the device. */
#define NUM_HAL_INTERRUPTS      35
#define NUM_IRQS				NUM_HAL_INTERRUPTS

// INTERRUPT
#define REG_SRCPND		(*(volatile unsigned *)0x4a000000)	//Interrupt request status
#define REG_INTMOD		(*(volatile unsigned *)0x4a000004)	//Interrupt mode control
#define REG_INTMSK		(*(volatile unsigned *)0x4a000008)	//Interrupt mask control
#define REG_PRIORITY	(*(volatile unsigned *)0x4a00000c)	//IRQ priority control
#define REG_INTPND		(*(volatile unsigned *)0x4a000010)	//Interrupt request status
#define REG_INTOFFSET	(*(volatile unsigned *)0x4a000014)	//Interruot request source offset
#define REG_SUBSRCPND	(*(volatile unsigned *)0x4a000018)	//Sub source pending
#define REG_INTSUBMSK	(*(volatile unsigned *)0x4a00001c)	//Interrupt sub mask
#define REG_EINTPEND	(*(volatile unsigned *)0x560000a8)	// EInt pending

// S3C2410X01 Interrupt controller bit positions
#define    BIT_EINT0        (0x1)
#define    BIT_EINT1        (0x1<<1)
#define    BIT_EINT2        (0x1<<2)
#define    BIT_EINT3        (0x1<<3)
#define    BIT_EINT4_7      (0x1<<4)
#define    BIT_EINT8_23     (0x1<<5)
#define    BIT_RSV1         (0x1<<6)
#define    BIT_BAT_FLT      (0x1<<7)
#define    BIT_TICK         (0x1<<8)
#define    BIT_WDT          (0x1<<9)
#define    BIT_TIMER0       (0x1<<10)
#define    BIT_TIMER1       (0x1<<11)
#define    BIT_TIMER2       (0x1<<12)
#define    BIT_TIMER3       (0x1<<13)
#define    BIT_TIMER4       (0x1<<14)
#define    BIT_UART2        (0x1<<15)
#define    BIT_LCD          (0x1<<16)
#define    BIT_DMA0         (0x1<<17)
#define    BIT_DMA1         (0x1<<18)
#define    BIT_DMA2         (0x1<<19)
#define    BIT_DMA3         (0x1<<20)
#define    BIT_SDI	        (0x1<<21)
#define    BIT_SPI0	        (0x1<<22)
#define    BIT_UART1        (0x1<<23)
#define    BIT_RSV2         (0x1<<24)
#define    BIT_USBD         (0x1<<25)
#define    BIT_USBH         (0x1<<26)
#define    BIT_IIC	        (0x1<<27)
#define    BIT_UART0        (0x1<<28)
#define    BIT_SPI1         (0x1<<29)
#define    BIT_RTC	        (0x1<<30)
#define    BIT_ADC	        (0x1<<31)
#define    BIT_ALLMSK       (0xffffffff)

#define    BIT_SUB_ADC		(0x1<<10)
#define    BIT_SUB_TC		(0x1<<9)
#define    BIT_SUB_ERR2		(0x1<<8)
#define    BIT_SUB_TXD2		(0x1<<7)
#define    BIT_SUB_RXD2		(0x1<<6)
#define    BIT_SUB_ERR1		(0x1<<5)
#define    BIT_SUB_TXD1		(0x1<<4)
#define    BIT_SUB_RXD1		(0x1<<3)
#define    BIT_SUB_ERR0		(0x1<<2)
#define    BIT_SUB_TXD0		(0x1<<1)
#define    BIT_SUB_RXD0		(0x1<<0)
#define    BIT_SUB_ALLMSK	(0x7ff)

#define    BIT_EINT_4		(0x1<<4)
#define    BIT_EINT_5		(0x1<<5)
#define    BIT_EINT_6		(0x1<<6)
#define    BIT_EINT_7		(0x1<<7)
#define    BIT_EINT_8		(0x1<<8)
#define    BIT_EINT_9		(0x1<<9)
#define    BIT_EINT_10		(0x1<<10)
#define    BIT_EINT_11		(0x1<<11)
#define    BIT_EINT_12		(0x1<<12)
#define    BIT_EINT_13		(0x1<<13)
#define    BIT_EINT_14		(0x1<<14)
#define    BIT_EINT_15		(0x1<<15)
#define    BIT_EINT_16		(0x1<<16)
#define    BIT_EINT_17		(0x1<<17)
#define    BIT_EINT_18		(0x1<<18)
#define    BIT_EINT_19		(0x1<<19)
#define    BIT_EINT_20		(0x1<<20)
#define    BIT_EINT_21		(0x1<<21)
#define    BIT_EINT_22		(0x1<<22)
#define    BIT_EINT_23		(0x1<<23)
#define    BIT_EINT_ALLMSK	(0xfffff0)

#define ClearPending(bit) {\
                REG_SRCPND = bit;\
                REG_INTPND = bit;\
                }

/* ARM modes */
#define USERMODE        0x10
#define FIQMODE         0x11
#define IRQMODE         0x12
#define SVCMODE         0x13
#define ABORTMODE       0x17
#define UNDEFMODE       0x1b
#define MODEMASK        0x1f
#define NOINT           0xc0

#endif	// !__S3C2410A_H__
