
#ifndef __REXY_UNIT_TEST_H__
#define __REXY_UNIT_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @file */
	
extern int ru_gTestCount;
extern int ru_gUnitCount;
int TestPrintF(const char * format, ...);

char * UnitTestRunner(void);

#define RU_ASSERT_TRACE

#define RU_RUN_TEST(_message, _func)			\
	do{	char * message; TestPrintF("#[TEST]:%s\n",_message); message =_func();ru_gTestCount++; \
		if(message) return message;	}while(0)	

#ifdef RU_ASSERT_TRACE
#define RU_ASSERT(_message)		  \
	do{								\
		TestPrintF("[TEST %04d-%04d]:%s(%04d):%s\n", ru_gTestCount,ru_gUnitCount, TrcGetShortFileName(__FILE__), __LINE__,_message);	\
		}while(0)	
#else
#define RU_ASSERT(_message)  \
		return _message;
#endif						

#define RU_MESSAGE(_message) \
	do{ TestPrintF("#[TEST]:%s\n",_message);}while(0)
	
#define RU_ASSERT_FALSE( _message, val)  \
	do{ru_gUnitCount++; if(val==0) RU_ASSERT(_message); else ru_gSuccCount++; } while(0)

#define RU_ASSERT_EQUAL(_message, actual, expected) \
	do{ru_gUnitCount++; if(actual==expected) RU_ASSERT( _message); else ru_gSuccCount++;} while(0)

#define RU_ASSERT_NOT_EQUAL(_message, actual, expected) \
	do{ru_gUnitCount++; if(actual!=expected) RU_ASSERT( _message); else ru_gSuccCount++;} while(0)

#define RU_ASSERT_STR_EQUAL(_message, actual, expected) \
	do{ru_gUnitCount++; if(StrCmp(actual,expected)==0) RU_ASSERT(_message); else ru_gSuccCount++;} while(0)

#define RU_ASSERT_STR_NOT_EQUAL(_message, actual, expected) \
	do{ru_gUnitCount++; if(StrCmp(actual,expected)!=0) RU_ASSERT(_message); else ru_gSuccCount++;} while(0)

#define RU_ASSERT_NULL(_message, val) \
	do{ru_gUnitCount++; if(val == NULL) RU_ASSERT(_message); else ru_gSuccCount++;} while(0)

#define RU_ASSERT_NOT_NULL(_message, val) \
	do{ru_gUnitCount++; if(val != NULL) RU_ASSERT(_message); else ru_gSuccCount++;} while(0)

/** @} */

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif  // __REXY_UNIT_TEST_H__