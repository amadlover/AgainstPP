#pragma once

#include "asset.hpp"
#include "actor.hpp"
#include "error.hpp"

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

class splash_screen_graphics
{
public:
	splash_screen_graphics ();
	~splash_screen_graphics ();

	egraphics_result init (std::vector<asset::mesh>& meshes);
	egraphics_result draw (const std::vector<asset::mesh>& meshes);
	void exit (std::vector<asset::mesh>& meshes);

private:
	egraphics_result create_renderpasses ();
	egraphics_result create_framebuffers ();
	egraphics_result create_shaders ();
	egraphics_result create_descriptor_sets (std::vector<asset::mesh>& meshes);
	egraphics_result create_graphics_pipeline_layout ();
	egraphics_result create_graphics_pipeline ();
	egraphics_result create_sync_objects ();
	egraphics_result allocate_command_buffers ();
	egraphics_result update_command_buffers (const std::vector<asset::mesh>& meshes);

	VkRenderPass render_pass;
	std::vector<VkFramebuffer> swapchain_framebuffers;
	VkPipelineLayout graphics_pipeline_layout;
	VkPipeline graphics_pipeline;
	std::vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stage_create_infos;
	VkShaderModule vertex_shader_module;
	VkShaderModule fragment_shader_module;
	VkDescriptorPool descriptor_pool;
	std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
	std::vector<VkSemaphore> swapchain_signal_semaphores;
	VkSemaphore swapchain_wait_semaphore;
	//std::vector<VkFence> swapchain_fences;
	std::vector<VkCommandBuffer> swapchain_command_buffers;

	//VkFence acquire_next_image_fence;

	VkBuffer vertex_index_buffer;
	VkDeviceMemory vertex_index_memory;

	VkBuffer staging_vertex_index_buffer;
	VkDeviceMemory staging_vertex_index_memory;

	std::vector<VkImage> scene_images;
	VkDeviceMemory scene_images_memory;

	VkBuffer staging_image_buffer;
	VkDeviceMemory staging_image_buffer_memory;
};