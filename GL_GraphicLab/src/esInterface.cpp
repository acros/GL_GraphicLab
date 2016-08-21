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
#include "Scene/VertexScene.h"
#include "Scene/FboScene.h"

EsInterface::EsInterface()
{
//	mScene = new TriangleScene(mRenderer);
//	mScene = new TextureScene(mRenderer);
//	mScene = new VertexScene(mRenderer);

	mScene = new FboScene(mRenderer);
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
	mScene->update(delta);
}

void EsInterface::draw ()
{
	mScene->render();
}

void EsInterface::shutdown ()
{

}





