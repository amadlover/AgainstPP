#include "splash_screen.hpp"
#include "utils.hpp"
#include "enums.hpp"

namespace splash_screen
{
	std::unique_ptr<splash_screen> splash_screen_obj_ptr (new splash_screen ());

	event::event* event_obj_ptr;

	void init (common_graphics::common_graphics* common_graphics_obj_ptr, event::event* ptr)
	{
		OutputDebugString (L"splash_screen::init\n");

		image::import_gltf_images (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), splash_screen_obj_ptr->gltf_images);
		mesh::import_gltf_meshes (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), splash_screen_obj_ptr->gltf_meshes);

		splash_screen_graphics::init (splash_screen_obj_ptr->gltf_meshes, splash_screen_obj_ptr->gltf_images, common_graphics_obj_ptr);
		event_obj_ptr = ptr;
	}

	void process_keyboard_input (WPARAM wParam, LPARAM lParam)
	{
		OutputDebugString (L"splash_screen::process_keyboard_input\n");
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

		mesh::destroy_gltf_meshes (splash_screen_obj_ptr->gltf_meshes);
		image::destroy_gltf_images (splash_screen_obj_ptr->gltf_images);

		splash_screen_graphics::exit ();
	}
}
