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

class Light {

    public:
        Light(string, glm::vec3);
        glm::vec3 lightVec;
        string lightName;

};