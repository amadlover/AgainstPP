#pragma once

#include "graphics.hpp"
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

		std::vector<asset::asset> assets;
		std::vector<actor::actor> actors;
		std::vector<image::image> images;
	};

	void init ();
	void run ();
	void exit ();
}