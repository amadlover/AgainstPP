#include "main_menu.hpp"

/*namespace main_menu
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
*/

main_menu::main_menu ()
{
	OutputDebugString (L"main_menu::main_menu\n");
}

main_menu::~main_menu ()
{
	OutputDebugString (L"main_menu::~main_menu\n");
}

egraphics_result main_menu::init (event* event_ptr)
{
	OutputDebugString (L"main_menu::init\n");
	this->event_ptr = event_ptr;
	state = e_scene_state::inited;

	return egraphics_result::success;
}

void main_menu::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_ESCAPE:
		event_ptr->go_to_scene (e_scene_type::splash_screen);
		break;

	case 0x53:
		break;

	default:
		break;
	}
}

egraphics_result main_menu::main_loop ()
{
	return egraphics_result::success;
}

void main_menu::exit ()
{
	OutputDebugString (L"main_menu::exit\n");
	state = e_scene_state::exited;
}
