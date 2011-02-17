#include "rexy.h"
#include "rexy_db.h"
#include "application.h"


#ifndef F_REXY_ENV_PHONE
// every warning is checked by jmjeong. [2005-10-03]
#pragma warning ( disable : 4127 )      // : warning C4127: conditional expression is constant
#endif

static field_type g_example_schema [3] = 
{
	{ "name", CHAR, 100 }, 
	{ "address", CHAR, 100 },
	{ "phone", CHAR, 100 }
};

static void SetExampleSchema(field_type * schema)
{
	int i;
	field_type * sample_schema;

	
	sample_schema = g_example_schema;

	for ( i = 0; i < sizeof(g_example_schema) / sizeof(field_type); ++i )
	{
		MemSet(&(schema[i]), 0, sizeof(field_type));
		schema[i].max_len = sample_schema[i].max_len;
		schema[i].type = sample_schema[i].type;
		StrCpy(schema[i].name , sample_schema[i].name);
	}

	return;
}

static BOOL ExampleCreation(WND wnd)
{
/*
table open, close , delete*/

	HTABLE new_table;
	field_type schema[3];

	GdiTextOut(wnd, "1. DB Creation Test", StrLen("1. DB Creation Test"), 10, 20);

	// Find table 
	if ( DB_FindTable("nodb.db") == TRUE )
	{
		TRACE(TR_Test, TR_LEVEL1,("Error : Found a non existing table\n"));
		GdiTextOut(wnd, "Error : Found a non existing table", StrLen("Error : Found a non existing table"), 10, 35);
		return FALSE;
	}

	// Open Table. The table can't exist on the file system because DB_FindTable didn't find the table. 
	if ( DB_OpenTable("testDB", NULL, 0) == TRUE )
	{
		TRACE(TR_Test, TR_LEVEL1,("Error : Opened a non existing table\n"));
		GdiTextOut(wnd, "Error : Opened a non existing table", StrLen("Error : Opened a non existing table"), 10, 35);
		return FALSE;
	}

	//set schema 
	SetExampleSchema(schema);

	//Create a db table with schema previously set. 
	new_table = DB_OpenTable("testDB", schema, 3);
	if ( new_table == INVALID_TABLE_ID )
	{
		TRACE(TR_Test, TR_LEVEL1,("Error : Failed to create a db table\n"));
		GdiTextOut(wnd, "Error : Failed to create a db table", StrLen("Error : Failed to create a db table"), 10, 35);
		return FALSE;
	}
	else
	{
		//close table if we succeeded to open. 
		DB_CloseTable(new_table);
	}

	// now we can gurantee the table exists on the file system. 
	new_table = DB_OpenTable("testDB", NULL, 0); // by setting the schema argument as null, we are opening an existing table. 
	if ( new_table == INVALID_TABLE_ID )
	{
		TRACE(TR_Test, TR_LEVEL1,("Failed to open a created db\n"));
		GdiTextOut(wnd, "Error : Failed to open a created db", StrLen("Error : Failed to open a created db"), 10, 35);
		return FALSE;
	}
	else
	{
		// now we succeded in opeing table. now we try to delete it. 
		DB_DeleteTable(new_table);
		TRACE(TR_Test, TR_LEVEL1,("Passed all the steps of Creation Test\n"));
		GdiTextOut(wnd, "Success : Passed all the steps of Creation Test", StrLen("Error : Passed all the steps of Creation Test"), 10, 35);
	}

	return TRUE;
}


