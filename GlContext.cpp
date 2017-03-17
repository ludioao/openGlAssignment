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


//using namespace std;

namespace {
OpenGLContext* currentInstance = nullptr;
}


#include "./Classes/Camera.h"
#include "./Classes/Drawer.h"
#include "./Classes/Shape.h"

#define SHADER_PATH "./Shaders/"

OpenGLContext::OpenGLContext(int argc, char *argv[]) {
	glutInit(&argc, argv);     // Initialize GLUT

	glutInitContextVersion(3, 0);               // IMPORTANT!
	glutInitContextProfile(GLUT_CORE_PROFILE); // IMPORTANT! or later versions, core was introduced only with 3.2
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // IMPORTANT! Double buffering!

	glutInitWindowSize(640, 480);
	glutCreateWindow("Trabalho de CG -- Ludio, Rogerio");      // Create window with the given title

	glutDisplayFunc(OpenGLContext::glutRenderCallback); // Register callback handler for window re-paint event
	glutReshapeFunc(OpenGLContext::glutReshapeCallback); // Register callback handler for window re-size event
    glutIdleFunc(OpenGLContext::glutRedisplayCallback);

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
    
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i]->drawShape();
    }

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




/* 
* Tratar Drawer.
*
*/

//
// Constructor
//
Drawer::Drawer()
{   
    cout << "Constructor " << endl;
}

//
// Destructor
//
Drawer::~Drawer()
{
    cout << "destructing " << endl;
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

    string command = "";

    string  c1 = "",
            c2 = "",
            c3 = "",
            c4 = "";

    auto algumaCoisa = currentInstance->getVAOId();

    cout << "alguma coisa dentro do drawer eh " << algumaCoisa;

    float   
            f1 = 0.0,
            f2 = 0.0,
            f3 = 0.0,
            f4 = 0.0;            

    // read until exit is needed.
    while ( 1 ) {
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
            exit();
            cout << "Program finished." << endl;
            break;
        }
        else {
            cout << "Command not recognized" << endl;
        }

    }
    
}


//
// Metodos de Comando 
//


//
// Adicionar um shape.
//
void
Drawer::addShape(string type, string shapeName)
{ 

    if (this->isValidShape(type)) {

        cout << "Ok!! Creating " << type << ": " << shapeName << endl;

        Shape *customShape;

        if (type.compare("cone") == 0)
        {
            // first element... 
             customShape = new Shape(0, type, shapeName);
             currentInstance->objects.push_back(customShape);
        }
    }
    else {
        cout << "Shape not recognized." << endl;    
    }    
    //console_log("Ok!! Creating shape > " + shapeName);
};



void
Drawer::removeShape(string shapeName)
{
    // Retorna o indice onde o Shape está.
    // int index = this->findShape(shapeName);
    // if (index != -1)
    // {
    //     // Remove shape.
    // }    
    // else {
    //     cout << "Shape not found in array" << endl;
    // }
};

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
        
void 
Drawer::setTranslate(string, float, float, float)
{

};
        
void 
Drawer::setScale(string shapeName, float, float, float)
{

};
        
void 
Drawer::setRotate(string shapeName, float, float, float, float)
{

};
        
void
Drawer::setLookAt(float, float, float)
{

};
        
void 
Drawer::setCam(float, float, float)
{

};
        
void 
Drawer::setColor(string, float, float, float)
{

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
    //this->drawShape();
}


// .  Funcao principal pra fazer a diferenciacao
// .  dos shapes e tratar
// .  o que vai ser iniciado

void
Shape::drawShape()
{
    if (this->type.compare("cone") == 0)
    {
        this->drawCone();
    }
    else if (this->type.compare("sphere") == 0)
    {
        this->drawSphere();
    }
    else if (this->type.compare("cube") == 0)
    {
        this->drawCube();
    }
    else if (this->type.compare("torus") == 0)
    {
        this->drawTorus();
    }
    else {
        cout << "Seloco tio, n tem esse shape aqui n... Ce ta trapaceando?" << endl;
    }
}

// .
// . Draw a cone.
// .
void 
Shape::drawCone()
{

    // do exemplo: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/;
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices

    glEnableVertexAttribArray(this->shapeId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);


}
    
// .
// . Draw a sphere
// .    
void 
Shape::drawSphere()
{

}
        

// .
// . Draw a cube
// .            
void 
Shape::drawCube()
{

}

// .
// . Draw a torus
// .                
void 
Shape::drawTorus()
{

}




















