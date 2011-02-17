/*
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	diagnostic command handler
 *
 */
#include "rexy.h"

typedef enum {
  DIAG_RET_OK,
  DIAG_RET_FATAL
} DiagRet;

extern bool DiagInit(void);
extern bool DiagDeinit(void);
extern DiagRet DiagHandler(void);


