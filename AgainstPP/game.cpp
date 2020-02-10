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
	keyboard_event.broadcast (wParam, lParam);
}

egraphics_result game::init (HINSTANCE hInstance, HWND hWnd)
{
	OutputDebugString (L"game::init\n");

	main_menu_ptr = new main_menu ();
	splash_screen_ptr = new splash_screen ();

	splash_screen_ptr->go_to_scene_event.add_binding (std::bind (&game::go_to_scene, this, std::placeholders::_1), unique_id);
	keyboard_event.add_binding (std::bind (&splash_screen::process_keyboard_input, splash_screen_ptr, std::placeholders::_1, std::placeholders::_2), splash_screen_ptr->unique_id);

	main_menu_ptr->go_to_scene_event.add_binding (std::bind (&game::go_to_scene, this, std::placeholders::_1), unique_id);
	keyboard_event.add_binding (std::bind (&main_menu::process_keyboard_input, main_menu_ptr, std::placeholders::_1, std::placeholders::_2), main_menu_ptr->unique_id);

	CHECK_AGAINST_RESULT (common_graphics::init (hInstance, hWnd));
	CHECK_AGAINST_RESULT (splash_screen_ptr->init ());

	current_scene_ptr = splash_screen_ptr;
	current_scene_type = e_scene_type::splash_screen;

	return egraphics_result::success;
}

egraphics_result game::main_loop ()
{
	CHECK_AGAINST_RESULT (current_scene_ptr->main_loop ());

	return egraphics_result::success;
}

egraphics_result game::go_to_scene (e_scene_type new_scene)
{
	switch (new_scene)
	{
	case e_scene_type::splash_screen:
		OutputDebugString (L"New scene splash screen\n");
		current_scene_ptr->exit ();
		CHECK_AGAINST_RESULT (splash_screen_ptr->init ());
		current_scene_ptr = splash_screen_ptr;
		break;

	case e_scene_type::main_menu:
		OutputDebugString (L"New scene main menu\n");
		current_scene_ptr->exit ();
		CHECK_AGAINST_RESULT (main_menu_ptr->init ());
		current_scene_ptr = main_menu_ptr;
		break;

	default:
		break;
	}

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
		delete splash_screen_ptr;
	}

	if (main_menu_ptr != nullptr)
	{
		main_menu_ptr->exit ();
		keyboard_event.remove_binding (main_menu_ptr->unique_id);
		main_menu_ptr->go_to_scene_event.remove_binding (unique_id);
		delete main_menu_ptr;
	}

	common_graphics::exit ();
}