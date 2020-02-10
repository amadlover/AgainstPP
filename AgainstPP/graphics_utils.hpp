#pragma once

#include <string>

#include <vulkan/vulkan.h>

#include "asset.hpp"
#include "common_graphics.hpp"

class graphics_utils
{
public:
	static egraphics_result create_vulkan_handles_for_meshes (
		std::vector<asset::mesh>& meshes,
		VkCommandPool command_pool,
		VkBuffer* staging_vertex_index_buffer,
		VkDeviceMemory* staging_vertex_index_memory,
		VkBuffer* vertex_index_buffer,
		VkDeviceMemory* vertex_index_memory,
		VkBuffer* staging_image_buffer,
		VkDeviceMemory* staging_image_memory,
		std::vector<VkImage>& scene_images,
		VkDeviceMemory* scene_images_memory
	);

	static egraphics_result create_buffer (
		VkDevice graphics_device,
		VkDeviceSize size,
		VkBufferUsageFlags usage, 
		VkSharingMode sharing_mode, 
		uint32_t graphics_queue_family_index, 
		VkBuffer* out_buffer
	);

	static egraphics_result allocate_bind_buffer_memory (
		VkDevice graphics_device, 
		VkBuffer* buffers, 
		uint32_t buffer_count, 
		VkPhysicalDeviceMemoryProperties physical_device_memory_properties, 
		VkMemoryPropertyFlags required_types, 
		VkDeviceMemory* out_memory
	);

	static egraphics_result map_data_to_device_memory (
		VkDevice graphics_device, 
		VkDeviceMemory memory, 
		VkDeviceSize offset, 
		VkDeviceSize size, 
		void* data_source
	);

	static egraphics_result create_image (
		VkDevice graphics_device, 
		uint32_t graphics_queue_family_index, 
		VkExtent3D extent, 
		uint32_t array_layers, 
		VkFormat format, 
		VkImageTiling tiling,
		VkImageType image_type,
		VkImageLayout initial_layout, 
		VkImageUsageFlags usage,
		VkSharingMode sharing_mode, 
		VkImage* out_image
	);	

	static egraphics_result allocate_bind_image_memory (
		VkDevice graphics_device, 
		VkImage* images, 
		uint32_t image_count, 
		VkPhysicalDeviceMemoryProperties physical_device_memory_properties, 
		VkMemoryPropertyFlags required_types, 
		VkDeviceMemory* out_memory
	);
	
	static egraphics_result change_image_layout (
		VkDevice graphics_device, 
		VkCommandPool command_pool, 
		uint32_t graphics_queue_family_index,
		VkImage image, 
		uint32_t layer_count, 
		VkImageLayout old_layout, 
		VkImageLayout new_layout, 
		VkAccessFlags src_access, 
		VkAccessFlags dst_access, 
		VkPipelineStageFlags src_stage, 
		VkPipelineStageFlags dst_stage
	);

	static egraphics_result copy_buffer_to_buffer (
		VkDevice graphics_device, 
		VkCommandPool command_pool, 
		VkQueue graphics_queue, 
		VkBuffer src_buffer, 
		VkBuffer dst_buffer, 
		VkDeviceSize size
	);

	static egraphics_result copy_buffer_to_image (
		VkDevice graphics_device,
		VkCommandPool command_pool,
		VkQueue graphics_queue,
		VkDeviceSize offset,
		VkBuffer buffer,
		VkImage* image,
		VkExtent3D extent,
		uint32_t layer_count
	);

	static egraphics_result copy_image_to_image (
		VkDevice graphics_device,
		VkCommandPool command_pool,
		VkImage src_image,
		VkImageLayout src_image_layout,
		VkImage dst_image,
		VkImageLayout dst_image_layout,
		VkExtent3D extent
	);

	static egraphics_result create_shader (
		const char* file_path, 
		VkDevice graphics_device, 
		VkShaderStageFlagBits shader_stage, 
		VkShaderModule* shader_module, 
		VkPipelineShaderStageCreateInfo* shader_stage_create_info
	);

	static void destroy_buffers_and_buffer_memory (
		VkDevice graphics_device, 
		VkBuffer* buffers, 
		uint32_t buffer_count,
		VkDeviceMemory* buffer_memory
	);

	static void destroy_images_and_image_memory (
		VkDevice graphics_device,
		VkImage* images,
		uint32_t image_count,
		VkDeviceMemory* image_memory
	);
};