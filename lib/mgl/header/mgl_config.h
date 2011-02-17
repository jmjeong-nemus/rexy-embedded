/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @file mgl_config.h
 * @brief MGL Core Module : MGL 컴파일을 위한 configuration 값 모음.
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
 * @brief	Configuration 값, 해당 설정값을 사용하겠다는 의미임.
 */

#define MGL_ON		1

/**
 * @brief	Configuration 값, 해당 설정값을 사용하지 않겠다는 의미임.
 */
#define MGL_OFF		0

/**
 * @brief	Fixed point operation 사용함. 0:사용하지 않음. 
 */
#define		MGL_CONFIG_USE_FIXED_POINT			MGL_ON	

/**
 * @brief	Color R(5bit)/G(6bit)/B(5bit) 체계를 사용함
 */
#define		MGL_COLOR_RGB565					MGL_ON				

/**
 * @brief	DropShadow Matrix size(By Size Property)
 */
#define		MGL_DROPSHADOW_MATRIX_SIZE_MAX		499 /**< (2*size)-1 <- size max = 250 */
#define		MGL_DROPSHADOW_MATRIX_ROW_SIZE		250	/**< Maximum matrix row size in Generation matrix */
#define		MGL_DROPSHADOW_MATRIX_COL_SIZE		250	/**< Maximum matrix column size in Generation matrix */

/**
 * @brief	소스 코드내에 디버깅 정보를 넣어줄것인지를 선택함. 
 */
#ifdef _DEBUG
	#define MGL_CONFIG_USE_DEBUG_INFO			
#endif

#ifdef MGL_CONFIG_USE_DEBUG_INFO
	#define	__MGL_FILE__	__FILE__ /**< 에러가 발생한 파일 */
	#define __MGL_FUNC__	NULL /**< 에러가 발생한 함수 */
	#define __MGL_LINE__	__LINE__ /**< 에러가 발생한 소스 줄 */
#else
	#define	__MGL_FILE__	NULL /**< 에러가 발생한 파일 */
	#define __MGL_FUNC__	NULL /**< 에러가 발생한 함수 */
	#define __MGL_LINE__	0 /**< 에러가 발생한 소스 줄 */
#endif

#define MGL_DEBUG_MSG_BUF_SIZE		300 /**< 디버그용 메시지 버퍼 크기 */
#define MGL_CONFIG_FPS_CHECK_TICKS	1000 /**< frames per second값을 체크하는 주기 */

#ifdef __cplusplus
}
#endif

#endif // __mgl_config_h_
