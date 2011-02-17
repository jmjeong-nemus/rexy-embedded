/*
 * $Id: rexy_string.c 9002 2005-11-03 03:32:48Z madholic $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */

/** @file */

/**
* @defgroup string Core String Service
* String APIs
* @ingroup RexyECoreGroup
* @{
*/

#include "rexy.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/**
* @brief		Convert an integer to a string.
* @param[in]	nValue		number to be converted
* @param[out]	pszResult	string result
* @return		Each of these functions returns a pointer to string. There is no error return.
* @remarks		Wrapper function for itoa() in the standard C library. 
*				But do not support radix conversion(just support for decimal number)
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
char*	StrIToA(int nValue, char* pszResult)
{
    StrPrintF(pszResult, "%d", nValue);
    return pszResult;
}
/**
* @brief		Convert an string to an integer value.
* @remarks		Wrapper function for atoi() in the standard C library. 
* @see			
* @if HISTORY
*				18-Sep-2006	Joshua created
* @endif
*/
int	StrAToI(char *pszAsc)
{
	int Len;

	Len = StrLen(pszAsc);
	
	if ( Len > 2 )
	{
		int j,k;
		int result=0;
		char CharDigit;
		int	BaseDigit;

		if ( pszAsc[0] == '0' && ( pszAsc[1] == 'x' || pszAsc[1] == 'X' ) )
		{
			for ( j=0; j<Len-2; j++ )
			{
				CharDigit = pszAsc[j+2];

				if ( CharDigit >= '0' && CharDigit <= '9') BaseDigit = CharDigit - '0';
				else if ( CharDigit >= 'a' && CharDigit <= 'f') BaseDigit = CharDigit - 'a' + 10;
				else if ( CharDigit >= 'A' && CharDigit <= 'F') BaseDigit = CharDigit - 'A' + 10;
				for ( k = 1; k < Len-2-j ; k++ ) BaseDigit *= 16;

				result += BaseDigit;
			}
			return result;
		}
	}
	return RalStrAToI(pszAsc);
}
/**
* @brief		Write formatted data to a string.
* @param[out]	buf			Storage location for output
* @param[in]	format		Format-control string
* @param[in]	...			Optional arguments
* @return		the number of characters written.
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int StrPrintF(char *buf, const char *format , ...)
{
    int ret;
    va_list arg;
    va_start( arg, format );
    ret = vsprintf( buf, format, arg );
    va_end( arg );
    return ret;
}

/**
* @brief		Finds the length in bytes of the characters from a specified string that fit within a passed width.
* @param[in]	str			Character string.
* @param[in]	str_len		Length in bytes of character strings.
* @param[in]	width		Maximum width to allow.
* @return		Returns the number of bytes of text that can appear within the width.
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int StrGetBreakPos(char *str, int str_len, int width)
{
    int str_width;
    int nPos=0;

    str_width=0;
    for (nPos = 0; nPos < str_len; nPos++)
    {
        {
            str_width += FntGetFontWidth(&FntGlobalContext, (byte)*str);
            if (str_width > width)
                break;
            str++;
        }
    }

    return nPos;
}

/**
* @brief		Finds the number of words from a specified string that fit within a passed width from a specified start position.
* @param[in]	str			Character string.
* @param[in]	str_len		Length in bytes of character strings.
* @param[in]	width		Maximum width to allow.
* @param[in]	start_pos	Starting point of string calculation.
* @return		Returns number of words available in specifide width.
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int StrGetWordBreakPos(char *str, int str_len, int width, int start_pos)
{
    int str_width;
    int i;
    int word_break=0;

    str_width=0;
    word_break=-1;

    for(i=start_pos; i<str_len; i++) {
        str_width+=FntGetFontWidth(&FntGlobalContext, (unsigned char)*str);
        if( *str==' ' ) word_break=i+1;

        if( str_width>width ) break;
        str++;
    }

    if( i==str_len || word_break<0 )
        word_break=i;

    return word_break;
}

/**
* @brief		Duplicate string.
* @param[in]	s Null-terminated source string
* @return		returns a pointer to the storage location for the copied string or NULL if storage cannot be allocated.
* @remarks		Wrapper function for strdup() in the standard C library.
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
char* StrDup(const char* s)
{
	return RalStrDup(s);
}

/**
* @brief		Copy a string.
* @param[out]	dest	Destination string
* @param[in]	src		Null-terminated source string
* @return		the value of dest
* @remarks		Wrapper function for the strcpy() in the standard C library.
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
char* StrCpy(char* dest, const char* src)
{
    return RalStrCpy( dest, src );
}

/**
* @brief		Copy N characters of the source string to the destination string.
* @param[out]	dest	Destination string
* @param[in]	src		Source string
* @param[in]	count	Number of characters to be copied
* @return		
* @remarks		Wrapper function for the strncpy() in the standard C library
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
char* StrNCpy(char* dest, const char* src,unsigned int count)
{
    return RalStrNCpy( dest, src, count );
}

/**
* @brief		Calculates the length of the string s, not including the terminating '\\0' character.
* @param[in]	s source string pointer
* @return		the byte length of string s
* @remarks		Wrapper function for strlen() in the standard C library. 
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int StrLen(const char *s)
{
	if ( !s ) return 0;
    return RalStrLen(s);
}

/**
* @anchor		StrCmp
* @brief		Compare strings.
* @param[in]	string1 Null-terminated strings to compare
* @param[in]	string2 Null-terminated strings to compare
* @return		The return value indicates the lexicographic relation of string1 to string2.
*				returns a value less than zero if string1 is less than string2. returns zero 
*				if two strings are equivalent. returns a value greater than zero if string1 is 
*				greater than string2
* @remarks		Wrapper function for the strcmp() in the standard C library. 
* @see			StrCaselessCmp
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int StrCmp(const char *string1, const char *string2)
{
    return RalStrCmp(string1,string2);
}

/**
* @anchor		StrCaselessCmp
* @brief		Compare strings without case sensitive
* @param[in]	string1 Null-terminated strings to compare
* @param[in]	string2 Null-terminated strings to compare
* @return		The return value indicates the lexicographic relation of string1 to string2.
*				returns a value less than zero if string1 is less than string2. returns zero 
*				if two strings are equivalent. returns a value greater than zero if string1 is 
*				greater than string2. 
* @remarks		Note that this API is different from 
*				StrCmp in that this won't consider capital case
* @see			StrCmp
* @if HISTORY
*				19-Sep-2006 Joshua Created
* @endif
*/
int StrCaselessCmp(const char *string1, const char *string2)
{
	int i,j;
	int	Len[2];
	char *SourceS[2];
	char *ConvertedS[2];
	int	 result;

	Len[0] = StrLen(string1);
	Len[1] = StrLen(string2);

	SourceS[0] = (char *)string1;
	SourceS[1] = (char *)string2;

	for ( i=0; i< 2; i++ )
	{
		ConvertedS[i] = RalMemAlloc(Len[i]+1);
		
		for ( j = 0; j <= Len[i]; j++ )
		{
			if ( *(SourceS[i]+j)>='a' && *(SourceS[i]+j)<='z' )
				*(ConvertedS[i]+j)=*(SourceS[i]+j) - 'a' + 'A';
			else
				*(ConvertedS[i]+j)=*(SourceS[i]+j);
		}
	}

	result = StrCmp(ConvertedS[0],ConvertedS[1]);

	RalMemFree(ConvertedS[0]);
	RalMemFree(ConvertedS[1]);

	return result;
}

