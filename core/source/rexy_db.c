/************************************************************************
 * 
 *	File	: RxNewDB.c                                                             
 *	Creator : Joshua ( Restructured )
 *	Date	: 14-Sep-2007
 *
 ************************************************************************/

#include "rexy.h"

/*
#ifndef NULL
#define NULL	0
#endif
*/

#define DBASSERT(x)		ASSERT(x)
#define FILE_ERROR	(-1)

#define DB_HEADER_STRING		"RexyNewDB"
#define DB_HEADER_STRING_LEN	9		// string length of DB_HEADER_STRING
#define REXY_NEW_DB_EXT			".jdb"

#define DB_VERSION				3
#define DB_FIELD_NAME_LEN		32

#define MAX_DB_RECORD		1024		// Must be can be diveded by 8

#define DBS_SHORT_TYPE		unsigned short
#define DBS_LONG_TYPE		ruint32

typedef struct {
	char			HeaderString[DB_HEADER_STRING_LEN];		// RexyNewDB
	unsigned short	Version;
	unsigned short	RecordNum;
	rint32				FieldNum;								// the number of fields in a record(4 byte)	
	unsigned char	BitmapTable[MAX_DB_RECORD/8];
} rx_db_header_type;

typedef struct {
	HRECORD	RecordID;
	bool	bDirty;
	void	*RecordChunk;
} rx_record_blob_type;

typedef struct {

	struct rx_db_info_type *PrevInfo;
	struct rx_db_info_type *NextInfo;
	
	char	*TableName;
	rint32		RefCount;
	HTABLE	TableHandle;
	rint32		FileHandle;
	bool	Exclusive;
	rx_db_header_type *TableHeader;
	field_type		*FieldTypeInfo;					// types of each field in a record
	rx_record_blob_type	RecordBlob;					// Temporal record storage, also works as cache buffer
	rint32		RecSize;
	
} rx_db_info_type;

typedef struct {

	struct rx_db_view_type *PrevView;
	struct rx_db_view_type *NextView;
	
	HTABLE		TableHandle;
	HVIEW		ViewHandle;
	CHECKPROC	CheckProc;
	COMPPROC	CompProc;
	rint32			NumRecord;
	rint32			SeekPosition;
	HRECORD		*RecordNoArray;

} rx_db_view_type;

static rint32	g_error_code = 0;

static rint32	g_db_count	= 0;		// how many different databases are open regardless of the reference count of each one
static rint32	g_view_count = 0;		// how may views are created regardless of its parent table

static rx_db_info_type *g_db_info_start = NULL;
static rx_db_info_type *g_db_info_end = NULL;

static rx_db_view_type *g_db_view_start = NULL;
static rx_db_view_type *g_db_view_end = NULL;

// Rint32ernal Functions Prototypes

// Rint32egrity Check
static bool		PrvCheckTableRint32egrity( HTABLE table_id );		// debugging purpose only
static bool		PrvCheckRecordRint32egrity( rx_db_header_type *TableHeader, HRECORD record_id );

// Table Operation
static HTABLE	PrvGetCleanTableHandle( void );
static HTABLE	PrvCreateTable( char *TableName, field_type * schema, rint32 field_num );
static HTABLE	PrvOpenTable( char *TableName );

// View Operation
static HVIEW	PrvGetCleanViewHandle( void );
static bool		PrvCreateView( HTABLE table_id, CHECKPROC check_proc, COMPPROC comp_proc, HVIEW view_id );
static void		PrvEraseView( HVIEW view_id );
static rx_db_view_type	*PrvGetViewChunk( HVIEW ViewHandle );
static bool		PrvDeleteViewChunk( HVIEW ViewHandle );

// Record Operation
static rint32		PrvCalcRecordSize( rx_db_info_type *DBInfo );
static rint32		PrvGetEmptySlot( unsigned char *BitmapTable );
static ruint32 PrvGetRecordFileOffset( rx_db_info_type *DBInfo, rint32 SlotNo );
static bool		PrvFillRecordBlob( rx_db_info_type *DBInfo, field_data *record );
static bool		PrvWriteRecord( rx_db_info_type *DBInfo, HRECORD RecordID );
static bool		PrvReadRecord( rx_db_info_type *DBInfo, HRECORD RecordID );
static bool		PrvGetRecordSlotStatus( unsigned char *BitmapTable, rint32 nPos );
static void		PrvSetRecordSlotStatus( unsigned char *BitmapTable, rint32 nPos, bool bValue );


// Field Operation
static rint32		PrvGetFieldOffset( rx_db_info_type *DBInfo, char *FieldName, rint32 FieldIndex );
static rint32		PrvGetFieldIndexByName( rx_db_info_type *DBInfo, char *FieldName );

// Table Info Chunk
static rx_db_info_type	*PrvGetInfoChunk( HTABLE TableHandle );
static rx_db_info_type	*PrvAddInfoChunk( HTABLE TableHandle );
static bool				PrvDeleteInfoChunk( HTABLE TableHandle );
static rx_db_header_type	*PrvGetDBHeader( HTABLE TableHandle );

// Mischievous
static void		PrvSyncHeaderToFile( rx_db_info_type *DBInfo );
static void		PrvMergeSort(HRECORD * data_array, rint32 start, rint32 end, HTABLE table_id, COMPPROC comp_proc, HRECORD *TempBuffer );
static char		*PrvCatDBFileExt( char *TableName );


//--------------------------------------------------------

/* db error code retrieval */
rint32 DB_GetDBErrorCode (void)
{
	return g_error_code;
}


