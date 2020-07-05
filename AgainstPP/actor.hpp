#pragma once

#include "error.hpp"

class actor
{
public:
    actor ();
    ~actor ();

    egraphics_result update ();
    egraphics_result update_command_buffer () const;
};