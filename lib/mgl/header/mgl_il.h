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
 * @brief Layer�� ������ �ٲ۴�. (�� �������� �׸������� ��ȿ��.)
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param color	�ٲٷ��� ����.
 * @return	���� ����.
 */
MGLcolor MGLI_SetBgColor( MGLvoid * pDrawBuf, MGLcolor color );

/** 
 * @brief	���� Layer�� ������� �ٲ۴�. 
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param color	�ٲٷ��� ����.
 * @return	���� ����.
 */
MGLcolor MGLI_SetFgColor( MGLvoid * pDrawBuf, MGLcolor color );

/** 
 * @brief	���� Layer�� ������ ���´�.  
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @return	���� ����.
 */
MGLcolor MGLI_GetBgColor( MGLvoid * pDrawBuf );

/** 
 * @brief	���� Layer�� ������� ���´�.  
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @return	���� ����.
 */
MGLcolor MGLI_GetFgColor( MGLvoid * pDrawBuf );

/** 
 * @brief	�־��� ��ǥ�� �ȼ� ���� ���� �ٲ۴�. 
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param x		X ��ǥ 
 * @param y		Y ��ǥ 
 * @param color	������ ��
 * @return	�־��� ��ǥ�� ���� ���� 
 */
MGLcolor MGLI_SetPixel( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLcolor color );

/** 
 * @brief	�־��� ��ǥ�� �ȼ� ���� ���� ���´�. 
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param x		X ��ǥ 
 * @param y		Y ��ǥ 
 * @return	�־��� ��ǥ�� ���� ��.
 */
MGLcolor MGLI_GetPixel( MGLvoid * pDrawBuf, MGLint x, MGLint y );

/** 
 * @brief	�� ���� �մ� ���� �׸���. 
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param x1	���� ���� X ��ǥ
 * @param x2	���� ���� Y ��ǥ
 * @param y1	���� �� X ��ǥ
 * @param y2	���� �� Y ��ǥ
 * @return	����.
 */
MGLvoid  MGLI_DrawLine( MGLvoid * pDrawBuf, MGLint x1, MGLint y1, MGLint x2, MGLint y2 );

/** 
 * @brief	�簢���� �׸���. 
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param left		���� ��ǥ : left
 * @param top		���� ��ǥ : top
 * @param width		X�� �ʺ�
 * @param height	Y�� ���� 
 * @return	����.
 */
MGLvoid	 MGLI_DrawRect( MGLvoid * pDrawBuf, MGLint left, MGLint top, MGLint width, MGLint height );

/** 
 * @brief	���ϴ� ������ ä���� �ִ� �簢���� �׸���.
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param left		���� ��ǥ : left
 * @param top		���� ��ǥ : top
 * @param width		X�� �ʺ�
 * @param height	Y�� ���� 
 * @param color		ä�� ���� ��. 
 * @return	����.
 */
MGLvoid	 MGLI_FillRect( MGLvoid * pDrawBuf, MGLint left, MGLint top, MGLint width, MGLint height, MGLcolor color );

/** 
 * @brief	���� �׸���. 
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param x	�߽��� X ��ǥ 
 * @param y �߽��� Y ��ǥ 
 * @param r ���� ������
 * @return	����.
 */
MGLvoid	 MGLI_DrawCircle( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLint r );

/** 
 * @brief	Ÿ���� �׸���. 
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param x		�߽��� X ��ǥ 
 * @param y		�߽��� Y ��ǥ 
 * @param xr	X�� ������
 * @param yr	Y�� ������
 * @return	����.
 */
MGLvoid	 MGLI_DrawEllipse( MGLvoid * pDrawBuf, MGLint x, MGLint y, MGLint xr, MGLint yr );


/*
 * Bitmap Integration Layer API
 */

/**
 * @brief	���� ���õ� layer�� bitmap�� �׸���. 
 * @param pDrawBuf ���� MGL�� ����ϰ� �ִ� Drawing �޸� �ּҳ� �̸� ������ ������ ��ü�� ���� ������
 * @param pBitmap	bitmap data
 * @param x			bitmap�� �׸� X ��ǥ
 * @param y			bitmap�� �׸� Y ��ǥ
 * @return	����.
 */

