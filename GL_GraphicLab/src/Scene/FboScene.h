#pragma once

#include "Scene.hpp"
#include "Camera/Camera.h"

/*
	The 3D scene

*/
class FboScene : public Scene
{
public:
	FboScene(Renderer& renderer);
	~FboScene();

	virtual void enter()override;

	virtual void update(float delta);

	virtual void render()override;

	virtual void exit()override;

protected:

	void renderToTexture();

protected:

// 	GLuint		mFbo;
// 	GLuint		mRbo;

	float		mCameraMoveTime;

	Camera*		mCam;

	AcObject*	mCube;

};