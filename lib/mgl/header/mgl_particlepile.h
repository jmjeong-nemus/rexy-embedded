/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @brief library for particle animation
 *
 * @file mgl_particlepile.h
 * 
 * @author <a href="mailto:wichang@mococo.com">Jang Wooil</a>
 */


#ifndef __mgl_ParticlePile_h_
#define __mgl_ParticlePile_h_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mgl.h"
#include "mgl_animator.h"
#include "mgl_ip.h"
	/**
	* @defgroup api_particle_pile_api_for_user Particle Pile API for User
	* 
	* Particle Pile API for User
	*
	* @addtogroup api_particle_pile_api_for_user
	* @{
	* */

#define MGL_PA_PILE_LIFE_DEAD		0	/**< 파티클이 소멸 되었는지를 체크하기 위한 메크로. */

#define MGL_PA_PILE_LIFE_ALIVE		1	/**< 파티클이 살아 있는지 체크하기 위한 메크로. */
	/**
	* @}
	* */

	/**
	* @defgroup api_particle_pile_coordinate_info Particle Pile Coordinate Info
	* 
	* Particle Pile Coordinate Info
	*
	* @addtogroup api_particle_pile_coordinate_info
	* @{
	* */

#define MGL_PA_PILE_NON_VISIBLE		0	/**< 파티클이 백그라운드 또는 포커스 아이콘에 쌓여 있지 않는지 여부를 체크하기 위한 값. */

#define MGL_PA_PILE_TRUE			1	/**< 파티클이 백그라운드에 쌓여 있는지 여부를 체크하기 위한 값. */

#define MGL_PA_PILE_FI_ABSCISSA     0	/**< 파티클이 가로로 쌓일 것인지 결정하기 위한 값. */

#define MGL_PA_PILE_FI_ORDINATE     1	/**< 파티클이 세로로 싸일것인지 결정하기 위한 값. */
	/**
	* @}
	* */

	/**
	* @defgroup api_particle_pile_option_imageprocessing Particle Pile Image Processing Option
	*
	* Particle Pile Image Processing Option
	* 
	* @addtogroup api_particle_pile_option_imageprocessing 
	* @{
	* */

#define MGL_PA_PILE_SET_IMG_PROC_DEFAULT		0	/**< 파티클의 하나의 이미지 ID와, 이미지 IP셋을 사용자로 부터 입력 받는다. */

#define MGL_PA_PILE_SET_IMG_PROC_MANUAL			1	/**< 파티클의 이미지 ID와 , 이미지 IP를 사용자로 부터 선택 받는다. */

#define MGL_PA_PILE_SET_IMG_PROC_KEEP_UP		2	/**< 파티클의 이미지 ID셋과 , 이미지 IP셋을 사용자로 부터 미리 입력 받는다.
이미지와, IP는 1:1 이며, 소멸 될때까지 속성을 그대로 유지한다. */

#define MGL_PA_PILE_SET_IMG_PROC_SPAN_SEQ			3	/**< 파티클의 순차적인 이미지 ID셋과, 이미지 IP셋을 사용자로 부터 미리 입력 받아,
	시작위치를 순차적으로 선택하여, SPAN 단위로 반복한다. */

#define MGL_PA_PILE_SET_IMG_PROC_FRAME_SEQ			4	/**< 파티클의 순차적인 이미지 ID셋과, 이미지 IP셋을 사용자로 부터 미리 입력 받아
	시작위치를 순차적으로 선택하여, 반복한다. */

