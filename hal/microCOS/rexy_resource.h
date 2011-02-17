#ifndef __REXY_RESOURCE_H__
#define __REXY_RESOURCE_H__

#include "..\..\resource\rexy_fontdata.h"
int RscGetFontWidth(FntID id, int code);
int RscGetFontHeight(FntID id);
bool RscGetFontImage(FntID id, byte* dst, int dstSize, int code );
int RscGetFontByteWidth(FntID id);


#endif//__REXY_RESOURCE_H__
