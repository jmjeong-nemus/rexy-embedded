/*
 * $Id: rexy_resource.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *
 */

#ifndef __REXY_RESOURCE_H__
#define __REXY_RESOURCE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rexy_font.h"

    /** @file */
typedef int BmpID;
typedef int CodePageID;


// This type defines every detail of one bitmap data.
typedef enum
{
  REXY_BMP_NOCOMP = 0,
  REXY_BMP_COMP_H	,
  REXY_BMP_COMP_V	
} BmpFormat;


typedef struct 
{
  BmpFormat bmpFormat;
  short width; 
  short height;
  void *dataPtr;
} BmpType; 

RxErr RscSetFont(FntID id,FntType *FntPtr);
int RscGetFontWidth(FntID id, int code);
bool RscGetFontImage(FntID id, byte* dst, int dstSize, int code);
int RscGetFontHeight(FntID id);
int RscGetFontByteWidth(FntID id);
bool RscIsValidFntID(FntID id);

RxErr RscSetBitmapGroup( BmpType *BmpArrayPtr );
int RscGetBmpWidth(BmpID id);
int RscGetBmpHeight(BmpID id);
bool RscGetBmpImage(BmpID id, byte* dst, int dstSize);
const UNITCL* RscGetRawImage(BmpID id);
bool RscIsValidBmpID(BmpID id);

bool RscCheckResourceFont(void);
bool RscCheckResourceIcon(void);
bool RscCheckResourceImage(void);
void RscRLEUncompress( unsigned char *in, unsigned char *out, unsigned int insize );

int	RscLoadBitmap(const BmpID id, RxBITMAP * bitmap);
int	RscDestroyBitmap(RxBITMAP * bitmap);
int	RscLoadBitmapFile(const char *name, RxBITMAP * bitmap, int nComp);

//////////////////////////////////////////////////////////////////////////
// RLE Compress
void RLE_Uncompress( unsigned char *in, unsigned char *out, unsigned int insize );
static unsigned int RGB565Uncompress( unsigned char *in, unsigned char *out, unsigned int insize );	
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_RESOURCE_H__
