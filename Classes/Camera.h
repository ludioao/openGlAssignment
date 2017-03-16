#pragma once 
#include <GL/gl.h>
//
// Classe para trabalhar com a Camera.
//
// Basicamente, apenas trata onde a camera vai olhar. :)

class Camera {

    private:
        float positionX;
        float positionY;
        float positionZ;
        //GLfloat positionW; // ? precisa disso ?
        
    public:
        Camera();
        void lookAt(float posX, float posY, float posZ);
        void updateCamera();

};