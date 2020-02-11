#pragma once

#include "scene.hpp"
#include "event.hpp"
#include "asset.hpp"

#include <Windows.h>
#include <vector>

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

	egraphics_result create_command_pool ();

	VkCommandPool command_pool; 
	VkDescriptorPool descriptor_pool;
	VkDescriptorSetLayout descriptor_set_layout;
	VkDescriptorSet descriptor_set;

	std::vector<asset::mesh> meshes;
	std::vector<VkImage> scene_images;

	VkBuffer vertex_index_buffer;
	VkDeviceMemory vertex_index_memory;
	VkBuffer staging_vertex_index_buffer;
	VkDeviceMemory staging_vertex_index_memory;
	VkBuffer staging_image_buffer;
	VkDeviceMemory staging_image_memory;
	VkDeviceMemory scene_images_memory;
};
