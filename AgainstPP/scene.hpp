#pragma once

#include <Windows.h>
#include "event.hpp"

class scene
{
public:
    scene () {}
    ~scene () {}

    virtual void init (event*) = 0;
    virtual void process_keyboard_input (WPARAM, LPARAM) = 0;
    virtual void main_loop () = 0;
    virtual void exit () = 0;

    e_scene_state state;

protected:
    event* event_ptr;
};