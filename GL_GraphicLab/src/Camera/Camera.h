#pragma once

#include "Base/AcObject.h"

class Camera : public AcObject
{
public:
	Camera(float aspect, float fov, float nearPlane, float farPlane);
	~Camera();


	virtual void update(float delta)override;


	const AcMatrix&	getViewMat();
	const AcMatrix&	getProjMat()const;

protected:

	float		mNear;
	float		mFar;

	float		mFov;
	float		mAspect;

//	bool		mViewMatDirty;

	AcVector	mLookAt;
	AcVector	mUp;

	ESMatrix	mViewMat;
	ESMatrix	mProjMat;

};