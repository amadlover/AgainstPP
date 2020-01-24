#pragma once

#include <string>

#include <vulkan.hpp>

#include "common_graphics.hpp"

namespace graphics_utils
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

	void map_data_to_buffer (
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

	void copy_buffer_to_image ();

	void create_shader (
		std::string file_path,
		vk::ShaderStageFlagBits shader_stage,
		vk::ShaderModule& shader_module,
		vk::PipelineShaderStageCreateInfo& shader_stage_create_info
	);
}