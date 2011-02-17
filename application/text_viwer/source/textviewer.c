/* 
* $Id: textviewer.c 491 2006-04-05 00:26:34Z madholic $
* Copyright(c) 2004-5, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy
*   
*/
#include "../header/textviewer.h"
#include "../../addin/hangle/hangle.h"
#include "rexy_ctl_popup.h"
//#include "registry.h"
#include "application.h"

#define SCR_SIZE_WIDTH txvscreenWidth
#define SCR_SIZE_HEIGHT txvscreenHeight

#define MAX_LST_NUMBER (txvscreenHeight/20)
#define MAX_CHAR_PER_LINE (txvscreenWidth/9)
#define MAX_TXT_BUFFER (MAX_CHAR_PER_LINE*MAX_LST_NUMBER)

#define MAX_ONE_LINE_WIDTH (MAX_CHAR_PER_LINE*8)

#define LIST_TEXT_X 10
#define LIST_TEXT_Y 10
#define LIST_TEXT_SPACE 15

#define LIST_TEXT_FOCUS_X 10
#define LIST_TEXT_FOCUS_Y  23
#define LIST_TEXT_FOCUS_LINE_SPACE 15

#define FOCUS_LINE_LENGTH 100

#define SCROLL_BAR_RECT_X	(txvscreenWidth -20)
#define SCROLL_BAR_RECT_Y	LIST_TEXT_Y
#define SCROLL_BAR_RECT_WIDTH	(SCROLL_BAR_RECT_X +10)
#define SCROLL_BAR_RECT_HEIGHT	 (txvscreenHeight -30)

#define FILE_NAME_WIDTH_MAX (txvscreenWidth -105)

#define MAX_LINE_NUM 20
#define MAX_FILE_NAME 212

#define TEXTVIEWER_PAGE_SCROLL_TIMER 3323
#define TEXTVIEWER_LINE_SCROLL_TIMER 3332
/*
#define TEXT_UNICODE_LITTLE		0
#define TEXT_UNICODE_BIG		1
#define TEXT_ANSI				2
#define TEXT_UTF8				3
  */

static rint32 txvscreenWidth;
static rint32 txvscreenHeight;

static rint32 txvpageOffSetChar=0;

static rint32 txvtotalFileNum;
static rint32 txvStateID;

unsigned char textList [MAX_LINE_NUM][MAX_FILE_NAME];
unsigned long textListIdentifier[MAX_LINE_NUM];

static rint32 txvfocusedList;
static rint32 txvlistCount;

static rint32 txvcurrentPage;
static rint32 txvlineNumber;
static rint32 txvtotalPage;
static bool txvrunningTimer;

static rint32 txvfileLocationPointer;
static rint32 txvpreFileLocationPointer;

static unsigned char * txvtextBuffer;

static rint32 txvpopUpId;

static WND gpWND = NULL;
static WND txvgptextViewerCtl = 0;

static rint32 txvbookMarkPage; //file save
//static unsigned char bookmakedFileName[10]; 
static unsigned char bookmakedFileName[MAX_FILE_NAME];
static bool txvbookCheck;//file save
static rint32 txvpageScrollTime;//file save
static rint32 txvlineScrollTime;//file save
static rint32 txvfocusedElement;//0->book mark,1->page scroll,2->line scroll

static rint32 txvmode;

static rint32 PrvTxvListHandler(WND wnd, rint32 msg, rint32 wparam, rint32 lparam);
static rint32 PrvTxvViewHandler(WND wnd, rint32 msg, rint32 wparam, rint32 lparam);
static rint32 PrvTxvSettingHandler(WND wnd, rint32 msg, rint32 wparam, rint32 lparam);
static unsigned short PrvTxvGetCharCode(unsigned char *buf, rint32 index, rint32 *count, AppFontIDType *FntId);
static void PrvTxvViewPage(WND hWnd);

extern rint32 GdiTextOutW(WND, ruint16, ruint16, ruint16 *, rint32);

static TxvFormHandle g_TxvFormHandleList[] =
{
	TEXTVIEW_STATE_FILE_LIST,			PrvTxvListHandler,
	TEXTVIEW_STATE_FILE_SHOW,			PrvTxvViewHandler,
	TEXTVIEW_STATE_SETTING,			PrvTxvSettingHandler
};


static void PrvTxvPopTextViewerDrawPostCB(WND pCtl)
{
	txvpopUpId=1;
	GdiTextOut(pCtl,"end of file", StrLen("end of file"), 30, 30);
}

static void PrvTxvPopTextViewerDrawPostCB2(WND pCtl)
{
	txvpopUpId=2;
	GdiTextOut(pCtl,"start of file", StrLen("start of file"), 30, 30);
}

static void PrvTxvPopTextViewerDrawPostCB3(WND pCtl)
{
	txvpopUpId=3;
	GdiTextOut(pCtl,"Sucessfully Saved", StrLen("Sucessfully Saved"), 30, 30);
}

static void PrvTxvPopTextViewerDrawPostCB4(WND pCtl)
{
	char tmp[4];
	MemSet(tmp,0,4);
	txvpopUpId=4;
	GdiTextOut(pCtl,"Page", StrLen("Page"), 30, 30);
	StrIToA(txvcurrentPage, tmp);
	GdiTextOut(pCtl,tmp, StrLen(tmp), 80, 30);
	GdiTextOut(pCtl,"Bookmarked", StrLen("Bookmarked"), 30, 50);
}

static void PrvTxvPopTextViewerDrawPostCB5(WND pCtl)
{
	txvpopUpId=5;
	GdiTextOut(pCtl,"can't read this file", StrLen("can't read this file"), 30, 30);
}


static rint32 PrvTxvPopTextViewerCB(WND pCtl, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr)
{
	switch(nMsg) {
	case AWM_PAINT:
		if(!bIsPre)
			PrvTxvPopTextViewerDrawPostCB(pCtl);
		break;
	}
	return 0;
}

static rint32 PrvTxvPopTextViewerCB2(WND pCtl, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr)
{
	switch(nMsg) {
	case AWM_PAINT:
		if(!bIsPre)
			PrvTxvPopTextViewerDrawPostCB2(pCtl);
		break;
	}
	return 0;
}

static rint32 PrvTxvPopTextViewerCB3(WND pCtl, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr)
{
	switch(nMsg) {
	case AWM_PAINT:
		if(!bIsPre)
			PrvTxvPopTextViewerDrawPostCB3(pCtl);
		break;
	}
	return 0;
}

static rint32 PrvTxvPopTextViewerCB4(WND pCtl, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr)
{
	switch(nMsg) {
	case AWM_PAINT:
		if(!bIsPre)
			PrvTxvPopTextViewerDrawPostCB4(pCtl);
		break;
	}
	return 0;
}

