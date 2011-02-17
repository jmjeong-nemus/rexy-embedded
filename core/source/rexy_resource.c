/*
* $Id: rexy_resource.c 435 2005-12-15 11:40:32Z klenui $
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
*
* This file is a part of Rexy Embedded 
*
*/

/** @file */

/**
* @defgroup resource Core Resource Service
* Font and Resource Manager
*	Font Manager
*     ------------------
*	Resource Manager
*     ------------------
*	Font Data
* @ingroup RexyECoreGroup
* @{
*/

#include "rexy.h"

/*======================================================================*/
/* Declarations								*/
/*======================================================================*/
/*
======================================================================
 Public APIs								
======================================================================
int	RscGetFontWidth(FntID id, int code);
bool RscGetFontImage(FntID id, byte* dst, int dstSize, int code );
int RscGetFontHeight(FntID id);
int RscGetFontByteWidth(FntID id);

int RscGetBmpWidth(BmpID id);
int RscGetBmpHeight(BmpID id);
bool RscGetBmpImage(BmpID id, byte* dst, int dstSize);
const UNITCL* RscGetRawImage(BmpID id);

bool RscCheckResourceFont(void);
bool RscCheckResourceIcon(void);
bool RscCheckResourceImage(void);*/


//Added here by Joshua
static FntType *FntData[MAX_FONT_RESOURCE_NUM]={0,};
static BmpType *BmpData;

/*======================================================================*/
/* Private								*/
/*======================================================================*/
/*======================================================================*/
/* Module Variables							*/
/*======================================================================*/
/*======================================================================*/
/* Implementation							*/
/*======================================================================*/

/** 
 * @brief	Set Bitmap group
 * @param[in]	BmpArrayPtr		The pointer to an array which contains BmpType information
 * @retval	RxErrNone						on success
 * @retval	RxErrRscClass | enInvalidParam	on failure
 * @if HISTORY
 *		16-May-2006	Joshua	Created
 * @endif
 */
RxErr RscSetBitmapGroup( BmpType *BmpArrayPtr )
{
	if ( !BmpArrayPtr ) return ( RxErrRscClass | enInvalidParam );
	BmpData = BmpArrayPtr;
	
	return RxErrNone;
}

/** 
 * @brief	Set Font resource
 * @param[in]	id			font ID
 * @param[in]	FntPtr		pointer to FntType structure
 * @retval	RxErrNone						on success
 * @retval	RxErrRscClass | enInvalidParam	on failure
 * @remark	To free previous font set in a specified fontId, set FntPtr as NULL
 * @if HISTORY
 *		15-May-2006	Joshua	Created
 * @endif
 */
RxErr RscSetFont(FntID id,FntType *FntPtr)
{
	RxErr result=RxErrNone;

	if ( id < 0 || id > MAX_FONT_RESOURCE_NUM )
		result = ( RxErrRscClass | enInvalidParam );
	else
		FntData[id]=FntPtr;
	
	return result;
}

