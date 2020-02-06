#pragma once

class entity
{
public:
    entity ();
    ~entity ();
    
    size_t unique_id;
private:
    static size_t entity_tag;
};