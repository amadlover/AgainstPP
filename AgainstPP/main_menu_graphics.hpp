#pragma once

#include <vector>
#include "asset.hpp"

class main_menu_graphics
{
public:
    main_menu_graphics ();
    ~main_menu_graphics ();

    egraphics_result init (std::vector<asset::mesh>& meshes);
    egraphics_result draw (const std::vector<asset::mesh>& meshes) const;
    void exit (std::vector<asset::mesh>& meshes);

protected:
	egraphics_result create_shaders ();
	egraphics_result create_descriptor_sets (std::vector<asset::mesh>& meshes);
	egraphics_result create_graphics_pipeline_layout ();
	egraphics_result create_graphics_pipeline ();
	egraphics_result update_command_buffers (const std::vector<asset::mesh>& meshes);

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