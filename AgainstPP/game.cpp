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

	if (current_scene_ptr != nullptr)
	{
		current_scene_ptr->process_keyboard_input (wParam, lParam);
	}
}

egraphics_result game::init (HINSTANCE hInstance, HWND hWnd)
{
	OutputDebugString (L"game::init\n");

	CHECK_AGAINST_RESULT (common_graphics::init (hInstance, hWnd));
	CHECK_AGAINST_RESULT (set_current_scene (e_scene_type::splash_screen));

	return egraphics_result::success;
}

egraphics_result game::main_loop ()
{
	if (current_scene_ptr != nullptr)
	{
		CHECK_AGAINST_RESULT (current_scene_ptr->main_loop ());
	}

	return egraphics_result::success;
}

egraphics_result game::set_current_scene (e_scene_type scene_type)
{
	if (current_scene_ptr != nullptr)
	{
		current_scene_ptr->exit ();
		current_scene_ptr.reset ();
	}
	
	switch (scene_type)
	{
	case e_scene_type::splash_screen:
		current_scene_ptr = std::make_shared<splash_screen> (this);
		break;

	case e_scene_type::main_menu:
		current_scene_ptr = std::make_shared<main_menu> (this);
		break;

	case e_scene_type::test_scene:
		current_scene_ptr = std::make_shared<test_scene> (); 
		break;

	default:
		break;
	}

	if (current_scene_ptr != nullptr)
	{
		CHECK_AGAINST_RESULT (current_scene_ptr->init ());
	}

	return egraphics_result::success;
}

void game::exit ()
{
	OutputDebugString (L"game::exit\n");

	if (current_scene_ptr != nullptr)
	{
		current_scene_ptr->exit ();
		current_scene_ptr.reset ();
	}

	common_graphics::exit ();
}