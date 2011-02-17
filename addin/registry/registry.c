
#include "rexy.h"
#include "rexy_io.h"
#include "registry.h"


#define REGISTRY_FILENAME		"\\RexyE.reg"	

#   define E_OK					0				/* normal end						*/
#   define DPUTS(s)				
#   define ERROR_RETURN(ercd)		return (ercd)
#   define FSTART(fname)
#   define WX(v)
#   define DPRINTF(param)	
#   define sync()





#ifdef __REGISTRY_OPEN_CHECK__

typedef struct _OPENED_APPID {
	ruint32	AppID;	
	struct _OPENED_APPID *NextOpendAppID;
} OPENED_APPID;

static bool g_rexy_registry_debug = TRUE;

static OPENED_APPID* g_RegistryFirst = {0,};

bool CheckAppID(word stateID);
void EraseAppID(word stateID);
OPENED_APPID* GetLastOpenAppID();

#endif

/** @file */

/**
 * @defgroup Registry Registry Function
 * Registry Function
 * @ingroup api
 * @{
 */

/*****************************************************************************
 * User Defined Macros
 ****************************************************************************/
#define EOF					(-1)
#define REG_KEYNAME_LEN		20
#define MAX_KEY_LEVEL		6
#define MAX_BUF_SIZE		999
#define ishexdigit(c)       (((c)>='0'&&c<='9') || ((c)>='a'&&c<='f') || ((c)>='A'&&c<='F'))

/*****************************************************************************
 * User Defined Types
 ****************************************************************************/
typedef struct tagTREGKey {
	byte			name[REG_KEYNAME_LEN+1];
	byte			type;
	byte*			value;
	word			maxsize;
	word			cursize;
	
	struct tagTREGKey* child;
	struct tagTREGKey* next;
} TREGKey;

typedef struct {
	enum {
		REG_TOKEN_EOF = 0,
		REG_TOKEN_ID,
		REG_TOKEN_NUMBER,
		REG_TOKEN_STRING
	} type;
	union {
		word integer;
		byte* string;
	} data;
} TREGToken;

/*****************************************************************************
 * Global Variables
 ****************************************************************************/
static TREGKey RegistryKeyHead;
static bool REG_Inited = FALSE;
static bool REG_Opened = FALSE;

/*****************************************************************************
 * Function Declarations
 ****************************************************************************/
REGAPI TREGError REG_Init(void);
REGAPI TREGError REG_Open(void);
REGAPI TREGError REG_Close(void);
REGAPI TREGError REG_Reset(word total);
REGAPI TREGError REG_Flush(void);
static void REG_FlushSub(word level, TREGKey* key, word fp);
REGAPI TREGError REG_CreateKey(byte* const mompath, byte* const keyname, TREGValueType type, word maxsize);
REGAPI TREGError REG_SetValue(byte* const path, TREGValueType type, word data, word size);
REGAPI TREGError REG_SetValueTemp(byte* const path, TREGValueType type, word data, word size);
REGAPI TREGError REG_QueryValue(byte* const path, TREGValueType type, void* data, word* size);
REGAPI TREGError REG_QueryValuePtr(byte* const path, TREGValueType type, void* data, word* size);
REGAPI TREGError REG_CheckKey(byte* const path);
REGAPI TREGError REG_Init(void);
REGAPI TREGError REG_Reload(void);
static TREGError REG_Load(void);
static TREGToken* REG_ReadToken(word fp);
static void REG_FreeSub(TREGKey* key);
static TREGKey* REG_FindKey(byte* const path);
static TREGKey* REG_FindKeySub(byte** path, TREGKey* key);


/*****************************************************************************
 * DUMMY Standard IO and ETC Functions Declarations
 ****************************************************************************/
extern word mon_cmd_reg(byte *s);
extern word mon_help_reg(byte *s);
//static void uart_printf(byte *fmt,...);
static word Fopen(const byte* filename, const byte* mode);
static word Fclose(word fid);
static word Fgetc(word fid);
static word Fputc(word c, word fid);
static word Fputs(const byte* str, word fid);
static word Fprintf(word fid, byte *fmt, ...);
// static int Fseek(int fid, int offset, int type);
//static byte* Fgets(byte* s, int n, int fid); // not need


/*****************************************************************************
 * Functions
 ****************************************************************************/
/** 
 * Refer 'Registry Service.doc'
 * 
 * 
 * @return 
 */
extern TREGError REG_Init(void)
{
	TREGError err;
	
	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_Init>>"));
	if ( REG_Inited )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Init returned REG_ERROR_ALREADY_INIT"));
		return REG_ERROR_ALREADY_INIT;
	}
	
	RegistryKeyHead.name[0] = '\0';
	RegistryKeyHead.type = REG_KEYTYPE_ROOT;
	RegistryKeyHead.maxsize = 0;
	RegistryKeyHead.cursize = 0;
	RegistryKeyHead.value = NULL;
	RegistryKeyHead.child = NULL;
	RegistryKeyHead.next = NULL;
	
	REG_Inited = TRUE;
	REG_Opened = FALSE;
	
	if ( (err = REG_Load()) != REG_OK )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Init failed(%d)", err));
		return err;
	}
	
	return REG_OK;
}

/** Refer 'Registry Service.doc'
 * 
 * 
 * 
 * @return 
 */
