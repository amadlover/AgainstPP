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

	void process_keyboard_input (WPARAM wParam, LPARAM lParam)
	{
		switch (current_scene)
		{
		case e_scene_type::splash_screen:
			splash_screen::process_keyboard_input (wParam, lParam);

			break;

		case e_scene_type::main_menu:
			main_menu::process_keyboard_input (wParam, lParam);

			break;

		default:
			break;
		}
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
			
			break;

		case e_scene_type::main_menu:
			main_menu::exit ();

			break;

		default:
			break;
		}

		current_scene = new_scene;

		switch (current_scene)
		{
		case e_scene_type::splash_screen:
			splash_screen::init (common_graphics_obj_ptr.get (), event_obj_ptr.get ());
			break;

		case e_scene_type::main_menu:
			main_menu::init (event_obj_ptr.get ());
		}
	}

	void init (HINSTANCE hInstance, HWND hWnd)
	{
		OutputDebugString (L"game::init\n");

		event_obj_ptr->gts = go_to_scene;

		common_graphics::init (hInstance, hWnd, common_graphics_obj_ptr.get ());
		graphics_utils::init (common_graphics_obj_ptr.get ());
		splash_screen::init (common_graphics_obj_ptr.get (), event_obj_ptr.get ());
	}

	void run ()
	{
		switch (current_scene)
		{
		case e_scene_type::splash_screen:
			splash_screen::run ();
			break;

		case e_scene_type::main_menu:
			main_menu::run ();
			break;
		}
	}

	void exit ()
	{
		OutputDebugString (L"game::exit\n");

		splash_screen::exit ();
		main_menu::exit ();

		common_graphics::exit ();
	}
}