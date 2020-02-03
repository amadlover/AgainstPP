#pragma once

#include <string>

#include <vulkan/vulkan.h>

#include "common_graphics.hpp"

/*namespace graphics_utils
{
	void init (common_graphics::common_graphics* ptr);

	vk::Buffer create_buffer (
		vk::DeviceSize size,
		vk::BufferUsageFlags usage,
		vk::SharingMode sharing_mode
	);

	vk::DeviceMemory allocate_bind_buffer_memory (
		const vk::Buffer& buffer,
		vk::MemoryPropertyFlags required_types
	);

	void map_data_to_device_memory (
		vk::DeviceMemory& memory,
		vk::DeviceSize offset,
		vk::DeviceSize size,
		const void* data_source
	);

	void copy_buffer_to_buffer (
		vk::CommandPool& command_pool,
		const vk::Buffer& src_buffer,
		const vk::Buffer& dst_buffer,
		vk::DeviceSize size
	);

	vk::Image create_image (
		vk::Extent3D extent,
		uint32_t array_layers,
		vk::Format format,
		vk::ImageLayout initial_layout,
		vk::ImageUsageFlags usage,
		vk::SharingMode sharing_mode
	);

	vk::DeviceMemory allocate_bind_image_memory (
		const std::vector<vk::Image>& images,
		vk::MemoryPropertyFlags required_types
	);

	void copy_image_to_image ();

	void create_shader (
		std::string file_path,
		vk::ShaderStageFlagBits shader_stage,
		vk::ShaderModule& shader_module,
		vk::PipelineShaderStageCreateInfo& shader_stage_create_info
	);
}*/

class graphics_utils
{
public:
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