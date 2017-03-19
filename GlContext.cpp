#include "GlContext.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>


#include <glm/glm.hpp>
#include <glm/vec2.hpp>// glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/geometric.hpp>// glm::cross, glm::normalize
#include <glm/packing.hpp>// glm::packUnorm2x16
#include <glm/integer.hpp>// glm::uint
#include <glm/gtc/type_precision.hpp>// glm::i8vec2, glm::i32vec2
#include <glm/exponential.hpp>// glm::pow
#include <glm/gtc/random.hpp>// glm::vecRand3

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <GL/freeglut.h>  // ou glut.h - GLUT, include glu.h and gl.h
#include <GL/gl.h>


namespace {
    OpenGLContext* currentInstance = nullptr;
}

#include "./Classes/Camera.h"
#include "./Classes/Drawer.h"
#include "./Classes/Shape.h"
#include "./Classes/LeitorObj.h"

using namespace std;

#define SHADER_PATH "./Shaders/"

#define SCREENWIDTH 640
#define SCREENHEIGHT 480

inline bool _check_gl_error(const char *file, int line) {
        GLenum err (glGetError());

        bool result = false;

        while(err!=GL_NO_ERROR) {
                std::string error;

                switch(err) {
                        case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                        case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                        case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                        case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
                        case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
                }

                std::cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<std::endl;
                err=glGetError();
                result = true;
        }
        return result;
}

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

#define return_check_gl_error() if (_check_gl_error(__FILE__,__LINE__)) return false;


struct CameraStruc {
    int Zoom = 1.0;
};

CameraStruc Camera;


OpenGLContext::OpenGLContext(int argc, char *argv[]) {
	glutInit(&argc, argv);     // Initialize GLUT

	glutInitContextVersion(3, 0);               // IMPORTANT!
	glutInitContextProfile(GLUT_CORE_PROFILE); // IMPORTANT! or later versions, core was introduced only with 3.2
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // IMPORTANT! Double buffering!

	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
	glutCreateWindow("Trabalho de CG -- Ludio, Rogerio");      // Create window with the given title

	glutDisplayFunc(OpenGLContext::glutRenderCallback); // Register callback handler for window re-paint event
	glutReshapeFunc(OpenGLContext::glutReshapeCallback); // Register callback handler for window re-size event
    glutIdleFunc(OpenGLContext::glutRedisplayCallback);

	GLenum error = glewInit();

    glEnable(GL_DEPTH_TEST);

    // Seta camera.
    this->cameraZoom  = 1.0;    
    this->cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

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

	
}

void OpenGLContext::glutReshapeCallback(int width, int height) {
	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
}

void OpenGLContext::glutRenderCallback() {
	currentInstance->render();
}

void OpenGLContext::glutRedisplayCallback()
{
    currentInstance->render();
    glutPostRedisplay();
}

void OpenGLContext::render() const {
	// Clear the colorbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//load everthing back
	glUseProgram(this->programId);
	glBindVertexArray(this->VAOId);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOId);

    drawObjects();

	// Draw our first triangle
	// glDrawArrays(GL_TRIANGLES, 0, 3);

	// //clean things up
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);
	glUseProgram(0);

	glutSwapBuffers();    //necessario para windows!
}