REGAPI TREGError REG_Open(void)
{
	//Joshua commented out below integrity check routine

#ifdef __REGISTRY_OPEN_CHECK__

	bool bRet;
	OPENED_APPID *pOpenedAppID;
	pOpenedAppID = g_RegistryFirst;

	if ( g_rexy_registry_debug )
	{
		StateIDType stateID;
		stateID = StaGetCurrentStateID();

		if(pOpenedAppID->AppID)
		{
			bRet = CheckAppID(stateID);

			if( bRet == FALSE )
			{
				OPENED_APPID *pNewAppID = NULL;
				OPENED_APPID *pLastAppID;
				pLastAppID = GetLastOpenAppID();

				pNewAppID->AppID = stateID;
				pNewAppID->NextOpendAppID = NULL;

				pLastAppID->NextOpendAppID = pNewAppID;
			}
			else
			{
				TrcPrintF("[WARNING] App(%d) cannot open because it open\n", stateID);
				REG_Close();
				EraseAppID(stateID);
				REG_Open();
			}
		}
		else
			pOpenedAppID->AppID = stateID;
			pOpenedAppID->NextOpendAppID = NULL;

	}

	#endif

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_Open>>"));
	REG_Opened = TRUE;
	
	return REG_OK;
}
/** 
 * Refer 'Registry Service.doc'
 * 
 * 
 * @return 
 */
REGAPI TREGError REG_Close(void)
{


	//Joshua commented out below integrity check routine
	/*
	if ( !REG_Opened )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Close returned REG_ERROR_NOT_OPEN"));
		return REG_ERROR_NOT_OPEN;
	}
	*/
#ifdef __REGISTRY_OPEN_CHECK__
	StateIDType stateID;
	stateID = StaGetCurrentStateID();

	EraseAppID(stateID);
#endif

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_Close>>"));
	
	REG_Opened = FALSE;

	return REG_OK;
}


REGAPI TREGError REG_InitVolatileKey(void)
{

	word err ;

	if(REG_Open()!= REG_OK)
	{
//		AS_Trace("open error\n");
		return REG_ERROR;
	}

	err = REG_SetValue("/NOTI/SETTING/regCC_InCallCount", REG_KEYTYPE_INT, 0, 0);
	if(err>0){ TRACE(TR_RexyEtc, TR_LEVEL2, ("Error")); return REG_ERROR;}
	err = REG_SetValue("/NOTI/SETTING/regCC_InCallTime", REG_KEYTYPE_INT, 0, 0);
	if(err>0){ TRACE(TR_RexyEtc, TR_LEVEL2, ("Error")); return REG_ERROR;}
	err = REG_SetValue("/NOTI/SETTING/regCC_CmailCount", REG_KEYTYPE_INT, 0, 0);
	if(err>0){ TRACE(TR_RexyEtc, TR_LEVEL2, ("Error")); return REG_ERROR;}
	err = REG_SetValue("/NOTI/SETTING/regCC_CmailTime", REG_KEYTYPE_INT, 0, 0);
	if(err>0){ TRACE(TR_RexyEtc, TR_LEVEL2, ("Error")); return REG_ERROR;}
	err = REG_SetValue("/NOTI/SETTING/regCC_AnsweringCount", REG_KEYTYPE_INT, 0, 0);
	if(err>0){ TRACE(TR_RexyEtc, TR_LEVEL2, ("Error")); return REG_ERROR;}
	err = REG_SetValue("/NOTI/SETTING/regCC_AnsweringTime", REG_KEYTYPE_INT, 0, 0);
	if(err>0){ TRACE(TR_RexyEtc, TR_LEVEL2, ("Error")); return REG_ERROR;}
	//err = REG_SetValue("/NOTI/SETTING/LOWBATT", REG_KEYTYPE_INT, 0, 0);

	REG_Close();

	return 1;

}


TREGError REG_Free(void)
{
	TREGKey *nextkey, *key = &RegistryKeyHead;

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_Close>>"));
	
	if ( !REG_Inited )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Close returned REG_ERROR_NOT_INIT"));
		return REG_ERROR_NOT_INIT;
	}
	
	if ( REG_Opened )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Close returned REG_ERROR_ALREADY_OPEN"));
		return REG_ERROR_ALREADY_OPEN;
	}
	
	do {
		if ( key->child )
			REG_FreeSub(key->child);
			
		if ( (key->type == REG_KEYTYPE_STR || key->type == REG_KEYTYPE_BIN) && key->value )
			MemFree(key->value);
			
		key->value = NULL;
		key->type = REG_KEYTYPE_ROOT;
		key->maxsize = 0;
		key->cursize = 0;
		key->child = NULL;

		nextkey = key->next;
		key->next = NULL;

		if ( key != &RegistryKeyHead )
			MemFree(key);

		key = nextkey;
	} while ( key );

	return REG_OK;
}

static void REG_FreeSub(TREGKey* key)
{
	TREGKey* nextkey;

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_FreeSub>>"));

	if ( key == NULL )
		return;
		
	do {
		if ( key->child )
			REG_FreeSub(key->child);
			
		if ( (key->type == REG_KEYTYPE_STR || key->type == REG_KEYTYPE_BIN) && key->value )
			MemFree(key->value);
			
		key->value = NULL;
		key->type = REG_KEYTYPE_ROOT;
		key->maxsize = 0;
		key->cursize = 0;
		key->child = NULL;
			
		nextkey = key->next;
		key->next = NULL;

		key = nextkey;
	} while ( key );
}

extern TREGError REG_Reload(void)
{
	TREGError err;
	
	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_Reload>>"));

	if ( !REG_Inited )
		return REG_ERROR_NOT_INIT;
	
	if ( REG_Opened )
		return REG_ERROR_ALREADY_OPEN;
		
	REG_Free();	

	err = REG_Load();

	return err;
}

