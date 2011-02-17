/*
 * $Id: rexy_device.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded
 *
 */

#ifndef __REXY_DEVICE_H__
#define __REXY_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rexy_type.h"

#define DISCONNECTED		1
#define CONNECTED			2
#define EMUL_PORT			10002
#define KEYBD_PORT			20002
#define MOUSE_PORT			30002 
#define SCREEN_INFO			0
#define MOUSE_INFO			1
#define KEYBD_INFO			2
#define SCREEN_QUERY		3
#define SCREEN_DATA			4
#define SCREEN_PARTIAL_DATA 5
#define	KEYBOARD_DEVICE		1

    typedef struct _LCD_INFO {
        int screen_width;
        int screen_height;
        int real_screen_width;
        int real_screen_height;
        int x_offset;
        int y_offset;
        int base_height;
        int bpp_mode;
    } LCD_INFO;

    typedef struct _KEY_DATA {
        int msg;
        int key_code;
    } KEY_DATA;

    int rc_open_devices(void);
    void rc_close_devices(void);


    void HostToDev(unsigned int src, unsigned char * dev);
    unsigned int DevToHost(unsigned char * dev);
    unsigned short DevToHostShort(unsigned char * dev);


#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_DEVICE_H__