static BOOL ExampleRecordInsertion(WND wnd)
{
	HTABLE new_table;
	field_type schema[3];

	int i, k;
	field_data test_record[3];
	char data_buffer_1[80];
	char data_buffer_2[80];
	char data_buffer_3[80];
	HVIEW view;
	HRECORD record_id;

    /*
	int test_sequence[9][3] = {
		{ 3, 3, 4 },
		{ 3, 4, 3 },
		{ 4, 3, 3 }, 
		{ 2, 5, 3 },
		{ 5, 3, 2 },
		{ 3, 2, 5 }, 
		{ 1, 3, 6 }, 
		{ 3, 6, 1 }, 
		{ 6, 1, 3 }
	};
	
	char * test_strings[3][3] = {
		{"DaeHyun Yoon", "Hanshin Apt, Dogok Dong, KangNam, Seoul, Korea", "010-4854-500" },
		{"SungYoung Park", "One Room, NakSungDaem, KwanAk, Seoul, Korea", "010-9189-8515" }, 
		{"SungHo Moon", "JooGong Apt, YangJae Dong, KangNam, Seoul, Korea", "011-234-1904" },
	};
	*/

	GdiTextOut(wnd, "2. Record Insertion Test", StrLen("2. Record Insertion Test"), 10, 50);
	
	// find table. 
	if ( DB_FindTable("testDB") == TRUE )
	{
		//TRACE(TR_Test, TR_LEVEL1, ("Retry after deleting testDB"));
		//return FALSE;
		new_table = DB_OpenTable("testDB", NULL, 0);
		if(new_table == INVALID_TABLE_ID)
		{
			GdiTextOut(wnd, "Error : Failed to open a created db", StrLen("Error : Failed to open a created db"), 10, 65);
			return FALSE;
		}
		DB_DeleteTable(new_table);
		new_table = INVALID_TABLE_ID;
	}

	test_record[0].data = data_buffer_1;
	test_record[1].data = data_buffer_2;
	test_record[2].data = data_buffer_3;

	SetExampleSchema(schema);
	new_table = DB_OpenTable("testDB", schema, 3);	//create the table with the given schema. 
	DB_CloseTable(new_table);
	new_table = DB_OpenTable("testDB", NULL, 0);

	for ( k = 0; k < 100; ++k )
	{
		for ( i = 0; i < 3; ++i )
		{
			if(k < 10)
				StrPrintF(test_record[i].data, "%02d%d",k, i);
			else if(k < 100)
				StrPrintF(test_record[i].data, "%01d%d",k, i);
			else
				StrPrintF(test_record[i].data, "%d%d",k, i);
			
			//test_record[i].len = StrLen(test_record[i].data) + 1;	//record length field should include a NULL character at the end of the string.
			test_record[i].len = 80;
		}

		record_id = DB_InsertRecord(new_table, test_record);
		if(record_id == INVALID_RECORD_ID)
		{
			GdiTextOut(wnd, "Error : Failed to insert record", StrLen("Error : Failed to insert record"), 10, 65);
			DB_CloseTable(new_table);
			return FALSE;
		}
	}
	
	view = DB_CreateView(new_table, NULL);
	DB_SetFirstRecord(view);
	
	for ( i = 0; i < 100; ++i)
	{
		record_id = DB_GetNextRecord(view);
		MemSet(data_buffer_1, 0, sizeof(data_buffer_1));
		DB_GetFieldDataByIndex(new_table, record_id, 0, data_buffer_1, 80);

		if(i < 10)
			StrPrintF(data_buffer_2, "%02d%d", i, 0);
		else if(i < 100)
			StrPrintF(data_buffer_2, "%01d%d", i, 0);
		else
			StrPrintF(data_buffer_2, "%d%d", i, 0);

		if ( StrCmp(data_buffer_1, data_buffer_2) != 0 )
		{
			DB_CloseView(view);
			DB_DeleteTable(new_table);
			GdiTextOut(wnd, "Error : Record Insertion Error", StrLen("Error : Record Insertion Error"), 10, 65);
			return FALSE;
		}
	}

	DB_CloseView(view);
	DB_CloseTable(new_table);
	new_table = INVALID_TABLE_ID;
	view = INVALID_VIEW_ID;
	
	new_table = DB_OpenTable("testDB", NULL, 0);	//create the table with the given schema. 
	view = DB_CreateView(new_table, NULL);
	DB_SetFirstRecord(view);
	
	for ( i = 0; i < 100; ++i)
	{
		record_id = DB_GetNextRecord(view);
		MemSet(data_buffer_1, 0, sizeof(data_buffer_1));
		DB_GetFieldDataByIndex(new_table, record_id, 0, data_buffer_1, 80);

		if(i < 10)
			StrPrintF(data_buffer_2, "%02d%d", i, 0);
		else if(i < 100)
			StrPrintF(data_buffer_2, "%01d%d", i, 0);
		else
			StrPrintF(data_buffer_2, "%d%d", i, 0);

		if ( StrCmp(data_buffer_1, data_buffer_2) != 0 )
		{
			DB_CloseView(view);
			DB_DeleteTable(new_table);
			GdiTextOut(wnd, "Error : Record Insertion Error", StrLen("Error : Record Insertion Error"), 10, 65);
			return FALSE;
		}
	}
	DB_CloseView(view);
	DB_DeleteTable(new_table);

	
	TRACE(TR_Test, TR_LEVEL1, ("Passed all the steps of Record Operation Test\n"));
	GdiTextOut(wnd, "Passed all the steps of Record Insertion Test", StrLen("Passed all the steps of Record Insertion Test"), 10, 65);

	return TRUE;
}



