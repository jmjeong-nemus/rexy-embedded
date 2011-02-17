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

#define MGL_PARTICLE_LIFE_DEAD			0	/**< ��ƼŬ�� �Ҹ� �Ǿ������� üũ�ϱ� ���� ��ũ��. */
#define MGL_PARTICLE_LIFE_ALIVE			1	/**< ��ƼŬ�� ��� �ִ��� üũ�ϱ� ���� ��ũ��. */
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

#define MGL_PARTICLE_SET_IMG_PROC_DEFAULT			0	/**< ��ƼŬ�� �ϳ��� �̹��� ID��, �ϳ��� IP ��. */

#define MGL_PARTICLE_SET_IMG_PROC_MANUAL			1	/**< ��ƼŬ�� �̹��� ID�� , �̹��� IP�� ����ڷ� ���� ���� �޴´�. */

#define MGL_PARTICLE_SET_IMG_PROC_KEEP_UP			2	/**< ��ƼŬ�� �̹��� ID�°� , �̹��� IP���� ����ڷ� ���� �̸� �Է� �޴´�.
�̹�����, IP�� 1:1 �̸�, �Ҹ� �ɶ����� �Ӽ��� �״�� �����Ѵ�. */

#define MGL_PARTICLE_SET_IMG_PROC_SPAN_SEQ			3	/**< ��ƼŬ�� �������� �̹��� ID�°�, �̹��� IP���� ����ڷ� ���� �̸� �Է� �޾�,
	������ġ�� ���������� �����Ͽ�, SPAN ������ �ݺ��Ѵ�. */

#define MGL_PARTICLE_SET_IMG_PROC_FRAME_SEQ			4	/**< ��ƼŬ�� �������� �̹��� ID�°�, �̹��� IP���� ����ڷ� ���� �̸� �Է� �޾�
	������ġ�� ���������� �����Ͽ�, ������ ������ �ݺ��Ѵ�. */

