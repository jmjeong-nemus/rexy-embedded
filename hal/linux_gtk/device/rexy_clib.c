/*
 * $Id: 
 * Copyright(c) 2000-5, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy 
 *
 */
#include "rexy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>

int RalSPrintf(char *buf, const char *format , ...);


///*
// *  @defgroup stdlib C Standard Libraries
// *  C Standard Libraries Set
// *  @ingroup clib
// *  @{
// */

/** @addtogroup util
 *  @{
 */

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
    char* ptmp;

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



/** Output debugging messages.
 * 
 * @param msg debugging message
 *
 *	@return none
 */
void RalDebugMessage( const char* msg )
{
    printf(msg);
    printf("\n");
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

void* RalMemCopy(void* dst, const void* src, unsigned int size)
{
    memcpy(dst, src, size);
    return dst;
}

/** Opens a file and return the file handle.
* This functions opens a file with read and write permission. 
* @param file	the file name to be opened.
* @return the file handle. If an error occurs, the return value is less than zero. 
* 
*/
int	RalOpenFile(const char * file)
{
     // on linux system, O_BINARY is not defined.
     // it's zero unlike windows 0x8000
     // commented by Joshua
     return open(file, 0 /*O_BINARY*/ | O_RDWR);
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
    return read( fd, buffer, length );
}

/**  Write data to the file of the file handle.
* @param fd		the file handle
* @param buffer	the buffer which has data to be stored
* @param length	length to be written.
* @return the number of bytes written.
*/
int RalWriteFile(int fd, const char * buffer, int length)
{
    return write( fd, buffer, length );
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
    return lseek( fd, offset, whence );
}

/** Close the file indicated by the file handle.
* @param fd		the file handle.
* @return  Return 0 if the file was successfully closed. 
*		    A return value of -1 indicates an error.
*/
int RalCloseFile(int fd)
{
    return close(fd);
}

/** Check if the file exists
* 
* @param szFileName the file name
* 
* @return TRUE if file exist, FALSE if the file does not exist
*/
bool RalFileExists(const char* szFileName)
{
    int hFile;

    if ( !szFileName ) return FALSE;

    hFile = creat( szFileName, S_IREAD );
    if ( hFile == -1 )
    {
        //ASSERT(GetLastError()==ERROR_FILE_NOT_FOUND);
        return FALSE;
    }
    close(hFile);
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
    return mkdir(szDirName, 777);
}


/** Create a file with read and write permission
* 
* @param szFileName the file name
* 
* @return File handle. On error, return -1
*/
int RalCreateFile(const char* szFileName)
{
    return creat( szFileName, S_IREAD | S_IWRITE );
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

/** @} */
