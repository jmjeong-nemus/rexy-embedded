#include "rexy.h"
#include "app_bmpdata.h"

//### MGL support
extern const unsigned short ex1[];
extern const unsigned short ex2[];
extern const unsigned short ex3[];
extern const unsigned short ex4[];
extern const unsigned short focus[];
extern const unsigned short icon_01_01[];
extern const unsigned short icon_01_02[];
extern const unsigned short icon_01_03[];
extern const unsigned short icon_01_04[];
extern const unsigned short icon_02_01[];
extern const unsigned short icon_02_02[];
extern const unsigned short icon_02_03[];
extern const unsigned short icon_02_04[];
extern const unsigned short icon_03_01[];
extern const unsigned short icon_03_02[];
extern const unsigned short icon_03_03[];
extern const unsigned short icon_03_04[];
extern const unsigned short icon_04_01[];
extern const unsigned short icon_04_02[];
extern const unsigned short icon_04_03[];
extern const unsigned short icon_04_04[];
extern const unsigned short icon_05_01[];
extern const unsigned short icon_05_02[];
extern const unsigned short icon_05_03[];
extern const unsigned short icon_05_04[];
extern const unsigned short icon_06_01[];
extern const unsigned short icon_06_02[];
extern const unsigned short icon_06_03[];
extern const unsigned short icon_06_04[];
extern const unsigned short icon_07_01[];
extern const unsigned short icon_07_02[];
extern const unsigned short icon_07_03[];
extern const unsigned short icon_07_04[];
extern const unsigned short icon_08_01[];
extern const unsigned short icon_08_02[];
extern const unsigned short icon_08_03[];
extern const unsigned short icon_08_04[];
extern const unsigned short icon_09_01[];
extern const unsigned short icon_09_02[];
extern const unsigned short icon_09_03[];
extern const unsigned short icon_09_04[];
extern const unsigned short icon_10_01[];
extern const unsigned short icon_10_02[];
extern const unsigned short icon_10_03[];
extern const unsigned short icon_10_04[];
extern const unsigned short icon_11_01[];
extern const unsigned short icon_11_02[];
extern const unsigned short icon_11_03[];
extern const unsigned short icon_11_04[];
extern const unsigned short icon_12_01[];
extern const unsigned short icon_12_02[];
extern const unsigned short icon_12_03[];
extern const unsigned short icon_12_04[];
extern const unsigned short menus_01[];
extern const unsigned short menus_02[];
extern const unsigned short menus_03[];
extern const unsigned short menus_04[];
extern const unsigned short particle_small[];
extern const unsigned short snow[];
extern const unsigned short softkey[];
extern const unsigned short title[];
extern const unsigned short back[];
extern const unsigned short focusBg[];
extern const unsigned short RexyE_Face[];

extern const unsigned short preMM_Alarm01[];	// added by namgung 11-Jan-2006

// list control example
extern const unsigned short imgsublistno00_a[];
extern const unsigned short imgsublistno00_n[];
extern const unsigned short imgsublistno01_a[];
extern const unsigned short imgsublistno01_n[];
extern const unsigned short imgsublistno02_a[];
extern const unsigned short imgsublistno02_n[];
extern const unsigned short imgsublistno03_a[];
extern const unsigned short imgsublistno03_n[];
extern const unsigned short imgsublistno04_a[];
extern const unsigned short imgsublistno04_n[];
extern const unsigned short imgsublistno05_a[];
extern const unsigned short imgsublistno05_n[];
extern const unsigned short imgsublistno06_a[];
extern const unsigned short imgsublistno06_n[];
extern const unsigned short imgsublistno07_a[];
extern const unsigned short imgsublistno07_n[];
extern const unsigned short imgsublistno08_a[];
extern const unsigned short imgsublistno08_n[];
extern const unsigned short imgsublistno09_a[];
extern const unsigned short imgsublistno09_n[];
extern const unsigned short imgsublistno10_a[];
extern const unsigned short imgsublistno10_n[];
extern const unsigned short imgsublistno11_a[];
extern const unsigned short imgsublistno11_n[];
extern const unsigned short imgsublistno12_a[];
extern const unsigned short imgsublistno12_n[];
extern const unsigned short imgsublistno13_a[];
extern const unsigned short imgsublistno13_n[];
extern const unsigned short imgsublistno14_a[];
extern const unsigned short imgsublistno14_n[];
extern const unsigned short imgsublistno15_a[];
extern const unsigned short imgsublistno15_n[];
extern const unsigned short imgsublistno16_a[];
extern const unsigned short imgsublistno16_n[];
extern const unsigned short imgsublistno17_a[];
extern const unsigned short imgsublistno17_n[];
extern const unsigned short imgsublistno18_a[];
extern const unsigned short imgsublistno18_n[];
extern const unsigned short imgsublistno19_a[];
extern const unsigned short imgsublistno19_n[];
extern const unsigned short imgsublistno20_a[];
extern const unsigned short imgsublistno20_n[];

