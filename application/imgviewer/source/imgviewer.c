/* 
* $Id: imgviewer.c 491 2006-05-30 00:26:34Z madholic $
* Copyright(c) 2004-5, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy
*   
*/


#include "rexy.h"
#include "rexy_haldevice.h"

#include "../header/imgviewer.h"
#include "../header/imgdata.h"
#include "../../addin/hangle/hangle.h"
#include "application.h"
#include "../header/jpgview.h"
#include "../header/gifview.h"
#include "../header/pngview.h"

static int imgscreenWidth;
static int imgscreenHeight;

#define MAX_LST_NUMBER (imgscreenHeight/20)
#define MAX_CHAR_PER_LINE (imgscreenWidth/9)
#define MAX_TXT_BUFFER (MAX_CHAR_PER_LINE*MAX_LST_NUMBER)

#define MAX_ONE_LINE_WIDTH (MAX_CHAR_PER_LINE*8)

#define MAX_LINE_NUMBER 20
#define MAX_FILE_NAME_LEN 212


#define LIST_TEXT_X 10
#define LIST_TEXT_Y 10
#define LIST_TEXT_SPACE 15

#define LIST_TEXT_FOCUS_X 10
#define LIST_TEXT_FOCUS_Y  23
#define LIST_TEXT_FOCUS_LINE_SPACE 15

#define FOCUS_LINE_LENGTH 100

#define SCROLL_BAR_RECT_X	(imgscreenWidth -20)
#define SCROLL_BAR_RECT_Y	LIST_TEXT_Y
#define SCROLL_BAR_RECT_WIDTH	(SCROLL_BAR_RECT_X +10)
#define SCROLL_BAR_RECT_HEIGHT	 (imgscreenHeight -30)

#define FILE_NAME_WIDTH_MAX (imgscreenWidth -105)


static rint32 imgtotalFileNum;
static rint32 imglistCount =0;
static rint32 imgfocusedList=0;
static rint32 imgStateID;

unsigned char fileList [MAX_LINE_NUMBER][MAX_FILE_NAME_LEN];
unsigned long fileListIdentifier[MAX_LINE_NUMBER];


static void PrvImgListView(WND wnd);


extern rint32 GdiTextOutW(WND, ruint16, ruint16, ruint16 *, rint32);



static void PrvImgTest(WND wnd)
{
	int fd;
	ruint16 i, j;
	char tmp[10];
	struct img_data *picture;
	unsigned char *p;
	COLORVAL color;

	GdiLockScreen(wnd);

	MemSet(tmp, 0, 10);
	fd = RalOpenFile("test.jpg");
	if (fd == -1 ) 
	{
		GdiTextOut(wnd, "File Not Found", StrLen("File Not Found"), 10, 10);
		return ;
	}
	//GdiTextOut(wnd, "check", StrLen("test"), 140, 10);
	picture = img_jpeg_read(fd, 240, 320);
	if (picture == NULL)
	{
		GdiTextOut(wnd, "JPG Read fail", StrLen("JPG Read fail"), 20, 300);

		return ;
	}

	StrIToA(picture->size, tmp);
	GdiTextOut(wnd, tmp, StrLen(tmp), 140, 10);

	p = picture->data;
	for(i=0; i<picture->height; i++)
	{
		for(j=0; j<picture->width; j++)
		{
			color = *p <<8 | (*(p+1) );
			GdiSetPixel(wnd, j, i, color );
			p += 2;
		}
	}


	if (picture) 
	{
		if (picture->data)
			free(picture->data);
		free(picture);
	}
	GdiUnlockScreen(wnd);

}

static void PrvImgView(WND wnd)
{
	int fd, len;
	ruint16 i, j;
	char tmp[10];
	struct img_data *picture;
	unsigned char *p;
	COLORVAL color;

	MemSet(tmp, 0, 10);
	fd = RalOpenFile((char *)&fileList[imgfocusedList]);
	if (fd == -1 ) 
	{
		GdiTextOut(wnd, "File Not Found", StrLen("File Not Found"), 20, 300);
		return ;
	}
	//GdiTextOut(wnd, "check", StrLen("test"), 140, 10);
	picture = NULL;
	len = StrLen(fileList[imgfocusedList]);
	p = &fileList[imgfocusedList][len-3];
	if ( !StrNCmp(p, "jpg", 3) || !StrNCmp(p, "JPG", 3) )
	{
		picture = img_jpeg_read(fd, 240, 320);
	}
	else if ( !StrNCmp(p, "gif", 3) || !StrNCmp(p, "GIF", 3) )
	{
		picture = img_gif_read(fd, 240, 320);
	}
	else if ( !StrNCmp(p, "png", 3) || !StrNCmp(p, "PNG", 3) )
	{
		picture = img_png_read(fd, 240, 320);
	}
	
	if (picture == NULL)
	{
		GdiTextOut(wnd, "Image File Read fail", StrLen("Image File Read fail"), 20, 300);
		return ;
	}

	imgStateID = IMAGEVIEW_STATE_FILE_SHOW;

	GdiLockScreen(wnd);
	GdiClrScreen(wnd, COLOR_BLACK);
	StrIToA(picture->size, tmp);
	//GdiTextOut(wnd, tmp, StrLen(tmp), 100, 310);

	p = picture->data;
	for(i=0; i<picture->height; i++)
	{
		for(j=0; j<picture->width; j++)
		{
			color = *p <<8 | (*(p+1) );
			GdiSetPixel(wnd, j, i, color );
			p += 2;
		}
	}
	GdiTextOut(wnd, fileList[imgfocusedList], StrLen(fileList[imgfocusedList]), 80, 300);


	if (picture) 
	{
		if (picture->data)
			free(picture->data);
		free(picture);
	}
	GdiUnlockScreen(wnd);

}


