/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @file mgl_config.h
 * @brief MGL Core Module : MGL �������� ���� configuration �� ����.
 * @author <a href="mailto:hylee@mococo.com">Hwayoung Lee</a>
 */

#ifndef __mgl_config_h_
#define __mgl_config_h_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Nemustech Premium Graphic Library
 * 
 * @bried Define Configuration
 *
 * @author <a href="mailto:hylee@mococo.com">Hwayoung Lee</a>
 *
 */

/**
 * @brief	Configuration ��, �ش� �������� ����ϰڴٴ� �ǹ���.
 */

#define MGL_ON		1

/**
 * @brief	Configuration ��, �ش� �������� ������� �ʰڴٴ� �ǹ���.
 */
#define MGL_OFF		0

/**
 * @brief	Fixed point operation �����. 0:������� ����. 
 */
#define		MGL_CONFIG_USE_FIXED_POINT			MGL_ON	

/**
 * @brief	Color R(5bit)/G(6bit)/B(5bit) ü�踦 �����
 */
#define		MGL_COLOR_RGB565					MGL_ON				

/**
 * @brief	DropShadow Matrix size(By Size Property)
 */
#define		MGL_DROPSHADOW_MATRIX_SIZE_MAX		499 /**< (2*size)-1 <- size max = 250 */
#define		MGL_DROPSHADOW_MATRIX_ROW_SIZE		250	/**< Maximum matrix row size in Generation matrix */
#define		MGL_DROPSHADOW_MATRIX_COL_SIZE		250	/**< Maximum matrix column size in Generation matrix */

/**
 * @brief	�ҽ� �ڵ峻�� ����� ������ �־��ٰ������� ������. 
 */
#ifdef _DEBUG
	#define MGL_CONFIG_USE_DEBUG_INFO			
#endif

#ifdef MGL_CONFIG_USE_DEBUG_INFO
	#define	__MGL_FILE__	__FILE__ /**< ������ �߻��� ���� */
	#define __MGL_FUNC__	NULL /**< ������ �߻��� �Լ� */
	#define __MGL_LINE__	__LINE__ /**< ������ �߻��� �ҽ� �� */
#else
	#define	__MGL_FILE__	NULL /**< ������ �߻��� ���� */
	#define __MGL_FUNC__	NULL /**< ������ �߻��� �Լ� */
	#define __MGL_LINE__	0 /**< ������ �߻��� �ҽ� �� */
#endif

#define MGL_DEBUG_MSG_BUF_SIZE		300 /**< ����׿� �޽��� ���� ũ�� */
#define MGL_CONFIG_FPS_CHECK_TICKS	1000 /**< frames per second���� üũ�ϴ� �ֱ� */

#ifdef __cplusplus
}
#endif

#endif // __mgl_config_h_
