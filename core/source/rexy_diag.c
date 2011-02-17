/*
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	diagnostic command handler
 *
 */
#include "rexy.h"
#include "rexy_halinterface.h"

bool DiagInit(void)
{
	//RalDiagType ret;
	return RalInitDiag();
}

bool DiagDeinit(void)
{
  return RalDeinitDiag();
}

DiagRet DiagHandler(void)
{
	RalDiagType ret;
	DiagCmd cmd;

	ret = RalReadDiagCmd(&cmd);

	if ( ret == RAL_DIAG_SUCCESS )
    {
		TrcPrintF("cmd: %d\n");
    }
	return DIAG_RET_OK;
}

