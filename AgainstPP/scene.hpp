#pragma once

#include <Windows.h>
#include "error.hpp"
#include "enums.hpp"

class scene
{
public:
    scene () {}
    ~scene () {}

    virtual egraphics_result init () = 0;
    virtual void process_keyboard_input (WPARAM, LPARAM) = 0;
    virtual egraphics_result main_loop () = 0;
    virtual void exit () = 0;

    e_scene_state state;
};