//mainmenu
extern const unsigned short imgsmatmenu_call_a1_eng[];
extern const unsigned short imgsmatmenu_call_a2_eng[];
extern const unsigned short imgsmatmenu_call_a3_eng[];
extern const unsigned short imgsmatmenu_call_n_eng[];
extern const unsigned short imgsmatmenu_camera_a1_eng[];
extern const unsigned short imgsmatmenu_camera_a2_eng[];
extern const unsigned short imgsmatmenu_camera_a3_eng[];
extern const unsigned short imgsmatmenu_camera_n_eng[];
extern const unsigned short imgsmatmenu_datafolder_a1_eng[];
extern const unsigned short imgsmatmenu_datafolder_a2_eng[];
extern const unsigned short imgsmatmenu_datafolder_a3_eng[];
extern const unsigned short imgsmatmenu_datafolder_n_eng[];
extern const unsigned short imgsmatmenu_ezweb_a1_eng[];
extern const unsigned short imgsmatmenu_ezweb_a2_eng[];
extern const unsigned short imgsmatmenu_ezweb_a3_eng[];
extern const unsigned short imgsmatmenu_ezweb_n_eng[];
extern const unsigned short imgsmatmenu_mail_a1_eng[];
extern const unsigned short imgsmatmenu_mail_a2_eng[];
extern const unsigned short imgsmatmenu_mail_a3_eng[];
extern const unsigned short imgsmatmenu_mail_n_eng[];
extern const unsigned short imgsmatmenu_mynumber_a1_eng[];
extern const unsigned short imgsmatmenu_mynumber_a2_eng[];
extern const unsigned short imgsmatmenu_mynumber_a3_eng[];
extern const unsigned short imgsmatmenu_mynumber_n_eng[];
extern const unsigned short imgsmatmenu_setting_a1_eng[];
extern const unsigned short imgsmatmenu_setting_a2_eng[];
extern const unsigned short imgsmatmenu_setting_a3_eng[];
extern const unsigned short imgsmatmenu_setting_n_eng[];
extern const unsigned short imgsmatmenu_tools_a1_eng[];
extern const unsigned short imgsmatmenu_tools_a2_eng[];
extern const unsigned short imgsmatmenu_tools_a3_eng[];
extern const unsigned short imgsmatmenu_tools_n_eng[];