void OpenGLContext::drawObjects() const {
    
    if (objects.size() == 0) {
        cout << "Nenhum objeto na renderizacao " << endl;
        return;
    }
    

    for (auto _drawable : objects)
    {
        _drawable->drawShape();
    }

    check_gl_error();

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
    drawer->listen();
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
	glBindAttribLocation (1, 0, "a_coord");

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

// void OpenGLContext::drawAxis()
// {
//     GLuint VertexArrayID;
//     glGenVertexArrays(10, VertexArrayID);
//     glGenBuffers(18, vertexbuffer);
//     fLineOneColor[0] = 0.0f; fLineOneColor[1] = 0.0f; fLineOneColor[2] = 1.0f;
//     fLineOneColor[3] = 0.0f; fLineOneColor[4] = 0.0f; fLineOneColor[5] = 1.0f;

//     fLineTwoColor[0] = 0.0f; fLineTwoColor[1] = 1.0f; fLineTwoColor[2] = 0.0f;
//     fLineTwoColor[3] = 0.0f; fLineTwoColor[4] = 1.0f; fLineTwoColor[5] = 0.0f;

//     fLineThreeColor[0] = 1.0f; fLineThreeColor[1] = 0.0f; fLineThreeColor[2] = 0.0f;
//     fLineThreeColor[3] = 1.0f; fLineThreeColor[4] = 0.0f; fLineThreeColor[5] = 0.0f;


//     shader.Use();
//         glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, MVP);
//         glBindVertexArray(VertexArrayID[0]);

//         static const GLfloat g_vertex_buffer_data[] = {
//             0, 0, 0,
//              0, 0, 0.5f,
//         };
//         glGenBuffers(18, vertexbuffer);
//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

//         glEnableVertexAttribArray(0);
//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[9]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineOneColor), fLineOneColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//         glBindVertexArray(VertexArrayID[1]);

//         static const GLfloat vertex_buffer_data[] = {
//              0, 0, 0,
//              0,  0.5f, 0,
//         };

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(0);
//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[10]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineTwoColor), fLineTwoColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//         glBindVertexArray(VertexArrayID[2]);

//         static const GLfloat vertex_buffer_data3[] = {
//              0, 0, 0,
//              0.5f, 0, 0,
//         };

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data3), vertex_buffer_data3, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(0);
//     //    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[11]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineThreeColor), fLineThreeColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//         glBindVertexArray(VertexArrayID[3]);

//         static const GLfloat vertex_buffer_data4[] = {
//              0.5f, 0, 0,
//              0.4f,  0.1f, 0,
//         };

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[3]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data4), vertex_buffer_data4, GL_STATIC_DRAW);

//         glEnableVertexAttribArray(0);
//     //    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[12]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineThreeColor), fLineThreeColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//         glBindVertexArray(VertexArrayID[4]);

//         static const GLfloat vertex_buffer_data5[] = {
//             0.5f, 0, 0,
//             0.4f, -0.1f, 0,
//         };

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[4]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data5), vertex_buffer_data5, GL_STATIC_DRAW);

//         glEnableVertexAttribArray(0);
//     //    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer5);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[13]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineThreeColor), fLineThreeColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//         glBindVertexArray(VertexArrayID[5]);

//         static const GLfloat vertex_buffer_data6[] = {
//             0, 0.5f, 0,
//              0.1f,  0.4f, 0,
//         };

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[5]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data6), vertex_buffer_data6, GL_STATIC_DRAW);

//         glEnableVertexAttribArray(0);
//     //    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer6);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[14]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineTwoColor), fLineTwoColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//         glBindVertexArray(VertexArrayID[6]);

//         static const GLfloat vertex_buffer_data7[] = {
//             0, 0.5f, 0,
//              -0.1f,  0.4f, 0,
//         };

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[6]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data7), vertex_buffer_data7, GL_STATIC_DRAW);

//         glEnableVertexAttribArray(0);
//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[6]);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[15]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineTwoColor), fLineTwoColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//         glBindVertexArray(VertexArrayID[7]);

//         static const GLfloat vertex_buffer_data8[] = {
//             0, 0, 0.5f,
//              -0.1f, 0, 0.4f,
//         };

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[7]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data8), vertex_buffer_data8, GL_STATIC_DRAW);

//         glEnableVertexAttribArray(0);
//     //    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer8);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[16]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineOneColor), fLineOneColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//         glBindVertexArray(VertexArrayID[8]);

//         static const GLfloat vertex_buffer_data9[] = {
//             0, 0, 0.5f,
//              0.1f, 0, 0.4f,
//         };

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[8]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data9), vertex_buffer_data9, GL_STATIC_DRAW);

//         glEnableVertexAttribArray(0);
//     //    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer9);
//         glVertexAttribPointer(
//             0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//             3,                  // size
//             GL_FLOAT,           // type
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             (void*)0            // array buffer offset
//         );
//         glDrawArrays(GL_LINES, 0, 2);

//         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[17]);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(fLineOneColor), fLineOneColor, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

//     //Destroy vao and vbo
//     glDeleteBuffers(20, vertexbuffer);
//     glDeleteVertexArrays(10, VertexArrayID);


// }


/* 
* Tratar Drawer.
*
*/

//
// Constructor
//
Drawer::Drawer()
{   
    //cout << "Constructor " << endl;
}

