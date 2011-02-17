/*
 * $Id: rexy_mem.c 9002 2005-11-03 03:32:48Z joshua $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */

/** @file */

/**
 * @defgroup memory Core Memory Service
 * These APIs are associated with Memory
 * @ingroup RexyECoreGroup
 * @{
 */

#include "rexy.h"

#ifdef __MEMORY_LEAK_CHECK__

typedef struct ALLOC_CHUCK_TYPE {
	int	StateID;
	int	ActiveWindow;
	int Address;
	int Size;
	int	SourceFileNameLength;
	char* SourceFileName;
	int  SourceLineNum;
	struct ALLOC_CHUCK_TYPE *PrevAllocatedChunk;
	struct ALLOC_CHUCK_TYPE *NextAllocatedChunk;
} ALLOCATED_CHUNK;

typedef struct { 
	int UsedMemorySize;
	int MaxLeakCheckTrackLength;
	int UsedMemoryByLeakChecker;
	ALLOCATED_CHUNK *StartChunkPtr;
	ALLOCATED_CHUNK *EndChunkPtr;
} CHUNK_HISTORY;

CHUNK_HISTORY gChunkAllocHistory = {0,};

static ALLOCATED_CHUNK *PrvFindChunkPtr(void *ptr);
static ALLOCATED_CHUNK *PrvMakeNewChunkPtr(void);
static void PrvFreeChunkPtr(ALLOCATED_CHUNK *ptr);
static void PrvEraseChunkPtr(ALLOCATED_CHUNK *ptr);
static int	PrvDumpLeakInfo(void);

static bool g_rexy_memory_debug = TRUE;

#endif

/**
* @brief		Copy memory blocks
* @param[out]	dest	destination memory pointer
* @param[in]	src		source memory pointer
* @param[in]	size	size
* @return		returns the value of dest
* @remarks		Wrapper function for memcpy in the standard C library. 
* @see			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void* MemCopy(void* dest, const void* src, unsigned int size)
{
    return RalMemCpy( dest, src, size );
}

/**
* @brief		Sets buffers with a specified character.
* @param[out]	dest	pointer to destination
* @param[in]	value	character to set
* @param[in]	size	number of characters
* @return		returns the value of dest
* @remarks		Wrapper function for memset in the standard C library.
* @see			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void* MemSet(void* dest, int value, unsigned int size)
{
    return RalMemSet(dest, value, size);
}


void* MemReAlloc(void* ptr, size_t size)
{
#ifdef __MEMORY_LEAK_CHECK__
	{
		int	oldSize;
		ALLOCATED_CHUNK *ChunkPtr;
		ChunkPtr = PrvFindChunkPtr( ptr );

		if ( ChunkPtr )
		{
			oldSize = ChunkPtr->Size;
			ChunkPtr->Size = size;
			gChunkAllocHistory.UsedMemorySize += (size - oldSize);
		}
	}
#endif    
	return RalRealloc(ptr, size);
}
/**
* @brief		Allocate memory blocks
* @param[in]	size	memory byte size
* @return		allocated memory pointer
* @remarks		Wrapper function for malloc in the standard C library. 
*				Returns NULL if it fails to allocate the required memory.
* @see			
* @if HISTORY
*				06-Jan-2006 madholic	Initiated 
*				23-Aug-2006	Joshua		Added leak checker
* @endif
*/
void* mem_alloc(unsigned int size, const char* name, int line)
{
	void *ptr;

	ptr = RalMemAlloc(size);

#ifdef __MEMORY_LEAK_CHECK__

	if ( g_rexy_memory_debug )
	{
		ALLOCATED_CHUNK *ChunkPtr;;
		ChunkPtr = PrvMakeNewChunkPtr();

		ChunkPtr->StateID = StaGetCurrentStateID();
		ChunkPtr->ActiveWindow = (int)WinGetSelection();
		ChunkPtr->Address = (int)ptr;
		ChunkPtr->Size	= size;
		
		if ( name )
		{
			ChunkPtr->SourceFileNameLength=StrLen(name);
			ChunkPtr->SourceFileName = RalMemAlloc( ChunkPtr->SourceFileNameLength + 1);
			ASSERT(ChunkPtr->SourceFileName);
			StrCpy(ChunkPtr->SourceFileName,name);
			ChunkPtr->SourceLineNum =line;
			gChunkAllocHistory.UsedMemoryByLeakChecker += ChunkPtr->SourceFileNameLength+1;
		}
		else
		{
			ChunkPtr->SourceFileNameLength=0;
			ChunkPtr->SourceFileName = 0;
			ChunkPtr->SourceLineNum = 0;
		}
		gChunkAllocHistory.UsedMemorySize += size;
	}

#endif
    return ptr;
}

