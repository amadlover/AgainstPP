#pragma once

#include "asset.hpp"
#include <glm/mat4x4.hpp>

class actor
{
public:
    actor ();
    actor (asset::mesh* mesh);
    ~actor ();

    egraphics_result update ();
    egraphics_result draw ();

    glm::mat4 transformation_matrix;

private:
    asset::mesh* mesh;
};