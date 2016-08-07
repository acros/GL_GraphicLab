#pragma once

#include "Scene.hpp"

class TriangleScene : public Scene
{
public:
	TriangleScene(Renderer& renderer);
	~TriangleScene();

	virtual void render()override;

protected:

};