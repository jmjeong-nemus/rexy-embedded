/******************************************************************************
 *
 * File: HwrDisplay.h
 *
 * Description:
 *      Display Driver for S3C2410
 *
 * History:
 *      05/13/2005 Created By Jaewoong Sim
 *   	
 *
 *****************************************************************************/

#ifndef __HWRDISPLAY_H__
#define __HWRDISPLAY_H__

#include "includes.h"

// Defines
#define DISPLAY_WIDTH			240
#define DISPLAY_HEIGHT			320
#define DISPLAY_DEPTH			16
#define DISPLAY_BASE_ADDR		0x32000000
#define DISPLAY_VRAM_SIZE		((DISPLAY_WIDTH * DISPLAY_HEIGHT * DISPLAY_DEPTH) >> 3)

// Prototypes
int HwrDisplayInit(INT32U width, INT32U height, INT32U depth);
void HwrDisplayWake(void);

#endif // !__HWRDISPLAY_H__
