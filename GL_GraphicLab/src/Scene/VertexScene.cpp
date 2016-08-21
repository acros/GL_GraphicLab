#include "VertexScene.h"

const int VERTEX_POS_INDX = 0;
const int VERTEX_COLOR_INDX = 1;

//Set the vertex we need to draw
const int VERTEX_POS_SIZE = 3;
const int VERTEX_COLOR_SIZE = 4;

GLfloat vertices[3 * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE)] =
{
	0.0f,  0.5f, 0.0f,        // v0
	1.0f,  0.0f, 0.0f, 1.0f,  // c0
	-0.5f, -0.5f, 0.0f,       // v1
	0.0f,  1.0f, 0.0f, 1.0f,  // c1
	0.5f, -0.5f, 0.0f,        // v2
	0.0f,  0.0f, 1.0f, 1.0f,  // c2
};

GLushort indices[3] = { 0, 1, 2 };

VertexScene::VertexScene(Renderer& renderer)
	: Scene(renderer)
	, mVboId(0)
	, mElemVboId(0)
	, mVaoId(0)
{
	mVertStr =
		"#version 300 es                            \n"
		"layout(location = 0) in vec4 a_position;   \n"
		"layout(location = 1) in vec4 a_color;      \n"
		"uniform float u_offset;                    \n"
		"out vec4 v_color;                          \n"
		"void main()                                \n"
		"{                                          \n"
		"    v_color = a_color;                     \n"
		"    gl_Position = a_position;              \n"
		"    gl_Position.x += u_offset;             \n"
		"}";


	mFragStr =
		"#version 300 es            \n"
		"precision mediump float;   \n"
		"in vec4 v_color;           \n"
		"out vec4 o_fragColor;      \n"
		"void main()                \n"
		"{                          \n"
		"    o_fragColor = v_color; \n"
		"}";
}

VertexScene::~VertexScene()
{

}

void VertexScene::enter()
{
	Scene::enter();
	
	//Init VBO (Vertex Buffer Object)
	glGenBuffers(1, &mVboId);
	glBindBuffer(GL_ARRAY_BUFFER, mVboId);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * (VERTEX_COLOR_SIZE + VERTEX_POS_SIZE), vertices,GL_STATIC_DRAW);

	//Init VBO (Element Vertex Buffer Object)
	glGenBuffers(1, &mElemVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElemVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLushort) ,indices,GL_STATIC_DRAW);

	//////////////////////////////////////////////////////////////////////////

	//Init VAO
	glGenVertexArrays(1, &mVaoId);

	//Bind vertex array
	glBindVertexArray(mVaoId);

	//Bind VertexArray, then set VBO attributes
	glBindBuffer(GL_ARRAY_BUFFER,mVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElemVboId);

	glEnableVertexAttribArray(VERTEX_POS_INDX);
	glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE), 0);

	GLint offset = sizeof(GLfloat) * VERTEX_POS_SIZE;
	glEnableVertexAttribArray(VERTEX_COLOR_INDX);
	glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE), (const void*)offset);

	glBindVertexArray(0);
}

void VertexScene::render()
{
	mRendererRef.beginDraw();

	// Use the program object
	glUseProgram(mShaderProgram);

	drawWithVertexArray();

	drawWithVBO();

	drawWithVAO();

	mRendererRef.endDraw();
}

void VertexScene::drawWithVertexArray()
{	//Shader value
	GLint offsetLoc = glGetUniformLocation(mShaderProgram, "u_offset");
	glUniform1f(offsetLoc, -0.5f);

	//Bind no buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Enable vertex attribute
	glEnableVertexAttribArray(VERTEX_POS_INDX);
	glEnableVertexAttribArray(VERTEX_COLOR_INDX);

	GLfloat* vertexBuf = vertices;

	GLsizei arrayStride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE);
	glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, arrayStride,vertexBuf);

	vertexBuf += (VERTEX_POS_SIZE * sizeof(GLfloat));
	glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FALSE, GL_FALSE, arrayStride, vertexBuf);

	glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_SHORT,indices);

}

void VertexScene::drawWithVBO()
{
	//Shader value
	float offsetLoc = glGetUniformLocation(mShaderProgram, "u_offset");
	glUniform1f(offsetLoc, 0.5f);

	//Bind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, mVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElemVboId);

	//Enable vertex attribute
	glEnableVertexAttribArray(VERTEX_POS_INDX);
	glEnableVertexAttribArray(VERTEX_COLOR_INDX);
	//Set attribute
	GLsizei arrayStride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE);

	//use VBO, no data need to copy in , so the last param offset is start from 0
	//so the last param is not point 
	GLuint offset = 0;
	glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, arrayStride, (const void *)offset);

	offset += (VERTEX_POS_SIZE * sizeof(GLfloat));
	glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, arrayStride, (const void *)offset);

	//Use VBO drawing
	// Element num type is unsigned short
	// Used element array buffer here, no data need to send, so the last param is 0
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

	//Close attribute
	glDisableVertexAttribArray(VERTEX_POS_INDX);
	glDisableVertexAttribArray(VERTEX_COLOR_INDX);

	//Reset VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexScene::drawWithVAO()
{
	//Shader value
	float offsetLoc = glGetUniformLocation(mShaderProgram, "u_offset");
	glUniform1f(offsetLoc, 0.0f);

	glBindVertexArray(mVaoId);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (const void*)0);

	glBindVertexArray(0);
}

void VertexScene::exit()
{
	glDeleteBuffers(1, &mVboId);
	glDeleteBuffers(1, &mElemVboId);
}