static BOOL ExampleRecordUpdate(WND wnd)
{
	HTABLE new_table;
	field_type schema[3];

	int j;
	int i;
	int found_number = 0;
	field_data test_record[3];
	char data_buffer_1[80];
	char data_buffer_2[80];
	char data_buffer_3[80];
	HVIEW view;
	HRECORD record_id, found_id;

	found_id = 0;

	GdiTextOut(wnd, "3. Record Update Test", StrLen("3. Record Update Test"), 10, 80);

	//create a new table
	SetExampleSchema(schema);
	new_table = DB_OpenTable("testDB", schema, 3);	//open the table. 
	if ( new_table == INVALID_TABLE_ID )
	{
		TRACE(TR_Test, TR_LEVEL1,("Error : Failed to create a db table\n"));
		GdiTextOut(wnd, "Error : Failed to create a db table", StrLen("Error : Failed to create a db table"), 10, 95);
		return FALSE;
	}
	else
	
	test_record[0].data = data_buffer_1;
	test_record[1].data = data_buffer_2;
	test_record[2].data = data_buffer_3;


	StrCpy(test_record[0].data, "Yoon");
	//test_record[0].len = StrLen(test_record[0].data) + 1;
	test_record[0].len = 80;
	StrCpy(test_record[1].data, "KangNam");
	//test_record[1].len = StrLen(test_record[1].data) + 1;
	test_record[1].len = 80;
	StrCpy(test_record[2].data, "1669");
	//test_record[2].len = StrLen(test_record[2].data) + 1;
	test_record[2].len = 80;

	for ( i = 0; i < 999; ++i )
	{
		DB_InsertRecord(new_table, test_record);	//insert a new record. 
	}

	StrCpy(test_record[0].data, "Park");
	//test_record[0].len = StrLen(test_record[0].data) + 1;
	test_record[0].len = 80;
	DB_InsertRecord(new_table, test_record);

	view = DB_CreateView(new_table, NULL);		//create a view to get record IDs. 
	DB_SetFirstRecord(view);
	found_number = 0;
	for ( i = 0; i < 1000; ++i )
	{
		record_id = DB_GetNextRecord(view);
		if ( record_id == INVALID_RECORD_ID )
		{
			TRACE(TR_Test, TR_LEVEL1,("Not all the records were saved in the DB\n"));
			DB_CloseView(view);
			DB_DeleteTable(new_table);
			GdiTextOut(wnd, "Error : Not all the records were saved in the DB", StrLen("Error : Not all the records were saved in the DB"), 10, 95);
			return FALSE;
		}

		if ( i == 999 )
		{
			j = 10;
		}

		DB_GetFieldDataByName(new_table, record_id, "name", data_buffer_1, 80);
		if ( StrCmp(data_buffer_1, "Park") == 0 )
		{
			found_number++;			//we inserted only one record whose name field is "Park"
			found_id = record_id;
		}
	}

	if ( found_number == 1 )	
	{
		StrCpy(test_record[0].data, "Choi");	//now we try to change the name field to "Choi"
		//test_record[0].len = StrLen(test_record[0].data) + 1; 
		test_record[0].len = 80;
		test_record[1].len = -1;	//For unchanged fields, we should set this "len" field to be "-1"
		test_record[2].len = -1;	

		DB_UpdateRecord(new_table, found_id, test_record);
	}
	else
	{
		TRACE(TR_Test, TR_LEVEL1,("Error : The number of the records found was not correct\n"));
		DB_CloseView(view);
		DB_DeleteTable(new_table);
		GdiTextOut(wnd, "Error : The number of the records found was not correct", StrLen("Error : The number of the records found was not correct"), 10, 95);
		return FALSE;
	}

	DB_RefreshView(view);		//Call this function whenever the record is inserted or deleted or updated. 
	DB_SetFirstRecord(view);	//traverse again.
	for ( i = 0; i < 1000; ++i )
	{
		record_id = DB_GetNextRecord(view);
		if ( record_id == INVALID_RECORD_ID )
		{
			TRACE(TR_Test, TR_LEVEL1,("Error : Not all the records were saved in the DB\n"));
			DB_CloseView(view);
			DB_DeleteTable(new_table);
			GdiTextOut(wnd, "Error : Not all the records were saved in the DB", StrLen("Error : Not all the records were saved in the DB"), 10, 95);
			return FALSE;
		}

		//try to check we have only one record whose "name" field is "Choi"
		DB_GetFieldDataByName(new_table, record_id, "name", data_buffer_1, 80);
		if ( StrCmp(data_buffer_1, "Choi") == 0 )
		{
			--found_number;
			found_id = record_id;
		}
	}
	
	//delete a found record. 
	DB_DeleteRecord(new_table, found_id);
	DB_RefreshView(view);
	if ( DB_GetViewRecordNumber(view) != 999 )	//check we deleted only one record. 
	{
		TRACE(TR_Test, TR_LEVEL1,("Error : Deletion was not correct\n"));
		DB_CloseView(view);
		DB_DeleteTable(new_table);
		GdiTextOut(wnd, "Error : Deletion was not correct", StrLen("Error : Deletion was not correct"), 10, 95);
		return FALSE;
	}

	DB_SetFirstRecord(view);
	for ( i = 0; i < 999; ++i )
	{
		record_id = DB_GetNextRecord(view);
		if ( record_id == INVALID_RECORD_ID )
		{
			TRACE(TR_Test, TR_LEVEL1,("Error : Not all the records were saved in the DB\n"));
			DB_CloseView(view);
			DB_DeleteTable(new_table);
			GdiTextOut(wnd, "Error : Not all the records were saved in the DB", StrLen("Error : Not all the records were saved in the DB"), 10, 95);
			return FALSE;
		}

		//we deleted a "Choi" record before so that we shouldn't meet any record whose name field is "Choi"
		DB_GetFieldDataByName(new_table, record_id, "name", data_buffer_1, 80);
		if ( StrCmp(data_buffer_1, "Choi") == 0 )
		{
			TRACE(TR_Test, TR_LEVEL1,("Error : Deletion was not correct\n"));
			DB_CloseView(view);
			DB_DeleteTable(new_table);
			GdiTextOut(wnd, "Error : Deletion was not correct", StrLen("Error : Deletion was not correct"), 10, 95);
			return FALSE;			
		}
	}


	DB_CloseView(view);
	DB_DeleteTable(new_table);
	TRACE(TR_Test, TR_LEVEL1,("All the update operation was finished successfully\n"));
	GdiTextOut(wnd, "All the update operation was finished successfully", StrLen("All the update operation was finished successfully"), 10, 95);

	return TRUE;
}