/** 
 * Refer 'Registry Service.doc'
 * 
 * 
 * @return 
 */
REGAPI TREGError REG_Flush(void)
{
	word fp;
	
	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_Flush>>"));

	if ( !REG_Inited )
		return REG_ERROR_NOT_INIT;

	if ( !REG_Opened )
		return REG_ERROR_NOT_OPEN;

	//if ( (fp = Fopen("\\kanna.reg", "wt")) < 0 )
	if ( (fp = Fopen(REGISTRY_FILENAME, "wt")) < 0 )
		return REG_ERROR_CANNOT_OPEN_REGFILE;
		
	REG_FlushSub(1, RegistryKeyHead.child, fp);		
	Fclose(fp);
	
	return REG_OK;
	// 중간에 실패할 경우를 위한 보다 안전한 저장 방법
	// rename kanna.reg kanna.reg.old
	// rename kanna.reg.new kanna.reg
	// delete kanna.reg.old
}

static void REG_FlushSub(word level, TREGKey* key, word fp)
{
	word i;
	byte c;
	static byte name[REG_KEYNAME_LEN+MAX_KEY_LEVEL+1];

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_FlushSub>>"));

	c = 0x00;
	
	if ( level < 1 || key == NULL || fp < 0 )
		return;
	
	while ( key )
	{		
		Fputc(level + '0', fp);
		
		for ( i=0 ; i<level ; i++ )
			Fputc(' ', fp);
		
		StrCpy(name, key->name);
		for ( i=StrLen(key->name) ; i<REG_KEYNAME_LEN+MAX_KEY_LEVEL ; i++ )
			name[i] = ' ';
		name[REG_KEYNAME_LEN+MAX_KEY_LEVEL-level] = '\0';
		Fputs(name, fp);

		switch ( key->type )
		{				
			case REG_KEYTYPE_ROOT:
				Fputs("R\n", fp);
				break;

			case REG_KEYTYPE_INT:
				Fprintf(fp, "I         %i\n", (word)key->value);
				break;
		
			case REG_KEYTYPE_HEX:
				Fprintf(fp, "H         0x%X\n", (word)key->value);
				break;


        case REG_KEYTYPE_STR:
            Fprintf(fp, "S %03i/%03i \"", key->cursize, key->maxsize);
            for ( i=0 ; (c = key->value[i]) !=0 ; i++ )
            {
                switch ( c )
                {
                case '\"':
                    Fputs("\\\"", fp);
                    break;

                case '\\':
                    Fputs("\\\\", fp);
                    break;

                case '\r':
                    Fputs("\\\r", fp);
                    break;

                case '\t':
                    Fputs("\\\t", fp);
                    break;

                case '\n':
                    Fputs("\\\n", fp);
                    break;

                default:
                    Fputc(c, fp);
                    break;
                }
            }
            Fputs("\"\n", fp);
            break;                 
				
			case REG_KEYTYPE_BIN:
				Fprintf(fp, "B %03i/%03i [ ", key->cursize, key->maxsize);
				for ( i=0 ; i<key->cursize ; i++ )
				{
					byte val;
					val = (byte)(key->value[i]);
					Fprintf(fp, "0x%02X ", val);
				}
				Fputs("]\n", fp);
				break;

			default:
				Fputc('\n', fp);
				break;
		}

		if ( key->child )
			REG_FlushSub(level+1, key->child, fp);

		key = key->next;
	}
}

/** 
 * Refer 'Registey Service.doc'
 * 
 * @param mompath 
 * @param keyname 
 * @param type 
 * @param maxsize 
 * 
 * @return 
 */
REGAPI TREGError REG_CreateKey(byte* const mompath, byte* const keyname, TREGValueType type, word maxsize)
{
	TREGKey *key, *newkey;
	
	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_CreateKey>>"));

	if ( !REG_Inited )
		return REG_ERROR_NOT_INIT;

	if ( !REG_Opened )
		return REG_ERROR_NOT_OPEN;

	if ( !mompath || !keyname || ((type==REG_KEYTYPE_STR || type==REG_KEYTYPE_BIN) && maxsize<=0) )
		ERROR_RETURN(REG_ERROR_INVALID_PARAMETER);
		
	if ( type < REG_KEYTYPE_ROOT || type > REG_KEYTYPE_BIN )
		ERROR_RETURN(REG_ERROR_INVALID_PARAMETER);
		
	if ( StrLen(keyname) > REG_KEYNAME_LEN )
		ERROR_RETURN(REG_ERROR_KEYNAME_LONG);
	
	if ( (key = REG_FindKey(mompath)) == NULL )
		ERROR_RETURN(REG_ERROR_KEY_NOT_FOUND);

	newkey = (TREGKey*) MemAlloc(sizeof(TREGKey));
	if ( !newkey )
		ERROR_RETURN(REG_ERROR_NOT_ENOUGH_MEM);
	StrCpy(newkey->name, keyname);
	newkey->type = (byte)type;
	newkey->value = NULL;
	newkey->maxsize = (word)maxsize;
	newkey->cursize = 0;
	newkey->next = NULL;
	newkey->child = NULL;
	if ( type == REG_KEYTYPE_STR || type == REG_KEYTYPE_BIN )
	{
		newkey->value = (byte*) MemAlloc(maxsize);
		if ( !newkey->value )
			ERROR_RETURN(REG_ERROR_NOT_ENOUGH_MEM);
	}
	else
		newkey->value = NULL;
		
	if ( key->child )
	{
		key = key->child;
		while ( key->next )
			key = key->next;
		
		key->next = newkey;
	}
	else
	{
		key->child = newkey;		
	}
	
	return REG_OK;
}

