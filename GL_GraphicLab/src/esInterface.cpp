//
//  esInterface.cpp
//  OpenGL_ES_Lab
//
//  Created by ZhaoZhihui on 16/7/27.
//  Copyright © 2016年 ZhaoZhihui. All rights reserved.
//

#include "esInterface.hpp"
#include "Scene/Scene.hpp"
#include <cstdio>
#include <cstdarg>

EsInterface::EsInterface()
: mScene(nullptr)
{
    
}

EsInterface::~EsInterface()
{
    if (mScene != nullptr) {
        delete mScene;
    }
}

int EsInterface::initEs(ESContext *esContext)
{
    mContext = esContext;
    
    //Check glVersion
    const char* glVer = (const char*)glGetString(GL_VERSION);
    logMessage("OpenGL-ES version is : %s.\n",glVer);
    
    const char* glShaderVer = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    logMessage("OpenGL-ES shader version is : %s.\n",glShaderVer);
    
    UserData *userData = (UserData*)esContext->userData;
    char vShaderStr[] =
    "#version 300 es                          \n"
    "layout(location = 0) in vec4 vPosition;  \n"
    "layout(location = 1) in vec4 vColor;     \n"
    "   out vec4 fColor;                           "
    "void main()                              \n"
    "{                                        \n"
    "   gl_Position = vPosition;              \n"
    "       fColor = vColor;                    "
    "}                                        \n";
    
    char fShaderStr[] =
    "#version 300 es                              \n"
    "precision mediump float;                     \n"
    "in vec4 fColor;                              \n"
    "out vec4 fragColor;                          \n"
    "void main()                                  \n"
    "{                                            \n"
    //     "fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
    "fragColor = fColor;                        \n"
    "}                                            \n";
    
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;
    
    // Load the vertex/fragment shaders
    vertexShader = loadShader( GL_VERTEX_SHADER, vShaderStr );
    fragmentShader = loadShader ( GL_FRAGMENT_SHADER, fShaderStr );
    
    // Create the program object
    programObject = glCreateProgram ( );
    
    if ( programObject == 0 )
    {
        return 0;
    }
    
    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );
    
    // Link the program
    glLinkProgram ( programObject );
    
    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );
    
    if ( !linked )
    {
        GLint infoLen = 0;
        
        glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
        
        if ( infoLen > 1 )
        {
            char *infoLog = (char*)malloc ( sizeof ( char ) * infoLen );
            
            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            logMessage ( "Error linking program:\n%s\n", infoLog );
            
            free ( infoLog );
        }
        
        glDeleteProgram ( programObject );
        return FALSE;
    }
    
    // Store the program object
    userData->programObject = programObject;
    
    glClearColor ( 0.1f, 0.1f, 0.1f, 0.0f );
    return TRUE;
}


GLuint  EsInterface::loadShader ( GLenum type, const char *shaderSrc )
{
    GLuint shader;
    GLint compiled;
    
    // Create the shader object
    shader = glCreateShader ( type );
    
    if ( shader == 0 )
    {
        return 0;
    }
    
    // Load the shader source
    glShaderSource ( shader, 1, &shaderSrc, NULL );
    
    // Compile the shader
    glCompileShader ( shader );
    
    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );
    
    if ( !compiled )
    {
        GLint infoLen = 0;
        
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        
        GLenum sx = glGetError();
        logMessage("Gl state error: %d",sx);
        
        if ( infoLen > 1 )
        {
            char *infoLog = (char*)malloc ( sizeof ( char ) * infoLen );
            
            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            logMessage ( "Error compiling shader:\n%s\n", infoLog );
            
            free ( infoLog );
        }
        
        glDeleteShader ( shader );
        return 0;
    }
    
    return shader;
}

void EsInterface::update(float delta)
{
    logMessage("Update called.");
}

void EsInterface::draw ()
{
    UserData *userData = (UserData*)mContext->userData;
    GLfloat vVertices[] = {
        0.0f,  1.f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f   };
    
    GLfloat vColors[] = {
        0.5f,0.f,0.0f,
        0.0f, 1.f,0.0f,
        0.0f,0.0f,1.0f
    };
    
    // Set the viewport
    glViewport ( 0, 0, mContext->width, mContext->height );
    
    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );
    
    // Use the program object
    glUseProgram ( userData->programObject );
    
    // Load the vertex data
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray ( 0 );
    
    //the next triangle
    glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE,  0, vColors );
    glEnableVertexAttribArray (1);
    
    glDrawArrays ( GL_TRIANGLES, 0, 3 );
    
}


void EsInterface::shutdown ()
{
    UserData *userData = (UserData*)mContext->userData;
    
    glDeleteProgram ( userData->programObject );
}

void EsInterface::logMessage ( const char *formatStr, ... )
{
    va_list params;
    char buf[BUFSIZ];
    
    va_start ( params, formatStr );
    vsprintf ( buf, formatStr, params );
    
    printf ( "%s", buf );
    
    va_end ( params );
}






