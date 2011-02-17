#include "rexy.h"
#include "HwrRTCTimer.h"
#include "HwrKeyMgr.h"
#include "queue.h"

#define DEVICE_WIDTH 240
#define DEVICE_HEIGHT 320


OS_EVENT* RalSema = NULL;

UNITCL lcd_bmp_buffer[240][320] = {0,};

bool RalEventGet(pmsg outMsg, ruint32 inTimeoutMs)
{
	int ret;
	KeyEventType key;
	int nKeyMsgType, nKeyCode, nKeyStatus;

	
	// Get Key Event From Device Driver
	while(1)
	{
		ret = GetKeyEvent(&key);	
		if( ret > 0 )
		{
			int validKey;
			
			if ( key.keyStatus == AWM_TIMER )
			{
				UsrHandleKernelTimerMessage();
				outMsg->wmsg = AWM_NONE;
				return TRUE;
			}
			if( key.keyStatus <= KEYREPEAT )
			{
				nKeyMsgType = key.keyStatus + AWM_KEYDOWN;
			}
			else
			{
				continue;
			}		
			
			nKeyCode = key.keyCode;		
			nKeyStatus = key.keyStatus;
			outMsg->wmsg = nKeyMsgType;			
			switch( nKeyCode )
			{
			case 0: outMsg->wparam = VK_PAD_0; validKey = 1; break;
			case 1: outMsg->wparam = VK_PAD_1; validKey = 1; break;
			case 2: outMsg->wparam = VK_PAD_2; validKey = 1; break;
			case 3: outMsg->wparam = VK_PAD_3; validKey = 1; break;
			case 4: outMsg->wparam = VK_PAD_4; validKey = 1; break;
			case 5: outMsg->wparam = VK_PAD_5; validKey = 1; break;
			case 6: outMsg->wparam = VK_USER_OK; validKey = 1; break;
			case 7: outMsg->wparam = VK_USER_CLR; validKey = 1; break;
			case 8: outMsg->wparam = VK_USER_UP; validKey = 1; break;
			case 9: outMsg->wparam = VK_USER_DOWN; validKey = 1; break;
			case 10: outMsg->wparam = VK_USER_LEFT; validKey = 1; break;
			case 11: outMsg->wparam = VK_USER_RIGHT; validKey = 1; break;
			default: outMsg->wparam = nKeyCode+'0';	validKey = 0; break;
			}
			outMsg->lparam = 0;
			//CONSOL_Printf("key wmsg:%d, wparam:%d\n", nKeyMsgType,nKeyCode); 
			//CONSOL_Printf("RalEventGet nKeyCode:%d, validKey:%d\n", nKeyCode, validKey);
			if ( validKey ) return TRUE;
		}
		/*
		if ( !MsgIsQueueEmpty() )
		{
			outMsg->wmsg = AWM_NONE;
			return TRUE;
		}
		*/
	}
}

bool RalScreenGetSize(rect* outSize)
{
	outSize->left = 0;
	outSize->right = DEVICE_WIDTH;
	outSize->top = 0;
	outSize->bottom = DEVICE_HEIGHT;
}

int  RalScreenGetWidth(void)
{
    return DEVICE_WIDTH;
}

int  RalScreenGetHeight(void)
{
    return DEVICE_HEIGHT;
}

UNITCL* RalScreenGetBuffer(void)
{
	return (UNITCL*)lcd_bmp_buffer;
}

int RalScreenGetMemSize(void)
{
	return sizeof(lcd_bmp_buffer);
}

bool RalScreenGetBpp(ruint8* outBpp)
{
	*outBpp = 16;
	return TRUE;
}

bool RalScreenFlush(void)
{
    bool ret;
    ret = HwrDisplayDump(lcd_bmp_buffer);
    return ret;
}

bool RalScreenFlushPart(rect* inBoundary)
{
    bool ret;
    ret = HwrDisplayDumpPart(inBoundary->left, inBoundary->top, (inBoundary->right - inBoundary->left) + 1 , (inBoundary->bottom - inBoundary->top) + 1, lcd_bmp_buffer);
    return ret;
}

bool RalScreenBacklight(bool inOn)
{
    return FALSE;
}

