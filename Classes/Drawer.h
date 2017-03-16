#ifndef __DRAWER_H__
#define __DRAWER_H__

#include <iostream>

using namespace std;

class DrawerListen 
{

    private:
        bool showAxis;



    public:
        DrawerListen();
        
        // Start
        // void start();
        
        // OpenGL

        // Listen.
        void listen();
        

        // Commands
        void addShape(const string shapeName);
        void removeShape(const string shapeName);
        void addLight(const string shapeName, float, float, float);
        void removeLight(const string shapeName);
        void enableReflection(const string, float);
        void disableReflection(const string, float);
        void setShading(const string);
        void setProjection(const string type);
        void setTranslate(const string, float, float, float);
        void setScale(const string shapeName, float, float, float);
        void setRotate(const string shapeName, float, float, float);
        void setLookAt(float, float, float);
        void setCam(float, float, float);
        void setColor(const string, float, float, float);
        void axis();
        void save(const string filename);
        void exit();

};


DrawerListen::DrawerListen()
{
    showAxis = false;
}

#endif