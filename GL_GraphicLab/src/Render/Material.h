#pragma once

#include <string>

#include "esUtil.h"
#include "Renderer.h"

using namespace std;

class Material
{
public:
	Material():mMvpLoc(-1), mShaderProgram(-1)
	{}


	void loadShader(Renderer& context,const string& vert,const string& frag);


	GLuint	mShaderProgram;

	//default value
	GLint	mMvpLoc;

};