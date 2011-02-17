/*
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Input/Output module for Rexy/E
 *
 */
#ifndef __REXY_IO_H__
#define __REXY_IO_H__

#include "rexy_type.h"
/* Seek method constants */

#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0

extern int FsOpenFile(const char * file);
extern int FsReadFile(int fd, char * buffer, int length);
extern int FsWriteFile(int fd, const char * buffer, int length);
extern int FsLseekFile(int fd, int offset, int whence);
extern int FsCloseFile(int fd);
extern bool FsFileExists(const char* szFileName);
extern int	FsMakeDir(const char* szDirName);
extern int  FsCreateFile(const char* szFileName);
extern int FsDeleteFile(const char* szFileName);
extern bool	FsShrinkFile(int fd, int new_size);
extern bool	FsShrink(int fd, int new_size);
extern long 	FsGetFileLength(int fd);

#endif
