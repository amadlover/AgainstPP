#pragma once

#include <Windows.h>

#include "common_graphics.hpp"
#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "template_event.hpp"

/*namespace game
{
	void init (HINSTANCE HInstance, HWND HWnd);
	void draw ();
	void exit ();

	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void process_left_mouse_click ();
	void process_middle_mouse_click ();
	void process_right_mouse_click ();
	void process_mouse_movement (WPARAM wParam, LPARAM lParam);

	void process_window_destroy ();
};*/

class game
{
public:
	game ();
	~game ();
	
	egraphics_result init (HINSTANCE hInstance, HWND hWnd);
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	egraphics_result main_loop ();
	void exit ();

	template_event<std::string> scene_change_event;

private:
	void go_to_scene (e_scene_type new_scene);

	splash_screen* splash_screen_ptr;
	main_menu* main_menu_ptr;
	scene* current_scene;
	event* event_ptr;

	e_scene_type current_scene_type;
};