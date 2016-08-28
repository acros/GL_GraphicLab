#include "LightScene.h"

LightScene::LightScene(Renderer& renderer)
	: Scene(renderer)
 	, mFbo(0)
	, mCam(nullptr)
	, mCube(nullptr)
	, mGround(nullptr)
	, mCameraMoveTime(0.f)
{
	mTexId[0] = 0;
	mTexId[1] = 0;

	mVertStr =
		"#version 300 es                            \n"
		"layout(location = 0) in vec4 a_position;   \n"
		"layout(location = 1) in vec2 a_texCoord;   \n"
		"out vec2 v_texCoord;                       \n"
		"void main()                                \n"
		"{                                          \n"
		"   gl_Position = a_position;               \n"
		"   v_texCoord = a_texCoord;                \n"
		"}                                          \n";

	mFragStr =
		"#version 300 es                                     \n"
		"precision mediump float;                            \n"
		"in vec2 v_texCoord;                                 \n"
		"layout(location = 0) out vec4 outColor;             \n"
		"uniform sampler2D s_texture;                        \n"
		"void main()                                         \n"
		"{                                                   \n"
		"  outColor = texture( s_texture, vec2(v_texCoord.x,1.0 - v_texCoord.y ));      \n"
		"}                                                   \n";
}

LightScene::~LightScene()
{

}

void LightScene::enter()
{
//	Scene::enter();
	//Load default 
	mDefaultProgram = mRendererRef.loadShaderProgram(mVertStr, mFragStr);

	//Set camera 
	mCam = new Camera(800/600.f,45.f,1.f,1000.f);
	AcVector eyePos(-10.f, 3.f, 10.f);
	mCam->setPosition(eyePos);
	const AcMatrix& vieMat = mCam->getViewMat();

	mCube = new AcObject();
	mCube->setPosition(AcVector(0.0f, 0.f, -3.0f));
	mCube->setScale(AcVector(1.0f, 2.5f, 1.0f));
	mCube->rotate(AcVector(0.0f, 1.0f, 0.0f),-15.f);

	mCube->createShape(ShapeType::ST_Cube);
	mCube->initDraw(mRendererRef);


	// Center the ground
	mGround = new AcObject();
	mGround->setPosition(AcVector(-8.0f, -2.f, -8.0f));
	mGround->setScale(AcVector(10.0f, 10.0f, 10.0f));
	mGround->rotate(AcVector(1.0f, 0.0f, 0.0f), -90.0f);

	mGround->createShape(ShapeType::ST_Plane);
	mGround->initDraw(mRendererRef);

	//Init the depth draw
	mRenderToTexture = true;
	if (mRenderToTexture)
	{
		GLint texWidth = 256, texHeight = 256;

		glGenFramebuffers(1, &mFbo);
		glGenTextures(2, mTexId);

		//Create texture for color attachment
		glBindTexture(GL_TEXTURE_2D, mTexId[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//Create texture , used as depth in render
		glBindTexture(GL_TEXTURE_2D, mTexId[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texWidth, texHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexId[0], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTexId[1], 0);

		GLenum state = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (state != GL_FRAMEBUFFER_COMPLETE)
		{
			assert(false);
		}
	}
}

void LightScene::update(float delta)
{
	//Move the camera around
	float x = -10.f;
	float y = 3.f;
	float z = 10.f;
	x *= sin(mCameraMoveTime);
	z *= cos(mCameraMoveTime);
	mCam->setPosition(AcVector(x,y,z));
	mCameraMoveTime += (0.5f * delta);

}

void LightScene::render()
{
	/*
		//Render to texture
	if (mRenderToTexture)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

		glViewport(0, 0, 256, 256);
		glClearColor(0.5f, 0.5f, 0.5f, 0.f);

		// clear depth buffer
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		mCube->draw(mRendererRef, mCam->getViewMat(), mCam->getProjMat());
		mGround->draw(mRendererRef, mCam->getViewMat(), mCam->getProjMat());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	*/

	glUseProgram(0);

	glClearColor(0.1f, 0.1f, 0.1f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	mCube->draw(mRendererRef, mCam->getViewMat(), mCam->getProjMat());
	mGround->draw(mRendererRef, mCam->getViewMat(), mCam->getProjMat());
}

void LightScene::exit()
{
	glDeleteFramebuffers(1, &mFbo);
	glDeleteTextures(2, mTexId);

	delete mCam;
	delete mCube;
	delete mGround;
}

void LightScene::drawDepthTexture()
{

}
