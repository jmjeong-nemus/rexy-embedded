/*
* $Id: rexy_halinterface.c 435 2005-02-16 08:25:12Z klenui $
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
*
* This file is a part of Rexy Embedded 
*
*/

/** @file */

/**
 * @defgroup halifc Hal Interface
 * These APIs are associated with Hal Interface
 * @ingroup api
 * @{
 */

#include "rexy.h"
#include "rexy_haldevice.h"

#include <windows.h>
#include "resource.h"
#include "rexy_debug_win.h"
#include "rexy_test_win.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>

#define DEVICE_HEIGHT	LCD_HEIGHT
#define DEVICE_WIDTH	LCD_WIDTH

static UNITCL lcd_bmp_buffer[DEVICE_HEIGHT][DEVICE_WIDTH] = {0,};
static UNITCL frame_buffer[DEVICE_HEIGHT][DEVICE_WIDTH] = {0,};

//////////////////////////////////////////////////////////////////////////
// Standard C Library Section Start
//////////////////////////////////////////////////////////////////////////

/** Allocate memory blocks
* Wrapper function for malloc in the standard C library. Returns NULL if it fails to allocate the required memory.
*
* @param size_t memory byte size
*
* @return void * allocated memory pointer
*/
void* RalMemAlloc(unsigned int size)
{
    void *ptr;

    ptr =  malloc(size);

    return ptr;
}


/** Free memory blocks
* Wrapper function for free in the standard C library
* 
* @param void *ptr memory pointer to free
*
* @return void
*/

void RalMemFree(void* ptr)
{
    if (!ptr) return;

    free( ptr );
}

/** Allocate memory blocs and initialize their contents with zero.
* Wrapper function for calloc in the standard C library. Returns NULL if it fails to allocate the required memory.
*
* @param num number of memory count
*
* @param size byte size
*
* @return void * allocated memory pointer
*/
void* RalCalloc(unsigned int num, unsigned int size)
{
    void *ptr;

    ptr = calloc(num, size);

    return ptr;
}

/** Reallocation of memory function
* Wrapper function for realloc in the standard C library. Returns NULL if it fails to allocate the required memory.
*
* @param ptr address of already assigned memory block
*
* @param size byte size
*
* @return void * re-allocated memory pointer
*/
void* RalRealloc(void* ptr, size_t size)
{
    void* newptr;

    newptr = realloc(ptr, size);

    return newptr;
}

/** Copy a string.
* Wrapper function for the strcpy in the standard C library.
*
* @param dest Destination string
* @param src  Null-terminated source string
*
* @return returns the value of dest
*/

char* RalStrCpy(char* dest, const char* src)
{
    return strcpy( dest, src );
}

/** Copy N characters of the source string to the destination string.
* Wrapper function for the strncpy in the standard C library
*
* @param dest Destination string
* @param src  Source string
* @param count Number of characters to be copied
*
* @return returns the value of dest
*/

char* RalStrNCpy(char* dest, const char* src,unsigned int count)
{
    return strncpy( dest, src, count );
}

/** Compare strings.
* Wrapper function for the strcmp in the standard C library. 
* 
* @param string1 Null-terminated strings to compare
* @param string2 Null-terminated strings to compare
*
* @return The return value indicates the lexicographic relation of string1 to string2.
*         returns a value less than zero if string1 is less than string2. returns zero 
*         if two strings are equivalent. returns a value greater than zero if string1 is 
*         greater than string2
* 
*/
int RalStrCmp(const char *string1, const char *string2)
{
    return strcmp(string1,string2);
}


/** Duplicate string.
* Wrapper function for strdup in the standard C library. Allocate a memory block, copy the source string and returns the 
* pointer to the new string.
*
* @param ptr string pointer
*
* @return void * allocated memory pointer
*/
char* RalStrDup(const char* ptr)
{
    //char* ptmp;

    //return ( CheckFile( szFileName, FS_TEST_FILE ) );
    return 0;
}


/** Copy memory blocks
* Wrapper function for memcpy in the standard C library. 
* 
* @param dest	destination memory pointer
* @param src	source memory pointer
* @param size	size
*
* @return returns the value of dest
*/

void* RalMemCpy(void* dest, const void* src, unsigned int size)
{
    return memcpy( dest, src, size );
}

