#pragma once

#include "asset.hpp"
#include <glm/mat4x4.hpp>

class skybox
{
public:
    skybox ();
    skybox (asset::mesh* mesh);
    ~skybox ();

    egraphics_result update ();
    egraphics_result draw ();

    glm::mat4 transformation_matrix;

private:
    asset::mesh* mesh;
};