#define MGL_PARTICLE_SET_IMG_PROC_FRAME_RAND		5	/**< ��ƼŬ�� �������� �̹��� ID�°�, �̹��� IP���� ����ڷ� ���� �̸� �Է� �޾�
	������ġ�� �����ϰ� �����Ͽ�, ������ ������ �ݺ��Ѵ�. */
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
	 * @brief	��ƼŬ �ý����� Force(��) ����ü
	 */

	typedef struct MGLparticleforceppt
	{
		MGLint fx;	/**< x(����)���� �ۿ��ϴ� �� */
		MGLint fy;	/**< y(����)���� �ۿ��ϴ� �� */
	}

	MGLparticleforceppt;

	/**
	 * @brief	��ƼŬ �ý����� Factor(�ܺ� ����) ����ü.
	 */

	typedef struct MGLparticlefactorppt
	{
		MGLfloat airFriction;				/**< �����̴� ��ü�� �ݴ� �������� �ۿ��ϴ� ���� ���� */
		MGLfloat deltaTime;					/**< �ð��� ��ġ ��ȭ�� */
		MGLparticleforceppt gravityForce;	/**< x,y(����)���� �ۿ��ϴ� �߷� ����ü */
	}

	MGLparticlefactorppt;

	/**
	 * @brief	�����Ǵ� ��ƼŬ�� ������ ���� �ϴ� ����ü
	 */

	typedef struct MGLparticlebasicppt
	{
		MGLint xMin;			/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� �ּ� x �� ���� */
		MGLint xMax;			/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� �ִ� x �� ���� */
		MGLint yMin;			/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� �ּ� y �� ���� */
		MGLint yMax;			/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� �ִ� y �� ���� */
		MGLint vxMin;			/**< ��ƼŬ�� ���� �ɶ�, x(����)������ �ּ� �ӵ� ���� */
		MGLint vxMax;			/**< ��ƼŬ�� ���� �ɶ�, x(����)������ �ִ� �ӵ� ���� */
		MGLint vyMin;			/**< ��ƼŬ�� ���� �ɶ�, y(����)������ �ּ� �ӵ� ���� */
		MGLint vyMax;			/**< ��ƼŬ�� ���� �ɶ�, y(����)������ �ִ� �ӵ� ���� */
		MGLint massMin;			/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ּ� ���� �� ���� */
		MGLint massMax;			/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ִ� ���� �� ���� */
		MGLint spanMin;			/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ּ� ���� �ֱ� ���� */
		MGLint spanMax;			/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ִ� ���� �ֱ� ���� */
	}

	MGLparticlebasicppt;

	/**
	 * @brief	�����Ǿ� Ȱ������ ��ƼŬ�� ���� �����ϴ� ����ü
	 */

	typedef struct MGLparticlebasicinfo
	{
		MGLboolean	bVisible;			/**< ��ƼŬ�� ȭ�鿡 ������ ���ΰ�? �Ⱥ��� �� ���ΰ�? */
		MGLint x;						/**< ��ƼŬ�� x ��ǥ�� */
		MGLint y;						/**< ��ƼŬ�� y ��ǥ�� */
		MGLint ax;						/**< ��ƼŬ�� x(����)������ ���ӵ� ��. */
		MGLint ay;						/**< ��ƼŬ�� y(����)������ ���ӵ� ��. */
		MGLint vx;						/**< ��ƼŬ�� x(����)������ �ӵ� ��.*/
		MGLint vy;						/**< ��ƼŬ�� y(����)������ �ӵ� ��.*/
		MGLint mass;					/**< ��ƼŬ�� ���� */
		MGLint span;					/**< ��ƼŬ�� ���� ����*/
		MGLint imageIndex;				/**< ��ƼŬ�� ���� ������ ����.( �̹����� �ٲ� �� ��� ȭ�鿡 ���Դ���?) */
		MGLint frameInfo;				/**< ��ƼŬ�� �̹��� �ε��� ����. ( ��ƼŬ�� ���° �ε����� �̹����� ������ �ִ���?) */
	}

	MGLparticlebasicinfo;

	/**
	 * @brief	��ƼŬ�� ��Ÿ���� ���� ����� �̹��� / ó�� ����.
	 */

	typedef struct MGLparticleipinfo
	{
		MGLint nDataIndex;			/**< �̹��� ���μ��� ������ �ε��� ���� */
		MGLint nParamId;			/**< �̹��� ���μ��� �Ķ���� ���� */
		MGLint * pData;				/**< �̹��� ���μ��� ����Ÿ�� �迭 ���� */
		MGLvoid * pImageIps;		/**< �̹��� ���μ��� ����ü ���� */
	}

	MGLparticleipinfo;

	/**
	 * @brief	��ƼŬ�� �̹����� ó���� ����� �Լ� ������.
	 * @param	pParticleBasicInfo : ��ƼŬ�� �⺻ �Ӽ� ������ �����ϴ� ����ü.
	 */

	typedef MGLvoid (*MGLparticleimageprocfuntptr) (MGLparticlebasicinfo * pParticleBasicInfo);

	/**
	 * @brief	��ƼŬ�� �̹��� ����/ó�� �� �����ϰ� �ִ� ��ü ����ü.
	 */

	typedef struct MGLparticleimageinfo
	{
		MGLint imageProcType;							/**< �̹��� Ÿ�� */
		MGLint nImageCnts;								/**< �Էµ� �̹����� ���� ���� */
		MGLint nImageChangeInfo;						/**< �̹����� ��������� �ٲ�°��� ���� ���� */
		MGLint * pImageIds;								/**< �Էµ� �̹��� ���� �����ϴ� �̹��� ���ҽ� ID �迭 ����. */
		MGLint nIpInfoCnts;								/**< MGLparticleipinfo ����ü�� ���� ���� */
		MGLint nDataCnts;								/**< �̹��� ���μ��� ����Ÿ�� �迭 ���� ���� */
		MGLparticleipinfo * pImageIpInfo;				/**< �̹��� ó�� ������ ó�� ������ �����ϴ� ����ü */
		MGLparticleimageprocfuntptr	fpImageProcFuncPtr; /**< ����� �̹��� ��� �Լ� ������ */
	}

	MGLparticleimageinfo;

	/**
	 * @brief	��ƼŬ�� ��Ÿ�� ����ȭ�� ���� ���� ����ü
	 */

	typedef struct MGLparticlebgareainfo
	{
		MGLint x;			/**< ����ȭ���� x��ǥ */
		MGLint y;			/**< ����ȭ���� y��ǥ */
		MGLint width;		/**< ����ȭ���� Width ���� */
		MGLint height;		/**< ����ȭ���� Height ���� */
	}

	MGLparticlebgareainfo;

	/**
	 * @brief	��ƼŬ �ý����� �����ϴ� �ٽ����� ����ü��, ��ƼŬ�� ǥ���ϴµ� �ʿ��� ��� ������ ��� �Ǿ� �ִ�.
	 */

	typedef struct MGLparticletotalinfo
	{
		MGLint nIndexCnt;							/**< ȭ�鿡 ��µ� ��ƼŬ �迭�� �ε��� ���� */
		MGLint nMaxParticleCnts;					/**< ��ƼŬ�� �ִ� ���� ���� */
		MGLparticlebgareainfo * pParticleBgAreaInfo; /**< ��ƼŬ�� ��Ÿ�� ����ȭ�� ���� ���� ������ */
		MGLparticleforceppt forcePptInfo;			/**< ��ƼŬ�� �� ����ü */
		MGLparticlefactorppt * pFactorPptInfo;		/**< ��ƼŬ�� Factor(�ܺ� ����) ����ü */
		MGLparticlebasicppt * pBasicPptInfo;		/**< ��ƼŬ ������ �ʿ��� �⺻ ���� ����ü */
		MGLparticlebasicinfo * pParticleBasicInfo;	/**< �����Ǿ��� ��ƼŬ�� �⺻ ���� ����ü */
		MGLparticleimageinfo * pParticleImageInfo;	/**< ��ƼŬ�� ���Ǿ��� �̹��� ID�� �̹��� ���μ��� ���� ����ü */
	}

	MGLparticletotalinfo;

	/**
	 * @brief	����ڰ� ���� �Ǿ��� ��ƼŬ�� x,y �Ӽ��� ���� �� �� �ִ� ����ü. 
	 */

	typedef struct MGLparticlecoordtppt
	{
		MGLint xMin;	/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� �ּ� x �� ���� */
		MGLint xMax;	/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� �ִ� x �� ���� */
		MGLint yMin;	/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� �ּ� y �� ���� */
		MGLint yMax;	/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� �ִ� y �� ���� */
	}

	MGLparticlecoordntppt;

	/**
	 * @brief	����ڰ� ���� �Ǿ��� ��ƼŬ�� vx,vy �Ӽ��� ���� �� �� �ִ� ����ü.
	 */

	typedef struct MGLparticleveloctppt
	{
		MGLint vxMin;	/**< ��ƼŬ�� ���� �ɶ�, x(����)������ �ּ� �ӵ� ���� */
		MGLint vxMax;	/**< ��ƼŬ�� ���� �ɶ�, x(����)������ �ִ� �ӵ� ���� */
		MGLint vyMin;	/**< ��ƼŬ�� ���� �ɶ�, y(����)������ �ּ� �ӵ� ���� */
		MGLint vyMax;	/**< ��ƼŬ�� ���� �ɶ�, y(����)������ �ִ� �ӵ� ���� */
	}

	MGLparticleveloctppt;

	/**
	 * @brief	����ڰ� ���� �Ǿ��� ��ƼŬ�� ���� ���� ���� �� �� �ִ� ����ü.
	 */

	typedef struct MGLparticlemassppt
	{
		MGLint massMin;		/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ּ� ���� �� ���� */
		MGLint massMax;		/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ִ� ���� �� ���� */
	}

	MGLparticlemassppt;

	/**
	 * @brief	����ڰ� ���� �Ǿ��� ��ƼŬ�� ���� �ֱ� ���� ���� �� �� �ִ� ����ü.
	 */

	typedef struct MGLparticlespanppt
	{
		MGLint spanMin;		/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ּ� ���� �ֱ� ���� */
		MGLint spanMax;		/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ִ� ���� �ֱ� ���� */
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
	 * @brief	��ƼŬ�� ����ϰڴٰ�, ����ý��ۿ� �˷��ִ� �߿��� API
	 * @param	pParticleTotalInfo :��ƼŬ�� ��� ������ �����ϴ� ����ü.
	 * @param	pParticleBasicInfo :��ƼŬ�� �⺻ ������ �����ϴ� ����ü �̸�, ���ø����̼� �����ڴ�
	 *								����� ��ƼŬ ���� ��ŭ�� ����ü �迭�� �Ҵ��Ͽ�, ���� �Ѵ�.
	 * @param	pParticlePpt :��ƼŬ�� �⺻ ������ ����ϴµ� �ʿ��� �⺻ Properties �����̸�, 
	 *						  pFactorPpt ����ü ������ �Բ� ���Ǿ�, pParticleBasicInfo ����ü�� ��� ���� ���� �Ѵ�.
	 * @param	pFactorPpt :��ƼŬ �⺻ ������ ����ϴµ� �ʿ��� Factor Properties �����̸�,
	 *                      pParticlePpt ����ü ������ �Բ� ���Ǿ�, pParticleBasicInfo ����ü�� ��� ���� ���� �Ѵ�.
	 * @param	nMaxParticleCnts : ����ϰ��� �ϴ� ��ƼŬ �ִ� ����.	
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_SetParticleSystem(MGLparticletotalinfo * pParticleTotalInfo, \
	                      MGLparticlebasicinfo * pParticleBasicInfo, \
	                      MGLparticlebasicppt * pParticlePpt, \
	                      MGLparticlefactorppt * pFactorPpt, \
	                      MGLint nMaxParticleCnts);

	/**
	 * @brief	��ƼŬ�� ��ƼŬ �ִ� ������ŭ �����Ѵ�. ó���� ��� ��ƼŬ��  �����ϸ�, ���Ŀ��� �Ҹ�� ��ƼŬ�� 
	 *          �ٽ� ���� �Ѵ�.
	 * @param	pParticleTotalInfo : SetParticleSystem() ȣ�� �� pParticleTotalInfo ����ü�� ��ƼŬ ������ ���� ������
	 *                               ü������, �̸� ������� ������ ��ƼŬ�� ���� �ȴ�.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_CreateParticle(MGLparticletotalinfo * pParticleTotalInfo);

	/**
	 * @brief	��ƼŬ ������ ���� �⺻ �Ӽ��� ���� �����Ͽ�, ��ƼŬ�� �����Ҷ� ���̴� API
	 * @param	pParticleTotalInfo : pParticlePpt ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	pParticlePpt : ������ ��ƼŬ �⺻ �Ӽ� ����ü ������ ����ڷ� ���� �Ҵ� �ޱ� ���� ����ü.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetBasicPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlebasicppt * pParticlePpt);

	/**
	 * @brief	��ƼŬ ������ ���� Factor �Ӽ��� ���� �����Ͽ�, ��ƼŬ�� �����Ҷ� ���̴� API.
	 * @param	pParticleTotalInfo : pFactorPpt ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	pFactorPpt : ������ ��ƼŬ Factor ����ü ������ ����ڷ� ���� �Ҵ� �ޱ� ���� ����ü.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetFactorPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlefactorppt * pFactorPpt);

	/**
	 * @brief	��ƼŬ ������ ���� ��ǥ �Ӽ��� ���� �����Ͽ�, ��ƼŬ�� �����Ҷ� ���̴� API.
	 * @param	pParticleTotalInfo : coordinate ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	pCoordinate : ������ ��ƼŬ coordinate ����ü ������ ����ڷ� ���� �Ҵ� �ޱ� ���� ����ü.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetCoordtPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlecoordntppt *pCoordinate);

	/**
	 * @brief	��ƼŬ ������ ���� �ӵ� �Ӽ��� ���� �����Ͽ�, ��ƼŬ�� �����Ҷ� ���̴� API.
	 * @param	pParticleTotalInfo : �ӵ� ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	pVelocity : ������ ��ƼŬ �ӵ� ����ü ������ ����ڷ� ���� �Ҵ� �ޱ� ���� ����ü.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetVeloctPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticleveloctppt *pVelocity);

	/**
	 * @brief	��ƼŬ ������ ���� ���� �Ӽ��� ���� �����Ͽ�, ��ƼŬ�� �����Ҷ� ���̴� API.
	 * @param	pParticleTotalInfo : ���� ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	pMass : ������ ��ƼŬ ���� ����ü ������ ����ڷ� ���� �Ҵ� �ޱ� ���� ����ü.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetMassPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlemassppt *pMass);

	/**
	 * @brief	��ƼŬ ������ ���� ���� �ֱ� �Ӽ��� ���� �����Ͽ�, ��ƼŬ�� �����Ҷ� ���̴� API.
	 * @param	pParticleTotalInfo : ���� �ֱ� ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	pSpan : ������ ��ƼŬ ���� �ֱ� ����ü ������ ����ڷ� ���� �Ҵ� �ޱ� ���� ����ü.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetSpanPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlespanppt *pSpan);

	/**
	 * @brief	��ƼŬ ������ ���� ��ƼŬ ������  ���� �����Ͽ�, ��ƼŬ�� �����Ҷ� ���̴� API.
	 * @param	pParticleTotalInfo : ��ƼŬ ���� ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	nMaxParticleCnts : ������ ��ƼŬ ���� ������ ����ڷ� ���� �Ҵ� �ޱ� ���� ����ü.
	 *			��, ��ƼŬ �ִ� ������ �ʱ� ����ڰ� �Է��� �ִ� ������ ���� �� �� ����, �ʱ� ������
	 *			��ƼŬ�� ������ �ʰ� �Ͽ� �Է� �Ǹ�, ������ ���� �ֱ� ������ ��ƼŬ ������ ���� �ȴ�.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetMaxCnt(MGLparticletotalinfo * pParticleTotalInfo, MGLint nMaxParticleCnts);

	/**
	 * @brief	��ƼŬ ������ ���� ��ƼŬ �ܺ� Factor�� �������� ����� �����Ͽ�,  ���ο� ��ƼŬ�� �����ϱ� ���� API
	 * @param	pParticleTotalInfo : ��ƼŬ ���� ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	fAirFicton : ������ ��ƼŬ�� �������� ����� ����ڷ� ���� �Ҵ� �ޱ� ���� ����.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetAfcPpt(MGLparticletotalinfo * pParticleTotalInfo, MGLfloat fAirFicton);

	/**
	 * @brief	��ƼŬ ������ ���� ��ƼŬ �ܺ� Factor�� �ð��� ��ġ ��ȭ���� �Ӽ���  �����Ͽ�,
	 *			���ο� ��ƼŬ�� �����ϱ� ���� API
	 * @param	pParticleTotalInfo : ��ƼŬ ���� ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	fDeltaTime : ������ ��ƼŬ�� �ð��� ��ġ ��ȭ���� ����  ����ڷ� ���� �Ҵ� �ޱ� ���� ����.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetDtimePpt(MGLparticletotalinfo * pParticleTotalInfo, MGLfloat fDeltaTime);

	/**
	 * @brief	��ƼŬ ������ ���� ��ƼŬ �ܺ� Factor�� �߷��� �Ӽ��� �����Ͽ�, 
	 *			���ο� ��ƼŬ�� �����ϱ� ���� API
	 * @param	pParticleTotalInfo : ��ƼŬ ���� ������ �ٽ� �����Ͽ�, �����ϱ� ���� ����ü.
	 * @param	nGForceX : x(����)�� �߷� �� ����.
	 * @param	nGForceY : y(����)�� �߷� �� ����.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetGforcePpt(MGLparticletotalinfo * pParticleTotalInfo, MGLint nGForceX , MGLint nGForceY );

	/**
	 * @brief	���ø����̼ǿ���, ����� Animator�� ����ý����� ����ϴ� API.
	 *			�ش� API�� ���� ���ø����̼ǿ��� ����� Animator ������, Animator���� ����� ����ý��۰���
	 *			�������� ������ ���� API.
	 * @param	pParticleTotalInfo : ��ƼŬ�� ��� ������ ������ �����ϴ� ����ü.
	 * @param	pSubsystemInfo : ���ø����̼ǿ���, ����� Animator�� ����ý��� ����ü.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetAnimator(MGLparticletotalinfo * pParticleTotalInfo, MGLsubsystem *pSubsystemInfo);

	/**
	 * @brief	���ø����̼ǿ��� ����� �̹��� Ÿ�԰�, �̹��� ó�� ���� ����ü�� ����ϴ� API.
	 * @param	pParticleTotalInfo : ����ڰ� ����� �̹��� ó�� ���� ����ü�� ������ �����ϱ� ���� ����ü. 
	 * @param	pParticleImageInfo : ��ƼŬ���� ����� �̹����� ���� ����ü.
	 * @param	pParticleIpInfo : ��ƼŬ���� ����� �̹��� ���μ��� ���� ����ü.
	 * @return	MGLvoid 
	 */
	MGLvoid
	MGL_ParticleSetImageProcSystem ( MGLparticletotalinfo * pParticleTotalInfo, MGLparticleimageinfo * pParticleImageInfo, MGLparticleipinfo * pParticleIpInfo );

	/**
	 * @brief	���ø����̼ǿ��� ����� �̹����� �迭�� �迭 ũ�⸦ ����ϴ� API
	 * @param	pParticleTotalInfo : ����ڰ� ����� �̹��� �迭�� ������, ������ �����ϱ� ���� ����ü.
	 * @param	pImageIds : ��ƼŬ���� ����� �̹��� �迭.
	 * @param	nImgageCnts : �̹��� �迭�� ũ��.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticleSetImagesInit( MGLparticletotalinfo * pParticleTotalInfo, MGLint * pImageIds, MGLint nImgageCnts );

	/**
	 * @brief	��ƼŬ���� ����� �̹��� ���μ��� ������ ������ ����ü ������, �̹��� ���μ��̿��� ����� �������� ������ ����ϱ� ���� API
	 * @param	pParticleTotalInfo : ����ڰ� ����� �̹��� ���μ��������� ������ ����ü ������ �������� ������ �����ϱ� ���� ����ü.
	 * @param	nIpInfoCnts : ��ƼŬ���� ����� �̹��� ���μ��������� ������ ����ü�� ����.
	 * @param	nDataCnts : �̹��� ���μ��� �������� ����.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticleSetIpInfo ( MGLparticletotalinfo * pParticleTotalInfo, MGLint nIpInfoCnts, MGLint nDataCnts );

	/**
	 * @brief	��ƼŬ���� ����� �̹��� ���μ��� ������ ������, �̹��� ���μ��� ����ü, �̹��� ���μ��� �Ķ���Ϳ�
				�̹��� ���μ��� ������ ������ ����ü�� �ε��� ������ ����ϱ� ���� API 
	 * @param	pParticleTotalInfo : ����ڰ� ����� �̹��� ���μ��� ���� ������ �����ϱ� ���� ����ü.
	 * @param	pData : �̹��� ���μ��̿��� ���� �������� �迭 ����.
	 * @param	pImageIps : �̹��� ���μ����� ����� ����ü ����.
	 * @param	nParamId : � �̹��� ���μ����� ������ �������� �����ϴ� �Ķ���� ����.
	 * @param	index : �̹��� ���μ��� ���� ������ ������ ����ü�� �ε��� ����.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticleSetIpInit ( MGLparticletotalinfo * pParticleTotalInfo, MGLint * pData, MGLvoid * pImageIps,
	                        MGLint nParamId, MGLint index );

	/**
	 * @brief	��ƼŬ���� ����� �̹��� �� �̹��� ���μ����� ��� ����� ���� ���� �� ������ �����ϴ� API.
	 *
	 * @param	pParticleTotalInfo : ����ڰ� ����� �̹��� ���μ��� Ÿ���� �����ϱ� ���� ����ü
	 * @param	imageProcType : ��ƼŬ�� �̹��� ���ð� ���μ����� Ÿ�� ����.
	 * @param	fpImageProcFuncPtr : imageProcType�� MGL_PARTICLE_SET_IMG_PROC_MANUAL�̸� ����� ���� �Լ� ������
	 *							   �� ����Ѵ�. �׿��� NULL ���� ������.
	 * @param	nImageChangeInfo : ��ƼŬ �ý��� ���������� �̹����� �̹��� ���μ��� ������ ������ ���� ���� ����
	 *							 imageProcType�� MGL_PARTICLE_SET_IMG_PROC_SPAN_SEQ �̶��, ��ƼŬ�� �����ֱ� �����̸�,
	 *							 imageProcType�� MGL_PARTICLE_SET_IMG_PROC_FRAME_SEQ �Ǵ� MGL_PARTICLE_SET_IMG_PROC_FRAME_RAND
	 *							 �̶�� ������ ������ �ȴ�.
	 * @return MGLvoid
	 */
	MGLvoid
	MGL_ParticleSetImageProcInit (MGLparticletotalinfo * pParticleTotalInfo, MGLint imageProcType, \
	                              MGLparticleimageprocfuntptr fpImageProcFuncPtr, MGLint nImageChangeInfo );

	/**
	 * @brief	����ȭ�� ���� ������ �����ϱ� ���� API.
	 *
	 * @param	pParticleTotalInfo : ��ƼŬ�� ��Ÿ�� ������ ������ �����ϱ� ���� ����ü.
	 * @param	pParticleBgAreaInfo : ����ȭ���� ��ǥ(x, y) ������ ũ��(Width, Height) ������ �����ϰ� �ִ� ����ü
	 * @return	MGLvoid
	 */

	MGLvoid
	MGL_SetParticleAreaInfo(MGLparticletotalinfo * pParticleTotalInfo, MGLparticlebgareainfo * pParticleBgAreaInfo);
	
	/**
	 * @brief	����ȭ�� ���� ������ �����ϱ� ���� API.
	 *
	 * @param	pParticleTotalInfo : ��ƼŬ�� ��Ÿ�� ������ ������ �����ϱ� ���� ����ü.
	 * @param	x : ����ȭ���� x ��ǥ ����.
	 * @param	y : ����ȭ���� y ��ǥ ����.
	 * @param	width  : ����ȭ���� Width ���� ����.
	 * @param	height : ����ȭ���� height ���� ����.
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