/** 
 * Refer 'Registry Service.doc'
 * 
 * @param path 
 * @param type 
 * @param data 
 * @param size 
 * 
 * @return 
 */
REGAPI TREGError REG_SetValue(byte* const path, TREGValueType type, word data, word size)
{
	TREGKey *key;
	byte* str = (byte*) data;

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_SetValue>>"));
	
	if ( !REG_Inited )
		return REG_ERROR_NOT_INIT;

	if ( !REG_Opened )
		return REG_ERROR_NOT_OPEN;

	if ( !path || ((type==REG_KEYTYPE_STR || type==REG_KEYTYPE_BIN) && str==NULL) )
		ERROR_RETURN(REG_ERROR_INVALID_PARAMETER);
		
	if ( type <= REG_KEYTYPE_ROOT || type > REG_KEYTYPE_BIN )
		ERROR_RETURN(REG_ERROR_INVALID_PARAMETER);
			
	if ( (key = REG_FindKey(path)) == NULL )
		ERROR_RETURN(REG_ERROR_KEY_NOT_FOUND);
		
	if ( key->type != type )
		ERROR_RETURN(REG_ERROR_KEYTYPE_NOT_MATCH);
		
	if ( type==REG_KEYTYPE_STR && (size=StrLen(str)+1)>key->maxsize )
		ERROR_RETURN(REG_ERROR_NOT_ENOUGH_BUF);
	else if ( type==REG_KEYTYPE_BIN && size>key->maxsize )
		ERROR_RETURN(REG_ERROR_NOT_ENOUGH_BUF);

	switch ( type )
	{
		case REG_KEYTYPE_INT:
		case REG_KEYTYPE_HEX:
			key->value = (void*) data;
			key->maxsize = key->cursize = sizeof(word);
			break;

		case REG_KEYTYPE_STR:
		case REG_KEYTYPE_BIN:
			MemCopy(key->value, str, size);
			key->cursize = (word)size;
			break;
	}	
	
	return REG_OK;
}

/** 
 * Refer 'Registry Service.doc'
 * 
 * @param path 
 * @param type 
 * @param data 
 * @param size 
 * 
 * @return 
 */
REGAPI TREGError REG_SetValueTemp(byte* const path, TREGValueType type, word data, word size)
{
	TREGKey *key;
	byte* str = (byte*) data;

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_SetValueTemp>>"));

	if ( !REG_Inited )
		return REG_ERROR_NOT_INIT;

	if ( !REG_Opened )
		return REG_ERROR_NOT_OPEN;
	
	if ( !path || ((type==REG_KEYTYPE_STR || type==REG_KEYTYPE_BIN) && str==NULL) )
		ERROR_RETURN(REG_ERROR_INVALID_PARAMETER);
		
	if ( type <= REG_KEYTYPE_ROOT || type > REG_KEYTYPE_BIN )
		ERROR_RETURN(REG_ERROR_INVALID_PARAMETER);
			
	if ( (key = REG_FindKey(path)) == NULL )
		ERROR_RETURN(REG_ERROR_KEY_NOT_FOUND);
		
	if ( key->type != type )
		ERROR_RETURN(REG_ERROR_KEYTYPE_NOT_MATCH);
		
	if ( type==REG_KEYTYPE_STR && (size=StrLen(str)+1)>key->maxsize )
		ERROR_RETURN(REG_ERROR_NOT_ENOUGH_BUF);
	else if ( type==REG_KEYTYPE_BIN && size>key->maxsize )
		ERROR_RETURN(REG_ERROR_NOT_ENOUGH_BUF);

	switch ( type )
	{
		case REG_KEYTYPE_INT:
		case REG_KEYTYPE_HEX:
			key->value = (void*) data;
			key->maxsize = key->cursize = sizeof(word);
			break;

		case REG_KEYTYPE_STR:
		case REG_KEYTYPE_BIN:
			MemCopy(key->value, str, size);
			key->cursize = (word)size;
			break;
	}	
	
	return REG_OK;
}
/** 
 * @brief control registry flush policy
 * @param val : 0 -> flush disable, val : 1 ->flush enable 
 * @ifdef HISTORY
 *		25-May-2007	Joshua	Created
 * @endif
 */
REGAPI TREGError REG_FlushONOFF(word val)
{
	return REG_OK;
};

/** 
 * Refer 'Registry Service.doc'
 * 
 * @param path 
 * @param type 
 * @param data 
 * @param size 
 * 
 * @return 
 */
