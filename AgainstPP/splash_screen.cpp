#include "splash_screen.hpp"
#include "utils.hpp"

namespace splash_screen
{
	std::unique_ptr<splash_screen> splash_screen_obj_ptr (new splash_screen ());

	void init ()
	{
		OutputDebugString (L"splash_screen::init\n");

		image::import_gltf_images (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), splash_screen_obj_ptr->gltf_images);
		asset::import_gltf_assets (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), splash_screen_obj_ptr->gltf_assets);

		splash_screen_graphics::init (splash_screen_obj_ptr->gltf_assets, splash_screen_obj_ptr->gltf_images);
	}

	void run ()
	{
		splash_screen_graphics::run ();
	}

	void exit ()
	{
		OutputDebugString (L"splash_screen::exit\n");

		asset::destroy_gltf_assets (splash_screen_obj_ptr->gltf_assets);
		image::destroy_gltf_images (splash_screen_obj_ptr->gltf_images);

		splash_screen_graphics::exit ();
	}
}
