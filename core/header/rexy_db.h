#ifndef REXY_DB_JP_H
#define REXY_DB_JP_H

/** @file */

/** @defgroup database database management API
 * @ingroup component
 * @{
 */

//DB schema
/** Record field type : character */
#define DBS_CHAR	0
/** Record field type : short ( 2 byte integer ) */
#define DBS_SHORT	1
/** Record field type : long ( 4 byte integer ) */
#define DBS_LONG	2
/** Record field type : binary  */
#define DBS_BIN		3

// Define char to DBS_CHAR to prevent the legacy code compilication error
//
#define CHAR        DBS_CHAR

/** Record Field Type Definition Structure */
typedef struct field_type { 
   char name[32];	///< field name
   rint32 type ; 		///< field type :  DBS_CHAR, DBS_SHORT, DBS_LONG
   rint32 max_len;		///< maximum length of the field in case of DBS_CHAR ( will be depreciated except DBS_CHAR )
} field_type;

/** Record Field Data Definition Structure */
typedef struct field_data { 
   rint32 len;	///< byte length of the data field. In case of string type(DBS_CHAR) 
            ///< the value of this field is the length of the field including 
            ///< NULL character at the end of the string. For example, if the 
            ///< data field contains the NULL-terminated string "ABC", the
            ///< value of this field should be 4. 

   void * data;  ///<  field to contain data
} field_data;

/** table id data type */
typedef rint32 HTABLE;

/** view id data type */
typedef rint32 HVIEW;

/** record id data type */
typedef ruint32 HRECORD;


// DB error code
/** DB error code : Another DB table with the same name already exists on the file system.*/
#define DBERR_DUPLICATE_TABLE		(-1)
/** DB error code : Failed to create a DB table on the file system. */
#define DBERR_CREATE_FAILURE		(-2)
/** DB error code : The file to read the DB table from doesn't exist on the file system. */
#define DBERR_NONEXISTING_TABLE		(-3)
/** DB error code : Tried to access an invalid or corrupted DB file, or not matching DB version */
#define DBERR_INVALID_FILE			(-4)
/** DB error code : Open the designated file error**/
#define DBERR_OPEN_FAILURE			(-5)
/** DB error code : The specified table cannot be open for it's locked */
#define DBERR_LOCKED				(-6)
/** DB error code : Field data type does not match */
#define DBERR_FIELD_TYPE_INCORRECT	(-7)	
/** DB error code : The specified record has been deleted already */
#define DBERR_DELETED_RECORD		(-8)
/** DB error code : There's no empty space in the table to add another record */
#define DBERR_RECORD_FULL			(-9)
/** DB error code : The given record id exceeded the last record id or it was negative */
#define DBERR_INVALID_RECORD_ID		(-10)
/** DB error code : File system error like read / write failure */
#define DBERR_FILE_SYSTEM_ERROR		(-11)

// invalid ids
/** Failed to create a DB table */
#define INVALID_TABLE_ID	0
/** Failed to create a view */
#define INVALID_VIEW_ID		0
/** Failed to create a record */
#define INVALID_RECORD_ID	(ruint32)(-1)

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

// db error code retrieval
/** DB_GetDBErrorCode returns error code set by DB_OpenTable
 *  In this DB module version, only DB_OpenTable function sets the error code which can 
 *  be retrieved by this functions. The returned error codes from this function are 
 *  DBERR_DUPLICATE_TABLE, DBERR_CREATE_FAILURE, DBERR_NONEXISTING_TABLE, DBERR_INVALID_FILE.
 *  @return  The error code set by the function which generates the most recent error. 
 */
rint32 DB_GetDBErrorCode (void);


//	table operations

/** Checks whether the table with the given name exists on the file system.
 * @param table_name	The name of the table to search on the file system
 * @return	If the table doesn't exist, this function returns FALSE. Returns TRUE if it exists. 			
 */
bool DB_FindTable(char * table_name);

