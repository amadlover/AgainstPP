#pragma once

#include <Windows.h>
#include "event.hpp"

namespace main_menu
{
	void init (event::event* event_ptr);
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void draw ();
	void exit ();
}