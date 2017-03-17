#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H
#include <string>
#include <vector>
#include "./Classes/Shape.h"

using namespace std;

class OpenGLContext
{
    public:
        vector<Shape*> objects;
        bool lightning;
        string shading;

        OpenGLContext(int argc, char *argv[]);     // Constructor
        ~OpenGLContext();    // Destructor

        void printVersion() const; // Show OpenGL Version
        void runLoop() const;
        void getCommands() const;

        // Getters
        unsigned int getProgramId() { return programId; }
        unsigned int getVBOId() { return VBOId; }
        unsigned int getVAOId() { return VAOId; }
        void render() const;       // Render Loop
        void drawObjects() const;


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
