#include <iostream>
#include <string>
#include "Drawer.h"

using namespace std;

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
Drawer::listen(const OpenGLContext * context)
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
    while ( 1 ) {
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
    // find by shapename
    int shapeId = this->findShape(shapeName);
    
    // Find..
    if (shapeId != -1)
    {
        // Update light
        this->updateLight(shapeId, true, f1, f2, f3);
    }
};

void 
Drawer::removeLight(string shapeName)
{
    int shapeId = this->findShape(shapeName);
    
    if (shapeId != -1)
    {
        this->destroyLight(shapeId);
    }    
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
