#include "actor.hpp"

actor::actor ()
{

}

actor::~actor ()
{

}

egraphics_result actor::update ()
{
    // update transform data as per need
    return egraphics_result::success;
}

egraphics_result actor::update_command_buffer () const
{
    // foreach material type.
    // bind graphics pipeline.
    // bind desriptor set. => uniform data - array of base_colors, array of normals etc ... ->
    // -> array number would be the max number of primitives of the material expected to be in the scene at any frame
    // foreach graphics primitive.
    // bind vertex buffers
    // bind index buffers
    // draw indexed

    return egraphics_result::success;
}