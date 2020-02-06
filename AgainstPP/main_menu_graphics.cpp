#include "main_menu_graphics.hpp"

main_menu_graphics::main_menu_graphics ()
{

}

main_menu_graphics::~main_menu_graphics ()
{
    
}

egraphics_result main_menu_graphics::create_shaders ()
{
    return egraphics_result ();
}

egraphics_result main_menu_graphics::create_descriptor_sets (std::vector<asset::mesh>& meshes)
{
    return egraphics_result ();
}

egraphics_result main_menu_graphics::create_graphics_pipeline_layout ()
{
    return egraphics_result ();
}

egraphics_result main_menu_graphics::create_graphics_pipeline ()
{
    return egraphics_result ();
}

egraphics_result main_menu_graphics::update_command_buffers (const std::vector<asset::mesh>& meshes)
{
    return egraphics_result ();
}

egraphics_result main_menu_graphics::init (std::vector<asset::mesh>& meshes)
{
    return egraphics_result::success;
}

egraphics_result main_menu_graphics::draw (const std::vector<asset::mesh>& meshes) const 
{
    return egraphics_result::success;
}

void main_menu_graphics::exit (std::vector<asset::mesh>& meshes)
{
    scene_graphics::exit (meshes);
}