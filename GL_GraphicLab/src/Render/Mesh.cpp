#include "Mesh.h"
#include "Material.h"

const int POSTITION_LOC = 0;
const int COLOR_LOC = 1;

Mesh::Mesh() 
	: vertices(nullptr)
	, indices(nullptr)
	, vertexSize(0)
	, indexSize(0)
	, mMaterial(nullptr)
{
	mVbo[0] = 0;
	mVbo[1] = 0;
}

Mesh::~Mesh()
{
	glDeleteBuffers(2, mVbo);

	if(vertices)
		free(vertices);

	if(indices)
		free(indices);

	if (mMaterial)
		delete mMaterial;
}

void Mesh::createCube()
{
	mShape = ST_Cube;

	//Load the index and init buffer, use VBO here
	indexSize = esGenCube(1.0f, &vertices, NULL, NULL, &indices);

	//Hack: Cube has 24 vertex
	vertexSize = 24;

	mMaterial = new Material();
}

void Mesh::createPlane()
{
	mShape = ST_Plane;

	int edgeTrigleNums = 3;
	indexSize = esGenSquareGrid(edgeTrigleNums, &vertices, &indices);
	vertexSize = edgeTrigleNums * edgeTrigleNums;

	mMaterial = new Material();
}

void Mesh::initDraw(Renderer& context)
{
	string vertStr, fragStr;
	mMaterial->loadShader(context,vertStr, fragStr);

	glGenBuffers(2, mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo[1]);

	glBufferData(GL_ARRAY_BUFFER, vertexSize * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(Renderer& context,const AcMatrix& mat)
{
	glUseProgram(mMaterial->mShaderProgram);

	//Render a cube
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo[1]);

	glEnableVertexAttribArray(POSTITION_LOC);	// Pos
	glVertexAttribPointer(POSTITION_LOC, 3, GL_FLOAT, GL_FALSE, (3 /*+ 2*/) * sizeof(GLfloat), (const void*)NULL);	//Pure position vertex array

	if (mShape == ST_Cube)
		glVertexAttrib4f(COLOR_LOC, 0.8f, 0.8f, 0.0f, 1.0f);		//Set the color to a Const value
	else if (mShape == ST_Plane)
		glVertexAttrib4f(COLOR_LOC, 0.7f, 0.7f, 0.7f, 1.0f);	

	glUniformMatrix4fv(mMaterial->mMvpLoc, 1, GL_FALSE, (GLfloat*)&(mat.m[0][0]));

	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(POSTITION_LOC);	// Pos
	glDisableVertexAttribArray(COLOR_LOC);	// Color

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
