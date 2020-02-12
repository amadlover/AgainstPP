#pragma once

#include "scene.hpp"
#include "event.hpp"
#include "asset.hpp"
#include "camera.hpp"
#include "skybox_actor.hpp"
#include "ui_actor.hpp"

#include <Windows.h>
#include <vector>
#include <memory>

class main_menu : public scene
{
public:
	main_menu ();
	~main_menu ();

	egraphics_result init () override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	egraphics_result main_loop () override;
	void exit () override;

protected:
	egraphics_result update () override;
	egraphics_result draw () const override;

	egraphics_result create_uniform_buffer ();
	egraphics_result create_command_pool ();
	egraphics_result create_descriptor_set ();
	egraphics_result create_graphics_pipeline ();
	egraphics_result create_render_pass ();
	egraphics_result create_framebuffers ();
	egraphics_result create_sync_objects ();
	egraphics_result allocate_command_buffers ();
	egraphics_result update_command_buffers ();

	VkCommandPool command_pool; 
	VkDescriptorPool descriptor_pool;
	VkDescriptorSetLayout skybox_descriptor_set_layout;
	VkDescriptorSet skybox_descriptor_set;
	VkRenderPass render_pass;
	std::vector<VkFramebuffer> framebuffers;
	VkPipelineShaderStageCreateInfo skybox_shader_stage_create_infos[2];
	VkShaderModule skybox_vertex_shader_module;
	VkShaderModule skybox_fragment_shader_module;
	VkPipelineLayout skybox_graphics_pipeline_layout;
	VkPipeline skybox_graphics_pipeline;
	VkSemaphore wait_semaphore;
	std::vector<VkSemaphore> signal_semaphores;
	std::vector<VkCommandBuffer> command_buffers;

	std::vector<asset::mesh> meshes;
	std::vector<VkImage> scene_images;

	std::unique_ptr<skybox_actor> skybox;
	std::vector<ui_actor> ui_actors;

	camera perspective_camera;
	camera ui_camera;

	VkBuffer uniform_buffer;
	VkBuffer uniform_buffer_memory;
	VkBuffer vertex_index_buffer;
	VkDeviceMemory vertex_index_memory;
	VkBuffer staging_vertex_index_buffer;
	VkDeviceMemory staging_vertex_index_memory;
	VkBuffer staging_image_buffer;
	VkDeviceMemory staging_image_memory;
	VkDeviceMemory scene_images_memory;

	void* uniform_buffer_data_ptr;
};
