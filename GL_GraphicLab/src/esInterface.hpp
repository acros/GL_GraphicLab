//
//  esInterface.hpp
//  OpenGL_ES_Lab
//
//  Created by ZhaoZhihui on 16/7/27.
//  Copyright © 2016年 ZhaoZhihui. All rights reserved.
//

#ifndef esInterface_hpp
#define esInterface_hpp

#include "esUtil.h"
class Scene;

class EsInterface{
    
public:
    EsInterface();
    ~EsInterface();
    
    int initEs(ESContext* esContext);
    
    void update(float delta);
    
    void draw();
    
    void shutdown ();
    
    void logMessage ( const char *formatStr, ... );
protected:
    GLuint loadShader ( GLenum type, const char *shaderSrc );
    
    ESContext*  mContext;
    
    Scene* mScene;
};

#endif /* esInterface_hpp */