//
// Destructor
//
Drawer::~Drawer()
{
    //cout << "destructing " << endl;
}


bool 
Drawer::isValidShape(string type)
{
    if (type.compare("cube") == 0 || type.compare("sphere") == 0 || type.compare("cone") == 0 || type.compare("torus") == 0)
           return true;
    
    return false;
}

//
// Funcao pra ficar escutando os comandos 
// solicitados pelo usuario no terminal
//
void 
Drawer::listen()
{
    cout << "Initializing openGL " << endl;
    string command = "", c1 = "", c2 = "",  c3 = "", c4 = "";
    float f1, f2, f3, f4;
         
    // Fica lendo ateh escrever "quit"
    while ( 1 ) {
        f1 = f2 = f3 = f4 = 0.0;
        cout << "Digite seu comando " << endl;
        currentInstance->render();

        command.clear();
        cin.clear();

        cin >> command;
        c1.clear();
        c2.clear();
        c3.clear();
        c4.clear();
        
        //cin >> command;

        if (command.compare("add_shape") == 0)
        {
            // add_shape...
            cin >> c1;
            cin >> c2;
            addShape(c1, c2);            
        }
        else if (command.compare("remove_shape") == 0)
        {
            // remove_shape.
            cin >> c1;
            removeShape(c1);
        }
        else if (command.compare("add_light") == 0)
        {
            // add_light
            cin >> c1; // lightname
            cin >> f1 >> f2 >> f3; // float 
            addLight(c1, f1, f2, f3);
        }
        else if (command.compare("remove_light") == 0)
        {
            // remove_light
            cin >> c1;
            removeLight(c1);
        }
        else if (command.compare("reflection_on") == 0)
        {
            cin >> c1;
            cin >> f1;
            enableReflection(c1, f1);
        }
        else if (command.compare("reflection_off") == 0)
        {
            cin >> c1;
            cin >> f1;
            disableReflection(c1, f1);
        }
        else if (command.compare("shading") == 0)
        {
            cin >> c1;
            setShading(c1);
        }
        else if (command.compare("projection") == 0)
        {
            cin >> c1;
            setProjection(c1);
        }
        else if (command.compare("translate") == 0)
        {
            cin >> c1;
            cin >> f1 >> f2 >> f3;
            setTranslate(c1, f1, f2, f3);            
        }
        else if (command.compare("scale") == 0)
        {
            cin >> c1;
            cin >> f1 >> f2 >> f3;
            setScale(c1, f1, f2, f3);
        }
        else if (command.compare("rotate") == 0)
        {
            cin >> c1;
            cin >> f1 >> f2 >> f3 >> f4;
            setRotate(c1, f1, f2, f3, f4);
        }
        else if (command.compare("lookat") == 0)
        {
            cin >> f1 >> f2 >> f3;
            setLookAt(f1, f2, f3); 
        }
        else if (command.compare("cam") == 0)
        {
            cin >> f1 >> f2 >> f3;
            setCam(f1, f2, f3);
        }
        else if (command.compare("color") == 0)
        {
            cin >> c1;
            cin >> f1 >> f2 >> f3;
            setColor(c1, f1, f2, f3);
        }
        else if (command.compare("axis") == 0)
        {
            axis();
        }
        else if (command.compare("save") == 0)
        {
            cin >> c1;
            cout << "Saving into file... " << c1;
            save(c1);
        }
        else if (command.compare("quit") == 0)
        {
            this->exit();
            cout << "Program finished." << endl;
            //currentInstance->finalize();
            delete currentInstance;            
            break;
        }
        else {
            cout << "Command not recognized" << endl;
        }

    }
    
}


//
// Adicionar um shape.
//
void
Drawer::addShape(string type, string shapeName)
{ 

    // Verifica se eh um shape valido
    if (this->isValidShape(type)) {
        // caso seja valido. entao cria a parada.
        cout << "Ok!! Creating " << type << ": " << shapeName << endl;
        // ponteiro bunitnhiu
        Shape *customShape;
        int objectSize = currentInstance->objects.size();
        // esse objectsize eh como se fosse pra pegar o ID do shape.. ta lgd??
        customShape = new Shape(objectSize, type, shapeName);
        currentInstance->objects.push_back(customShape);
    }
    else {
        // ok , nao pode fazer a criacao desse shape
        cout << "Shape not recognized." << endl;    
    }    
    //console_log("Ok!! Creating shape > " + shapeName);
};


