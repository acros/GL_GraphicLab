#include "Camera.h"

Camera::Camera(float aspect, float fov, float nearPlane, float farPlane)
	: mAspect(aspect)
	, mFov(fov)
	, mNear(nearPlane)
	, mFar(farPlane)
	, mLookAt(0.f,0.f,0.f)
	, mUp (0.0f, 1.0f, 0.0f)
{
	esMatrixLoadIdentity(&mProjMat);
	esPerspective(&mProjMat, mFov, mAspect, mNear, mFar);
}

Camera::~Camera()
{

}

void Camera::update(float delta)
{
	//TODO: Do sth to control the camera

}

const AcMatrix& Camera::getViewMat()
{
	esMatrixLoadIdentity(&mViewMat);
	esMatrixLookAt(&mViewMat, mPos.x, mPos.y, mPos.z, mLookAt.x, mLookAt.y, mLookAt.z, mUp.x, mUp.y, mUp.z);

	return mViewMat;
}

const AcMatrix& Camera::getProjMat() const
{
	return mProjMat;
}
