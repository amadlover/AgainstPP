#include "actor.hpp"

actor::actor ()
{
}

actor::actor (asset::mesh* mesh)
{
    this->mesh = mesh;
}

actor::~actor ()
{
}

egraphics_result actor::update ()
{
    return egraphics_result ();
}

egraphics_result actor::draw ()
{
    return egraphics_result ();
}
