#pragma once // compilar apenas uma vez
#include "../GlContext.h"
#include <iostream>
#include <string>

class Drawer 
{

    private:
        bool showAxis;

    public:
        Drawer();
        ~Drawer();
        
        void listen();

        // Helpers.
        bool isValidShape(std::string type);

        // Commands
        void addShape(std::string shapeType, std::string shapeName);
        void removeShape(std::string shapeName);
        void addLight(std::string shapeName, float, float, float);
        void removeLight(std::string shapeName);
        void enableReflection(std::string, float);
        void disableReflection(std::string, float);
        void setShading(std::string);
        void setProjection(std::string type);
        void setTranslate(std::string, float, float, float);
        void setScale(std::string shapeName, float, float, float);
        void setRotate(std::string shapeName, float, float, float, float);
        void setLookAt(float, float, float);
        void setCam(float, float, float);
        void setColor(std::string, float, float, float);
        void axis();
        void save(std::string filename);
        void exit();

};
