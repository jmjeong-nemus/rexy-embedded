/*
 * $Id: rexymem.c 435 2005-02-16 08:25:12Z longdal $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * memory management routine.
 *
 */
#include "rexy.h"
#include "rexy_halinterface.h"
#include "rexy_clib.h"

typedef struct rexymem_pool_info_s
{
    unsigned short     size;     /* Size of buffer */
    unsigned short     total_num;/* Total number of buffers of this size */
    unsigned short     h_watermark; /* High watermark not to exceed. */
    unsigned short     num_used; /* Number of buffers being used. */
} rexymem_pool_info_s_type;

#define    BPOOL_SIZE_00    16
#define    BPOOL_SIZE_01    32
#define    BPOOL_SIZE_02    64
#define    BPOOL_SIZE_03    160
#define    BPOOL_SIZE_04    320
#define    BPOOL_SIZE_05    2200

#define    BPOOL_NUM_00     200
#define    BPOOL_NUM_01     600
#define    BPOOL_NUM_02     600
#define    BPOOL_NUM_03     200
#define    BPOOL_NUM_04     20
#define    BPOOL_NUM_05     2

#define    BPOOL_HIGH_WM_00 250
#define    BPOOL_HIGH_WM_01 700
#define    BPOOL_HIGH_WM_02 700
#define    BPOOL_HIGH_WM_03 250
#define    BPOOL_HIGH_WM_04 32
#define    BPOOL_HIGH_WM_05 2

/* Pool information table. This table is used at init time
** to create buffer pools. Buffer pools should be in the order of
** increasing sizes. 
*/
static rexymem_pool_info_s_type rexymem_pool_info[] =
    {
        /* id */  /* size */        /* total_num */ /* high wmark */  /*used*/
        /* 0 */   {BPOOL_SIZE_00,   BPOOL_NUM_00,   BPOOL_HIGH_WM_00,  0},
        /* 1 */   {BPOOL_SIZE_01,   BPOOL_NUM_01,   BPOOL_HIGH_WM_01,  0},
        /* 2 */   {BPOOL_SIZE_02,   BPOOL_NUM_02,   BPOOL_HIGH_WM_02,  0},
        /* 3 */   {BPOOL_SIZE_03,   BPOOL_NUM_03,   BPOOL_HIGH_WM_03,  0},
        /* 4 */   {BPOOL_SIZE_04,   BPOOL_NUM_04,   BPOOL_HIGH_WM_04,  0},
        /* 5 */   {BPOOL_SIZE_05,   BPOOL_NUM_05,   BPOOL_HIGH_WM_05,  0},
    };


/* Max number of buffer pools */
#define REXYMEM_MAX_NBPOOLS \
    (sizeof(rexymem_pool_info) / sizeof(rexymem_pool_info[0]))       

/* Buffer header, used to store the pool id or the pointer to
** the next buffer item */
#define REXYMEM_BUF_HEADER sizeof(int)

/* Total number of bytes for the pool. Used to static allocate
** the memory as the heap. */
#define REXYMEM_DYN_MEMORY \
    (   (BPOOL_SIZE_00 + REXYMEM_BUF_HEADER) * ( BPOOL_NUM_00 ) \
     +                                                         \
        (BPOOL_SIZE_01 + REXYMEM_BUF_HEADER) * ( BPOOL_NUM_01 ) \
     +                                                         \
        (BPOOL_SIZE_02 + REXYMEM_BUF_HEADER) * ( BPOOL_NUM_02 ) \
     +                                                         \
        (BPOOL_SIZE_03 + REXYMEM_BUF_HEADER) * ( BPOOL_NUM_03 ) \
     +                                                         \
        (BPOOL_SIZE_04 + REXYMEM_BUF_HEADER) * ( BPOOL_NUM_04 ) \
     +                                                         \
        (BPOOL_SIZE_05 + REXYMEM_BUF_HEADER) * ( BPOOL_NUM_05 ))

/* Structure which holds buffer pool information.
*/
typedef struct rexymem_bpool {
    unsigned int bpsize;             /* Size of this pool */
    int    *bpnext;                  /* Pointer to free buffer list */
} rexymem_bpool_type;

