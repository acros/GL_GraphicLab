#include "TextureScene.h"


TextureScene::TextureScene(Renderer& renderer)
	: Scene(renderer)
	, mSampleLocation(0)
	, mTexId(0)
{
	mVertStr =
		"#version 300 es                            \n"
		"layout(location = 0) in vec4 a_position;   \n"
		"layout(location = 1) in vec2 a_texCoord;   \n"
		"out vec2 v_texCoord;                       \n"
		"void main()                                \n"
		"{                                          \n"
		"   gl_Position = a_position;               \n"
		"   v_texCoord = a_texCoord;                \n"
		"}                                          \n";

	mFragStr =
		"#version 300 es                                     \n"
		"precision mediump float;                            \n"
		"in vec2 v_texCoord;                                 \n"
		"layout(location = 0) out vec4 outColor;             \n"
		"uniform sampler2D s_texture;                        \n"
		"void main()                                         \n"
		"{                                                   \n"
		"  outColor = texture( s_texture, v_texCoord );      \n"
		"}                                                   \n";

}


TextureScene::~TextureScene()
{
	// Delete texture object
	glDeleteTextures(1, &mTexId);
}

void TextureScene::enter()
{
	Scene::enter();

	// Set the sampler texture unit to 0
	mSampleLocation = glGetUniformLocation(mShaderProgram, "s_texture");

	mTexId = mRendererRef.createSimpleTexture2D();
}

void TextureScene::render()
{
	mRendererRef.beginDraw();

	GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f,  // Position 0
		0.0f,  0.0f,        // TexCoord 0 
		-0.5f, -0.5f, 0.0f,  // Position 1
		0.0f,  1.0f,        // TexCoord 1
		0.5f, -0.5f, 0.0f,  // Position 2
		1.0f,  1.0f,        // TexCoord 2
		0.5f,  0.5f, 0.0f,  // Position 3
		1.0f,  0.0f         // TexCoord 3
	};
	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	// Use the program object
	glUseProgram(mShaderProgram);

	// Load the vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices);
	// Load the texture coordinate
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexId);

	glUniform1i(mSampleLocation, 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	mRendererRef.endDraw();
}
