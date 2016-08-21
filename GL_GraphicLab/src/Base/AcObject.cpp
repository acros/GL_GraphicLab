#include "AcObject.h"
#include "Render/Mesh.h"

AcVector AcVector::ONE = AcVector(1.f,1.f,1.f);

AcObject::~AcObject()
{
	if (mMesh != nullptr)
		delete mMesh;
}

const AcMatrix& AcObject::getModelMat()
{
	esMatrixLoadIdentity(&mModelMat);
	esTranslate(&mModelMat, mPos.x, mPos.y, mPos.z);
	esScale(&mModelMat, mScale.x, mScale.y, mScale.z);
	esRotate(&mModelMat, mAngleY, mRot.x, mRot.y, mRot.z);

	return mModelMat;
}

void AcObject::rotate(const AcVector& rotAxis, float angle)
{
	mRot = rotAxis;
	mAngleY = angle;
}

void AcObject::initDraw(Renderer& context)
{
	if (mMesh != nullptr)
	{
		mMesh->initDraw(context);
	}
}

void AcObject::draw(Renderer& context, const AcMatrix& viewMat,const AcMatrix& proj)
{
	if (mMesh != nullptr)
	{
		AcMatrix modelview;
		AcMatrix mvpMatrix;
		esMatrixMultiply(&modelview, &getModelMat(), &viewMat);

		// Compute MVP for scene rendering by multiplying the modelview and perspective matrices together
		esMatrixMultiply(&mvpMatrix, &modelview, &proj);

		mMesh->draw(context, mvpMatrix);
	}
}

void AcObject::createShape()
{
	if (mMesh != nullptr)
		delete mMesh;

	mMesh = new Mesh();
	mMesh->createCube();
}