REGAPI TREGError REG_QueryValue(byte* const path, TREGValueType type, void* data, word* size)
{
	TREGKey *key;
//	byte* str = (byte*) data;		

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_QueryValue>>"));
	
	if ( !REG_Inited )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_QueryValue : REG_ERROR_NOT_INIT"));
		return REG_ERROR_NOT_INIT;
	}

	if ( !REG_Opened )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_QueryValue : REG_ERROR_NOT_OPEN"));
		return REG_ERROR_NOT_OPEN;
	}

	if ( !path || data == NULL )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_QueryValue : REG_ERROR_INVALID_PARAMETER"));
		return REG_ERROR_INVALID_PARAMETER;
	}

	// mococo hjim 2005-03-15 modify type check	
	if ( type <= REG_KEYTYPE_ROOT /*|| type > REG_KEYTYPE_HEX*/ )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_QueryValue : REG_ERROR_INVALID_PARAMETER"));
		return REG_ERROR_INVALID_PARAMETER;
	}
			
	if ( (key = REG_FindKey(path)) == NULL )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_QueryValue : REG_ERROR_KEY_NOT_FOUND(%s)", path));
		return REG_ERROR_KEY_NOT_FOUND;
	}
		
	if ( key->type != type )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_QueryValue : REG_ERROR_KEYTYPE_NOT_MATCH"));
		return REG_ERROR_KEYTYPE_NOT_MATCH;
	}
	
	switch ( type )
	{
		case REG_KEYTYPE_INT:
		case REG_KEYTYPE_HEX:
			*((word*)data) = (word) key->value;
			if ( size ) *size = sizeof(word);
			break;

		case REG_KEYTYPE_BIN:
		case REG_KEYTYPE_STR:
			MemCopy(data, key->value, key->cursize);
			if ( size ) *size = key->cursize;
			break;
	}	
		
	return REG_OK;
}

/** 
 * Refer 'Registry Service.doc'
 * 
 * @param path 
 * @param type 
 * @param data 
 * @param size 
 * 
 * @return 
 */
REGAPI TREGError REG_QueryValuePtr(byte* const path, TREGValueType type, void* data, word* size)
{
	TREGKey *key;
//	byte* str = (byte*) data;
	
	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_QueryValuePtr>>"));

	if ( !REG_Inited )
		return REG_ERROR_NOT_INIT;

	if ( !REG_Opened )
		return REG_ERROR_NOT_OPEN;

	if ( !path || data == NULL )
		return REG_ERROR_INVALID_PARAMETER;
	//2005-04-13 hjim KEY_TYPE_BIN	
	if ( type <= REG_KEYTYPE_ROOT || type > REG_KEYTYPE_BIN/*type > REG_KEYTYPE_HEX*/ )
		return REG_ERROR_INVALID_PARAMETER;
			
	if ( (key = REG_FindKey(path)) == NULL )
		return REG_ERROR_KEY_NOT_FOUND;
		
	if ( key->type != type )
		return REG_ERROR_KEYTYPE_NOT_MATCH;
	
	switch ( type )
	{
		case REG_KEYTYPE_INT:
		case REG_KEYTYPE_HEX:
			*((word*)data) = (word) key->value;
			if ( size ) *size = sizeof(word);
			break;

		case REG_KEYTYPE_STR:
		case REG_KEYTYPE_BIN:
			*((byte**)data) = key->value;
			if ( size ) *size = key->cursize;
			break;
	}	
		
	return REG_OK;
}

/** 
 * Refer 'Registry Service.doc'
 * 
 * @param path 
 * 
 * @return 
 */
REGAPI TREGError REG_CheckKey(byte* const path)
{
	TREGKey* pKey = NULL;

	pKey = REG_FindKey(path);

	if(pKey)
		return REG_OK;
	else
		return REG_ERROR_KEY_NOT_FOUND;
}

static TREGKey* REG_FindKey(byte* const path)
{
	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_FindKey>>"));

	if ( path == NULL )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_FindKey : NULL"));
		return NULL;
	}
		
	if ( !StrCmp(path, "\\") || !StrCmp(path, "/") )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_FindKey : head"));
		return &RegistryKeyHead;
	}

	if ( RegistryKeyHead.child == NULL )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_FindKey : NULL"));
		return NULL;
	}

	return REG_FindKeySub((byte**)&path, RegistryKeyHead.child);
}

static TREGKey* REG_FindKeySub(byte** path, TREGKey* key)
{
	word i;
	byte keyname[REG_KEYNAME_LEN+1];
#ifdef _DEBUG
	byte* tpath;
	tpath = *(path);
#endif

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_FindKeySub>>"));
	
	if ( **path != '\\' && **path != '/' )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_FindKeySub : NULL"));
		return NULL;
	}	

	(*path)++;
	for ( i=0 ; isalnum(**path) || **path == '_' ; ) 
	{
		keyname[i++] = *(*path)++;
		if ( i > REG_KEYNAME_LEN )
		{
			TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_FindKeySub : too long key name(%s)", path));
			return NULL;
		}
	}
	keyname[i] = '\0';
	
	TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_FindKeySub : try to find key(%s)", keyname));
	while ( key )
	{
		if ( !StrCmp(key->name, keyname) )
		{
			if ( **path == '\\' || **path == '/' )
				return REG_FindKeySub(path, key->child);
			else
				return key;
		}
		key = key->next;
	}
	
	TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_FindKeySub : can't find key(%s)", keyname));
	return NULL;
}


