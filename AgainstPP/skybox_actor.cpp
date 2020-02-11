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

VkImageView skybox::get_image_view () const
{
    return mesh->graphics_primitves[0].material.base_color_texture.texture_image.image_view;
}
