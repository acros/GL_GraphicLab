#pragma once

#include "Scene.hpp"

/*
	Example For GL Texture use
*/
class TextureScene : public Scene
{
public:
	TextureScene(Renderer& renderer);
	~TextureScene();

	virtual void enter()override;

	virtual void render()override;

protected:

	GLuint createSimpleTexture2D();

	GLint mSampleLocation;
	GLuint mTexId;

};