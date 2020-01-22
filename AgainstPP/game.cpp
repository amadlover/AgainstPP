#include "game.hpp"

#include <Windows.h>
#include <memory>

#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "graphics_utils.hpp"
#include "event.hpp"

#include "enums.hpp"

namespace game 
{
	e_scene_type current_scene = e_scene_type::splash_screen;

	e_scene_state splash_screen_state = e_scene_state::exited;
	e_scene_state main_menu_state = e_scene_state::exited;

	std::unique_ptr<common_graphics::common_graphics> common_graphics_obj_ptr (new common_graphics::common_graphics ());
	std::unique_ptr<event::event> event_obj_ptr (new event::event ());

	void(*current_scene_draw)();
	void(*current_scene_process_keyboard_input)(WPARAM, LPARAM);

	void process_keyboard_input (WPARAM wParam, LPARAM lParam)
	{
		current_scene_process_keyboard_input (wParam, lParam);
	}

	void process_left_mouse_click ()
	{
	}

	void process_middle_mouse_click ()
	{
	}

	void process_right_mouse_click ()
	{
	}

	void process_mouse_movement (WPARAM wParam, LPARAM lParam)
	{
	}

	void process_window_destroy ()
	{

	}

	void go_to_scene (e_scene_type new_scene)
	{
		switch (current_scene)
		{
		case e_scene_type::splash_screen:
			splash_screen::exit ();
			splash_screen_state = e_scene_state::exited;
			
			break;

		case e_scene_type::main_menu:
			main_menu::exit ();
			main_menu_state = e_scene_state::exited;

			break;

		default:
			break;
		}

		current_scene = new_scene;

		switch (current_scene)
		{
		case e_scene_type::splash_screen:
			splash_screen::init (common_graphics_obj_ptr.get (), event_obj_ptr.get ());
			splash_screen_state = e_scene_state::inited;
			current_scene_draw = splash_screen::draw;
			current_scene_process_keyboard_input = splash_screen::process_keyboard_input;
			break;

		case e_scene_type::main_menu:
			main_menu::init (event_obj_ptr.get ());
			main_menu_state = e_scene_state::inited;
			current_scene_draw = main_menu::draw;
			current_scene_process_keyboard_input = main_menu::process_keyboard_input;
			break;
		}
	}

	void init (HINSTANCE hInstance, HWND hWnd)
	{
		OutputDebugString (L"game::init\n");

		event_obj_ptr->go_to_scene = go_to_scene;

		common_graphics::init (hInstance, hWnd, common_graphics_obj_ptr.get ());
		graphics_utils::init (common_graphics_obj_ptr.get ());
		splash_screen::init (common_graphics_obj_ptr.get (), event_obj_ptr.get ());
		current_scene_draw = splash_screen::draw;
		current_scene_process_keyboard_input = splash_screen::process_keyboard_input;
	}

	void draw ()
	{
		current_scene_draw ();
	}

	void exit ()
	{
		OutputDebugString (L"game::exit\n");

		if (splash_screen_state != e_scene_state::exited)
		{
			splash_screen::exit ();
			splash_screen_state = e_scene_state::exited;
		}
		
		if (main_menu_state != e_scene_state::exited)
		{
			main_menu::exit ();
			main_menu_state = e_scene_state::exited;
		}

		common_graphics::exit ();
	}
}