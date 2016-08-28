#pragma once

#include "Scene.hpp"

/*
	Vertex Draw
	Example for VAO VBO
*/
class ParticleScene : public Scene
{
public:
	ParticleScene(Renderer& renderer);
	~ParticleScene();

	GLuint loadTexture(void * ioContext, char * fileName);

	virtual void enter()override;

	virtual void update(float delta)override;

	virtual void render()override;

	virtual void exit()override;

protected:
	
	//Particle data - Uniform location
	GLint timeLoc;
	GLint colorLoc;
	GLint centerPositionLoc;
	GLint samplerLoc;

	// Texture handle
	GLuint textureId;

	// Current time
	float time;

};