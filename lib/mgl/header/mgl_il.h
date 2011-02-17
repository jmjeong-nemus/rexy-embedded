/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @file mgl_il.h
 * @brief MGL Integration
 * @author <a href="mailto:hylee@mococo.com">Hwayoung Lee</a>
 * @author <a href="mailto:mio@mococo.com">Jinwoo Min</a>
 */

/**
 * @addtogroup integration
 * @{
 */

#ifndef __mgl_il_h_
#define __mgl_il_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "mgl.h"

/* 
 * 2D Graphic Integration Layer API
 */
/** 
 * @brief Layer의 배경색을 바꾼다. (한 프레임을 그릴때에만 유효함.)
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param color	바꾸려는 색상값.
 * @return	이전 색상값.
 */
MGLcolor MGLI_SetBgColor( MGLvoid * pDrawBuf, MGLcolor color );

/** 
 * @brief	현재 Layer의 전경색을 바꾼다. 
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param color	바꾸려는 색상값.
 * @return	이전 색상값.
 */
MGLcolor MGLI_SetFgColor( MGLvoid * pDrawBuf, MGLcolor color );

/** 
 * @brief	현재 Layer의 배경색을 얻어온다.  
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @return	현재 색상값.
 */
MGLcolor MGLI_GetBgColor( MGLvoid * pDrawBuf );

/** 
 * @brief	현재 Layer의 전경색을 얻어온다.  
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @return	현재 색상값.
 */
MGLcolor MGLI_GetFgColor( MGLvoid * pDrawBuf );

/** 
 * @brief	주어진 좌표의 픽셀 색상 값을 바꾼다. 
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param x		X 좌표 
 * @param y		Y 좌표 
 * @param color	변경할 색
 * @return	주어진 좌표의 이전 색상 
 */
MGLcolor MGLI_SetPixel( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLcolor color );

/** 
 * @brief	주어진 좌표의 픽셀 색상 값을 얻어온다. 
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param x		X 좌표 
 * @param y		Y 좌표 
 * @return	주어진 좌표의 색상 값.
 */
MGLcolor MGLI_GetPixel( MGLvoid * pDrawBuf, MGLint x, MGLint y );

/** 
 * @brief	두 점을 잇는 선을 그린다. 
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param x1	선의 시작 X 좌표
 * @param x2	선의 시작 Y 좌표
 * @param y1	선의 끝 X 좌표
 * @param y2	선의 끝 Y 좌표
 * @return	없음.
 */
MGLvoid  MGLI_DrawLine( MGLvoid * pDrawBuf, MGLint x1, MGLint y1, MGLint x2, MGLint y2 );

/** 
 * @brief	사각형을 그린다. 
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param left		영역 좌표 : left
 * @param top		영역 좌표 : top
 * @param width		X축 너비
 * @param height	Y축 높이 
 * @return	없음.
 */
MGLvoid	 MGLI_DrawRect( MGLvoid * pDrawBuf, MGLint left, MGLint top, MGLint width, MGLint height );

/** 
 * @brief	원하는 색으로 채워져 있는 사각형을 그린다.
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param left		영역 좌표 : left
 * @param top		영역 좌표 : top
 * @param width		X축 너비
 * @param height	Y축 높이 
 * @param color		채울 색상 값. 
 * @return	없음.
 */
MGLvoid	 MGLI_FillRect( MGLvoid * pDrawBuf, MGLint left, MGLint top, MGLint width, MGLint height, MGLcolor color );

/** 
 * @brief	원을 그린다. 
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param x	중심점 X 좌표 
 * @param y 중심점 Y 좌표 
 * @param r 원의 반지름
 * @return	없음.
 */
MGLvoid	 MGLI_DrawCircle( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLint r );

/** 
 * @brief	타원을 그린다. 
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param x		중심점 X 좌표 
 * @param y		중심점 Y 좌표 
 * @param xr	X축 반지름
 * @param yr	Y축 반지름
 * @return	없음.
 */