static rint32 PrvTxvPopTextViewerCB5(WND pCtl, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr)
{
	switch(nMsg) {
	case AWM_PAINT:
		if(!bIsPre)
			PrvTxvPopTextViewerDrawPostCB5(pCtl);
		break;
	}
	return 0;
}


static void PrvTxvCreateTextViewerPop(/*REXY_CTL *gpCtl,*/rint32 (* TextViewerCB)(WND pCtl, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr),rint32 time)
{
	rint32 err;
    txvgptextViewerCtl = CtlCreate(gpWND, CTL_TYPE_POPUP, NULL, TextViewerCB, &err);
	if(time>0)
	{
		//CtlSeti(txvgptextViewerCtl, CTL_POPUP_CMD_SET_TIMEOUT, time, &err);
	}
	CtlDraw(txvgptextViewerCtl, &err);
	//CtlDestroy(txvgptextViewerCtl, &err);
	//txvgptextViewerCtl = 0;
}

static void PrvTxvCloseTextViewerPop(/*REXY_CTL *gpCtl*/)
{
	rint32 err;
	if(txvgptextViewerCtl != 0)
	{
		CtlDestroy(txvgptextViewerCtl, &err);
		txvgptextViewerCtl = 0;
	}
}

static rint32 PrvTxvUTF8toUCS2(char *src, unsigned short *ucs2) // little endian 
{
    unsigned char c;
    unsigned short unicode;
    rint32 count=0;
    rint32 len;
    rint32 i;

    len = StrLen(src);

    for (i=0; i<len;) 
	{
		 c = src[i]&0xe0;
		 if (c < 0x80) 
	 	 {
		     unicode = (unsigned short) src[i];
		     i++;
		 }
		 else if (c < 0xe0) 
	 	 {
		     unicode = (unsigned short) src[i] & 0x1f;
		     i++;
		     unicode = unicode << 6;
		     unicode = unicode | ((unsigned short) src[i] & 0x3f);
		     i++;
		 }
		 else if (c < 0xf0) 
	 	 {
		     unicode = (unsigned short) src[i] & 0x0f;
		     i++;
		     unicode = unicode << 6;
		     unicode = unicode | ((unsigned short) src[i] & 0x3f);
		     i++;
		     unicode = unicode << 6;
		     unicode = unicode | ((unsigned short) src[i] & 0x3f);
		     i++;
 		 }
		 ucs2[count] = unicode;
		 count++;
    }
    return count;
}

static rint32 PrvTxvGetText(rint32 fileLocationPointer1)
{
	rint32 stream;

	if (txvtextBuffer == NULL)
	{
		if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG)
		{
			txvtextBuffer=MemAlloc(MAX_TXT_BUFFER*2);
			MemSet(txvtextBuffer,0,MAX_TXT_BUFFER*2);
		}
		else 
		{
			txvtextBuffer=MemAlloc(MAX_TXT_BUFFER*2);
			MemSet(txvtextBuffer,0,MAX_TXT_BUFFER*2);
		}
	}
	MemSet(txvtextBuffer,0,MAX_TXT_BUFFER*2);
	if((stream = FsOpenFile((char *)&textList[txvfocusedList])) == 0)
	{
		return 0;
	}
	else
	{
		if(FsLseekFile(stream,fileLocationPointer1,0) <0)
		{
			return 0;
		}
		if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG)
			FsReadFile(stream,txvtextBuffer,MAX_TXT_BUFFER*2);
		else
			FsReadFile(stream,txvtextBuffer,MAX_TXT_BUFFER);
		FsCloseFile(stream);
		return 1;
	}
}

static rint32 PrvTxvGetTotalPageNumber()
{
	rint32 txvlineNumber=0;
	rint32 lineCount=0;
	rint32 width =0;
	unsigned short value;
	FntID PrevFntID = 0;
	rint32 i=0, j=0, count;
	FntID CurrentFntID = 0;
	
	count = 0;
	PrevFntID = FntGetFont(&FntGlobalContext);
	width = 0;
	while(PrvTxvGetText(j*MAX_TXT_BUFFER) != 0 && (txvtextBuffer[i] !=0 ||txvtextBuffer[i+1] !=0 || txvtextBuffer[i+2] !=0))//파일이 있을때까지..
	{
		while(i<MAX_TXT_BUFFER && (txvtextBuffer[i] !=0 ||txvtextBuffer[i+1] !=0 || txvtextBuffer[i+2] !=0))
		{
			value = PrvTxvGetCharCode(txvtextBuffer, txvpageOffSetChar+i, &count, &CurrentFntID);
			if (value == 0x0d)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 4;
				else i = i + 2;
				txvlineNumber++;
				width = 0;
				continue;
			}
			else if (value == 0x0a)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 2;
				else i = i + 1;
				txvlineNumber++;
				width = 0;
				continue;
			} 
			else if (txvmode == TEXT_UTF8 && txvtextBuffer[txvpageOffSetChar+i] == 0xEF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xBB \
				&& txvtextBuffer[txvpageOffSetChar+i+2] == 0xBF )
			{
				i = i + 3;
				continue;
			} 
			else if (txvmode == TEXT_UNICODE_LITTLE && txvtextBuffer[txvpageOffSetChar+i] == 0xFF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFE )
			{
				i = i + 2;
				continue;
			}
			else if (txvmode == TEXT_UNICODE_BIG && txvtextBuffer[txvpageOffSetChar+i] == 0xFE && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFF )
			{
				i = i + 2;
				continue;
			}

			FntSetFont(&FntGlobalContext, CurrentFntID);
			width = width + FntGetTextWidth(&FntGlobalContext,(char *)&value,1); // 1 Char Width

			FntSetFont(&FntGlobalContext, PrevFntID);
			i = i + count;
			if(width >= MAX_ONE_LINE_WIDTH)
			{
				width = 0;
				txvlineNumber++;
			}

		}
		j++;
		i = 0;

	}
	if(txvlineNumber%MAX_LST_NUMBER > 0)
	{
		return txvlineNumber/MAX_LST_NUMBER+1;
	}
	else if(txvlineNumber <MAX_LST_NUMBER)
	{
		return 1;
	}
	else
	{
		return txvlineNumber/MAX_LST_NUMBER;
	}
}

static rint32 PrvTxvStrToInt(char * ch,rint32 i)
{
	rint32 result=0;
	i--;
	for(;i>=0 ;i--)
	{
		if(ch[i] != 0)
			result = result*10+(ch[i] - '0');
	}
	return result;
}

