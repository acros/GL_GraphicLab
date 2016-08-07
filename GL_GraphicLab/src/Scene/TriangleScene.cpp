#include "TriangleScene.h"

TriangleScene::TriangleScene(Renderer& renderer)
	: Scene(renderer)
{
	mVertStr =
		"#version 300 es                          \n"
		"layout(location = 0) in vec4 vPosition;  \n"
		"void main()                              \n"
		"{                                        \n"
		"   gl_Position = vPosition;              \n"
		"}                                        \n";

	mFragStr =
		"#version 300 es                              \n"
		"precision mediump float;                     \n"
		"out vec4 fragColor;                          \n"
		"void main()                                  \n"
		"{                                            \n"
		"   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
		"}                                            \n";
}

TriangleScene::~TriangleScene()
{

}

void TriangleScene::render()
{
	mRendererRef.beginDraw();

	GLfloat vVertices[] = {
		0.0f,  1.f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f };

	GLfloat vColors[] = {
		0.5f,0.f,0.0f,
		0.0f, 1.f,0.0f,
		0.0f,0.0f,1.0f
	};

	// Use the program object
	glUseProgram(mShaderProgram);

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);

	//the next triangle
	// 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vColors);
	// 	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	mRendererRef.endDraw();
}