//	table operation
bool DB_FindTable(char * table_name)
{
	rint32 fd;
	char *DBFileName;
	
	if ( !table_name )
	{
		g_error_code = DBERR_NONEXISTING_TABLE;
		return FALSE;
	}

	DBFileName = PrvCatDBFileExt( table_name );
	fd = FsOpenFile(DBFileName);
	MemFree( DBFileName );

	if ( fd > FILE_ERROR )
	{
		AS_CloseFile(fd);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

HTABLE DB_OpenTable(char * table_name, field_type * schema, rint32 field_num)
{
	bool	bFileExist;
	HTABLE	TableHandle;

	// Check Structure Rint32egrity
	ASSERT( MAX_DB_RECORD % 8 == 0 );

 	if ( !AS_StrLen( table_name ) )
	{
		g_error_code = DBERR_NONEXISTING_TABLE;
		return INVALID_TABLE_ID;
	}

	bFileExist = DB_FindTable( table_name );

	if ( schema )	// Create New One
	{
		if ( bFileExist )
		{
			g_error_code = DBERR_DUPLICATE_TABLE;
			TableHandle = INVALID_TABLE_ID;
		}
		else
		{
			TableHandle = PrvCreateTable( table_name, schema, field_num );
		}
	}
	else	// Open Existing One
	{
		if ( bFileExist )
		{
			TableHandle = PrvOpenTable( table_name );
		}
		else
		{
			g_error_code = DBERR_NONEXISTING_TABLE;
			TableHandle =  INVALID_TABLE_ID;
		}
	}

#ifdef WIN32
	if ( TableHandle != INVALID_TABLE_ID ) PrvCheckTableRint32egrity( TableHandle );
#endif

	return TableHandle;
}

void DB_CloseTable(HTABLE table_id)
{
	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	
#ifdef WIN32
	PrvCheckTableRint32egrity( table_id );
#endif

	if ( DBInfo ) 
	{
		PrvSyncHeaderToFile( DBInfo );
		DBInfo->RefCount--;

		if ( !DBInfo->RefCount )
		{
			// Calculate File Size
			ruint32	NewSize;
			rint32				LastSlot;

			LastSlot = PrvGetLastSlot( DBInfo->TableHeader->BitmapTable );
			NewSize = sizeof( rx_db_header_type ) + sizeof( field_type ) * DBInfo->TableHeader->FieldNum + ( LastSlot + 1 ) * DBInfo->RecSize; 
			AS_ShrinkFile( DBInfo->FileHandle, NewSize );
			AS_CloseFile( DBInfo->FileHandle );
			PrvDeleteInfoChunk( table_id );
		}
	}
}

void DB_DeleteTable(HTABLE table_id)
{
	rx_db_info_type *DBInfo;
	rx_db_header_type *DBDeleteHeader = 0;

	DBInfo = PrvGetInfoChunk( table_id );
	if ( DBInfo )
	{
		DBDeleteHeader = DBInfo->TableHeader;

		//연결된 DB Record 관련정보 삭제
		//1. Record갯수 0으로 셋팅
		DBDeleteHeader->RecordNum = 0;

		//2. BitmapTable Reset
		AS_MemSet( DBDeleteHeader->BitmapTable, 0, MAX_DB_RECORD/8 );

	#ifdef WIN32
		PrvCheckTableRint32egrity( table_id );
	#endif
	}
}

//	record operation
HRECORD DB_InsertRecord(HTABLE table_id, field_data * record)
{

	rx_db_info_type	  *DBInfo;
	HRECORD			  RecordID;

	DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return INVALID_RECORD_ID;

	RecordID = PrvGetEmptySlot( DBInfo->TableHeader->BitmapTable );

	if ( RecordID != INVALID_RECORD_ID )	// except storage full
	{
		PrvFillRecordBlob( DBInfo, record );
		PrvWriteRecord( DBInfo, RecordID );

		// Increase record num
		DBInfo->TableHeader->RecordNum++;
	}
	else
	{
		g_error_code = DBERR_RECORD_FULL;
		RecordID = INVALID_RECORD_ID;
	}

#ifdef WIN32
	PrvCheckTableRint32egrity( table_id );
#endif

	return RecordID;
}

bool DB_DeleteRecord(HTABLE table_id, HRECORD record_id)
{
	rx_db_info_type	  *DBInfo;

	DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return FALSE;

	// Already deleted Record ?
	if ( !PrvCheckRecordRint32egrity( DBInfo->TableHeader, record_id ) ) return FALSE;

	// Slot상태 FALSE로 변경
	PrvSetRecordSlotStatus(DBInfo->TableHeader->BitmapTable, record_id, FALSE);
	
	// Decrease Record Num
	DBInfo->TableHeader->RecordNum--;

#ifdef WIN32
	PrvCheckTableRint32egrity( table_id );
#endif

	return TRUE;

}

bool DB_UpdateRecord(HTABLE table_id, HRECORD record_id, field_data * new_data)
{
	rx_db_info_type		*DBInfo;
	rint32					i;

	DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return FALSE;

	// Already deleted Record ?
	if ( !PrvCheckRecordRint32egrity( DBInfo->TableHeader, record_id ) ) return FALSE;

	// Check if we have to prepare RecordBlob
	for ( i = 0 ; i < DBInfo->TableHeader->FieldNum ; i ++ )
	{
		if ( new_data[i].len < 0 )
		{
			if ( !PrvReadRecord( DBInfo, record_id ) ) 
				return FALSE;
			else
				break;
		}
	}

	PrvFillRecordBlob( DBInfo, new_data );
	if ( !PrvWriteRecord( DBInfo, record_id ) ) return FALSE;

#ifdef WIN32
	PrvCheckTableRint32egrity( table_id );
#endif


	return TRUE;
}

// field retrieval 
bool DB_GetFieldDataByName(HTABLE table_id, HRECORD record_id, char * field_name, char * buffer, rint32 buf_len)
{
	rint32 FieldIndex;

	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return FALSE;

	FieldIndex = PrvGetFieldIndexByName( DBInfo, field_name );
	return DB_GetFieldDataByIndex( table_id, record_id, FieldIndex, buffer, buf_len );
}

rint32 DB_GetFieldLenByName(HTABLE table_id, HRECORD record_id, char * field_name)
{
	rint32 index;
	
	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return -1;

	index = PrvGetFieldIndexByName( DBInfo, field_name );
	return DB_GetFieldLenByIndex( table_id, record_id, index );
}

bool DB_GetFieldDataByIndex(HTABLE table_id, HRECORD record_id, rint32 field_index, char * buffer, rint32 buf_len)
{
	bool Result;
	rint32	 FieldOffset;
	rint32	 CopySize;

	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return FALSE;

	DBASSERT( DBInfo->FieldTypeInfo[field_index].type == DBS_CHAR || DBInfo->FieldTypeInfo[field_index].type == DBS_BIN );
	if ( DBInfo->FieldTypeInfo[field_index].type != DBS_CHAR && DBInfo->FieldTypeInfo[field_index].type != DBS_BIN )
	{
		g_error_code = DBERR_FIELD_TYPE_INCORRECT;
		return FALSE;
	}

	// Fetch the record binary chunk from file system to DBInfo->RecordBlob.RecordChunk
	Result = PrvReadRecord( DBInfo, record_id );	
	if ( Result )
	{
		FieldOffset = PrvGetFieldOffset( DBInfo, NULL, field_index );
		if ( FieldOffset < 0 ) return FALSE;

		CopySize = ( DBInfo->FieldTypeInfo[field_index].max_len < buf_len ? DBInfo->FieldTypeInfo[field_index].max_len : buf_len );
		AS_MemCpy( buffer, ((unsigned char *)DBInfo->RecordBlob.RecordChunk + FieldOffset), CopySize );
	}
	return Result;
}

rint32 DB_GetFieldLenByIndex(HTABLE table_id, HRECORD record_id, rint32 field_index)
{
	// Mainly to find out the string length;
	bool Result;
	rint32 nFieldLen = 0;
	rint32	FieldOffset;

	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return -1;	

	if ( field_index < 0 || field_index >= DBInfo->TableHeader->FieldNum ) return -1;

	if ( DBInfo->FieldTypeInfo[field_index].type == DBS_CHAR )
	{
		Result = PrvReadRecord( DBInfo, record_id );
		if ( !Result ) return -1;
		
		FieldOffset = PrvGetFieldOffset( DBInfo, NULL, field_index );
		nFieldLen = AS_StrLen((char *)((unsigned char *)(DBInfo->RecordBlob.RecordChunk) + FieldOffset));
	}
	else
	{
		nFieldLen = DBInfo->FieldTypeInfo[field_index].max_len;
	}
	return nFieldLen;
}

rint32 DB_GetTableRecordNumber(HTABLE table_id)
{
	rx_db_header_type	*DBHeader = PrvGetDBHeader( table_id );
	
	if ( DBHeader ) return DBHeader->RecordNum;
	else return -1;
}	

HVIEW DB_CreateView(HTABLE table_id, CHECKPROC check_proc)
{
	
	HVIEW			ViewHandle;

	ViewHandle = PrvGetCleanViewHandle();

	if ( PrvCreateView( table_id, check_proc, NULL, ViewHandle ) )
		return ViewHandle;
	else
		return INVALID_VIEW_ID;
}

void DB_CloseView(HVIEW view_id)
{
	PrvEraseView( view_id );
}

void DB_SortView(HVIEW view_id, COMPPROC compare_proc)
{
	HRECORD	*TempBuffer;
	rx_db_view_type	*view;
	rint32 length;

	if ( compare_proc ) 
	{
		view = PrvGetViewChunk( view_id );
		DBASSERT( view );

		view->CompProc = compare_proc;
		
		length = DB_GetViewRecordNumber(view_id);
		TempBuffer = AS_MALLOC( sizeof( HRECORD ) * ( length ) );

		PrvMergeSort( view->RecordNoArray, 0, length - 1, view->TableHandle, compare_proc, TempBuffer );
		MemFree( TempBuffer );
	}
	return;
}

rint32 DB_GetViewRecordNumber(HVIEW view_id)
{
	rx_db_view_type *View;
	View = PrvGetViewChunk( view_id );
	
	if ( !View ) return -1;
	
	return View->NumRecord;
}

void DB_SetFirstRecord(HVIEW view_id)
{
	rx_db_view_type *View = PrvGetViewChunk( view_id );
	if( View ) View->SeekPosition = 0;
}

void DB_SetRecordPos(HVIEW view_id, rint32 pos)
{
	rx_db_view_type *View;
	View = PrvGetViewChunk( view_id );
	
	if ( View ) 
	{
		if ( pos >=0 && pos < View->NumRecord )
			View->SeekPosition = pos;
	}
}

rint32 DB_GetRecordPos(HVIEW view_id)
{
	rx_db_view_type *View = PrvGetViewChunk( view_id );
	if ( !View ) return INVALID_RECORD_ID;

	return View->SeekPosition;
}

HRECORD DB_GetPrevRecord(HVIEW view_id)
{
	HRECORD	RecordNo;
	rx_db_view_type *View = PrvGetViewChunk( view_id );

	if ( !View ) return INVALID_RECORD_ID;
	
	if ( View->SeekPosition < 0 || View->SeekPosition >= View->NumRecord ) 
		return INVALID_RECORD_ID;

	RecordNo = View->RecordNoArray[View->SeekPosition];
	View->SeekPosition--;

	return RecordNo;
}
HRECORD DB_GetNextRecord(HVIEW view_id)
{
	HRECORD	RecordNo;
	rx_db_view_type *View = PrvGetViewChunk( view_id );

	if ( !View ) return INVALID_RECORD_ID;
	
	if ( View->SeekPosition < 0 || View->SeekPosition >= View->NumRecord ) 
		return INVALID_RECORD_ID;

	RecordNo = View->RecordNoArray[View->SeekPosition];
	View->SeekPosition++;

	return RecordNo;
}

void DB_RefreshView(HVIEW view_id)
{
	HTABLE		table_id;
	CHECKPROC	check_proc;
	COMPPROC	comp_proc;
	rx_db_view_type	*view;

	view = PrvGetViewChunk( view_id );
	DBASSERT( view );

	if ( view )
	{
		// Copy prev info
		table_id	= view->TableHandle;
		check_proc	= view->CheckProc;
		comp_proc	= view->CompProc;

		PrvEraseView( view_id );
		PrvCreateView( table_id, check_proc, comp_proc, view_id );
		DB_SortView( view_id, comp_proc );
	}
}

/************************************************************************
 * Extended APIs on Rexy New DB
 ************************************************************************/
ruint32 DB_GetFieldValueByIndex(HTABLE table_id, HRECORD record_id, rint32 field_index)
{
	ruint32 value = 0;
	rint32	 FieldOffset;

	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return FALSE;

	DBASSERT( DBInfo->FieldTypeInfo[field_index].type == DBS_SHORT || DBInfo->FieldTypeInfo[field_index].type == DBS_LONG );
	if ( DBInfo->FieldTypeInfo[field_index].type != DBS_SHORT && DBInfo->FieldTypeInfo[field_index].type != DBS_LONG )
	{
		g_error_code = DBERR_FIELD_TYPE_INCORRECT;
		return FALSE;
	}

	// Fetch the record binary chunk from file system to DBInfo->RecordBlob.RecordChunk
	if ( PrvReadRecord( DBInfo, record_id ) )
	{
		FieldOffset = PrvGetFieldOffset( DBInfo, NULL, field_index );
		if ( FieldOffset < 0 ) return 0;
		
		switch ( DBInfo->FieldTypeInfo[field_index].type )
		{
		case DBS_SHORT:
			value = *(DBS_SHORT_TYPE *)((unsigned char *)DBInfo->RecordBlob.RecordChunk + FieldOffset);
			break;
		case DBS_LONG:
			value = *(DBS_LONG_TYPE *)((unsigned char *)DBInfo->RecordBlob.RecordChunk + FieldOffset);
			break;
		}
	}
	return value;
}
ruint32 DB_GetFieldValueByName(HTABLE table_id, HRECORD record_id, char *field_name )
{
	rint32 FieldIndex;
	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return FALSE;

	FieldIndex = PrvGetFieldIndexByName( DBInfo, field_name );
	return DB_GetFieldValueByIndex( table_id, record_id, FieldIndex );
}
/** 
 * @brief			lock database table, you cannot open locked database
 * @return			TRUE on success					
 */
bool DB_SetLockStatus( HTABLE table_id, bool bLock )
{
	rx_db_info_type	*DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return FALSE;

	DBInfo->Exclusive = bLock;
	return TRUE;
}
bool DB_Sync( HTABLE table_id )
{
	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return FALSE;

	PrvSyncHeaderToFile( DBInfo );

	// DOLATER
	// Please insert any file system cache API here,
	// if exists.
	// 
	// sync();

	return TRUE;
}
rint32 DB_Defrag( HTABLE table_id )
{
	rint32 start;		// increasing slot
	rint32 end;		// decreasing slot
	unsigned char *BitmapTable;

	rx_db_info_type *DBInfo = PrvGetInfoChunk( table_id );
	if ( !DBInfo ) return DBERR_NONEXISTING_TABLE;

	BitmapTable = DBInfo->TableHeader->BitmapTable;
	
	start = PrvGetEmptySlot( BitmapTable );
	end	  = PrvGetLastSlot( BitmapTable );

	while ( start < end )
	{
		PrvReadRecord( DBInfo, end );
		PrvSetRecordSlotStatus( BitmapTable, end, FALSE );

		PrvWriteRecord( DBInfo, start );

		start = PrvGetEmptySlot( BitmapTable );
		end	  = PrvGetLastSlot( BitmapTable );
	}
	

#ifdef WIN32
	PrvCheckTableRint32egrity( table_id );
#endif

	return 0;		
}
// -----------------------------

/************************************************************************/
/* Rint32ernal Functions                                                   */
/************************************************************************/


// Caller should free this returned porint32er as soon as they use
static char *PrvCatDBFileExt( char *TableName )
{
	char *DBFileName;
	rint32	 TableNameLen;

	TableNameLen = AS_StrLen( TableName );
	if ( !TableNameLen ) return NULL;

	DBFileName = AS_MALLOC( TableNameLen + AS_StrLen( REXY_NEW_DB_EXT ) + 1 );	//include null
	AS_StrCpy( DBFileName, TableName );
	AS_StrCat( DBFileName, ".jdb" );

	return DBFileName;
}
static HTABLE  PrvOpenTable( char *TableName )
{
	char *DBFileName;
	rx_db_info_type *DBInfo;
	rx_db_info_type *NextInfo;
	rx_db_header_type *TableHeader;
	HTABLE	TableHandle;
	rint32		FileHandle;

	// Check already opened
	DBInfo = g_db_info_start;

	while ( DBInfo )
	{
		if ( !AS_StrCmp( DBInfo->TableName, TableName) )
		{
			// we've found already opened one
			if ( DBInfo->Exclusive ) 
			{
				g_error_code = DBERR_LOCKED;
				return INVALID_TABLE_ID;
			}
			else
			{
				DBInfo->RefCount++;
				return DBInfo->TableHandle;
			}
		}
		NextInfo = DBInfo->NextInfo;
		DBInfo = NextInfo;
	}
	
	// Open DB File from file system
	DBFileName = PrvCatDBFileExt( TableName );
	FileHandle = FsOpenFile( DBFileName );
	MemFree( DBFileName );

	if ( FileHandle <= FILE_ERROR )
	{
		g_error_code = DBERR_OPEN_FAILURE;
		return INVALID_TABLE_ID;
	}

	// Allocate Database Header on memory
	TableHandle = PrvGetCleanTableHandle();
	TableHeader = AS_MALLOC( sizeof(rx_db_header_type) );
	AS_ReadFile( FileHandle, (char *)TableHeader, sizeof ( rx_db_header_type ) );
	
	if ( AS_StrCmp( TableHeader->HeaderString, DB_HEADER_STRING ) || TableHeader->Version != DB_VERSION )
	{
		g_error_code = DBERR_INVALID_FILE;
		MemFree( TableHeader );
		AS_CloseFile( FileHandle );
		return INVALID_TABLE_ID;
	}

	DBInfo = PrvAddInfoChunk( TableHandle );

	// Fill DBInfo
	DBInfo->TableHeader = TableHeader;
	
	DBInfo->FieldTypeInfo = AS_MALLOC( sizeof( field_type ) * TableHeader->FieldNum );
	AS_ReadFile( FileHandle, (char *)DBInfo->FieldTypeInfo, sizeof( field_type ) * TableHeader->FieldNum );

	DBInfo->FileHandle = FileHandle;
	DBInfo->RefCount++;
	DBInfo->TableName = AS_MALLOC( AS_StrLen(TableName) + 1); 	AS_StrCpy( DBInfo->TableName, TableName );

	// Make Record Blob
	DBInfo->RecSize	= PrvCalcRecordSize( DBInfo );
	DBInfo->RecordBlob.RecordID = INVALID_RECORD_ID;		// since we didn't read any record
	DBInfo->RecordBlob.bDirty	  = TRUE;					// we have to read again even if the specified record id matches
	DBInfo->RecordBlob.RecordChunk = AS_MALLOC( DBInfo->RecSize );

	return TableHandle;
}
static HTABLE PrvCreateTable( char *TableName, field_type * schema, rint32 field_num )
{
	HTABLE				TableHandle;
	rx_db_info_type		*DBInfo;
	rx_db_header_type	*TableHeader;
	char				*DBFileName;
	rint32					i;

	// Get a clean table handle
	TableHandle = PrvGetCleanTableHandle();

	// Make Database Header
	TableHeader = AS_MALLOC( sizeof(rx_db_header_type) );
	AS_MemSet( TableHeader, 0, sizeof(rx_db_header_type) );

	AS_StrCpy( TableHeader->HeaderString, DB_HEADER_STRING );
	TableHeader->Version = DB_VERSION;
	TableHeader->FieldNum = field_num;

	// Register open db info after creation
	DBInfo = PrvAddInfoChunk( TableHandle );
	DBInfo->TableName = AS_MALLOC( AS_StrLen(TableName) + 1);
	AS_StrCpy( DBInfo->TableName, TableName );
	DBInfo->RefCount = 1;
	DBInfo->Exclusive = FALSE;
	DBInfo->TableHeader = TableHeader;

	// Make Field Info Block
	DBInfo->FieldTypeInfo = AS_MALLOC( sizeof( field_type ) * field_num );
	AS_MemCpy( DBInfo->FieldTypeInfo, schema, sizeof( field_type ) * field_num );
	
	// Fix basic field length info;
	for ( i = 0; i< field_num; i++ )
	{
		switch ( DBInfo->FieldTypeInfo[i].type )
		{
		case DBS_SHORT:
			DBInfo->FieldTypeInfo[i].max_len = sizeof( DBS_SHORT_TYPE );
			break;
		case DBS_LONG:
			DBInfo->FieldTypeInfo[i].max_len = sizeof( DBS_LONG_TYPE );
			break;
		}
	}

	// Make Record Blob
	DBInfo->RecSize	= PrvCalcRecordSize( DBInfo );
	DBInfo->RecordBlob.RecordID = INVALID_RECORD_ID;		// since we didn't read any record
	DBInfo->RecordBlob.bDirty	  = TRUE;					// we have to read again even if the specified record id matches
	DBInfo->RecordBlob.RecordChunk = AS_MALLOC( DBInfo->RecSize );

	// Create Database File
	DBFileName = PrvCatDBFileExt( TableName );
	DBInfo->FileHandle = AS_CreateFile( DBFileName );
	MemFree( DBFileName );

	if ( DBInfo->FileHandle <= FILE_ERROR )
	{
		PrvDeleteInfoChunk( TableHandle );
		g_error_code = DBERR_CREATE_FAILURE;
		return INVALID_TABLE_ID;
	}

	// Write Header
	AS_WriteFile( DBInfo->FileHandle, (char *)TableHeader, sizeof( rx_db_header_type ) );

	// Write Field Info
	AS_WriteFile( DBInfo->FileHandle, (char *)DBInfo->FieldTypeInfo, sizeof( field_type ) * TableHeader->FieldNum );

	return TableHandle;
}
static bool PrvCreateView( HTABLE table_id, CHECKPROC check_proc, COMPPROC comp_proc, HVIEW view_id )
{
	rint32				i;
	rint32				FilteredRecordNum;
	rint32				LiveRecordNum;
	rint32				TotalRecordNum;
	rx_db_view_type	*CurViewChunk;
	rx_db_info_type	*DBInfo;

	DBInfo = PrvGetInfoChunk( table_id );
	DBASSERT( DBInfo );

	// Allocate Memory
	TotalRecordNum	= DB_GetTableRecordNumber( table_id );
	CurViewChunk	= AS_MALLOC( sizeof( rx_db_view_type ) );
	AS_MemSet( CurViewChunk, 0, sizeof( rx_db_view_type ) );

	if ( TotalRecordNum ) 
	{
		CurViewChunk->RecordNoArray = AS_MALLOC( sizeof(HRECORD) * TotalRecordNum );
		CurViewChunk->NumRecord = TotalRecordNum;
	}

	CurViewChunk->PrevView = g_db_view_end;
	CurViewChunk->NextView = NULL;
	
	// Register to linked list chain
	if ( !g_db_view_end )	// This is the first chunk
	{	
		g_db_view_start = CurViewChunk;
	}
	else
	{
		g_db_view_end->NextView = CurViewChunk;
	}
	g_db_view_end = CurViewChunk;

	// Fill contents;
	CurViewChunk->TableHandle = table_id;
	CurViewChunk->ViewHandle = view_id;
	CurViewChunk->CheckProc = check_proc;
	CurViewChunk->CompProc	= comp_proc;

	FilteredRecordNum =0;
	LiveRecordNum = 0;

	for ( i = 0; i < MAX_DB_RECORD ; i++ )
	{
		if ( PrvGetRecordSlotStatus( DBInfo->TableHeader->BitmapTable, i ))
		{
			if ( check_proc != NULL )
			{
				if ( check_proc(table_id, i) )
				{
					CurViewChunk->RecordNoArray[FilteredRecordNum] = i;
					FilteredRecordNum++;
				}
			}
			else
			{
				CurViewChunk->RecordNoArray[LiveRecordNum] = i;
				LiveRecordNum++;
			}
		}
		if ( LiveRecordNum == TotalRecordNum ) break;
	}

	// Refresh filtered record number in the view
	if ( check_proc )
	{
		CurViewChunk->NumRecord = FilteredRecordNum;
	}

	g_view_count++;
	return TRUE;
}
static void PrvEraseView( HVIEW view_id )
{
	rx_db_view_type *TargetViewChunk;
	rx_db_view_type	*PrevChunk;
	rx_db_view_type *NextChunk;

	TargetViewChunk = PrvGetViewChunk( view_id );
	DBASSERT( TargetViewChunk );

	PrevChunk = TargetViewChunk->PrevView;
	NextChunk = TargetViewChunk->NextView;

	if ( PrevChunk ) PrevChunk->NextView = NextChunk;
	if ( NextChunk ) NextChunk->PrevView = PrevChunk;

	if ( PrevChunk )
	{
		if ( !NextChunk ) 	// last one
		{
			g_db_view_end = PrevChunk;
		}
	}
	else
	{
		if ( NextChunk )	// it was the first one
		{
			g_db_view_start = NextChunk;
		}
		else
		{
			g_db_view_start = 0;
			g_db_view_end = 0;
		}
	}

	if ( TargetViewChunk->RecordNoArray ) MemFree( TargetViewChunk->RecordNoArray );
	MemFree( TargetViewChunk );

	g_view_count--;
}
static rx_db_info_type *PrvGetInfoChunk( HTABLE TableHandle )
{
	bool	bFound = FALSE;
	rx_db_info_type	*DBInfo;
	rx_db_info_type	*NextChunk;

	DBInfo = g_db_info_start;
	
	while ( DBInfo )
	{
		if ( DBInfo->TableHandle == TableHandle )
		{
			bFound = TRUE;
			break;
		}
		NextChunk = DBInfo->NextInfo;
		DBInfo = NextChunk;
	}

	if ( !bFound )
	{
		g_error_code = DBERR_NONEXISTING_TABLE;
		DBInfo = 0;
	}
	return DBInfo;
}

/** 
 * @brief	Get a randomized clean table handle
 * @ifdef history
 *		Joshua	Created	14-Nov-2007
 * @endif
 */
static HTABLE PrvGetCleanTableHandle( void )
{
	HTABLE	TableHandle;
	rx_db_info_type *DBInfo=NULL;

	do 
	{
		TableHandle = rand();
		if ( TableHandle <= 0 ) continue;		

		DBInfo = PrvGetInfoChunk( TableHandle );
	} while( DBInfo );

	return TableHandle;
}
static HVIEW PrvGetCleanViewHandle( void )
{
	HTABLE	ViewHandle;
	rx_db_view_type *ViewChunk=NULL;

	do 
	{
		ViewHandle = rand();
		if ( ViewHandle <= 0 ) continue;		

		ViewChunk = PrvGetViewChunk( ViewHandle );
	} while( ViewChunk );

	return ViewHandle;
}
static rx_db_header_type *PrvGetDBHeader( HTABLE TableHandle )
{
	rx_db_header_type *DBHeader = 0;
	rx_db_info_type	*DBInfo;

	DBInfo = PrvGetInfoChunk( TableHandle );

	if ( DBInfo ) 
	{
		DBHeader = DBInfo->TableHeader;
	}
	return DBHeader;
}

static rint32 PrvCalcRecordSize( rx_db_info_type *DBInfo )
{
	rint32 RecSize = 0;
	rint32	i;

	rx_db_header_type *Table;
	Table = DBInfo->TableHeader;

	for ( i=0; i< Table->FieldNum; i++ )
	{
		RecSize += DBInfo->FieldTypeInfo[i].max_len;
	}

	return RecSize;
}
static rx_db_info_type *PrvAddInfoChunk( HTABLE TableHandle )
{
	rx_db_info_type	*CurDBInfoChunk;

	CurDBInfoChunk = AS_MALLOC( sizeof(rx_db_info_type) );
	AS_MemSet( CurDBInfoChunk, 0, sizeof( rx_db_info_type ) );
	
	CurDBInfoChunk->TableHandle = TableHandle;
	CurDBInfoChunk->PrevInfo = g_db_info_end;
	CurDBInfoChunk->NextInfo = NULL;
	
	if ( !g_db_info_end )	// This is the first chunk
	{	
		g_db_info_start = CurDBInfoChunk;
	}
	else
	{
		g_db_info_end->NextInfo = CurDBInfoChunk;
	}
	g_db_info_end = CurDBInfoChunk;
	g_db_count++;

	return CurDBInfoChunk;
}
static bool PrvDeleteInfoChunk( HTABLE TableHandle )
{
	rx_db_info_type *TargetInfoChunk;
	rx_db_info_type	*PrevChunk;
	rx_db_info_type *NextChunk;

	TargetInfoChunk = PrvGetInfoChunk( TableHandle );
	DBASSERT( TargetInfoChunk );

	PrevChunk = TargetInfoChunk->PrevInfo;
	NextChunk = TargetInfoChunk->NextInfo;

	if ( PrevChunk ) PrevChunk->NextInfo = NextChunk;
	if ( NextChunk ) NextChunk->PrevInfo = PrevChunk;

	if ( PrevChunk )
	{
		if ( !NextChunk ) 	// last one
		{
			g_db_info_end = PrevChunk;
		}
	}
	else
	{
		if ( NextChunk )	// it was the first one
		{
			g_db_info_start = NextChunk;
		}
		else
		{
			g_db_info_start = 0;
			g_db_info_end = 0;
		}
	}
	
	// Free the memory of this chunk
	MemFree(TargetInfoChunk->FieldTypeInfo);
	MemFree(TargetInfoChunk->RecordBlob.RecordChunk);
	MemFree(TargetInfoChunk->TableName);
	MemFree(TargetInfoChunk->TableHeader);
	MemFree(TargetInfoChunk);

	g_db_count--;
	return TRUE;	// on success
}

static rx_db_view_type *PrvGetViewChunk( HVIEW ViewHandle )
{
	bool	bFound = FALSE;
	rx_db_view_type	*ViewChunk;
	rx_db_view_type	*NextChunk;

	ViewChunk = g_db_view_start;
	
	while ( ViewChunk )
	{
		if ( ViewChunk->ViewHandle == ViewHandle )
		{
			bFound = TRUE;
			break;
		}
		NextChunk = ViewChunk->NextView;
		ViewChunk = NextChunk;
	}

	if ( !bFound ) ViewChunk = 0;
	return ViewChunk;
}

static bool PrvGetRecordSlotStatus( unsigned char *BitmapTable, rint32 nPos )
{
	rint32 nBitmapPos, nSlotPos;
	unsigned char TempBit;

	DBASSERT( nPos >= 0 && nPos < MAX_DB_RECORD );

	// to get faster speed
	nBitmapPos = nPos >> 3;	
	nSlotPos = nPos % 8;
	TempBit	 = ( unsigned char )( 0x80 >> nSlotPos );

	return BitmapTable[nBitmapPos] & TempBit;

}

static void PrvSetRecordSlotStatus( unsigned char *BitmapTable, rint32 nPos, bool bValue )
{
	rint32 nBitmapPos, nSlotPos;
	unsigned char	DesignatedBit;

	// to get faster speed
	nBitmapPos = nPos >> 3;
	nSlotPos = nPos % 8 ;
	
	//지정된 Slot 상태 변경하기
	DesignatedBit = (unsigned char)( 0x80 >> nSlotPos );

	if( bValue )
		BitmapTable[nBitmapPos] = (unsigned char)(BitmapTable[nBitmapPos] | DesignatedBit );	
	else
		BitmapTable[nBitmapPos] = (unsigned char)(BitmapTable[nBitmapPos] & (~DesignatedBit));	
}

static bool PrvWriteRecord( rx_db_info_type *DBInfo, HRECORD RecordID )
{
	ruint32 Offset;
	rint32			  WrittenBytes;
	Offset		= PrvGetRecordFileOffset( DBInfo, RecordID );

	// Write
	FsLseekFile( DBInfo->FileHandle, Offset, SEEK_SET );
	WrittenBytes = FsWriteFile( DBInfo->FileHandle, DBInfo->RecordBlob.RecordChunk, DBInfo->RecSize );

	if ( WrittenBytes == DBInfo->RecSize )
	{
		// Add New info on Bitmap & Offset Table
		PrvSetRecordSlotStatus( DBInfo->TableHeader->BitmapTable, RecordID, TRUE );


		// Set Cache Info
		DBInfo->RecordBlob.RecordID = RecordID;
		DBInfo->RecordBlob.bDirty = FALSE;

		return TRUE;	// On Success
	}
	else
	{
		g_error_code = DBERR_FILE_SYSTEM_ERROR;
		return FALSE;
	}
}

static bool PrvReadRecord( rx_db_info_type *DBInfo, HRECORD RecordID )
{
	rint32	Offset;

	if ( !PrvCheckRecordRint32egrity( DBInfo->TableHeader, RecordID ) ) 
	{
		g_error_code = DBERR_DELETED_RECORD;
		return FALSE;
	}

	// Use Cache
	if ( DBInfo->RecordBlob.RecordID != RecordID || DBInfo->RecordBlob.bDirty )
	{	
		Offset	= PrvGetRecordFileOffset( DBInfo, RecordID );
		AS_LseekFile( DBInfo->FileHandle, Offset, SEEK_SET );

		AS_ReadFile( DBInfo->FileHandle, DBInfo->RecordBlob.RecordChunk, DBInfo->RecSize );
		DBInfo->RecordBlob.bDirty = FALSE;
		DBInfo->RecordBlob.RecordID = RecordID;
	}
	return TRUE;	// on success
}
static ruint32 PrvGetRecordFileOffset( rx_db_info_type *DBInfo, rint32 SlotNo )
{
	ruint32	BaseOffset;
	ruint32	Offset;

	BaseOffset	= sizeof ( rx_db_header_type ) + sizeof( field_type ) * DBInfo->TableHeader->FieldNum;
	Offset		= BaseOffset + DBInfo->RecSize * SlotNo;

	return Offset;
}
static rint32 PrvGetFieldIndexByName( rx_db_info_type *DBInfo, char *FieldName )
{
	rint32 i;
	rint32 index = -1;

	for ( i = 0; i < DBInfo->TableHeader->FieldNum ; i++ )
	{
		if ( AS_StrCmp( FieldName, DBInfo->FieldTypeInfo[i].name ) == 0 ) 
		{
			index = i;
			break;
		}
		
	}
	return index;
}
static rint32 PrvGetFieldOffset( rx_db_info_type *DBInfo, char *FieldName, rint32 FieldIndex )
{
	rint32		i;
	bool	bFound = FALSE;
	rint32		FieldOffset = 0;

	if ( FieldName )
	{
		for ( i = 0; i < DBInfo->TableHeader->FieldNum ; i++ )
		{
			if ( AS_StrCmp( FieldName, DBInfo->FieldTypeInfo[i].name ) == 0 ) 
			{
				bFound = TRUE;
				break;
			}
			FieldOffset += DBInfo->FieldTypeInfo[i].max_len;
		}
	}
	else
	{
		// Seek to the specified field_index
		if ( FieldIndex <= DBInfo->TableHeader->FieldNum )
		{
			bFound = TRUE;

			for ( i = 0; i < FieldIndex ; i++ )
			{
				FieldOffset += DBInfo->FieldTypeInfo[i].max_len;
			}
		}
	}

	if ( !bFound ) FieldOffset = -1;
	return FieldOffset;
}
static rint32 PrvGetEmptySlot( unsigned char *BitmapTable )
{
	rint32	nStartByte;
	rint32 nSlotPos;
	rint32	nBytePos;
	bool bStatus = FALSE;

	// To Get Faster Speed
	for ( nBytePos = 0; nBytePos < MAX_DB_RECORD / 8 ; nBytePos ++ )
	{
		if ( BitmapTable[nBytePos] != 0xFF ) break;
	}

	if ( nBytePos == MAX_DB_RECORD / 8 )	// No slot available !
	{
		return -1;
	}

	nStartByte = nBytePos << 3;

	for ( nSlotPos = nStartByte; nSlotPos < nStartByte + 8; nSlotPos++)
	{
		bStatus = PrvGetRecordSlotStatus( BitmapTable, nSlotPos );
		if(!bStatus) break;		
	}

	DBASSERT( !bStatus );
	return nSlotPos;	
}

static rint32 PrvGetLastSlot( unsigned char *BitmapTable )
{
	rint32		i;
	rint32		nSlotPos;
	bool	bStatus;
	
	// To get faster speed
	for ( i = MAX_DB_RECORD / 8 - 1 ; i >= 0 ; i-- )
	{
		if ( BitmapTable[i] != 0x00 ) 
		{
			for ( nSlotPos = (i+1) * 8 -1; nSlotPos >= 0 ; nSlotPos-- )
			{
				if ( PrvGetRecordSlotStatus(BitmapTable, nSlotPos) )
					return nSlotPos;
			}
		}
	}

	// There was no active slot
	return -1;		
}
static bool PrvCheckRecordRint32egrity( rx_db_header_type *TableHeader, HRECORD record_id )
{
	bool	result;
	if ( record_id < 0 || record_id >= MAX_DB_RECORD ) 
	{
		g_error_code = DBERR_INVALID_RECORD_ID;
		return FALSE;
	}
	result = PrvGetRecordSlotStatus( TableHeader->BitmapTable, record_id );

	if ( !result ) g_error_code = DBERR_DELETED_RECORD;

	return result;
}
static bool PrvCheckTableRint32egrity( HTABLE table_id )
{
	rx_db_header_type *DBHeader = 0;
	rint32 nRecordNum = 0;
	rint32 nSlotOnNum = 0;
	bool bStatus;
	rint32 i;

	rx_db_info_type	*DBInfo;
	bool bReturnValue;
	rint32	 LiveRecordNum;

	//1. Header에 저장된 Record 갯수
	DBHeader = PrvGetDBHeader(table_id);
	DBASSERT( DBHeader );

	nRecordNum = DBHeader->RecordNum;


	//2. 켜져있는 Slot 갯수
	for(i = 0;  i < MAX_DB_RECORD; i++)
	{
		bStatus = PrvGetRecordSlotStatus(DBHeader->BitmapTable, i);
		if ( bStatus ) nSlotOnNum++;		
	}

	if( nRecordNum == nSlotOnNum ) bReturnValue =  TRUE;
	else bReturnValue =  FALSE;

	DBASSERT( bReturnValue );

	//3. 데이타 체크
	DBInfo = PrvGetInfoChunk( table_id );
	DBASSERT( DBInfo );

	LiveRecordNum = 0;
	i = 0;

	if ( nRecordNum )
	{
		for ( i = 0; i < MAX_DB_RECORD ; i++ )
		{
			if ( PrvGetRecordSlotStatus( DBInfo->TableHeader->BitmapTable, i ))
			{
				LiveRecordNum++;
				// We have to escape before we reach the limit
				if ( LiveRecordNum == nRecordNum ) break;
			}
		}
	}

	DBASSERT( i != MAX_DB_RECORD );
	DBASSERT( bReturnValue );

	return bReturnValue;
	
}
static bool	PrvFillRecordBlob( rx_db_info_type *DBInfo, field_data *record )
{
	rint32 i, FieldLen;
	rint32 offset	= 0;
	unsigned char *RecordChunk = (unsigned char *)DBInfo->RecordBlob.RecordChunk;
	rx_db_header_type	*Table = DBInfo->TableHeader;

	for ( i=0; i< Table->FieldNum; i++ )
	{
		if ( record[i].len > 0 )	// ignore negative length fields
		{
			switch ( DBInfo->FieldTypeInfo[i].type )
			{
			case DBS_CHAR:
			case DBS_BIN:
				DBASSERT( record[i].len <= DBInfo->FieldTypeInfo[i].max_len );
				FieldLen = ( record[i].len < DBInfo->FieldTypeInfo[i].max_len ? record[i].len : DBInfo->FieldTypeInfo[i].max_len );
				AS_MemCpy( RecordChunk + offset, record[i].data, FieldLen );
				break;
			case DBS_SHORT:
				*(DBS_SHORT_TYPE *)( RecordChunk + offset ) = ( DBS_SHORT_TYPE )record[i].data;
				break;
			case DBS_LONG: 
				*(DBS_LONG_TYPE *)( RecordChunk + offset ) = ( DBS_LONG_TYPE  )record[i].data;
				break;
			}
		}
		offset += DBInfo->FieldTypeInfo[i].max_len;
	}
	DBASSERT( offset == DBInfo->RecSize );

	return TRUE;
}
static void PrvSyncHeaderToFile( rx_db_info_type *DBInfo )
{
	// Write only header to file
	AS_LseekFile( DBInfo->FileHandle, 0, SEEK_SET );
	AS_WriteFile( DBInfo->FileHandle, (char *)DBInfo->TableHeader, sizeof( rx_db_header_type) );
}

static void PrvMergeSort(HRECORD * data_array, rint32 start, rint32 end, HTABLE table_id, COMPPROC comp_proc, HRECORD *TempBuffer )
{
	rint32 i, j, k;
    rint32 middle;

	if ( end <= start )
	{
		return;
	}

	if ( end == start + 1 )
	{
		if ( comp_proc(table_id, data_array[start], data_array[end]) > 0 )
		{
			HRECORD swap;
			swap = data_array[start];
			data_array[start] = data_array[end];
			data_array[end] = swap;
		}

		return;
	}

	middle = (end + start) / 2;
	PrvMergeSort(data_array, start, middle, table_id, comp_proc, TempBuffer );
	PrvMergeSort(data_array, middle + 1, end, table_id, comp_proc, TempBuffer );

	i = start;
	j = middle + 1;
	k = start;

	while( (i <= middle) && (j <= end) )
	{
		if ( comp_proc(table_id, data_array[i], data_array[j]) <= 0 )
		{
			TempBuffer[k]= data_array[i];
			++k;
			++i;
		}
		else
		{
			TempBuffer[k] = data_array[j];
			++k;
			++j;
		}
	}

	if ( i <= middle )
	{
		for ( ; i <= middle; ++i )
		{
			TempBuffer[k] = data_array[i];
			++k;
		}
	}
	else
	{
		if ( j <= end )
		{
			for ( ; j <= end; ++j )
			{
				TempBuffer[k] = data_array[j];
				++k;
			}
		}
	}

	AS_MemCpy(&data_array[start], &TempBuffer[start], (end - start + 1) * sizeof( HRECORD ));
	return;
}