static rint32 PrvTxvPageBack()
{
	rint32 p;
	rint32 j=0;
	rint32 k=0;
	rint32 i1=0;
	rint32 width=0;
	FntID CurrentFntID = 0, PrevFntID = 0;
	rint32 i = 0, count = 0;
	unsigned short value;

	if(PrvTxvGetText(0) ==0)
		return 0;

	PrevFntID = FntGetFont(&FntGlobalContext);
	for(p=1;p<txvcurrentPage;p++)
	{
		j = 0;
		i = 0;
		while(j < MAX_LST_NUMBER && i<MAX_TXT_BUFFER && (txvtextBuffer[i] !=0 ||txvtextBuffer[i+1] !=0 || txvtextBuffer[i+2] !=0))
		{
			value = PrvTxvGetCharCode(txvtextBuffer, txvpageOffSetChar+i, &count, &CurrentFntID);
			if (value == 0x0d)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 4;
				else i = i + 2;
				j++;
				width = 0;
				continue;
			}
			else if (value == 0x0a)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 2;
				else i = i + 1;
				j++;
				width = 0;
				continue;
			} 
			else if (txvmode == TEXT_UTF8 && txvtextBuffer[txvpageOffSetChar+i] == 0xEF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xBB \
				&& txvtextBuffer[txvpageOffSetChar+i+2] == 0xBF )
			{
				i = i + 3;
				continue;
			} 
			else if (txvmode == TEXT_UNICODE_LITTLE && txvtextBuffer[txvpageOffSetChar+i] == 0xFF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFE )
			{
				i = i + 2;
				continue;
			}
			else if (txvmode == TEXT_UNICODE_BIG && txvtextBuffer[txvpageOffSetChar+i] == 0xFE && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFF )
			{
				i = i + 2;
				continue;
			}

			FntSetFont(&FntGlobalContext, CurrentFntID);
			width = width + FntGetTextWidth(&FntGlobalContext,(char *)&value,1); // 1 Char Width

			FntSetFont(&FntGlobalContext, PrevFntID);
			i = i + count;
			if(width >= MAX_ONE_LINE_WIDTH)
			{
				width = 0;
				j++;
			}

		}

		if(p!= txvcurrentPage)
		{
			txvpreFileLocationPointer= txvfileLocationPointer;//이전의 파일 위치를  저장한다. key up를 위해..
			txvfileLocationPointer=txvfileLocationPointer+i;
		}
		if(PrvTxvGetText(txvfileLocationPointer) ==0)
			return 0;
	}
	return 1;

}

static rint32 PrvTxvSearchBookMark()
{
	rint32 p;
	rint32 j=0;
	rint32 k=0;
	rint32 i1=0;
	rint32 width=0;
	FntID CurrentFntID = 0, PrevFntID = 0;
	rint32 i = 0, count = 0;
	unsigned short value;

	if(txvcurrentPage == 1)
	{
		if(PrvTxvGetText(txvfileLocationPointer) ==0)
			return 0;
		return 0;
	}

	PrevFntID = FntGetFont(&FntGlobalContext);
	for(p=1;p<txvcurrentPage;p++)
	{
		if(PrvTxvGetText(txvfileLocationPointer) ==0)
			return 0;
		j = 0;
		i = 0;
		while(j < MAX_LST_NUMBER && i<MAX_TXT_BUFFER && (txvtextBuffer[i] !=0 ||txvtextBuffer[i+1] !=0 || txvtextBuffer[i+2] !=0))
		{
			value = PrvTxvGetCharCode(txvtextBuffer, txvpageOffSetChar+i, &count, &CurrentFntID);
			if (value == 0x0d)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 4;
				else i = i + 2;
				j++;
				width = 0;
				continue;
			}
			else if (value == 0x0a)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 2;
				else i = i + 1;
				j++;
				width = 0;
				continue;
			} 
			else if (txvmode == TEXT_UTF8 && txvtextBuffer[txvpageOffSetChar+i] == 0xEF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xBB \
				&& txvtextBuffer[txvpageOffSetChar+i+2] == 0xBF )
			{
				i = i + 3;
				continue;
			} 
			else if (txvmode == TEXT_UNICODE_LITTLE && txvtextBuffer[txvpageOffSetChar+i] == 0xFF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFE )
			{
				i = i + 2;
				continue;
			}
			else if (txvmode == TEXT_UNICODE_BIG && txvtextBuffer[txvpageOffSetChar+i] == 0xFE && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFF )
			{
				i = i + 2;
				continue;
			}

			FntSetFont(&FntGlobalContext, CurrentFntID);
			width = width + FntGetTextWidth(&FntGlobalContext,(char *)&value,1); // 1 Char Width

			FntSetFont(&FntGlobalContext, PrevFntID);
			i = i + count;
			if(width >= MAX_ONE_LINE_WIDTH)
			{
				width = 0;
				j++;
			}

		}

		if(p!= txvcurrentPage)
		{
			txvpreFileLocationPointer= txvfileLocationPointer; //이전의 파일 위치를  저장한다. key up를 위해..
			txvfileLocationPointer=txvfileLocationPointer+i;
		}
	}
	return 1;
}

static rint32 PrvTxvGetOneLine()
{
	rint32 i = 0, count = 0, width = 0;
	FntID CurrentFntID = 0, PrevFntID = 0;
	unsigned short value;

	PrevFntID = FntGetFont(&FntGlobalContext);
	while(txvtextBuffer[i] !=0 ||txvtextBuffer[i+1] !=0 || txvtextBuffer[i+2] !=0)
	{
			value = PrvTxvGetCharCode(txvtextBuffer, txvpageOffSetChar+i, &count, &CurrentFntID);
			if (value == 0x0d)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 4;
				else i = i + 2;
				return i;
			}
			else if (value == 0x0a)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 2;
				else i = i + 1;
				return i;
			} 
			else if (txvmode == TEXT_UTF8 && txvtextBuffer[txvpageOffSetChar+i] == 0xEF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xBB \
				&& txvtextBuffer[txvpageOffSetChar+i+2] == 0xBF )
			{
				i = i + 3;
				continue;
			} 
			else if (txvmode == TEXT_UNICODE_LITTLE && txvtextBuffer[txvpageOffSetChar+i] == 0xFF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFE )
			{
				i = i + 2;
				continue;
			}
			else if (txvmode == TEXT_UNICODE_BIG && txvtextBuffer[txvpageOffSetChar+i] == 0xFE && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFF )
			{
				i = i + 2;
				continue;
			}

			FntSetFont(&FntGlobalContext, CurrentFntID);
			width = width + FntGetTextWidth(&FntGlobalContext,(char *)&value,1); // 1 Char Width

			FntSetFont(&FntGlobalContext, PrevFntID);
			i = i + count;
			if(width >= MAX_ONE_LINE_WIDTH)
			{
				return i;
			}
	}
	return 0;
}




