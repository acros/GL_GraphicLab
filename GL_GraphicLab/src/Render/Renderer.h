#pragma once

#include <string>
#include "esUtil.h"

using namespace std;

class Renderer {
public:
	Renderer();
	~Renderer();

	void checkRendererVersion()const;

	void	setViewport(int width,int height);
	GLuint	loadShaderProgram(const string& vertStr,const string& fragStr);

	void	beginDraw();

	void	draw();

	void	endDraw();

protected:

	GLuint loadShader(GLenum type, const char *shaderSrc);

protected:

	GLuint mProgramObject;

};