#pragma once

#include "Scene.hpp"

/*
	Vertex Draw
	Example for VAO VBO
*/
class VertexScene : public Scene
{
public:
	VertexScene(Renderer& renderer);
	~VertexScene();

	virtual void enter()override;

	virtual void render()override;

	virtual void exit()override;

protected:

	void drawWithVertexArray();
	void drawWithVBO();

	//Only for ES-3.0
	void drawWithVAO();

protected:

	//For VBO draw
	GLuint	mVboId;
	GLuint	mElemVboId;

	//For VAO draw
	GLuint	mVaoId;
};