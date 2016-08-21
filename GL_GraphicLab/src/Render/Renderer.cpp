#include "Renderer.h"

Renderer::Renderer()
	: mProgramObject(0)
{

}

Renderer::~Renderer()
{
	if (mProgramObject != 0)
	{
		glDeleteProgram(mProgramObject);
	}
}

void Renderer::setViewport(int width, int height)
{
	glViewport(0, 0, width, height);

	glClearColor(0.1f, 0.1f, 0.1f, 0.f);
}

void Renderer::checkRendererVersion() const
{
	//Check glVersion
    const char* glVer = (const char*)glGetString(GL_VERSION);
    logMessage("OpenGL-ES version is : %s.\n",glVer);
    
    const char* glShaderVer = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    logMessage("OpenGL-ES shader version is : %s.\n",glShaderVer);
}

GLuint Renderer::loadShaderProgram(const string& vertStr, const string& fragStr)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = loadShader(GL_VERTEX_SHADER, vertStr.c_str());
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragStr.c_str());

	// Create the program object
	mProgramObject = glCreateProgram();

	if (mProgramObject == 0)
	{
		return 0;
	}

	glAttachShader(mProgramObject, vertexShader);
	glAttachShader(mProgramObject, fragmentShader);

	// Link the program
	glLinkProgram(mProgramObject);

	// Check the link status
	glGetProgramiv(mProgramObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(mProgramObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char *infoLog = (char*)malloc(sizeof(char) * infoLen);

			glGetProgramInfoLog(mProgramObject, infoLen, NULL, infoLog);
			logMessage("Error linking program:\n%s\n", infoLog);

			free(infoLog);
		}

		glDeleteProgram(mProgramObject);
		return 0;
	}

	return mProgramObject;
}


void Renderer::beginDraw()
{
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw()
{

}

void Renderer::endDraw()
{

}

GLuint Renderer::loadShader(GLenum type, const char *shaderSrc)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
	{
		return 0;
	}

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		GLenum sx = glGetError();
		logMessage("GL state error: %d", sx);

		if (infoLen > 1)
		{
			char *infoLog = (char*)malloc(sizeof(char) * infoLen);

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			logMessage("Error compiling shader:\n%s\n", infoLog);

			free(infoLog);
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}
