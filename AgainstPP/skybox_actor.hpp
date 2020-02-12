#pragma once

#include "asset.hpp"

#include <glm/mat4x4.hpp>

class skybox_actor
{
public:
    skybox_actor ();
    skybox_actor (asset::mesh* mesh);
    ~skybox_actor ();

    egraphics_result update ();
    egraphics_result draw ();

    glm::mat4 transformation_matrix;

    asset::mesh* mesh;
};