/**
* @brief		Compare characters of two strings.
* @param[in]	string1		Strings to compare
* @param[in]	string2		Strings to compare
* @param[in]	count		Number of characters to compare
* @retval		< 0 string1		substring less than string2 substring 
* @retval		0   string1		substring identical to string2 substring 
* @retval		> 0 string1		substring greater than string2 substring 
* @remarks		Wrapper function for the strncmp() in the standard C library. 
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int	StrNCmp(const char *string1, const char *string2, unsigned int count)
{
	return RalStrNCmp(string1, string2, count);
}

/**
* @brief		Append a string.
* @param[out]	dst		Null-terminated destination string
* @param[in]	src		Null-terminated source string
* @return		returns the destination string 
* @remarks		Wrapper function for the strcat() in the standard C library. 
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
char* StrCat(char* dst, const char* src)
{
	return RalStrCat(dst, src);
}

/**
* @brief		Append characters of a string.
* @param[out]	dst		Null-terminated destination string
* @param[in]	src		Null-terminated source string
* @param[in]	count	Number of characters to append
* @return		returns a pointer to the destination string
* @remarks		Wrapper function for the strncat() in the standard C library. 
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
char* StrNCat(char* dst, const char* src, size_t count)
{
	return RalStrNCat(dst, src, count);
}

/**
* @brief		Find a character in a string.
* @param[in]	src		Null-terminated source string
* @param[in]	nChar	Character to be located
* @return		returns a pointer to the first occurrence of nChar in string, or NULL if nChar is not found
* @remarks		Wrapper function for the strchr() in the standard C library. 
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
char* StrChr(const char* src, int nChar)
{
	return RalStrChr(src, nChar);
}

/** @} */ // End of String
