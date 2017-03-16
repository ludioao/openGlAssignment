#ifndef __DRAWER_CPP
#define __DRAWER_CPP
#include <iostream>
#include <string>
#include "Drawer.h"

using namespace std;

inline string console_log(string val)
{
    cout << val << endl;
}


//
// Constructor
//
DrawerListen::DrawerListen()
{

}

//
// Destructor
//
DrawerListen::~DrawerListen()
{
    
}

//
// Funcao pra ficar escutando os comandos 
// solicitados pelo usuario no terminal
//
void 
DrawerListen::listen()
{
    cout << "Initializing openGL " << endl;

    string command = "";

    string  c1 = "",
            c2 = "",
            c3 = "",
            c4 = "";

    float   
            f1 = 0.0,
            f2 = 0.0,
            f3 = 0.0,
            f4 = 0.0;            

    // read until exit is needed.
    while ( cin >> command ) {
        c1.clear();
        c2.clear();
        c3.clear();
        c4.clear();
        
        //cin >> command;

        if (command.compare("add_shape") == 0)
        {
            // add_shape...
            cin >> c1;
            addShape(c1);            
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
DrawerListen::addShape(const string shapeName)
{   
    console_log("Ok!! Creating shape > " + shapeName);
};

void
DrawerListen::removeShape(const string shapeName)
{
    console_log("Removing shape > " + shapeName);
};

void 
DrawerListen::addLight(const string shapeName, float, float, float)
{

};

void 
DrawerListen::removeLight(const string shapeName)
{

};

void 
DrawerListen::enableReflection(const string, float)
{

};

void 
DrawerListen::disableReflection(const string, float)
{

};
        
void 
DrawerListen::setShading(const string)
{

};
        
void 
DrawerListen::setProjection(const string type)
{

};
        
void 
DrawerListen::setTranslate(const string, float, float, float)
{

};
        
void 
DrawerListen::setScale(const string shapeName, float, float, float)
{

};
        
void 
DrawerListen::setRotate(const string shapeName, float, float, float, float)
{

};
        
void
DrawerListen::setLookAt(float, float, float)
{

};
        
void 
DrawerListen::setCam(float, float, float)
{

};
        
void 
DrawerListen::setColor(const string, float, float, float)
{

};
        
void 
DrawerListen::axis()
{

};
        
void 
DrawerListen::save(const string filename)
{

};
        
void 
DrawerListen::exit()
{

};


#endif