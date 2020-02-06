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

main_menu::main_menu () : scene ()
{
	OutputDebugString (L"main_menu::main_menu\n");
	++unique_id;
}

main_menu::~main_menu ()
{
	OutputDebugString (L"main_menu::~main_menu\n");
}

egraphics_result main_menu::init ()
{
	OutputDebugString (L"main_menu::init\n");
	graphics = new main_menu_graphics ();
	if (graphics == nullptr)
	{
		return egraphics_result::e_against_error_system_allocate_memory;
	}

	CHECK_AGAINST_RESULT (graphics->init (meshes));
	state = e_scene_state::inited;

	return egraphics_result::success;
}

void main_menu::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	OutputDebugString (L"main_menu::process_keyboard_input\n");
	switch (wParam)
	{
	case VK_ESCAPE:
		go_to_scene_event.broadcast (e_scene_type::splash_screen);
		break;

	case 0x53:
		break;

	default:
		break;
	}
}

egraphics_result main_menu::main_loop ()
{
	CHECK_AGAINST_RESULT (update ());
	CHECK_AGAINST_RESULT (draw ());
	
	return egraphics_result::success;
}

egraphics_result main_menu::update ()
{
	return egraphics_result::success;
}

egraphics_result main_menu::draw () const
{
	CHECK_AGAINST_RESULT (graphics->draw (meshes));
	return egraphics_result::success;
}

void main_menu::exit ()
{
	OutputDebugString (L"main_menu::exit\n");
	graphics->exit (meshes);
	asset::destroy_meshes (meshes);
	state = e_scene_state::exited;
	delete graphics;
}