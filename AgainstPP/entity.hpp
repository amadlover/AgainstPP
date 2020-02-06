#pragma once

class entity
{
public:
    entity ();
    ~entity ();
    
    static size_t entity_tag;
    size_t unique_id;
};