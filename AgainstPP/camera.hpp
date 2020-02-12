#pragma once

#include <glm/mat4x4.hpp>

class camera
{
public:
    camera () {}
    ~camera () {}

    glm::mat4 transformation_matrix;
    glm::mat4 projection_matrix;
};