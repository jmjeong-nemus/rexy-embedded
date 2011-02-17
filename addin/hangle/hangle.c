#include "hangle11172.h"
#include "hangle.h"

extern int StrLen(const char* s);

int BinaryHanCodeSearch(int wordvalue, int nCode)
{	
	int m;
	int l = 0; 
	int h = 11172-1; 
	if(nCode==EUC)
	{
		while (l<=h) 
		{ 
			m = (l+h)/2; 
			if (EUC2ISO[m][0] < (unsigned int)wordvalue) l = m+1; 
			else if (EUC2ISO[m][0] == (unsigned int)wordvalue)
				return EUC2ISO[m][1];	
			else h = m-1; 
		}
	}
	else if(nCode==JOL || nCode==IW || nCode==IPF)
	{	
		while (l<=h) 
		{ 
			m = (l+h)/2; 
			if (HANCODE[m][nCode] < (unsigned int)wordvalue) l = m+1; 
			else if (HANCODE[m][nCode] == (unsigned int)wordvalue)
				return m;		
			else h = m-1; 
		}
	}
	
	return -1;
}

int HanCodeConvert(unsigned int fromvalue, unsigned int * tovalue, int nFromCode,int nToCode)
{
	int index = -1;
	if(nFromCode==EUC)
	{		
		if(fromvalue>=EUC_START_CODE && fromvalue<=EUC_END_CODE) 			
		{
			index = BinaryHanCodeSearch(fromvalue, nFromCode);
		}
	}
	else if(nFromCode==JOL)
	{
		if(fromvalue>=JOL_START_CODE && fromvalue<=JOL_END_CODE) 			
		{
			index = BinaryHanCodeSearch(fromvalue, nFromCode);
	}
	else if(nFromCode==IW)
	{
		}
		if(fromvalue>=IW_START_CODE && fromvalue<=IW_END_CODE) 			
		{
			index = BinaryHanCodeSearch(fromvalue, nFromCode);			
		}
	}
	if(index>=0)
	{
		if(nToCode!=IPF)	// 목적코드가 첫가끝 조합형이 아닐때 
		{
			*tovalue = HANCODE[index][nToCode];
		}		
		else
		{
			int i;
			for(i=0;(unsigned int)i<HANCODE[index][IPFSIZE];i++)
			{
				*tovalue = HANCODE[index][nToCode+i+1];				
			}
		}
		return 1;
	}
	return 0;
}
 
int UTF8toUCS2(unsigned char *utf8, unsigned int *ucs2)
{
    unsigned char c;
    unsigned int unicode;
    int count=0;
    int len;
    int i;

    len = StrLen(utf8);
    for (i=0; i<len;) 
	{
		 c = utf8[i]&0xe0;
		 if (c < 0x80) 
	 	 {
		     unicode = (unsigned short) utf8[i];
		     i++;
		 }
		 else if (c < 0xe0) 
	 	 {
		     unicode = (unsigned short) utf8[i] & 0x1f;
		     i++;
		     unicode = unicode << 6;
		     unicode = unicode | ((unsigned short) utf8[i] & 0x3f);
		     i++;
		 }
		 else if (c < 0xf0) 
	 	 {
		     unicode = (unsigned short) utf8[i] & 0x0f;
		     i++;
		     unicode = unicode << 6;
		     unicode = unicode | ((unsigned short) utf8[i] & 0x3f);
		     i++;
		     unicode = unicode << 6;
		     unicode = unicode | ((unsigned short) utf8[i] & 0x3f);
		     i++;
 		 }
		 ucs2[count] = unicode;
		 count++;
    }
    return count;
}