static rint32 PrvTxvListHandler(WND wnd, rint32 msg, rint32 wparam, rint32 lparam)
{
	int bHandled = FALSE;
	FntID PrevFntID = 1;
	txvStateID = TEXTVIEW_STATE_FILE_LIST;
	switch(msg)
	{
	case AWM_WINCREATE:
		GdiGetScreenInfo(&txvscreenWidth,&txvscreenHeight);
		FsChangeDir(0);
		txvtotalFileNum = RalGetFileTotalNumber();
		txvlistCount =0;
		txvfocusedList=0;
		//RalTextViewerStart(1);
		RalGetFileList((char*)textList,textListIdentifier,MAX_LST_NUMBER,MAX_FILE_NAME,txvlistCount);	
		PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);
		bHandled = TRUE;
		break;

	case AWM_PAINT:
		{
			rint32 i;
			rint32 width=0;
			rint32 j=0;
			unsigned short tmp1,tmp2;
			GdiLockScreen(wnd);
			GdiClrScreen(wnd, COLOR_WHITE);

			for(i=0; i<MAX_LST_NUMBER; i++)
			{
				if(textList[i][0]  != 0)
				{
					if(textListIdentifier[i] & 0x10 /*==1*/ )
						GdiTextOut(wnd, "D",StrLen("D"),(ruint16)(LIST_TEXT_X),(ruint16)(LIST_TEXT_Y+(LIST_TEXT_SPACE*i)));
					else
					{

						GdiTextOut(wnd, "F",StrLen("F"),(ruint16)(LIST_TEXT_X),(ruint16)(LIST_TEXT_Y+(LIST_TEXT_SPACE*i)));
					}
				}
				
				width=0;
				j=0;
				while(textList[i][j] != 0)
				{
					if(textList[i][j]>0x80) // Hangul
					{	
						PrevFntID = FntGetFont(&FntGlobalContext);
						FntSetFont(&FntGlobalContext, FntIDUnicode12Han/*FntIDUnicode16*/);
						tmp1 = textList[i][j];
						tmp1 = tmp1<<8;
						j++;
						tmp1= tmp1 | textList[i][j];
						HanCodeConvert(tmp1, (ruint32 *)&tmp2,EUC,IW);
						tmp2-=0xac00;
					}
					else
					{
						PrevFntID = FntGetFont(&FntGlobalContext);
						FntSetFont(&FntGlobalContext, FntIDUnicode12BL/*FntIDAlpha*/);
						tmp2 =textList[i][j];
					}
					width = width + GdiTextOutW(wnd, (ruint16)(LIST_TEXT_X+15 + width),(ruint16)(LIST_TEXT_Y+(LIST_TEXT_SPACE*i)), &tmp2, 1);//한 글자 식 한다.
					j++;
					FntSetFont(&FntGlobalContext, PrevFntID);

					
					if(width >= FILE_NAME_WIDTH_MAX)
					{
						 GdiTextOut(wnd, "~",StrLen("~"),(ruint16)(LIST_TEXT_X+15 + width),(ruint16)(LIST_TEXT_Y+(LIST_TEXT_SPACE*i)));
						 break;
					}
				}
				if(txvfocusedList == i)
				{
					GdiDrawLine(wnd,LIST_TEXT_X +15, (ruint16)(LIST_TEXT_FOCUS_Y+(txvfocusedList*LIST_TEXT_FOCUS_LINE_SPACE)),(ruint16)(LIST_TEXT_X +15+width) , (ruint16)(LIST_TEXT_FOCUS_Y+(txvfocusedList*LIST_TEXT_FOCUS_LINE_SPACE)));
				}
			}
			// Drawing ScrollBar
			{
				rint32 tmp;
				rect txvrect1; 
				rect txvrect2; 
				if(txvtotalFileNum%MAX_LST_NUMBER)
				{
					tmp = (txvtotalFileNum/MAX_LST_NUMBER) + 1;
				}
				else
				{
					tmp = txvtotalFileNum/MAX_LST_NUMBER;
				}
				txvrect1.bottom=SCROLL_BAR_RECT_HEIGHT;
				txvrect1.left=SCROLL_BAR_RECT_WIDTH;
				txvrect1.right=SCROLL_BAR_RECT_X;
				txvrect1.top=SCROLL_BAR_RECT_Y;

				GdiRectangle(wnd,&txvrect1);

				GdiSetFgColor(wnd,COLOR_RED) ; 

				txvrect2.top=txvrect1.top + (txvlistCount/MAX_LST_NUMBER)*((txvrect1.bottom - txvrect1.top)/tmp);
				txvrect2.bottom=txvrect2.top + (txvrect1.bottom-txvrect1.top)/tmp;
				txvrect2.left=txvrect1.left-2;
				txvrect2.right=txvrect1.right+2;
			
			
				if(txvtotalFileNum == (txvlistCount+1))
				{
					txvrect2.bottom = txvrect1.bottom;
					GdiRectangle(wnd,&txvrect2);
				}
				else
				{
					GdiRectangle(wnd,&txvrect2);
				}
			}
			GdiSetFgColor(wnd,COLOR_BLACK) ;
			// Drawing ScrollBar...

			GdiUnlockScreen(wnd);
		}
		bHandled = TRUE;
		break;
	case AWM_KEYDOWN:
		PrvTxvCloseTextViewerPop();
		switch(wparam) {
		case VK_PAD_0:
			break;

		case VK_PAD_1:
				txvtextBuffer=MemAlloc(MAX_TXT_BUFFER*2);
				MemSet(txvtextBuffer,0,MAX_TXT_BUFFER*2);
				if(textListIdentifier[txvfocusedList] & 0x10 /*==1*/)
				{
					MemFree(txvtextBuffer);
					txvtextBuffer = NULL;
					if(RalChDir((char*)&textList[txvfocusedList]) == 0)
					{
						txvtotalFileNum = RalGetFileTotalNumber();
						txvlistCount =0;
						txvfocusedList=0;
						RalGetFileList((char*)textList,textListIdentifier,MAX_LST_NUMBER,MAX_FILE_NAME,txvlistCount);
						PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);

					}
					else
					{
						//popup message;
					}
				}
				else
				{
					MemFree(txvtextBuffer);
					txvtextBuffer = NULL;

					if(PrvTxvGetText(0) == 0)
					{
						//can not read this file message
						PrvTxvCreateTextViewerPop(PrvTxvPopTextViewerCB5,1000);
					}
					else
					{
						PrvTxvViewHandler(wnd, AWM_WINCREATE, 0, 0);
					}
				}
			break;
		case VK_PAD_9://back key로 쓴다.
			//RalTextViewerStart(0);
			StaGotoState(wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );

			break;
		case VK_PAD_3:
			break;

		case VK_USER_DOWN://list down key
			if((txvtotalFileNum -1) > txvlistCount)
			{
				if(txvfocusedList >= (MAX_LST_NUMBER -1) && txvlistCount%MAX_LST_NUMBER == (MAX_LST_NUMBER-1))
				{
					if(RalGetFileList((char*)textList,textListIdentifier,MAX_LST_NUMBER,MAX_FILE_NAME,txvlistCount) == 0)//if no more file
					{
					}
					else // if more file
					{
						txvlistCount++;
						txvfocusedList=0;
						PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);
					}
				}
				else
				{
					if(txvfocusedList < (MAX_LST_NUMBER-1))
					{
						txvfocusedList++;
						txvlistCount++;
						PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);
					}
					else
					{
						txvfocusedList = 0;
						txvlistCount++;
						PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);
					}
				}
			}
			break;

		case VK_USER_UP://list up key
				if(txvlistCount != 0 && (txvlistCount%MAX_LST_NUMBER) == 0)
				{
					txvlistCount--;
					txvfocusedList = MAX_LST_NUMBER -1;
					RalGetFileList((char *)textList,textListIdentifier,(rint32)MAX_LST_NUMBER,(rint32)MAX_FILE_NAME,(rint32)(txvlistCount-MAX_LST_NUMBER+1));
				}
				else
				{
					if(txvlistCount > 0)
					{
						txvlistCount --;
					}

					if(txvfocusedList > 0 )
					{
						txvfocusedList--;
					}
				}
				PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);
			break;
		case VK_USER_LEFT:
			PrvTxvSettingHandler(wnd, AWM_WINCREATE, 0, 0);
			break;
		}
		return 1;

	case AWM_TIMER:
		if(wparam == REXY_POPUPDLG_TIMER)
		{
			if(txvpopUpId==5)
			{
				PrvTxvCloseTextViewerPop(/*txvgptextViewerCtl*/);
				PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);
			}
			else 
			{
				// do nothing
			}
		}
		bHandled = TRUE;
		break;
	}

	return bHandled;
}

