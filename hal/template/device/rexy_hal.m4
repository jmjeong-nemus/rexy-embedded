define(`copyright',`/*
 * $Id: 
 * Copyright(c) 2000-6, Mococo Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */')dnl
define(`doxygen_start',`/**
 * @defgroup $1
 * $2
 * @ingroup $3
 * @{')dnl
define(`doxygen_end',`/** @} */')dnl
define(`USE_MALLOC', `/** Allocate memory blocks
* Wrapper function for malloc in the standard C library. Returns NULL if it fails to allocate the required memory.
*
* @param size_t memory byte size
*
* @return void * allocated memory pointer
*/
void* RalMemAlloc(unsigned int size)
{
    void *ptr;

    ptr =  $1(size);

    return ptr;
}')dnl
define(`USE_CALLOC',`/** Allocate memory blocs and initialize their contents with zero.
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

    ptr = $1(num, size);

    return ptr;
}')dnl
define(`USE_REALLOC',`/** Reallocation of memory function
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

    newptr = $1(ptr, size);

    return newptr;
}')dnl
define(`USE_FREE',`/** Free memory blocks
* Wrapper function for free in the standard C library
* 
* @param void *ptr memory pointer to free
*
* @return void
*/

void RalMemFree(void* ptr)
{
    if (!ptr) return;

    $1( ptr );
}')dnl
define(`USE_MEMCPY',`/** Copy memory blocks
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
    return $1( dest, src, size );
}')dnl
define(`USE_MEMSET',`void* RalMemSet(void* dst, int value, unsigned int size)
{
    $1(dst, value, size);
    return dst;
}')dnl
define(`USE_ZEROMEMORY',`/** Initialize the buffer with zero.
* Equivalent to memset(dest, 0, sizeof(dest))
*
* @param dest pointer to the buffer to clear its contents
* @param size byte length of the buffer
*
*	@return none
*/
void RalZeroMemory(void* dest, unsigned int size)
{
ifelse($1,`ZeroMemory',`	ZeroMemory(dest,size);',$1,`memset',`	memset(dest,0,size);',`')
}')dnl
define(`USE_STRDUP',`/** Duplicate string.
* Wrapper function for strdup in the standard C library. Allocate a memory block, copy the source string and returns the 
* pointer to the new string.
*
* @param ptr string pointer
*
* @return void * allocated memory pointer
*/
char* RalStrDup(const char* ptr)
{
    return $1(ptr);
}')dnl
define(`USE_STRCPY',`/** Copy a string.
* Wrapper function for the strcpy in the standard C library.
*
* @param dest Destination string
* @param src  Null-terminated source string
*
* @return returns the value of dest
*/

char* RalStrCpy(char* dest, const char* src)
{
    return $1( dest, src );
}')dnl
define(`USE_STRNCPY',`/** Copy N characters of the source string to the destination string.
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
    return $1( dest, src, count );
}')dnl
define(`USE_STRCMP',`/** Compare strings.
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
    return $1(string1,string2);
}')dnl
define(`USE_STRNCMP',`/** Compare N characters of two strings.
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
    return $1(string1,string2,count);
}')dnl
define(`USE_STRLEN',`/** Calculates the length of the string s, not including the terminating character.
*  Wrapper function for strlen in the standard C library. 
* 
* @param s source string pointer
* 
* @return the byte length of String s
*/
int RalStrLen(const char *s)
{	
    return $1(s);
}')dnl
define(`USE_STRCAT',`/** Append a string.
*  Wrapper function for strcat in the standard C library. 
* 
* @param	 dest	Null-terminated destination string
* @param	 src	Null-terminated source string
* 
* @return the destination string. No return value is reserved to indicate an error.
*/
char*	RalStrCat(char* dest, const char* src)
{
    return $1( dest, src );
}')dnl
define(`USE_STRNCAT',`/** Append a string.
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
    return $1(dest, src, count);
}')dnl
define(`USE_STRCHR',`/** Find a character in a string.
 *  Wrapper function for strchr in the standard C library. 
 * 
 * @param	pszSource	Null-terminated source string
 * @param	nChar		Character code to be found
 * 
 * @return	a pointer to the first occurrence of c in string, or NULL if c is not found.
 */
char*	RalStrChr(const char* pszSource, int nChar )
{
    return $1(pszSource, nChar);
}')dnl
define(`USE_ITOA',`char*	RalItoa(int nValue, char* pszResult)
{
}')dnl
define(`USE_OPENFILE',`/** Opens a file and return the file handle.
* This functions opens a file with read and write permission. 
* @param file	the file name to be opened.
* @return the file handle. If an error occurs, the return value is less than zero. 
* 
*/
int RalOpenFile(const char* file)
{
#error You must implement RalOpenFile
}')dnl
define(`USE_READFILE',`/**  Read data from the file indicated by the file handle.
* 
* @param fd		the file handle
* @param buffer	the buffer where data to be stored.
* @param length	length to be read.
* @return the number of bytes read.
*/
int RalReadFile(int fd, const char* buffer, DWORD length)
{
#error You must implement RalReadFile
}')dnl
define(`USE_WRITEFILE',`/**  Write data to the file of the file handle.
* @param fd		the file handle
* @param buffer	the buffer which has data to be stored
* @param length	length to be written.
* @return the number of bytes written.
*/
int RalWriteFile(int fd, const char* buffer, DWORD length)
{
#error You must Implement RalWriteFile
}')dnl
define(`USE_LSEEKFILE',`/** Move a file pointer to the specified location.
*  Repositions the offset of the open file associated with the file descriptor fildes to the argument offset according to the directive whence as follows.
* - FILE_BEGIN	    The offset is set to offset bytes.
* - FILE_CURRENT	The offset is set to its current location plus offset bytes.
* - FILE_END		The offset is set to the size of the file plus offset bytes.
* 
* @param fd the file handle
* @param offset the offset of the open file
* @param whence directive hence
* 
* @return same as lseek function in POSIX
*/
int RalLseekFile(int fd, int offset, int whence)
{
#error You must implement RalLseekFile
}')dnl
define(`USE_CLOSEFILE',`/** Close the file indicated by the file handle.
* @param fd		the file handle.
* @return  Return nonzero if the file was successfully closed. 
*		    A return value of 0 indicates an error.
*/
int RalCloseFile(int fd)
{
#error You must implement RalCloseFile
}')dnl
define(`USE_FILEEXISTS',`/** Check if the file exists
* 
* @param szFileName the fie name
* 
* @return TRUE if file exist, FALSE if the file does not exist
*/
bool RalFileExists(const char* szFileName)
{
#error You must implement RalFileExists
}')dnl
define(`USE_MAKEDIR',`/** Create directory
* 
* @param szDirName the directory name
* 
* @return nonzero. On error, return 0
*/
int RalMakeDir(const char* szDirName)
{
#error You must implement RalMakeDir
}')dnl
define(`USE_CREATEFILE',`/** Create a file with read and write permission
* 
* @param szFileName the file name
* 
* @return File handle. On error, return -1
*/
int RalCreateFile(const char* szFileName)
{
#error You must implement RalCreateFile
}')dnl
define(`USE_DELETEFILE',`/** Delete a file.
* 
* @param szFileName the file name
* 
* @return On success, return TRUE. On failure, return FALSE
*/
bool RalDeleteFile(const char* szFileName)
{
#error You must implement RalDeleteFile
}')dnl
define(`USE_SHRINKFILE',`/** Shrink the file.
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
#error You must implement RalShrinkFile
}')dnl
define(`USE_SPRINTF',`')dnl
define(`USE_DEBUGMESSAGE',`/** Output debugging messages.
* 
* @param msg debugging message
*
*	@return none
*/
void RalDebugMessage( const char* msg )
{
ifelse($1,`none',`',`	$1((LPCTSTR)msg);')
}')dnl
define(`USE_DEBUGBREAK',`/** Halt an execution.
* function stops the execution of the application which called this function. 
*/
void RalDebugBreak(void)
{
ifelse($1,`none',`',`	$1();')
}')dnl
define(`USE_TRACE',`')dnl
define(`INTERNAL_MKTIME',`    int y`,'m`,'d`,'h`,'min`,'s;
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
    return sec;')dnl
define(`INTERNAL_LOCALTIME',`    int year;
    unsigned int days;
    static struct tm ts;

    const int WDAY = 4; // Jan 1`,' 1970 is Wed

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
        int mon`,' i;
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
    return &ts;')dnl
define(`USE_MKTIME',`/**
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
ifelse($1,`internal',INTERNAL_MKTIME,$1,`none',`	return 0;',`	return $1(time);')
}')dnl
define(`USE_LOCALTIME',`/**
* Converts unsigned int into time structure(struct tm)
* 
* @param time unsigned int value which presents time after Jan, 1, 1970 in seconds
* 
* @return the pointer of struct tm which contains struct tm data of parameter "time"
*/
struct tm* RalLocalTime( unsigned int time )
{
ifelse($1,`internal',INTERNAL_LOCALTIME,$1,`none',`	return NULL;',`	return $1(time);')
}')dnl
define(`USE_SCREEN_WIDTH',`/** Get screen width.
*
* @return Return screen width
*/
int  RalScreenGetWidth(void)
{
#error you must implement RalScreenGetWidth
}')dnl
define(`USE_SCREEN_HEIGHT',`/** Get screen height.
*
* @return Return screen height
*/
int  RalScreenGetHeight(void)
{
#error you must implement RalScreenGetHeight
}')dnl
define(`USE_SCREEN_BUFFER',`/** Get screen buffer.
*
* @return Return screen buffer
*/
UNITCL* RalScreenGetBuffer(void)
{
#error you must implement RalScreenGetBuffer
}')dnl
define(`USE_SCREEN_MEMSIZE',`/** Get screen memory size.
*
* @return Return screen memory size
*/
int RalScreenGetMemSize(void)
{
#error you must implement RalScreenGetMemSize
}')dnl
define(`USE_TIME_GETCURRENT',`/** Get current time.
*
* @param outCurrentTime 
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalTimeGetCurrent(struct tm* outCurrentTime)
{
#error you must implement RalTimeGetCurrent
}')dnl
define(`USE_TIME_GETTICKS',`ruint32 RalTimeGetTicks(void)
{
#error you must implement RalTimeGetTicks
}')dnl
define(`USE_TIME_SETTIMER',`bool RalTimeSetTimer(ruint32 inMilliseconds, int id)
{
#error you must implement RalTimeSetTimer
}')dnl
define(`USE_TIME_KILLTIMER',`bool RalTimeKillTimer(int id)
{
#error you must implement RalTimeKillTimer
}')dnl
define(`USE_EVENT_GET',`/** Get event.
*
* @param outMsg 
* @param inTimeoutMs 
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalEventGet(pmsg outMsg, ruint32 inTimeoutMs)
{
#error you must implement RalEventGet
}')dnl
define(`USE_HOSTTODEV',`/** Convert host to dev.
*
* @param src 
* @param dev 
*
* @return converted value
*/
void HostToDev(unsigned int src, unsigned char * dev)
{
#error you must implement HostToDev
}')dnl
define(`USE_DEVTOHOST',`/** Convert dev to host.
*
* @param dev 
*
* @return converted value
*/
unsigned int DevToHost(unsigned char * dev)
{
#error you must implement DevToHost
}')dnl
define(`USE_DEVTOHOSTSHORT',`/** Convert dev to host short.
*
* @param dev 
*
* @return converted value
*/
unsigned short DevToHostShort(unsigned char * dev)
{
#error you must implement DevToHostShort
}')dnl
define(`USE_SCREEN_FLUSH',`/** Flush screen,
*
* @return On success, return TRUE; otherwise return FALSE
*/
bool RalScreenFlush(void)
{
#error you must implement RalScreenFlush
}')dnl
define(`USE_SCREEN_FLUSHPART',`bool RalScreenFlushPart(rect* inBoundary)
{
#error you must implement RalScreenFlushPart
}')dnl
define(`USE_LOCK_INIT',`/**
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
#error you must implement RalLckInit
}')dnl
define(`USE_LOCK_LOCK',`/**
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
#error you must implement RalLckLock
}')dnl
define(`USE_LOCK_UNLOCK',`/** 
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
#error you must implement RalLckUnlock
}')dnl
define(`USE_SYSTEMFEATURE',`/** 
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
#error you must implement RalGetSystemFeature
}')dnl 
