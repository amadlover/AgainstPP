#pragma once

#include <Windows.h>
#include <memory>

#include "common_graphics.hpp"
#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "entity.hpp"

#include "event.hpp"

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
	egraphics_result go_to_scene (e_scene_type new_scene);

	event_two_param<WPARAM, LPARAM> keyboard_event;

	std::shared_ptr<splash_screen> splash_screen_ptr;
	std::shared_ptr<main_menu> main_menu_ptr;
	std::shared_ptr<scene> current_scene_ptr;

	e_scene_type current_scene_type;
};