static rint32 PrvTxvViewHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	int bHandled = FALSE;
	char tmp[5];

	static FntID PrevFntID = 0;
	txvStateID = TEXTVIEW_STATE_FILE_SHOW;
    switch(msg)
    {
	    case AWM_WINCREATE:
			txvtextBuffer=MemAlloc(MAX_TXT_BUFFER*2);
			MemSet(txvtextBuffer,0,MAX_TXT_BUFFER*2);

			if(1) // RegInit())//1이면 new file
			{
				//RegWriteBinary(ID_SET_BOOKMARK, "1", 4);
				//RegWriteBinary(ID_SET_BOOKCHECK, "0", 1);
				//RegWriteBinary(ID_SET_PAGESCROLLTIME, "1", 2);
				//RegWriteBinary(ID_SET_LINESCROLLTIME, "1", 2);
				//RegWriteBinary(ID_SET_CHECKUTF8, "2", 2);
				txvbookMarkPage=1;
				txvbookCheck=0;
				txvpageScrollTime =1;
				txvlineScrollTime=1;
				txvmode = 2;
			}
			else
			{

				char buf[5];
				
				MemSet(tmp,0,5);
				MemSet(buf,0,5);
				//RegReadBinary(ID_SET_BOOKMARK,buf,4);
				txvbookMarkPage=PrvTxvStrToInt(buf,5);
				
				MemSet(tmp,0,5);
				MemSet(buf,0,5);
				//RegReadBinary(ID_SET_BOOKCHECK, buf, 1);
				txvbookCheck=PrvTxvStrToInt(buf,5);
				
				MemSet(tmp,0,5);
				MemSet(buf,0,5);
				//RegReadBinary(ID_SET_PAGESCROLLTIME,buf, 2);
				txvpageScrollTime =PrvTxvStrToInt(buf,5);
				
				MemSet(tmp,0,5);
				MemSet(buf,0,5);
				//RegReadBinary(ID_SET_LINESCROLLTIME,buf, 2);
				txvlineScrollTime=PrvTxvStrToInt(buf,5);
				
				MemSet(tmp,0,5);
				MemSet(buf,0,5);
				//RegReadBinary(ID_SET_CHECKUTF8,buf, 2);
				txvmode = PrvTxvStrToInt(buf,5);
			}
			PrvTxvGetText(0); 
			
			if(txvmode != 3)
			{
				

				if(txvtextBuffer[0] == 0xFF && txvtextBuffer[1] == 0xFE)
				{
					txvmode = TEXT_UNICODE_LITTLE;// unicode
				}
				else if(txvtextBuffer[0] == 0xFE && txvtextBuffer[1] == 0xFF)
				{
					txvmode = TEXT_UNICODE_BIG;// big endian
				}
				else if(txvtextBuffer[0] ==0xEF && txvtextBuffer[1]== 0xBB && txvtextBuffer[2]== 0xBF)//utf-8 데이타..
				{
					txvmode = TEXT_UTF8;
				}
				else
				{
					txvmode = TEXT_ANSI_CODE;// 조합형.or ascii모드..
				}
			}

			txvtotalPage = PrvTxvGetTotalPageNumber();

			if(txvbookCheck >0)//bookmark enable
			{
				unsigned char fileNameBuffer[MAX_FILE_NAME];
				MemSet(fileNameBuffer, 0, MAX_FILE_NAME);
				MemCopy(&fileNameBuffer[0], &textList[txvfocusedList], MAX_FILE_NAME);
				if(StrCmp(&bookmakedFileName[0],&fileNameBuffer[0])==0)
				{
					txvfileLocationPointer=0;
					txvpreFileLocationPointer=0;
					txvpageOffSetChar=0;
					txvcurrentPage = txvbookMarkPage;
					txvrunningTimer=0;
					PrvTxvSearchBookMark();
				}
				else
				{
					txvfileLocationPointer=0;
					txvpreFileLocationPointer=0;
					txvpageOffSetChar=0;
					txvcurrentPage =1;
					txvrunningTimer=0;
				}
			}
			else
			{
				txvfileLocationPointer=0;
				txvpreFileLocationPointer=0;
				txvpageOffSetChar=0;
				txvcurrentPage =1;
				txvrunningTimer=0;
			}

			if(PrvTxvGetText(txvfileLocationPointer)==0)
			{
				GdiClrScreen(hWnd, COLOR_WHITE);
				PrvTxvListHandler(hWnd, AWM_WINCREATE, 0, 0);
			}
			else
			{
				PrvTxvViewPage(hWnd);
			}
			bHandled = TRUE;
			break;

	    case AWM_PAINT:	
			PrvTxvViewPage(hWnd);
			bHandled = TRUE;
			break;

	    case AWM_KEYDOWN:
			PrvTxvCloseTextViewerPop();
	        switch(wParam)
	        {
			 case VK_USER_LEFT://page scroll timer
				 if(txvrunningTimer == 0 && txvpageScrollTime>0)
				 {
					 txvrunningTimer =1;
					 UsrSetTimer(hWnd, TEXTVIEWER_PAGE_SCROLL_TIMER, txvpageScrollTime*1000 );
				 }
				 else
				 {
					 txvrunningTimer=0;
					 UsrKillTimer(hWnd, TEXTVIEWER_PAGE_SCROLL_TIMER);
				 }
				break;
			 case VK_USER_RIGHT:// line scroll timer
				 if(txvrunningTimer == 0 && txvlineScrollTime>0)
				 {
					 txvrunningTimer =1;
					 UsrSetTimer(hWnd, TEXTVIEWER_LINE_SCROLL_TIMER, txvlineScrollTime*1000 );
				 }
				 else
				 {
					 txvrunningTimer=0;
					 UsrKillTimer(hWnd, TEXTVIEWER_LINE_SCROLL_TIMER);
				 }
				break;
		     case VK_PAD_9://back key
				MemFree(txvtextBuffer);
				txvtextBuffer = NULL;
				if(txvrunningTimer ==1)
				{
					UsrKillTimer(hWnd, TEXTVIEWER_PAGE_SCROLL_TIMER);
					UsrKillTimer(hWnd, TEXTVIEWER_LINE_SCROLL_TIMER);
				}
				if(txvbookCheck==1)//bookmark enable
				{
					txvbookMarkPage=txvcurrentPage;//bookmark된 page저장.
					MemCopy(&bookmakedFileName[0],&textList[txvfocusedList],10);//bookmark된 파일 이름 저장.
					MemSet(tmp,0,5);
					StrIToA(txvbookMarkPage, tmp);
					//RegWriteBinary(ID_SET_BOOKMARK, tmp, 4);

					if(txvgptextViewerCtl == 0)
					{
						PrvTxvCreateTextViewerPop(PrvTxvPopTextViewerCB4,2000);
					}
					PrvTxvListHandler(hWnd, AWM_PAINT, 0, 0);
					
				}
				else//bookmark disable
				{
					PrvTxvListHandler(hWnd, AWM_PAINT, 0, 0);
				}
				break;
			 case VK_USER_DOWN://page down
				 txvpageOffSetChar =0;
				 txvcurrentPage++;
				 if(txvcurrentPage > txvtotalPage)
				 {
					 txvcurrentPage--;
					 if(txvgptextViewerCtl == 0)
					 {
						PrvTxvCreateTextViewerPop(PrvTxvPopTextViewerCB,1000);
					 }
				 }
				 else
				 {
					 PrvTxvGetText(txvfileLocationPointer);
					 PrvTxvViewPage(hWnd);
				 }
				 break;
			 case VK_USER_UP://page up
				  if(txvcurrentPage >1)
				  {
						txvcurrentPage--;
						txvfileLocationPointer=0;
						txvpreFileLocationPointer=0;
						txvpageOffSetChar=0;
						PrvTxvPageBack();
						PrvTxvViewPage(hWnd);

				  }
				  else
				  {
				  	 if(txvgptextViewerCtl == 0)
					 {
					  	 PrvTxvCreateTextViewerPop(PrvTxvPopTextViewerCB2,1000);
				  	 }
				  }
				  break;
	        }
			bHandled = TRUE;
		    break;
	
	case AWM_TIMER:
		if(wParam == TEXTVIEWER_PAGE_SCROLL_TIMER)//page 단위 넘김.
		{
			if((txvtotalPage)>txvcurrentPage)
			{
				txvcurrentPage++;
				PrvTxvGetText(txvfileLocationPointer);
				PrvTxvViewPage(hWnd);
			}
			else
			{
				UsrKillTimer(hWnd, TEXTVIEWER_PAGE_SCROLL_TIMER);
			}
		}
		else if(wParam == TEXTVIEWER_LINE_SCROLL_TIMER)//line 단위 넘김.
		{
			if((txvtotalPage)>txvcurrentPage)
			{
				txvlineNumber++;
				if(txvlineNumber%(MAX_LST_NUMBER-1)==0)
				{
					txvlineNumber=0;
					txvcurrentPage++;
				}
				txvfileLocationPointer =txvpreFileLocationPointer + PrvTxvGetOneLine();
				PrvTxvGetText(txvfileLocationPointer);
				PrvTxvViewPage(hWnd);
			}
			else
			{
					UsrKillTimer(hWnd, TEXTVIEWER_LINE_SCROLL_TIMER);
			}
		}
		else if(wParam == REXY_POPUPDLG_TIMER)
		{
			if(txvpopUpId==4)//
			{
				PrvTxvCloseTextViewerPop(/*txvgptextViewerCtl*/);
				PrvTxvListHandler(hWnd, AWM_PAINT, 0, 0);
			}
			else 
			{
				PrvTxvCloseTextViewerPop(/*txvgptextViewerCtl*/);
			}
		}
		bHandled = TRUE;
		break;
    default:
        break;
    }

	return bHandled;
}

