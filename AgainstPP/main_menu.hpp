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

class game;

class main_menu : public scene
{
public:
	main_menu ();
	main_menu (game* ptr);
	~main_menu ();

	egraphics_result init () override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	egraphics_result main_loop () override;
	void exit () override;

protected:
	egraphics_result update () override;
	egraphics_result draw () const override;

private:
	egraphics_result create_transforms_buffer ();
	egraphics_result create_command_pool ();
	egraphics_result create_descriptor_set ();
	egraphics_result create_graphics_pipeline ();
	egraphics_result create_render_pass ();
	egraphics_result create_framebuffers ();
	egraphics_result create_sync_objects ();
	egraphics_result create_ui_pass_data ();
	egraphics_result allocate_command_buffers ();
	egraphics_result update_command_buffers ();
	egraphics_result init_cameras ();
	egraphics_result update_actor_transforms ();

	VkCommandPool command_pool; 
	VkDescriptorPool descriptor_pool;
	VkDescriptorSetLayout texture_descriptor_set_layout;
	VkDescriptorSet texture_descriptor_set;
	VkRenderPass render_pass;
	std::vector<VkFramebuffer> framebuffers;
	VkPipelineShaderStageCreateInfo skybox_shader_stage_create_infos[2];
	VkShaderModule skybox_shader_modules[2];
	VkPipelineLayout skybox_graphics_pipeline_layout;
	VkPipeline skybox_graphics_pipeline;

	VkRenderPass ui_pass_render_pass;
	std::vector<VkFramebuffer> ui_pass_framebuffers;
	VkImage ui_pass_image;
	std::vector<VkImageView> ui_pass_image_views;

	VkPipelineShaderStageCreateInfo ui_shader_stage_create_infos[2];
	VkShaderModule ui_shader_modules[2];
	VkPipelineLayout ui_graphics_pipeline_layout;
	VkPipeline ui_graphics_pipeline;

	VkSemaphore wait_semaphore;
	std::vector<VkSemaphore> signal_semaphores;
	std::vector<VkCommandBuffer> command_buffers;

	VkDescriptorSetLayout transform_descriptor_set_layout;
	VkDescriptorSet transform_descriptor_set;

	std::vector<asset::mesh> meshes;
	std::vector<VkImage> scene_images;

	std::unique_ptr<skybox_actor> skybox;
	std::vector<ui_actor> ui_actors;

	camera scene_camera;
	camera ui_camera;

	VkBuffer transformation_buffer;
	VkBuffer transformation_memory;
	VkBuffer vertex_index_buffer;
	VkDeviceMemory vertex_index_memory;
	VkBuffer staging_vertex_index_buffer;
	VkDeviceMemory staging_vertex_index_memory;
	VkBuffer staging_image_buffer;
	VkDeviceMemory staging_image_memory;
	VkDeviceMemory scene_images_memory;

	void* transform_buffer_data_ptr;
	uint8_t* transform_data;
};
