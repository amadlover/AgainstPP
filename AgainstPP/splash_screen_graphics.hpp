#pragma once

#include "asset.hpp"
#include "actor.hpp"
#include "error.hpp"


class splash_screen_graphics
{
public:
	splash_screen_graphics ();
	~splash_screen_graphics ();

	egraphics_result init (std::vector<asset::mesh>& meshes);
	egraphics_result draw (const std::vector<asset::mesh>& meshes) const;
	void exit (std::vector<asset::mesh>& meshes);

protected:
	egraphics_result create_renderpass ();
	egraphics_result create_framebuffers ();
	egraphics_result create_shaders ();
	egraphics_result create_descriptor_sets (std::vector<asset::mesh>& meshes);
	egraphics_result create_graphics_pipeline_layout ();
	egraphics_result create_graphics_pipeline ();
	egraphics_result update_command_buffers (const std::vector<asset::mesh>& meshes);

private:
	VkRenderPass render_pass;
	std::vector<VkFramebuffer> swapchain_framebuffers;

	VkBuffer vertex_index_buffer;
	VkDeviceMemory vertex_index_memory;

	VkBuffer staging_vertex_index_buffer;
	VkDeviceMemory staging_vertex_index_memory;

	std::vector<VkImage> scene_images;
	VkDeviceMemory scene_images_memory;

	VkBuffer staging_image_buffer;
	VkDeviceMemory staging_image_memory;
};