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
	++unique_id;
}

void game::go_to_scene (e_scene_type new_scene)
{
	switch (new_scene)
	{
	case e_scene_type::splash_screen:
		OutputDebugString (L"New scene splash screen\n");
		main_menu_ptr->exit ();
		keyboard_event.remove_binding (main_menu_ptr->unique_id);
		main_menu_ptr->go_to_scene_event.remove_binding (unique_id);
		main_menu_ptr.reset ();
		splash_screen_ptr = std::make_shared<splash_screen> ();
		splash_screen_ptr->init ();
		keyboard_event.add_binding (std::bind (&splash_screen::process_keyboard_input, splash_screen_ptr.get (), std::placeholders::_1, std::placeholders::_2), splash_screen_ptr->unique_id);
		splash_screen_ptr->go_to_scene_event.add_binding (std::bind (&game::go_to_scene, this, std::placeholders::_1), unique_id);
		current_scene = splash_screen_ptr;
		break;

	case e_scene_type::main_menu:
		OutputDebugString (L"New scene main menu\n");
		splash_screen_ptr->exit ();
		keyboard_event.remove_binding (splash_screen_ptr->unique_id);
		splash_screen_ptr->go_to_scene_event.remove_binding (unique_id);
		splash_screen_ptr.reset ();
		main_menu_ptr = std::make_shared <main_menu> ();
		main_menu_ptr->init ();
		keyboard_event.add_binding (std::bind (&main_menu::process_keyboard_input, main_menu_ptr.get (), std::placeholders::_1, std::placeholders::_2), main_menu_ptr->unique_id);
		main_menu_ptr->go_to_scene_event.add_binding (std::bind (&game::go_to_scene, this, std::placeholders::_1), unique_id);
		current_scene = main_menu_ptr;
		break;

	default:
		break;
	}
}

void game::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	keyboard_event.broadcast (wParam, lParam);
}

egraphics_result game::init (HINSTANCE hInstance, HWND hWnd)
{
	OutputDebugString (L"game::init\n");

	splash_screen_ptr = std::make_shared <splash_screen> ();
	splash_screen_ptr->go_to_scene_event.add_binding (std::bind (&game::go_to_scene, this, std::placeholders::_1), unique_id);
	keyboard_event.add_binding (std::bind (&splash_screen::process_keyboard_input, splash_screen_ptr.get (), std::placeholders::_1, std::placeholders::_2), splash_screen_ptr->unique_id);
	CHECK_AGAINST_RESULT (common_graphics::init (hInstance, hWnd));
	CHECK_AGAINST_RESULT (splash_screen_ptr->init ());

	current_scene = splash_screen_ptr;
	current_scene_type = e_scene_type::splash_screen;

	return egraphics_result::success;
}

egraphics_result game::main_loop ()
{
	current_scene->main_loop ();

	return egraphics_result::success;
}

void game::exit ()
{
	OutputDebugString (L"game::exit\n");

	if (splash_screen_ptr != nullptr && splash_screen_ptr->state == e_scene_state::inited)
	{
		splash_screen_ptr->exit ();
		keyboard_event.remove_binding (splash_screen_ptr->unique_id);
		splash_screen_ptr->go_to_scene_event.remove_binding (unique_id);
	}

	if (main_menu_ptr != nullptr && main_menu_ptr->state == e_scene_state::inited)
	{
		main_menu_ptr->exit ();
		keyboard_event.remove_binding (main_menu_ptr->unique_id);
		main_menu_ptr->go_to_scene_event.remove_binding (unique_id);
	}

	common_graphics::exit ();
}

game::~game ()
{
	OutputDebugString (L"game::~game\n");
}