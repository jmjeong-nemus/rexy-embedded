#ifndef REGISTRY_H
#define REGISTRY_H
#define REGAPI				extern

#define __REGISTRY_OPEN_CHECK__

typedef enum { 
	REG_KEYTYPE_ROOT = 0,
	REG_KEYTYPE_INT, 
	REG_KEYTYPE_HEX,
	REG_KEYTYPE_STR, 
	REG_KEYTYPE_BIN, 
} TREGValueType;

typedef enum {
	REG_OK = 0,
	REG_ERROR,
	REG_ERROR_NOT_EXIST_KEY,
	REG_ERROR_NOT_EXIST_VALUE,
	REG_ERROR_INVALID_PARAMETER,
	REG_ERROR_ALREADY_INIT,
	REG_ERROR_ALREADY_OPEN,
	REG_ERROR_NOT_OPEN,
	REG_ERROR_NOT_INIT,
	REG_ERROR_CANNOT_OPEN_REGFILE,
	REG_ERROR_KEYNAME_LONG,
	REG_ERROR_KEY_NOT_FOUND,
	REG_ERROR_NOT_ENOUGH_MEM,
	REG_ERROR_NOT_ENOUGH_BUF,
	REG_ERROR_KEYTYPE_NOT_MATCH,
	REG_ERROR_READ_FAIL,
} TREGError;

typedef ruint8 		byte;
typedef ruint16 	uint;
typedef ruint32 	word;

REGAPI TREGError REG_Init(void);
REGAPI TREGError REG_Open(void);
REGAPI TREGError REG_Close(void);
REGAPI TREGError REG_Reset(word total);
REGAPI TREGError REG_Flush(void);
REGAPI TREGError REG_CreateKey(byte* const mompath, byte* const keyname, TREGValueType type, word maxsize);
REGAPI TREGError REG_SetValue(byte* const path, TREGValueType type, word data, word size);
REGAPI TREGError REG_SetValueTemp(byte* const path, TREGValueType type, word data, word size);
REGAPI TREGError REG_QueryValue(byte* const path, TREGValueType type, void* data, word* size);
REGAPI TREGError REG_QueryValuePtr(byte* const path, TREGValueType type, void* data, word* size);
REGAPI TREGError REG_CheckKey(byte* const path);
REGAPI TREGError REG_Reload(void) ;
REGAPI TREGError REG_Free(void);
REGAPI TREGError REG_Check(void);
REGAPI TREGError REG_FlushONOFF(word val);  //val : 0 -> flush disable, val : 1 ->flush enable 

#endif