/** Create a new table or opens a DB table already existing on the file system. 
 * If the schema parameter is NULL, it tries to open an existing DB table. 
 * If the parameter is not NULL, it tries to create a new DB table. 
 * If it finds an existing DB table with the same name during a DB table creation process, 
 * this functions stops execution and returns an error code, INVALID_TABLE_ID. 
 * If the whole process is finished successfully, the function returns a DB table id. 
 * In case of function failure, the more detailed cause of the error can be found by invoking 
 * DB_GetDBErrorCode()
 * @param table_name	the name of the table to create or open.
 * @param schema		an array containing the types of fields composing a record in the DB table. 
 *						This parameter is used only when the function tries to create a new DB table.
 *						Suppose that the record to save in a new DB table contains 3 string type fields, 
 *						whose names are "name", "address", "phone_number", and that the maximum lengths(including NULL character) 
 *						of the each field are 10, 50, 20 bytes. Then the schema parameter should be a 
 *						following array. <BR> { {"name", DBS_CHAR, 10}, {"address", DBS_CHAR, 50}, {"phone_number", DBS_CHAR, 20} } <BR>
 * @param field_num		the number of the field contained a record. This parameter is meaningful only in the process of a new DB table creation. In the aforementioned case, the value of "field_num" should be 3. 
 * @return INVALID_TABLE_ID or newly created table id. 
 */
HTABLE DB_OpenTable(char * table_name, field_type * schema, rint32 field_num);


/** Close a DB table. 
 * perations performed with the same table id would fail after the table is closed.
 * @param table_id	the id of the DB table to close. 
 */
void DB_CloseTable(HTABLE table_id);


/** Delete a DB table
 * All records contained in the table would be deleted.
 * @param table_id	the id of the DB table to delete on the file system. 
 */
void DB_DeleteTable(HTABLE table_id);

/** Returns the number of records contained in the DB table.
 * @param table_id	the id of the table to find the number of the records contained 
 * @return the number of records contained in the DB table. 
 */
rint32 DB_GetTableRecordNumber(HTABLE table_id);



//	record operations

/** Inserts a new record into the DB table. 
 * The order of the record fields in the "record" parameter should be same as that of the 
 * "schema" parameter used in DB_OpenTable function. If one of the field doesn't get any value
 * (as long as its type is DBS_CHAR), the data field of field_data structure should be set as "" 
 * and the length field as 1. For example, suppose that the record in this DB table is composed of 
 * three fields whose names are "name", "address", "phone number" , and this field order is used 
 * in the "schema" parameter of the DB_OpenTable function to define the schema of a DB table. 
 * If you should insert a new record which has a certain value in a name field("John") and 
 * a phone number field("3430-1234") but not in an address field, the "record" parameter given 
 * in DB_InsertRecord function should be a following array. 
 * { {5, "John"}, {1, ""}, {10, "3430-3644"} } The length of the first record field is set as 5 
 * because the string("John") lengthshould include a null character at the end of the string.  
 * 
 * @param table_id	the id of the DB table to insert the record into. 
 * @param record	the data of the record to be inserted. It is composed of the array of the data fields. 
 *
 * @return	DB_InsertRecords returns the newly inserted record's id if it succeeds in inserting the give record into the DB table and returns INVALID_RECORD_ID if it fails. 
 */
HRECORD DB_InsertRecord(HTABLE table_id, field_data * record);

/** Deletes a record from the DB table. 
 * From the moment when the deleting process is completed, the deleted record should not be accessed 
 * using the record id. 
 *
 * @param table_id	the id of the DB table to delete the record from. 
 * @param record_id	the id of the record to delete
 * @return DB_DeleteRecord returns TRUE if it succeeds in deleting a given record, and returns FALSE if it fails.
 */
bool DB_DeleteRecord(HTABLE table_id, HRECORD record_id);


/** Updates the value of the specified fields in a record in the DB table. 
 * The order of the record field values in "new_data" parameter should be same as the order 
 * used to compose the "record" parameter in InsertRecord. The record field to update should have 
 * its "len" field in the field_data structure equal to or greater than 0. If its "len" field is 
 * smaller than 0, that field is not updated
 *
 * @param table_id		the id of the table which have the record to update.
 * @param record_id		the id of the record to update.
 * @param new_data		an array containing the data fields of the record to update. 
 *						The arranging order of the fields are same as the one used in 
 *						DB_InsertRecordfunction. 
 *
 * @return DB_UpdateRecord returns TRUE if it succeeds in updating specified fields, and returns FALSE if it fails. 
 */