bool RalTimeGetCurrent(struct tm* outCurrentTime)
{
    char	IS_Y2K = 1;
    short t_mdays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    //double newTime;
    unsigned long newTime;
    double	tmpSec;
    signed short yy, mm, dd, h, m, s;

    bool ret;

    ret = TRUE;
    newTime = GetRTCTime();

    CONSOL_Printf("== Cur Second : %ld\n", newTime);

    if (newTime >= (double)SEC_1904_2K) {
        newTime -= (double)SEC_1904_2K;
        IS_Y2K = 1;
    } else
        IS_Y2K = 0;

    // year
    for (yy = IS_Y2K ? 2000 : 1904, tmpSec = (double)LEAP_YEAR_SEC; newTime >= tmpSec; ) {
        newTime -= tmpSec;
        yy++;

        tmpSec = (double)(yy%4 == 0) && (yy%100 != 0) || (yy%400 == 0) ? LEAP_YEAR_SEC : YEAR_SEC;
    }

    // month
    for (mm = 1, tmpSec = (double)t_mdays[mm]*86400; newTime >= tmpSec; ) {
        newTime -= tmpSec;
        mm++;
        if (mm==2 && ((yy%4 == 0) && (yy%100 != 0) || (yy%400 == 0)))
            tmpSec = (double)2505600; //29*24*60*60
        else
            tmpSec = (double)t_mdays[mm]*86400; /*24*60*60*/
    }


    // day
    for (dd = 1; newTime >= (double)86400; dd++, newTime -= (double)86400);

    // hour
    for (h = 0; newTime >= (double)3600; h++, newTime -= (double)3600);

    // minute
    for (m = 0; newTime >= (double)60; m++, newTime -= (double)60);

    s = newTime; // by chc

    CONSOL_Printf("==-- s(%d)/NewTime(%d)\n", s, newTime );

    outCurrentTime->tm_sec = s;
    outCurrentTime->tm_min = m;
    outCurrentTime->tm_hour = h;
    outCurrentTime->tm_mday = dd;
    outCurrentTime->tm_mon = mm;
    outCurrentTime->tm_year = yy;
    outCurrentTime->tm_yday = 0;
    outCurrentTime->tm_wday = 1;
    outCurrentTime->tm_isdst = 0;

    CONSOL_Printf("==-- curTSec(%d)\n", outCurrentTime->tm_sec);
    return ret;
}

static void TimerFunc(void)
{
	KeyEventType timerEvt;
	DisableKeyTimerInterrupt();
	timerEvt.keyCode = 0xFFFF;
	timerEvt.keyStatus = AWM_TIMER;
	EnqueueKeyEvent(&timerEvt);
	EnableKeyTimerInterrupt();
}
bool RalTimeSetTimer(ruint32 inMilliseconds, int id)
{
    extern void RxInsertTimerMsg(void);
    SetTimer(inMilliseconds, TimerFunc); // 1ms
    return FALSE;
}

bool RalTimeKillTimer(int id)
{
	return FALSE;
}

void HostToDev(unsigned int src, unsigned char * dev)
{
    dev[0] = (unsigned char)((src >> 24) & 0xFF);
    dev[1] = (unsigned char)((src >> 16) & 0xFF);
    dev[2] = (unsigned char)((src >> 8 ) & 0xFF);
    dev[3] = (unsigned char)((src) & 0xFF);
}

unsigned int DevToHost(unsigned char * dev)
{
    unsigned int ret_val;

    ret_val = 0;

    ret_val |= dev[0] << 24;
    ret_val |= dev[1] << 16;
    ret_val |= dev[2] << 8;
    ret_val |= dev[3];

    return ret_val;
}

unsigned short DevToHostShort(unsigned char * dev)
{
    unsigned short ret_val;

    ret_val = 0;

    ret_val |= (unsigned short)(dev[0] << 8);
    ret_val |= (unsigned short)(dev[1] << 0);

    return ret_val;
}

ruint32 RalTimeGetTicks(void)
{
	return HwrGetTicks()*10;
}
/**
 * @brief		타겟에 사용하는 락 시스템을 초기화
 * @return		초기화 여부
 * @remarks		
 * @see			
 * @if HISTORY
 *				05-Jul-2006 yich Initiated 
 * @endif
 */
bool RalLckInit(void)
{
	CONSOL_Printf("RalLckInit\n");
	RalSema = OSSemCreate(1);
    return (RalSema!=NULL);
}

/**
 * @brief		락을 작동시킴
 * @param[in]	lockID	작동시킬 락 ID
 * @return		락 작동 여부
 * @remarks		
 * @see			
 * @if HISTORY
 *				05-Jul-2006 yich Initiated 
 * @endif
 */
bool RalLckLock(LockIDType lockID)
{
	/*
	INT8U err;
	OSSemPend(RalSema, 0, &err);
    return (err == OS_NO_ERR);
    */
    return TRUE;
}

/** 
 * @brief		타겟에서 작동된 락을 해제
 * @param[in]	lockID 해제할 락 ID
 * @return 		락 해제 여부
 * @remarks
 * @see
 * @if HISTORY
 *				05-Jul-2006 yich Initiated 
 * @endif
 */
bool RalLckUnlock(LockIDType lockID)
{
/*
	INT8U err;
	err = OSSemPost(RalSema);
    return (err==OS_NO_ERR);
    */
    return TRUE;
}

int RalRand()
{
	return rand();
}
bool RalInitDiag(void)
{
	return TRUE;
}

bool RalDeinitDiag(void)
{
		return TRUE;
}

int RalGetSystemFeature (FeatureType type)
{
	return 0;
}
/**
 * @return
 */
RalDiagType RalReadDiagCmd(DiagCmd* pCmd)
{
	return RAL_DIAG_NO_CMD;
}

/**
 * @return
 */
RalDiagType RalWriteDiagCmd(DiagCmd* pCmd)
{
	return RAL_DIAG_SUCCESS;
}

int RalStrAToI(const char *s)
{
	return atoi(s);
}
