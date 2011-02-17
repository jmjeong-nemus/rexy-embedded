/** 
 * @if HISTORY
 *		18-Jan-2006 Joshua	revised for Rexy Embedded
 * @endif
 */

#ifdef __cplusplus
extern "C" {
#endif


#include "rexy.h"
#include "rexy_haldevice.h"

#include "mgl.h"
#include "mgl_il.h"

#define NUMB_OF_WORKBUFFER		2
//#define	LCD_WIDTH				RXLCD_WIDTH
//#define	LCD_HEIGHT				RXLCD_HEIGHT

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// System API Function
//
static MGLvoid *	g_tWorkBuffers[NUMB_OF_WORKBUFFER];

MGLvoid MGLI_InitDC( MGLdc * pDC, MGLvoid * data )
{
		int ScreenWidth, ScreenHeight;
		WND		hWork1;
		WND		hWork2;

		WND wnd = (WND)data;
	
		// Added by Joshua to support Rexy for Embedded
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight );
	
		wnd->dev = MemAlloc(sizeof(SCR_DEV));

		wnd->dev->height = ScreenHeight;
		wnd->dev->width	 = ScreenWidth;
		wnd->dev->mem_addr = GdiGetScreenPtr();
		wnd->dev->mem_size = GdiScreenGetMemSize();
	
		pDC->width  = ScreenWidth;
		pDC->height = ScreenHeight;
	
		
		pDC->pCurRC = NULL;
	
		pDC->tBuffers.pRenderBuffer = data;
		pDC->transparent = COLOR_TRANSPARENT;
	
	
		pDC->tBuffers.nNumWorkBuffers = NUMB_OF_WORKBUFFER;
	
		hWork1 = WinCreateWindow( NULL, 0, 0, ScreenWidth, ScreenHeight, WND_CUSTOM_MEM );	// We will use custom memory
		hWork2 = WinCreateWindow( NULL, 0, 0, ScreenWidth, ScreenHeight, WND_CUSTOM_MEM );	// so WND_SCREEN instead of WND_MEM
	
		hWork1->dev->mem_addr = MemAlloc( LCD_WIDTH * LCD_HEIGHT * 2 );		// Use Custom Static Memory instead of dynamic allocated memory
		hWork2->dev->mem_addr = MemAlloc( LCD_WIDTH * LCD_HEIGHT * 2 );
		
		g_tWorkBuffers[0] = (MGLvoid *)hWork1;
		g_tWorkBuffers[1] = (MGLvoid *)hWork2;
		pDC->tBuffers.pWorkBuffers = g_tWorkBuffers;
	
		return;	
}

MGLvoid MGLI_DeinitDC( MGLdc * pDC )
{
	WinDestroyWindow( pDC->tBuffers.pWorkBuffers[0] );
	WinDestroyWindow( pDC->tBuffers.pWorkBuffers[1] );
	return;
}

MGLvoid * MGLI_GetBufferData( MGLvoid * pBuffer )
{
	WND hWnd = (WND)pBuffer;

	return hWnd->dev->mem_addr;
}

MGLvoid	 MGLI_FlushAll( MGLvoid * pBuffer )
{
	GdiFlushAll( (WND)pBuffer );
	return;
}

