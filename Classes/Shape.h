#pragma once // compilar apenas uma vez

#include <iostream>
#include <string>

#include "../GlContext.h"

#include <glm/gtc/matrix_transform.hpp>


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

        glm::mat4 modelShape;

    
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

        
        void setRotateable(float Angulo, glm::vec3 Direcao)
        {
             this->modelShape = glm::rotate(this->modelShape, Angulo, Direcao); 
        };
        void setScalable(float x, float y, float z)
        {
            this->scalable = true;
            this->modelShape = glm::scale(this->modelShape, glm::vec3(x, y, z));
        };
        void setTranslateable(float x, float y, float z)
        {
            this->modelShape = glm::translate(this->modelShape, glm::vec3(x, y, z));
        };

        glm::mat4& getModel()
        {
            return this->modelShape;
        }

};
