/*
 * $Id: rexy_string.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Device dependent한 함수 및 상수가 상수가 정의되어 있다.
 *
 */

#ifndef __REXY_STRING_H__
#define __REXY_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */

    char*	StrCpy(char* dest, const char* src);
	char*	StrNCpy(char* dest, const char* src,unsigned int count);
    int		StrLen(const char *s);
    int		StrCmp(const char *string1, const char *string2);
    int		StrNCmp(const char *string1, const char *string2, unsigned int count);
	int		StrCaselessCmp(const char *string1, const char *string2);
	char*	StrCat(char* dst, const char* src);
	char*	StrNCat(char* dst, const char* src, size_t count);
	char*	StrChr(const char* src, int nChar);
	char*	StrDup(const char* s);

	int		StrPrintF(char *buf, const char *format , ...);
	char*	StrIToA(int nValue, char* pszResult);
	int		StrAToI(char *pszAsc);

    int		StrGetBreakPos(char *str, int str_len, int width);
    int		StrGetWordBreakPos(char *str, int str_len, int width, int start_pos);

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_STRING_H__
