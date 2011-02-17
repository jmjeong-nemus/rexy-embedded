/*
 * $Id: 
 * Copyright(c) 2000-5, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy 
 *
 */
#ifndef __REXY_CLIB_H__
#define	__REXY_CLIB_H__

extern void*	RalMalloc(unsigned int size);
extern void*	RalCalloc(unsigned int num, unsigned int size);		//chemistz, 2005-02-03
extern void* 	RalRealloc(void* ptr, size_t size);			//chemistz, 2005-04-07
extern void	RalFree(void*);

extern void*	RalMemCopy(void*, const void*, unsigned int);
extern void*	RalMemSet(void*, int, unsigned int);
extern void	RalZeroMemory(void* dest, unsigned int size);

extern char*	RalStrDup(const char*);
extern char*	RalStrCpy(char*, const char*);
extern char*	RalStrNCpy(char* dest, const char* src,unsigned int count);
extern int	RalStrCmp(const char *string1, const char *string2);
extern int      RalStrLen(const char *s);
extern int	RalStrNCmp(const char *string1, const char *string2,unsigned int count);
extern char*	RalStrCat(char* dest, const char* src);
extern char  *  RalStrNCat(char * dest, const char * src, size_t count);
extern char*	RalStrChr(const char* pszSource, int nChar );

extern char*	RalItoa(int nValue, char* pszResult);

extern int	   RalOpenFile(const char * file);
extern int      RalReadFile(int fd, char * buffer, int length);
extern int      RalWriteFile(int fd, const char * buffer, int length);
extern int      RalLseekFile(int fd, int offset, int whence);
extern int      RalCloseFile(int fd);
extern bool     RalFileExists(const char* szFileName);
extern int		RalMakeDir(const char* szDirName);
extern int      RalCreateFile(const char* szFileName);
extern bool     RalDeleteFile(const char* szFileName);
extern bool		RalShrinkFile(int fd, int new_size);
extern bool		RalShrink(int fd, int new_size);	// will be deprecated in 23rd release

extern int		RalSPrintf(char *, const char * ,...);
extern void		RalDebugMessage( const char* msg );
extern void		RalDebugBreak(void);
extern int		RalTrace(const char * ,...);

extern unsigned int RalMkTime(const struct tm* time);
extern struct tm* RalLocalTime(unsigned int time);


#endif//__REXY_CLIB_H__
