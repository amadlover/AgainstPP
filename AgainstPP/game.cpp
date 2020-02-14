#include "game.hpp"

#include <Windows.h>

#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "graphics_utils.hpp"
#include "event.hpp"
#include "enums.hpp"


game::game () : entity ()
{
	OutputDebugString (L"game::game\n");
}

game::~game ()
{
	OutputDebugString (L"game::~game\n");
}

void game::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	OutputDebugString (L"process_keyboard_input\n");
	keyboard_event.broadcast (wParam, lParam);
}

egraphics_result game::init (HINSTANCE hInstance, HWND hWnd)
{
	OutputDebugString (L"game::init\n");

	splash_screen_ptr = std::make_shared<splash_screen> ();
	main_menu_ptr = std::make_shared<main_menu> ();

	splash_screen_ptr->go_to_scene_event.add_binding (std::bind (&game::go_to_scene, this, std::placeholders::_1), unique_id);
	keyboard_event.add_binding (std::bind (&splash_screen::process_keyboard_input, splash_screen_ptr, std::placeholders::_1, std::placeholders::_2), splash_screen_ptr->unique_id);

	CHECK_AGAINST_RESULT (common_graphics::init (hInstance, hWnd));
	CHECK_AGAINST_RESULT (splash_screen_ptr->init ());

	current_scene_ptr = splash_screen_ptr;
	current_scene_type = e_scene_type::splash_screen;

	is_last_scene_exited = false;
	is_scene_changed = false;

	return egraphics_result::success;
}

egraphics_result game::main_loop ()
{
	if (is_scene_changed) {
		if (!is_last_scene_exited)
		{
			switch (last_scene_type)
			{
			case e_scene_type::splash_screen:
				splash_screen_ptr->exit ();
				splash_screen_ptr->go_to_scene_event.remove_binding (unique_id);
				keyboard_event.remove_binding (splash_screen_ptr->unique_id);
				break;
			case e_scene_type::main_menu:
				main_menu_ptr->exit ();
				main_menu_ptr->go_to_scene_event.remove_binding (unique_id);
				keyboard_event.remove_binding (main_menu_ptr->unique_id);
				break;
			default:
				break;
			}

			is_last_scene_exited = true;
		}
	}

	if (current_scene_ptr->state == e_scene_state::exited)
	{
		switch (current_scene_type)
		{
		case e_scene_type::splash_screen:
			if (splash_screen_ptr->state != e_scene_state::inited)
			{
				CHECK_AGAINST_RESULT (splash_screen_ptr->init ());
				splash_screen_ptr->go_to_scene_event.add_binding (std::bind (&game::go_to_scene, this, std::placeholders::_1), unique_id);
				keyboard_event.add_binding (std::bind (&splash_screen::process_keyboard_input, splash_screen_ptr, std::placeholders::_1, std::placeholders::_2), splash_screen_ptr->unique_id);
				current_scene_ptr = splash_screen_ptr;
			}
			break;

		case e_scene_type::main_menu:
			if (main_menu_ptr->state != e_scene_state::inited)
			{
				CHECK_AGAINST_RESULT (main_menu_ptr->init ());
				main_menu_ptr->go_to_scene_event.add_binding (std::bind (&game::go_to_scene, this, std::placeholders::_1), unique_id);
				keyboard_event.add_binding (std::bind (&main_menu::process_keyboard_input, main_menu_ptr, std::placeholders::_1, std::placeholders::_2), main_menu_ptr->unique_id);
				current_scene_ptr = main_menu_ptr;
			}
			break;

		default:
			break;
		}
	}

	CHECK_AGAINST_RESULT (current_scene_ptr->main_loop ());

	return egraphics_result::success;
}

void game::go_to_scene (e_scene_type new_scene_type)
{
	last_scene_type = current_scene_type;
	current_scene_type = new_scene_type;
	is_scene_changed = true;
	is_last_scene_exited = false;
}

egraphics_result game::init_new_scene (e_scene_type new_scene_type)
{
	return egraphics_result::success;
}

void game::exit ()
{
	OutputDebugString (L"game::exit\n");

	if (splash_screen_ptr != nullptr)
	{
		splash_screen_ptr->exit ();
		keyboard_event.remove_binding (splash_screen_ptr->unique_id);
		splash_screen_ptr->go_to_scene_event.remove_binding (unique_id);
	}

	if (main_menu_ptr != nullptr)
	{
		main_menu_ptr->exit ();
		keyboard_event.remove_binding (main_menu_ptr->unique_id);
		main_menu_ptr->go_to_scene_event.remove_binding (unique_id);
	}

	common_graphics::exit ();
}