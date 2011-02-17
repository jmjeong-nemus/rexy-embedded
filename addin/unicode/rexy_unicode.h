#ifndef __REXYE_DBCS_H__
#define __REXYE_DBCS_H__

/** @file */

int GdiTextOutW(WND wnd, ruint16 x, ruint16 y, wchar_t * string, int length);
int FntGetTextWidthExW(FntContextType* cxt, wchar_t * pStr, int len, int space);
UNITCL* FntGetStrImageW(FntContextType* cxt, int x, int y, int width, int height, wchar_t * str, int len, int space);

/** @} */

#endif//__REXYE_DBCS_H__