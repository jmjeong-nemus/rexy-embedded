#ifndef __REXY_BMP_DATA_H__
#define __REXY_BMP_DATA_H__

/** @file */

typedef enum
{
  //### 20051213 hylee - MGL Example
  MGL_ex1_I,		//ex1 enum value
  MGL_ex2_I,		//ex2 enum value
  MGL_ex3_I,		//ex3 enum value
  MGL_ex4_I,		//ex4 enum value
  MGL_focus_I,		//focus enum value
  MGL_icon_01_01_I,		//icon_01_01 enum value
  MGL_icon_01_02_I,		//icon_01_02 enum value
  MGL_icon_01_03_I,		//icon_01_03 enum value
  MGL_icon_01_04_I,		//icon_01_04 enum value
  MGL_icon_02_01_I,		//icon_02_01 enum value
  MGL_icon_02_02_I,		//icon_02_02 enum value
  MGL_icon_02_03_I,		//icon_02_03 enum value
  MGL_icon_02_04_I,		//icon_02_04 enum value
  MGL_icon_03_01_I,		//icon_03_01 enum value
  MGL_icon_03_02_I,		//icon_03_02 enum value
  MGL_icon_03_03_I,		//icon_03_03 enum value
  MGL_icon_03_04_I,		//icon_03_04 enum value
  MGL_icon_04_01_I,		//icon_04_01 enum value
  MGL_icon_04_02_I,		//icon_04_02 enum value
  MGL_icon_04_03_I,		//icon_04_03 enum value
  MGL_icon_04_04_I,		//icon_04_04 enum value
  MGL_icon_05_01_I,		//icon_05_01 enum value
  MGL_icon_05_02_I,		//icon_05_02 enum value
  MGL_icon_05_03_I,		//icon_05_03 enum value
  MGL_icon_05_04_I,		//icon_05_04 enum value
  MGL_icon_06_01_I,		//icon_06_01 enum value
  MGL_icon_06_02_I,		//icon_06_02 enum value
  MGL_icon_06_03_I,		//icon_06_03 enum value
  MGL_icon_06_04_I,		//icon_06_04 enum value
  MGL_icon_07_01_I,		//icon_07_01 enum value
  MGL_icon_07_02_I,		//icon_07_02 enum value
  MGL_icon_07_03_I,		//icon_07_03 enum value
  MGL_icon_07_04_I,		//icon_07_04 enum value
  MGL_icon_08_01_I,		//icon_08_01 enum value
  MGL_icon_08_02_I,		//icon_08_02 enum value
  MGL_icon_08_03_I,		//icon_08_03 enum value
  MGL_icon_08_04_I,		//icon_08_04 enum value
  MGL_icon_09_01_I,		//icon_09_01 enum value
  MGL_icon_09_02_I,		//icon_09_02 enum value
  MGL_icon_09_03_I,		//icon_09_03 enum value
  MGL_icon_09_04_I,		//icon_09_04 enum value
  MGL_icon_10_01_I,		//icon_10_01 enum value
  MGL_icon_10_02_I,		//icon_10_02 enum value
  MGL_icon_10_03_I,		//icon_10_03 enum value
  MGL_icon_10_04_I,		//icon_10_04 enum value
  MGL_icon_11_01_I,		//icon_11_01 enum value
  MGL_icon_11_02_I,		//icon_11_02 enum value
  MGL_icon_11_03_I,		//icon_11_03 enum value
  MGL_icon_11_04_I,		//icon_11_04 enum value
  MGL_icon_12_01_I,		//icon_12_01 enum value
  MGL_icon_12_02_I,		//icon_12_02 enum value
  MGL_icon_12_03_I,		//icon_12_03 enum value
  MGL_icon_12_04_I,		//icon_12_04 enum value
  MGL_menus_01_I,		//menus_01 enum value
  MGL_menus_02_I,		//menus_02 enum value
  MGL_menus_03_I,		//menus_03 enum value
  MGL_menus_04_I,		//menus_04 enum value
  MGL_particle_small_I,		//particle_small enum value
  MGL_snow_I,		//snow enum value
  MGL_softkey_I,		//softkey enum value
  MGL_title_I,		//title enum value
  MGL_back_I,		//back enum value
  MGL_focusBg_I,		//focusBg enum value
  MGL_RexyE_Face_I,		//RexyE_Face enum value
  MGL_preMM_Alarm01_I,		//preMM_Alarm01 enum value	// added by namgung 11-Jan-2006

	// list control example
	IMGSUBLISTN000_N_I,
	IMGSUBLISTN001_N_I,
	IMGSUBLISTN002_N_I,
	IMGSUBLISTN003_N_I,
	IMGSUBLISTN004_N_I,
	IMGSUBLISTN005_N_I,
	IMGSUBLISTN006_N_I,
	IMGSUBLISTN007_N_I,
	IMGSUBLISTN008_N_I,
	IMGSUBLISTN009_N_I,
	IMGSUBLISTN010_N_I,
	IMGSUBLISTN011_N_I,
	IMGSUBLISTN012_N_I,
	IMGSUBLISTN013_N_I,
	IMGSUBLISTN014_N_I,
	IMGSUBLISTN015_N_I,
	IMGSUBLISTN016_N_I,
	IMGSUBLISTN017_N_I,
	IMGSUBLISTN018_N_I,
	IMGSUBLISTN019_N_I,
	IMGSUBLISTN020_N_I,
	IMGSUBLISTN000_A_I,
	IMGSUBLISTN001_A_I,
	IMGSUBLISTN002_A_I,
	IMGSUBLISTN003_A_I,
	IMGSUBLISTN004_A_I,
	IMGSUBLISTN005_A_I,
	IMGSUBLISTN006_A_I,
	IMGSUBLISTN007_A_I,
	IMGSUBLISTN008_A_I,
	IMGSUBLISTN009_A_I,
	IMGSUBLISTN010_A_I,
	IMGSUBLISTN011_A_I,
	IMGSUBLISTN012_A_I,
	IMGSUBLISTN013_A_I,
	IMGSUBLISTN014_A_I,
	IMGSUBLISTN015_A_I,
	IMGSUBLISTN016_A_I,
	IMGSUBLISTN017_A_I,
	IMGSUBLISTN018_A_I,
	IMGSUBLISTN019_A_I,
	IMGSUBLISTN020_A_I,

  MGL_mainmenu_call_n_I,
  MGL_mainmenu_call_a1_I,   //MainMenu rexy type image //added by regina 30-Aug-2006
  MGL_mainmenu_call_a2_I,
  MGL_mainmenu_call_a3_I,
  MGL_mainmenu_camera_n_I,
  MGL_mainmenu_camera_a1_I,
  MGL_mainmenu_camera_a2_I,
  MGL_mainmenu_camera_a3_I,
  MGL_mainmenu_datafolder_n_I,
  MGL_mainmenu_datafolder_a1_I,
  MGL_mainmenu_datafolder_a2_I,
  MGL_mainmenu_datafolder_a3_I,
  MGL_mainmenu_ezweb_n_I,
  MGL_mainmenu_ezweb_a1_I,
  MGL_mainmenu_ezweb_a2_I,
  MGL_mainmenu_ezweb_a3_I,
  MGL_mainmenu_mail_n_I,
  MGL_mainmenu_mail_a1_I,
  MGL_mainmenu_mail_a2_I,
  MGL_mainmenu_mail_a3_I,
  MGL_mainmenu_mynumber_n_I,
  MGL_mainmenu_mynumber_a1_I,
  MGL_mainmenu_mynumber_a2_I,
  MGL_mainmenu_mynumber_a3_I,
  MGL_mainmenu_setting_n_I,
  MGL_mainmenu_setting_a1_I,
  MGL_mainmenu_setting_a2_I,
  MGL_mainmenu_setting_a3_I,
  MGL_mainmenu_tools_n_I,
  MGL_mainmenu_tools_a1_I,
  MGL_mainmenu_tools_a2_I,
  MGL_mainmenu_tools_a3_I,



  BmpIDMaxNum

} BmpIDEnum;

extern const BmpType AppBmpData[BmpIDMaxNum];

/** @} */


#endif//__REXY_BMP_DATA_H__
