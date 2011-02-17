/*
 * Copyright(c) 2000-10, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded
 * NOTE : Do not call these functions unless you are a part of rexy core module
 *		  Applications should call rexy's api instead of these RAL functions
 */


#ifndef __REXY_HAL_INTERFACE_H__
#define __REXY_HAL_INTERFACE_H__

#include "rexy_lock.h"
#include "rexy_feature.h"

// Ported Standard C Library
extern void*	RalMemAlloc(unsigned int size);
extern void*	RalCalloc(unsigned int num, unsigned int size);
extern void* 	RalRealloc(void* ptr, ruint32 size);
extern void		RalMemFree(void*);

extern void*	RalMemCpy(void*, const void*, unsigned int);
extern void*	RalMemSet(void*, int, unsigned int);
extern void		RalZeroMemory(void* dest, unsigned int size);

extern char*	RalStrDup(const char*);
extern char*	RalStrCpy(char*, const char*);
extern char*	RalStrNCpy(char* dest, const char* src,unsigned int count);
extern int		RalStrCmp(const char *string1, const char *string2);
extern int      RalStrLen(const char *s);
extern int		RalStrNCmp(const char *string1, const char *string2,unsigned int count);
extern char*	RalStrCat(char* dest, const char* src);
extern char*	RalStrNCat(char * dest, const char * src, ruint32 count);
extern char*	RalStrChr(const char* pszSource, int nChar );
extern int		RalStrAToI(const char *s);

extern int	    RalOpenFile(const char * file);
extern int      RalReadFile(int fd, char * buffer, int length);
extern int      RalWriteFile(int fd, const char * buffer, int length);
extern int      RalLseekFile(int fd, int offset, int whence);
extern int      RalCloseFile(int fd);
extern bool     RalFileExists(const char* szFileName);
extern int		RalMakeDir(const char* szDirName);
extern int      RalCreateFile(const char* szFileName);
extern int	    RalDeleteFile(const char* szFileName);
extern bool		RalShrinkFile(int fd, int new_size);
extern int		RalChDir(const char * filepath);
extern long		RalGetFileLength(int fd);

extern void		RalDebugMessage( const char* msg );
extern void		RalDebugBreak(void);

extern unsigned int RalMkTime(const struct tm* time);
extern struct tm* RalLocalTime(unsigned int time);

extern int		RalGetFileTotalNumber(void);
extern bool     RalGetFileList(char * text_lst,unsigned long * textsize,int max_lst,int max_file_name,int start_lst/*,char * filepath*/);
extern int		RalChDir(const char * filepath);	
extern int 		RalGetFileNameLength(void);
extern int		RalRand(void);

// Event
bool RalEventGet(pmsg outMsg, ruint32 inTimeoutMs);

// Timer related functions
bool RalTimeGetCurrent(struct tm* outCurrentTime);
ruint32 RalTimeGetTicks(void);
bool RalTimeSetTimer(ruint32 inMilliseconds, int id);
bool RalTimeKillTimer(int id);

// Screen related functions
bool RalScreenGetSize(rect* outSize);
int  RalScreenGetWidth(void);
int  RalScreenGetHeight(void);
UNITCL* RalScreenGetBuffer(void);
int RalScreenGetMemSize(void);
bool RalScreenGetBpp(ruint8* outBpp);
bool RalScreenFlush(void);
bool RalScreenFlushPart(rect* inBoundary);
bool RalScreenBacklight(bool inOn);

// Lock related functions
bool RalLckInit(void);
bool RalLckLock(LockIDType lockID);
bool RalLckUnlock(LockIDType lockID);

//  
void HostToDev(unsigned int src, unsigned char * dev);
unsigned int DevToHost(unsigned char * dev);
unsigned short DevToHostShort(unsigned char * dev);

// System Feature related function
int RalGetSystemFeature (FeatureType type);
//

/*------- Platform unique --------*/
extern int RalMessageBufferID;
typedef struct _RalMessageType
{
    long mtype;
    int mtext[3];
} RalMessageType;

typedef enum {
  RAL_DIAG_NO_CMD,
  RAL_DIAG_SUCCESS,
  RAL_DIAG_FATAL
} RalDiagType;

typedef struct {
  int cmd;
} DiagCmd;

bool RalInitDiag(void);
bool RalDeinitDiag(void);
RalDiagType RalReadDiagCmd(DiagCmd* pCmd);
RalDiagType RalWriteDiagCmd(DiagCmd* pCmd);


#endif