/**
* @brief		Free memory blocks
* @param[in]	*ptr	memory pointer to free
* @return		void
* @remarks		Wrapper function for free in the standard C library
* @see			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
*				24-Aug-2006	Joshua	 MLC imported
* @endif
*/
void MemFree(void* ptr)
{
    if (!ptr) return;
	RalMemFree( ptr );
 
#ifdef __MEMORY_LEAK_CHECK__
	{
		ALLOCATED_CHUNK *ChunkPtr;
		ChunkPtr = PrvFindChunkPtr( ptr );
		
		if ( ChunkPtr )
		{
			gChunkAllocHistory.UsedMemorySize -= ChunkPtr->Size;
			PrvEraseChunkPtr(ChunkPtr);
		}
	}
#endif
}

/** 
 * @brief	Initialize memory leak check info
 * @return	The size of previously allocated memory
 * @remark	System use only
 * @ifdef HISTORY
 *		18-May-2006	Joshua	Created
 * @endif
 */
int MemInitLeakInfo(void)
{
#ifdef __MEMORY_LEAK_CHECK__
	int i;
	int PrevAllocatedMemorySize=0;	
	ALLOCATED_CHUNK *CurrentChunkPtr;
	ALLOCATED_CHUNK	*NextChunkPtr;
	
	// Save previous info to return
	PrevAllocatedMemorySize = gChunkAllocHistory.UsedMemorySize;	

	// Free previously locked chunks if any
	if ( gChunkAllocHistory.MaxLeakCheckTrackLength )
	{
		CurrentChunkPtr = gChunkAllocHistory.StartChunkPtr;

		i = 0;

		while ( i < gChunkAllocHistory.MaxLeakCheckTrackLength -1 )
		{
			NextChunkPtr = CurrentChunkPtr->NextAllocatedChunk;
			RalMemFree( CurrentChunkPtr );
			CurrentChunkPtr = NextChunkPtr;
			i++;
		}
		RalMemFree( CurrentChunkPtr );
	}
	 
	// initializing
	gChunkAllocHistory.UsedMemorySize=0;
	gChunkAllocHistory.UsedMemoryByLeakChecker=0;
	gChunkAllocHistory.MaxLeakCheckTrackLength=0;
	gChunkAllocHistory.StartChunkPtr=0;
	gChunkAllocHistory.EndChunkPtr=0;

	TrcPrint("Memory leak checker has been initialized.\n");
	return PrevAllocatedMemorySize;
#else
	TrcPrint("Memory leak checker is not included in this binary.\n");
#endif
}
/** 
 * @brief	Dump currently allocated memory info
 * @remark  Created for application developers
 * @return	Memory leak amount in bytes
 * @ifdef HISTORY
 *		18-May-2006		Joshua	Created
 * @endif
 */
int	MemDumpLeakInfo(void)
{
#ifdef __MEMORY_LEAK_CHECK__
	int UsedMemory=0;
	
	TrcPrint("========  Current Memory Info  Start ========\n");		
	UsedMemory = PrvDumpLeakInfo();
	TrcPrint("========  Current Memory Info   End  =========\n");	
	return UsedMemory;
#else
	TrcPrint("Memory leak checker is not included in this binary.\n");
#endif
}


#ifdef __MEMORY_LEAK_CHECK__

