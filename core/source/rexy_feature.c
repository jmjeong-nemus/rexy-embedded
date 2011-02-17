/*
*
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
*
* This file is a part of Rexy Embedded 
*
*/

/** @file */

/**
 * @defgroup SystemFeature Core System Feature Information Service
 * @ingroup RexyECoreGroup
 * @{
 */

#include "rexy.h"
#include "rexy_feature.h"

/**
 * @brief	Get a system's hardware feature
 * @param[in]	type	For detailed information, see 'FeatureType'.
 * @remark	The value of the feature is hardware dependent
 * @return	Value of the feature
 */
int FtrGetSystemFeature(FeatureType type)
{
	return RalGetSystemFeature (type);

}

/** @} */ // end feature
