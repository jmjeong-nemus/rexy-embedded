#ifndef __REXY_EMUL_INTERFACE_H__
#define __REXY_EMUL_INTERFACE_H__


#define DEVICE_WIDTH		240
#define DEVICE_HEIGHT		320

bool RalEventGet(pmsg outMsg, ruint32 inTimeoutMs);

bool RalScreenGetSize(rect* outSize);
int  RalScreenGetWidth(void);
int  RalScreenGetHeight(void);
UNITCL* RalScreenGetBuffer(void);
int RalScreenGetMemSize(void);

bool RalScreenGetBpp(ruint8* outBpp);
bool RalScreenFlush(void);
bool RalScreenFlushPart(rect* inBoundary);
bool RalScreenBacklight(bool inOn);

bool RalTimeGetCurrent(struct tm* outCurrentTime);
bool RalTimeSetTimer(ruint32 inMilliseconds, int id);

void HostToDev(unsigned int src, unsigned char * dev);
unsigned int DevToHost(unsigned char * dev);
unsigned short DevToHostShort(unsigned char * dev);

#endif
