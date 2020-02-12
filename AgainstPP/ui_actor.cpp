#include "ui_actor.hpp"

ui_actor::ui_actor ()
{
    transformation_matrix = glm::mat4 (1.f);
}

ui_actor::ui_actor (asset::mesh* mesh)
{
    this->mesh = mesh;
    transformation_matrix = mesh->transformation_matrix;
}

ui_actor::~ui_actor ()
{
    
}