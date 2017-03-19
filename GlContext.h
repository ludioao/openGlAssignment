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

using namespace std;

/*#define ESZ(elem) (int)elem.size()


//http://code.runnable.com/VHb0hWMZp-ws1gAr/splitting-a-string-into-a-vector-for-c%2B%2B
// You could also take an existing vector as a parameter.
vector<string> split(string s, string t)
{
	vector<string> res;
	while(1)
	{
		int pos = s.find(t);
		if(pos == -1){res.push_back(s); break;}
		res.push_back(s.substr(0, pos));
		s = s.substr(pos+1, ESZ(s)-pos-1);
	}
	return res;

}*/

class OpenGLContext
{
    public:
        vector<Shape*> objects;
        bool lightning;
        string shading;

        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;

        float cameraZoom;

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
