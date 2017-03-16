#include "GlContext.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <GL/freeglut.h>  // ou glut.h - GLUT, include glu.h and gl.h
#include <GL/gl.h>
#include "./Classes/Camera.h"
#include "./Classes/Drawer.h"

#define SHADER_PATH "./Shaders/"

//using namespace std;

namespace {
OpenGLContext* currentInstance = nullptr;
}

OpenGLContext::OpenGLContext(int argc, char *argv[]) {
	glutInit(&argc, argv);     // Initialize GLUT

	glutInitContextVersion(3, 0);               // IMPORTANT!
	glutInitContextProfile(GLUT_CORE_PROFILE); // IMPORTANT! or later versions, core was introduced only with 3.2
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // IMPORTANT! Double buffering!

	glutInitWindowSize(640, 480);
	glutCreateWindow("Trabalho de CG -- Ludio, Rogerio");      // Create window with the given title

	glutDisplayFunc(OpenGLContext::glutRenderCallback); // Register callback handler for window re-paint event
	glutReshapeFunc(OpenGLContext::glutReshapeCallback); // Register callback handler for window re-size event

	GLenum error = glewInit();

	if (error != GLEW_OK) {
		throw std::runtime_error("Error initializing GLEW.");
	}

	currentInstance = this;

	this->initialize();
}

OpenGLContext::~OpenGLContext() {
	this->finalize();
}

void OpenGLContext::initialize() {
	// Set "clearing" or background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque

	// Create and compile our GLSL program from the shaders
    std::string vertexFile = std::string(SHADER_PATH) + "/Cg.vp";
    std::string fragmentFile = std::string(SHADER_PATH) + "/Cg.fp";
	GLint vertexShaderId = this->loadAndCompileShader(vertexFile,
	GL_VERTEX_SHADER);
	GLint fragmentShaderId = this->loadAndCompileShader(fragmentFile,
	GL_FRAGMENT_SHADER);
	this->programId = this->linkShaderProgram(vertexShaderId, fragmentShaderId);

	// Create and Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glGenVertexArrays(1, static_cast<GLuint*>(&this->VAOId));
	glBindVertexArray(static_cast<GLuint>(this->VAOId));

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = { -0.5f, -0.5f, 0.0f, // Left
			0.5f, -0.5f, 0.0f, // Right
			0.0f, 0.5f, 0.0f  // Top
			};
	glGenBuffers(1, static_cast<GLuint*>(&this->VBOId));
	glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(this->VBOId));
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 0=location do atributo no shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
			(GLvoid*) 0);
	glBindAttribLocation(this->programId, 0, "vertexPosition");//name of attribute in shader
	//name of attribute in shader
	glEnableVertexAttribArray(0);

	// Note that this is allowed, the call to glVertexAttribPointer registered VBO
	// as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	glBindVertexArray(0);
}

void OpenGLContext::glutReshapeCallback(int width, int height) {
	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
}

void OpenGLContext::glutRenderCallback() {
	currentInstance->render();
}

void OpenGLContext::render() const {
	// Clear the colorbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//load everthing back
	glUseProgram(this->programId);
	glBindVertexArray(this->VAOId);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOId);

	// Draw our first triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//clean things up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glutSwapBuffers();    //necessario para windows!
}

void OpenGLContext::printVersion() const {
	std::string glRender = reinterpret_cast<char const*>(glGetString(
	GL_RENDERER));
	std::string glVersion = reinterpret_cast<char const*>(glGetString(
	GL_VERSION));
	std::string glslVersion = reinterpret_cast<char const*>(glGetString(
	GL_SHADING_LANGUAGE_VERSION));

	std::cout << "OpenGL Renderer  : " << glRender << '\n'
			<< "OpenGL Version   : " << glVersion << '\n'
			<< "OpenGLSL Version : " << glslVersion << std::endl;
}

void OpenGLContext::runLoop() const {
	this->render();
    this->getCommands();    
	glutMainLoop();
}

void OpenGLContext::getCommands() const {
    // 
    Drawer* drawer = new Drawer();
    drawer->listen(this);
}

void OpenGLContext::finalize() const {
	// Properly de-allocate all resources once they've outlived their purpose
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &(this->VAOId));
	glDeleteBuffers(1, &(this->VBOId));
	glUseProgram(0);
}

unsigned int OpenGLContext::loadAndCompileShader(const std::string &filename,
		const int glType) const {
	std::string shaderCode;

	// Read the Vertex Shader code from the file
	{
		// Open file stream
		std::ifstream shaderStream(filename, std::ios::in);

		if (shaderStream.good() == false) {
			throw std::runtime_error(
					"Error when opening " + filename
							+ ". Are you in the right directory ?");
		}

		// Copy file contents to string
		shaderCode.assign((std::istreambuf_iterator<char>(shaderStream)),
				(std::istreambuf_iterator<char>()));

	} // End of scope, RAII shaderStream closed.

	// Some temporaries for using in the code ahead...
	char const * sourcePointer = shaderCode.c_str(); // C style pointer to string
	GLint result = GL_FALSE;               // Return result for OGL API calls
	int infoLogLength = 0;                   // Length of log (in case of error)
	unsigned int shaderId = glCreateShader(glType); // Create a shader on GPU

	// Compile Shader
	std::cout << "Compiling shader : " << filename << std::endl;

	glShaderSource(shaderId, 1, &sourcePointer, nullptr);
	glCompileShader(shaderId);

	// Check Shader
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	// In case of error...
	if (result == GL_FALSE) {
		// Allocate vector of char to store error message
		std::vector<char> errorMessage(infoLogLength + 1);

		// Copy error message to string
		glGetShaderInfoLog(shaderId, infoLogLength, nullptr, &errorMessage[0]);

		// Print error message
		std::cerr << "Error compiling " << filename << '\n' << "  "
				<< std::string(errorMessage.begin(), errorMessage.end())
				<< std::endl;
	}

	return shaderId;
}

unsigned int OpenGLContext::linkShaderProgram(unsigned int vertexShaderId,
		unsigned int fragmentShaderId) const {
	// Create shader program on GPU
	unsigned int shaderProgramId = glCreateProgram();

	// Attach shader to be linked
	glAttachShader(shaderProgramId, vertexShaderId);
	glAttachShader(shaderProgramId, fragmentShaderId);

	// Setup Vertex Attributes (only for GL < 3.3 and GLSL < 3.3)
	// glBindAttribLocation (this->getId(), 0, "vertexPosition_modelspace");

	// Link the program!
	glLinkProgram(shaderProgramId);

	// Some temporaries for using in the code ahead...
	GLint result = GL_FALSE;
	int InfoLogLength = 0;

	// Check the program
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &result);
	glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// In case of error...
	if (result == GL_FALSE) {
		// Allocate vector of char to store error message
		std::vector<char> errorMessage(InfoLogLength + 1);

		// Copy error message to string
		glGetProgramInfoLog(shaderProgramId, InfoLogLength, nullptr,
				&errorMessage[0]);

		// Print error message
		std::cerr << std::string(errorMessage.begin(), errorMessage.end())
				<< std::endl;
	}

	// Detach...
	glDetachShader(shaderProgramId, vertexShaderId);
	glDetachShader(shaderProgramId, fragmentShaderId);

	// ...  and delete the shaders source code, work done
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return shaderProgramId;
}
