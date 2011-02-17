#include "rexy.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <glib.h>

#define DEVICE_WIDTH 240
#define DEVICE_HEIGHT 320
#define LISTENQ 1024
#define SERV_PORT 8080

static bool gSemaphore[LOCK_ID_MAX];
extern bool RxGdkSetTimer(ruint32 timeval, int id);
extern bool RxGdkKillTimer(int id);

long RalGetFileLength(int fd)
{
     return lseek(fd, 0, SEEK_END);
}

/** 특정경로에 있는 파일들의 총 개수를 리턴하다.
* @return 파일의 총 개수를 리턴한다.
*/

// Not implemented on linux yet
// Maybe we have to use opendir/readdir and the codes below later
int RalGetFileTotalNumber()
{
     return 0;
}
/*
//
//  Simple string pattern matching functions using wildcards ('?' & '*').
//

#include <stdio.h>
#include <ctype.h>

//
//  Case-sensitive pattern match
//
int patMatch (const char* pattern, const char* string)
{
      switch (pattern[0])
      {
      case '\0':
            return !string[0];

      case '*':
            return patMatch(pattern+1, string) || string[0] && patMatch(pattern, string+1);

      case '?':
            return string[0] && patMatch(pattern+1, string+1);


      default:
            return (pattern[0] == string[0]) && patMatch(pattern+1, string+1);
      }
}

//
//  Case-insensitive pattern match
//
int patiMatch (const char* pattern, const char* string)
{
      switch (pattern[0])
      {
      case '\0':
            return !string[0];

      case '*' :
            return patiMatch(pattern+1, string) || string[0] && patiMatch(pattern, string+1);

      case '?' :
            return string[0] && patiMatch(pattern+1, string+1);

      default  :
            return (toupper(pattern[0]) == toupper(string[0])) && patiMatch(pattern+1, string+1);
      }
}

//
// Test functions
//
int main(int argc, char *argv[])
{
      if (argc != 3)
      {
            puts("Usage: wildcards pattern string");
            return -1;
      }

      printf("Case sensitive:   \"%s\" \"%s\" %s\n",
             argv[1], argv[2],
             patMatch(argv[1], argv[2])?"matches":"does not match");
      printf("Case insensitive: \"%s\" \"%s\" %s\n",
             argv[1], argv[2],
             patiMatch(argv[1], argv[2])?"matches":"does not match");
    
      return 0;
}

int RalGetFileTotalNumber()
{
	int i=0;
	long hFile;
	struct _finddata_t c_file;

	if( (hFile = _findfirst( "*.*", &c_file )) == -1L )
	{
		//no file
		return 0;
	}
	else
	{
		i++;
		while( _findnext( hFile, &c_file ) == 0)
		{
			 i++;
		}
		_findclose( hFile );
		return i;
	}

}
*/

/**지원하는 최대 파일 이름 길이를 반환한다.
*/
int RalGetFileNameLenght(void)
{
	return 210;
}

/** 특정 경로에 있는 파일 이름들을 가져온다.
* 
* @param text_lst, 이름을 가져올 buffer
*
* @param max_lst, 몇개의 파일이름을 가져올 것인지 나타냄.
*
* @param max_file_name, 지원하는 파일이름 최대길이를 나타냄.
* @param start_lst, 몇번째 부터 파일 이름을 가져올 것인지 나타냄.
* @return 1 success, return 0. On failure, return false
*  
*/
bool RalGetFileList(char * text_lst,unsigned long * textsize,int max_lst,int max_file_name,int start_lst/*,char * filepath*/)//이 함수는 DEVICE 의존적인 함수이다.
{
/*
	int i;
	struct _finddata_t c_file;
	long hFile;
	int tmp;
	
	memset(text_lst,0,(max_lst*max_file_name));
	
	if( (hFile = _findfirst( "*.*", &c_file )) == -1L )
	{
		return 0;// no file exist
	}
	else
	{
		  if(start_lst ==0)
		  {
			tmp = strlen(c_file.name);
			if(tmp<=max_file_name)
			{
				strncpy(&text_lst[0],c_file.name,tmp);
				textsize[0]= c_file.size;
			}
			else
			{
			  strncpy(&text_lst[0],c_file.name,max_file_name);
			}
			textsize[0]=c_file.attrib; //temp code
			
			i=1;
			while( _findnext( hFile, &c_file ) == 0 && i<max_lst)
			{
				textsize[i]=c_file.attrib;//temp code
			    strcpy(&text_lst[i*max_file_name],c_file.name);
				i++;
			}
		  }
		  else
		  {
			
			while(start_lst)
			{
				 _findnext( hFile, &c_file );
				 start_lst--;
			}
			i=0;
			while( _findnext( hFile, &c_file ) == 0 && i < max_lst)
			{
				  strcpy(&text_lst[i*max_file_name],c_file.name);
				  textsize[i]=c_file.attrib;
				 i++;
			}
		  }

       _findclose( hFile );

	}
	if(text_lst[0] == 0)
	{
		return 0;// no file exist
	}
	else
	{
		return 1;//file exist
	}
*/
     return 0;
}

int RalChDir(const char * filepath)
{
	return chdir(filepath);
}

UNITCL lcd_bmp_buffer[DEVICE_HEIGHT][DEVICE_WIDTH];

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

