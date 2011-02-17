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

#define MGL_PA_PILE_LIFE_DEAD		0	/**< ��ƼŬ�� �Ҹ� �Ǿ������� üũ�ϱ� ���� ��ũ��. */

#define MGL_PA_PILE_LIFE_ALIVE		1	/**< ��ƼŬ�� ��� �ִ��� üũ�ϱ� ���� ��ũ��. */
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

#define MGL_PA_PILE_NON_VISIBLE		0	/**< ��ƼŬ�� ��׶��� �Ǵ� ��Ŀ�� �����ܿ� �׿� ���� �ʴ��� ���θ� üũ�ϱ� ���� ��. */

#define MGL_PA_PILE_TRUE			1	/**< ��ƼŬ�� ��׶��忡 �׿� �ִ��� ���θ� üũ�ϱ� ���� ��. */

#define MGL_PA_PILE_FI_ABSCISSA     0	/**< ��ƼŬ�� ���η� ���� ������ �����ϱ� ���� ��. */

#define MGL_PA_PILE_FI_ORDINATE     1	/**< ��ƼŬ�� ���η� ���ϰ����� �����ϱ� ���� ��. */
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

#define MGL_PA_PILE_SET_IMG_PROC_DEFAULT		0	/**< ��ƼŬ�� �ϳ��� �̹��� ID��, �̹��� IP���� ����ڷ� ���� �Է� �޴´�. */

#define MGL_PA_PILE_SET_IMG_PROC_MANUAL			1	/**< ��ƼŬ�� �̹��� ID�� , �̹��� IP�� ����ڷ� ���� ���� �޴´�. */

#define MGL_PA_PILE_SET_IMG_PROC_KEEP_UP		2	/**< ��ƼŬ�� �̹��� ID�°� , �̹��� IP���� ����ڷ� ���� �̸� �Է� �޴´�.
�̹�����, IP�� 1:1 �̸�, �Ҹ� �ɶ����� �Ӽ��� �״�� �����Ѵ�. */

#define MGL_PA_PILE_SET_IMG_PROC_SPAN_SEQ			3	/**< ��ƼŬ�� �������� �̹��� ID�°�, �̹��� IP���� ����ڷ� ���� �̸� �Է� �޾�,
	������ġ�� ���������� �����Ͽ�, SPAN ������ �ݺ��Ѵ�. */

#define MGL_PA_PILE_SET_IMG_PROC_FRAME_SEQ			4	/**< ��ƼŬ�� �������� �̹��� ID�°�, �̹��� IP���� ����ڷ� ���� �̸� �Է� �޾�
	������ġ�� ���������� �����Ͽ�, �ݺ��Ѵ�. */

