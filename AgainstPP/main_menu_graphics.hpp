#pragma once

#include <vector>
#include "scene_graphics.hpp"

class main_menu_graphics : public scene_graphics
{
public:
    main_menu_graphics ();
    ~main_menu_graphics ();

    egraphics_result init (std::vector<asset::mesh>& meshes) override;
    egraphics_result draw (const std::vector<asset::mesh>& meshes) const override;
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