bool DB_UpdateRecord(HTABLE table_id, HRECORD record_id, field_data * new_data);


//	callback function type
/**	Used in the function, CreateView.
 *  The first parameter indicates the DB table to create a view from.
 *	The second parameter indicates a record belonging to the DB table.
 *  returns non-zero value if the given record fits the condition for it 
 *  to be a member of a view. 
 */
typedef rint32 (* CHECKPROC)(HTABLE table_id, HRECORD);

/** Used in the function, SortView.
 *  The first parameter indicates the DB table from which the view was created.
 *	The second and the third parameter indicate the record contained in the view
 *  returns -1 if first record should be placed before the second record given as an argument. 
 *  The return value is 1 if first record should be placed after the second record, 
 *  0 if two records are equivalent in the sort order. 
 */
typedef rint32 (* COMPPROC)(HTABLE table_id, HRECORD, HRECORD);



//	view operations

/** Creates a view containing records which meets the given condition. 
 * "check_proc" parameter is a pointer to a callback function. CreateView traverses all the records 
 * in the table, and invokes "check_proc" passing the record id of each record as its argument. 
 * Check_proc can access the contents of the record using the given record id, and check 
 * whether that record meets a certain condition. If it meets the condition, check_proc should 
 * return a non-zero value, and if it doesn't, it should return zero. The application programmer 
 * should implement his/her own check_proc function and pass it as an argument invoking CreateView. 
 * CreateView inserts into the new view only the records which cause the check_proc function 
 * to return a non-zero value. 
 *
 * @param table_id		the id of the table to create a new view from
 * @param check_proc	a pointer to a function to check whether the given record meets a certain condition
 *
 * return DB_CreateView returns an id of the new view if it succeeds, and returns INVALID_VIEW_ID when it fails. 
 */
HVIEW DB_CreateView(HTABLE table_id, CHECKPROC check_proc);


/** Close a view.
 * @param view_id	the id of the view to close. 
 */
void DB_CloseView(HVIEW view_id);

/** Updates a view when a record insert, delete or update operation was performed on the DB table after the view was created.
 *
 * Those record data changes will not be reflected to the view until DB_RefreshView was invoked. 
 * The view can have corrupted record data if DB_RefreshView was not called after record insert, delete or 
 * update operation was done. Every time those records changes happened, the DB_RefreshView should be 
 * invoked even if the changed record doesn't belong to the view. 
 *
 * @param view_id	the id of the view to update 
 */
void DB_RefreshView(HVIEW view_id);

/** Sort the records in the view. 
 * The "compare_proc" is a pointer to a callback function. Compare_proc receives two records 
 * to compare and should tell which of the two should be placed  before the other in the desired 
 * sorted sequence. SortView uses this compare_proc function to sort all the records in the view. 
 * The application programmer should implement compare_proc and pass it to the SortView as an argument.
 *
 * @param view_id		the id of the view to sort 
 * @param compare_proc	a pointer to the callback function which compares the records given 
 *						in the arguments and returns the comparison result. If the record received 
 *						as the second parameter should be placed before the record received as the third 
 *						parameter, the function should return -1. If the firstrecord should be placed 
 *						after then the second record, compare_proc shouldreturn 1. If they are equivalent, 
 *						compare_proc should return zero. 
 */
void DB_SortView(HVIEW view_id, COMPPROC compare_proc);

/** Returns the number of the records in the view. 
 * @param view_id	the id of the view to find the number of the records contained. 
 * @return the number of the records in the view
 */
rint32 DB_GetViewRecordNumber(HVIEW view_id);

//	record traversal operations

/** Sets the internal iterator to porint32 the first record in the records sequence of the view. 
 * View offers the function to retrieve each of the records contained in it sequentially. 
 * But in this record traversal process, the view doesn't expose the "iterator" which  means 
 * the position of the current retrieved record in the records sequence of the view. 
 * When SetFirstRecord is invoked, the iterator hidden inside the view is set to porint32 the first 
 * record of the records sequence in the view. If DB_SortView is invoked, you can be sure of the order 
 * in which the records are sorted and thus placed in the sequence. However when the view is created 
 * through DB_CreateView, the records are placed in the random order. Though DB_CreateView also sets the 
 * iterator to porint32 the first record of the view, you can't be sure of what the first record is. 
 *
 * @param view_id	the id of the view to initialize the internal iterator
 *
 */
