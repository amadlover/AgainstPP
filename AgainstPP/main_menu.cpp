#include "main_menu.hpp"

main_menu::main_menu () : scene ()
{
	OutputDebugString (L"main_menu::main_menu\n");
}

main_menu::~main_menu ()
{
	OutputDebugString (L"main_menu::~main_menu\n");
}

egraphics_result main_menu::init ()
{
	OutputDebugString (L"main_menu::init\n");
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
	return egraphics_result::success;
}

void main_menu::exit ()
{
	OutputDebugString (L"main_menu::exit\n");
	asset::destroy_meshes (meshes);
	state = e_scene_state::exited;
}