static BOOL ExampleRecordDeletion(WND wnd)
{
	HTABLE new_table;
	field_type schema[3];

	int i, k;
	field_data test_record[3];
	char data_buffer_1[80];
	char data_buffer_2[80];
	char data_buffer_3[80];
	char record_buffer[80];
	HVIEW view;
	HRECORD record_ids[1000];
	
	GdiTextOut(wnd, "3. Record Deletion Test", StrLen("3. Record Deletion Test"), 10, 110);
	
	// find table. 
	if ( DB_FindTable("testDB") == TRUE )
	{
		//TRACE(TR_Test, TR_LEVEL1, ("Retry after deleting testDB"));
		//return FALSE;
		new_table = DB_OpenTable("testDB", NULL, 0);
		if(new_table == INVALID_TABLE_ID)
		{
			GdiTextOut(wnd, "Error : Failed to open a created db", StrLen("Error : Failed to open a created db"), 10, 125);
			return FALSE;
		}
		DB_DeleteTable(new_table);
		new_table = INVALID_TABLE_ID;
	}

	test_record[0].data = data_buffer_1;
	test_record[1].data = data_buffer_2;
	test_record[2].data = data_buffer_3;

	SetExampleSchema(schema);
	new_table = DB_OpenTable("testDB", schema, 3);	//create the table with the given schema. 
	DB_CloseTable(new_table);
	new_table = DB_OpenTable("testDB", NULL,0);

	for ( k = 0; k < 1000; ++k )
	{
		for ( i = 0; i < 3; ++i )
		{
			StrPrintF(test_record[i].data, "%d%d",k, i);
			test_record[i].len = StrLen(test_record[i].data) + 1;	//record length field should include a NULL character at the end of the string.
		}

		record_ids[k] = DB_InsertRecord(new_table, test_record);
	}

	for ( k = 0; k < 500; ++k )
	{
		DB_DeleteRecord(new_table, record_ids[k]);
	}

	for ( k = 500; k < 1000; ++k )
	{
		DB_GetFieldDataByIndex(new_table, record_ids[k], 0, record_buffer, 80);
		StrPrintF(data_buffer_1, "%d%d", k, 0);
		if ( StrCmp(record_buffer, data_buffer_1) != 0 )
		{
			//AS_Trace("Oops. Records corrupted after deletion\n");
			//ASSERT(FALSE);'
			GdiTextOut(wnd, "Error : Records corrupted after deletion", StrLen("Error : Records corrupted after deletion"), 10, 125);
			DB_DeleteTable(new_table);
			return FALSE;
		}
	}

	DB_CloseTable(new_table);
	new_table = DB_OpenTable("testDB", NULL, -1);	//create the table with the given schema. 
	if ( DB_GetTableRecordNumber(new_table) != 500 )
	{
		//AS_Trace("Deleting Records Failed\n");
		//ASSERT(FALSE);
		GdiTextOut(wnd, "Error : Deleting Records Failed", StrLen("Error : Deleting Records Failed"), 10, 125);
		DB_DeleteTable(new_table);
		return FALSE;
	}

	for ( k = 500; k < 1000; ++k )
	{
		DB_GetFieldDataByIndex(new_table, record_ids[k], 1, record_buffer, 80);
		StrPrintF(data_buffer_1, "%d%d", k, 1);
		if ( StrCmp(record_buffer, data_buffer_1) != 0 )
		{
			//AS_Trace("Oops. Records corrupted after deletion\n");
			//ASSERT(FALSE);
			GdiTextOut(wnd, "Error : Records corrupted after deletion", StrLen("Error : Records corrupted after deletion"), 10, 125);
			DB_DeleteTable(new_table);
			return FALSE;
		}
	}

	view = DB_CreateView(new_table, NULL);
	DB_SetFirstRecord(view);
	if ( DB_GetViewRecordNumber(view) != DB_GetTableRecordNumber(new_table) )
	{
		//AS_Trace("Record Insertion to View has failed\n");
		//ASSERT(FALSE);
		GdiTextOut(wnd, "Error : Record Insertion to View has failed", StrLen("Error : Record Insertion to View has failed"), 10, 125);
		DB_CloseView(view);
		DB_DeleteTable(new_table);
		return FALSE;
	}
	
	for ( i = 0; i < DB_GetViewRecordNumber(view); ++i)
	{
		HRECORD record_id;
		record_id = DB_GetNextRecord(view);
		for ( k = 500; k < 1000; ++k )
		{
			if ( record_ids[k] == record_id )
			{
				record_ids[k] = INVALID_RECORD_ID;
				break;
			}
		}

		if ( k == 1000 )
		{
			//AS_Trace("Records in the view are corrupted\n");
			//ASSERT(FALSE);
			GdiTextOut(wnd, "Error : Records in the view are corrupted", StrLen("Error : Records in the view are corrupted"), 10, 125);
			DB_CloseView(view);
			DB_DeleteTable(new_table);
			return FALSE;
		}
	}

	DB_CloseView(view);
	DB_DeleteTable(new_table);

	
	TRACE(TR_Test, TR_LEVEL1, ("Passed all the steps of Record Operation Test\n"));
	GdiTextOut(wnd, "Passed all the steps of Record Deletion Test", StrLen("Passed all the steps of Record Deletion Test"), 10, 125);

	return TRUE;
}

