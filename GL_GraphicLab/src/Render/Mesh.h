#pragma once

//#include "AcObject.h"
#include "esUtil.h"
#include "Renderer.h"
#include "Base/Math.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void createCube();
	void createPlane();

	void initDraw(Renderer& context);

	//Now we just set each mesh has a VBO
	void draw(Renderer& context,const AcMatrix& mat);

protected:
	ShapeType	mShape;

	GLuint		mVbo[2];

	GLuint		vertexSize;
	GLuint		indexSize;

	GLfloat*	vertices;
	GLuint*		indices;

	class Material*	mMaterial;
};