// nao sei pra q eu fiz essa funcao 
// achei melhor percorrer toda vez... pq dai eu n preciso ficar manipulando o objeto ....
int
Drawer::findShape(string shapeName)
{
    for (int i = 0; i < currentInstance->objects.size(); i++)
    {
        if (currentInstance->objects[i]->getName().compare(shapeName) == 0)
            return currentInstance->objects[i]->getShapeId();
    }
    return -1;
}


// Remove um shape do vector. 
// Verifica se existe um vector com o nome
// e remove pelo indice... 
void
Drawer::removeShape(string shapeName)
{
    for (int i = 0; i < currentInstance->objects.size(); i++)
    {
        if (currentInstance->objects[i]->getName().compare(shapeName) == 0)
            currentInstance->objects.erase(currentInstance->objects.begin() + i);            
    }
};


// Adiciona luz no shape
void 
Drawer::addLight(string shapeName, float f1, float f2, float f3)
{
    // // find by shapename
    // int shapeId = this->findShape(shapeName);
    
    // // Find..
    // if (shapeId != -1)
    // {
    //     // Update light
    //     this->updateLight(shapeId, true, f1, f2, f3);
    // }
};


// vei acho q nem precisa comentar, esses metodos tao mt intuitivo...
// Remove a luz do shape ()
void 
Drawer::removeLight(string shapeName)
{
    // int shapeId = this->findShape(shapeName);
    
    // if (shapeId != -1)
    // {
    //     this->destroyLight(shapeId);
    // }    
};

void 
Drawer::enableReflection(string Type, float CoefficientK)
{
    if (Type.compare("specular") == 0 || Type.compare("diffuse") == 0 || Type.compare("ambient") == 0)
    {
        cout << "Enabling reflection on " << Type << " with coefficient K in " << CoefficientK << endl;
    }
    else 
    {
        cout << "Reflection not recognized" << endl;
    }
};

void 
Drawer::disableReflection(string, float)
{

};
        
void 
Drawer::setShading(string)
{

};
        
void 
Drawer::setProjection(string type)
{

};
        

// Metodos pra trabalhar com transformacao.        
void 
Drawer::setTranslate(string shapeName, float x, float y, float z)
{
    for (auto _drawable : currentInstance->objects)
    {
        if (_drawable->getName().compare(shapeName) == 0)
        {
            _drawable->setTranslateable(x, y, z);
        }        
    }
};
        
void 
Drawer::setScale(string shapeName, float x, float y, float z)
{
    for (auto _drawable : currentInstance->objects)
    {
        if (_drawable->getName().compare(shapeName) == 0)
        {
            _drawable->setScalable(x, y, z);
        }        
    }
};
        
void 
Drawer::setRotate(string shapeName, float angle, float x, float y, float z)
{
    for (auto _drawable : currentInstance->objects)
    {
        if (_drawable->getName().compare(shapeName) == 0)
        {
            _drawable->setRotateable(angle, glm::vec3(x, y, z));
        }        
    }
};
        
void
Drawer::setLookAt(float, float, float)
{

};
        
void 
Drawer::setCam(float x, float y, float z)
{   
    currentInstance->cameraPos = glm::vec3(x, y, z);
    cout << "Setting camera to " << x << " " << y << " " << z << endl;
};
        
void 
Drawer::setColor(string shapeName, float r, float g, float b)
{
    for (auto _drawable : currentInstance->objects)
    {
        if (_drawable->getName().compare(shapeName) == 0)
        {
            cout << "Ok! Color for this shape now is " << r << " " << g  << " " << b << endl;
            _drawable->setColor(r, g, b);
        }        
    }
};
        
void 
Drawer::axis()
{

};
        
void 
Drawer::save(string filename)
{

};
        
void 
Drawer::exit()
{
    cout << "Finalizando openGL ! u.u" << endl;
};




// CLASSES FOR SHAPE 