static int PrvDumpLeakInfo()
{	
	ALLOCATED_CHUNK *ChunkPtr;
	TrcPrintF("Consumed memory by leak checker : %d bytes\n",gChunkAllocHistory.UsedMemoryByLeakChecker);

	if ( gChunkAllocHistory.UsedMemorySize )
	{
		int i;		
		TrcPrintF("[WARNING] Unfreed memory chunks detected (%d bytes)!!!!!!!!\n",gChunkAllocHistory.UsedMemorySize);

		ChunkPtr = gChunkAllocHistory.StartChunkPtr;
		
		for (i=0;i< gChunkAllocHistory.MaxLeakCheckTrackLength ; i++)
		{
			TrcPrintF("-Pointer:0x%x (state:%d, wnd:%d, size: %d) has not been freed yet.\n", 
				ChunkPtr->Address, 
				ChunkPtr->StateID,
				ChunkPtr->ActiveWindow,
				ChunkPtr->Size);

			if (ChunkPtr->SourceFileName)
			{
				TrcPrintF(" Source line:%d in file '%s'\n"
					,ChunkPtr->SourceLineNum
					,ChunkPtr->SourceFileName); 
			}
			ChunkPtr = ChunkPtr->NextAllocatedChunk;
		}
	}
	else
		TrcPrint("No memory leak found.\n");
	return gChunkAllocHistory.UsedMemorySize;
}

static ALLOCATED_CHUNK *PrvMakeNewChunkPtr()
{
	ALLOCATED_CHUNK *ptr;
	ALLOCATED_CHUNK *PrevChunkPtr;

	ptr = RalMemAlloc(sizeof( ALLOCATED_CHUNK ));
	ASSERT(ptr);
	
	PrevChunkPtr = gChunkAllocHistory.EndChunkPtr;

	if ( gChunkAllocHistory.EndChunkPtr )
		gChunkAllocHistory.EndChunkPtr->NextAllocatedChunk = ptr;
	else
		gChunkAllocHistory.StartChunkPtr = ptr;
	
	gChunkAllocHistory.EndChunkPtr = ptr;
	gChunkAllocHistory.EndChunkPtr->PrevAllocatedChunk = PrevChunkPtr;
	gChunkAllocHistory.EndChunkPtr->NextAllocatedChunk = 0;
	gChunkAllocHistory.EndChunkPtr->SourceFileName = 0;

	gChunkAllocHistory.UsedMemoryByLeakChecker += sizeof( ALLOCATED_CHUNK );
	gChunkAllocHistory.MaxLeakCheckTrackLength++;

	return ptr;
}
// Free any linked memory in chunk ptr
// This function is a subsidiary of 'PrvEraseChunkPtr'
// Created by Joshua
static void PrvFreeChunkPtr(ALLOCATED_CHUNK *ptr)
{
	if ( ptr->SourceFileName )
	{
		gChunkAllocHistory.UsedMemoryByLeakChecker -= ( ptr->SourceFileNameLength +1 );
		RalMemFree( ptr->SourceFileName );
	}
	RalMemFree( ptr );	
	gChunkAllocHistory.UsedMemoryByLeakChecker -= sizeof( ALLOCATED_CHUNK );
}
static void PrvEraseChunkPtr(ALLOCATED_CHUNK *ptr)
{
	// Here,we have to chain the remained ones after deletion
	ALLOCATED_CHUNK *PrevPtr;
	ALLOCATED_CHUNK *NextPtr;

	PrevPtr = ptr->PrevAllocatedChunk;
	NextPtr = ptr->NextAllocatedChunk;

	if ( PrevPtr ) PrevPtr->NextAllocatedChunk = NextPtr;
	if ( NextPtr ) NextPtr->PrevAllocatedChunk = PrevPtr;
	
	if ( PrevPtr )
	{
		if ( !NextPtr ) 	// last one
		{
			gChunkAllocHistory.EndChunkPtr = PrevPtr;
		}
	}
	else
	{
		if ( NextPtr )	// it was the first one
		{
			gChunkAllocHistory.StartChunkPtr = NextPtr;
		}
		else
		{
			gChunkAllocHistory.StartChunkPtr = 0;
			gChunkAllocHistory.EndChunkPtr = 0;
		}
	}

	PrvFreeChunkPtr( ptr );
	gChunkAllocHistory.MaxLeakCheckTrackLength--;

	ASSERT(gChunkAllocHistory.MaxLeakCheckTrackLength>=0);
}
static ALLOCATED_CHUNK *PrvFindChunkPtr(void *ptr)
{
	int i;
	ALLOCATED_CHUNK *ChunkPtr = gChunkAllocHistory.StartChunkPtr;

	for ( i = 0; i< gChunkAllocHistory.MaxLeakCheckTrackLength ; i++ )
	{
		if ( ChunkPtr->Address == (int)ptr ) return ChunkPtr;
		ChunkPtr = ChunkPtr->NextAllocatedChunk;
	}
	return 0;
}

#endif

/** @} */ // End of Memory
