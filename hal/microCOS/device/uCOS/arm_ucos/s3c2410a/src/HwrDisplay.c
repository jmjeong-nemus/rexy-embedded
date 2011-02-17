/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: HwrDisplay.c
 *
 * Description:
 *      Display Driver for S3C2410A
 *
 * History:
 *      05/13/2005 Created By Jaewoong Sim
 *   	
 *
 *****************************************************************************/

#include "includes.h"
#include "HwrDisplay.h"

// HwrDisplayInit-*

int HwrDisplayInit(INT32U width, INT32U height, INT32U depth)
{
    HwrS3C2410LCDRegPtr LCD_baseP = (HwrS3C2410LCDRegPtr)HWRS3C2410_LCD_BASE;
    HwrS3C2410IOPRegPtr IOP_baseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;
    INT32U newBaseAddr = DISPLAY_BASE_ADDR;
    INT8U bppMode, swap;

    if( width != 240 )
        return 1;

    if( height != 320 )
        return 1;

    if( depth != 16 )
        return 1;

    LCD_baseP->rLCDCON1 &= ~(1<<0);		// ENVID
    LCD_baseP->rLCDCON5 &= ~(1<<3);		// PWREN

    // LCD Port Init
    IOP_baseP->rGPCUP  = 0xFFFFFFFF;
    IOP_baseP->rGPCCON = 0xAAAAAAAA;	// LCD Control Signal, VD0~7
    IOP_baseP->rGPDUP  = 0xFFFFFFFF;
    IOP_baseP->rGPDCON = 0xAAAAAAAA;	// VD8~23

    if ((width * depth) % 32)
        width = (((width * depth) / 32) + 1) * 32 / depth;

    switch (depth) {
    case 1:  bppMode = 8;  swap = (1<<1)|(0<<0); break;
    case 2:  bppMode = 9;  swap = (1<<1)|(0<<0); break;
    case 4:  bppMode = 10; swap = (1<<1)|(0<<0); break;
    case 8:  bppMode = 11; swap = (1<<1)|(0<<0); break;
    case 16: bppMode = 12; swap = (0<<1)|(1<<0); break;
    }

    // LCD Register Init
    LCD_baseP->rLCDCON1 = (4<<8)|(0<<7)|(3<<5)|(bppMode<<1)|(0);
    LCD_baseP->rLCDCON2 = (1<<24)|((height-1)<<14)|(2<<6)|(1);
    LCD_baseP->rLCDCON3 = (6<<19)|((width-1)<<8)|(2);
    LCD_baseP->rLCDCON4 = (3);
    LCD_baseP->rLCDCON5 = (1<<11)|(1<<9)|(1<<8)|(swap);
    LCD_baseP->rLCDSADDR1 = ((newBaseAddr>>22)<<21)|M5D(newBaseAddr>>1);
    LCD_baseP->rLCDSADDR2 = M5D((newBaseAddr+((width*height*depth)>>3))>>1);
    LCD_baseP->rLCDSADDR3 = (((width-width)/1)<<11)|(width/1);
    LCD_baseP->rLPCSEL |= (2<<0); // Meritech

    return 0;
}

// HwrDisplayWake-*
// Wake Up LCD Display
void HwrDisplayWake()
{
    HwrS3C2410LCDRegPtr LCD_baseP = (HwrS3C2410LCDRegPtr)HWRS3C2410_LCD_BASE;
    HwrS3C2410IOPRegPtr IOP_baseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // Enable the LCD controller
    LCD_baseP->rLCDCON1 |= (1<<0);	// ENVID
    IOP_baseP->rGPGUP |= (1<<4);
    IOP_baseP->rGPGCON |= (3<<8);	// LCD_PWREN (EINT12)
    LCD_baseP->rLCDCON5 |= (1<<3);	// PWREN
}

int HwrDisplayDump(INT16U * dumpImage)
{
    int i;
#if 0	
    INT32U* vAddr = (INT32U*)DISPLAY_BASE_ADDR;
    INT32U bufferSize = 240 * 160;
    for( i=0; i<bufferSize; i++ )
    {
        INT32U tmp = 0x0;
        tmp = (*dumpImage);
        tmp = tmp<<16;
        tmp += *dumpImage++;
        *vAddr++ = tmp;
    }
#else
    INT16U * vAddr = (INT16U *)DISPLAY_BASE_ADDR;
    INT32U bufferSize = 240 * 320 / 32;

    //	memcpy(vAddr, dumpImage, bufferSize);
    for( i=0; i<bufferSize; i++ )
    {
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
        *vAddr++ = *dumpImage++;
    }
#endif
    // no err
    return 0;
}

int HwrDisplayDumpPart(int x, int y, int width, int height, INT16U * dumpImage)
{
    INT16U* vAddr = (INT16U*)DISPLAY_BASE_ADDR;
    int i,j;

    // check parameter
    if( x < 0 || y < 0 )
        return 1;

    if( width > 240 || height > 320 )
        return 1;

    if( x + width > 240 || y + height > 320 )
        return 1;

    vAddr += 240 * (y) + x;
	dumpImage += 240 * (y) + x;

    for( i=0; i<height; i++ )
    {
        // width 만큼 출력한다.
        for( j=0; j<width; j++ )
            *vAddr++ = *dumpImage++;
        //memcpy(vAddr, dumpImage, width*2);

        // 나머지 x 만큼 이동한다.
        vAddr += (240-width);
        dumpImage += (240-width);
        
    }

    return 0;
}