/* Memory pools storage allocation */
//static char  rexymem_heap[REXYMEM_DYN_MEMORY];
static char*  rexymem_heap = NULL;

/* Allocation index for heap */
static int   alloc_idx = 0;

/* Buffer pool table */
static rexymem_bpool_type bptab[ REXYMEM_MAX_NBPOOLS ];

/* Current number of pools
*/
static int    nbpools=0;


/*===========================================================================

FUNCTION rexymem_GETMEM

DESCRIPTION
  This function is used to get RAM space. This interface is done so that
  if REX supports getmem() in future the interfaces will remain same.
  Currently it just returns the pointer to statically allocated buffer 
  space.

RETURN VALUE
  The address of the memory block available.

DEPENDENCIES

===========================================================================*/

static char *rexymem_getmem( unsigned int size )
{
    char *buffer = NULL;

    if ( (alloc_idx + size) <= REXYMEM_DYN_MEMORY )
    {
        /* return pointer to statically allocated buffer space */
        buffer = &rexymem_heap[alloc_idx];
        alloc_idx += size;
    }
    return buffer;

} /* End rexymem_getmem( ) */

/*===========================================================================

FUNCTION rexymem_make_pool

DESCRIPTION
  This function is used to make Dynamic buffer pools, to be used by the
  Security Services.

RETURN VALUE
  TRUE, if the memory pool is made successfully.
  FALSE, if it runs out of the free memory space.

DEPENDENCIES

===========================================================================*/

static bool rexymem_make_pool(unsigned int size, int num)
{
    int    poolid;
    char   *where;

    size = (size + 3) & ~3;  /* Round up to 4 byte boundary */

    /* Now get memory for this pool. sizeof(int) is added so that enough space
    ** is allowed for storing pool id.
    */
    if (  (where = rexymem_getmem( (size + REXYMEM_BUF_HEADER)*num) ) == NULL )
    {
        /* Ran out of memory, return error */
        return FALSE;
    }

    /* memory allocation was a Success, go ahead and create a pool
    ** of buffers.
    */

    /* Increment the num of pools being currently used.
    */
    poolid = nbpools++;

    /* Point to the first available buffer in this pool.
    */
    bptab[poolid].bpnext = (int *)where;

    /* Indicate the size of buffer allocated.
    */
    bptab[poolid].bpsize = size;

    /* Add size so that pool id can be stored along with the buffer.
    */
    size += sizeof(int);

    /* Now link all the buffers together, last buffer will be linked
    ** at the end of this loop.
    */
    for (num--; num > 0; num--, where += size )
    {
        *( (int *) where) = size +(int)where;
    }
    *( (int *) where) = (int)NULL;   /* Last buffer in the pool */

    return TRUE;

} /* End rexymem_make_pool( ) */


/*===========================================================================

FUNCTION rexymem_POOL_INIT

DESCRIPTION
  This function is used to initialize the Dynamic buffer pools. It creates
  the memory pool based on the different pool id (size) and number of 
  buffers for this pool id, as defined in rexymem_pool_info. As such, these
  numbers should be tuned to achieve minimum yet sufficient memory usage.

RETURN VALUE
  None

DEPENDENCIES
  Called only once from SEC_TASK

===========================================================================*/

void rexymem_pool_init ( void )
{
    int i;
    unsigned long tmp;
    bool ret_val;


    /* chemistz, 2005-06-22	*/
    /*
    	(I think...)
    	in SHMV sometimes, declaration and initialization can not be performed in same time.
    	So I added an (additional) initialization.
    */  

    rexymem_heap = NULL;


    if ( rexymem_heap )
    {
        TRACE(TR_RexyCore, TR_LEVEL3, ("rexymem_pool_init had been called already"));
        ASSERT(FALSE);
        return;
    }

//    rexymem_heap = AS_MALLOC(REXYMEM_DYN_MEMORY);
    if ( !rexymem_heap )
    {
        TRACE(TR_RexyCore, TR_LEVEL3, ("can't alloc memory for rexymem_heap"));
        TRACE(TR_RexyCore, TR_LEVEL3, ("for rexymem_heap, we need %d bytes at least", REXYMEM_DYN_MEMORY));
        TRACE(TR_RexyCore, TR_LEVEL3, ("system halted"));
        ASSERT(FALSE);
        while(1);
    }

    tmp = REXYMEM_DYN_MEMORY;
    for ( i=0; i < REXYMEM_MAX_NBPOOLS; i++ )
    {
        /* check if the size or num of buffer is > 0 */
        if ( (rexymem_pool_info[i].total_num != 0) &&
                (rexymem_pool_info[i].size != 0) )
        {
            /* Now make pools of size and num specified in pool information data
            ** structure. */
            if ( (ret_val = rexymem_make_pool((int)rexymem_pool_info[i].size,
                                              (int)rexymem_pool_info[i].total_num))
                    == FALSE )
            {
                break;  /* Break out, can't make buffer pools */
            } /* If make_pool() failed to create pool */
        } /* End if number and size of pool to create != 0 */
    } /* End for all the buffer pools to be created */
    return;

} /* End pdsm_pool_init() */

