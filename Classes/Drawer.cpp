#include <iostream>
#include <string>
#include "Drawer.h"

//
// Constructor
//
Drawer::Drawer()
{   
    std::cout << "Constructor " << std::endl;
}

//
// Destructor
//
Drawer::~Drawer()
{
    std::cout << "destructing " << std::endl;
}

//
// Funcao pra ficar escutando os comandos 
// solicitados pelo usuario no terminal
//
void 
Drawer::listen(const OpenGLContext * context)
{
    std::cout << "Initializing openGL " << std::endl;

    std::string command = "";

    std::string  c1 = "",
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
        std::cin.clear();

        std::cin >> command;
        c1.clear();
        c2.clear();
        c3.clear();
        c4.clear();
        
        //std::cin >> command;

        if (command.compare("add_shape") == 0)
        {
            // add_shape...
            std::cin >> c1;
            std::cin >> c2;
            addShape(c1);            
        }
        else if (command.compare("remove_shape") == 0)
        {
            // remove_shape.
            std::cin >> c1;
            removeShape(c1);
        }
        else if (command.compare("add_light") == 0)
        {
            // add_light
            std::cin >> c1; // lightname
            std::cin >> f1 >> f2 >> f3; // float 
            addLight(c1, f1, f2, f3);
        }
        else if (command.compare("remove_light") == 0)
        {
            // remove_light
            std::cin >> c1;
            removeLight(c1);
        }
        else if (command.compare("reflection_on") == 0)
        {
            std::cin >> c1;
            std::cin >> f1;
            enableReflection(c1, f1);
        }
        else if (command.compare("reflection_off") == 0)
        {
            std::cin >> c1;
            std::cin >> f1;
            disableReflection(c1, f1);
        }
        else if (command.compare("shading") == 0)
        {
            std::cin >> c1;
            setShading(c1);
        }
        else if (command.compare("projection") == 0)
        {
            std::cin >> c1;
            setProjection(c1);
        }
        else if (command.compare("translate") == 0)
        {
            std::cin >> c1;
            std::cin >> f1 >> f2 >> f3;
            setTranslate(c1, f1, f2, f3);            
        }
        else if (command.compare("scale") == 0)
        {
            std::cin >> c1;
            std::cin >> f1 >> f2 >> f3;
            setScale(c1, f1, f2, f3);
        }
        else if (command.compare("rotate") == 0)
        {
            std::cin >> c1;
            std::cin >> f1 >> f2 >> f3 >> f4;
            setRotate(c1, f1, f2, f3, f4);
        }
        else if (command.compare("lookat") == 0)
        {
            std::cin >> f1 >> f2 >> f3;
            setLookAt(f1, f2, f3); 
        }
        else if (command.compare("cam") == 0)
        {
            std::cin >> f1 >> f2 >> f3;
            setCam(f1, f2, f3);
        }
        else if (command.compare("color") == 0)
        {
            std::cin >> c1;
            std::cin >> f1 >> f2 >> f3;
            setColor(c1, f1, f2, f3);
        }
        else if (command.compare("axis") == 0)
        {
            axis();
        }
        else if (command.compare("save") == 0)
        {
            std::cin >> c1;
            std::cout << "Saving into file... " << c1;
            save(c1);
        }
        else if (command.compare("quit") == 0)
        {
            exit();
            std::cout << "Program finished." << std::endl;
        }
        else {
            std::cout << "Command not recognized" << std::endl;
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
Drawer::addShape(std::string shapeName)
{   
    std::cout << "Ok!! Creating shape: " << shapeName << std::endl;
    //console_log("Ok!! Creating shape > " + shapeName);
};

void
Drawer::removeShape(std::string shapeName)
{
    //console_log("Removing shape > " + shapeName);
};

void 
Drawer::addLight(std::string shapeName, float, float, float)
{

};

void 
Drawer::removeLight(std::string shapeName)
{

};

void 
Drawer::enableReflection(std::string, float)
{

};

void 
Drawer::disableReflection(std::string, float)
{

};
        
void 
Drawer::setShading(std::string)
{

};
        
void 
Drawer::setProjection(std::string type)
{

};
        
void 
Drawer::setTranslate(std::string, float, float, float)
{

};
        
void 
Drawer::setScale(std::string shapeName, float, float, float)
{

};
        
void 
Drawer::setRotate(std::string shapeName, float, float, float, float)
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
Drawer::setColor(std::string, float, float, float)
{

};
        
void 
Drawer::axis()
{

};
        
void 
Drawer::save(std::string filename)
{

};
        
void 
Drawer::exit()
{

};
