/*
 * Nemustech Premium Graphic Library
 * Copyright (c) 2005 Nemustech, Inc.
 * All rights reserved.
 */

/**
 * @brief library for particle animation
 *
 * @file mgl_particle.h
 * 
 * @author <a href="mailto:wichang@mococo.com">Jang Wooil</a>
 */

#ifndef __mgl_particle_h_
#define __mgl_particle_h_


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* @defgroup api_particle_api_for_user Particle API for User
	* 
	* Particle API for User
	*
	* @addtogroup api_particle_api_for_user
	* @{
	* */

#define MGL_PARTICLE_LIFE_DEAD			0	/**< 파티클이 소멸 되었는지를 체크하기 위한 메크로. */
#define MGL_PARTICLE_LIFE_ALIVE			1	/**< 파티클이 살아 있는지 체크하기 위한 메크로. */
	/**
	* @}
	* */

	/**
	* @defgroup api_particle_option_imageprocessing Particle Image Processing Option
	*
	* Particle Image Processing Option
	* 
	* @addtogroup api_particle_option_imageprocessing 
	* @{
	* */

#define MGL_PARTICLE_SET_IMG_PROC_DEFAULT			0	/**< 파티클의 하나의 이미지 ID와, 하나의 IP 셋. */

#define MGL_PARTICLE_SET_IMG_PROC_MANUAL			1	/**< 파티클의 이미지 ID와 , 이미지 IP를 사용자로 부터 선택 받는다. */

#define MGL_PARTICLE_SET_IMG_PROC_KEEP_UP			2	/**< 파티클의 이미지 ID셋과 , 이미지 IP셋을 사용자로 부터 미리 입력 받는다.
이미지와, IP는 1:1 이며, 소멸 될때까지 속성을 그대로 유지한다. */

#define MGL_PARTICLE_SET_IMG_PROC_SPAN_SEQ			3	/**< 파티클의 순차적인 이미지 ID셋과, 이미지 IP셋을 사용자로 부터 미리 입력 받아,
	시작위치를 순차적으로 선택하여, SPAN 단위로 반복한다. */

#define MGL_PARTICLE_SET_IMG_PROC_FRAME_SEQ			4	/**< 파티클의 순차적인 이미지 ID셋과, 이미지 IP셋을 사용자로 부터 미리 입력 받아
	시작위치를 순차적으로 선택하여, 프레임 단위로 반복한다. */

#define MGL_PARTICLE_SET_IMG_PROC_FRAME_RAND		5	/**< 파티클의 순차적인 이미지 ID셋과, 이미지 IP셋을 사용자로 부터 미리 입력 받아
	시작위치를 랜덤하게 선택하여, 프레임 단위로 반복한다. */
	/**
	* @}
	* */

	/**
	* @defgroup api_particle_data_structure Particle Data Structure
	*
	* Particle Data Structur
	* 
	* @addtogroup api_particle_data_structure 
	* @{
	* */

	/**
	 * @brief	파티클 시스템의 Force(힘) 구조체
	 */

	typedef struct MGLparticleforceppt
	{
		MGLint fx;	/**< x(방향)으로 작용하는 힘 */
		MGLint fy;	/**< y(방향)으로 작용하는 힘 */
	}

	MGLparticleforceppt;

	/**
	 * @brief	파티클 시스템의 Factor(외부 요인) 구조체.
	 */

	typedef struct MGLparticlefactorppt
	{
		MGLfloat airFriction;				/**< 움직이는 물체의 반대 방향으로 작용하는 공기 저항 */
		MGLfloat deltaTime;					/**< 시간당 위치 변화율 */
		MGLparticleforceppt gravityForce;	/**< x,y(방향)으로 작용하는 중력 구조체 */
	}

	MGLparticlefactorppt;

	/**
	 * @brief	생성되는 파티클의 성질을 결정 하는 구조체
	 */

	typedef struct MGLparticlebasicppt
	{
		MGLint xMin;			/**< 파티클이 생성 될때, 생성될 위치의 최소 x 값 결정 */
		MGLint xMax;			/**< 파티클이 생성 될때, 생성될 위치의 최대 x 값 결정 */
		MGLint yMin;			/**< 파티클이 생성 될때, 생성될 위치의 최소 y 값 결정 */
		MGLint yMax;			/**< 파티클이 생성 될때, 생성될 위치의 최대 y 값 결정 */
		MGLint vxMin;			/**< 파티클이 생성 될때, x(방향)으로의 최소 속도 결정 */
		MGLint vxMax;			/**< 파티클이 생성 될때, x(방향)으로의 최대 속도 결정 */
		MGLint vyMin;			/**< 파티클이 생성 될때, y(방향)으로의 최소 속도 결정 */
		MGLint vyMax;			/**< 파티클이 생성 될때, y(방향)으로의 최대 속도 결정 */
		MGLint massMin;			/**< 파티클이 생성 될때, 파티클의 최소 질량 값 결정 */
		MGLint massMax;			/**< 파티클이 생성 될때, 파티클의 최대 질량 값 결정 */
		MGLint spanMin;			/**< 파티클이 생성 될때, 파티클의 최소 생성 주기 결정 */
		MGLint spanMax;			/**< 파티클이 생성 될때, 파티클의 최대 생성 주기 결정 */
	}

	MGLparticlebasicppt;

	/**
	 * @brief	생성되어 활동중인 파티클의 값을 보관하는 구조체
	 */

	typedef struct MGLparticlebasicinfo
	{
		MGLboolean	bVisible;			/**< 파티클이 화면에 보여질 것인가? 안보여 질 것인가? */
		MGLint x;						/**< 파티클의 x 좌표값 */
		MGLint y;						/**< 파티클의 y 좌표값 */
		MGLint ax;						/**< 파티클의 x(방향)으로의 가속도 값. */
		MGLint ay;						/**< 파티클의 y(방향)으로의 가속도 값. */
		MGLint vx;						/**< 파티클의 x(방향)으로의 속도 값.*/
		MGLint vy;						/**< 파티클의 y(방향)으로의 속도 값.*/
		MGLint mass;					/**< 파티클의 질량 */
		MGLint span;					/**< 파티클의 남은 생명*/
		MGLint imageIndex;				/**< 파티클의 현재 프레임 정보.( 이미지가 바뀐 후 몇번 화면에 나왔는지?) */
		MGLint frameInfo;				/**< 파티클의 이미지 인덱스 정보. ( 파티클이 몇번째 인덱스의 이미지를 가지고 있는지?) */
	}

	MGLparticlebasicinfo;

	/**
	 * @brief	파티클을 나타내기 위해 사용할 이미지 / 처리 정보.
	 */

	typedef struct MGLparticleipinfo
	{
		MGLint nDataIndex;			/**< 이미지 프로세싱 데이터 인덱스 정보 */
		MGLint nParamId;			/**< 이미지 프로세싱 파라미터 정보 */
		MGLint * pData;				/**< 이미지 프로세싱 데이타의 배열 정보 */
		MGLvoid * pImageIps;		/**< 이미지 프로세싱 구조체 정보 */
	}

	MGLparticleipinfo;

	/**
	 * @brief	파티클의 이미지를 처리할 사용자 함수 포인터.
	 * @param	pParticleBasicInfo : 파티클의 기본 속성 정보를 포함하는 구조체.
	 */

	typedef MGLvoid (*MGLparticleimageprocfuntptr) (MGLparticlebasicinfo * pParticleBasicInfo);

	/**
	 * @brief	파티클의 이미지 정보/처리 를 포함하고 있는 전체 구조체.
	 */

	typedef struct MGLparticleimageinfo
	{
		MGLint imageProcType;							/**< 이미지 타입 */
		MGLint nImageCnts;								/**< 입력된 이미지의 갯수 정보 */
		MGLint nImageChangeInfo;						/**< 이미지가 어느시점에 바뀌는가에 대한 정보 */
		MGLint * pImageIds;								/**< 입력된 이미지 셋을 구성하는 이미지 리소스 ID 배열 정보. */
		MGLint nIpInfoCnts;								/**< MGLparticleipinfo 구조체의 갯수 정보 */
		MGLint nDataCnts;								/**< 이미지 프로세싱 데이타의 배열 갯수 정보 */
		MGLparticleipinfo * pImageIpInfo;				/**< 이미지 처리 정보와 처리 갯수를 포함하는 구조체 */
		MGLparticleimageprocfuntptr	fpImageProcFuncPtr; /**< 사용자 이미지 담당 함수 포인터 */
	}

	MGLparticleimageinfo;

	/**
	 * @brief	파티클이 나타날 바탕화면 영역 정보 구조체
	 */

	typedef struct MGLparticlebgareainfo
	{
		MGLint x;			/**< 바탕화면의 x좌표 */
		MGLint y;			/**< 바탕화면의 y좌표 */
		MGLint width;		/**< 바탕화면의 Width 영역 */
		MGLint height;		/**< 바탕화면이 Height 영역 */
	}

	MGLparticlebgareainfo;

	/**
	 * @brief	파티클 시스템을 구성하는 핵심적인 구조체로, 파티클을 표현하는데 필요한 모든 정보가 등록 되어 있다.
	 */

	typedef struct MGLparticletotalinfo
	{
		MGLint nIndexCnt;							/**< 화면에 출력될 파티클 배열의 인덱스 정보 */
		MGLint nMaxParticleCnts;					/**< 파티클의 최대 갯수 정보 */
		MGLparticlebgareainfo * pParticleBgAreaInfo; /**< 파티클이 나타날 바탕화면 영역 정보 구조에 */
		MGLparticleforceppt forcePptInfo;			/**< 파티클의 힘 구조체 */
		MGLparticlefactorppt * pFactorPptInfo;		/**< 파티클의 Factor(외부 요인) 구조체 */
		MGLparticlebasicppt * pBasicPptInfo;		/**< 파티클 생성에 필요한 기본 설정 구조체 */
		MGLparticlebasicinfo * pParticleBasicInfo;	/**< 생성되어진 파티클의 기본 정보 구조체 */
		MGLparticleimageinfo * pParticleImageInfo;	/**< 파티클에 사용되어질 이미지 ID및 이미지 프로세싱 정보 구조체 */
	}

	MGLparticletotalinfo;

	/**
	 * @brief	사용자가 생성 되어질 파티클의 x,y 속성을 변경 할 수 있는 구조체. 
	 */

	typedef struct MGLparticlecoordtppt
	{
		MGLint xMin;	/**< 파티클이 생성 될때, 생성될 위치의 최소 x 값 결정 */
		MGLint xMax;	/**< 파티클이 생성 될때, 생성될 위치의 최대 x 값 결정 */
		MGLint yMin;	/**< 파티클이 생성 될때, 생성될 위치의 최소 y 값 결정 */
		MGLint yMax;	/**< 파티클이 생성 될때, 생성될 위치의 최대 y 값 결정 */
	}

	MGLparticlecoordntppt;

	/**
	 * @brief	사용자가 생성 되어질 파티클의 vx,vy 속성을 변경 할 수 있는 구조체.
	 */

	typedef struct MGLparticleveloctppt
	{
		MGLint vxMin;	/**< 파티클이 생성 될때, x(방향)으로의 최소 속도 결정 */
		MGLint vxMax;	/**< 파티클이 생성 될때, x(방향)으로의 최대 속도 결정 */
		MGLint vyMin;	/**< 파티클이 생성 될때, y(방향)으로의 최소 속도 결정 */
		MGLint vyMax;	/**< 파티클이 생성 될때, y(방향)으로의 최대 속도 결정 */
	}

	MGLparticleveloctppt;

	/**
	 * @brief	사용자가 생성 되어질 파티클의 질량 값을 변경 할 수 있는 구조체.
	 */

	typedef struct MGLparticlemassppt
	{
		MGLint massMin;		/**< 파티클이 생성 될때, 파티클의 최소 질량 값 결정 */
		MGLint massMax;		/**< 파티클이 생성 될때, 파티클의 최대 질량 값 결정 */
	}

	MGLparticlemassppt;

	/**
	 * @brief	사용자가 생성 되어질 파티클의 생성 주기 값을 변경 할 수 있는 구조체.
	 */

	typedef struct MGLparticlespanppt
	{
		MGLint spanMin;		/**< 파티클이 생성 될때, 파티클의 최소 생성 주기 결정 */
		MGLint spanMax;		/**< 파티클이 생성 될때, 파티클의 최대 생성 주기 결정 */
	}

	MGLparticlespanppt;

	/**
	* @}
	* */

	/* API for User */

	/**
	* @defgroup api_particle_api_for_user Particle API for User
	* 
	* Particle API for User
	*
	* @addtogroup api_particle_api_for_user
	* @{
	* */

	/**
	 * @brief	파티클을 사용하겠다고, 서브시스템에 알려주는 중요한 API
	 * @param	pParticleTotalInfo :파티클의 모든 정보를 포함하는 구조체.
	 * @param	pParticleBasicInfo :파티클의 기본 정보를 포함하는 구조체 이며, 어플리케이션 개발자는
	 *								사용할 파티클 갯수 만큼의 구조체 배열을 할당하여, 전달 한다.
	 * @param	pParticlePpt :파티클의 기본 정보를 계산하는데 필요한 기본 Properties 정보이며, 
	 *						  pFactorPpt 구조체 정보와 함께 계산되어, pParticleBasicInfo 구조체에 결과 값을 저장 한다.
	 * @param	pFactorPpt :파티클 기본 정보를 계산하는데 필요한 Factor Properties 정보이며,
	 *                      pParticlePpt 구조체 정보와 함께 계산되어, pParticleBasicInfo 구조체에 결과 값을 저장 한다.
	 * @param	nMaxParticleCnts : 사용하고자 하는 파티클 최대 갯수.	
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_SetParticleSystem(MGLparticletotalinfo * pParticleTotalInfo, \
	                      MGLparticlebasicinfo * pParticleBasicInfo, \
	                      MGLparticlebasicppt * pParticlePpt, \
	                      MGLparticlefactorppt * pFactorPpt, \
	                      MGLint nMaxParticleCnts);

	/**
	 * @brief	파티클을 파티클 최대 갯수만큼 생성한다. 처음엔 모든 파티클을  생성하며, 이후에는 소멸된 파티클만 
	 *          다시 생성 한다.
	 * @param	pParticleTotalInfo : SetParticleSystem() 호출 후 pParticleTotalInfo 구조체에 파티클 생성을 위한 값들이
	 *                               체워지며, 이를 기반으로 적절한 파티클이 생성 된다.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_CreateParticle(MGLparticletotalinfo * pParticleTotalInfo);

	/**
	 * @brief	파티클 생성을 위한 기본 속성을 변경 적용하여, 파티클을 생성할때 쓰이는 API
	 * @param	pParticleTotalInfo : pParticlePpt 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	pParticlePpt : 변경할 파티클 기본 속성 구조체 정보를 사용자로 부터 할당 받기 위한 구조체.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetBasicPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlebasicppt * pParticlePpt);

	/**
	 * @brief	파티클 생성을 위한 Factor 속성을 변경 적용하여, 파티클을 생성할때 쓰이는 API.
	 * @param	pParticleTotalInfo : pFactorPpt 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	pFactorPpt : 변경할 파티클 Factor 구조체 정보를 사용자로 부터 할당 받기 위한 구조체.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetFactorPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlefactorppt * pFactorPpt);

	/**
	 * @brief	파티클 생성을 위한 좌표 속성을 변경 적용하여, 파티클을 생성할때 쓰이는 API.
	 * @param	pParticleTotalInfo : coordinate 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	pCoordinate : 변경할 파티클 coordinate 구조체 정보를 사용자로 부터 할당 받기 위한 구조체.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetCoordtPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlecoordntppt *pCoordinate);

	/**
	 * @brief	파티클 생성을 위한 속도 속성을 변경 적용하여, 파티클을 생성할때 쓰이는 API.
	 * @param	pParticleTotalInfo : 속도 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	pVelocity : 변경할 파티클 속도 구조체 정보를 사용자로 부터 할당 받기 위한 구조체.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetVeloctPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticleveloctppt *pVelocity);

	/**
	 * @brief	파티클 생성을 위한 질량 속성을 변경 적용하여, 파티클을 생성할때 쓰이는 API.
	 * @param	pParticleTotalInfo : 질량 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	pMass : 변경할 파티클 질량 구조체 정보를 사용자로 부터 할당 받기 위한 구조체.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetMassPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlemassppt *pMass);

	/**
	 * @brief	파티클 생성을 위한 생성 주기 속성을 변경 적용하여, 파티클을 생성할때 쓰이는 API.
	 * @param	pParticleTotalInfo : 생성 주기 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	pSpan : 변경할 파티클 생성 주기 구조체 정보를 사용자로 부터 할당 받기 위한 구조체.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetSpanPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlespanppt *pSpan);

	/**
	 * @brief	파티클 생성을 위한 파티클 갯수를  변경 적용하여, 파티클을 생성할때 쓰이는 API.
	 * @param	pParticleTotalInfo : 파티클 갯수 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	nMaxParticleCnts : 변경할 파티클 갯수 정보를 사용자로 부터 할당 받기 위한 구조체.
	 *			단, 파티클 최대 갯수는 초기 사용자가 입력한 최대 갯수를 벗어 날 수 없고, 초기 설정된
	 *			파티클의 갯수를 초과 하여 입력 되면, 변경한 값은 최기 설정된 파티클 갯수로 저장 된다.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetMaxCnt(MGLparticletotalinfo * pParticleTotalInfo, MGLint nMaxParticleCnts);

	/**
	 * @brief	파티클 생성을 위한 파티클 외부 Factor중 공기저항 계수를 변경하여,  새로운 파티클을 생성하기 위한 API
	 * @param	pParticleTotalInfo : 파티클 갯수 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	fAirFicton : 변경할 파티클의 공기저항 계수를 사용자로 부터 할당 받기 위한 변수.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetAfcPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLfloat fAirFicton);

	/**
	 * @brief	파티클 생성을 위한 파티클 외부 Factor중 시간당 위치 변화율의 속성을  변경하여,
	 *			새로운 파티클을 생성하기 위한 API
	 * @param	pParticleTotalInfo : 파티클 갯수 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	fDeltaTime : 변경할 파티클의 시간당 위치 변화율의 값을  사용자로 부터 할당 받기 위한 변수.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetDtimePpt(MGLparticletotalinfo * pParticleTotalInfo, MGLfloat fDeltaTime);

	/**
	 * @brief	파티클 생성을 위한 파티클 외부 Factor중 중력의 속성을 변경하여, 
	 *			새로운 파티클을 생성하기 위한 API
	 * @param	pParticleTotalInfo : 파티클 갯수 정보를 다시 갱신하여, 저장하기 위한 구조체.
	 * @param	nGForceX : x(방향)의 중력 재 설정.
	 * @param	nGForceY : y(방향)의 중력 재 설정.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetGforcePpt(MGLparticletotalinfo * pParticleTotalInfo, MGLint nGForceX , MGLint nGForceY );

	/**
	 * @brief	어플리케이션에서, 사용할 Animator와 서브시스템을 등록하는 API.
	 *			해당 API는 실제 어플리케이션에서 사용할 Animator 정보와, Animator에서 사용할 서브시스템간의
	 *			유기적인 연동을 위한 API.
	 * @param	pParticleTotalInfo : 파티클의 모든 정보의 내용을 포함하는 구조체.
	 * @param	pSubsystemInfo : 어플리케이션에서, 사용할 Animator의 서브시스템 구조체.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetAnimator(MGLparticletotalinfo * pParticleTotalInfo, MGLsubsystem *pSubsystemInfo);

	/**
	 * @brief	어플리케이션에서 사용할 이미지 타입과, 이미지 처리 정보 구조체를 등록하는 API.
	 * @param	pParticleTotalInfo : 사용자가 등록한 이미지 처리 정보 구조체의 내용을 보관하기 위한 구조체. 
	 * @param	pParticleImageInfo : 파티클에서 사용할 이미지의 정보 구조체.
	 * @param	pParticleIpInfo : 파티클에서 사용할 이미지 프로세싱 정보 구조체.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetImageProcSystem ( MGLparticletotalinfo * pParticleTotalInfo, MGLparticleimageinfo * pParticleImageInfo, MGLparticleipinfo * pParticleIpInfo );

	/**
	 * @brief	어플리케이션에서 사용할 이미지의 배열및 배열 크기를 등록하는 API
	 * @param	pParticleTotalInfo : 사용자가 등록한 이미지 배열의 정보와, 갯수를 보관하기 위한 구조체.
	 * @param	pImageIds : 파티클에서 사용할 이미지 배열.
	 * @param	nImgageCnts : 이미지 배열의 크기.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticleSetImagesInit( MGLparticletotalinfo * pParticleTotalInfo, MGLint * pImageIds, MGLint nImgageCnts );

	/**
	 * @brief	파티클에서 사용할 이미지 프로세싱 정보를 포함한 구조체 갯수와, 이미지 프로세싱에서 사용할 데이터의 갯수를 등록하기 위한 API
	 * @param	pParticleTotalInfo : 사용자가 등록한 이미지 프로세싱정보를 포함한 구조체 갯수와 데이터의 갯수를 보관하기 위한 구조체.
	 * @param	nIpInfoCnts : 파티클에서 사용할 이미지 프로세싱정보를 포함한 구조체의 갯수.
	 * @param	nDataCnts : 이미지 프로세싱 데이터의 갯수.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticleSetIpInfo ( MGLparticletotalinfo * pParticleTotalInfo, MGLint nIpInfoCnts, MGLint nDataCnts );

	/**
	 * @brief	파티클에서 사용할 이미지 프로세싱 데이터 정보와, 이미지 프로세싱 구조체, 이미지 프로세싱 파라미터와
				이미지 프로세싱 정보를 포함한 구조체의 인덱스 정보를 등록하기 위한 API 
	 * @param	pParticleTotalInfo : 사용자가 등록한 이미지 프로세싱 관련 정보를 저장하기 위한 구조체.
	 * @param	pData : 이미지 프로세싱에서 사용될 데이터의 배열 정보.
	 * @param	pImageIps : 이미지 프로세싱을 담당할 구조체 정보.
	 * @param	nParamId : 어떤 이미지 프로세싱을 수행할 것인지를 결정하는 파라미터 정보.
	 * @param	index : 이미지 프로세싱 관련 정보를 저장할 구조체의 인덱스 정보.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticleSetIpInit ( MGLparticletotalinfo * pParticleTotalInfo, MGLint * pData, MGLvoid * pImageIps,
	                        MGLint nParamId, MGLint index );

	/**
	 * @brief	파티클에서 사용할 이미지 와 이미지 프로세싱을 어떠한 방법에 의해 진행 할 것인지 결정하는 API.
	 *
	 * @param	pParticleTotalInfo : 사용자가 등록한 이미지 프로세싱 타입을 저장하기 위한 구조체
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
	MGL_ParticleSetImageProcInit (MGLparticletotalinfo * pParticleTotalInfo, MGLint imageProcType, \
	                              MGLparticleimageprocfuntptr fpImageProcFuncPtr, MGLint nImageChangeInfo );

	/**
	 * @brief	바탕화면 영역 정보를 설정하기 위한 API.
	 *
	 * @param	pParticleTotalInfo : 파티클이 나타날 영역의 정보를 저장하기 위한 구조체.
	 * @param	pParticleBgAreaInfo : 바탕화면의 좌표(x, y) 정보와 크기(Width, Height) 정보를 포함하고 있는 구조체
	 * @return	MGLvoid
	 */

	MGLvoid
	MGL_SetParticleAreaInfo(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlebgareainfo * pParticleBgAreaInfo);
	
	/**
	 * @brief	바탕화면 영역 정보를 설정하기 위한 API.
	 *
	 * @param	pParticleTotalInfo : 파티클이 나타날 영역의 정보를 저장하기 위한 구조체.
	 * @param	x : 바탕화면의 x 좌표 정보.
	 * @param	y : 바탕화면의 y 좌표 정보.
	 * @param	width  : 바탕화면의 Width 영역 정보.
	 * @param	height : 바탕화면의 height 영역 정보.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_SetParticleBgInfo( MGLparticletotalinfo * pParticleTotalInfo, MGLint x, MGLint y, MGLint width, MGLint height );


	/**
	* @}
	* */
#ifdef __cplusplus
}

#endif

#endif // __mgl_particle_h_
