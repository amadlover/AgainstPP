#pragma once

#include "common_graphics.hpp"
#include "asset.hpp"
#include "actor.hpp"

#include <memory>

namespace splash_screen_graphics
{
	struct splash_screen_graphics
	{
		vk::DeviceMemory vertex_index_buffer_memory;
		vk::Buffer vertex_index_buffer;

		vk::DeviceMemory image_memory;
		std::vector<vk::ImageView> image_views;
		std::vector<vk::Image> images;
	};

	void init (std::vector<asset::mesh::mesh>& meshes, std::vector<asset::image::image>& images, common_graphics::common_graphics* ptr);
	void draw ();
	void exit ();
}