const BmpType AppBmpData[BmpIDMaxNum] = 
  {
    //### MGL Sample
    { REXY_BMP_NOCOMP, 120, 110, (void*)ex1},		//ex1_I
    { REXY_BMP_NOCOMP, 120, 110, (void*)ex2},		//ex2_I
    { REXY_BMP_NOCOMP, 120, 110, (void*)ex3},		//ex3_I
    { REXY_BMP_NOCOMP, 120, 110, (void*)ex4},		//ex4_I
    { REXY_BMP_NOCOMP, 82, 82, (void*)focus},		//focus_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_01_01},		//icon_01_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_01_02},		//icon_01_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_01_03},		//icon_01_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_01_04},		//icon_01_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_02_01},		//icon_02_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_02_02},		//icon_02_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_02_03},		//icon_02_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_02_04},		//icon_02_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_03_01},		//icon_03_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_03_02},		//icon_03_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_03_03},		//icon_03_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_03_04},		//icon_03_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_04_01},		//icon_04_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_04_02},		//icon_04_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_04_03},		//icon_04_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_04_04},		//icon_04_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_05_01},		//icon_05_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_05_02},		//icon_05_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_05_03},		//icon_05_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_05_04},		//icon_05_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_06_01},		//icon_06_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_06_02},		//icon_06_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_06_03},		//icon_06_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_06_04},		//icon_06_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_07_01},		//icon_07_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_07_02},		//icon_07_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_07_03},		//icon_07_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_07_04},		//icon_07_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_08_01},		//icon_08_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_08_02},		//icon_08_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_08_03},		//icon_08_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_08_04},		//icon_08_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_09_01},		//icon_09_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_09_02},		//icon_09_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_09_03},		//icon_09_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_09_04},		//icon_09_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_10_01},		//icon_10_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_10_02},		//icon_10_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_10_03},		//icon_10_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_10_04},		//icon_10_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_11_01},		//icon_11_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_11_02},		//icon_11_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_11_03},		//icon_11_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_11_04},		//icon_11_04_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_12_01},		//icon_12_01_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_12_02},		//icon_12_02_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_12_03},		//icon_12_03_I
    { REXY_BMP_NOCOMP, 72, 70, (void*)icon_12_04},		//icon_12_04_I
    { REXY_BMP_NOCOMP, 240, 60, (void*)menus_01},		//menus_01_I
    { REXY_BMP_NOCOMP, 240, 52, (void*)menus_02},		//menus_02_I
    { REXY_BMP_NOCOMP, 240, 52, (void*)menus_03},		//menus_03_I
    { REXY_BMP_NOCOMP, 240, 50, (void*)menus_04},		//menus_04_I
    { REXY_BMP_NOCOMP, 20, 14, (void*)particle_small},		//particle_small_I
    { REXY_BMP_NOCOMP, 8, 8, (void*)snow},		//snow_I
    { REXY_BMP_NOCOMP, 240, 38, (void*)softkey},		//softkey_I
    { REXY_BMP_NOCOMP, 240, 66, (void*)title},		//title_I
    { REXY_BMP_NOCOMP, 240, 216, (void*)back},		//back_I
    { REXY_BMP_NOCOMP, 98, 98, (void*)focusBg},		//focusBg_I
	{ REXY_BMP_NOCOMP, 240, 320, (void*)RexyE_Face},		//RexyE_Face_I

	{ REXY_BMP_NOCOMP, 240, 178, (void*)preMM_Alarm01},		//preMM_Alarm01_I	added by namgung 11-Jan-2006

	// list control example
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno00_n},			//IMGSUBLISTNO00_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno01_n},			//IMGSUBLISTNO01_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno02_n},			//IMGSUBLISTNO02_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno03_n},			//IMGSUBLISTNO03_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno04_n},			//IMGSUBLISTNO04_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno05_n},			//IMGSUBLISTNO05_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno06_n},			//IMGSUBLISTNO06_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno07_n},			//IMGSUBLISTNO07_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno08_n},			//IMGSUBLISTNO08_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno09_n},			//IMGSUBLISTNO09_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno10_n},			//IMGSUBLISTNO10_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno11_n},			//IMGSUBLISTNO11_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno12_n},			//IMGSUBLISTNO12_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno13_n},			//IMGSUBLISTNO13_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno14_n},			//IMGSUBLISTNO14_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno15_n},			//IMGSUBLISTNO15_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno16_n},			//IMGSUBLISTNO16_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno17_n},			//IMGSUBLISTNO17_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno18_n},			//IMGSUBLISTNO18_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno19_n},			//IMGSUBLISTNO19_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno20_n},			//IMGSUBLISTNO20_N_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno00_a},			//IMGSUBLISTNO00_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno01_a},			//IMGSUBLISTNO01_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno02_a},			//IMGSUBLISTNO02_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno03_a},			//IMGSUBLISTNO03_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno04_a},			//IMGSUBLISTNO04_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno05_a},			//IMGSUBLISTNO05_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno06_a},			//IMGSUBLISTNO06_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno07_a},			//IMGSUBLISTNO07_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno08_a},			//IMGSUBLISTNO08_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno09_a},			//IMGSUBLISTNO09_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno10_a},			//IMGSUBLISTNO10_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno11_a},			//IMGSUBLISTNO11_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno12_a},			//IMGSUBLISTNO12_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno13_a},			//IMGSUBLISTNO13_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno14_a},			//IMGSUBLISTNO14_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno15_a},			//IMGSUBLISTNO15_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno16_a},			//IMGSUBLISTNO16_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno17_a},			//IMGSUBLISTNO17_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno18_a},			//IMGSUBLISTNO18_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno19_a},			//IMGSUBLISTNO19_A_I
	{ REXY_BMP_NOCOMP,  24,  24, (void*)imgsublistno20_a},			//IMGSUBLISTNO20_A_I

	//mainmenu
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_call_a1_eng}, // mainmenu added by regina 31-Aug-2006
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_call_a2_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_call_a3_eng},
	{ REXY_BMP_NOCOMP, 223, 43, (void*)imgsmatmenu_call_n_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_camera_a1_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_camera_a2_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_camera_a3_eng},
	{ REXY_BMP_NOCOMP, 223, 43, (void*)imgsmatmenu_camera_n_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_datafolder_a1_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_datafolder_a2_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_datafolder_a3_eng},
	{ REXY_BMP_NOCOMP, 223, 43, (void*)imgsmatmenu_datafolder_n_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_ezweb_a1_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_ezweb_a2_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_ezweb_a3_eng},
	{ REXY_BMP_NOCOMP, 223, 43, (void*)imgsmatmenu_ezweb_n_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_mail_a1_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_mail_a2_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_mail_a3_eng},
	{ REXY_BMP_NOCOMP, 223, 43, (void*)imgsmatmenu_mail_n_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_mynumber_a1_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_mynumber_a2_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_mynumber_a3_eng},
	{ REXY_BMP_NOCOMP, 223, 43, (void*)imgsmatmenu_mynumber_n_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_setting_a1_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_setting_a2_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_setting_a3_eng},
	{ REXY_BMP_NOCOMP, 223, 43, (void*)imgsmatmenu_setting_n_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_tools_a1_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_tools_a2_eng},
	{ REXY_BMP_NOCOMP, 223, 92, (void*)imgsmatmenu_tools_a3_eng},
	{ REXY_BMP_NOCOMP, 223, 43, (void*)imgsmatmenu_tools_n_eng},

  };