int tst_comp_proc(HTABLE table_id, HRECORD rd1, HRECORD rd2)
{
	char data_buffer1[80];
	char data_buffer2[80];

	MemSet(data_buffer1, 0, sizeof(data_buffer1));
	MemSet(data_buffer2, 0, sizeof(data_buffer2));

	DB_GetFieldDataByIndex(table_id, rd1, 0, data_buffer1, 80);
	DB_GetFieldDataByIndex(table_id, rd2, 0, data_buffer2, 80);

	return (-1) * StrNCmp(data_buffer1, data_buffer2, 4);
}

static void ExampleSort(WND wnd)
{
	HTABLE new_table;
	field_type schema[3];

	int i, k;
	field_data test_record[3];
	char data_buffer_1[80];
	char data_buffer_2[80];
	char data_buffer_3[80];
	HVIEW view;
	
	GdiTextOut(wnd, "4. Sorting Test", StrLen("4. Sorting Test"), 10, 140);
	
	// find table. 
	if ( DB_FindTable("test_sortDB") == TRUE )
	{
		//TRACE(TR_Test, TR_LEVEL1, ("Retry after deleting testDB"));
		//return;
		new_table = DB_OpenTable("test_sortDB", NULL, 0);
		if(new_table == INVALID_TABLE_ID)
		{
			GdiTextOut(wnd, "Error : Failed to open a created db", StrLen("Error : Failed to open a created db"), 10, 155);
			return;
		}
		DB_DeleteTable(new_table);
		new_table = INVALID_TABLE_ID;
	}

	test_record[0].data = data_buffer_1;
	test_record[1].data = data_buffer_2;
	test_record[2].data = data_buffer_3;

	SetExampleSchema(schema);
	new_table = DB_OpenTable("test_sortDB", schema, 3);	//create the table with the given schema. 

	for ( k = 0; k < 2; ++k )
	{
		for ( i = 0; i < 3; ++i )
		{
			StrPrintF(test_record[i].data, "0000%d", k);
			test_record[i].len = StrLen(test_record[i].data) + 1;	//record length field should include a NULL character at the end of the string.
		}

			DB_InsertRecord(new_table, test_record);
	}

	view = DB_CreateView(new_table, NULL);
	if ( DB_GetViewRecordNumber(view) != DB_GetTableRecordNumber(new_table) )
	{
		//AS_Trace("Record Insertion to View has failed\n");
		//ASSERT(FALSE);
		GdiTextOut(wnd, "Error : Record Insertion to View has failed", StrLen("Error : Record Insertion to View has failed"), 10, 155);
		DB_CloseView(view);
		DB_DeleteTable(new_table);
		return;
	}

	DB_SortView(view, tst_comp_proc);
	DB_SetFirstRecord(view);

	for ( i = 0; i < DB_GetViewRecordNumber(view); ++i)
	{
		HRECORD record_id;
		record_id = DB_GetNextRecord(view);

		MemSet(data_buffer_1, 0, sizeof(data_buffer_1));
		DB_GetFieldDataByIndex(new_table, record_id, 0, data_buffer_1, 80);
		//AS_Trace("%s", data_buffer_1);
	}

//	DB_CloseView(view);

	for ( k = 2; k < 4; ++k )
	{
		for ( i = 0; i < 3; ++i )
		{
			StrPrintF(test_record[i].data, "0020%d", k);
			test_record[i].len = StrLen(test_record[i].data) + 1;	//record length field should include a NULL character at the end of the string.
		}

			DB_InsertRecord(new_table, test_record);
	}

//	view = DB_CreateView(new_table, NULL);
//	DB_SortView(view, tst_comp_proc);
	DB_RefreshView(view);
	DB_SetFirstRecord(view);
	
	for ( i = 0; i < DB_GetViewRecordNumber(view); ++i)
	{
		HRECORD record_id;
		record_id = DB_GetNextRecord(view);
		
		MemSet(data_buffer_1, 0, sizeof(data_buffer_1));
		DB_GetFieldDataByIndex(new_table, record_id, 0, data_buffer_1, 80);
		//AS_Trace("%s", data_buffer_1);
	}


	DB_CloseView(view);
	DB_DeleteTable(new_table);

	GdiTextOut(wnd, "Passed Sorting", StrLen("Passed Sorting"), 10, 155);
	return;
}

