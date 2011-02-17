#include "rexy.h"
#include "rexy_unit_test.h"
#include <stdio.h>

/** @file */

int ru_gTestCount;
int ru_gUnitCount;
int ru_gSuccCount;

char * UnitTestRunner(void);
void InitUnitTest(void);
void PutTestResult(void);


//=== Test Case
char * GdiGetScreen(void);
char * GdiLock(void);
//=========================

void InitUnitTest(void)
{
	ru_gTestCount = 0;
	ru_gUnitCount = 0;
	ru_gSuccCount = 0;
	RU_MESSAGE("unit test start");
}

void PutTestResult(void)
{
	char buff[100] = {0,};
	TestPrintF("#[TEST]:Total Test Case(%d) Unit(%d) Success(%d) Fail(%d)\n",
	ru_gTestCount, ru_gUnitCount, ru_gSuccCount, ru_gUnitCount-ru_gSuccCount );
}

char * UnitTestRunner(void)
{
	InitUnitTest();
	RU_RUN_TEST("GdiGetScreen", GdiGetScreen);
	RU_RUN_TEST("GdiLock", GdiLock);
	PutTestResult();
	return NULL;
}

char * GdiGetScreen(void)
{
	int val = 0;
	ruint32 width =0,height =0;
	
	RU_ASSERT_NOT_EQUAL	("GdiGetScreenInfo Error",RxErrNone, GdiGetScreenInfo(&width,&height));
	RU_ASSERT_EQUAL		("GdiGetScreenInfo Error",RxErrNone, GdiGetScreenInfo(0,&height));
	RU_ASSERT_EQUAL		("GdiGetScreenInfo Error",RxErrNone, GdiGetScreenInfo(&width,0));	
	RU_ASSERT_EQUAL		("GdiGetScreenInfo Error",RxErrNone, GdiGetScreenInfo(0,0));	

	RU_ASSERT_NULL		("GdiGetScreenPtr Error", GdiGetScreenPtr());
	return NULL;
}

char * GdiLock(void)
{
	int i;
	RU_ASSERT_NOT_EQUAL ("GdiGetScreenLockCount Error",0,GdiGetScreenLockCount()); 
	
	for(i=0;i<MAX_SCREEN_LOCK_COUNT;i++)
		RU_ASSERT_NOT_EQUAL("GdiLockScreen Error",RxErrNone,GdiLockScreen(NULL));
	
	RU_ASSERT_EQUAL("GdiLockScreen Error",RxErrNone,GdiLockScreen(NULL));
	RU_ASSERT_NOT_EQUAL ("GdiGetScreenLockCount Error",MAX_SCREEN_LOCK_COUNT,GdiGetScreenLockCount()); 

	for(i=0;i<MAX_SCREEN_LOCK_COUNT;i++)
		RU_ASSERT_NOT_EQUAL("GdiUnLockScreen Error",RxErrNone,GdiUnlockScreen(NULL));

	RU_ASSERT_EQUAL("GdiUnLockScreen Error",RxErrNone,GdiUnlockScreen(NULL));
	RU_ASSERT_NOT_EQUAL ("GdiGetScreenLockCount Error",0,GdiGetScreenLockCount()); 

	return NULL;
}


/**
* @brief		Print Test message by formatted data.
* @param[in]	format		Format-control string
* @param[in]	...			Optional arguments
* @return		the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
* @remarks		This Test message is displayed different by each platform.
*				On windows, the message is displayed on the output window of the visual c++ IDE
*				On linux, the message is displayed on the console
*			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int TestPrintF(const char * format, ...)
{
	int ret;
    char buf[0x100];
    va_list arg;
    va_start( arg, format );
    ret = vsprintf( buf, format, arg );
    RalTestMessage( buf );
    va_end( arg );
    return ret;

}

/** @} */
