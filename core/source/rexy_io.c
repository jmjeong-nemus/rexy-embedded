/*
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 * This file is a part of Rexy Embedded 
 * Input/Output module for Rexy Embedded
 *
 */
#include "rexy_io.h"
#include "rexy_halinterface.h"

int	    FsOpenFile(const char * file)
{
	return RalOpenFile(file);
}
int     FsReadFile(int fd, char * buffer, int length)
{
	return RalReadFile(fd,buffer,length);
}
int     FsWriteFile(int fd, const char * buffer, int length)
{
	return RalWriteFile(fd,buffer,length);
}
int     FsLseekFile(int fd, int offset, int whence)
{
	return RalLseekFile(fd,offset,whence);
}
int     FsCloseFile(int fd)
{
	return RalCloseFile(fd);
}
bool    FsFileExists(const char* szFileName)
{
	return RalFileExists(szFileName);
}
int		FsMakeDir(const char* szDirName)
{
	return RalMakeDir(szDirName);
}
int     FsCreateFile(const char* szFileName)
{
	return RalCreateFile(szFileName);
}
int    FsDeleteFile(const char* szFileName)
{
	return RalDeleteFile(szFileName);
}
bool	FsShrinkFile(int fd, int new_size)
{
	return RalShrinkFile(fd,new_size);
}
int		FsChangeDir(const char *filepath)
{
	return RalChDir(filepath);
}

long	FsGetFileLength(int fd)
{
	return RalGetFileLength(fd);
}