void rexymem_pool_deinit(void)
{
    if ( rexymem_heap )
    {
//        AS_FREE( rexymem_heap );
        rexymem_heap = NULL;
    }
}

/*===========================================================================

FUNCTION rexymem_MALLOC

DESCRIPTION
    This function is used for dynamic buffer allocation. It searches
    in the buffer pool corresponding to the size requested. If no more
    free space in this pool id, it searches for the next higher sized
    memory pool. At the most two pools are searched for a free buffer
    to avoid memory waistage.

RETURN VALUE
    If successful, a pointer to the allocated memory block is returned.
    Otherwise, a NULL pointer is returned.

DEPENDENCIES

===========================================================================*/

void *rexymem_malloc( unsigned int size )
{
    int i;

    rexymem_bpool_type   *pool_ptr;  /* pointer to pool table */
    //  char entry         = NULL;      /* Indicate failure to get free buffer */
    unsigned int entry         = 0;      /* Indicate failure to get free buffer */
    int  *buffer       = NULL;      /* Buffer pointer to return to user */

    /*-----------------------------------------------------------------------*/
    /* Do sanity check */
    if (size == 0)
    {
        return ((void *)NULL);
    }

    for (i=0, pool_ptr = &bptab[0]; i < nbpools; pool_ptr++,i++)
    {
        /* Check if pool buffer size fits the requested buffer size
        */
        if (pool_ptr->bpsize >= size)
        {
            /* Find a free buffer */
            if (pool_ptr->bpnext != NULL)
            {
#ifdef FEATURE_SEC_MEM_TUNING
                if ( rexymem_pool_info[i].h_watermark <=
                        rexymem_pool_info[i].num_used )
                {
                }
                rexymem_pool_info[i].num_used++;
#endif /* FEATURE_SEC_MEM_TUNING */

                buffer = pool_ptr->bpnext;
                pool_ptr->bpnext = (int *)*buffer; /* Remove this buffer and link
                                                   ** the next free buffer in Q.
                                                   */

                *buffer++ = i;  /* Store the poolid in allocated buffer */
                return ((void*)buffer);
            }
            else /* No free buffers in this pool */
            {
                /* If it fails to get buffer in next higher size
                ** pool, then we simply return with NULL pointer.
                */
                if (( i == nbpools - 1 ) || ( entry == size ))
                {
                    /* Failure in next higher pool */
                    //           MSG_ERROR("No free memory pool for size %d", size, 0, 0);
                    return ((void *)NULL);
                }
                entry = size;    /* Indicate that pool has been found
                                 ** and there are no free buffers in pool.
                                 */
            } /* End else  if(bpnext!=NULL) */
        }/* End if pool buffer size > size requested */

    }/* End For all the buffer pools */

    return ((void *)buffer);

} /* End rexymem_malloc() */