static void ImgViewerKeyEvent(WND wnd, int wparam, int lparam)
{
	switch(wparam) {
	case VK_PAD_0:
		break;
	case VK_PAD_1:
		if (imgStateID == IMAGEVIEW_STATE_FILE_LIST) {
			if(RalChDir((char*)&fileList[imgfocusedList]) == 0)
			{
				imgtotalFileNum = RalGetFileTotalNumber();
				imglistCount =0;
				imgfocusedList=0;
				RalGetFileList((char*)fileList,fileListIdentifier,MAX_LST_NUMBER,MAX_FILE_NAME_LEN,imglistCount);
				PrvImgListView(wnd);

			}
			else
			{
				PrvImgView(wnd);
			}
			/*
				if(fileListIdentifier[imgfocusedList]==1)
				{
					if(RalChDir((char*)&fileList[imgfocusedList]) == 0)
					{
						imgtotalFileNum = RalGetFileTotalNumber();
						imglistCount =0;
						imgfocusedList=0;
						RalGetFileList((char*)fileList,(char*)fileListIdentifier,MAX_LST_NUMBER,MAX_FILE_NAME_LEN,imglistCount);
						PrvImgListView(wnd);

					}
					else
					{
						//popup message;
					}
				}
				else
				{

					if(1)
					{
						//can not read this file message
					}
					else
					{
						//PrvTxvViewHandler(wnd, AWM_CREATE, 0, 0);
					}
					PrvImgView(wnd);
				}
				*/
		}
		break;
	case VK_PAD_9://back key로 쓴다.
		if (imgStateID == IMAGEVIEW_STATE_FILE_LIST) {
			StaGotoState(wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );
		}
		else {
			imgStateID = IMAGEVIEW_STATE_FILE_LIST;
			PrvImgListView(wnd);
		}
		break;
	case VK_PAD_3:
		break;
	case VK_USER_DOWN:
		if (imgStateID == IMAGEVIEW_STATE_FILE_LIST) {
			if((imgtotalFileNum -1) > imglistCount)
			{
				if(imgfocusedList >= (MAX_LST_NUMBER -1) && imglistCount%MAX_LST_NUMBER == (MAX_LST_NUMBER-1))
				{
					if(RalGetFileList((char*)fileList,fileListIdentifier,MAX_LST_NUMBER,MAX_FILE_NAME_LEN,imglistCount) == 0)//if no more file
					{
					}
					else // if more file
					{
						imglistCount++;
						imgfocusedList=0;
						PrvImgListView(wnd);
					}
				}
				else
				{
					if(imgfocusedList < (MAX_LST_NUMBER-1))
					{
						imgfocusedList++;
						imglistCount++;
						PrvImgListView(wnd);
					}
					else
					{
						imgfocusedList = 0;
						imglistCount++;
						PrvImgListView(wnd);
					}
				}
			}
		}
		break;
	case VK_USER_UP:
		if (imgStateID == IMAGEVIEW_STATE_FILE_LIST) {
				if(imglistCount != 0 && (imglistCount%MAX_LST_NUMBER) == 0)
				{
					imglistCount--;
					imgfocusedList = MAX_LST_NUMBER -1;
					RalGetFileList((char *)fileList,fileListIdentifier,(rint32)MAX_LST_NUMBER,(rint32)MAX_FILE_NAME_LEN,(rint32)(imglistCount-MAX_LST_NUMBER+1));
				}
				else
				{
					if(imglistCount > 0)
					{
						imglistCount --;
					}

					if(imgfocusedList > 0 )
					{
						imgfocusedList--;
					}
				}
				PrvImgListView(wnd);
		}
		break;
	case VK_USER_LEFT:
		break;
	}

}

