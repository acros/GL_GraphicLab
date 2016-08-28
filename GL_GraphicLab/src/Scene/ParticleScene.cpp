#include "ParticleScene.h"


// Particle vertex data
static const int NUM_PARTICLES = 1000;
static const int PARTICLE_SIZE = 7;

float sParticleData[NUM_PARTICLES * PARTICLE_SIZE];

#define ATTRIBUTE_LIFETIME_LOCATION       0
#define ATTRIBUTE_STARTPOSITION_LOCATION  1
#define ATTRIBUTE_ENDPOSITION_LOCATION    2


ParticleScene::ParticleScene(Renderer& renderer)
	: Scene(renderer)
	, textureId(-1)
{
	mVertStr =
		"#version 300 es                                      \n"
		"uniform float u_time;                                \n"
		"uniform vec3 u_centerPosition;                       \n"
		"layout(location = 0) in float a_lifetime;            \n"
		"layout(location = 1) in vec3 a_startPosition;        \n"
		"layout(location = 2) in vec3 a_endPosition;          \n"
		"out float v_lifetime;                                \n"
		"void main()                                          \n"
		"{                                                    \n"
		"  if ( u_time <= a_lifetime )                        \n"
		"  {                                                  \n"
		"    gl_Position.xyz = a_startPosition +              \n"
		"                      (u_time * a_endPosition);      \n"
		"    gl_Position.xyz += u_centerPosition;             \n"
		"    gl_Position.w = 1.0;                             \n"
		"  }                                                  \n"
		"  else                                               \n"
		"  {                                                  \n"
		"     gl_Position = vec4( -1000, -1000, 0, 0 );       \n"
		"  }                                                  \n"
		"  v_lifetime = 1.0 - ( u_time / a_lifetime );        \n"
		"  v_lifetime = clamp ( v_lifetime, 0.0, 1.0 );       \n"
		"  gl_PointSize = ( v_lifetime * v_lifetime ) * 40.0; \n"
		"}";

	mFragStr =
		"#version 300 es                                      \n"
		"precision mediump float;                             \n"
		"uniform vec4 u_color;                                \n"
		"in float v_lifetime;                                 \n"
		"layout(location = 0) out vec4 fragColor;             \n"
		"uniform sampler2D s_texture;                         \n"
		"void main()                                          \n"
		"{                                                    \n"
		"  vec4 texColor;                                     \n"
		"  texColor = texture( s_texture, gl_PointCoord );    \n"
		"  fragColor = vec4( u_color ) * texColor;            \n"
		"  fragColor.a *= v_lifetime;                         \n"
		"}                                                    \n";
}

ParticleScene::~ParticleScene()
{

}

GLuint ParticleScene::loadTexture(void *ioContext, char *fileName)
{
	int width,
		height;
	char *buffer = esLoadTGA(ioContext, fileName, &width, &height);
	GLuint texId;

	if (buffer == NULL)
	{
//		esLogMessage("Error loading (%s) image.\n", fileName);
		return 0;
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	free(buffer);

	return texId;
}
void ParticleScene::enter()
{
	Scene::enter();
	
//	userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

	// Get the uniform locations
	timeLoc = glGetUniformLocation(mShaderProgram, "u_time");
	centerPositionLoc = glGetUniformLocation(mShaderProgram, "u_centerPosition");
	colorLoc = glGetUniformLocation(mShaderProgram, "u_color");
	samplerLoc = glGetUniformLocation(mShaderProgram, "s_texture");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Fill in particle data array
	srand(0);

	// LifeTime (x1 byte) + EndPos Vec3 (x3 bytes) + StartPos Vec3 x （x3 bytes)
	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		float *particleData = &sParticleData[i * PARTICLE_SIZE];

		// Lifetime of particle
		(*particleData++) = ((float)(rand() % 10000) / 10000.0f);

		// End position of particle
		(*particleData++) = ((float)(rand() % 10000) / 5000.0f) - 1.0f;
		(*particleData++) = ((float)(rand() % 10000) / 5000.0f) - 1.0f;
		(*particleData++) = ((float)(rand() % 10000) / 5000.0f) - 1.0f;

		// Start position of particle
		(*particleData++) = ((float)(rand() % 10000) / 40000.0f) - 0.125f;
		(*particleData++) = ((float)(rand() % 10000) / 40000.0f) - 0.125f;
		(*particleData++) = ((float)(rand() % 10000) / 40000.0f) - 0.125f;

	}

	// Initialize time to cause reset on first update
	time = 1.0f;

	textureId = loadTexture(nullptr, "C:\\codingEnv\\GL_GraphicLab\\x64\\Debug\\smoke.tga");

	assert(textureId >= 0);
}

void ParticleScene::update(float delta)
{
	time += delta;
	if (time >= 1.0f)
	{
		time = 0.0f;

		// Use the program object
		glUseProgram(mShaderProgram);

		float centerPos[3];
		float color[4];

		// Pick a new start location and color
		centerPos[0] = ((float)(rand() % 10000) / 10000.0f) - 0.5f;
		centerPos[1] = ((float)(rand() % 10000) / 10000.0f) - 0.5f;
		centerPos[2] = ((float)(rand() % 10000) / 10000.0f) - 0.5f;

		glUniform3fv(centerPositionLoc, 1, &centerPos[0]);

		// Random color
		color[0] = ((float)(rand() % 10000) / 20000.0f) + 0.5f;
		color[1] = ((float)(rand() % 10000) / 20000.0f) + 0.5f;
		color[2] = ((float)(rand() % 10000) / 20000.0f) + 0.5f;
		color[3] = 0.5;

		glUniform4fv(colorLoc, 1, &color[0]);

	}

	// Load uniform time variable
	glUniform1f(timeLoc, time);
}

void ParticleScene::render()
{
	mRendererRef.beginDraw();

	glUseProgram(mShaderProgram);

	// Load the vertex attributes
	glVertexAttribPointer(ATTRIBUTE_LIFETIME_LOCATION, 1, GL_FLOAT,GL_FALSE, PARTICLE_SIZE * sizeof(GLfloat),sParticleData);
	glVertexAttribPointer(ATTRIBUTE_ENDPOSITION_LOCATION, 3, GL_FLOAT,GL_FALSE, PARTICLE_SIZE * sizeof(GLfloat),&sParticleData[1]);
	glVertexAttribPointer(ATTRIBUTE_STARTPOSITION_LOCATION, 3, GL_FLOAT,GL_FALSE, PARTICLE_SIZE * sizeof(GLfloat),&sParticleData[4]);

	glEnableVertexAttribArray(ATTRIBUTE_LIFETIME_LOCATION);
	glEnableVertexAttribArray(ATTRIBUTE_ENDPOSITION_LOCATION);
	glEnableVertexAttribArray(ATTRIBUTE_STARTPOSITION_LOCATION);

	// Blend particles
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set the sampler texture unit to 0
	glUniform1i(samplerLoc, 0);

	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	mRendererRef.endDraw();
}


void ParticleScene::exit()
{
	// Delete texture object
	glDeleteTextures(1, &textureId);
}

