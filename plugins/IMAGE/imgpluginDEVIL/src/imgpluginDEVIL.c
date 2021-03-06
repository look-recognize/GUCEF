/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>             /* for memory utils */

#include "imgpluginDEVIL.h"     /* function prototypes for this plugin */

#ifndef __IL_H__
#include "il.h"                 /* main API include for the DevIL library */
#define __IL_H__
#endif /* __IL_H__ ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* make sure no structure packing is enabled, this allows us to use the structures directly */
#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif /* WIN32 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define IMGPLUGIN_DEVIL_MAYOR_VERSION       1
#define IMGPLUGIN_DEVIL_MINOR_VERSION       0
#define IMGPLUGIN_DEVIL_PATCH_VERSION       0
#define IMGPLUGIN_DEVIL_RELEASE_VERSION     1

/*---------------------------------------------------------------------------*/

static TIOAccess* currentResource = NULL;
static UInt32 codecCount = 0;

static char* supportedTypes[] = {

    #ifdef IL_TGA
    "tga", "vda", "icb", "vst",
    #endif  
    #ifdef IL_JPG
    "jpg", "jpe", "jpeg",
    #endif
    #ifdef IL_PNG
    "png",
    #endif
    #ifdef IL_BMP
    "bmp", "dib",
    #endif
    #ifdef IL_GIF
    "gif",
    #endif
    #ifdef IL_CUT
    "cut",
    #endif
    #ifdef IL_HDR
    "hdr",
    #endif
    #ifdef IL_ICO
    "ico",
    #endif
    #ifdef IL_JNG
    "jng",
    #endif
    #ifdef IL_LIF
    "lif",
    #endif
    #ifdef IL_MDL
    "mdl",
    #endif    
    #ifdef IL_MNG
    "mng", "jng",
    #endif 
    #ifdef IL_PCD
    "pcd",
    #endif
    #ifdef IL_PCX
    "pcx",
    #endif
    #ifdef IL_PIC
    "pic",
    #endif
    #ifdef IL_PIX
    "pix",
    #endif
    #ifdef IL_PNM
    "pbm", "pgm", "pnm", "ppm",
    #endif
    #ifdef IL_PSD
    "psd", "pdd",
    #endif
    #ifdef IL_PSP
    "psp",
    #endif    
    #ifdef IL_PXR
    "pxr",
    #endif
    #ifdef IL_SGI
    "sgi", "bw", "rgb", "rgba",
    #endif
    #ifdef IL_TIF
    "tif", "tiff",
    #endif
    #ifdef IL_WAL
    "wal",
    #endif 
    #ifdef IL_XPM
    "xpm",
    #endif                   
    NULL
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

ILvoid ILAPIENTRY
ilfCloseProc( ILHANDLE handle )
{
    TIOAccess* input = (TIOAccess*) handle;
    if ( NULL != input )
    {
        input->close( input );
    }
}

/*---------------------------------------------------------------------------*/

ILboolean ILAPIENTRY 
ilfEofProc( ILHANDLE handle )
{
    TIOAccess* input = (TIOAccess*) handle;
    if ( NULL != input )
    {
        return input->eof( input ) != 0 ? IL_TRUE : IL_FALSE;
    }
    return IL_TRUE;
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfGetcProc( ILHANDLE handle )
{
    TIOAccess* input = (TIOAccess*) handle;
    if ( NULL != input )
    {
        return input->getc( input );
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

ILHANDLE ILAPIENTRY 
ilfOpenProc( const ILstring ilstring )
{
    TIOAccess* input = currentResource;
    if ( NULL != input )
    {
        input->open( input );
    }
    
    return input;
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfReadProc( void* data, 
             ILuint size     , 
             ILuint elements , 
             ILHANDLE handle )
{
    TIOAccess* input = (TIOAccess*) handle;
    if ( NULL != input )
    {
        return input->read( input, data, size, elements );
    }
    return 0;        
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfSeekProc( ILHANDLE handle , 
             ILint offset    , 
             ILint origin    )
{
    TIOAccess* input = (TIOAccess*) handle;
    if ( NULL != input )
    {
        return input->seek( input, offset, origin );
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfTellProc( ILHANDLE handle )
{
    TIOAccess* input = (TIOAccess*) handle;
    return input->tell( input );
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfPutcProc( ILubyte putByte , 
             ILHANDLE handle )
{
    TIOAccess* input = (TIOAccess*) handle;
    return input->write( input, &putByte, 1, 1 );
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfWriteProc( const void* data , 
              ILuint eSize     , 
              ILuint eCount    ,
              ILHANDLE handle  )
{
    TIOAccess* input = (TIOAccess*) handle;
    return input->write( input, data, eSize, eCount );
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Init( void** plugdata   ,
                  const int argc    ,
                  const char** argv ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    /* count the total number of available codecs */
    const char** sTypes = supportedTypes;
    while ( *sTypes != NULL )
    {
        ++codecCount;
        ++sTypes;
    }

    if ( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION ) return 0;    
    
    ilInit();
    
    ilSetRead( ilfOpenProc  , 
               ilfCloseProc , 
               ilfEofProc   , 
               ilfGetcProc  , 
               ilfReadProc  , 
               ilfSeekProc  , 
               ilfTellProc  );

    ilSetWrite( ilfOpenProc  ,
                ilfCloseProc ,
                ilfPutcProc  , 
                ilfSeekProc  , 
                ilfTellProc  , 
                ilfWriteProc );

    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    ilSetRead( NULL , 
               NULL , 
               NULL , 
               NULL , 
               NULL , 
               NULL , 
               NULL );
               
    ilSetWrite( NULL , 
                NULL , 
                NULL , 
                NULL , 
                NULL , 
                NULL );

    ilShutDown();
    
    return 1;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Description( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "imgpluginDEVIL: image codec collection using DevIL as a backend\0";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.\0";
}

/*---------------------------------------------------------------------------*/

TVersion GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TVersion version;
    
    version.major = IMGPLUGIN_DEVIL_MAYOR_VERSION;
    version.minor = IMGPLUGIN_DEVIL_MINOR_VERSION;
    version.patch = IMGPLUGIN_DEVIL_PATCH_VERSION;
    version.release = IMGPLUGIN_DEVIL_RELEASE_VERSION;
    
    return version;
}

/*---------------------------------------------------------------------------*/

Int32
ConvertILPixelFormatToGUCEFPixelFormat( ILint devilType )
{
    switch ( devilType )
    {
        case IL_RGB : return PSF_RGB;
        case IL_RGBA : return PSF_RGBA;
        case IL_BGR : return PSF_BGR;
        case IL_BGRA : return PSF_BGRA;
        case IL_LUMINANCE : return PSF_SINGLE_CHANNEL_LUMINANCE;
        case IL_LUMINANCE_ALPHA : return PSF_SINGLE_CHANNEL_ALPHA;
        default: return -1;
    }
}

/*---------------------------------------------------------------------------*/

ILint
ConvertGUCEFPixelFormatToILPixelFormat( Int32 gucefType )
{
    switch ( gucefType )
    {
        case PSF_RGB : return IL_RGB;
        case PSF_RGBA : return IL_RGBA;
        case PSF_BGR : return IL_BGR;
        case PSF_BGRA : return IL_BGRA;
        case PSF_SINGLE_CHANNEL_LUMINANCE : return IL_LUMINANCE;
        case PSF_SINGLE_CHANNEL_ALPHA : return IL_LUMINANCE_ALPHA;
        default: return -1;
    }    
}

/*---------------------------------------------------------------------------*/

ILint
ConvertGUCEFTypeToILType( Int32 gucefType )
{
    switch ( gucefType )
    {
        case DT_FLOAT32 : return IL_FLOAT;
        case DT_FLOAT64 : return IL_DOUBLE;
        case DT_UINT8 : return IL_UNSIGNED_BYTE;
        case DT_INT8 : return IL_BYTE;
        case DT_UINT16 : return IL_UNSIGNED_SHORT;
        case DT_INT16 : return IL_SHORT;
        case DT_UINT32 : return IL_UNSIGNED_INT;
        case DT_INT32 : return IL_INT;
        default: return -1;
    }
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Encode( void* plugdata         ,
                    void* codecData        ,
                    const char* familyType ,
                    const char* codecType  ,
                    TIOAccess* input       ,
                    TIOAccess* output      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32 i=0, n=0, m=0, readBytes=0, imageBufferSize=0, pixeMapSize=0;
    TImageInfo imageInfo;
    TImageFrame* imageFrames = NULL;
    TImageFrameInfo* imageFrameInfo = NULL;
    TImageMipMapLevelInfo* mipInfo = NULL;
    void* imageBuffer = NULL;
    
    /* input sanity check */
    if ( ( NULL == familyType ) || ( NULL == codecType ) || ( NULL == input ) || ( NULL == output ) ) return 0;
    
    /* read the image header */    
    readBytes = input->read( input, &imageInfo, sizeof( TImageInfo ), 1 );
    
    /* sanity check */
    if ( ( readBytes != sizeof( TImageInfo ) )                  ||
         ( imageInfo.version != GUCEF_IMAGE_TIMAGEINFO_VERSION ) ) 
    {
        return 0;
    }

    /* First we gather all information */    
    imageFrames = (TImageFrame*) malloc( sizeof( TImageFrame ) * imageInfo.nrOfFramesInImage );
    for ( i=0; i<imageInfo.nrOfFramesInImage; ++i )
    {
        /* read the frame header */
        readBytes = input->read( input, &imageFrames[ i ].frameInfo, sizeof( TImageFrameInfo ), 1 );
        
        /* sanity check */
        if ( ( readBytes != sizeof( TImageFrameInfo ) )                  ||
             ( imageInfo.version != GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION ) ) 
        {
            free( imageFrames );
            return 0;
        }

        /* read the frame's mipmap info */
        imageFrameInfo = &imageFrames[ i ].frameInfo;
        imageFrames[ i ].mipmapLevel = (TImageMipMapLevel*) malloc( sizeof( TImageMipMapLevel ) * imageFrameInfo->nrOfMipmapLevels );        
        for ( n=0; n<imageFrameInfo->nrOfMipmapLevels; ++n )
        {
            /* get a shortcut */
            mipInfo = &imageFrames[ i ].mipmapLevel[ n ].mipLevelInfo;
            
            /* read the mipmap header */
            readBytes = input->read( input, &mipInfo, sizeof( TImageMipMapLevelInfo ), 1 );
            
            /* sanity check */
            if ( ( readBytes != sizeof( TImageMipMapLevelInfo ) )                  ||
                 ( imageInfo.version != GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION ) ) 
            {
                for ( m=0; m<imageInfo.nrOfFramesInImage; ++m ) free( imageFrames[ i ].mipmapLevel );
                free( imageFrames );
                return 0;
            }            
        }
    }
    
    /* generate an image ID and make that ID the ID of the current image */
    ilBindImage( ilGenImage() );

    /* Only 1 layer is supported atm */
    ilActiveLayer( 0 );

    imageBuffer = malloc( 10240 );
    imageBufferSize = 10240;
    for ( i=0; i<imageInfo.nrOfFramesInImage; ++i )
    {
        /* activate the frame */
        ilActiveImage( i );        
        
        imageFrameInfo = &imageFrames[ i ].frameInfo;
        for ( n=0; n<imageFrameInfo->nrOfMipmapLevels; ++n )
        {
            /* activate the mip-map */
            ilActiveMipmap( n );
            
            /* create a shortcut */
            mipInfo = &imageFrames[ i ].mipmapLevel[ n ].mipLevelInfo;
            
            /* calculate the size of the pixel map */
            pixeMapSize = ( mipInfo->channelComponentSize * mipInfo->channelCountPerPixel ) * 
                          ( mipInfo->frameWidth * mipInfo->frameHeight )                    ;
                          
            /* check if our local buffer is large enough to hold the pixel data */
            if ( imageBufferSize < pixeMapSize )
            {
                imageBuffer = realloc( imageBuffer, pixeMapSize );
                imageBufferSize = pixeMapSize;
            }
            
            /* read the pixel map */
            readBytes = input->read( input, imageBuffer, pixeMapSize, 1 );
            
            /* sanity check */
            if ( readBytes != pixeMapSize )
            {
                for ( m=n; m<imageInfo.nrOfFramesInImage; ++m ) free( imageFrames[ i ].mipmapLevel );
                free( imageFrames );    
                imageFrames = NULL;
                free( imageBuffer );
                return 0;
            }
            
            /* hand the data over to DevIL */
            if ( IL_TRUE != ilTexImage( (ILuint)mipInfo->frameWidth                                                   ,
                                        (ILuint)mipInfo->frameHeight                                                  ,
                                        (ILuint)1                                                                     ,
                                        (ILubyte)mipInfo->channelCountPerPixel                                        ,
                                        (ILenum)ConvertGUCEFPixelFormatToILPixelFormat( mipInfo->pixelStorageFormat ) ,
                                        (ILenum)ConvertGUCEFTypeToILType( mipInfo->pixelComponentDataType )           ,
                                        (ILvoid*)imageBuffer                                                          ) )
            {
                /* Failed to transfer the data over to DevIL */
                for ( m=n; m<imageInfo.nrOfFramesInImage; ++m ) free( imageFrames[ i ].mipmapLevel );
                free( imageFrames );    
                imageFrames = NULL;
                free( imageBuffer );
                return 0;
            }
                        
        }
        free( imageFrames[ i ].mipmapLevel );
        imageFrames[ i ].mipmapLevel = NULL;
    }
    free( imageFrames );    
    imageFrames = NULL;
    free( imageBuffer );
    
    /* now we can perform the actual save */
    currentResource = output;
    if ( IL_TRUE == ilSaveF( ilTypeFromExt( (const wchar_t*) codecType ), output ) )
    {
        currentResource = NULL;
        return 1;
    }
    
    currentResource = NULL;
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Decode( void* plugdata         ,
                    void* codecData        ,
                    const char* familyType ,
                    const char* codecType  ,
                    TIOAccess* input       ,
                    TIOAccess* output      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32 i=0, n=0, mipmapCount=0, frameCount=0;
    ILint imageID = 0, imageSize = 0;
    TImageInfo imageInfo;
    TImageFrameInfo imageFrameInfo;
    TImageMipMapLevelInfo imageMMInfo;
    
    if ( ( NULL == familyType ) || ( NULL == codecType ) || ( NULL == input ) || ( NULL == output ) ) return 0;
    
    /* generate an image ID and make that ID the ID of the current image */
    imageID = ilGenImage();
    ilBindImage( imageID );
    
    currentResource = input;
    
    if ( IL_TRUE == ilLoadF( ilTypeFromExt( (const wchar_t*) codecType ), input ) )
    {        
        /* write the TImageInfo section */
        imageInfo.version = GUCEF_IMAGE_TIMAGEINFO_VERSION;
        frameCount = imageInfo.nrOfFramesInImage = (UInt32) ilGetInteger( IL_NUM_IMAGES );
        if ( frameCount == 0 )
        {
            /* DevIL returns a image count of 0 for single-image images */
            frameCount = imageInfo.nrOfFramesInImage = 1;
        }
        output->write( output, &imageInfo, sizeof( imageInfo ), 1  );

        /* Only 1 layer is supported atm */
        ilActiveLayer( 0 );
        
        for ( i=0; i<frameCount; ++i )
        {
            /* activate the frame */
            ilActiveImage( i );
            
            /* write the TImageFrameInfo section */
            imageFrameInfo.version = GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION;
            mipmapCount = imageFrameInfo.nrOfMipmapLevels = (UInt32) ilGetInteger( IL_NUM_MIPMAPS );
            if ( mipmapCount == 0 )
            {
                /* DevIL returns a mipmap count of 0 images without mipmapping, we use 0 for the base image */
                mipmapCount = imageFrameInfo.nrOfMipmapLevels = 1;
            }            
            output->write( output, &imageFrameInfo, sizeof( imageFrameInfo ), 1 );
        
            for ( n=0; n<mipmapCount; ++n )
            {
                /* activate the mip-map */
                ilActiveMipmap( n );
                
                // Converted paletted images
                if( ilGetInteger( IL_IMAGE_FORMAT ) == IL_COLOUR_INDEX )
                {
                    ilConvertImage( IL_BGRA, IL_UNSIGNED_BYTE );
                }
                
                /* write the TImageMipMapLevelInfo section */
                imageMMInfo.version = GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION;
                imageMMInfo.channelComponentSize = 8; /* DevIL only supports UInt8 */
                imageMMInfo.channelCountPerPixel = ilGetInteger( IL_IMAGE_BPP );
                imageMMInfo.frameHeight = ilGetInteger( IL_IMAGE_HEIGHT );
                imageMMInfo.frameWidth = ilGetInteger( IL_IMAGE_WIDTH );
                imageMMInfo.pixelComponentDataType = DT_UINT8; /* DevIL only supports this type */
                imageMMInfo.pixelStorageFormat = ConvertILPixelFormatToGUCEFPixelFormat( ilGetInteger( IL_IMAGE_FORMAT ) );
                output->write( output, &imageMMInfo, sizeof( imageMMInfo ), 1 );
            }
        }
        
        /* now we append the pixel data */
        for ( i=0; i<frameCount; ++i )
        {
            /* activate the frame */
            ilActiveImage( i );
            
            for ( n=0; n<mipmapCount; ++n )
            {
                /* activate the mip-map */
                ilActiveMipmap( n );
                ilCompressFunc( IL_COMPRESS_NONE );
                
                /* write the pixel data */
                output->write( output, ilGetData(), ilGetInteger( IL_IMAGE_SIZE_OF_DATA ), 1 );
            }            
        }
    }
    
    /*
     *  the image has been loaded into our GUCEF structures so we have no need for
     *  DevIL to store the data any more 
     */
    ilDeleteImage( imageID );
    
    currentResource = NULL;
    
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_GetCodecSetBegin( void* plugdata  , 
                              void** iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    *iterator = malloc( sizeof( UInt32 ) );
    *( (UInt32*) *iterator ) = 0;
    return 1;    
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX 
CODECPLUGIN_GetCodecLink( void* plugdata               ,
                          void* iterator               , 
                          TCodecPluginLink** codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32 codecIndex = *( (UInt32*) iterator );
    
    if ( codecIndex < codecCount )
    {
        *codecLink = (TCodecPluginLink*) malloc( sizeof( TCodecPluginLink ) );
        (*codecLink)->codecData = NULL;
        (*codecLink)->codecFamily = "ImageCodec";
        (*codecLink)->codecType = supportedTypes[ codecIndex ];
        (*codecLink)->encode = CODECPLUGIN_Encode;
        (*codecLink)->decode = CODECPLUGIN_Decode;
        return 1;
    }
    
    /* There is no codec with the given index */
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_FreeCodecLink( void* plugdata              , 
                           TCodecPluginLink* codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    free( codecLink );
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_FreeCodecIterator( void* plugdata , 
                               void* iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32* codecIndex = (UInt32*) iterator;
    free( codecIndex );
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_GetCodecSetNextItem( void* plugdata ,
                                 void* iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32* codecIndex = ( (UInt32*) iterator );
    ++(*codecIndex);
    
    if ( (*codecIndex) < codecCount )
    {
        return 1;
    }
    
    /* There are no more codecs */
    return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* restore structure packing */
#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(pop)
#else
#pragma pack()
#endif /* WIN32 */

/*---------------------------------------------------------------------------*/