#define MGL_PA_PILE_SET_IMG_PROC_FRAME_RAND		5	/**< 파티클의 순차적인 이미지 ID셋과, 이미지 IP셋을 사용자로 부터 미리 입력 받아
	시작위치를 랜덤하게 선택하여, 반복한다. */
	/**
	* @}
	* */

	/**
	* @defgroup api_particle_pile_data_structure Particle Pile Data Structure
	*
	* Particle Pile Data Structur
	* 
	* @addtogroup api_particle_pile_data_structure 
	* @{
	* */

	/**
	 * @brief	파티클이 쌓일 포커스 아이콘 영역의 구조체
	 */

	typedef struct MGLpapilefiareainfo
	{
		MGLint x;			/**< Focus Icon의 x좌표  */
		MGLint y;			/**< Focus Icon의 y좌표  */
		MGLint width;		/**< Focus Icon의 width 영역 */
		MGLint height;		/**< Focus Icon의 Height 영역 */
	}

	MGLpapilefiareainfo;

	/**
	 * @brief	파티클이 쌓일 백그라운 영역의 구조체
	 */

	typedef struct MGLpapilebgareainfo
	{
		MGLint x;			/**< 백그라운드의 x좌표 */
		MGLint y;			/**< 백그라운드의 y좌표 */
		MGLint width;		/**< 백그라운드의 Width 영역 */
		MGLint height;		/**< 백그라운드이 Height 영역 */
	}

	MGLpapilebgareainfo;


	/**
	 * @brief	파티클 시스템의 Force(힘) 구조체
	 */

	typedef struct MGLpapileforceinfo
	{
		MGLint fx;				/**< x(방향)으로 작용하는 힘 */
		MGLint fy;				/**< y(방향)으로 작용하는 힘 */
	}

	MGLpapileforceinfo;

	/**
	 * @brief	파티클 시스템의 Factor(외부 요인) 구조체.
	 */

	typedef struct MGLpapilefactorattrib
	{
		MGLfloat afc;				/**< 움직이는 물체의 반대 방향으로 작용하는 공기 저항 */
		MGLfloat dt;				/**< 시간당 위치 변화율 */
		MGLpapileforceinfo gForce;	/**< x,y(방향)으로 작용하는 중력 구조체 */
	}

	MGLpapilefactorattrib;

	/**
	 * @brief	생성되는 파티클의 성질을 결정 하는 구조체
	 */

	typedef struct MGLpapilestdattrib
	{
		MGLint xMin;				/**< 파티클이 생성 될때, 생성될 위치의 x의 최소 값 결정 */
		MGLint xMax;				/**< 파티클이 생성 될때, 생성될 위치의 x의 최대 값 결정 */
		MGLint yMin;				/**< 파티클이 생성 될때, 생성될 위치의 y 값 결정 */
		MGLint yMax;				/**< 파티클이 생성 될때, 생성될 위치의 y 값 결정 */
		MGLint vxMin;				/**< 파티클이 생성 될때, x(방향)으로의 최소 속도 결정 */
		MGLint vxMax;				/**< 파티클이 생성 될때, x(방향)으로의 최대 속도 결정 */
		MGLint vyMin;				/**< 파티클이 생성 될때, y(방향)으로의 최소 속도 결정 */
		MGLint vyMax;				/**< 파티클이 생성 될때, y(방향)으로의 최대 속도 결정 */
		MGLint msMin;				/**< 파티클이 생성 될때, 파티클의 최소 질량 값 결정 */
		MGLint msMax;				/**< 파티클이 생성 될때, 파티클의 최대 질량 값 결정 */
		MGLint spMin;				/**< 파티클이 생성 될때, 파티클의 최소 생성 주기 결정 */
		MGLint spMax;				/**< 파티클이 생성 될때, 파티클의 최대 생성 주기 결정 */
	}

	MGLpapilestdattrib;

	/**
	 * @brief	생성되어 활동중인 파티클의 값을 보관하는 구조체
	 */

	typedef struct MGLpapilestdinfo
	{
		MGLboolean bVisible;	/**< 파티클이 화면에 보여질 것인가? 안보여 질 것인가? */
		MGLint x;				/**< 파티클의 x 좌표값 */
		MGLint y;				/**< 파티클의 y 좌표값 */
		MGLfixed vx;			/**< 파티클의 x(방향)으로의 가속도 값. */
		MGLfixed vy;			/**< 파티클의 y(방향)으로의 가속도 값. */
		MGLfixed ax;			/**< 파티클의 x(방향)으로의 속도 값.*/
		MGLfixed ay;			/**< 파티클의 y(방향)으로의 속도 값.*/
		MGLint mass;			/**< 파티클의 질량 */
		MGLint span;			/**< 파티클의 남은 생명*/
		MGLint imageIndex;		/**< 파티클의 이미지 인덱스 정보. ( 파티클이 몇번째 인덱스의 이미지를 가지고 있는지?) */
		MGLint frameInfo;		/**< 파티클의 현재 프레임 정보.( 이미지가 바뀐 후 몇번 화면에 나왔는지?) */
	}

	MGLpapilestdinfo;


	/**
	 * @brief	파티클을 나타내기 위해 사용할 이미지 / 처리 정보.
	 */

	typedef struct MGLpapileipinfo
	{
		MGLint nDataIndex;			/**< 이미지 프로세싱 데이터 인덱스 정보 */
		MGLint nParamId;			/**< 이미지 프로세싱 파라미터 정보 */
		MGLint * pData;				/**< 이미지 프로세싱 데이타의 배열 정보 */
		MGLvoid * pImageIps;		/**< 이미지 프로세싱 구조체 정보 */
	}

	MGLpapileipinfo;

	/**
	 * @brief	파티클의 이미지를 처리할 사용자 함수 포인터.
	 */

	typedef MGLvoid (*MGLpapileimageprocfuntptr) (MGLpapilestdinfo * pPapilestdinfo);

	/**
	 * @brief	파티클의 이미지 정보/처리 를 포함하고 있는 전체 구조체.
	 */

	typedef struct MGLpapileimageinfo
	{
		MGLint imageProcType;							/**< 이미지 타입 */
		MGLint nImageCnts;								/**< 입력된 이미지의 갯수 정보 */
		MGLint nImageChangeInfo;						/**< 이미지가 어느시점에 바뀌는가에 대한 정보 */
		MGLint * pImageIds;								/**< 입력된 이미지 셋을 구성하는 이미지 리소스 ID 배열 정보. */
		MGLint nIpInfoCnts;								/**< MGLparticleipinfo 구조체의 갯수 정보 */
		MGLint nDataCnts;								/**< 이미지 프로세싱 데이타의 배열 갯수 정보 */
		MGLpapileipinfo * pImageIpInfo;					/**< 이미지 처리 정보와 처리 갯수를 포함하는 구조체 */
		MGLpapileimageprocfuntptr	fpImageProcFuncPtr; /**< 사용자 이미지 담당 함수 포인터 */
	}

	MGLpapileimageinfo;

	/**
	 * @brief	포커스 아이콘에 쌓일 영역의 좌표와, 이미지의 정보를 포함하는 구조체
	 */

	typedef struct MGLpapilefirenderinfo
	{
		MGLint nX;											/**< 파티클이 쌓일 x 좌표 정보 */
		MGLint nY;											/**< 파티클이 쌓일 y 좌표 정보 */
		MGLint pileImage;									/**< 파티클이 쌓일 이미지 정보 */
	}

	MGLpapilefirenderinfo;

	/**
	 * @brief	바탕화면에 쌓일 영역의 좌표와, 이미지의 정보를 포함하는 구조체 
	 */

	typedef struct MGLpapilebgrenderinfo
	{

		MGLint nX;										/**< 파티클이 쌓인 x 좌표 정보 */
		MGLint nY;										/**< 파티클이 쌓일 y 좌표 정보 */
		MGLint pileImage;								/**< 파티클이 쌓일 이미지 정보 */
	}

	MGLpapilebgrenderinfo;

	/**
	 * @brief	파티클 시스템을 구성하는 핵심적인 구조체로, 파티클을 표현하는데 필요한 모든 정보가 등록 되어 있다.
	 */

	typedef struct MGLpapileunityinfo
	{
		MGLint nIndexCnt;									/**< 화면에 출력될 파티클 배열의 인덱스 정보 */
		MGLint nMaxPapileCnts;								/**< 파티클의 최대 갯수 정보 */
		MGLint nBgCoordiSize;								/**< pPileBgCoordiInfo 배열의 크기 정보 */
		MGLint nFiCoordiSize;								/**< pPileFiCoordiInfo 배열의 크기 정보 */
		MGLint nBgRenderSize;								/**< pPapileFiRenderInfo 배열의 크기 정보 */
		MGLint nFiRenderSize;								/**< pPapileBgRenderInfo 배열의 크기 정보 */
		MGLint nBgRenderCnts;								/**< 포커스 아이콘에 한번에 쌓일 파티클 갯수 정보 */
		MGLint nFiRenderCnts;								/**< 바탕화면에 한번에 샇일 파티클 갯수 정보 */	
		MGLbyte * pPileBgCoordiInfo;						/**< 파티클이 쌓이는 바탕화면 위치 정보를 저장할 화면 전체 크기의 배열 */
		MGLbyte * pPileFiCoordiInfo;						/**< 파티클이 쌓이는 포커스 아이콘 위치 정보를  저장할 화면 전체 크기의 배열 */
		MGLpapilebgrenderinfo * pPapileBgRenderInfo;		/**< 파티클이 바탕화면에 쌓일 영역의 좌표와, 이미지의 정보를 포함하는 구조체*/
		MGLpapilefirenderinfo * pPapileFiRenderInfo;		/**< 파티클이 포커스 아이콘에 쌓일 영역의 좌표와, 이미지의 정보를 포함하는 구조체*/
		MGLpapilefiareainfo * pPapileFiAreaInfo;		/**< 파티클이 FI영역에 도달 하면, 뭘해야 하는데, 그 영역을 주는 구조체*/
		MGLpapilebgareainfo * pPapileBgAreaInfo;		/**< 파티클이 BG영역에 도달 하면, 뭘해야 하는데, 그 영역을 주는 구조체*/
		MGLpapileforceinfo papileForceInfo;				/**< 파티클의 힘 구조체 */
		MGLpapilefactorattrib * pPapileFactorAttrib;	/**< 파티클의 Factor(외부 요인) 구조체 */
		MGLpapilestdattrib * pPapileStdAttrib;			/**< 파티클 생성에 필요한 기본 설정 구조체 */
		MGLpapilestdinfo * pPapileStdInfo;				/**< 생성되어진 파티클의 기본 정보 구조체 */
		MGLpapileimageinfo * pPapileImageInfo;			/**< 파티클에 사용되어질 이미지 ID및 이미지 프로세싱 정보 구조체 */
	}

	MGLpapileunityinfo;
	/**
	* @}
	* */

	/* API for User */

	/**
	* @defgroup api_particle_pile_api_for_user Particle Pile API for User
	* 
	* Particle Pile API for User
	*
	* @addtogroup api_particle_pile_api_for_user
	* @{
	* */

	/**
	 * @brief	파티클을 사용하겠다고, 서브시스템에 알려주는 중요한 API
	 * @param	pPapileUnityInfo :파티클의 모든 정보를 포함하는 구조체.
	 * @param	pPapileStdInfo :파티클의 기본 정보를 포함하는 구조체 이며, 어플리케이션 개발자는
	 *							사용할 파티클 갯수 만큼의 구조체 배열을 할당하여, 전달 한다.
	 * @param	pPapileStdAttrib :파티클의 기본 정보를 계산하는데 필요한 기본 Properties 정보이며, 
	 *							  pFactorPpt 구조체 정보와 함께 계산되어, pParticleBasicInfo 구조체에 결과 값을 저장 한다.
	 * @param	pPapileFactorAttrib :파티클 기본 정보를 계산하는데 필요한 Factor Properties 정보이며,
	 *								 pParticlePpt 구조체 정보와 함께 계산되어, pParticleBasicInfo 구조체에 결과 값을 저장 한다.
	 * @param	nMaxPapileCnts : 사용하고자 하는 파티클 최대 갯수.	
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_SetParticlePileSystem(MGLpapileunityinfo * pPapileUnityInfo, \
	                          MGLpapilestdinfo * pPapileStdInfo, \
	                          MGLpapilestdattrib * pPapileStdAttrib, \
	                          MGLpapilefactorattrib * pPapileFactorAttrib, \
	                          MGLint nMaxPapileCnts);

	/**
	 * @brief	파티클을 파티클 최대 갯수만큼 생성한다. 처음엔 모든 파티클을  생성하며, 이후에는 소멸된 파티클만 
	 *          다시 생성 한다.
	 * @param	pPapileUnityInfo : SetParticleSystem() 호출 후 pPapileUnityInfo 구조체에 파티클 생성을 위한 값들이
	 *                             체워지며, 이를 기반으로 적절한 파티클이 생성 된다.
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_CreateParticlePile(MGLpapileunityinfo *  pPapileUnityInfo);

	/**
	 * @brief	파티클이 생성될 최대 갯수를 다시 설정한다. 단, 초기에 설정된 파티클의 갯수를 넘어서는 안되면,
	 *			만약 초기의 최대 파티클 갯수를 넘어 섰다면, 파티클의 수는 초기의 파티클의 수를 유지 한다.
	 * @param	pPapileUnityInfo : 파티클의 최대 갯수를 유지하기 위한 구조체.
	 * @param	maxParticleCnts : 파티클의 최대 갯수.
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_ParticlePileSetMaxCnt(MGLpapileunityinfo *pPapileUnityInfo, MGLint maxParticleCnts);

	/**
	 * @brief	어플리케이션에서, 사용할 Animator와 서브시스템을 등록하는 API.
	 *			해당 API는 실제 어플리케이션에서 사용할 Animator 정보와, Animator에서 사용할 서브시스템간의
	 *			유기적인 연동을 위한 API.
	 * @param	pPapileUnityInfo : 파티클의 모든 정보의 내용을 포함하는 구조체.
	 * @param	pSubsystemInfo : 어플리케이션에서, 사용할 Animator의 서브시스템 구조체.
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_ParticlePileSetAnimator(MGLpapileunityinfo *  pPapileUnityInfo, MGLsubsystem *pSubsystemInfo);

	/**
	 * @brief	어플리케이션에서 사용할 이미지 타입과, 이미지 처리 정보 구조체를 등록하는 API.
	 * @param	pPapileUnityInfo : 사용자가 등록한 이미지 처리 정보 구조체의 내용을 보관하기 위한 구조체. 
	 * @param	pPapileImageInfo : 파티클에서 사용할 이미지의 정보 구조체.
	 * @param	pPapileIpInfo : 파티클에서 사용할 이미지 프로세싱 정보 구조체.
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_ParticlePileSetImageProcSystem ( MGLpapileunityinfo *  pPapileUnityInfo, MGLpapileimageinfo * pPapileImageInfo, MGLpapileipinfo * pPapileIpInfo );

	/**
	 * @brief	어플리케이션에서 사용할 이미지의 배열및 배열 크기를 등록하는 API
	 * @param	pPapileUnityInfo : 사용자가 등록한 이미지 배열의 정보와, 갯수를 보관하기 위한 구조체.
	 * @param	pImageIds : 파티클에서 사용할 이미지 배열.
	 * @param	nImgageCnts : 이미지 배열의 크기.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticlePileSetImagesInit( MGLpapileunityinfo * pPapileUnityInfo, MGLint * pImageIds, MGLint nImgageCnts );

	/**
	 * @brief	파티클에서 사용할 이미지 프로세싱 정보를 포함한 구조체 갯수와, 이미지 프로세싱에서 사용할 데이터의 갯수를 등록하기 위한 API
	 * @param	pPapileUnityInfo : 사용자가 등록한 이미지 프로세싱정보를 포함한 구조체 갯수와 데이터의 갯수를 보관하기 위한 구조체.
	 * @param	nIpInfoCnts : 파티클에서 사용할 이미지 프로세싱정보를 포함한 구조체의 갯수.
	 * @param	nDataCnts : 이미지 프로세싱 데이터의 갯수.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticlePileSetIpInfo ( MGLpapileunityinfo * pPapileUnityInfo, MGLint nIpInfoCnts, MGLint nDataCnts );

	/**
	 * @brief	파티클에서 사용할 이미지 프로세싱 데이터 정보와, 이미지 프로세싱 구조체, 이미지 프로세싱 파라미터와
				이미지 프로세싱 정보를 포함한 구조체의 인덱스 정보를 등록하기 위한 API 
	 * @param	pPapileUnityInfo : 사용자가 등록한 이미지 프로세싱 관련 정보를 저장하기 위한 구조체.
	 * @param	pData : 이미지 프로세싱에서 사용될 데이터의 배열 정보.
	 * @param	pImageIps : 이미지 프로세싱을 담당할 구조체 정보.
	 * @param	nParamId : 어떤 이미지 프로세싱을 수행할 것인지를 결정하는 파라미터 정보.
	 * @param	index : 이미지 프로세싱 관련 정보를 저장할 구조체의 인덱스 정보.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticlePileSetIpInit ( MGLpapileunityinfo * pPapileUnityInfo, MGLint * pData, MGLvoid * pImageIps,
	                            MGLint nParamId, MGLint index );


	/**
	 * @brief	파티클에서 사용할 이미지 와 이미지 프로세싱을 어떠한 방법에 의해 진행 할 것인지 결정하는 API.
	 *
	 * @param	pPapileUnityInfo : 사용자가 등록한 이미지 프로세싱 타입을 저장하기 위한 구조체
	 * @param	imageProcType : 파티클의 이미지 선택과 프로세싱의 타입 정보.
	 * @param	fpImageProcFuncPtr : imageProcType이 MGL_PARTICLE_SET_IMG_PROC_MANUAL이면 사용자 정의 함수 포인터
	 *							   를 등록한다. 그외의 NULL 값을 가진다.
	 * @param	nImageChangeInfo : 파티클 시스템 내부적으로 이미지와 이미지 프로세싱 정보를 변경할 때의 조건 정보
	 *							 imageProcType이 MGL_PARTICLE_SET_IMG_PROC_SPAN_SEQ 이라면, 파티클의 생명주기 조건이며,
	 *							 imageProcType이 MGL_PARTICLE_SET_IMG_PROC_FRAME_SEQ 또는 MGL_PARTICLE_SET_IMG_PROC_FRAME_RAND
	 *							 이라면 프레임 조건이 된다.
	 * @return MGLvoid
	 */

	MGLvoid
	MGL_ParticlePileSetImageProcInit (MGLpapileunityinfo * pPapileUnityInfo, MGLint imageProcType, \
	                                  MGLpapileimageprocfuntptr fpImageProcFuncPtr, MGLint nImageChangeInfo );

	/**
	 * @brief	파티클이 쌓이는 좌표 정보를 유지하기 위한 API. 
	 *
	 * @param	pPapileUnityInfo : 파티클의 쌓이는 좌표 정보 및 이미지 정보를 유지하기 위한 구조체.
	 * @param	pPapileBgRenderInfo : 바탕화면에 파티클이 쌓여야할 위치와, 파티클이 쌓여야 한다면,
	 * 						          어떠한 이미지가 그려질 것인지에 대한 정보를 유지하기 위한 배열.
	 * @param	nBgRenderSize	: pPapileBgRenderInfo 배열의 크기 정보.
	 * @param	pPapileFiRenderInfo : 포커스 아이콘에 파티클이 쌓여야할 위치와, 파티클이 쌓여야 한다면,
	 *								  어떠한 이미지가 그려질 것인지에 대한 정보를 유지하기 위한 배열.
	 * @param	nFiRenderSize	: pPapileFiRenderInfo 배열의 크기 정보.
	 *
	 * @return MGLvoid
	 */

	MGLvoid
	MGL_SetParticlePileRenderInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLpapilebgrenderinfo * pPapileBgRenderInfo, MGLint nBgRenderSize, \
								 MGLpapilefirenderinfo * pPapileFiRenderInfo, MGLint nFiRenderSize);

	/**
	 * @brief	파티클이 쌓일 전체 위치 정보를 유지하기 위한 API. 
	 *
	 * @param	pPapileUnityInfo : 파티클의 쌓여있는 전체 좌표 정보를 유지하기 위한 구조체.
	 * @param	pPapileBgCoordiInfo : 바탕화면의 파티클이 쌓여있는 전체 위치 정보를  유지하기 위한 배열.
	 * @param	nBgCoordiSize	: pPapileBgCoordiInfo 배열의 크기 정보.
	 * @param	pPapileFiCoordiInfo : 포커스 아이콘에 파티클이 쌓여있는 전체 위치 정보를 유지하기 위한 배열.
	 * @param	nFiCoordiSize	: pPapileFiRenderInfo 배열의 크기 정보.
	 *
	 * @return MGLvoid
	 */
	MGLvoid
	MGL_SetParticlePileCoordiInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLbyte * pPapileBgCoordiInfo, MGLint nBgCoordiSize,\
	                              MGLbyte * pPapileFiCoordiInfo, MGLint nFiCoordiSize);

	/**
	 * @brief	바탕화면 / 포커스 아이콘 영역 정보를 설정하기 위한 API.
	 *
	 * @param	pPapileUnityInfo : 포커스 된 정보를 유지하기 위한 구조체
	 * @param	pPapileBgAreaInfo : 바탕화면의 Width, Height 정보를 포함하고 있는 구조체
	 * @param	pPapileFiAreaInfo : 포커스 아이콘의 좌표 정보를 포함하고 있는 구조체.
	 * @return	MGLvoid
	 */

	MGLvoid
	MGL_SetParticlePileAreaInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLpapilebgareainfo * pPapileBgAreaInfo, MGLpapilefiareainfo * pPapileFiAreaInfo);

	/**
	* @brief	파티클이 쌓이 바탕화면 영역의 좌표 정보를 설정한다. 
	*
	* @param	pPapileUnityInfo : 바탕화면 영역 정보를 유지하기 위한 구조체
	* @param	x : 바탕화면의 x영역 좌표.
	* @param	y : 바탕화면의 y영역 좌표.
	* @param	width : 바탕화면의 width 영역.
	* @param	height : 바탕화면의 height 영역.
	* @return	MGLvoid
	*/

	MGLvoid
	MGL_SetParticlePileBgInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLint x, MGLint y, MGLint width, MGLint height );

	/**
	* @brief	파티클이 쌓이 포커스 아이콘 좌표 정보를 설정한다. 
	*
	* @param	pPapileUnityInfo : 바탕화면 영역 정보를 유지하기 위한 구조체
	* @param	x : 포커스 아이콘의 x영역 좌표.
	* @param	y : 포커스 아이콘의 y영역 좌표.
	* @param	width : 포커스 아이콘의 width 영역.
	* @param	height : 포커스 아이콘의 height 영역.
	* @return	MGLvoid
	*/
	MGLvoid
	MGL_SetParticlePileFiInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLint x, MGLint y, MGLint width, MGLint height );

	/**
	* @brief	바탕화면에 파티클이 쌓이는 모습을 그리기 위한 사용자 API.
	*			Custom Layer에서 직접 호출 된다.
	*
	* @param	pBuffer : Custom Layer의 버퍼 정보.
	* @param	pParam : 바탕화면에 그릴 파티클 쌓이는 모습의 좌표 정보를 포함하는 구조체. 
	* @return	MGLvoid
	*/

	MGLvoid
	MGL_DrawParticlePileBgArea(MGLvoid * pBuffer, MGLvoid * pParam );

	/**
	* @brief	포커스 아이콘에 쌓이는 모습을 그리기 위한 사용자 API.
	*			Custom Layer에서 직접 호출 된다.
	*
	* @param	pBuffer : Custom Layer의 버퍼 정보.
	* @param	pParam : 포커스 아이콘위에 파티클 쌓이는 모습의 좌표 정보를 포함하는 구조체. 
	* @return	MGLvoid
	*/

	MGLvoid
	MGL_DrawParticlePileFiArea(MGLvoid * pBuffer, MGLvoid * pParam );

	/**
	* @brief	파티클이 포커스 아이콘위에 쌓였을때, 유지하는 모든 좌표 정보를 삭제 한다.
	*
	* @param	pPapileUnityInfo : 파티클이 포커스 아이콘위에 쌓였을때, 모든 좌표 정보를 포함하는 구조체.
	* @return	MGLvoid
	*/
	MGLvoid
	MGL_ClearParticlePileFiArea(MGLpapileunityinfo * pPapileUnityInfo);//, MGLint width, MGLint height);

	/**
	* @brief	파티클이 바탕화면에 쌓였을때, 유지하는 모든 좌표 정보를 삭제 한다.
	*
	* @param	pPapileUnityInfo : 파티클이 바탕화면에 쌓였을때, 모든 좌표 정보를 포함하는 구조체.
	* @return	MGLvoid
	*/
	MGLvoid
	MGL_ClearParticlePileBgArea(MGLpapileunityinfo * pPapileUnityInfo);//, MGLint width, MGLint height);
	/**
	* @}
	* */

#ifdef __cplusplus
}

#endif

#endif //__mgl_ParticlePilepile_h_
