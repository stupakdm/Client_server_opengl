#ifndef CUBE_H
#define CUBE_H
#include <cmath>
//#include "glad/glad.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include "GLFW/glfw3.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/constants.hpp>
#include "stb_image.h"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
};

const glm::vec3 DARK_GREEN = {0.0f, 0.45f, 0.1f};
const glm::vec3 LIGHT_GREEN = {0.1f, 0.8f, 0.15f};

const Vertex CUBE_VERTICIES[] = {
    { {-1, +1, -1}, DARK_GREEN},
    { {+1, +1, -1}, DARK_GREEN},
    { {+1, -1, -1}, DARK_GREEN},
    { {-1, -1, -1}, DARK_GREEN},
    { {-1, +1, +1}, LIGHT_GREEN},
    { {+1, +1, +1}, LIGHT_GREEN},
    { {+1, -1, +1}, LIGHT_GREEN},
    { {-1, -1, +1}, LIGHT_GREEN},
};




class cube
{
public:
    cube();
};

#endif CUBE_H
