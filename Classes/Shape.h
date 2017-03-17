#pragma once // compilar apenas uma vez

#include <iostream>
#include <string>

//#include "../GlContext.h"

using namespace std;

class Shape {

    private:
        int shapeId;
        string shapeName;
        string type;
    
    public:
        Shape(){};
        Shape(int shapeId, string type, string shapeName);
        ~Shape();
        
        // Main action
        void drawShape();

        // Shapes
        void drawCone();
        void drawSphere();
        void drawCube();
        void drawTorus();

};
