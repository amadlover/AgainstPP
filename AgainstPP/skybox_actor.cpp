#include "skybox_actor.hpp"

skybox::skybox ()
{

}

skybox::skybox (asset::mesh* mesh)
{
    this->mesh = mesh;
}

skybox::~skybox ()
{

}

egraphics_result skybox::update ()
{
    return egraphics_result::success;
}

egraphics_result skybox::draw ()
{
    return egraphics_result::success;
}