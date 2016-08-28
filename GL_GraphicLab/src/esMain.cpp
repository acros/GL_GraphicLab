//
//  main.cpp
//  OpenGL_ES_Lab
//
//  Created by ZhaoZhihui on 16/7/27.
//  Copyright © 2016年 ZhaoZhihui. All rights reserved.
//

#include <stdio.h>
#include "esUtil.h"
#include "esInterface.hpp"

#ifndef __APPLE__

extern GLboolean ESUTIL_API esCreateWindow(ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags);

#endif

#ifdef ANDROID
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
typedef AAsset esFile;
#else
#pragma warning( disable : 4996) 
typedef FILE esFile;
#endif

EsInterface  gEsInterface;

void shutdown ( ESContext *esContext )
{
    gEsInterface.shutdown();
}

void draw ( ESContext *esContext )
{
    gEsInterface.draw();    
}

void update(ESContext* esContext,float delta)
{
    gEsInterface.update(delta);
}


void logMessage(const char *formatStr, ...)
{
	va_list params;
	char buf[BUFSIZ];

	va_start(params, formatStr);
	//	vsprintf(buf, formatStr, params);
	vsprintf_s(buf, formatStr, params);

	printf("%s", buf);

	va_end(params);
}


extern "C"{

int esMain ( ESContext *esContext )
{
    esContext->userData = malloc ( sizeof ( UserData ) );
  
#ifndef __APPLE__
	esCreateWindow(esContext, "GL_Graphic Lab", esContext->width, esContext->height, ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL);
#endif

    if ( !gEsInterface.init( esContext ) )
    {
        return GL_FALSE;
    }
    
    esContext->shutdownFunc = shutdown;
    
    esContext->drawFunc = draw;
    
    esContext->updateFunc = update;
    
    return GL_TRUE;
}
}


///
// esFileRead()
//
//    Wrapper for platform specific File open
//
static esFile *esFileOpen(void *ioContext, const char *fileName)
{
	esFile *pFile = NULL;

#ifdef ANDROID

	if (ioContext != NULL)
	{
		AAssetManager *assetManager = (AAssetManager *)ioContext;
		pFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
	}

#else
#ifdef __APPLE__
	// iOS: Remap the filename to a path that can be opened from the bundle.
	fileName = GetBundleFileName(fileName);
#endif
	pFile = fopen(fileName, "rb");
#endif

	return pFile;
}

///
// esFileRead()
//
//    Wrapper for platform specific File read
//
static int esFileRead(esFile *pFile, int bytesToRead, void *buffer)
{
	int bytesRead = 0;

	if (pFile == NULL)
	{
		return bytesRead;
	}

#ifdef ANDROID
	bytesRead = AAsset_read(pFile, buffer, bytesToRead);
#else
	//64 bit warning
	bytesRead = fread(buffer, bytesToRead, 1, pFile);
#endif

	return bytesRead;
}

#pragma pack(1)
typedef struct 
#ifdef __APPLE__
__attribute__((packed))
#endif
{
	unsigned char  IdSize,
		MapType,
		ImageType;
	unsigned short PaletteStart,
		PaletteSize;
	unsigned char  PaletteEntryDepth;
	unsigned short X,
		Y,
		Width,
		Height;
	unsigned char  ColorDepth,
		Descriptor;

} TGA_HEADER;

///
// esFileRead()
//
//    Wrapper for platform specific File close
//
static void esFileClose(esFile *pFile)
{
	if (pFile != NULL)
	{
#ifdef ANDROID
		AAsset_close(pFile);
#else
		fclose(pFile);
		pFile = NULL;
#endif
	}
}

///
// esLoadTGA()
//
//    Loads a 8-bit, 24-bit or 32-bit TGA image from a file
//
char *ESUTIL_API esLoadTGA(void *ioContext, const char *fileName, int *width, int *height)
{
	char        *buffer;
	esFile      *fp;
	TGA_HEADER   Header;
	int          bytesRead;

	// Open the file for reading
	fp = esFileOpen(ioContext, fileName);

	if (fp == NULL)
	{
		// Log error as 'error in opening the input file from apk'
//		esLogMessage("esLoadTGA FAILED to load : { %s }\n", fileName);
		return NULL;
	}

	bytesRead = esFileRead(fp, sizeof(TGA_HEADER), &Header);

	*width = Header.Width;
	*height = Header.Height;

	if (Header.ColorDepth == 8 ||
		Header.ColorDepth == 24 || Header.ColorDepth == 32)
	{
		int bytesToRead = sizeof(char) * (*width) * (*height) * Header.ColorDepth / 8;

		// Allocate the image data buffer
		buffer = (char *)malloc(bytesToRead);

		if (buffer)
		{
			bytesRead = esFileRead(fp, bytesToRead, buffer);
			esFileClose(fp);

			return (buffer);
		}
	}

	return (NULL);
}