static rint32 PrvTxvSettingHandler(WND wnd, rint32 msg, rint32 wparam, rint32 lparam)
{
	int bHandled = FALSE;
	char tmp[5];
	txvStateID = TEXTVIEW_STATE_SETTING;
	switch(msg)
	{
	case AWM_WINCREATE:

		if(1) // RegInit())//1이면 new file
		{
			//RegWriteBinary(ID_SET_BOOKMARK, "1", 4);
			//RegWriteBinary(ID_SET_BOOKCHECK, "0", 1);
			//RegWriteBinary(ID_SET_PAGESCROLLTIME, "1", 2);
			//RegWriteBinary(ID_SET_LINESCROLLTIME, "1", 2);
			//RegWriteBinary(ID_SET_CHECKUTF8, "2", 2);
			txvbookMarkPage=1;
			txvbookCheck=0;
			txvpageScrollTime =1;
			txvlineScrollTime=1;
			txvmode = 2;
		}
		else
		{

			char buf[5];
			MemSet(tmp,0,5);
			MemSet(buf,0,5);
			//RegReadBinary(ID_SET_BOOKMARK,buf,4);
			txvbookMarkPage=PrvTxvStrToInt(buf,5);
			
			MemSet(tmp,0,5);
			MemSet(buf,0,5);
			//RegReadBinary(ID_SET_BOOKCHECK, buf, 1);
			txvbookCheck=PrvTxvStrToInt(buf,5);
			
			MemSet(tmp,0,5);
			MemSet(buf,0,5);
			//RegReadBinary(ID_SET_PAGESCROLLTIME,buf, 2);
			txvpageScrollTime =PrvTxvStrToInt(buf,5);
			
			MemSet(tmp,0,5);
			MemSet(buf,0,5);
			//RegReadBinary(ID_SET_LINESCROLLTIME,buf, 2);
			txvlineScrollTime=PrvTxvStrToInt(buf,5);

			MemSet(tmp,0,5);
			MemSet(buf,0,5);
			//RegReadBinary(ID_SET_CHECKUTF8,buf, 2);
			txvmode = PrvTxvStrToInt(buf,5);
		}

		txvfocusedElement=0;
		PrvTxvSettingHandler(wnd, AWM_PAINT, 0, 0);
		bHandled = TRUE;
		break;

	case AWM_PAINT:
		PrvTxvCloseTextViewerPop();
		GdiLockScreen(wnd);
		GdiClrScreen(wnd, COLOR_WHITE);

		GdiSetFgColor(wnd,COLOR_RED);
		GdiTextOut(wnd, ">", StrLen(">"), (ruint16)2, (ruint16)(10+(20*txvfocusedElement)));
		GdiSetFgColor(wnd,COLOR_BLACK); 

		GdiTextOut(wnd, "Bookmark", StrLen("Bookmark"), (ruint16)10, (ruint16)10);
		if(txvbookCheck ==0)
			GdiTextOut(wnd, "disable", StrLen("disable"), 180, 10);
		else
			GdiTextOut(wnd, "enable", StrLen("enable"), 180, 10);

		GdiTextOut(wnd, "time for page scroll", StrLen("time for page scroll"), (ruint16)10, (ruint16)30);
		MemSet(tmp,0,5);
		StrIToA(txvpageScrollTime, tmp);
		GdiTextOut(wnd, tmp, StrLen(tmp), 180, 30);

		GdiTextOut(wnd, "time for line scroll", StrLen("time for line scroll"), 10, 50);
		MemSet(tmp,0,5);
		StrIToA(txvlineScrollTime, tmp);
		GdiTextOut(wnd, tmp, StrLen(tmp), 180, 50);

		GdiTextOut(wnd, "character set utf-8",StrLen("character set utf-8"),10,70);
		if(txvmode == 3)
		{
			GdiTextOut(wnd, "enable", StrLen("enable"), 180, 70);
		}
		else
		{
			GdiTextOut(wnd, "disable", StrLen("disable"), 180, 70);
		}
		

		GdiUnlockScreen(wnd);
		bHandled = TRUE;
		break;
	case AWM_KEYDOWN:
		PrvTxvCloseTextViewerPop();
		switch(wparam) {
		case VK_PAD_0:
			break;

		case VK_PAD_1:
			if(txvbookCheck ==0)
			{
				//RegWriteBinary(ID_SET_BOOKMARK, "1", 4);
			}
			MemSet(tmp,0,5);
			StrIToA(txvbookCheck, tmp);
			//RegWriteBinary(ID_SET_BOOKCHECK, tmp, 1);

			MemSet(tmp,0,5);
			StrIToA(txvpageScrollTime, tmp);
			//RegWriteBinary(ID_SET_PAGESCROLLTIME, tmp, 2);

			MemSet(tmp,0,5);
			StrIToA(txvlineScrollTime, tmp);
			//RegWriteBinary(ID_SET_LINESCROLLTIME, tmp, 2);

			MemSet(tmp,0,5);
			StrIToA(txvmode, tmp);
			//RegWriteBinary(ID_SET_CHECKUTF8, tmp, 2);

			PrvTxvCreateTextViewerPop(PrvTxvPopTextViewerCB3,2000);
			break;
		case VK_PAD_9://back key TEXTVIEWER_SETTING
			PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);
			break;

		case VK_USER_DOWN://count down
			if(txvfocusedElement==3)
			{
				txvfocusedElement =0;
			}
			else
			{
				txvfocusedElement++;
			}
			PrvTxvSettingHandler(wnd, AWM_PAINT, 0, 0);
			break;

		case VK_USER_UP://count up
			if(txvfocusedElement == 0)
			{
				txvfocusedElement=3;
			}
			else
			{
				txvfocusedElement--;
			}
			PrvTxvSettingHandler(wnd, AWM_PAINT, 0, 0);
			break;
		case VK_USER_LEFT:
			if(txvfocusedElement == 0)//bookmark
			{
				if(txvbookCheck==0)
					txvbookCheck=1;
				else
					txvbookCheck=0;
			}
			else if(txvfocusedElement == 1)//page scroll time
			{
				if(txvpageScrollTime==1)
				{
					//do nothing
				}
				else
				{
					txvpageScrollTime--;
				}
			}
			else if(txvfocusedElement == 2)//line scroll time
			{
				if(txvlineScrollTime==1)
				{
					//do nothing
				}
				else
				{
					txvlineScrollTime--;
				}
			}
			else if(txvfocusedElement == 3)
			{
				if(txvmode !=3)
				{
					txvmode =3;
				}
				else if(txvmode == 3)
				{
					txvmode = 2;
				}
			}
			PrvTxvSettingHandler(wnd, AWM_PAINT, 0, 0);
			break;
		case VK_USER_RIGHT:
		    if(txvfocusedElement == 0)//bookmark
			{
				if(txvbookCheck==0)
					txvbookCheck=1;
				else
					txvbookCheck=0;
			}
			else if(txvfocusedElement == 1)//page scroll time
			{
				if(txvpageScrollTime ==10)
				{
					txvpageScrollTime=1;
				}
				else
				{
					txvpageScrollTime++;
				}
			}
			else if(txvfocusedElement == 2)//line scroll time
			{
				if(txvlineScrollTime==10)
				{
					txvlineScrollTime=1;
				}
				else
				{
					txvlineScrollTime++;
				}
			}
			else if(txvfocusedElement == 3)
			{
				if(txvmode !=3)
				{
					txvmode =3;
				}
				else if(txvmode ==3)
				{
					txvmode = 2;
				}
			}
			PrvTxvSettingHandler(wnd, AWM_PAINT, 0, 0);
			break;
		}
		return 1;

	case AWM_TIMER:
		if(wparam == REXY_POPUPDLG_TIMER)
		{
			PrvTxvCloseTextViewerPop(/*txvgptextViewerCtl*/);
			PrvTxvListHandler(wnd, AWM_PAINT, 0, 0);
		}
		bHandled = TRUE;
		break;
	}
	return bHandled;
}