/** Opens a file and return the file handle.
* This functions opens a file with read and write permission. 
* @param file	the file name to be opened.
* @return the file handle. If an error occurs, the return value is less than zero. 
* 
*/
int	RalOpenFile(const char * file)
{
    return _open(file, _O_BINARY | _O_RDWR);
}

/**  Read data from the file indicated by the file handle.
* 
* @param fd		the file handle
* @param buffer	the buffer where data to be stored.
* @param length	length to be read.
* @return the number of bytes read.
*/
int RalReadFile(int fd, char* buffer, int length)
{
    return _read(fd, buffer, length);
}

/**  Write data to the file of the file handle.
* @param fd		the file handle
* @param buffer	the buffer which has data to be stored
* @param length	length to be written.
* @return the number of bytes written.
*/
int RalWriteFile(int fd, const char * buffer, int length)
{
    return _write(fd, buffer, length);
}

/** Move a file pointer to the specified location.
*  Repositions the offset of the open file associated with the file descriptor fildes to the argument offset according to the directive whence as follows.
* - SEEK_SET The offset is set to offset bytes.
* - SEEK_CUR The offset is set to its current location plus offset bytes.
* - SEEK_END The offset is set to the size of the file plus offset bytes.
* 
* @param fd the file handle
* @param offset the offset of the open file
* @param whence directive hence
* 
* @return same as lseek function in POSIX
*/
int RalLseekFile(int fd, int offset, int whence)
{
    return _lseek(fd, offset, whence);
}

/** Close the file indicated by the file handle.
* @param fd		the file handle.
* @return  Return 0 if the file was successfully closed. 
*		    A return value of -1 indicates an error.
*/
int RalCloseFile(int fd)
{
    return _close(fd);
}

/** Check if the file exists
* 
* @param szFileName the fie name
* 
* @return TRUE if file exist, FALSE if the file does not exist
*/
bool RalFileExists(const char* szFileName)
{
    HANDLE hFile;

    if ( !szFileName ) return FALSE;

    hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
    {
        //ASSERT(GetLastError()==ERROR_FILE_NOT_FOUND);
        return FALSE;
    }
    CloseHandle(hFile);
    return TRUE;
}

/** Create directory
* 
* @param szDirName the directory name
* 
* @return File handle. On error, return -1
*/
int RalMakeDir(const char* szDirName)
{
    return mkdir(szDirName);
}


/** Create a file with read and write permission
* 
* @param szFileName the file name
* 
* @return File handle. On error, return -1
*/
int RalCreateFile(const char* szFileName)
{
    return creat( szFileName, 0000400 | 0000200 /* _S_IREAD | _S_IWRITE */ );
}

/** Delete a file.
* 
* @param szFileName the file name
* 
* @return On success, return TRUE. On failure, return FALSE
*/
int RalDeleteFile(const char* szFileName)
{
    return (remove ( szFileName ));
}

/** Shrink the file.
* 
* @param fd file descriptor
*
* @param new_size the desired byte length of the file.
*
* @return On success, return TRUE. On failure, return false
*  
*/

bool RalShrinkFile(int fd, int new_size)
{
    //    return ((ftruncate ( fd, new_size ) == FS_OKAY_S) ? TRUE : FALSE);
    return 0;
}

long RalGetFileLength(int fd)
{
	_lseek(fd, 0, SEEK_END);
    return _tell(fd);
}

/** 특정경로에 있는 파일들의 총 개수를 리턴하다.
* @return 파일의 총 개수를 리턴한다.
*/
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
}

int RalChDir(const char * filepath)
{
	return _chdir(filepath);
}

/** Output debugging messages.
* 
* @param msg debugging message
*
*	@return none
*/
void RalDebugMessage( const char* msg )
{
    //OutputDebugString(msg);
	mon_send_msg(msg);
}


/** Output testing messages.
* 
* @param msg testing message
*
*	@return none
*/
void RalTestMessage( const char* msg )
{
    //OutputDebugString(msg);
	test_send_msg(msg);
}



/** Halt an execution.
* function stops the execution of the application which called this function. 
*/
void RalDebugBreak(void)
{
}

