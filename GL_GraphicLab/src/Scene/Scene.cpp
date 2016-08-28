//
//  Scene.cpp
//  OpenGL_ES_Lab
//
//  Created by ZhaoZhihui on 16/8/2.
//  Copyright © 2016年 ZhaoZhihui. All rights reserved.
//

#include "Scene.hpp"


Scene::Scene(Renderer& render)
	: mRendererRef(render)
	, mShaderProgram(0)
{

}

Scene::~Scene()
{
	if (mShaderProgram != 0)
	{
		glDeleteProgram(mShaderProgram);
	}
}

void Scene::enter()
{
	mShaderProgram = mRendererRef.loadShaderProgram(getVertexStr().c_str(), getFragmentStr().c_str());
}

void Scene::update(float delta)
{
    
}

void Scene::render()
{
    
}

void Scene::exit()
{
    
}