MGLvoid	 MGLI_FlushPart( MGLvoid * pBuffer, MGLint left, MGLint top, MGLint width, MGLint height )
{
	rect rtBound;

	rtBound.left   = left;
	rtBound.top    = top;
	rtBound.right  = left + width;
	rtBound.bottom = top + height;
	GdiFlushPart( (WND)pBuffer, &rtBound );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 2D Graphic API Function
//

MGLcolor MGLI_SetFgColor( MGLvoid * pDrawBuf, MGLcolor color )
{
	color = (MGLcolor)GdiSetFgColor( (WND)pDrawBuf, (COLORVAL)color );

	return color;
}

MGLcolor MGLI_SetBgColor( MGLvoid * pDrawBuf, MGLcolor color )
{
	color = (MGLcolor)GdiSetBkColor( (WND)pDrawBuf, (COLORVAL)color );

	return color;
}

MGLcolor MGLI_GetFgColor( MGLvoid * pDrawBuf )
{
	return (MGLcolor)GdiGetFgColor( (WND)pDrawBuf );
}

MGLcolor MGLI_GetBgColor( MGLvoid * pDrawBuf )
{
	return (MGLcolor)GdiGetBkColor( (WND)pDrawBuf );
}

MGLcolor MGLI_SetPixel( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLcolor color )
{
	color = (MGLcolor) GdiSetPixel( (WND)pDrawBuf, (ruint16)x, (ruint16)y, (COLORVAL)color );

	return color;
}

MGLcolor MGLI_GetPixel( MGLvoid * pDrawBuf, MGLint x, MGLint y )
{
	return (MGLcolor) GdiGetPixel( (WND)pDrawBuf, (ruint16)x, (ruint16)y );
}

MGLvoid MGLI_DrawLine( MGLvoid * pDrawBuf, MGLint x1, MGLint y1, MGLint x2, MGLint y2 )
{
	GdiDrawLine( (WND)pDrawBuf, (ruint16)x1, (ruint16)y1, (ruint16)x2, (ruint16)y2 );

	return;
}

MGLvoid	MGLI_DrawRect( MGLvoid * pDrawBuf, MGLint left, MGLint top, MGLint width, MGLint height )
{
	rect rt;
	
	rt.left   = left;
	rt.top    = top;
	rt.right  = left + width -1;
	rt.bottom = top + height -1;

	GdiRectangle( (WND)pDrawBuf, &rt );
	
	return;
}

MGLvoid	MGLI_FillRect( MGLvoid * pDrawBuf, MGLint left, MGLint top, MGLint width, MGLint height, MGLcolor color )
{
	rect rt;

	rt.left   = left;
	rt.top    = top;
	rt.right  = left + width -1;
	rt.bottom = top + height -1;

	if ( ! GdiGetScreenLockCount() ) GdiLockScreen( WinGetSelection() );
	GdiFillRect( (WND)pDrawBuf, &rt, (COLORVAL)color );

	return;
}

MGLvoid	MGLI_DrawCircle( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLint r )
{
	//### Rexy는 Circle 함수 없음.
	return;
}

MGLvoid	MGLI_DrawEllipse( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLint xr, MGLint yr )
{
	//### Rexy는 Ellipse 함수 없음. 
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Bitmap API function 
//

MGLvoid MGLI_DrawBitmap( MGLvoid * pDrawBuf, MGLbitmap * pBitmap, MGLint x, MGLint y )
{
	RxBITMAP tempBitmap;

	tempBitmap.bpp = pBitmap->bpp;
	tempBitmap.data = pBitmap->data;
	tempBitmap.height = pBitmap->height;
	tempBitmap.bTransparent = pBitmap->bTransparent;
	tempBitmap.transparentColor = pBitmap->nTrColor;	// DOLATER : How can we determine transparent color from MGLbitmap?
	tempBitmap.width = pBitmap->width;

	//### Parameter :   WND,           BITMAP,          src, dst,    dst size
	if ( ! GdiGetScreenLockCount() ) GdiLockScreen( WinGetSelection() );
	GdiDrawBitmap( (WND)pDrawBuf, &tempBitmap, 0, 0, x, y, pBitmap->width, pBitmap->height);
	return;
}

MGLvoid MGLI_BitBlt( MGLvoid * pDst, MGLint dst_x, MGLint dst_y, MGLint width, MGLint height, 
				     MGLvoid * pSrc, MGLint src_x, MGLint src_y )
{

// rexy의 AS_BitBlt()는 투명색에 대한 처리를 지원하지 않아서 내부 구현을 사용한다.
#define __USE_INTERNAL_BITBLT

#ifdef __USE_INTERNAL_BITBLT
	// bitblt 임시 구현 (transparent color 지원)
	MGLushort * pOrgSrcAddr, * pOrgDstAddr, * pSrcAddr, * pDstAddr;
	MGLint i, j;
	MGLcolor color, nTrColor;
	MGLint nDCWidth, nDCHeight;

	nDCWidth = MGL_GetDCWidth();
	nDCHeight = MGL_GetDCHeight();
	
	pOrgSrcAddr = MGLI_GetBufferData(pSrc);
	pOrgDstAddr = MGLI_GetBufferData(pDst);
	nTrColor = MGL_GetDCTrColor();

	pSrcAddr = pOrgSrcAddr + src_x + src_y * nDCWidth;
	pDstAddr = pOrgDstAddr + src_x + src_y * nDCWidth;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			//color = MGLI_GetPixel(pSrc, j + src_x, i + src_y);
			color = *pSrcAddr;
			if (color != nTrColor)
			{
				//MGLI_SetPixel(pDst, j + dst_x, i + dst_y, color);
				*pDstAddr = color;
			}
			pSrcAddr++;
			pDstAddr++;
		}
		pSrcAddr += (nDCWidth - width);
		pDstAddr += (nDCWidth - width);
	}
#else
	MGLint	nOP = SRC_COPY;

	GdiBitBlt( (WND)pDst, dst_x, dst_y, (WND)pSrc, src_x, src_y, width, height, nOP, alpha );
#endif

	return;
}