static TREGToken* REG_ReadToken(word fp)
{
	word len;
	static byte ch;
	static bool hexmode;
	static TREGToken token;
	static byte buf[1000];
	static enum { SKIPING, MAKING_TOKEN_ID, MAKING_TOKEN_NUMBER, MAKING_TOKEN_STRING } state;

	FSTART("REG_ReadToken");

	len = 0;

	token.type = REG_TOKEN_EOF;
	token.data.integer = 0;

	// 전역변수를 쓰지 않고 함수내의 static 변수로 대체하는 방법을 쓰기 때문에 이런 초기화 방법이 필요
	if ( fp < 0 )
	{
		hexmode = FALSE;
		state = SKIPING;
		return &token;
	}

	while ( 1 )
	{
		ch = (byte)Fgetc(fp);

		switch ( state )
		{
			case SKIPING:
				if ( isalpha(ch) || ch == '_' )
				{
					state = MAKING_TOKEN_ID;
					len = 0;
					buf[len++] = ch;
				}
				else if ( isdigit(ch) || (hexmode && ishexdigit(ch)) )
				{
					state = MAKING_TOKEN_NUMBER;
					len = 0;
					buf[len++] = ch;
				}
				else if ( ch == '\"' )
				{
					state = MAKING_TOKEN_STRING;
					len = 0;					
				}
				else if ( ch == '[' )
				{
					hexmode = TRUE;
				}
				else if ( ch == ']' )
				{
					hexmode = FALSE;
				}
				else if ( ch == EOF )
				{
					TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_ReadToken : REG_TOKEN_EOF"));
					return &token;
				}
				break;
			
			case MAKING_TOKEN_ID:
				if ( isalnum(ch) || ch == '_' )
				{
					buf[len++] = ch;
					continue;
				}
				else
				{
					buf[len] = '\0';
					token.type = REG_TOKEN_ID;
					token.data.string = buf;
					state = SKIPING;
					DPRINTF(("REG_TOKEN_ID: %s\n", token.data.string));
					return &token;
				}
				break;
			
			case MAKING_TOKEN_NUMBER:
				if ( buf[0]=='0' && len==1 && (ch=='X' || ch=='x') )
				{
					buf[len++] = ch;
				}
				else if ( ishexdigit(ch) )
				{
					buf[len++] = ch;
					continue;
				}
				else
				{
					buf[len] = '\0';
					token.type = REG_TOKEN_NUMBER;
					if ( !StrNCmp(buf, "0x", 2) || !StrNCmp(buf, "0X", 2) )
						token.data.integer = strtol(buf+2, NULL, 16);
					else if ( hexmode )
						token.data.integer = strtol(buf, NULL, 16);
					else
						token.data.integer = strtol(buf, NULL, 10);
					state = SKIPING;
					DPRINTF(("REG_TOKEN_NUMBER: %i\n", token.data.integer));
					return &token;
				}
				break;

			case MAKING_TOKEN_STRING:
				if ( ch == '\\' )
				{
					ch = (byte)Fgetc(fp);
					switch ( ch )
					{
						case '\"':
							buf[len++] = '\"';
							break;
						case '\\':
							buf[len++] = '\\';
							break;
						case 't':
							buf[len++] = '\t';
							break;
						case 'r':
							buf[len++] = '\r';
							break;
						case 'n':
							buf[len++] = '\n';
							break;
					}
				}
				else if ( ch == '\"' )
				{
					buf[len] = '\0';
					token.type = REG_TOKEN_STRING;
					token.data.string = buf;
					state = SKIPING;
					DPRINTF(("REG_TOKEN_STRING: %s\n", token.data.string));
					return &token;
				}
				else if ( ch != EOF )
				{
					buf[len++] = ch;
				}
				else // ch == EOF
				{
					DPUTS("REG_TOKEN_EOF");
					return &token;
				}
				break;

			default:
				DPUTS("ILLEGAL_STATE");
				break;
		}		
	}
}

TREGError REG_Check(void)
{
	word i, curdepth;
	TREGKey* key[MAX_KEY_LEVEL];

	key[0] = &RegistryKeyHead;
	curdepth = 0;

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_Check>>"));
	
	do
	{
		for ( i=0 ; i<=curdepth ; i++ )
		{			
			DPRINTF(("%s/", key[i]->name));
		}

		switch ( key[curdepth]->type )
		{
			case REG_KEYTYPE_ROOT:
				DPRINTF(("\n"));
				break;
			case REG_KEYTYPE_INT:
				DPRINTF((" INT(%i)\n", key[curdepth]->value));
				break;
			case REG_KEYTYPE_HEX:
				DPRINTF((" HEX(0x%X)\n", key[curdepth]->value));
				break;
			case REG_KEYTYPE_BIN:
				DPRINTF((" BIN(%i/%i)(%s)\n", key[curdepth]->cursize, key[curdepth]->maxsize, key[curdepth]->value));
				break;
			case REG_KEYTYPE_STR:
				DPRINTF((" STR(%i/%i)(%s)\n", key[curdepth]->cursize, key[curdepth]->maxsize, key[curdepth]->value));
				break;
			default:
				DPRINTF((" UNKNOWN_TYPE\n"));
				return REG_ERROR;
		}
		
		if ( key[curdepth]->child )
		{
			key[curdepth+1] = key[curdepth]->child;
			curdepth++;

			if ( curdepth >= MAX_KEY_LEVEL )
			{
				DPUTS("EXCEED MAX_KEY_LEVEL");
				return REG_ERROR;
			}
		}
		else if ( key[curdepth]->next )
		{
			key[curdepth] = key[curdepth]->next;
		}
		else
		{
			do {
				curdepth--;
			} while ( key[curdepth]->next == NULL && curdepth > 0 );

			if ( curdepth > 0 )
				key[curdepth] = key[curdepth]->next;
		}
	} while ( curdepth > 0 );

	return REG_OK;
}