/**
* @brief		Get font width of a single chracter.
* @param[in]	id		font ID
* @param[in]	code	code of character(ascii or mbcs, etc..)
* @return		font width. if there is no font image for the code, return a half of font size.
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int RscGetFontWidth(FntID id, int code)
{
    const FntType* fnt;

    fnt = FntData[id];

    if ( fnt->widthTable )
    {
        if ( code - fnt->minCode > fnt->maxCode ) return 0;
        else return fnt->widthTable[code-fnt->minCode];
    }
    else {
        return fnt->width;
    }
}

/**
* @brief		Get current font height.
* @param[in]	id		font ID
* @return		font height
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int RscGetFontHeight(FntID id)
{
    const FntType* fnt;

    fnt = FntData[id];
    return fnt->height;
}

/**
* @brief		Get the byte value that one character has hold
* @param[in]	id		font ID
* @return		width by byte
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int RscGetFontByteWidth(FntID id)
{
    const FntType* fnt;

    fnt = FntData[id];
    return fnt->byteWidth;
}

/**
* @brief		Get font image of a single character. 
* @param[in]	id			font ID
* @param[out]	dst			a pointer of buffer to which the font image will be written.
* @param[in]	dstSize		font size(height)
* @param[in]	code		code of chracter(ascii or mbcs etc..)
* @retval		TRUE if font image for the code exists.
* @retval		FALSE if else. 
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
bool RscGetFontImage(FntID id, byte* dst, int dstSize, int code )
{
    const FntType* fnt;
    int index;
    int sizeFntImg;

    ASSERT(dst);

    fnt = FntData[id];
    index = code - fnt->minCode;
    if ( index > fnt->maxCode ) return FALSE;

    sizeFntImg = fnt->height * fnt->byteWidth;
    if ( dstSize < sizeFntImg ) return FALSE;

	MemCopy( dst, (byte*)fnt->img + (sizeFntImg * index),  sizeFntImg );

    return TRUE;
}

/**
* @brief		Get bitmap width
* @param[in]	id		bitmap ID
* @return		bitmap width
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int RscGetBmpWidth(BmpID id)
{   
	return BmpData[id].width;
    
}

/**
* @brief		Get bitmap height
* @param[in]	id		bitmap ID
* @return		bitmap height
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int RscGetBmpHeight(BmpID id)
{
	//ASSERT( id >= 0 && id < BmpIDMaxNum );
	return BmpData[id].height;
}

/**
* @brief		Get bitmap image
* @param[in]	id			bitmap ID
* @param[out]	dst			The pointer that the image will be saved
* @param[in]	dstSize		The size of the destination buffer
* @return		Always return TRUE
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
bool RscGetBmpImage(BmpID id, byte* dst, int dstSize)
{
	int sizeBmpImg;
	
	ASSERT(dst);
    
	sizeBmpImg = BmpData[id].height * BmpData[id].width * sizeof(UNITCL);
	if ( dstSize < sizeBmpImg ) return FALSE;
	
	MemCopy( dst, (byte*)BmpData[id].dataPtr, sizeBmpImg );
	
	return TRUE;
}

/**
* @brief		Get bitmap image.
* @param[in]	id		bitmap ID
* @return		The pointer of the image data
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
const UNITCL* RscGetRawImage(BmpID id)
{
	return BmpData[id].dataPtr;
}

/**
* @brief		Check whether font resources are readable or not.
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
bool RscCheckResourceFont(void)
{
    // we will use static font resource
    // so the return value must be always TRUE.
    return TRUE;
}

/**
* @brief		
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
bool RscCheckResourceIcon(void)
{
    return TRUE;
}

/**
* @brief		
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
bool RscCheckResourceImage(void)
{
    return TRUE;
}

/**
* @brief		Uncompress a block of data using an RLE decoder.
* @param[in]	in			Input (compressed) buffer.
* @param[out]	out			Output (uncompressed) buffer. This buffer must be large enough to hold the uncompressed data.
* @param[in]	insize		Number of input bytes.
* @return		void
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void RscRLEUncompress( unsigned char *in, unsigned char *out, unsigned int insize )
{
    unsigned char marker, symbol;
    unsigned int  i, inpos, outpos, count;

    /* Do we have anything to compress? */
    if( insize < 1 )
    {
        return;
    }

    /* Get marker symbol from input stream */
    inpos = 0;
    marker = in[ inpos ++ ];

    /* Main decompression loop */
    outpos = 0;
    do
    {
        symbol = in[ inpos ++ ];
        if( symbol == marker )
        {
            /* We had a marker byte */
            count = in[ inpos ++ ];
            if( count <= 2 )
            {
                /* Counts 0, 1 and 2 are used for marker byte repetition
                   only */
                for( i = 0; i <= count; ++ i )
                {
                    out[ outpos ++ ] = marker;
                }
            }
            else
            {
                if( count & 0x80 )
                {
                    count = ((count & 0x7f) << 8) + in[ inpos ++ ];
                }
                symbol = in[ inpos ++ ];
                for( i = 0; i <= count; ++ i )
                {
                    out[ outpos ++ ] = symbol;
                }
            }
        }
        else
        {
            /* No marker, plain copy */
            out[ outpos ++ ] = symbol;
        }
    }
    while( inpos < insize );
}

