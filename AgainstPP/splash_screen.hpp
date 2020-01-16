#pragma once

#include "common_graphics.hpp"
#include "splash_screen_graphics.hpp"
#include "image.hpp"
#include "actor.hpp"
#include "asset.hpp"

#include <memory>

namespace splash_screen
{
	struct splash_screen
	{
		std::unique_ptr<splash_screen_graphics::splash_screen_graphics> graphics;

		std::vector<asset::gltf_asset> gltf_assets;
		std::vector<image::gltf_image> gltf_images;

		std::vector<actor::actor> actors;
	};

	void init ();
	void run ();
	void exit ();
}