#pragma once

#include <Windows.h>
#include "scene.hpp"
#include "event.hpp"

/*namespace main_menu
{
	void init (event::event* event_ptr);
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void draw ();
	void exit ();
}*/

class main_menu : public scene
{
public:
	main_menu (event* event_ptr);
	~main_menu ();

	void init (common_graphics* common_graphics_ptr) override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void main_loop () override {}
	void exit () override;
};
