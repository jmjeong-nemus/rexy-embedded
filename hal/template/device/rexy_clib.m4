include(rexy_hal.m4) dnl
dnl
dnl =============================================
dnl Start generation
dnl
copyright
doxygen_start

dnl header files
#include "rexy.h"
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
*/

dnl ============================
dnl Dynamic memory allocation
dnl Available parameter: <func_name>
USE_MALLOC(malloc)
USE_CALLOC(calloc)
USE_REALLOC(realloc)
USE_FREE(none)
dnl
dnl ============================
dnl Memory manipulation
dnl Available parameter: <func_name>
USE_MEMCPY(memcpy)
USE_MEMSET(memset)
dnl available parameter: ZeroMemory, memset, none
USE_ZEROMEMORY(ZeroMemory)
dnl
dnl ============================
dnl String manipulation
dnl Available parameter: <func_name>
USE_STRDUP(strdup)
USE_STRCPY(strcpy)
USE_STRNCPY(strncpy)
USE_STRCMP(strcmp)
USE_STRNCMP(strncmp)
USE_STRLEN(strlen)
USE_STRCAT(strcat)
USE_STRNCAT(strncat)
USE_STRCHR(strchr)
USE_ITOA(itoa)
dnl
dnl ============================
dnl File manipulation
dnl Available parameter: `', <func_name>, none
dnl USE_OPENFILE
dnl USE_READFILE
dnl USE_WRITEFILE
dnl USE_LSEEKFILE(lseek)
dnl USE_CLOSEFILE(close)
dnl USE_FILEEXISTS(none)
dnl USE_MAKEDIR(mkdir)
dnl USE_CREATEFILE(create)
dnl USE_DELETEFILE(delete)
dnl USE_SHRINKFILE(shrink)
dnl
dnl ============================
dnl Debug support
dnl Available parameter: `', <func_name>, none
USE_SPRINTF(sprintf)
USE_DEBUGMESSAGE(printf)
USE_DEBUGBREAK(none)
USE_TRACE(trace)
dnl
dnl ============================
dnl Time
dnl Available parameter: <func_name>, none, internal
USE_MKTIME(mktime)
USE_LOCALTIME(internal)
dnl
doxygen_end
