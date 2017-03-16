#include <iostream>
#include <string>
#include "Camera.h"

using namespace std;

Camera::Camera()
{
    // Ok! Agora estou olhando em ...
}

void 
Camera::lookAt(float posX, float posY, float posZ)
{
    cout << "Ok! Agora a camera vai mudar de posicao" << endl;
    this->positionX = posX;
    this->positionY = posY;
    this->positionZ = posZ;
}

void 
Camera::updateCamera()
{
    cout << "Atualizando camera..." << endl;

    // glm::vec3 Visao;
    // Visao.x = this->positionX;
    // Visao.y = this->positionY;
    // Visao.z = this->positionZ;

    // VisaoCamera = glm::normalize(Visao);
    // Normalizar o vetor.
    //glm::normalize();
}

