#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H
#include <string>
#include <vector>
#include <sstream>

#include <GL/glew.h>
#include <GL/freeglut.h>  // ou glut.h - GLUT, include glu.h and gl.h
#include <GL/gl.h>

// biblioteca externa GLM. 
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

#include "./Classes/Shape.h"
#include "./Classes/Light.h"
using namespace std;

class OpenGLContext
{
    public:
        vector<Shape*> objects;
        vector<Light*> lights;
        string shading;
        bool enableAxis;

        // seguindo o tutorial bolado:
        // https://learnopengl.com/#!Getting-started/Camera

        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        glm::vec3 cameraDirection;
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;

        string projectionType;

        float cameraZoom;

        // luz
        glm::vec3 lightPosition;    
        float intensidadeLuzAmbiente;
        float intensidadeLuzEspecular;

        // Tipos de Luz.
        bool enableSpecularLight;
        bool enableDiffuseLight;
        bool enableAmbientLight;

        OpenGLContext(int argc, char *argv[]);     // Constructor
        ~OpenGLContext();    // Destructor

        void printVersion() const; // Show OpenGL Version
        void runLoop() const;
        void getCommands() const;


        int findShape(std::string shapeName);
        // Getters
        unsigned int getProgramId() { return programId; }
        unsigned int getVBOId() { return VBOId; }
        unsigned int getVAOId() { return VAOId; }
        void render() const;       // Render Loop
        void drawObjects() const;

        void drawAxis(vector<glm::vec3> custom_vertices, float colorR, float colorG, float colorB) const;

    private:
        unsigned int VAOId;
        unsigned int VBOId;
        unsigned int programId;

        static void glutRenderCallback();                       // Render window
        static void glutReshapeCallback(int width, int height); // Reshape window
        static void glutRedisplayCallback(); // redisplay

        void initialize();   // Init our render
        
        void finalize() const;     // Finalize our render

            

        unsigned int loadAndCompileShader(const std::string& filename, const int glType) const;
        unsigned int linkShaderProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId) const;


        

};

#endif // OPENGLCONTEXT_H
