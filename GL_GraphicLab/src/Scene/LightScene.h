#pragma once

#include "Scene.hpp"
#include "Camera/Camera.h"

/*
	The Light scene

*/
class LightScene : public Scene
{
public:
	LightScene(Renderer& renderer);
	~LightScene();

	virtual void enter()override;

	virtual void update(float delta);

	virtual void render()override;

	virtual void exit()override;

protected:

	void drawDepthTexture();

protected:
	GLuint		mDefaultProgram;

	bool		mRenderToTexture;
	GLuint		mFbo;
	GLuint		mTexId[2];

	float		mCameraMoveTime;

	Camera*		mCam;

	AcObject*	mCube;
	AcObject*	mGround;

};