/**
* @brief		Load Bitmap data from ROMFS(partition 4). 
* @param[in]	id	Points to a null-terminated string that contains the name of the bitmap resource.
* @param[out]	bitmap		bitmap structure to receive bitmap resource
* @return		Error Code
* @remarks		Bitmap format is ASB.
*	
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int RscLoadBitmap(const BmpID id, RxBITMAP * bitmap)
{
  if ( !bitmap ) return (RxErrGdiClass|enInvalidParam);

  MemSet(bitmap, 0, sizeof(RxBITMAP));
  
  bitmap->width = RscGetBmpWidth(id);
  bitmap->height = RscGetBmpHeight(id);

  // TODO : joshua
  // Below paragraphs should be enahanced later 
  bitmap->bpp = 16;

  /*
  bmpSize = bitmap->width * bitmap->height * sizeof(UNITCL);
  bitmap->data = MemAlloc(bmpSize);
  if ( !bitmap->data ) return (RxErrGdiClass|enNotEnoughMem);

  if ( !RscGetBmpImage( id, bitmap->data, bmpSize ) )
    {
      MemFree(bitmap->data);
      return (RxErrGdiClass|enSystemError);
    }
  */
  bitmap->data = (void *)RscGetRawImage(id);

  return RxErrNone;
}



int RscLoadBitmapRGB565(const char * name, RxBITMAP * bitmap)
{
    int ret, fd,  addr, offset;
	long size;

    char filename[60];
    int len;
    int i;
	int UncompressedLen;

    //////////////////////////////////////////////////////////////////////////
    // RLE Compress
    unsigned char* bufIn;
    unsigned int bufOutSize=0;
    //////////////////////////////////////////////////////////////////////////

    //unsigned char buffer[16];
    unsigned short data;
    unsigned char * raw_data;

    StrCpy(filename, name);

    // check arguments.
    if ( StrLen(filename) > MAX_PATH_LEN)
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <LoadBitmapRGB565> filename is too long."));
        return -1;
    }
    if ( bitmap == NULL )
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> bitmap is NULL."));
        return -1;
    }

	fd = FsOpenFile(filename);
	
    if ( fd < 0 )
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> can't open file %s.", filename));
        return -1;
	}

	size = FsGetFileLength(fd);

	if( size < 0 )
	{
		return -1;
	}

    // do we need to add some verification process?
    // add it here.

    bufIn = (unsigned char*)MemAlloc(size);
    if ( bufIn == NULL )
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> can't AS_MALLOC data"));
        TRACE(TR_RexyCore, TR_LEVEL4, ("			alloc size : %d", size ));
        goto err;
    }

#ifdef ROMFS_REAL_SIMULATION
#ifndef F_REXY_ENV_PHONE
	Sleep( 0.00233433734 * size + 22 );
#endif	
#endif


	ret = FsReadFile(fd, bufIn, size);
    if ( ret <=  0)
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> can't read file"));
        TRACE(TR_RexyCore, TR_LEVEL4, ("			of file %s", filename ));
        TRACE(TR_RexyCore, TR_LEVEL4, ("romfs_read_direct_data returned %d", ret ));

        MemFree(bufIn);
        goto err;
    }


    raw_data = (unsigned char*)(bufIn);
    offset = 2;		// original size

    bufOutSize = DevToHost(raw_data + offset);
    offset += sizeof(int);	
	offset += sizeof(short);	// compression method

    bitmap->width = DevToHostShort(raw_data + offset);
    offset += sizeof(short);
    TRACE(TR_RexyCore, TR_LEVEL1, ("width : %d", bitmap->width));

    bitmap->height = DevToHostShort(raw_data + offset);
    offset += sizeof(short);
    TRACE(TR_RexyCore, TR_LEVEL1, ("height : %d", bitmap->height));

    bitmap->bTransparent = DevToHostShort(raw_data + offset);
    offset += sizeof(short);
    TRACE(TR_RexyCore, TR_LEVEL1, ("transparent : %d", bitmap->bTransparent ));
	offset += sizeof(short);	//transparent color

	//bitmap->bpp = DevToHostShort(raw_data + offset);
	bitmap->bpp = 16;
	offset += sizeof(short);

