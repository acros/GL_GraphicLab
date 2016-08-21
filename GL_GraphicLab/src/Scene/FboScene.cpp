#include "FboScene.h"

FboScene::FboScene(Renderer& renderer)
	: Scene(renderer)
// 	, mFbo(0)
// 	, mRbo(0)
// 	, mMvpLoc(-1)
	, mCam(nullptr)
	, mCube(nullptr)
	, mCameraMoveTime(0.f)
{
// 	mVbo[0] = 0;
// 	mVbo[1] = 0;
}

FboScene::~FboScene()
{

}

void FboScene::enter()
{
//	Scene::enter();

	//Set camera 
	mCam = new Camera(800/600.f,45.f,1.f,1000.f);
	AcVector eyePos(-10.f, 3.f, 10.f);
	mCam->setPosition(eyePos);	const AcMatrix& vieMat = mCam->getViewMat();

	mCube = new AcObject();
	mCube->setPosition(AcVector(0.0f, 0.f, -3.0f));
	mCube->setScale(AcVector(1.0f, 2.5f, 1.0f));
	mCube->rotate(AcVector(0.0f, 1.0f, 0.0f),-15.f);
	mCube->createShape();
	mCube->initDraw(mRendererRef);

}

void FboScene::update(float delta)
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

void FboScene::render()
{
	mRendererRef.beginDraw();

	mCube->draw(mRendererRef, mCam->getViewMat(), mCam->getProjMat());

	mRendererRef.endDraw();
}

void FboScene::exit()
{
	delete mCam;
	delete mCube;
}

void FboScene::renderToTexture()
{

}