MGLvoid	 MGLI_DrawEllipse( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLint xr, MGLint yr );


/*
 * Bitmap Integration Layer API
 */

/**
 * @brief	현재 선택된 layer에 bitmap을 그린다. 
 * @param pDrawBuf 현재 MGL이 사용하고 있는 Drawing 메모리 주소나 이를 포함한 윈도우 객체에 대한 포인터
 * @param pBitmap	bitmap data
 * @param x			bitmap을 그릴 X 좌표
 * @param y			bitmap을 그릴 Y 좌표
 * @return	없음.
 */

MGLvoid MGLI_DrawBitmap( MGLvoid * pDrawBuf, MGLbitmap * pBitmap, MGLint x, MGLint y );

/** 
 * @brief	소스 영역의 데이터를 그릴려고 하는 곳에 그려준다. 
 *
 * @param pDst		그려져야하는 영역에 대한 포인터
 * @param dst_x		그려져야하는 영역 : X 좌표
 * @param dst_y		그려져야하는 영역 : Y 좌표
 * @param width		그려져야하는 영역 : width
 * @param height	그려져야하는 영역 : height
 * @param pSrc		소스 영역에 대한 포인터
 * @param src_x		소스 영역 : X 좌표
 * @param src_y		소스 영역 : Y 좌표
 * @return	없음.
 * @note BitBlt 함수 내부의 Operation Code 값은 COPY만 존재하기로 한다. 
 */
MGLvoid MGLI_BitBlt( MGLvoid * pDst, MGLint dst_x, MGLint dst_y, MGLint width, MGLint height, 
				     MGLvoid * pSrc, MGLint src_x, MGLint src_y );

/** 
 * @brief	Bitmap 데이터를 가져온다. 
 *
 * @param nID		데이터를 얻으려는 Bitmap ID
 * @param pBitmap	Bitmap 데이터 저장 공간
 * @return	없음.
 */
MGLvoid MGLI_LoadBitmap( MGLint nID, MGLbitmap *  pBitmap );

/* 
 * System Integration Layer API
 */
/**
 * @brief MGL이 동작 가능하도록 Device Context ( DC )를 설정합니다.
 * @param pDC 초기화를 해야하는 Device Context의 포인터
 * @param data MGL_InitEngine(...) 함수에 넘겨주었던 초기화 값.
 * @note 
 *		-# DC에 설정해야하는 값들은 다음과 같습니다. 
 *			- width : Target 시스템의 LCD 사이즈, 너비.
 *			- height : Target 시스템의 LCD 사이즈, 높이.
 *			- pCurRC : 현재 Rendering Context 값을 설정해줍니다. 일반적으로 초기화시에는 NULL 값을 가집니다.
 *			- tBuffers : MGL이 사용하는 버퍼를 만들어 줍니다. 
 *				- pRenderBuffer : MGL의 결과 값을 그려야하는 곳의 버퍼를 연결합니다.
 *				- pCompBuffer : Recdering Context 안의 Layer들의 내용을 그려야하는 곳의 버퍼를 연결합니다.
 *				- nNumWorkBuffers : 여분의 Working 버퍼의 개수를 설정합니다.
 *				- pWorkBuffers : 여분의 Working 버퍼들을 연결합니다. 
 *			- transparent : Device Context에서 사용되는 투명색 값을 설정합니다.
 *		-# tBuffers에 적당한 버퍼값을 연결하지 않으면 MGL은 동작하지 않습니다. 
 *		-# 생성하여 연결시켜준 버퍼들은 시스템을 종료 할 때 삭제해줘야합니다.
 *			이에 해당하는 코드는 MGLI_DeinitDC(...) 에 넣어둡니다. 
 */
MGLvoid MGLI_InitDC( MGLdc * pDC, MGLvoid * data );

/**
 * @brief MGL을 초기화 하면서 생성하는 것들을 삭제 및 해제합니다.  
 *		특히 DC안의 tBuffers 멤버에 동적으로 메모리를 연결 시켰다면 그것은 삭제해야합니다. 
 * @param pDC 사용 해제하려는  Device Context의 포인터
 */