#ifdef MEMORY_PROFILING    
    SnapHeapSize();
#endif
    //////////////////////////////////////////////////////////////////////////
    // RLE_Compress
    TRACE(TR_RexyCore, TR_LEVEL1, ("Request alloc [%d]", bufOutSize));
    bitmap->data = MemAlloc((unsigned int)bufOutSize);
    if (!bitmap->data) {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> can't AS_MALLOC data"));
        TRACE(TR_RexyCore, TR_LEVEL4, ("			alloc size : %d", bufOutSize ));
        // while (1)
        // ;
        goto err;
    }

    TRACE(TR_RexyCore, TR_LEVEL1, ("Allocation Successful"));
#ifdef MEMORY_PROFILING
    PrintHeapDiff("LoadResource", "_LoadResource");
#endif

    TRACE(TR_RexyCore, TR_LEVEL1, ("Before RGB565 Uncompress"));
    UncompressedLen = RGB565Uncompress((unsigned char*)(bufIn+offset), (unsigned char*)(bitmap->data), (unsigned int)(size-offset));
    TRACE(TR_RexyCore, TR_LEVEL1, ("After RGB565_Uncompress"));

    len = bitmap->width * bitmap->height * sizeof(UNITCL);

	ASSERT (UncompressedLen == len);

    if(bufIn)
    {
        MemFree(bufIn);
        bufIn = NULL;
    }

    // read palette here if it is required.

    //////////////////////////////////////

    // job done. close file
    TRACE(TR_RexyCore, TR_LEVEL1, ("loading file %s success.", filename));
    return len;

err :
    return -1;
}



static int RscLoadBitmapRLE(const char * name, RxBITMAP * bitmap)
{
    int ret, addr, offset, fd;
	long size;
    char filename[60];
    int len;
    int i;

    //////////////////////////////////////////////////////////////////////////
    // RLE Compress
    unsigned char* bufIn;
    unsigned int bufOutSize=0;
    //////////////////////////////////////////////////////////////////////////

    //unsigned char buffer[16];
    unsigned short data;
    unsigned char * raw_data;


    StrCpy(filename, name);
    //    AS_StrCat(filename, ".asb");		// madholic

    // check arguments.
    if ( StrLen(filename) > MAX_PATH_LEN)
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> filename is too long."));
        return -1;
    }
    if ( bitmap == NULL )
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> bitmap is NULL."));
        return -1;
    }

	fd = FsOpenFile(filename);

    if ( fd < -1 )
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> can't open file %s.", filename));
        return -1;
    }

	size = FsGetFileLength(fd);

    // do we need to add some verification process?
    // add it here.

    bufIn = (unsigned char*)MemAlloc(size);
    if ( bufIn == NULL )
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> can't AS_MALLOC data"));
        TRACE(TR_RexyCore, TR_LEVEL4, ("			alloc size : %d", size ));
        goto err;
    }

#ifdef ROMFS_REAL_SIMULATION
#ifndef F_REXY_ENV_PHONE
	Sleep( 0.00233433734 * size + 22 );
