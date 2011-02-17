/*
 * $Id: rexy_trace.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Device dependent한 함수 및 상수가 상수가 정의되어 있다.
 *
 */

#ifndef __REXY_TRACE_H__
#define __REXY_TRACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @file */

// Undefine ASSERT if any for PPC device
// by Joshua on 12-Apr-2007
#ifdef ASSERT
#undef ASSERT
#endif

#define ASSERT(expression) {                                        \
        if ( !(expression) ) {                                      \
            char buf[0x100];                                        \
            StrPrintF(buf, "%s:%u: failed assertion `%s'\n",        \
                       __FILE__, __LINE__, #expression);            \
            TrcAssert( buf );										\
        }                                                           \
    }


typedef int (*CTFuncPtr)(struct _window * hwnd);

typedef struct {
	char		CTName[25];
	CTFuncPtr	Proc;
} CTMenuItemType;

void		TrcPrint(const char * str );
int			TrcPrintF(const char * format, ...);
const char* TrcGetShortFileName(const char* szFileName);
void		TrcPrintHdr(unsigned int area, unsigned int level);
extern void TrcDumpAreaStatus(void);
extern void TrcSetTraceLevel(int level);
extern void TrcControlTraceAll(bool on);
extern RxErr TrcControlTraceArea(const char* areaName, int level);
extern void TrcProbeReport(char *FunctionName);
extern int	TrcAddBreakFunction(char *FunctionName);
extern int TrcDelBreakFunction(int SlotNumber);
extern void TrcReleaseBreak(void);
extern int TrcStrCmp(const char *string1, const char *string2);
extern void TrcAssert(char *log);
extern unsigned int TRACELEVEL;
extern unsigned int TRACESELECT;

extern const char* GetKeyName(const int keycode);
extern const char* GetMessageName(const int message);
/*
  Trace Areas
*/
enum TR_AREA {
    TR_Test             = 0x00000001,

    TR_RexyCore         = 0x00000002,
    TR_RexyControl      = 0x00000004,
    TR_RexyEtc          = 0x00000008,
    TR_SystemEvent      = 0x00000010,
    TR_COMPONENT		= 0x00000020,

    TR_Memory           = 0x10000000
};

typedef struct {
    const char* szAreaName;
    unsigned int szAreaSelect;
    int          szAreaLevel;
} AREA_LIST;

extern AREA_LIST arrAreaList[];
extern const int AREA_COUNT;

/*
  Trace Levels
*/
enum TR_LEVEL {
    TR_LEVEL1 = 0x01,           // tracing level 1 (0001)
    TR_LEVEL2 = 0x03,           // tracing level 2 (0011)
    TR_LEVEL3 = 0x07,           // tracing level 3 (0111)
    TR_LEVEL4 = 0x0f            // tracing level 4 (1111)
};


enum TR_MARKLEVEL {
    TR_MARKLEVEL1 = 0xf,        // tracing level mark 1 (1111)
    TR_MARKLEVEL2 = 0xe,        // tracing level mark 2 (1110)
    TR_MARKLEVEL3 = 0xc,        // tracing level mark 3 (1100)
    TR_MARKLEVEL4 = 0x8         // tracing level mark 4 (1000)
};

#ifdef  _DEBUG

#define TRACE(_area, _level, _message) {											\
        if ((TRACESELECT & (_area)) && (TRACELEVEL&(_level))) {						\
			TrcPrintHdr(_area, _level);												\
            TrcPrintF("%08d:%-15s(%04d) ", RalTimeGetTicks(), TrcGetShortFileName(__FILE__), __LINE__);	\
            TrcPrintF _message;														\
        }																			\
    }
#else
#define TRACE(_area, _level, _message)
#endif


    unsigned long SnapHeapSize(void);
    void PrintHeapDiff(const char* prefix, const char* funcName);

    typedef enum { EVENT_HOOK_HANDLER, ACTION_HOOK_HANDELR, REXY_FUNC, APP_HANDLER } HANDLER_TYPE;
    void SetCurrentHandler(int type, int app, int state);

    //External API
	void TrcPrintAreaName(void);
    void TrcSetCurrentHandler(int type, int app, int state);
    void TrcDbgBreak(const char* file_name, const int line_num);
	void TrcDumpMemory(void *Ptr,int DumpSize,int NumColumn);

#ifdef __cplusplus
}
#endif

#endif // end of __REXY_TRACE_H__
          


