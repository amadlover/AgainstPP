#include "entity.hpp"

size_t entity::entity_tag = 0;

entity::entity ()
{
    ++entity_tag;
    unique_id = entity_tag;
}

entity::~entity ()
{    
}