static int PrvTxvFormHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	int ret = FALSE;
	rint32 i;
	for (i=0; i < TEXTVIEW_STATE_SETTING; i++)
	{
		if ( txvStateID == g_TxvFormHandleList[i].eStateId) 
		{
			ret = g_TxvFormHandleList[i].pfnFormHandler(hWnd, msg, wParam, lParam);
			break;
		}
	}
	return ret;
}

rint32 TxvTextViewProc(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	int bHandled = FALSE;
	int err;
	if (msg == AWM_CREATE)
	{
		txvStateID = TEXTVIEW_STATE_FILE_LIST;
		GdiGetScreenInfo(&txvscreenWidth,&txvscreenHeight);
		gpWND = WinCreateWindow(PrvTxvFormHandler, 0, 0, txvscreenWidth, txvscreenHeight, WND_PANEL);
		bHandled = TRUE;
		//PrvTxvFormHandler(hWnd, msg, wParam, lParam);
	}
	else if (msg == AWM_DESTROY)
	{
		// Handle quit event of this application here if necessary
		if(txvgptextViewerCtl != 0)
		{
			CtlDestroy(txvgptextViewerCtl, &err);
			txvgptextViewerCtl = 0;
		}

		WinDestroyWindow(hWnd);
		gpWND = NULL;
		bHandled = TRUE;
	}
	
	return bHandled;
}

