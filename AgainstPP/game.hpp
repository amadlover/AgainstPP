#pragma once

#include <Windows.h>
#include <memory>

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
	game (HINSTANCE hInstance, HWND hWnd);
	~game ();
	
	void init (HINSTANCE hInstance, HWND hWnd);
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void main_loop ();
	void exit ();

	template_event<std::string> scene_change_event;

private:
	void go_to_scene (e_scene_type new_scene);

	std::unique_ptr<common_graphics> common_graphics_ptr;
	std::shared_ptr<splash_screen> splash_screen_ptr;
	std::shared_ptr<main_menu> main_menu_ptr;
	std::shared_ptr<scene> current_scene;
	std::unique_ptr<event> event_ptr;

	static game* ptr;
};