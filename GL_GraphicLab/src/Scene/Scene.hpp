//
//  Scene.hpp
//  OpenGL_ES_Lab
//
//  Created by ZhaoZhihui on 16/8/2.
//  Copyright © 2016年 ZhaoZhihui. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <string>
#include "esUtil.h"
#include "Render/Renderer.h"

using namespace std;

class Scene{
public:
	Scene(Renderer&	render);
	virtual ~Scene();

	const string& getVertexStr()const { return mVertStr; }
	const string& getFragmentStr()const { return mFragStr; }

    virtual void enter();
    
    virtual void update(float delta);
    
    virtual void render() = 0;
    
    virtual void exit();
    
protected:
    
	string	mVertStr;
	string	mFragStr;

	GLuint	mShaderProgram;

	Renderer&	mRendererRef;
};


#endif /* Scene_hpp */