//### 20051120 hylee : 파일에서 가져와야하는지 id에서 가져와야하는지..rexy는 파일에서 가져오고 있음.
MGLvoid MGLI_LoadBitmap( MGLint nID, MGLbitmap *  pBitmap )
{
	// TODO: 에러처리 필요. bitmap loading에 실패하는 경우에 대해서 에러코드를 리턴한다.
	// 에러코드는: MGL_ERROR_BITMAP_LOAD_FAILURE
	RxBITMAP tempBitmap;

	if (RscLoadBitmap( (const BmpID) nID, &tempBitmap ))
	{
		MGL_SetError((MGLerror)MGL_ERROR_BITMAP_LOAD_FAILURE);
	}
	else
	{
		pBitmap->bpp = tempBitmap.bpp;
		pBitmap->data = tempBitmap.data;
		pBitmap->height= tempBitmap.height;
		pBitmap->width = tempBitmap.width;
		pBitmap->bTransparent = BITMAP_TRANSPARENT;
		pBitmap->nTrColor = COLOR_TRANSPARENT;
	}
	return;
}

MGLvoid MGLI_MemSet( MGLvoid * src, MGLint size, MGLint value)
{
	MemSet( src, size, value );
	return;
}

MGLvoid MGLI_MemCopy( MGLvoid * dest, MGLvoid * src, MGLint size)
{
	MemCopy( dest, src, size );
}

MGLbyte * MGLI_StrCpy( MGLbyte * dst, MGLbyte * src )
{
	return StrCpy((char*)dst, (char*)src);
}

MGLvoid MGLI_Assert( MGLint nCondition, MGLbyte * pszFile, MGLbyte * pszFunc, MGLint nLine )
{
	if( nCondition )	
	{
		// TODO: MGL_Assert 에 전해지는 비교 구문의 결과값이 참이다.
	}
	else
	{
		// TODO: MGL_Assert 에 전해지는 비교 구문의 결과값이 거짓이다. 
	}

	return;
}

MGLvoid MGLI_DebugMsg(MGLbyte * msg)
{
	TrcPrintF((char *)msg);
}

MGLvoid MGLI_DebugFPS(MGLint nFPSCount, MGLulong nFPSTicks)
{
	TrcPrintF("fps=%u (frame=%u,ticks=%u) \n", nFPSCount / ((MGL_GetTicks() - nFPSTicks) / MGL_TicksForSecond()), nFPSCount, (MGL_GetTicks() - nFPSTicks));
}

MGLulong MGLI_TicksForSecond( MGLvoid )
{
	return 1000;
}

MGLulong MGLI_GetTicks( MGLvoid )
{
	return UsrGetCurrentTicks();
}

MGLbyte * MGLI_GetStringProperty(MGLint nPropID)
{
        switch (nPropID)
        {
        case MGL_TARGET_PROP_STR_HW_MODEL:
#ifdef REXY_EMULATOR
                return "IBM PC Compatible";
#else   
                return "S3C2410TK";
#endif //REXY_EMULATOR         
                break;

        case MGL_TARGET_PROP_STR_HW_CPU:
#ifdef REXY_EMULATOR   
                return "Intel Pentium Processor";
#else
                return "S3C2410";
#endif //REXY_EMULAOTR
                break;

        case MGL_TARGET_PROP_STR_HW_MEMORY:
                return "Not specified.";
                break;

        case MGL_TARGET_PROP_STR_HW_GPU:
                return "Not specified.";
                break;

        case MGL_TARGET_PROP_STR_SW_OS:
#ifdef REXY_EMULATOR
                return "Microsoft Windows";
#else
                return "uC/OS 2.60";
#endif //REXY_EMULATOR
                break;

        case MGL_TARGET_PROP_STR_SW_GUI:
                return "Rexy (JProject Rev.1)";
                break;

        default:
                MGL_SetError(MGL_ERROR_UNKNOWN_PROP_ID);
                return NULL;
        }
}

MGLint MGLI_GetIntProperty(MGLint nPropID)
{
        switch (nPropID)
        {
        default:
                MGL_SetError(MGL_ERROR_UNKNOWN_PROP_ID);
                return 0;
        }
}

MGLboolean MGLI_GetBooleanProperty(MGLint nPropID)
{
        switch (nPropID)
        {
        default:
                MGL_SetError(MGL_ERROR_UNKNOWN_PROP_ID);
                return FALSE;
        }
}

#ifdef __cplusplus
}
#endif
