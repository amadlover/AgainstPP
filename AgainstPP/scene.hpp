#pragma once

#include "entity.hpp"
#include "error.hpp"
#include "enums.hpp"
#include "asset.hpp"
#include "event.hpp"

#include <Windows.h>
#include <vector>

class game;

class scene : public entity
{
public:
    scene ();
    scene (game* ptr);
    ~scene ();

    virtual egraphics_result init () = 0;
    virtual void process_keyboard_input (WPARAM, LPARAM) = 0;
    virtual egraphics_result main_loop () = 0;
    virtual void exit () = 0;

    event_one_param<e_scene_type> go_to_scene_event;
    e_scene_state state;

    game* game_ptr;
    
protected:
    virtual egraphics_result update () = 0;
    virtual egraphics_result draw () const = 0;
};
