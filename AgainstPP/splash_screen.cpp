#include "splash_screen.hpp"
#include "utils.hpp"
#include "enums.hpp"

/*namespace splash_screen
{
	std::unique_ptr<splash_screen> splash_screen_obj_ptr (new splash_screen ());

	event::event* event_obj_ptr;

	void init (common_graphics::common_graphics* common_graphics_obj_ptr, event::event* ptr)
	{
		OutputDebugString (L"splash_screen::init\n");

		asset::image::import_images (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), splash_screen_obj_ptr->images);
		asset::mesh::import_meshes (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), splash_screen_obj_ptr->meshes);

		splash_screen_graphics::init (splash_screen_obj_ptr->meshes, splash_screen_obj_ptr->images, common_graphics_obj_ptr);
		event_obj_ptr = ptr;
	}

	void process_keyboard_input (WPARAM wParam, LPARAM lParam)
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			event_obj_ptr->go_to_scene (e_scene_type::main_menu);
			break;

		case 0x53:
			break;

		default:
			break;
		}
	}

	void draw ()
	{
		splash_screen_graphics::draw ();
	}

	void exit ()
	{
		OutputDebugString (L"splash_screen::exit\n");

		splash_screen_graphics::exit ();

		asset::mesh::destroy_meshes (splash_screen_obj_ptr->meshes);
		asset::image::destroy_images (splash_screen_obj_ptr->images);
	}
}
*/

splash_screen::splash_screen ()
{
	OutputDebugString (L"splash_screen::splash_screen\n");
	
}

egraphics_result splash_screen::init (event* event_ptr)
{
	OutputDebugString (L"splash_screen::init\n");
	graphics = new splash_screen_graphics ();
	if (graphics == nullptr)
	{
		return egraphics_result::e_against_error_system_allocate_memory;
	}

	this->event_ptr = event_ptr;
	CHECK_AGAINST_RESULT (graphics->init ());
	state = e_scene_state::inited;

	return egraphics_result::e_success;
}

void splash_screen::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_ESCAPE:
		event_ptr->go_to_scene (e_scene_type::main_menu);
		break;

	case 0x53:
		break;

	default:
		break;
	}
}

egraphics_result splash_screen::main_loop ()
{
	return egraphics_result::e_success;
}

void splash_screen::exit ()
{
	OutputDebugString (L"splash_screen::exit\n");
	graphics->exit ();
	state = e_scene_state::exited;
	delete graphics;
}

splash_screen::~splash_screen ()
{
	OutputDebugString (L"splash_screen::~splash_screen\n");
}