static TREGError REG_Load(void)
{
	word i, fp, new_level, last_level=0;
	TREGKey key;
	TREGKey* newkey;
	TREGToken* token;
	TREGKey* tree[MAX_KEY_LEVEL] = { NULL, };

	TRACE(TR_RexyEtc, TR_LEVEL1, ("<<REG_Load>>"));

	FSTART("REG_Load");
	
	if ( !REG_Inited )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load returned REG_ERROR_NOT_INIT"));
		ERROR_RETURN(REG_ERROR_NOT_INIT);
	}

	if ( REG_Opened )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load returned REG_ERROR_ALREADY_OPEN"));
		ERROR_RETURN(REG_ERROR_ALREADY_OPEN);
	}

	if ( (fp = Fopen(REGISTRY_FILENAME, "rt")) < 0 )
	{
		TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load returned REG_ERROR_CANNOT_OPEN_REGFILE"));
		ERROR_RETURN(REG_ERROR_CANNOT_OPEN_REGFILE);
	}

	tree[0] = &RegistryKeyHead;
	REG_ReadToken(-1);

	while ( 1 )
	{
		key.name[0] = '\0';
		key.type = REG_KEYTYPE_ROOT;
		key.value = NULL;
		key.cursize = key.maxsize = 0;
		key.child = NULL;
		key.next = NULL;

		if ( (token = REG_ReadToken(fp))->type == REG_TOKEN_NUMBER )
		{
			new_level = token->data.integer;
		}
		else if ( token->type == REG_TOKEN_EOF )
		{
			TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : Finally Read Complete"));
			DPUTS("Finally Read Complete");
			break;
		}
		else 
		{
			TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cannot find keylevel"));
			DPUTS("cannot find keylevel");
			goto error;
		}

		if ( (token = REG_ReadToken(fp))->type == REG_TOKEN_ID )
		{
			if ( StrLen(token->data.string) > REG_KEYNAME_LEN )
			{
				TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : keyname is very long"));
				DPUTS("keyname is very long");
				goto error;
			}

			StrCpy(key.name, token->data.string);
		}
		else 
		{
			TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cannot find keyname"));
			DPUTS("cannot find keyname");
			goto error;
		}

		if ( (token = REG_ReadToken(fp))->type == REG_TOKEN_ID )
		{
			switch ( token->data.string[0] )
			{
				case 'R':
					key.type = REG_KEYTYPE_ROOT;
					goto skip;
					break;

				case 'I':
					key.type = REG_KEYTYPE_INT;
					break;

				case 'H':
					key.type = REG_KEYTYPE_HEX;
					break;

				case 'B':
					key.type = REG_KEYTYPE_BIN;
					break;

				case 'S':
					key.type = REG_KEYTYPE_STR;
					break;

				default:
					{
						DPUTS("cannot find keytype");
						goto error;
					}
			}

		}
		else 
		{
			goto error;
		}

		if ( key.type == REG_KEYTYPE_BIN || key.type == REG_KEYTYPE_STR )
		{
			if ( (token = REG_ReadToken(fp))->type == REG_TOKEN_NUMBER )
			{
				key.cursize = (word)token->data.integer;
			}
			else 
			{
				TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cannot find cursize"));
				DPUTS("cannot find cursize");
				goto error;
			}

			if ( (token = REG_ReadToken(fp))->type == REG_TOKEN_NUMBER )
			{
				key.maxsize = (word)token->data.integer;
			}
			else 
			{
				TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cannot find maxsize"));
				DPUTS("cannot find maxsize");
				goto error;
			}

			if ( key.cursize > key.maxsize )
			{
				TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cursize > maxsize"));
				DPUTS("cursize > maxsize");
				goto error;
			}
		}

		switch ( key.type )
		{
			case REG_KEYTYPE_INT:
				if ( (token = REG_ReadToken(fp))->type == REG_TOKEN_NUMBER )
				{
					key.value = token->data.string;
				}
				else 
				{
					TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cannot find value"));
					DPUTS("cannot find value");
					goto error;
				}
				break;

			case REG_KEYTYPE_HEX:
				if ( (token = REG_ReadToken(fp))->type == REG_TOKEN_NUMBER )
				{
					key.value = token->data.string;
				}
				else 
				{
					TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cannot find value"));
					DPUTS("cannot find value");
					goto error;
				}
				break;

			case REG_KEYTYPE_BIN:
				if ( (key.value = (byte*)MemAlloc(key.maxsize)) == NULL )
				{
					TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : not enough mem"));
					DPUTS("not enough mem");
					goto error;
				}

				for ( i=0 ; i<key.cursize ; i++ )
					if ( (token = REG_ReadToken(fp))->type==REG_TOKEN_NUMBER )
						key.value[i] = (byte)token->data.integer;
					else 
					{
						TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cannot find value"));
						DPUTS("cannot find value");
						goto error;
					}
				break;

			case REG_KEYTYPE_STR:
				if ( (token = REG_ReadToken(fp))->type == REG_TOKEN_STRING )
				{
					word len = StrLen(token->data.string) + 1;

					if ( (key.value = (byte*)MemAlloc(key.maxsize)) == NULL )
					{
						TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : not enough mem"));
						DPUTS("not enough mem");
						goto error;
					}

					if ( len > key.maxsize || len > key.cursize )
					{
						TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : string is very long"));
						DPUTS("string is very long");
						goto error;
					}

					StrCpy(key.value, token->data.string);
				}
				else
				{
					TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : cannot find value"));
					DPUTS("cannot find value");
					goto error;
				}
				break;
		}
skip:
		if ( (newkey = (TREGKey*)MemAlloc(sizeof(TREGKey))) == NULL )
		{
			TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : not enough mem"));
			DPUTS("not enough mem");
			goto error;
		}
		MemCopy(newkey, &key, sizeof(TREGKey));
		
		if ( new_level < last_level )
		{	
			last_level = new_level;
			if ( tree[last_level] )
				tree[last_level]->next = newkey;
			tree[last_level] = newkey;
		}
		else if ( new_level == last_level+1 )
		{
			if ( tree[last_level] )
			{
				WX(tree[last_level]);
				tree[last_level]->child = newkey;
			}

			tree[new_level] = newkey;
			last_level = new_level;
		}
		else if ( new_level == last_level )
		{
			if ( tree[last_level] )
				tree[last_level]->next = newkey;
			tree[last_level] = newkey;
		}
		else
		{
			TRACE(TR_RexyEtc, TR_LEVEL2, ("REG_Load : new_level and last_level is strange"));
			DPUTS("new_level and last_level is strange");
			MemFree(newkey);
			goto error;
		}
	}

	Fclose(fp);
	return REG_OK;

