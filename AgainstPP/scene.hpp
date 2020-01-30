#pragma once

#include <Windows.h>
#include <memory>
#include "event.hpp"

class scene
{
public:
    scene () {}
    ~scene () {}

    virtual void init () = 0;
    virtual void process_keyboard_input (WPARAM wParam, LPARAM lParam) = 0;
    virtual void main_loop () = 0;
    virtual void exit () = 0;

protected:
    event* event_ptr;
};