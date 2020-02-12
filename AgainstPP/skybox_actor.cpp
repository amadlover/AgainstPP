#include "skybox_actor.hpp"

skybox_actor::skybox_actor ()
{
    transformation_matrix = glm::mat4 (1.f);
}

skybox_actor::skybox_actor (asset::mesh* mesh)
{
    this->mesh = mesh;
    transformation_matrix = mesh->transformation_matrix;
}

skybox_actor::~skybox_actor ()
{

}
