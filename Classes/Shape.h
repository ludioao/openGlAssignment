#pragma once // compilar apenas uma vez

#include <iostream>
#include <string>

using namespace std;

class Shape {

    private:
        int shapeId;
        string shapeName;
        string type;
    
    public:
        Shape(){};
        Shape(int shapeId, string type, string shapeName);
};
