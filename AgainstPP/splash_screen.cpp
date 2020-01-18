#include "splash_screen.hpp"
#include "utils.hpp"

namespace splash_screen
{
	std::unique_ptr<splash_screen> splash_screen_obj_ptr (new splash_screen ());

	void init (common_graphics::common_graphics* common_graphics_obj_ptr)
	{
		OutputDebugString (L"splash_screen::init\n");

		image::import_gltf_images (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), splash_screen_obj_ptr->gltf_images);
		mesh::import_gltf_meshes (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), splash_screen_obj_ptr->gltf_meshes);

		splash_screen_graphics::init (splash_screen_obj_ptr->gltf_meshes, splash_screen_obj_ptr->gltf_images, common_graphics_obj_ptr);
	}

	void run ()
	{
		splash_screen_graphics::run ();
	}

	void exit ()
	{
		OutputDebugString (L"splash_screen::exit\n");

		mesh::destroy_gltf_meshes (splash_screen_obj_ptr->gltf_meshes);
		image::destroy_gltf_images (splash_screen_obj_ptr->gltf_images);

		splash_screen_graphics::exit ();
	}
}
