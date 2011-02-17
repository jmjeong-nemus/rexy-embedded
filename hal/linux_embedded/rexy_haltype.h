#ifndef _RX_HAL_TYPE_H_
#define _RX_HAL_TYPE_H_

#ifdef __cpluscplus
extern "C" {
#endif

#define RX_HAL_TYPE "LINUX"

#define RX_MSG_ALL	0
#define RX_MSG_KEY	1
#define RX_MSG_TMR	2

     typedef unsigned char ruint8;
     typedef unsigned short ruint16;
     typedef unsigned int ruint32;
     typedef signed char rint8;
     typedef signed short rint16;
     typedef signed int rint32;
     typedef char rchar;
     typedef unsigned char ruchar;


#ifdef __cplusplus
}
#endif	// __cplusplus
#endif//_RX_HAL_TYPE_H_