BOOL ExampleDBAPI(WND wnd)
{
	if ( ExampleCreation(wnd) == FALSE )
	{
		TRACE(TR_Test, TR_LEVEL1,("Table Operation Error\n"));
		return FALSE;
	}

	if ( ExampleRecordInsertion(wnd) == FALSE )
	{
	 	TRACE(TR_Test, TR_LEVEL1,("Record Operation Error\n"));
		return FALSE;
	}

	if ( ExampleRecordUpdate(wnd) == FALSE )
	{
		TRACE(TR_Test, TR_LEVEL1,("View Operation Error\n"));
		return FALSE;
	}
	
	if ( ExampleRecordDeletion(wnd) == FALSE )
	{
		TRACE(TR_Test, TR_LEVEL1, ("Delete Operation Error\n"));
		return FALSE;
	}

	ExampleSort(wnd);

	GdiTextOut(wnd, "Press any key", StrLen("Press any key"), 10, 180);

	return TRUE;
}

int PrvSampleDBProc(WND wnd, int msg, int wparam, int lparam)
{
    int bHandled = FALSE;

	switch(msg)
	{
	case AWM_CREATE:
		GdiClrScreen(wnd, COLOR_WHITE);
		ExampleDBAPI(wnd);
		bHandled = TRUE;
		break;

	case AWM_DESTROY:
		bHandled = TRUE;
		break;
		
	case AWM_KEYDOWN:
		StaGotoState(wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );
		bHandled = TRUE;
		break;
	}

	return bHandled;
}

int SampleDBTestProc(WND wnd, int msg, int wparam, int lparam)
{
	int ScreenWidth, ScreenHeight;
	int bHandled = FALSE;
    switch(msg)
    {
    case AWM_CREATE:
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight );
		WinCreateWindow(PrvSampleDBProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL);
		bHandled = TRUE;
        break;
	case AWM_DESTROY:
		WinDestroyWindow(wnd);
		bHandled = TRUE;
		break;
	}
    return bHandled;
}


