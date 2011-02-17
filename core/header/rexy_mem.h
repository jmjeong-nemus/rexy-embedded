/*
 * $Id: rexy_mem.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Device dependent한 함수 및 상수가 상수가 정의되어 있다.
 *
 */

#ifndef __REXY_MEM_H__
#define __REXY_MEM_H__

#define __MEMORY_LEAK_CHECK__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */
    void* MemCopy(void* dest, const void* src, unsigned int size);
    void* MemSet(void* dest, int value, unsigned int size);
    void* MemReAlloc(void* ptr, size_t size);
	#define MemAlloc(size)	mem_alloc(size,TrcGetShortFileName(__FILE__),__LINE__)
	void* mem_alloc(unsigned int size, const char* name, int line);
	void MemFree(void* ptr);

	int	MemDumpLeakInfo(void);
	int MemInitLeakInfo(void);

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif//__REXY_MEM_H__