static void PrvImgListView(WND wnd)
{
	FntID PrevFntID = 1;

	rint32 i;
	rint32 width=0;
	rint32 j=0;
	
	unsigned short tmp1,tmp2;
	GdiLockScreen(wnd);
	GdiClrScreen(wnd, COLOR_WHITE);

	for(i=0; i<MAX_LST_NUMBER; i++)
	{
		if(fileList[i][0]  != 0)
		{
			if(fileListIdentifier[i] & 0x10 /*==1*/ )
				GdiTextOut(wnd, "D",StrLen("D"),(ruint16)(LIST_TEXT_X),(ruint16)(LIST_TEXT_Y+(LIST_TEXT_SPACE*i)));
			else
			{

				GdiTextOut(wnd, "F",StrLen("F"),(ruint16)(LIST_TEXT_X),(ruint16)(LIST_TEXT_Y+(LIST_TEXT_SPACE*i)));
			}
		}
				
		width=0;
		j=0;
		while(fileList[i][j] != 0)
		{
			if(fileList[i][j]>0x80) // Hangul
			{	
				PrevFntID = FntGetFont(&FntGlobalContext);
				FntSetFont(&FntGlobalContext, FntIDUnicode12Han/*FntIDUnicode16*/);
				tmp1 = fileList[i][j];
				tmp1 = tmp1<<8;
				j++;
				tmp1= tmp1 | fileList[i][j];
				HanCodeConvert(tmp1, (ruint32 *)&tmp2,EUC,IW);
				tmp2-=0xac00;
			}
			else
			{
				PrevFntID = FntGetFont(&FntGlobalContext);
				FntSetFont(&FntGlobalContext, FntIDUnicode12BL/*FntIDAlpha*/);
				tmp2 = fileList[i][j];
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
		if(imgfocusedList == i)
		{
			GdiDrawLine(wnd,(rint32)(LIST_TEXT_X +15), (rint32)(LIST_TEXT_FOCUS_Y+(imgfocusedList*LIST_TEXT_FOCUS_LINE_SPACE)),
				(rint32)(LIST_TEXT_X +15 + width) , (rint32)(LIST_TEXT_FOCUS_Y+(imgfocusedList*LIST_TEXT_FOCUS_LINE_SPACE)));
		}
	}
	// Drawing ScrollBar
	{
		rint32 tmp;
		rect txvrect1; 
		rect txvrect2; 
		if(imgtotalFileNum%MAX_LST_NUMBER)
		{
			tmp = (imgtotalFileNum/MAX_LST_NUMBER) + 1;
		}
		else
		{
			tmp = imgtotalFileNum/MAX_LST_NUMBER;
		}
		txvrect1.bottom=SCROLL_BAR_RECT_HEIGHT;
		txvrect1.left=SCROLL_BAR_RECT_WIDTH;
		txvrect1.right=SCROLL_BAR_RECT_X;
		txvrect1.top=SCROLL_BAR_RECT_Y;

		GdiRectangle(wnd,&txvrect1);

		GdiSetFgColor(wnd,COLOR_RED) ; 

		txvrect2.top=txvrect1.top + (imglistCount/MAX_LST_NUMBER)*((txvrect1.bottom - txvrect1.top)/tmp);
		txvrect2.bottom=txvrect2.top + (txvrect1.bottom-txvrect1.top)/tmp;
		txvrect2.left=txvrect1.left-2;
		txvrect2.right=txvrect1.right+2;
			
			
		if(imgtotalFileNum == (imglistCount+1))
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



int PrvImgViewerProc(WND hWnd, int nMsg, int wParam, int lParam)
{
	int bHandled = FALSE;
	char * buffer = NULL;

    switch(nMsg)
    {
    case AWM_CREATE:
		GdiValidateAll();
		
		imgStateID = IMAGEVIEW_STATE_FILE_LIST;
		GdiGetScreenInfo(&imgscreenWidth,&imgscreenHeight);
		RalChDir(0);
		imgtotalFileNum = RalGetFileTotalNumber();
		imglistCount =0;
		imgfocusedList=0;
		RalGetFileList((char*)fileList,fileListIdentifier,MAX_LST_NUMBER,MAX_FILE_NAME_LEN,imglistCount);	
		
		PrvImgViewerProc(hWnd, AWM_PAINT, 0, 0);
		bHandled = TRUE;
		break;
	case AWM_DESTROY:
		bHandled = TRUE;
		break;
    case AWM_PAINT:
		if (imgStateID == IMAGEVIEW_STATE_FILE_LIST)
		{
			PrvImgListView(hWnd);
		}
		else
		{
			PrvImgView(hWnd);
		}
		bHandled = TRUE;
        break;

    case AWM_KEYDOWN:
		// key operation
		ImgViewerKeyEvent(hWnd, wParam, lParam);
		bHandled = TRUE;
		break;
	
	case AWM_TIMER:
		break;
    default:
        break;
    }

    return bHandled;
}

int ImgViewerProc(WND hWnd, int nMsg, int wParam, int lParam)
{
	int bHandled = FALSE;
	WND wnd;
	int	ScreenWidth,ScreenHeight;
    switch(nMsg)
    {
    case AWM_CREATE:
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); 
		wnd = WinCreateWindow(PrvImgViewerProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL);
		bHandled = TRUE;
        break;

    case AWM_DESTROY:
		WinDestroyWindow(hWnd);
		bHandled = TRUE;
        break;
    }

    return bHandled;
}
