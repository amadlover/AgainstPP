#include "main_menu.hpp"

#include <Windows.h>

namespace main_menu
{
	event::event* event_obj_ptr;

	void init (event::event* event_ptr)
	{
		OutputDebugString (L"main_menu::init\n");
		event_obj_ptr = event_ptr;
	}

	void process_keyboard_input (WPARAM wParam, LPARAM lParam)
	{
		OutputDebugString (L"main_menu::process_keyboard_input\n");
		switch (wParam)
		{
		case VK_ESCAPE:
			event_obj_ptr->go_to_scene (e_scene_type::splash_screen);

		default:
			break;
		}
	}

	void draw ()
	{
	}

	void exit ()
	{
		OutputDebugString (L"main_menu::exit\n");
	}
}