MGLvoid MGLI_DeinitDC( MGLdc * pDC );

/**
 * @brief MGL에 넘겨주었던 버퍼의 메모리 주소를 가져옵니다. 
 * @param pBuffer Drawing 메모리 버퍼나 이를 소유한 객체의 포인터
 * @note 해당 플랫폼의 GDI 구조가 윈도우 객체 등과 같은 Drawing 버퍼를 포함하는 하나의 
 *		객체로 되어 있지 않고 바로 메모리 주소를 사용한다면 pBuffer를 바로 리턴해줘야 한다. 
 */
MGLvoid * MGLI_GetBufferData( MGLvoid * pBuffer );

/**
 * @brief pBuffer의 내용을 LCD 화면 전체에 그려주는 함수
 * @param pBuffer Drawing 메모리 버퍼나 이를 소유한 객체의 포인터
 */
MGLvoid	 MGLI_FlushAll( MGLvoid * pBuffer );

/**
 * @brief pBuffer의 내용을 LCD 화면에서 주어진 영역만큼만 그려주는 함수
 * @param pBuffer Drawing 메모리 버퍼나 이를 소유한 객체의 포인터
 * @param left LCD 화면에 그릴 영역 좌표 : left
 * @param top LCD 화면에 그릴 영역 좌표 : top
 * @param width LCD 화면에 그릴 영역 좌표 : width
 * @param height LCD 화면에 그릴 영역 좌표 : height
 */
MGLvoid	 MGLI_FlushPart( MGLvoid * pBuffer, MGLint left, MGLint top, MGLint width, MGLint height );

/**
 * @brief 특정 문자로 버퍼를 채운다. 
 * @param dest 특정 문자로 채울 버퍼
 * @param value 채울 특정 문자
 * @param size 채울 문자 수
 */
MGLvoid MGLI_MemSet( MGLvoid * dest, MGLint value, MGLint size );

/**
 * @brief src 버퍼의 주어진 크기만큼을 dest 버퍼에 복사한다. 
 * @param dest 채울 버퍼
 * @param src 원본 버퍼
 * @param size 복사할 크기
 */
MGLvoid MGLI_MemCopy( MGLvoid * dest, MGLvoid * src, MGLint size );

/**
 * @brief src 버퍼의 내용을 dest 버퍼에 복사한다. 
 * @param dst 채울 버퍼
 * @param src 원본 버퍼
 */
MGLbyte * MGLI_StrCpy( MGLbyte * dst, MGLbyte * src );

/**
 * @brief Assertion을 실행한다. 만약 조건이 FALSE(0)이면 Debug 메시지가 출력된다.
 * 
 * @param[in] nCondition 조건문의 결과값 
 * @param[in] pszFile Assertion을 실행하는 파일명
 * @param[in] pszFunc Assertion을 실행하는 곳의 함수명
 * @param[in] nLine Assertion을 실행하는 곳의 줄 수
 * @note mgl_config.h 에 MGL_CONFIG_USE_DEBUG_INFO의 값이 OFF되어있다면 아무런 동작도 하지 않는다.
 */
MGLvoid MGLI_Assert( MGLint nCondition, MGLbyte * pszFile, MGLbyte * pszFunc, MGLint nLine );

/**
 * @brief 디버그용 메시지를 표시한다.
 *
 * @param[in] msg 디버그용 메시지
 */
MGLvoid MGLI_DebugMsg(MGLbyte * msg);

/** 
 * @brief 초당 Tick수를 구한다.
 *
 * @return Tick수
 */
MGLulong MGLI_TicksForSecond( MGLvoid );
/** 
 * @brief 현재의 Tick값을 구한다.
 *
 * @return Tick값
 */
MGLulong MGLI_GetTicks( MGLvoid );


#ifdef __cplusplus
}
#endif

#endif // __mgl_il_h_

/**
 * @}
 */

