#pragma once

#include <Windows.h>
#include <memory>

#include "common_graphics.hpp"
#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "entity.hpp"

#include "event.hpp"

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

class game : public entity
{
public:
	game ();
	~game ();
	
	egraphics_result init (HINSTANCE hInstance, HWND hWnd);
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	egraphics_result main_loop ();
	void exit ();

private:
	void go_to_scene (e_scene_type new_scene);

	event_two_param<WPARAM, LPARAM> keyboard_event;

	std::shared_ptr<splash_screen> splash_screen_ptr;
	std::shared_ptr<main_menu> main_menu_ptr;
	std::shared_ptr<scene> current_scene;

	e_scene_type current_scene_type;
};