void DB_SetFirstRecord(HVIEW view_id);

/** Sets the position of the internal iterator. 
 * If "pos" parameter is zero, the iterator gets to porint32 the first record. 
 * If it is 1, the iterator will points the second record, and so on. 
 * If it is smaller than zero, the iterator will porint32 the last record of the record sequence of the view. 
 * The iterator will also porint32 the last record when the value of "pos" is greater or equal to the 
 * number of the records contained in the view. 
 *
 * @param view_id	the id of the view to set the iterator position
 * @param pos		the zero-based position of record for the iterator to porint32.
 *
 */
void DB_SetRecordPos(HVIEW view_id, rint32 pos);

/** Returns the zero-based position of the record pointed by the iterator in the view. 
 * For example, if the iterator of the view points the first record currently, DB_GetRecordPos will return 0. 
 * If it pointed the second record, this function would have returned 1. 
 * 
 * @param view_id	the id of the view to find the position of the iterator
 * @return zero-based position index of the view iterator
 *
 */
rint32 DB_GetRecordPos(HVIEW view_id);

/** Returns the id of the record pointed by the iterator, and make the iterator porint32 the next record. 
 * If the iterator was pointing outside the last record in the record sequence of the view 
 * when DB_GetNextRecord is invoked, DB_GetNextRecord returns INVALID_RECORD_ID
 * 
 * @param view_id	the id of the view to get record id from.
 * @return record_id of the record pointed by the view iterator
 */
HRECORD DB_GetNextRecord(HVIEW view_id);

/** Returns the id of the record pointed by the iterator, and make the iterator porint32 the previous record. 
 * If the iterator was pointing outside the first record in the record sequence of the view when 
 * DB_GetNextRecord is invoked, DB_GetNextRecord returns INVALID_RECORD_ID.
 *
 * @param view_id	the id of the view to get record id from. 
 * @return record_id of the record pointed by the view iterator
 */
HRECORD DB_GetPrevRecord(HVIEW view_id);



// field retrieval operations 

/** Copies the record field data specified by the field name into the buffer.
 *	Use DB_GetFieldValueByName for DBS_SHORT or DBS_LONG type instead of this
 * 
 * @param table_id		the id of the DB table containing the record field data. 
 * @param record_id		the id of the record containing the field data
 * @param field_name	the name of the field to retrieve. The name used to specify the field should be 
 *						the same as the name used in the "schema" parameter of OpenTable 
 * @param buffer		buffer to copy the field data into.
 * @param buf_len		the byte length of the buffer parameter
 * @return	If the buffer passed as an argument doesn't have enough space to store the field data or 
 *			if the function fails to copy the field data, DB_GetFieldDataByName returns FALSE. 
 *			If it finishes its operation successfully, it returns TRUE.
 */
bool DB_GetFieldDataByName(HTABLE table_id, HRECORD record_id, char * field_name, char * buffer, rint32 buf_len);

/** Returns the byte length of the specified field in the given record. 
 * @param table_id		the id of the DB table containing the record field data. 
 * @param record_id		the id of the record containing the field data
 * @param field_name	the name of the field to retrieve. The name used to specify the field should be 
 *						the same as the name used in the "schema" parameter of DB_OpenTable 
 *
 * @return	If the DB_GetFieldLenByName succeeds in finding out the length of the specified field, 
 *			it returns the length equal to or greater than zero. If it fails, it returns a value smaller 
 *			than zero. 
 */ 
rint32 DB_GetFieldLenByName(HTABLE table_id, HRECORD record_id, char * field_name);

