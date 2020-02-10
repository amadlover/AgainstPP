#pragma once

#include "common_graphics.hpp"
#include "splash_screen_graphics.hpp"
#include "actor.hpp"
#include "asset.hpp"
#include "event.hpp"
#include "scene.hpp"


class splash_screen : public scene
{
public:
	splash_screen ();
	~splash_screen ();

	egraphics_result init () override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam) override;
	egraphics_result main_loop () override;
	void exit () override;

protected:
	egraphics_result update () override;
	egraphics_result draw () const override;

	egraphics_result create_shaders ();
	egraphics_result create_render_pass ();
	egraphics_result create_framebuffers ();
	egraphics_result create_command_pool ();
	egraphics_result allocate_command_buffers ();
	egraphics_result create_sync_objects ();
	egraphics_result create_descriptors ();
	egraphics_result create_graphics_pipeline ();
	egraphics_result update_command_buffers ();

	std::vector<VkFramebuffer> swapchain_framebuffers;
	VkCommandPool command_pool;
	std::vector<VkCommandBuffer> swapchain_command_buffers;
	VkRenderPass render_pass;
	std::vector<VkSemaphore> swapchain_signal_semaphores;
	VkSemaphore wait_semaphore;
	VkDescriptorPool descriptor_pool;
	VkDescriptorSetLayout descriptor_set_layout;
	VkPipelineLayout graphics_pipeline_layout;
	VkPipeline graphics_pipeline;
	VkPipelineShaderStageCreateInfo shader_stages_create_infos[2];
	VkShaderModule vertex_shader_module;
	VkShaderModule fragment_shader_module;

	std::vector<asset::mesh> meshes;
	std::vector<VkImage> scene_images;
	
	VkBuffer staging_vertex_index_buffer;
	VkDeviceMemory staging_vertex_index_memory;
	VkBuffer vertex_index_buffer;
	VkDeviceMemory vertex_index_memory;
	VkBuffer staging_image_buffer;
	VkDeviceMemory staging_image_memory;
	VkDeviceMemory scene_images_memory;
};