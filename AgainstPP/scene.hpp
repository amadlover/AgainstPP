#pragma once

#include <Windows.h>
#include <memory>
#include "event.hpp"
#include "common_graphics.hpp"

class scene
{
public:
    scene () {}
    ~scene () {}

    virtual void init (common_graphics*, event*) = 0;
    virtual void process_keyboard_input (WPARAM, LPARAM) = 0;
    virtual void main_loop () = 0;
    virtual void exit () = 0;

protected:
    event* event_ptr;
};