static void PrvTxvViewPage(WND hWnd)
{
	char tmp[5];
	unsigned short value;
	static FntID PrevFntID = 0;
	rint32 i=0, j=0, count, width;
	FntID CurrentFntID = 0;
	GdiSetFgColor(hWnd, COLOR_BLACK);
	GdiLockScreen(hWnd);
	GdiClrScreen(hWnd, COLOR_WHITE);

	PrevFntID = FntGetFont(&FntGlobalContext);

	for(;j<MAX_LST_NUMBER;j++)
	{
		width=0;
		while( width<MAX_ONE_LINE_WIDTH && (txvtextBuffer[txvpageOffSetChar+i] != 0x00 || txvtextBuffer[txvpageOffSetChar+1+i] != 0x00 || txvtextBuffer[txvpageOffSetChar+2+i] != 0x00))
		{
			value = PrvTxvGetCharCode(txvtextBuffer, txvpageOffSetChar+i, &count, &CurrentFntID);
			if (value == 0x0d)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 4;
				else i = i + 2;
				break;
			}
			else if (value == 0x0a)
			{
				if (txvmode == TEXT_UNICODE_LITTLE || txvmode == TEXT_UNICODE_BIG) i = i + 2;
				else i = i + 1;
				break;
			} 
			else if (txvmode == TEXT_UTF8 && txvtextBuffer[txvpageOffSetChar+i] == 0xEF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xBB \
				&& txvtextBuffer[txvpageOffSetChar+i+2] == 0xBF )
			{
				i = i + 3;
				continue;
			} 
			else if (txvmode == TEXT_UNICODE_LITTLE && txvtextBuffer[txvpageOffSetChar+i] == 0xFF && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFE )
			{
				i = i + 2;
				continue;
			}
			else if (txvmode == TEXT_UNICODE_BIG && txvtextBuffer[txvpageOffSetChar+i] == 0xFE && txvtextBuffer[txvpageOffSetChar+i+1] == 0xFF )
			{
				i = i + 2;
				continue;
			}

			FntSetFont(&FntGlobalContext, CurrentFntID);
			width = width + GdiTextOutW(hWnd, (ruint16)(10 + width),(ruint16)(25+(15*j)), &value, 1 ); // 1 Char View

			FntSetFont(&FntGlobalContext, PrevFntID);
			i = i + count;
		}

	}
	txvpreFileLocationPointer= txvfileLocationPointer;
	txvfileLocationPointer=txvfileLocationPointer+i;

	MemSet(tmp,0,5);
	StrIToA(txvtotalPage, tmp);
	GdiTextOut(hWnd, "/", StrLen("/"),(ruint16)(SCR_SIZE_WIDTH -40), (ruint16)(SCR_SIZE_HEIGHT-20));
	GdiTextOut(hWnd, tmp, StrLen(tmp), (ruint16)(SCR_SIZE_WIDTH-30), (ruint16)(SCR_SIZE_HEIGHT-20));
	StrIToA(txvcurrentPage, tmp);
	GdiTextOut(hWnd, tmp, StrLen(tmp), (ruint16)(SCR_SIZE_WIDTH-70), (ruint16)(SCR_SIZE_HEIGHT-20));
	GdiUnlockScreen(hWnd);

}

static unsigned short PrvTxvGetCharCode(unsigned char *buf, rint32 index, rint32 *count, AppFontIDType *FntId)
{
	unsigned short tmp1, tmp2, ret;
	unsigned char src[4];
	tmp1 = (unsigned short)buf[index];
	tmp2 = (unsigned short)buf[index+1];
	MemSet(src, 0, 4);
	switch(txvmode)
	{
	case TEXT_UNICODE_LITTLE:
		ret = ( tmp1 | (tmp2<<8) );
		if ( ret > 0x80 )  // Hangul
		{
			*FntId = FntIDUnicode12Han;
			ret -= 0xac00;
		} else 
		{
			*FntId = FntIDUnicode12BL;
		}
		*count = 2;
		break;
	case TEXT_UNICODE_BIG:
		ret = ( (tmp1<<8) | tmp2 );
		if ( ret > 0x80 )  // Hangul
		{
			*FntId = FntIDUnicode12Han;
			ret -= 0xac00;
		} else 
		{
			*FntId = FntIDUnicode12BL;
		}
		*count = 2;
		break;
	case TEXT_ANSI_CODE:
		if ( tmp1 > 0x80 ) // Hangul
		{
			*FntId = FntIDUnicode12Han;
			ret = ( (tmp1<<8) | tmp2 ) ;
			HanCodeConvert(ret, (ruint32 *)&ret,EUC,IW);
			ret -= 0xac00;
			*count = 2;
		}
		else
		{
			*FntId = FntIDUnicode12BL;
			ret = tmp1;
			*count = 1;
		}
		break;
	case TEXT_UTF8:
		if ( tmp1 > 0x80 ) // Hangul
		{
			*FntId = FntIDUnicode12Han;
			src[0] = txvtextBuffer[txvpageOffSetChar+index];
			src[1] = txvtextBuffer[txvpageOffSetChar+index+1];
			src[2] = txvtextBuffer[txvpageOffSetChar+index+2];
			PrvTxvUTF8toUCS2(&src[0],&ret);
			ret -= 0xac00;
			*count = 3;
		}
		else
		{
			*FntId = FntIDUnicode12BL;
			src[0] = txvtextBuffer[txvpageOffSetChar+index];
			PrvTxvUTF8toUCS2(&src[0],&ret);
			*count = 1;
		}
		break;
	default :
		//*FntId = FntIDUnicode12BL;
		count = 0;
		return 0;
	}
	return ret;
}