#endif	
#endif

    // now read the contents.
	ret = FsReadFile(fd, bufIn, size);

    if ( ret != NULL )
    {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> can't read file"));
        TRACE(TR_RexyCore, TR_LEVEL4, ("			of file %s", filename ));
        TRACE(TR_RexyCore, TR_LEVEL4, ("romfs_read_direct_data returned %d", ret ));

        MemFree(bufIn);
        goto err;
    }

    offset = 0;
    raw_data = (unsigned char*)(bufIn);

    bufOutSize = DevToHost(raw_data + offset);
    offset += sizeof(int);

    bitmap->width = DevToHostShort(raw_data + offset);
    offset += sizeof(short);
    TRACE(TR_RexyCore, TR_LEVEL1, ("width : %d", bitmap->width));

    bitmap->height = DevToHostShort(raw_data + offset);
    offset += sizeof(short);
    TRACE(TR_RexyCore, TR_LEVEL1, ("height : %d", bitmap->height));

    bitmap->bTransparent = DevToHostShort(raw_data + offset);
    offset += sizeof(short);
    TRACE(TR_RexyCore, TR_LEVEL1, ("transparent : %d", bitmap->bTransparent ));

	bitmap->bpp = 16;

#ifdef MEMORY_PROFILING    
    SnapHeapSize();
#endif
    //////////////////////////////////////////////////////////////////////////
    // RLE_Compress
    TRACE(TR_RexyCore, TR_LEVEL1, ("Request alloc [%d]", bufOutSize));
    bitmap->data = MemAlloc((unsigned int)bufOutSize);
    if (!bitmap->data) {
        TRACE(TR_RexyCore, TR_LEVEL4, ("Warning : <AS_LoadBitmap> can't AS_MALLOC data"));
        TRACE(TR_RexyCore, TR_LEVEL4, ("			alloc size : %d", bufOutSize ));
        // while (1)
        // ;
        goto err;
    }

    TRACE(TR_RexyCore, TR_LEVEL1, ("Allocation Successful"));
#ifdef MEMORY_PROFILING
    PrintHeapDiff("LoadResource", "_LoadResource");
#endif

    TRACE(TR_RexyCore, TR_LEVEL1, ("Before RLE Uncompress"));
    RLE_Uncompress((unsigned char*)(bufIn+offset), (unsigned char*)(bitmap->data), (unsigned int)(size-offset));
    TRACE(TR_RexyCore, TR_LEVEL1, ("After RLE_Uncompress"));
    //	Huffman_Uncompress(bufIn+offset, bitmap->data, size-offset, bufOutSize);

    //////////////////////////////////////////////////////////////////////////
    // bitmap->data is not include header information
    raw_data = (unsigned char *)(bitmap->data);
    for ( i = 0; i < bitmap->width * bitmap->height; ++i )
    {
        data = 0;
        data |= (unsigned short)(raw_data[2 * i] << 8);
        data |= (unsigned short)(raw_data[2 * i + 1]);
        *(unsigned short *)(&raw_data[2 * i]) = data;
    }

    len = bitmap->width * bitmap->height * sizeof(UNITCL);

    if(bufIn)
    {
        MemFree(bufIn);
        bufIn = NULL;
    }

    // read palette here if it is required.

    //////////////////////////////////////

    // job done. close file
    TRACE(TR_RexyCore, TR_LEVEL1, ("loading file %s success.", filename));
    return len;

err :
    return -1;
}


int RscLoadBitmapFile(const char *name, RxBITMAP * bitmap, int nComp)
{
	switch(nComp)
	{
	case 0: //RGB565_Compress
		RscLoadBitmapRGB565(name, bitmap);
		break;
	case 1:
		RscLoadBitmapRLE(name, bitmap);
		break;
	}
}

