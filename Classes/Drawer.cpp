#include <iostream>
#include <string>

void 
DrawerListen::listen()
{
    string command;

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
    do {
        c1 = c2 = c3 = c4 = "";

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
            save();
        }
        else if (command.compare("quit") == 0)
        {
            exit();
            cout << "Program finished." << endl;
        }
        else {
            cout << "Command not recognized" << endl;
        }

    } while ( cin >> command );
    
}