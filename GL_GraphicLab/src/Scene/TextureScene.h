#pragma once

#include "Scene.hpp"

class TextureScene : public Scene
{
public:
	TextureScene(Renderer& renderer);
	~TextureScene();

	virtual void enter()override;

	virtual void render()override;

protected:

	GLint mSampleLocation;
	GLuint mTexId;

};