/**
* @brief		Destroy bitmap
* @param[in]	bitmap Bitmap to destroy.
* @return		Always returns 0.
* @remarks		Memories allocated by RscLoadBitmap and RscLoadDIBitmap are freed by this function.
*		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int RscDestroyBitmap(RxBITMAP * bitmap)
{
    // bitmap->data is not include header information
    if ( bitmap->data != NULL )
    {
        TRACE(TR_RexyCore, TR_LEVEL1, ("GdiDestroyBitmap : free memory"));

        MemFree((unsigned char*)(bitmap->data));
        bitmap->data = NULL;
    }
    else {
        TRACE(TR_RexyCore, TR_LEVEL4, ("GdiDestroyBitmap : Called with NULL memory ******************"));

        // ASSERT(1==0);
    }

    return 0;
}

/** 
 * @brief	Decompress image data which is optimized with RGB565 structure
 * @param[in]	in	The pointer to compressed data buffer
 * @param[in]	out	The pointer to buffer where uncompressed image is to be stored
 * @param[in]	insize	compressed data buffer size
 * @return		The size of uncompressed image data in byte.
 * @ifdef HISTORY
 *			10-May-2006	Joshua	Created
 * @endif
 */
static unsigned int RGB565Uncompress( unsigned char *in, unsigned char *out, unsigned int insize )
{
	unsigned int	inpos =0;
	unsigned int	outpos =0;
	unsigned char	TempByte;
	unsigned char	CompressionMark;
	unsigned short	quantity;
	unsigned short	i;
	unsigned char	UpperSymbol;
	unsigned char	LowerSymbol;

	CompressionMark = *in;
	inpos++;

	while ( inpos < insize )
	{
		TempByte = *(in+inpos);

		if ( TempByte == CompressionMark )
		{
			if ( *(in+inpos+1) & 0x80 ) // Normal compression
			{
				quantity = ( *(in+inpos+1) & 0x7F ) << 8;
				quantity += *(in+inpos+2);
				
				UpperSymbol = *(in+inpos+3);
				LowerSymbol = *(in+inpos+4);

				for ( i = 0; i< quantity; i++ )
				{
					*(out+outpos) = LowerSymbol;
					*(out+outpos+1) = UpperSymbol;

					outpos+=2;
				}
				inpos += 5;
			}
			else	// Compression Mark was data itself
			{
				quantity = *(in+inpos+1);

				UpperSymbol = TempByte;
				LowerSymbol = *(in+inpos+2);

				for ( i = 0; i< quantity; i++ )
				{
					*(out+outpos) = LowerSymbol;
					*(out+outpos+1) = UpperSymbol;

					outpos+=2;
				}
				inpos += 3;
			}
		}
		else
		{
			// Endian order is reversed
			*(out+outpos) = *(in+inpos+1);
			*(out+outpos+1) = *(in+inpos);

			inpos +=2;
			outpos +=2;
		}
	}

	return outpos;
}

/*************************************************************************
* RLE_Uncompress() - Uncompress a block of data using an RLE decoder.
*  in      - Input (compressed) buffer.
*  out     - Output (uncompressed) buffer. This buffer must be large
*            enough to hold the uncompressed data.
*  insize  - Number of input bytes.
*************************************************************************/

void RLE_Uncompress( unsigned char *in, unsigned char *out,
    unsigned int insize )
{
    unsigned char marker, symbol;
    unsigned int  i, inpos, outpos, count;

    /* Do we have anything to compress? */
    if( insize < 1 )
    {
        return;
    }

    /* Get marker symbol from input stream */
    inpos = 0;
    marker = in[ inpos ++ ];

    /* Main decompression loop */
    outpos = 0;
    do
    {
        symbol = in[ inpos ++ ];
        if( symbol == marker )
        {
            /* We had a marker byte */
            count = in[ inpos ++ ];
            if( count <= 2 )
            {
                /* Counts 0, 1 and 2 are used for marker byte repetition
                   only */
                for( i = 0; i <= count; ++ i )
                {
                    out[ outpos ++ ] = marker;
                }
            }
            else
            {
                if( count & 0x80 )
                {
                    count = ((count & 0x7f) << 8) + in[ inpos ++ ];
                }
                symbol = in[ inpos ++ ];
                for( i = 0; i <= count; ++ i )
                {
                    out[ outpos ++ ] = symbol;
                }
            }
        }
        else
        {
            /* No marker, plain copy */
            out[ outpos ++ ] = symbol;
        }
    }
    while( inpos < insize );
}
/** @} */ // End of Resource
