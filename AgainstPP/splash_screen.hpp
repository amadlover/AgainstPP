#pragma once

#include "common_graphics.hpp"
#include "splash_screen_graphics.hpp"
#include "image.hpp"
#include "actor.hpp"
#include "mesh.hpp"

#include <memory>

namespace splash_screen
{
	struct splash_screen
	{
		std::unique_ptr<splash_screen_graphics::splash_screen_graphics> graphics;

		std::vector<mesh::gltf_mesh> gltf_meshes;
		std::vector<image::gltf_image> gltf_images;

		std::vector<actor::actor> actors;
	};

	static std::unique_ptr<splash_screen> splash_screen_obj_ptr (new splash_screen ());

	void init (common_graphics::common_graphics* common_graphics_obj_ptr);
	void run ();
	void exit ();
}