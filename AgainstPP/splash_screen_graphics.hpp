#pragma once

#include "common_graphics.hpp"
#include "asset.hpp"
#include "actor.hpp"

#include <memory>

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

	void init (common_graphics* common_graphics_ptr);
	void draw ();
	void exit ();

private:
	void create_renderpasses ();
	void create_framebuffers ();
	void create_shaders ();
	void create_graphics_pipeline_layout ();
	void create_graphics_pipeline ();
	void create_sync_objects ();
	void allocate_command_buffers ();

	common_graphics* common_graphics_ptr;

	VkRenderPass render_pass;
	std::vector<VkFramebuffer> swapchain_framebuffers;
	VkPipelineLayout graphics_pipeline_layout;
	VkPipeline graphics_pipeline;
	std::vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stage_create_infos;
	VkShaderModule vertex_shader_module;
	VkShaderModule fragment_shader_module;
	VkDescriptorPool descriptor_pool;
	std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
	VkSemaphore signal_semaphore;
	VkSemaphore wait_semaphore;
	std::vector<VkFence> swapchain_fences;
	std::vector<VkCommandBuffer> swapchain_command_buffers;
};