MGLvoid MGLI_DrawBitmap( MGLvoid * pDrawBuf, MGLbitmap * pBitmap, MGLint x, MGLint y );

/** 
 * @brief	�ҽ� ������ �����͸� �׸����� �ϴ� ���� �׷��ش�. 
 *
 * @param pDst		�׷������ϴ� ������ ���� ������
 * @param dst_x		�׷������ϴ� ���� : X ��ǥ
 * @param dst_y		�׷������ϴ� ���� : Y ��ǥ
 * @param width		�׷������ϴ� ���� : width
 * @param height	�׷������ϴ� ���� : height
 * @param pSrc		�ҽ� ������ ���� ������
 * @param src_x		�ҽ� ���� : X ��ǥ
 * @param src_y		�ҽ� ���� : Y ��ǥ
 * @return	����.
 * @note BitBlt �Լ� ������ Operation Code ���� COPY�� �����ϱ�� �Ѵ�. 
 */
MGLvoid MGLI_BitBlt( MGLvoid * pDst, MGLint dst_x, MGLint dst_y, MGLint width, MGLint height, 
				     MGLvoid * pSrc, MGLint src_x, MGLint src_y );

/** 
 * @brief	Bitmap �����͸� �����´�. 
 *
 * @param nID		�����͸� �������� Bitmap ID
 * @param pBitmap	Bitmap ������ ���� ����
 * @return	����.
 */
MGLvoid MGLI_LoadBitmap( MGLint nID, MGLbitmap *  pBitmap );

/* 
 * System Integration Layer API
 */
/**
 * @brief MGL�� ���� �����ϵ��� Device Context ( DC )�� �����մϴ�.
 * @param pDC �ʱ�ȭ�� �ؾ��ϴ� Device Context�� ������
 * @param data MGL_InitEngine(...) �Լ��� �Ѱ��־��� �ʱ�ȭ ��.
 * @note 
 *		-# DC�� �����ؾ��ϴ� ������ ������ �����ϴ�. 
 *			- width : Target �ý����� LCD ������, �ʺ�.
 *			- height : Target �ý����� LCD ������, ����.
 *			- pCurRC : ���� Rendering Context ���� �������ݴϴ�. �Ϲ������� �ʱ�ȭ�ÿ��� NULL ���� �����ϴ�.
 *			- tBuffers : MGL�� ����ϴ� ���۸� ����� �ݴϴ�. 
 *				- pRenderBuffer : MGL�� ��� ���� �׷����ϴ� ���� ���۸� �����մϴ�.
 *				- pCompBuffer : Recdering Context ���� Layer���� ������ �׷����ϴ� ���� ���۸� �����մϴ�.
 *				- nNumWorkBuffers : ������ Working ������ ������ �����մϴ�.
 *				- pWorkBuffers : ������ Working ���۵��� �����մϴ�. 
 *			- transparent : Device Context���� ���Ǵ� ����� ���� �����մϴ�.
 *		-# tBuffers�� ������ ���۰��� �������� ������ MGL�� �������� �ʽ��ϴ�. 
 *		-# �����Ͽ� ��������� ���۵��� �ý����� ���� �� �� ����������մϴ�.
 *			�̿� �ش��ϴ� �ڵ�� MGLI_DeinitDC(...) �� �־�Ӵϴ�. 
 */
MGLvoid MGLI_InitDC( MGLdc * pDC, MGLvoid * data );

/**
 * @brief MGL�� �ʱ�ȭ �ϸ鼭 �����ϴ� �͵��� ���� �� �����մϴ�.  
 *		Ư�� DC���� tBuffers ����� �������� �޸𸮸� ���� ���״ٸ� �װ��� �����ؾ��մϴ�. 
 * @param pDC ��� �����Ϸ���  Device Context�� ������
 */
MGLvoid MGLI_DeinitDC( MGLdc * pDC );

