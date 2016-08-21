#include "Material.h"

void Material::loadShader(Renderer& context,const string& vert, const string& frag)
{
	string vertStr =
		"#version 300 es                                   \n"
		"uniform mat4 u_mvpMatrix;                         \n"
		"layout(location = 0) in vec4 a_position;          \n"
		"layout(location = 1) in vec4 a_color;             \n"			//TODO: use texCoord here
		"out vec4 v_color;                                 \n"
		"													\n"
		"void main()                                       \n"
		"{                                                 \n"
		"   v_color = a_color;                             \n"
		"   gl_Position = u_mvpMatrix * a_position;        \n"
		"}                                                 \n";
	string fragStr =
		"#version 300 es                                    \n"
		"precision lowp float;                              \n"
		"in vec4 v_color;                                   \n"
		"layout(location = 0) out vec4 outColor;            \n"
		"                                                   \n"
		"void main()                                        \n"
		"{                                                  \n"
		"   outColor = v_color ;	                        \n"
		"}                                                  \n";

	mShaderProgram = context.loadShaderProgram(vertStr.c_str(), fragStr.c_str());

	mMvpLoc = glGetUniformLocation(mShaderProgram, "u_mvpMatrix");

}
