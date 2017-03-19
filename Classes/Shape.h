#pragma once // compilar apenas uma vez

#include <iostream>
#include <string>

#include "../GlContext.h"

#include <glm/gtc/matrix_transform.hpp>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>// glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/geometric.hpp>// glm::cross, glm::normalize
#include <glm/packing.hpp>// glm::packUnorm2x16
#include <glm/integer.hpp>// glm::uint
#include <glm/gtc/type_precision.hpp>// glm::i8vec2, glm::i32vec2
#include <glm/exponential.hpp>// glm::pow
#include <glm/gtc/random.hpp>// glm::vecRand3


using namespace std;

class Shape {

    private:
        int shapeId;
        string shapeName;
        string type;
        bool resizable;
        bool scalable;
        bool rotateable;
        bool translateable;
        GLuint VAO_;
        GLuint VBO_;

        glm::mat4 modelShape;
        
        // cores.
        float   colorR,
                colorG,
                colorB;

        // translada
        float   transX,
                transY,
                transZ;

        // 
        glm::vec3 scaleCoordinates;
        
        // rotacao
        glm::vec3 DirecaoRotate;
        float anguloRotacao;
    
    public:
        // VARIAVEIS COMPARTILHADAS P/ SEREM UTILIZADSA PELO PARSER
        vector<glm::vec3> data;
        vector<glm::vec3> normals;
        vector<glm::vec3> colors;

        Shape(){};
        Shape(int shapeId, string type, string shapeName);
        ~Shape();
        
        string getName(){ return shapeName; };
        string getType(){ return type; };
        int getShapeId() { return shapeId; };

        void initializeBuffers();

        // Main action
        void drawShape();

        // some actions .
        // geters and seters 
        bool getResizable() { return resizable; };
        bool getScalable() { return scalable; }; 
        bool getRotateable() { return rotateable; };
        bool getTranslateable() { return translateable; };
        
        void setColor(float r, float g, float b)
        {
            this->colorR = r;
            this->colorG = g;
            this->colorB = b;
        };
        
        void setRotateable(float Angulo, glm::vec3 Direcao)
        {
            this->rotateable = true;
            this->anguloRotacao = Angulo;
            this->DirecaoRotate = Direcao;
            //this->modelShape = glm::rotate(this->modelShape, Angulo, Direcao); 
        };
        void setScalable(float x, float y, float z)
        {
            this->scalable = true;

            this->scaleCoordinates = glm::vec3(x, y, z);
            //this->modelShape = glm::scale(this->modelShape, glm::vec3(x, y, z));
        };
        void setTranslateable(float x, float y, float z)
        {
            this->translateable = true;
            this->transX = x;
            this->transY = y;
            this->transZ = z;
            //this->modelShape = glm::translate(this->modelShape, glm::vec3(x, y, z));
        };

        glm::mat4& getModel()
        {
            return this->modelShape;
        }

};