/*===========================================================================

FUNCTION rexymem_FREE

DESCRIPTION
  This function is used for deallocating the memory. It also checks for 
  the possible memory corruptions.

RETURN VALUE
  None

DEPENDENCIES

===========================================================================*/
void rexymem_free( void *buf )
{
    int poolid;
    int *buffer;

    /* Check if poolid is proper in user given buffer pointer.
    */
    if ( buf == NULL)
    {
        return;
    }

    buffer = (int *)buf;

    /* extract the poolid from buffer pointer.
    */
    poolid = *(--buffer);

    /* If there is an error in pool id return error.
    */
    if( poolid < 0 || poolid >= nbpools )
    {
        /* Indicate an error here, this can happen only when user is
        ** trying to deallocate a wrong buffer or the pool id in buffer
        ** got corrupted.
        */
        return;
    }
    else
    {
        /* Now link the buffer back to the free pool.
        */
        *buffer = (int)bptab[poolid].bpnext;
        bptab[poolid].bpnext = buffer;

#ifdef FEATURE_SEC_MEM_TUNING
        rexymem_pool_info[poolid].num_used--;
#endif /* FEATURE_SEC_MEM_TUNING */

        return;
    } /* End Else when the pool id is proper. */

} /* End rexymem_free( ) */


/*===========================================================================

FUNCTION rexymem_REALLOC

DESCRIPTION
  This function changes the size of the block pointed to by ptr to
  size bytes and returns a pointer to the (possibly moved) block. 
  The contents are unchanged up to the lesser of the new and old sizes.
  If ptr is a null pointer, this function behaves like malloc() for 
  the specified size. If size is zero and ptr is not a null pointer, 
  the object it points to is freed.

RETURN VALUE
  If successful, a pointer to the re-allocated memory block is returned.
  Otherwise, a NULL pointer is returned.

DEPENDENCIES

===========================================================================*/


void *rexymem_realloc( void *buf, unsigned int size )
{
    int poolid;
    void *buffer        = buf;   /* Buffer pointer to return to user */

    /* ----------------------------------------------------------------------*/

    /* If the buf is NULL, rexymem_realloc behaves likes the rexymem_malloc */
    if ( buf == NULL)
    {
        buffer = rexymem_malloc(size);
    }
    else
    {
        if (size == 0)
        {
            /* If size is zero and ptr is not a null pointer, just free
            ** the block
            */
            rexymem_free(buf);
            buffer = (void *)NULL;
        }
        else
        {

            /* extract the poolid from buffer pointer.
            */
            poolid = *((int *)buf - 1);

            /* If there is an error in pool id return error.
            */
            if( poolid < 0 || poolid >= nbpools )
            {
                /* Indicate an error here, this can happen only when user is
                ** trying to deallocate a wrong buffer or the pool id in buffer
                ** got corrupted.
                */
                buffer = (void *)NULL;
            }
            //      else if ( ((poolid == 0) &&
            //                 (size < bptab[poolid].bpsize))
            //               ||
            //                ((poolid > 0) &&
            //                 (bptab[poolid-1].bpsize < size) &&
            //                 (size <= bptab[poolid].bpsize))
            //              )
            //      {
            //        /* If the size fits in the current block, and there is no better
            //        ** way to alloc the memory, just return the current pointer.
            //        */
            //        buffer = buf;
            //      }
            else if (size <= bptab[poolid].bpsize)
            {
                /* If the size fits in the current block, just return the current
                ** pointer.
                */
                buffer = buf;
            }
            else
            {
                /* Do malloc first, and copy the contents from the old block to the
                ** new block, then free the old block. If the new block is shortened, 
                ** bytes are discarded off the end. If the block is lengthened, the 
                ** new bytes added are not initialized and will have garbage values.
                */
                buffer = rexymem_malloc(size);

                if (buffer != NULL)
                {
                    (void)MemCopy((void *)buffer, (const void *)buf,
                                    ((size < bptab[poolid].bpsize) ? size : bptab[poolid].bpsize));
                    rexymem_free(buf);
                }
            }
        }
    }

    return (buffer);
} /* End rexymem_realloc() */


/*===========================================================================

FUNCTION rexymem_REMALLOC

DESCRIPTION
  This function frees the old block, and allocates a new block of memory.

RETURN VALUE
  If successful, a pointer to the re-malloc'd memory block is returned,
  Otherwise, a NULL pointer is returned.

DEPENDENCIES

===========================================================================*/

void *rexymem_remalloc( void *buf, unsigned int size )
{
    if (buf)
    {
        rexymem_free(buf);
    }

    return (rexymem_malloc(size));

} /* End rexymem_remalloc() */
