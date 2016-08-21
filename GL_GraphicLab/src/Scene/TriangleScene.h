#pragma once

#include "Scene.hpp"

/*
	Basic Example
*/
class TriangleScene : public Scene
{
public:
	TriangleScene(Renderer& renderer);
	~TriangleScene();

	virtual void render()override;

protected:

};