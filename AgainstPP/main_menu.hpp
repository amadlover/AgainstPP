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
	main_menu ();
	~main_menu ();

	void init (event* event_ptr) override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void main_loop () override {}
	void exit () override;
};