void* RalMemSet(void* dst, int value, unsigned int size)
{
    memset(dst, value, size);
    return dst;
}

void* RalMemCopy(void* dst, void* src, int size)
{
    memcpy(dst, src, size);
    return dst;
}

/** Initialize the buffer with zero.
* Equivalent to memset(dest, 0, sizeof(dest))
*
* @param dest pointer to the buffer to clear its contents
* @param size byte length of the buffer
*
*	@return none
*/
void RalZeroMemory(void* dest, unsigned int size)
{
    memset( dest, 0, size );
}

/** Calculates the length of the string s, not including the terminating `\0' character.
*  Wrapper function for strlen in the standard C library. 
* 
* @param s source string pointer
* 
* @return the byte length of String s
*/
int RalStrLen(const char *s)
{
    return strlen(s);
}

int RalStrAToI(const char *s)
{
	return atoi(s);
}

/** Compare N characters of two strings.
* Wrapper function for the strncmp in the standard C library. 
* 
* @param string1 Null-terminated strings to compare
* @param string2 Null-terminated strings to compare
*
* @return The return value is same with that of RalStrCmp. The only difference of this function is 
*         that RalStrNCmp compares only the first "count" characters of the given argument strings. 
*/
int RalStrNCmp(const char *string1, const char *string2,unsigned int count)
{
    return strncmp(string1,string2,count);
}

/** Append a string.
*  Wrapper function for strcat in the standard C library. 
* 
* @param	 dest	Null-terminated destination string
* @param	 src	Null-terminated source string
* 
* @return the destination string. No return value is reserved to indicate an error.
*/
char*	RalStrCat(char* dest, const char* src)
{
    return strcat( dest, src );
}

/** Append a string.
*  Wrapper function for strncat in the standard C library. 
* 
* @param	 dest	Null-terminated destination string
* @param	 src	Null-terminated source string
* @param	 count	the maximum number of the characters to copy from src
* 
* @return the destination string. No return value is reserved to indicate an error.
*/
char  * RalStrNCat(char * dest, const char * src, size_t count)
{
    return strncat(dest, src, count);
}

/** Find a character in a string.
*  Wrapper function for strchr in the standard C library. 
* 
* @param	pszSource	Null-terminated source string
* @param	nChar		Character code to be found
* 
* @return	a pointer to the first occurrence of c in string, or NULL if c is not found.
*/
char*	RalStrChr(const char* pszSource, int nChar )
{
    return strchr(pszSource, nChar);
}

const unsigned char days_in_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
#include "time.h"
bool _is_leap(int year)
{
    //TODO
    return FALSE;
}

/**
* Converts time structure(struct tm) into unsinged int
* 
* @param time time structure. Refer struct tm
* 
* @return the seconds since Jan. 1970
*
* Joshua Commented : Year in time structer should be 1900 based.
*/
unsigned int RalMkTime(const struct tm* time)
{
    int y,m,d,h,min,s;
    int leap;
    unsigned int sec;

    sec = 0;

    if ( time ->tm_year > 1900 || time ->tm_year < 0 )
    {
        //ASSERT( 0 );
    }
    y = 70;
    while( y < time->tm_year )
    {
        sec += (_is_leap(y) ? 366*24*60*60 : 365*24*60*60);
        y++;
    }

    leap = _is_leap(y);
    m = 0;
    while( m < time->tm_mon )
    {
        if ( leap && m == 1 )
        {
            sec += (days_in_month[m] + 1)*24*60*60;
        }
        else
        {
            sec += (days_in_month[m])*24*60*60;
        }
        m++;
    }

    d = 1;
    while( d < time->tm_mday )
    {
        sec += 24 * 60 * 60;
        d++;
    }

    h = 0;
    while( h < time->tm_hour )
    {
        sec += 60 * 60;
        h++;
    }
    min = 0;
    while( min < time->tm_min )
    {
        sec += 60;
        min++;
    }
    s = 0;
    while( s < time->tm_sec )
    {
        sec++;
        s++;
    }

    // Tokyo time zone.
    sec -= 9 * 60 * 60;
    return sec;
}


/**
* Converts unsigned int into time structure(struct tm)
* 
* @param time unsigned int value which presents time after Jan, 1, 1970 in seconds
* 
* @return the pointer of struct tm which contains struct tm data of parameter "time"
*/
struct tm* RalLocalTime( unsigned int time )
{
    int year;
    unsigned int days;
    static struct tm ts;