extern void SetFrameBuffer(rect* area);

bool RalScreenFlush(void)
{
    SetFrameBuffer(NULL);
    return TRUE;
}

bool RalScreenFlushPart(rect* inBoundary)
{
    SetFrameBuffer(inBoundary);
    return TRUE;
}

void RalScreenDestroy()
{
	
}
// Created by Joshua on 10-Jan-2006
ruint32 RalTimeGetTicks(void)
{
	ruint32 MilliTick;
	struct timeval tv;
	gettimeofday(&tv);
	MilliTick = (tv.tv_sec&0x00000FFFL) * 1000 +  tv.tv_usec / 1000;
	return MilliTick;
}

bool RalTimeGetCurrent(struct tm* outCurrentTime)
{
    const struct tm *pTime;
    time_t curTime;

    curTime = time(NULL);
    pTime = localtime(&curTime);
    memcpy( outCurrentTime, pTime, sizeof(struct tm) );

    return TRUE;
}

bool RalTimeSetTimer(ruint32 inMilliseconds, int id)
{
    return RxGdkSetTimer( inMilliseconds, id );
}

bool RalTimeKillTimer(int id)
{
	return RxGdkKillTimer(id);
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

GAsyncQueue* RalKernelBuffer = 0;

bool RalEventGet(pmsg outMsg, ruint32 inTimeoutMs)
{
    int ret;
    pmsg rcvMsg;

    if ( !RalKernelBuffer ) return FALSE;

	/*
	while(1)
	{
		ret = msgrcv(RalMessageBufferID, &halMsg, 12, 0, IPC_NOWAIT);
		if ( ret == -1 ) break;
	}
	*/
    
    rcvMsg = g_async_queue_pop( RalKernelBuffer );

    outMsg->wmsg = rcvMsg->wmsg;
    outMsg->wparam = rcvMsg->wparam;
    outMsg->lparam = rcvMsg->lparam;

    free( rcvMsg );
    return ((outMsg->wmsg == AWM_QUIT)?FALSE:TRUE);
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
     LockIDType i;
     
     for ( i = 0 ; i < LOCK_ID_MAX ; i ++ )
     {
	  gSemaphore[i] = FALSE;
     }
     return TRUE;
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
     if ( gSemaphore[lockID] ) printf("LockID:%d is locked...\n",lockID);
     while ( gSemaphore[lockID] );
     printf("unlocked...\n");
     gSemaphore[lockID] = TRUE;
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
     gSemaphore[lockID] = FALSE;
     return TRUE;
}

int RalRand()
{
	return rand();
}

/** 
 * @brief		타겟에서 제공하는 기능에 따른 값을 반환
 * @param[in]	type	타겟에서 사용하는 값을 알고자하는 기능 ID
 * @return 		타겟에서 사용하는 값을 반환
 * @remarks
 * @see
 * @if HISTORY
 *				05-Jul-2006 wijang Initiated 
 * @endif
 */

int RalGetSystemFeature (FeatureType type)
{
	return 0;
}

static int RalDiagBufferID = -1;
static int RalDiagPID = -1;


static void* diagThread(void* args)
{
	int listenfd, connfd;
	socklen_t clilen;
	struct sockaddr_in servaddr, cliaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( listenfd < 0 )
	{
		printf("can't create socket\n");
		pthread_exit(-1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if ( bind(listenfd, (struct socketaddr*)&servaddr, sizeof(servaddr)) < 0 )
	{
		printf("can't bind socket\n");
		pthread_exit(-2);
	}

	if ( listen(listenfd, LISTENQ) < 0 )
	{
		printf("can't listen\n");
		pthread_exit(-3);
	}


	for(;;)
	{
		char buf[20];
		ssize_t n;

		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct socketaddr*)&cliaddr, &clilen);

		if ( connfd < 0 ) pthread_exit(-4);
		
		do {
			n = read(connfd, buf, sizeof(buf));
			if ( n > 0 ){ printf (buf); fflush(stdout);}
		} while( n > 0 || (n < 0 && errno == EINTR) );
	}

	close(listenfd);

	pthread_exit(0);
}

bool RalInitDiag(void)
{
    int ret;
	int pid;
    key_t msgkey;
	
    msgkey = ftok(".", ',');
    RalDiagBufferID = msgget(msgkey, IPC_CREAT|0660);
    if ( RalDiagBufferID == -1 )
    {
        printf("can't create message buffer\n");
        return FALSE;
    }
    
    pthread_create(&RalDiagPID, NULL, diagThread, NULL);

	return TRUE;
}

bool RalDeinitDiag(void)
{
	pthread_cancel(RalDiagPID, NULL);
	return TRUE;
}

/**
 * @return
 */
RalDiagType RalReadDiagCmd(DiagCmd* pCmd)
{
	int ret;
	DiagCmd cmd;

	if ( RalDiagBufferID == -1 ) return RAL_DIAG_NO_CMD;
	if ( !pCmd ) return RAL_DIAG_FATAL;

	ret = msgrcv(RalDiagBufferID, &cmd, sizeof(DiagCmd), 0, IPC_NOWAIT);
	if ( ret != sizeof(DiagCmd) ) return RAL_DIAG_FATAL;

	memcpy( pCmd, &cmd, sizeof(DiagCmd) );

	return RAL_DIAG_SUCCESS;
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
