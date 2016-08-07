//
//  esInterface.cpp
//  OpenGL_ES_Lab
//
//  Created by ZhaoZhihui on 16/7/27.
//  Copyright © 2016年 ZhaoZhihui. All rights reserved.
//

#include <cstdio>
#include <cstdarg>
#include "esInterface.hpp"

#include "Scene/TextureScene.h"
#include "Scene/TriangleScene.h"

EsInterface::EsInterface()
{
//	mScene = new TriangleScene(mRenderer);
	mScene = new TextureScene(mRenderer);

}

EsInterface::~EsInterface()
{
    if (mScene != nullptr) {
        delete mScene;
    }
}

int EsInterface::init(ESContext *esContext)
{
//    mContext = esContext;

	mScene->enter();

	mRenderer.setViewport(esContext->width, esContext->height);

	return true;
}

void EsInterface::update(float delta)
{
 //   logMessage("Update called.");
}

void EsInterface::draw ()
{
	mScene->render();
}

void EsInterface::shutdown ()
{

}





