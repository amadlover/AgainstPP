#pragma once

#include "asset.hpp"
#include "actor.hpp"
#include "error.hpp"
#include "scene_graphics.hpp"

/*
namespace splash_screen_graphics
{
	void init (
		std::vector<asset::mesh::mesh>& meshes,
		std::vector<asset::image::image>& images,
		common_graphics::common_graphics* ptr
	);
	void update_command_buffers (
		const std::vector<asset::mesh::mesh>& meshes
	);
	void draw ();
	void exit ();
}
*/

class splash_screen_graphics : public scene_graphics
{
public:
	splash_screen_graphics ();
	~splash_screen_graphics ();

	egraphics_result init (std::vector<asset::mesh>& meshes) override;
	void exit (std::vector<asset::mesh>& meshes) override;

protected:
	egraphics_result create_shaders () override;
	egraphics_result create_descriptor_sets (std::vector<asset::mesh>& meshes) override;
	egraphics_result create_graphics_pipeline_layout () override;
	egraphics_result create_graphics_pipeline () override;
	egraphics_result update_command_buffers (const std::vector<asset::mesh>& meshes) override;

private:
	VkBuffer vertex_index_buffer;
	VkDeviceMemory vertex_index_memory;

	VkBuffer staging_vertex_index_buffer;
	VkDeviceMemory staging_vertex_index_memory;

	std::vector<VkImage> scene_images;
	VkDeviceMemory scene_images_memory;

	VkBuffer staging_image_buffer;
	VkDeviceMemory staging_image_buffer_memory;
};