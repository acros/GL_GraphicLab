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


extern "C"{

int esMain ( ESContext *esContext )
{
    esContext->userData = malloc ( sizeof ( UserData ) );
  
#ifndef __APPLE__
	esCreateWindow(esContext, "GL_Graphic Lab", esContext->width, esContext->height, ES_WINDOW_RGB);
#endif

    if ( !gEsInterface.initEs( esContext ) )
    {
        return GL_FALSE;
    }
    
    esContext->shutdownFunc = shutdown;
    
    esContext->drawFunc = draw;
    
    esContext->updateFunc = update;
    
    return GL_TRUE;
}
}