    const int WDAY = 4; // Jan 1, 1970 is Wed

    // Tokyo time zone.
    time += 9 * 60 * 60;

    //ts.isdst = -1;
    days = time / (24 * 3600);
    ts.tm_wday = (days+WDAY)%7;

    year = 1970;		//Joshua modified
    while(1)
    {
        unsigned int ydays;
        if ( _is_leap(year-1900) )
        {
            ydays = 366;
        }
        else
        {
            ydays = 365;
        }
        if ( days < ydays ) break;
        days -= ydays;
        year++;
    }
    ts.tm_year = year;
    ts.tm_yday = days;
    {
        int mon, i;
        unsigned int sum;
        int SpecialFeb = 0;

        sum = mon = 0;

        for ( i = 0 ; i < 12 ; i++ )
        {
            SpecialFeb = 0;

            if ( i == 1 )
            {
                if ( _is_leap( ts.tm_year - 1900 ) )
                    SpecialFeb = 1;
            }

            if ( (days+1) <= sum + days_in_month[i] + SpecialFeb ) break;

            mon++;
            sum += days_in_month[i] + SpecialFeb;
        }

        ts.tm_mon = mon;
        ts.tm_mday = days - sum + 1;
    }
    time %= (24 * 3600);
    ts.tm_hour = time / 3600;
    time %= 3600;
    ts.tm_min = time / 60;
    ts.tm_sec = time % 60;

    ts.tm_year -= 1900;	//added by Joshua 19-Aug-2005
    return &ts;
}

int RalRand(void)
{
	return rand();
}
//////////////////////////////////////////////////////////////////////////
// Standard C Library Section End
//////////////////////////////////////////////////////////////////////////


/** Get screen width.
*
* @return Return screen width
*/
int  RalScreenGetWidth(void)
{
    return DEVICE_WIDTH;
}

/** Get screen height.
*
* @return Return screen height
*/
int  RalScreenGetHeight(void)
{
    return DEVICE_HEIGHT;
}

/** Get screen buffer.
*
* @return Return screen buffer
*/
UNITCL* RalScreenGetBuffer(void)
{
    return (UNITCL*)lcd_bmp_buffer;
}

/** Get screen memory size.
*
* @return Return screen memory size
*/
int RalScreenGetMemSize(void)
{
    return sizeof(lcd_bmp_buffer);
}

