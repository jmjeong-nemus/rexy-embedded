/*
 * $Id: rexy_font.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Device dependent한 함수 및 상수가 상수가 정의되어 있다.
 *
 */

#ifndef __REXY_FONT_H__
#define __REXY_FONT_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @file */
#define MAX_FONT_RESOURCE_NUM 100

/* font type */
typedef struct FntTypeTag {
    int minCode;		/* the minimal code value that font data contains.. */
    int maxCode;		/* the maximal code value that font data contains.. */
    int width;		/* the width of font image */
    int height;		/* the height of font image */
    int byteWidth;	/* the length of byte for a single line */
    const ruint8* widthTable;	/* size of width table of data chunk */
    const void* img;		/* data chunk */
} FntType;

typedef int FntID;


/* font context */
typedef struct FntContextTypeTag {
    COLORVAL crFg;
    COLORVAL crBg;
    FntID fntID;
} FntContextType;

extern FntContextType FntGlobalContext;

int		FntGetFontWidth(FntContextType* cxt, WORD code);
int		FntGetTextHeight(FntContextType* cxt);
int		FntGetByteWidth(FntContextType* cxt);

int		FntGetTextWidth(FntContextType* cxt, char * pStr, int len);
int		FntGetTextWidthEx(FntContextType* cxt, char * pStr, int len, int space);
FntID	FntSetFont(FntContextType* cxt, FntID font);
FntID	FntGetFont(FntContextType* cxt);
unsigned char * FntGetFontImage(FntContextType* cxt, WORD code);
int		FntGetTextLengthFittingInWidth(FntContextType* cxt, char *pStr, int len, int width);
bool	FntIsDBCSLeadByte(byte TestChar);
char*	FntCharNext(const char *lpsz);
char*	FntCharPrev(const char *lpszStart, const char *lpszCurrent);
WORD	FntGetCharCode(const char *lpsz);
UNITCL* FntGetStrImage(FntContextType* cxt, int x, int y, int width, int height, char * str, int len, int space);

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif//__REXY_FONT_H__