// .
// . Inicia o Shape
// .
Shape::Shape(int shapeId, string t, string shapeName)
{
    this->shapeId   = shapeId;
    this->type      = t;
    this->shapeName = shapeName;

    // Cor inicial eh um verde mt loko...
    this->colorR = 0.0;
    this->colorG = 1.0;
    this->colorB = 0.0;

    this->rotateable = false;
    this->scalable = false;
    this->translateable = false;

    // sem transladar.
    this->transX = 0.0;
    this->transY = 0.0;
    this->transZ = 0.0;

    // rotacao inicial
    this->anguloRotacao = 0.0;
    this->DirecaoRotate = glm::vec3(0.0, 0.0, 1.0);
    this->scaleCoordinates = glm::vec3(1.0, 1.0, 1.0);

    //this->drawShape();
}


// .  Funcao principal pra fazer a diferenciacao
// .  dos shapes e tratar
// .  o que vai ser iniciado

void
Shape::drawShape()
{
    // Objectpath 
    string filePath = "./BlenderObjs/";
    LeitorObj *leitorObj = new LeitorObj();
    
    if (this->type.compare("cone") == 0) 
        filePath += "Cone.obj";            
    else if (this->type.compare("sphere") == 0) 
        filePath += "Sphere.obj";        
    else if (this->type.compare("cube") == 0)  
        filePath += "Cube.obj";
    else if (this->type.compare("torus") == 0) 
        filePath += "Torus.obj";
    else {
        cout << "Seloco tio, n tem esse shape aqui n... Ce ta trapaceando?" << endl;
        return;
    }

    // Carrega o arquivo .
    leitorObj->Load(filePath);

    // Preenche os vetores com os dados do arquivo OBJ 
    leitorObj->Fill(this->data, this->normals, this->colors);

    // glGenBuffers(1, &VBO);
    // Inicializa os buffers.
    this->initializeBuffers();

    // Pega o valor VAO setado na funcao anterior.
    glBindVertexArray(VAO_);
    
    // GLUint lightVAO;
    // glGenVertexArrays(1, &lightVAO);
    // glBindVertexArray(lightVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GLint modelLoc = glGetUniformLocation(currentInstance->getProgramId(), "model");
    glBindVertexArray(VAO_);
    glm::mat4 model;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, this->data.size() * sizeof(glm::vec3));
    glBindVertexArray(0);
    
    
}


void 
Shape::initializeBuffers()
{
        
    int programId = currentInstance->getProgramId();        
    cout << "ProgramID > " << programId << endl;
    glUseProgram(programId);  // 
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    // bind nos vertices.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);    
    GLuint bufferVerticeSize = data.size() * sizeof(glm::vec3);

    glBufferData(GL_ARRAY_BUFFER, bufferVerticeSize, &data[0], GL_STATIC_DRAW);    
    glBindVertexArray(VAO_);
    
    // positions attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    // normals.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    // Os vertices sao o mesmo p objeto lgiht;
    // setar Vao, VBO eh o mesmo!!!! 
    GLuint lightVAO;    
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, data.size() * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    // nesse ponto?!
    // Clear the colorbuffer
    /*glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

    // Trata transformacao da Camera;
    glm::mat4 view;
    view = glm::lookAt(currentInstance->cameraPos, currentInstance->cameraPos + currentInstance->cameraFront, currentInstance->cameraUp);
    glm::mat4 projection = glm::perspective(currentInstance->cameraZoom, (float)SCREENWIDTH/(float)SCREENHEIGHT, 0.1f, 100.0f);
    // Pega os uniformes.
    
    // check_gl_error();


   
    GLint viewLoc = glGetUniformLocation(currentInstance->getProgramId(), "view");
    GLint projLoc = glGetUniformLocation(currentInstance->getProgramId(), "projection");
    
    // Passa as matrizes. pro shader.
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        

    // check_gl_error();

    

    // Cores.
    // Atualiza uniform de cores.
    GLint vertexColorLocation = glGetUniformLocation(currentInstance->getProgramId(), "ourColor");
    // atribui cor verde.
    glUniform4f(vertexColorLocation, this->colorR, this->colorG, this->colorB, 1.0f);

    // Create transformations
    glm::mat4 transform;
    transform = glm::scale(transform, this->scaleCoordinates);
    transform = glm::translate(transform, glm::vec3(this->transX, this->transY, this->transZ));
    transform = glm::rotate(transform, this->anguloRotacao, this->DirecaoRotate);

    // Get matrix's uniform location and set matrix
    GLint transformLoc = glGetUniformLocation(currentInstance->getProgramId(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));        
}
















