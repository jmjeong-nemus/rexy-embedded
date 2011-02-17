/*
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of RexyE
 *
 */

#ifndef __rx_feature_h__
#define __rx_feature_h__

/** @file */

// this file contains information about which features will be applied.

typedef enum {
	FEATURE_LCD_WIDTH,
	FEATURE_LCD_HEIGHT,
	FEATURE_LCD_BPP,
	FEATURE_UNICODE,
	FEATURE_BIGENDIAN
} FeatureType;

int FtrGetSystemFeature(FeatureType type);

/** @} */

#endif//__rx_feature_h__