/** Get current time.
*
* @param outCurrentTime 
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalTimeGetCurrent(struct tm* outCurrentTime)
{
    const struct tm *pTime;
    time_t curTime;

    curTime = time(NULL);
    pTime = localtime(&curTime);
    memcpy( outCurrentTime, pTime, sizeof(struct tm) );

    return TRUE;
}
// Created by Joshua on 10-Jan-2006
ruint32 RalTimeGetTicks(void)
{
	return GetTickCount();
}

extern HWND		g_hMainWnd;

bool RalTimeSetTimer(ruint32 inMilliseconds, int id)
{
    SetTimer(g_hMainWnd, id, inMilliseconds, NULL);
    return TRUE;
}

bool RalTimeKillTimer(int id)
{
	KillTimer(g_hMainWnd, id);
	return TRUE;
}


/** Get event.
*
* @param outMsg 
* @param inTimeoutMs 
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalEventGet(pmsg outMsg, ruint32 inTimeoutMs)
{
    MSG tmsg;
	rint32 start_tick;

    while(1)
    {
        int ret;

		if ( !inTimeoutMs )
			ret = GetMessage(&tmsg, NULL, 0, 0);
		else
		{
			start_tick = UsrGetCurrentTicks();
			do 
			{
				ret = PeekMessage(&tmsg,NULL,0,0,PM_REMOVE);
				if ( ret ) break;
			} while( UsrGetCurrentTicks() - start_tick < inTimeoutMs );

			if ( !ret ) 
			{
				outMsg->wmsg = AWM_NONE;
				outMsg->wparam = 0;
				outMsg->lparam = 0;
			}
		}

        if ( ret == -1 ) 
		{
			return FALSE;		//Should not reach here
		}
        else if ( ret == 0 ) return FALSE;	//Exit

        else
        {
            TranslateMessage(&tmsg);
			DispatchMessage(&tmsg);
			if ( tmsg.hwnd != g_hMainWnd ) 
			{
				continue;
			}
			
            if ( tmsg.message == WM_COMMAND )	// for later simulator upgrade
            {
				if ( tmsg.wParam == ID_DEBUG_MONITOR )
				{
					ShowWindow(GetDebugWindow(), SW_SHOW);
				}
				else
				if( tmsg.wParam == ID_TEST_MONITOR)
				{
					ShowWindow(GetTestWindow(), SW_SHOW);
				}

                return TRUE;
            }
            else if ( tmsg.message == WM_KEYDOWN )
            {
                switch(tmsg.wParam)
                {
                case VK_LEFT:
                    outMsg->wparam = VK_USER_LEFT;
                    outMsg->wmsg = AWM_KEYDOWN;
                    outMsg->lparam = tmsg.lParam;
                    return TRUE;
                case VK_RIGHT:
                    outMsg->wparam = VK_USER_RIGHT;
                    outMsg->wmsg = AWM_KEYDOWN;
                    outMsg->lparam = tmsg.lParam;
                    return TRUE;
                case VK_UP:
                    outMsg->wparam = VK_USER_UP;
                    outMsg->wmsg = AWM_KEYDOWN;
                    outMsg->lparam = tmsg.lParam;
                    return TRUE;
                case VK_DOWN:
                    outMsg->wparam = VK_USER_DOWN;
                    outMsg->wmsg = AWM_KEYDOWN;
                    outMsg->lparam = tmsg.lParam;
                    return TRUE;
				case VK_DELETE:
					outMsg->wparam = VK_USER_DELETE;
                    outMsg->wmsg = AWM_KEYDOWN;
                    outMsg->lparam = tmsg.lParam;
                    return TRUE;
				case VK_BACK:
					outMsg->wparam = VK_USER_BACKSPACE;
                    outMsg->wmsg = AWM_KEYDOWN;
                    outMsg->lparam = tmsg.lParam;
                    return TRUE;
                }
            }
            else if ( tmsg.message == WM_CHAR )
            {
                switch(tmsg.wParam)
                {
                case VK_RETURN:
                    outMsg->wparam = VK_USER_OK;
                    break;
                case VK_ESCAPE:
                    outMsg->wparam = VK_USER_CLR;
                    break;
                default:
                    outMsg->wparam = tmsg.wParam;
                    break;
                }

                outMsg->wmsg = AWM_KEYDOWN;
                outMsg->lparam = tmsg.lParam;

                return TRUE;
            }
            else if ( tmsg.message == WM_TIMER && tmsg.wParam == HAL_TIMER_ID )
			{
				KillTimer( g_hMainWnd, tmsg.wParam );
				
				outMsg->wmsg = AWM_TIMER;
				outMsg->wparam = tmsg.wParam;
				outMsg->lparam = 0;
				return TRUE;
			}
        }
    }
    return FALSE;
}

/** Convert host to dev.
*
* @param src 
* @param dev 
*
* @return converted value
*/
void HostToDev(unsigned int src, unsigned char * dev)
{
    dev[0] = (unsigned char)((src >> 24) & 0xFF);
    dev[1] = (unsigned char)((src >> 16) & 0xFF);
    dev[2] = (unsigned char)((src >> 8 ) & 0xFF);
    dev[3] = (unsigned char)((src) & 0xFF);
}

/** Convert dev to host.
*
* @param dev 
*
* @return converted value
*/
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

/** Convert dev to host short.
*
* @param dev 
*
* @return converted value
*/
unsigned short DevToHostShort(unsigned char * dev)
{
    unsigned short ret_val;

    ret_val = 0;

    ret_val |= (unsigned short)(dev[0] << 8);
    ret_val |= (unsigned short)(dev[1] << 0);

    return ret_val;
}

static HBITMAP frame_bitmap = NULL;