error:
	if ( key.type == REG_KEYTYPE_BIN || key.type == REG_KEYTYPE_STR )
		if ( key.value != NULL )
			MemFree(key.value);

	Fclose(fp);
	return REG_ERROR_READ_FAIL;		
}


/*****************************************************************************
 * DUMMY Standard IO Functions
 ****************************************************************************/

static word Fopen(const byte* filename, const byte* mode)
{
	FSTART("Fopen");
	
	if ( (!StrCmp(mode, "wt")) )
	{
		 return FsCreateFile(filename);  
	}
	else if ( (!StrCmp(mode, "rt"))  || (!StrCmp(mode, "w+t")) )
	{
		return FsOpenFile(filename);
	}
	else return -1;
}


static word Fclose(word fid)
{
	word ret;
	FSTART("Fclose");
	ret = FsCloseFile(fid);
	sync();
	return ret;
}


static word Fputs(const byte* str, word fid)
{
	return FsWriteFile(fid, str, StrLen(str));
}


static word Fprintf(word fid, byte *fmt, ...)
{
    va_list ap;
    byte string[256];
	word retval;
    string[0] = '\0';

	va_start(ap,fmt);
	vsprintf(string, fmt, ap); 
    retval = Fputs(string, fid);
    va_end(ap);

	return retval;
}


//static int Fseek(int fid, int offset, int type)
//{
//	return AS_LseekFile(fid, offset, type);
//}

static word Fgetc(word fid)
{
	byte c;
	
	if ( FsReadFile(fid, &c, 1) <= 0 )
		return EOF;
	else
		return c;
}

static word Fputc(word c, word fid)
{
	byte ch = (byte)c;
	if ( FsWriteFile(fid, &ch, 1) <= 0 )
		return EOF;
	else
		return c;
}

extern word mon_cmd_reg(byte *s)
{	
	if ( !StrNCmp(s, "init", 4) )
	{
		REG_Init();
	}
	else if ( !StrNCmp(s, "reload", 4) )
	{
		REG_Reload();
	}
	else if ( !StrNCmp(s, "check", 5) )
	{
		REG_Check();
	}
	else if ( !StrNCmp(s, "flush", 5) )
	{
		REG_Open();
		REG_Flush();
		REG_Close();
	}
	else if ( !StrNCmp(s, "new", 4) )
	{
		REG_Free();

		REG_Open();

		REG_CreateKey("/", "M_Function", REG_KEYTYPE_ROOT, 0);

		REG_CreateKey("/M_Function", "Profile", REG_KEYTYPE_HEX, 0);
		REG_CreateKey("/M_Function/Profile", "Name", REG_KEYTYPE_HEX, 0);
		REG_CreateKey("/M_Function/Profile", "AddrRecordID", REG_KEYTYPE_HEX, 0);

		REG_CreateKey("/M_Function", "Display", REG_KEYTYPE_HEX, 0);


		REG_CreateKey("/M_Function/Display", "Main", REG_KEYTYPE_BIN, 50);
		REG_CreateKey("/M_Function/Display", "Sub", REG_KEYTYPE_BIN, 50);

		REG_CreateKey("/", "Dialer", REG_KEYTYPE_INT, 0);

		REG_Close();
	}
	else
	{
		//Uart_SendString("REG: unknown command\n");
	}

	return E_OK;
}

extern word mon_help_reg(byte* s)
{
	//Uart_SendString("Registy Service Test\n");
	return E_OK;
}

/*
static void uart_printf(byte *fmt,...)
{
	
    va_list ap;
    byte string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(string);
    va_end(ap);
	
}
*/


bool CheckAppID(word stateID)
{
	OPENED_APPID* checkAppID = g_RegistryFirst; 
	while(checkAppID->AppID)
	{
		if(checkAppID->AppID == stateID)
			return TRUE;

		checkAppID = checkAppID->NextOpendAppID;		
	}

	return FALSE;
}
void EraseAppID(word stateID)
{
	OPENED_APPID* prevAppID = NULL;
	OPENED_APPID* checkAppID = g_RegistryFirst;
	
	while(checkAppID->AppID)
	{
		if(checkAppID->AppID == stateID)
		{
			prevAppID->NextOpendAppID = checkAppID->NextOpendAppID;
			return;
		}
		prevAppID = checkAppID;
		checkAppID = checkAppID->NextOpendAppID;
	}
}

OPENED_APPID* GetLastOpenAppID()
{
	OPENED_APPID* lastAppID = g_RegistryFirst;
	
	while(lastAppID->AppID)
	{
		lastAppID = lastAppID->NextOpendAppID;
	}

	return lastAppID;
}

/** @} */ // End of Registry
