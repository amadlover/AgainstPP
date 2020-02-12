#include "skybox_actor.hpp"

skybox_actor::skybox_actor ()
{

}

skybox_actor::skybox_actor (asset::mesh* mesh)
{
    this->mesh = mesh;
}

skybox_actor::~skybox_actor ()
{

}

egraphics_result skybox_actor::update ()
{
    return egraphics_result::success;
}

egraphics_result skybox_actor::draw ()
{
    return egraphics_result::success;
}
