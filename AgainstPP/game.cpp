#include "game.hpp"

#include <Windows.h>

#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "graphics_utils.hpp"
#include "event.hpp"
#include "enums.hpp"

/*
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
*/

game::game ()
{
	OutputDebugString (L"game::game\n");
}

void game::go_to_scene (e_scene_type new_scene)
{
	switch (new_scene)
	{
	case e_scene_type::splash_screen:
		OutputDebugString (L"New scene splash screen\n");
		main_menu_ptr->exit ();
		splash_screen_ptr->init (event_ptr);
		current_scene = splash_screen_ptr;
		break;

	case e_scene_type::main_menu:
		OutputDebugString (L"New scene main menu\n");
		splash_screen_ptr->exit ();
		main_menu_ptr->init (event_ptr);
		current_scene = main_menu_ptr;
		break;

	default:
		break;
	}

	scene_change_event.broadcast ();
}

void game::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	current_scene->process_keyboard_input (wParam, lParam);
}

egraphics_result game::init (HINSTANCE hInstance, HWND hWnd)
{
	OutputDebugString (L"game::init\n");
	event_ptr = new event ();
	if (event_ptr == nullptr)
	{
		return egraphics_result::e_against_error_system_allocate_memory;
	}

	event_ptr->go_to_scene = std::bind (&game::go_to_scene, this, std::placeholders::_1);

	splash_screen_ptr = new splash_screen ();
	if (splash_screen_ptr == nullptr)
	{
		return egraphics_result::e_against_error_system_allocate_memory;
	}

	main_menu_ptr = new main_menu ();
	if (main_menu_ptr == nullptr)
	{
		return egraphics_result::e_against_error_system_allocate_memory;
	}

	CHECK_AGAINST_RESULT (common_graphics::init (hInstance, hWnd));
	CHECK_AGAINST_RESULT (splash_screen_ptr->init (event_ptr));

	current_scene = splash_screen_ptr;
	current_scene_type = e_scene_type::splash_screen;

	return egraphics_result::e_success;
}

egraphics_result game::main_loop ()
{
	current_scene->main_loop ();

	return egraphics_result::e_success;
}

void game::exit ()
{
	OutputDebugString (L"game::exit\n");

	if (splash_screen_ptr->state == e_scene_state::inited)
	{
		splash_screen_ptr->exit ();
	}
	
	if (main_menu_ptr->state == e_scene_state::inited)
	{
		main_menu_ptr->exit ();
	}
	
	common_graphics::exit ();
}

game::~game ()
{
	OutputDebugString (L"game::~game\n");
	
	delete event_ptr;
	delete splash_screen_ptr;
	delete main_menu_ptr;
}