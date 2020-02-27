#include "scene.hpp"
#include "common_graphics.hpp"

scene::scene () : entity ()
{

}

scene::scene (game* ptr)
{
    this->game_ptr = ptr;
}

scene::~scene ()
{

}