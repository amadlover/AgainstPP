#include "game.hpp"

#include <Windows.h>

#include <memory>

#include "splash_screen.hpp"
#include "main_menu.hpp"

namespace game 
{
	enum class ecurrent_scene
	{
		splash_screen,
		main_menu,
	};

	enum class escene_state
	{
		inited,
		exited
	};

	ecurrent_scene current_scene = ecurrent_scene::splash_screen;

	escene_state splash_screen_state = escene_state::exited;
	escene_state main_menu_state = escene_state::exited;

	std::unique_ptr<common_graphics::common_graphics> common_graphics_obj_ptr (new common_graphics::common_graphics ());

	void process_keyboard_input (WPARAM wParam, LPARAM lParam)
	{
		switch (current_scene)
		{
		case ecurrent_scene::splash_screen:
			switch (wParam)
			{
			case VK_ESCAPE:
				splash_screen::exit ();
				splash_screen_state = escene_state::exited;

				current_scene = ecurrent_scene::main_menu;

				break;

			default:
				break;
			}

			break;

		case ecurrent_scene::main_menu:
			switch (wParam)
			{
			case VK_ESCAPE:
				main_menu::exit ();
				main_menu_state = escene_state::exited;

				current_scene = ecurrent_scene::splash_screen;
				
				break;

			default:
				break;
			}

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

	void init (HINSTANCE hInstance, HWND hWnd)
	{
		OutputDebugString (L"game::init\n");

		common_graphics::init (hInstance, hWnd, common_graphics_obj_ptr.get ());
	}

	void run ()
	{
		switch (current_scene)
		{
		case ecurrent_scene::splash_screen:
			if (splash_screen_state == escene_state::exited)
			{
				splash_screen::init (common_graphics_obj_ptr.get ());
				splash_screen_state = escene_state::inited;
			}
			else if (splash_screen_state == escene_state::inited)
			{
				splash_screen::run ();
			}
			
			break;

		case ecurrent_scene::main_menu:
			if (main_menu_state == escene_state::exited)
			{
				main_menu::init ();
				main_menu_state = escene_state::inited;
			}
			else if (main_menu_state == escene_state::inited)
			{
				main_menu::run ();
			}

			break;
		}
	}

	void exit ()
	{
		OutputDebugString (L"game::exit\n");

		if (splash_screen_state == escene_state::inited)
		{
			splash_screen::exit ();
			splash_screen_state = escene_state::exited;
		}

		if (main_menu_state == escene_state::inited)
		{
			main_menu::exit ();
			main_menu_state = escene_state::exited;
		}

		common_graphics::exit ();
	}
}