static void PrvDumpScreen(void)
{
    BITMAPINFOHEADER binfo_header;
    BITMAPINFO * b_info;
    HDC hdc;


    if (frame_bitmap)
    {
        DeleteObject(frame_bitmap);
        frame_bitmap = NULL;
    }

    // make bitmap from the g_screen_buffer .
    binfo_header.biSize = sizeof(BITMAPINFOHEADER);
    binfo_header.biWidth = DEVICE_WIDTH;
    binfo_header.biHeight = -1 * DEVICE_HEIGHT;
    binfo_header.biPlanes = 1;
    binfo_header.biBitCount = 16;
    binfo_header.biCompression = BI_BITFIELDS;

    binfo_header.biSizeImage = DEVICE_WIDTH * DEVICE_HEIGHT * 2;
    binfo_header.biClrUsed = 0;
    binfo_header.biClrImportant = 0;

    //binfo_header.
    b_info = (BITMAPINFO *)calloc(1, sizeof(BITMAPINFOHEADER) + sizeof(DWORD) * 3);
    b_info->bmiHeader = binfo_header;

    // for BPP_16
    ((DWORD *)b_info->bmiColors)[0] = 0xF800;
    ((DWORD *)b_info->bmiColors)[1] = 0x07E0;
    ((DWORD *)b_info->bmiColors)[2] = 0x001F;

    hdc = GetDC(g_hMainWnd);

    frame_bitmap = CreateDIBitmap(hdc, &binfo_header, CBM_INIT,
                                  frame_buffer, b_info, DIB_RGB_COLORS);
	ReleaseDC(g_hMainWnd,hdc);

    free(b_info);

    InvalidateRect(g_hMainWnd, NULL, FALSE);
    UpdateWindow(g_hMainWnd);
}

/** Flush screen,
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalScreenFlush(void)
{
    memcpy( frame_buffer, lcd_bmp_buffer, sizeof(frame_buffer) );
    PrvDumpScreen();
    return TRUE;
}

bool RalScreenFlushPart(rect* inBoundary)
{
    int x, y;
    int sx, sy;
    int ex, ey;		

	// revised as pixel position concept by Joshua
	// initialization
    sx = 0;
    sy = 0;
    ex = DEVICE_WIDTH -1;	
    ey = DEVICE_HEIGHT-1;

	if ( inBoundary )
	{
        if ( inBoundary->left > sx ) sx = inBoundary->left;
        if ( inBoundary->right < ex ) ex = inBoundary->right;
        if ( inBoundary->top > sy ) sy = inBoundary->top;
        if ( inBoundary->bottom < ey ) ey = inBoundary->bottom;
    }

    for( y = sy ; y <= ey ; y++ )
    {
        for ( x = sx ; x <= ex ; x++ )
        {
            frame_buffer[y][x] = lcd_bmp_buffer[y][x];
        }
    }
    PrvDumpScreen();
    return TRUE;
}
/** Repaint screen.
*
* @param hWnd   Window Handle
* @param ps		Paint Structure
*
*/
void RalScreenRepaint(HWND hWnd, LPPAINTSTRUCT ps)
{
 
    HDC hdc;
	HDC hMemDC;
	HBITMAP hBitmapOld;

	hdc = ps->hdc;

	// copy the mem dc contents into the screen dc
	hMemDC = CreateCompatibleDC(hdc);
	hBitmapOld = (HBITMAP)SelectObject(hMemDC, (HGDIOBJ)frame_bitmap);
	StretchBlt(hdc, 0, 0, DEVICE_WIDTH, DEVICE_HEIGHT, hMemDC, 0, 0, DEVICE_WIDTH, DEVICE_HEIGHT, SRCCOPY);
	SelectObject(hMemDC, (HGDIOBJ)hBitmapOld);

	DeleteDC(hMemDC);
 
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
    return TRUE;
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
	switch(type)
	{
	case FEATURE_LCD_WIDTH: return LCD_WIDTH;
	case FEATURE_LCD_HEIGHT: return LCD_HEIGHT;
	case FEATURE_LCD_BPP:	return 16;
	case FEATURE_UNICODE:	return FALSE;
	case FEATURE_BIGENDIAN: return FALSE;
	default:
		ASSERT(FALSE);
		return 0;
	}
}

bool RalInitDiag(void)
{
	return FALSE;
}

bool RalDeinitDiag(void)
{
	return FALSE;
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

/** @} */ // End of Hal Interface
