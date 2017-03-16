//g++ --std=c++11 hello.cpp -o main -lGLEW -lGL -lGLU -lglut
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>


// Define openGL parameters.
#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1

#include <GL/glew.h>
#include <GL/freeglut.h>  // ou glut.h - GLUT, include glu.h and gl.h
#include <GL/gl.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Classes/Drawer.h"

using namespace std;

int main(int argc, char **argv)
{
    //drawer.start();
    DrawerListen drawer;
    drawer.listen();
    
    //glutInit(&argc, argv);
    //glutCreateWindow("GLUT");

//    glewInit();
    //printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

    return 0;
}
