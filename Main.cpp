// Compile command:
// Linux
// g++ --std=c++11 hello.cpp -o main -lGLEW -lGL -lGLU -lglut
// Windows
// g++ --std=c++11 hello.cpp -o main -lglew32 -lfreeglut -lglu32 -lopengl32

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


int main(int argc, char *argv[]) {
	OpenGLContext context { argc, argv };
    context.printVersion();
    context.runLoop();    
    //void printVersion() const; // Show OpenGL Version
    //void runLoop() const;
    //    auto drawer = new Drawer();
    //drawer->listen( context );
	

	return 0;
}