#define MGL_PA_PILE_SET_IMG_PROC_FRAME_RAND		5	/**< ��ƼŬ�� �������� �̹��� ID�°�, �̹��� IP���� ����ڷ� ���� �̸� �Է� �޾�
	������ġ�� �����ϰ� �����Ͽ�, �ݺ��Ѵ�. */
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
	 * @brief	��ƼŬ�� ���� ��Ŀ�� ������ ������ ����ü
	 */

	typedef struct MGLpapilefiareainfo
	{
		MGLint x;			/**< Focus Icon�� x��ǥ  */
		MGLint y;			/**< Focus Icon�� y��ǥ  */
		MGLint width;		/**< Focus Icon�� width ���� */
		MGLint height;		/**< Focus Icon�� Height ���� */
	}

	MGLpapilefiareainfo;

	/**
	 * @brief	��ƼŬ�� ���� ��׶�� ������ ����ü
	 */

	typedef struct MGLpapilebgareainfo
	{
		MGLint x;			/**< ��׶����� x��ǥ */
		MGLint y;			/**< ��׶����� y��ǥ */
		MGLint width;		/**< ��׶����� Width ���� */
		MGLint height;		/**< ��׶����� Height ���� */
	}

	MGLpapilebgareainfo;


	/**
	 * @brief	��ƼŬ �ý����� Force(��) ����ü
	 */

	typedef struct MGLpapileforceinfo
	{
		MGLint fx;				/**< x(����)���� �ۿ��ϴ� �� */
		MGLint fy;				/**< y(����)���� �ۿ��ϴ� �� */
	}

	MGLpapileforceinfo;

	/**
	 * @brief	��ƼŬ �ý����� Factor(�ܺ� ����) ����ü.
	 */

	typedef struct MGLpapilefactorattrib
	{
		MGLfloat afc;				/**< �����̴� ��ü�� �ݴ� �������� �ۿ��ϴ� ���� ���� */
		MGLfloat dt;				/**< �ð��� ��ġ ��ȭ�� */
		MGLpapileforceinfo gForce;	/**< x,y(����)���� �ۿ��ϴ� �߷� ����ü */
	}

	MGLpapilefactorattrib;

	/**
	 * @brief	�����Ǵ� ��ƼŬ�� ������ ���� �ϴ� ����ü
	 */

	typedef struct MGLpapilestdattrib
	{
		MGLint xMin;				/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� x�� �ּ� �� ���� */
		MGLint xMax;				/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� x�� �ִ� �� ���� */
		MGLint yMin;				/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� y �� ���� */
		MGLint yMax;				/**< ��ƼŬ�� ���� �ɶ�, ������ ��ġ�� y �� ���� */
		MGLint vxMin;				/**< ��ƼŬ�� ���� �ɶ�, x(����)������ �ּ� �ӵ� ���� */
		MGLint vxMax;				/**< ��ƼŬ�� ���� �ɶ�, x(����)������ �ִ� �ӵ� ���� */
		MGLint vyMin;				/**< ��ƼŬ�� ���� �ɶ�, y(����)������ �ּ� �ӵ� ���� */
		MGLint vyMax;				/**< ��ƼŬ�� ���� �ɶ�, y(����)������ �ִ� �ӵ� ���� */
		MGLint msMin;				/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ּ� ���� �� ���� */
		MGLint msMax;				/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ִ� ���� �� ���� */
		MGLint spMin;				/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ּ� ���� �ֱ� ���� */
		MGLint spMax;				/**< ��ƼŬ�� ���� �ɶ�, ��ƼŬ�� �ִ� ���� �ֱ� ���� */
	}

	MGLpapilestdattrib;

	/**
	 * @brief	�����Ǿ� Ȱ������ ��ƼŬ�� ���� �����ϴ� ����ü
	 */

	typedef struct MGLpapilestdinfo
	{
		MGLboolean bVisible;	/**< ��ƼŬ�� ȭ�鿡 ������ ���ΰ�? �Ⱥ��� �� ���ΰ�? */
		MGLint x;				/**< ��ƼŬ�� x ��ǥ�� */
		MGLint y;				/**< ��ƼŬ�� y ��ǥ�� */
		MGLfixed vx;			/**< ��ƼŬ�� x(����)������ ���ӵ� ��. */
		MGLfixed vy;			/**< ��ƼŬ�� y(����)������ ���ӵ� ��. */
		MGLfixed ax;			/**< ��ƼŬ�� x(����)������ �ӵ� ��.*/
		MGLfixed ay;			/**< ��ƼŬ�� y(����)������ �ӵ� ��.*/
		MGLint mass;			/**< ��ƼŬ�� ���� */
		MGLint span;			/**< ��ƼŬ�� ���� ����*/
		MGLint imageIndex;		/**< ��ƼŬ�� �̹��� �ε��� ����. ( ��ƼŬ�� ���° �ε����� �̹����� ������ �ִ���?) */
		MGLint frameInfo;		/**< ��ƼŬ�� ���� ������ ����.( �̹����� �ٲ� �� ��� ȭ�鿡 ���Դ���?) */
	}

	MGLpapilestdinfo;


	/**
	 * @brief	��ƼŬ�� ��Ÿ���� ���� ����� �̹��� / ó�� ����.
	 */

	typedef struct MGLpapileipinfo
	{
		MGLint nDataIndex;			/**< �̹��� ���μ��� ������ �ε��� ���� */
		MGLint nParamId;			/**< �̹��� ���μ��� �Ķ���� ���� */
		MGLint * pData;				/**< �̹��� ���μ��� ����Ÿ�� �迭 ���� */
		MGLvoid * pImageIps;		/**< �̹��� ���μ��� ����ü ���� */
	}

	MGLpapileipinfo;

	/**
	 * @brief	��ƼŬ�� �̹����� ó���� ����� �Լ� ������.
	 */

	typedef MGLvoid (*MGLpapileimageprocfuntptr) (MGLpapilestdinfo * pPapilestdinfo);

	/**
	 * @brief	��ƼŬ�� �̹��� ����/ó�� �� �����ϰ� �ִ� ��ü ����ü.
	 */

	typedef struct MGLpapileimageinfo
	{
		MGLint imageProcType;							/**< �̹��� Ÿ�� */
		MGLint nImageCnts;								/**< �Էµ� �̹����� ���� ���� */
		MGLint nImageChangeInfo;						/**< �̹����� ��������� �ٲ�°��� ���� ���� */
		MGLint * pImageIds;								/**< �Էµ� �̹��� ���� �����ϴ� �̹��� ���ҽ� ID �迭 ����. */
		MGLint nIpInfoCnts;								/**< MGLparticleipinfo ����ü�� ���� ���� */
		MGLint nDataCnts;								/**< �̹��� ���μ��� ����Ÿ�� �迭 ���� ���� */
		MGLpapileipinfo * pImageIpInfo;					/**< �̹��� ó�� ������ ó�� ������ �����ϴ� ����ü */
		MGLpapileimageprocfuntptr	fpImageProcFuncPtr; /**< ����� �̹��� ��� �Լ� ������ */
	}

	MGLpapileimageinfo;

	/**
	 * @brief	��Ŀ�� �����ܿ� ���� ������ ��ǥ��, �̹����� ������ �����ϴ� ����ü
	 */

	typedef struct MGLpapilefirenderinfo
	{
		MGLint nX;											/**< ��ƼŬ�� ���� x ��ǥ ���� */
		MGLint nY;											/**< ��ƼŬ�� ���� y ��ǥ ���� */
		MGLint pileImage;									/**< ��ƼŬ�� ���� �̹��� ���� */
	}

	MGLpapilefirenderinfo;

	/**
	 * @brief	����ȭ�鿡 ���� ������ ��ǥ��, �̹����� ������ �����ϴ� ����ü 
	 */

	typedef struct MGLpapilebgrenderinfo
	{

		MGLint nX;										/**< ��ƼŬ�� ���� x ��ǥ ���� */
		MGLint nY;										/**< ��ƼŬ�� ���� y ��ǥ ���� */
		MGLint pileImage;								/**< ��ƼŬ�� ���� �̹��� ���� */
	}

	MGLpapilebgrenderinfo;

	/**
	 * @brief	��ƼŬ �ý����� �����ϴ� �ٽ����� ����ü��, ��ƼŬ�� ǥ���ϴµ� �ʿ��� ��� ������ ��� �Ǿ� �ִ�.
	 */

	typedef struct MGLpapileunityinfo
	{
		MGLint nIndexCnt;									/**< ȭ�鿡 ��µ� ��ƼŬ �迭�� �ε��� ���� */
		MGLint nMaxPapileCnts;								/**< ��ƼŬ�� �ִ� ���� ���� */
		MGLint nBgCoordiSize;								/**< pPileBgCoordiInfo �迭�� ũ�� ���� */
		MGLint nFiCoordiSize;								/**< pPileFiCoordiInfo �迭�� ũ�� ���� */
		MGLint nBgRenderSize;								/**< pPapileFiRenderInfo �迭�� ũ�� ���� */
		MGLint nFiRenderSize;								/**< pPapileBgRenderInfo �迭�� ũ�� ���� */
		MGLint nBgRenderCnts;								/**< ��Ŀ�� �����ܿ� �ѹ��� ���� ��ƼŬ ���� ���� */
		MGLint nFiRenderCnts;								/**< ����ȭ�鿡 �ѹ��� ���� ��ƼŬ ���� ���� */	
		MGLbyte * pPileBgCoordiInfo;						/**< ��ƼŬ�� ���̴� ����ȭ�� ��ġ ������ ������ ȭ�� ��ü ũ���� �迭 */
		MGLbyte * pPileFiCoordiInfo;						/**< ��ƼŬ�� ���̴� ��Ŀ�� ������ ��ġ ������  ������ ȭ�� ��ü ũ���� �迭 */
		MGLpapilebgrenderinfo * pPapileBgRenderInfo;		/**< ��ƼŬ�� ����ȭ�鿡 ���� ������ ��ǥ��, �̹����� ������ �����ϴ� ����ü*/
		MGLpapilefirenderinfo * pPapileFiRenderInfo;		/**< ��ƼŬ�� ��Ŀ�� �����ܿ� ���� ������ ��ǥ��, �̹����� ������ �����ϴ� ����ü*/
		MGLpapilefiareainfo * pPapileFiAreaInfo;		/**< ��ƼŬ�� FI������ ���� �ϸ�, ���ؾ� �ϴµ�, �� ������ �ִ� ����ü*/
		MGLpapilebgareainfo * pPapileBgAreaInfo;		/**< ��ƼŬ�� BG������ ���� �ϸ�, ���ؾ� �ϴµ�, �� ������ �ִ� ����ü*/
		MGLpapileforceinfo papileForceInfo;				/**< ��ƼŬ�� �� ����ü */
		MGLpapilefactorattrib * pPapileFactorAttrib;	/**< ��ƼŬ�� Factor(�ܺ� ����) ����ü */
		MGLpapilestdattrib * pPapileStdAttrib;			/**< ��ƼŬ ������ �ʿ��� �⺻ ���� ����ü */
		MGLpapilestdinfo * pPapileStdInfo;				/**< �����Ǿ��� ��ƼŬ�� �⺻ ���� ����ü */
		MGLpapileimageinfo * pPapileImageInfo;			/**< ��ƼŬ�� ���Ǿ��� �̹��� ID�� �̹��� ���μ��� ���� ����ü */
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
	 * @brief	��ƼŬ�� ����ϰڴٰ�, ����ý��ۿ� �˷��ִ� �߿��� API
	 * @param	pPapileUnityInfo :��ƼŬ�� ��� ������ �����ϴ� ����ü.
	 * @param	pPapileStdInfo :��ƼŬ�� �⺻ ������ �����ϴ� ����ü �̸�, ���ø����̼� �����ڴ�
	 *							����� ��ƼŬ ���� ��ŭ�� ����ü �迭�� �Ҵ��Ͽ�, ���� �Ѵ�.
	 * @param	pPapileStdAttrib :��ƼŬ�� �⺻ ������ ����ϴµ� �ʿ��� �⺻ Properties �����̸�, 
	 *							  pFactorPpt ����ü ������ �Բ� ���Ǿ�, pParticleBasicInfo ����ü�� ��� ���� ���� �Ѵ�.
	 * @param	pPapileFactorAttrib :��ƼŬ �⺻ ������ ����ϴµ� �ʿ��� Factor Properties �����̸�,
	 *								 pParticlePpt ����ü ������ �Բ� ���Ǿ�, pParticleBasicInfo ����ü�� ��� ���� ���� �Ѵ�.
	 * @param	nMaxPapileCnts : ����ϰ��� �ϴ� ��ƼŬ �ִ� ����.	
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_SetParticlePileSystem(MGLpapileunityinfo * pPapileUnityInfo, \
	                          MGLpapilestdinfo * pPapileStdInfo, \
	                          MGLpapilestdattrib * pPapileStdAttrib, \
	                          MGLpapilefactorattrib * pPapileFactorAttrib, \
	                          MGLint nMaxPapileCnts);

	/**
	 * @brief	��ƼŬ�� ��ƼŬ �ִ� ������ŭ �����Ѵ�. ó���� ��� ��ƼŬ��  �����ϸ�, ���Ŀ��� �Ҹ�� ��ƼŬ�� 
	 *          �ٽ� ���� �Ѵ�.
	 * @param	pPapileUnityInfo : SetParticleSystem() ȣ�� �� pPapileUnityInfo ����ü�� ��ƼŬ ������ ���� ������
	 *                             ü������, �̸� ������� ������ ��ƼŬ�� ���� �ȴ�.
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_CreateParticlePile(MGLpapileunityinfo *  pPapileUnityInfo);

	/**
	 * @brief	��ƼŬ�� ������ �ִ� ������ �ٽ� �����Ѵ�. ��, �ʱ⿡ ������ ��ƼŬ�� ������ �Ѿ�� �ȵǸ�,
	 *			���� �ʱ��� �ִ� ��ƼŬ ������ �Ѿ� ���ٸ�, ��ƼŬ�� ���� �ʱ��� ��ƼŬ�� ���� ���� �Ѵ�.
	 * @param	pPapileUnityInfo : ��ƼŬ�� �ִ� ������ �����ϱ� ���� ����ü.
	 * @param	maxParticleCnts : ��ƼŬ�� �ִ� ����.
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_ParticlePileSetMaxCnt(MGLpapileunityinfo *pPapileUnityInfo, MGLint maxParticleCnts);

	/**
	 * @brief	���ø����̼ǿ���, ����� Animator�� ����ý����� ����ϴ� API.
	 *			�ش� API�� ���� ���ø����̼ǿ��� ����� Animator ������, Animator���� ����� ����ý��۰���
	 *			�������� ������ ���� API.
	 * @param	pPapileUnityInfo : ��ƼŬ�� ��� ������ ������ �����ϴ� ����ü.
	 * @param	pSubsystemInfo : ���ø����̼ǿ���, ����� Animator�� ����ý��� ����ü.
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_ParticlePileSetAnimator(MGLpapileunityinfo *  pPapileUnityInfo, MGLsubsystem *pSubsystemInfo);

	/**
	 * @brief	���ø����̼ǿ��� ����� �̹��� Ÿ�԰�, �̹��� ó�� ���� ����ü�� ����ϴ� API.
	 * @param	pPapileUnityInfo : ����ڰ� ����� �̹��� ó�� ���� ����ü�� ������ �����ϱ� ���� ����ü. 
	 * @param	pPapileImageInfo : ��ƼŬ���� ����� �̹����� ���� ����ü.
	 * @param	pPapileIpInfo : ��ƼŬ���� ����� �̹��� ���μ��� ���� ����ü.
	 * @return	MGLvoid 
	 */

	MGLvoid
	MGL_ParticlePileSetImageProcSystem ( MGLpapileunityinfo *  pPapileUnityInfo, MGLpapileimageinfo * pPapileImageInfo, MGLpapileipinfo * pPapileIpInfo );

	/**
	 * @brief	���ø����̼ǿ��� ����� �̹����� �迭�� �迭 ũ�⸦ ����ϴ� API
	 * @param	pPapileUnityInfo : ����ڰ� ����� �̹��� �迭�� ������, ������ �����ϱ� ���� ����ü.
	 * @param	pImageIds : ��ƼŬ���� ����� �̹��� �迭.
	 * @param	nImgageCnts : �̹��� �迭�� ũ��.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticlePileSetImagesInit( MGLpapileunityinfo * pPapileUnityInfo, MGLint * pImageIds, MGLint nImgageCnts );

	/**
	 * @brief	��ƼŬ���� ����� �̹��� ���μ��� ������ ������ ����ü ������, �̹��� ���μ��̿��� ����� �������� ������ ����ϱ� ���� API
	 * @param	pPapileUnityInfo : ����ڰ� ����� �̹��� ���μ��������� ������ ����ü ������ �������� ������ �����ϱ� ���� ����ü.
	 * @param	nIpInfoCnts : ��ƼŬ���� ����� �̹��� ���μ��������� ������ ����ü�� ����.
	 * @param	nDataCnts : �̹��� ���μ��� �������� ����.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticlePileSetIpInfo ( MGLpapileunityinfo * pPapileUnityInfo, MGLint nIpInfoCnts, MGLint nDataCnts );

	/**
	 * @brief	��ƼŬ���� ����� �̹��� ���μ��� ������ ������, �̹��� ���μ��� ����ü, �̹��� ���μ��� �Ķ���Ϳ�
				�̹��� ���μ��� ������ ������ ����ü�� �ε��� ������ ����ϱ� ���� API 
	 * @param	pPapileUnityInfo : ����ڰ� ����� �̹��� ���μ��� ���� ������ �����ϱ� ���� ����ü.
	 * @param	pData : �̹��� ���μ��̿��� ���� �������� �迭 ����.
	 * @param	pImageIps : �̹��� ���μ����� ����� ����ü ����.
	 * @param	nParamId : � �̹��� ���μ����� ������ �������� �����ϴ� �Ķ���� ����.
	 * @param	index : �̹��� ���μ��� ���� ������ ������ ����ü�� �ε��� ����.
	 * @return	MGLvoid
	 */
	MGLvoid
	MGL_ParticlePileSetIpInit ( MGLpapileunityinfo * pPapileUnityInfo, MGLint * pData, MGLvoid * pImageIps,
	                            MGLint nParamId, MGLint index );


	/**
	 * @brief	��ƼŬ���� ����� �̹��� �� �̹��� ���μ����� ��� ����� ���� ���� �� ������ �����ϴ� API.
	 *
	 * @param	pPapileUnityInfo : ����ڰ� ����� �̹��� ���μ��� Ÿ���� �����ϱ� ���� ����ü
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
	MGL_ParticlePileSetImageProcInit (MGLpapileunityinfo * pPapileUnityInfo, MGLint imageProcType, \
	                                  MGLpapileimageprocfuntptr fpImageProcFuncPtr, MGLint nImageChangeInfo );

	/**
	 * @brief	��ƼŬ�� ���̴� ��ǥ ������ �����ϱ� ���� API. 
	 *
	 * @param	pPapileUnityInfo : ��ƼŬ�� ���̴� ��ǥ ���� �� �̹��� ������ �����ϱ� ���� ����ü.
	 * @param	pPapileBgRenderInfo : ����ȭ�鿡 ��ƼŬ�� �׿����� ��ġ��, ��ƼŬ�� �׿��� �Ѵٸ�,
	 * 						          ��� �̹����� �׷��� �������� ���� ������ �����ϱ� ���� �迭.
	 * @param	nBgRenderSize	: pPapileBgRenderInfo �迭�� ũ�� ����.
	 * @param	pPapileFiRenderInfo : ��Ŀ�� �����ܿ� ��ƼŬ�� �׿����� ��ġ��, ��ƼŬ�� �׿��� �Ѵٸ�,
	 *								  ��� �̹����� �׷��� �������� ���� ������ �����ϱ� ���� �迭.
	 * @param	nFiRenderSize	: pPapileFiRenderInfo �迭�� ũ�� ����.
	 *
	 * @return MGLvoid
	 */

	MGLvoid
	MGL_SetParticlePileRenderInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLpapilebgrenderinfo * pPapileBgRenderInfo, MGLint nBgRenderSize, \
								 MGLpapilefirenderinfo * pPapileFiRenderInfo, MGLint nFiRenderSize);

	/**
	 * @brief	��ƼŬ�� ���� ��ü ��ġ ������ �����ϱ� ���� API. 
	 *
	 * @param	pPapileUnityInfo : ��ƼŬ�� �׿��ִ� ��ü ��ǥ ������ �����ϱ� ���� ����ü.
	 * @param	pPapileBgCoordiInfo : ����ȭ���� ��ƼŬ�� �׿��ִ� ��ü ��ġ ������  �����ϱ� ���� �迭.
	 * @param	nBgCoordiSize	: pPapileBgCoordiInfo �迭�� ũ�� ����.
	 * @param	pPapileFiCoordiInfo : ��Ŀ�� �����ܿ� ��ƼŬ�� �׿��ִ� ��ü ��ġ ������ �����ϱ� ���� �迭.
	 * @param	nFiCoordiSize	: pPapileFiRenderInfo �迭�� ũ�� ����.
	 *
	 * @return MGLvoid
	 */
	MGLvoid
	MGL_SetParticlePileCoordiInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLbyte * pPapileBgCoordiInfo, MGLint nBgCoordiSize,\
	                              MGLbyte * pPapileFiCoordiInfo, MGLint nFiCoordiSize);

	/**
	 * @brief	����ȭ�� / ��Ŀ�� ������ ���� ������ �����ϱ� ���� API.
	 *
	 * @param	pPapileUnityInfo : ��Ŀ�� �� ������ �����ϱ� ���� ����ü
	 * @param	pPapileBgAreaInfo : ����ȭ���� Width, Height ������ �����ϰ� �ִ� ����ü
	 * @param	pPapileFiAreaInfo : ��Ŀ�� �������� ��ǥ ������ �����ϰ� �ִ� ����ü.
	 * @return	MGLvoid
	 */

	MGLvoid
	MGL_SetParticlePileAreaInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLpapilebgareainfo * pPapileBgAreaInfo, MGLpapilefiareainfo * pPapileFiAreaInfo);

	/**
	* @brief	��ƼŬ�� ���� ����ȭ�� ������ ��ǥ ������ �����Ѵ�. 
	*
	* @param	pPapileUnityInfo : ����ȭ�� ���� ������ �����ϱ� ���� ����ü
	* @param	x : ����ȭ���� x���� ��ǥ.
	* @param	y : ����ȭ���� y���� ��ǥ.
	* @param	width : ����ȭ���� width ����.
	* @param	height : ����ȭ���� height ����.
	* @return	MGLvoid
	*/

	MGLvoid
	MGL_SetParticlePileBgInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLint x, MGLint y, MGLint width, MGLint height );

	/**
	* @brief	��ƼŬ�� ���� ��Ŀ�� ������ ��ǥ ������ �����Ѵ�. 
	*
	* @param	pPapileUnityInfo : ����ȭ�� ���� ������ �����ϱ� ���� ����ü
	* @param	x : ��Ŀ�� �������� x���� ��ǥ.
	* @param	y : ��Ŀ�� �������� y���� ��ǥ.
	* @param	width : ��Ŀ�� �������� width ����.
	* @param	height : ��Ŀ�� �������� height ����.
	* @return	MGLvoid
	*/
	MGLvoid
	MGL_SetParticlePileFiInfo(MGLpapileunityinfo * pPapileUnityInfo, MGLint x, MGLint y, MGLint width, MGLint height );

	/**
	* @brief	����ȭ�鿡 ��ƼŬ�� ���̴� ����� �׸��� ���� ����� API.
	*			Custom Layer���� ���� ȣ�� �ȴ�.
	*
	* @param	pBuffer : Custom Layer�� ���� ����.
	* @param	pParam : ����ȭ�鿡 �׸� ��ƼŬ ���̴� ����� ��ǥ ������ �����ϴ� ����ü. 
	* @return	MGLvoid
	*/

	MGLvoid
	MGL_DrawParticlePileBgArea(MGLvoid * pBuffer, MGLvoid * pParam );

	/**
	* @brief	��Ŀ�� �����ܿ� ���̴� ����� �׸��� ���� ����� API.
	*			Custom Layer���� ���� ȣ�� �ȴ�.
	*
	* @param	pBuffer : Custom Layer�� ���� ����.
	* @param	pParam : ��Ŀ�� ���������� ��ƼŬ ���̴� ����� ��ǥ ������ �����ϴ� ����ü. 
	* @return	MGLvoid
	*/

	MGLvoid
	MGL_DrawParticlePileFiArea(MGLvoid * pBuffer, MGLvoid * pParam );

	/**
	* @brief	��ƼŬ�� ��Ŀ�� ���������� �׿�����, �����ϴ� ��� ��ǥ ������ ���� �Ѵ�.
	*
	* @param	pPapileUnityInfo : ��ƼŬ�� ��Ŀ�� ���������� �׿�����, ��� ��ǥ ������ �����ϴ� ����ü.
	* @return	MGLvoid
	*/
	MGLvoid
	MGL_ClearParticlePileFiArea(MGLpapileunityinfo * pPapileUnityInfo);//, MGLint width, MGLint height);

	/**
	* @brief	��ƼŬ�� ����ȭ�鿡 �׿�����, �����ϴ� ��� ��ǥ ������ ���� �Ѵ�.
	*
	* @param	pPapileUnityInfo : ��ƼŬ�� ����ȭ�鿡 �׿�����, ��� ��ǥ ������ �����ϴ� ����ü.
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