/**
 * @brief MGL�� �Ѱ��־��� ������ �޸� �ּҸ� �����ɴϴ�. 
 * @param pBuffer Drawing �޸� ���۳� �̸� ������ ��ü�� ������
 * @note �ش� �÷����� GDI ������ ������ ��ü ��� ���� Drawing ���۸� �����ϴ� �ϳ��� 
 *		��ü�� �Ǿ� ���� �ʰ� �ٷ� �޸� �ּҸ� ����Ѵٸ� pBuffer�� �ٷ� ��������� �Ѵ�. 
 */
MGLvoid * MGLI_GetBufferData( MGLvoid * pBuffer );

/**
 * @brief pBuffer�� ������ LCD ȭ�� ��ü�� �׷��ִ� �Լ�
 * @param pBuffer Drawing �޸� ���۳� �̸� ������ ��ü�� ������
 */
MGLvoid	 MGLI_FlushAll( MGLvoid * pBuffer );

/**
 * @brief pBuffer�� ������ LCD ȭ�鿡�� �־��� ������ŭ�� �׷��ִ� �Լ�
 * @param pBuffer Drawing �޸� ���۳� �̸� ������ ��ü�� ������
 * @param left LCD ȭ�鿡 �׸� ���� ��ǥ : left
 * @param top LCD ȭ�鿡 �׸� ���� ��ǥ : top
 * @param width LCD ȭ�鿡 �׸� ���� ��ǥ : width
 * @param height LCD ȭ�鿡 �׸� ���� ��ǥ : height
 */
MGLvoid	 MGLI_FlushPart( MGLvoid * pBuffer, MGLint left, MGLint top, MGLint width, MGLint height );

/**
 * @brief Ư�� ���ڷ� ���۸� ä���. 
 * @param dest Ư�� ���ڷ� ä�� ����
 * @param value ä�� Ư�� ����
 * @param size ä�� ���� ��
 */
MGLvoid MGLI_MemSet( MGLvoid * dest, MGLint value, MGLint size );

/**
 * @brief src ������ �־��� ũ�⸸ŭ�� dest ���ۿ� �����Ѵ�. 
 * @param dest ä�� ����
 * @param src ���� ����
 * @param size ������ ũ��
 */
MGLvoid MGLI_MemCopy( MGLvoid * dest, MGLvoid * src, MGLint size );

/**
 * @brief src ������ ������ dest ���ۿ� �����Ѵ�. 
 * @param dst ä�� ����
 * @param src ���� ����
 */
MGLbyte * MGLI_StrCpy( MGLbyte * dst, MGLbyte * src );

/**
 * @brief Assertion�� �����Ѵ�. ���� ������ FALSE(0)�̸� Debug �޽����� ��µȴ�.
 * 
 * @param[in] nCondition ���ǹ��� ����� 
 * @param[in] pszFile Assertion�� �����ϴ� ���ϸ�
 * @param[in] pszFunc Assertion�� �����ϴ� ���� �Լ���
 * @param[in] nLine Assertion�� �����ϴ� ���� �� ��
 * @note mgl_config.h �� MGL_CONFIG_USE_DEBUG_INFO�� ���� OFF�Ǿ��ִٸ� �ƹ��� ���۵� ���� �ʴ´�.
 */
MGLvoid MGLI_Assert( MGLint nCondition, MGLbyte * pszFile, MGLbyte * pszFunc, MGLint nLine );

/**
 * @brief ����׿� �޽����� ǥ���Ѵ�.
 *
 * @param[in] msg ����׿� �޽���
 */
MGLvoid MGLI_DebugMsg(MGLbyte * msg);

/** 
 * @brief �ʴ� Tick���� ���Ѵ�.
 *
 * @return Tick��
 */
MGLulong MGLI_TicksForSecond( MGLvoid );
/** 
 * @brief ������ Tick���� ���Ѵ�.
 *
 * @return Tick��
 */
MGLulong MGLI_GetTicks( MGLvoid );


#ifdef __cplusplus
}
#endif

#endif // __mgl_il_h_

/**
 * @}
 */

