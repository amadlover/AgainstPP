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
		switch (wParam)
		{
		case VK_ESCAPE:
			event_obj_ptr->gts (e_scene_type::splash_screen);

		default:
			break;
		}
	}

	void run ()
	{
	}

	void exit ()
	{
		OutputDebugString (L"main_menu::exit\n");
	}
}