/** This function performs basically same operation as DB_GetFieldGetFieldDataByName.
 * Only difference between theses two functions is the field identifier, which is given as 
 * a 0-based index in DB_GetFieldDataByIndex while a field name is used in DB_GetFieldDataByName. 
 * This index indicates the order used to arrange the array elements in the "schema" parameter 
 * of the DB_OpenTable function. For example, if the schema was specified to have three fields 
 * with the following order - "name", "address", "phone number", the index for the "name" field is 0. 
 * The value of the index for the "address field" is 1 and 2 for the "phone number" field. 
 * Use DB_GetFieldValueByName for DBS_SHORT or DBS_LONG type instead of this
 *
 * @param table_id		the id of the DB table containing the record field data. 
 * @param record_id		the id of the record containing the field data
 * @param field_index	the index of the field to retrieve. The index used to define schema
 *						(field data type) in OpenTable should be applied here. 
 * @param buffer		buffer to copy the field data into.
 * @param buf_len		the byte length of the buffer parameter
 * @return	If the buffer passed as an argument doesn't have enough space to store the field data or 
 *			if the function fails to copy the field data, DB_GetFieldDataByName returns FALSE. 
 *			If it finishes its operation successfully, it returns TRUE.
 */
bool DB_GetFieldDataByIndex(HTABLE table_id, HRECORD record_id, rint32 field_index, char * buffer, rint32 buf_len);

/** Returns the byte length of the specified string field in the given record. 
 * If the specified field is not DBS_CHAR type, simply returns its max length instead of the real string lengths
 * 
 * @param table_id		the id of the DB table containing the record field data. 
 * @param record_id		the id of the record containing the field data
 * @param field_index	the index of the field to retrieve. The index used to define schema
 *						(field data type) in DB_OpenTable should be applied here.
 * @return	DB_GetFieldDataByIndex returns the length equal to or greater than zero. 
 *			If it fails, it returns a value smaller than zero.
 */
rint32 DB_GetFieldLenByIndex(HTABLE table_id, HRECORD record_id, rint32 field_index);

/** Get the numeric field data with given index. for DBS_CHAR or DBS_BIN type, use
 *	DB_GetFieldDataByIndex instead of this function.
 * @param table_id		the id of the DB table containing the record field data. 
 * @param record_id		the id of the record containing the field data
 * @param field_index	the index of the field to retrieve. The index used to define schema
 *						(field data type) in OpenTable should be applied her
 * @return	The value of matching field. In case of error, default return value is 0.
 */
unsigned long DB_GetFieldValueByIndex(HTABLE table_id, HRECORD record_id, rint32 field_index);

/** Get the numeric field data with the given field name. for DBS_CHAR or DBS_BIN type, use
 *	DB_GetFieldDataByName instead of this function.
 * 
 * @param table_id		the id of the DB table containing the record field data. 
 * @param record_id		the id of the record containing the field data
 * @param field_name	the name of the field to retrieve. The name used to specify the field should be 
 *						the same as the name used in the "schema" parameter of OpenTable 
 *
 * @return	The value of matching field. In case of error, default return value is 0.
 */
unsigned long DB_GetFieldValueByName(HTABLE table_id, HRECORD record_id, char *field_name );

/** You can lock the database through this function. Others cannot open this database unless
 *  you unlock the locked database.
 *
 * @brief				Lock database table
 * @param	table_id	the id of the DB table containing the record field data.
 * @param	bLock		TRUE to lock the DB, FALSE to unlock
 * @return				TRUE on success					
 */
bool DB_SetLockStatus( HTABLE table_id, bool bLock );

/** DB_CloseTable function flushes database cache to file perfectly. However, if you need
 *  integrity of database immediately, call this function. Please note that heavy use of
 *  this function can degrade performance of your application.
 *  By default, database is created as unlocked.
 *
 *	@param	table_id	the id of the DB table containing the record field data.
 *  @return	TRUE on success, FALSE otherwise
 */
bool DB_Sync( HTABLE table_id );

/** Eliminate fragmentation in the specified data table
 *  By calling this function, you can get a optimized database ( shorter size ),
 *  but please note that after calling this function, you have to refresh your open view if any,
 *  since record id can be changed. Please do not call this function frequently for it's rather
 *	expensive operation
 *
 *  @param table_id		the id of the DB table containing the record field data
 *  @return				zero on success, err codes for error. 
 **/
rint32 DB_Defrag( HTABLE table_id );

/** @} */

#endif 

