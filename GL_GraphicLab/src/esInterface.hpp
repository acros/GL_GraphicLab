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
#include "Render/Renderer.h"

class Scene;

class EsInterface{
    
public:
    EsInterface();
    ~EsInterface();
    
    int init(ESContext* esContext);
    
    void update(float delta);
    
    void draw();
    
    void shutdown ();

protected:
    
	Renderer	mRenderer;

 //   ESContext*  mContext;
    
    Scene* mScene;
};

#endif /* esInterface_hpp */
