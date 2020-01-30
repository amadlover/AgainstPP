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
	main_menu () { OutputDebugString (L"main_menu::main_menu\n"); }
	main_menu (event* event_ptr) { this->event_ptr = event_ptr; }
	~main_menu () { OutputDebugString (L"main_menu::~main_menu\n"); }

	void init () override { OutputDebugString (L"main_menu::init\n"); };
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void main_loop () override {}
	void exit () override { OutputDebugString (L"main_menu::exit\n"); }
};
