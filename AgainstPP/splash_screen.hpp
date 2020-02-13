#pragma once

#include "common_graphics.hpp"
#include "asset.hpp"
#include "event.hpp"
#include "scene.hpp"

#include <chrono>

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

	egraphics_result create_fade_in_uniform_buffer ();
	egraphics_result create_render_pass ();
	egraphics_result create_framebuffers ();
	egraphics_result create_command_pool ();
	egraphics_result allocate_command_buffers ();
	egraphics_result create_sync_objects ();
	egraphics_result create_descriptors ();
	egraphics_result create_graphics_pipeline ();
	egraphics_result update_command_buffers ();

	std::vector<VkFramebuffer> framebuffers;
	VkCommandPool command_pool;
	std::vector<VkCommandBuffer> command_buffers;
	VkRenderPass render_pass;
	std::vector<VkSemaphore> signal_semaphores;
	VkSemaphore wait_semaphore;
	VkDescriptorPool descriptor_pool;
	VkDescriptorSetLayout texture_descriptor_set_layout;
	VkDescriptorSet texture_descriptor_set;
	VkPipelineLayout skybox_graphics_pipeline_layout;
	VkPipeline skybox_graphics_pipeline;
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

	float fade_in;
	std::chrono::time_point<std::chrono::steady_clock> start_time;
	std::chrono::duration<float> scene_change_threshold;
	VkBuffer fade_in_uniform_buffer;
	VkDeviceMemory fade_in_device_memory;
	void* fade_in_data_ptr;

	bool is_scene_changed;
};