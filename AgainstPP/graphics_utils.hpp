#pragma once

#include <string>

#include <vulkan.hpp>

#include "common_graphics.hpp"

class GraphicUtils
{
public:
	static void CreateBufferAndBufferMemory (BaseGraphics* G, vk::DeviceSize Size, vk::BufferUsageFlags Usage, vk::SharingMode SharingMode, const std::vector<uint32_t>& QueueFamilies, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Buffer& OutBuffer, vk::DeviceMemory& OutBufferMemory);
	static void CreateImageAndImageMemory (BaseGraphics* G, vk::ImageType ImageType, vk::Format Format, vk::Extent3D Extent, uint32_t MipLevels, uint32_t ArrayLayers, vk::SampleCountFlagBits Samples, vk::ImageTiling Tiling, vk::ImageUsageFlags Usage, vk::SharingMode SharingMode, std::vector<uint32_t> QueueFamilies, vk::ImageLayout InitialLayout, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Image& OutImage, vk::DeviceMemory& OutImageMemory);
	static void ChangeImageLayout (BaseGraphics* G, vk::CommandPool CommandPool, vk::Image& Image, vk::ImageLayout OldLayout, vk::ImageLayout NewLayout, vk::PipelineStageFlags SrcStageMask, vk::PipelineStageFlags DstStageMask, vk::AccessFlags SrcAccessMask, vk::AccessFlags DstAccessMask);
	static void CopyBufferToImage (BaseGraphics* G, vk::CommandPool CommandPool, vk::Buffer Buffer, vk::Image& Image, vk::Extent3D ImageExtent, vk::ImageLayout DstImageLayout);
	static void CopyBufferToBuffer (BaseGraphics* G, vk::CommandPool CommandPool, vk::Buffer SrcBuffer, vk::Buffer DstBuffer, vk::DeviceSize Size);

	static void CreateShader (vk::Device GraphicsDevice, std::string FilePath, vk::ShaderStageFlagBits ShaderStage, vk::ShaderModule& ShaderModule, vk::PipelineShaderStageCreateInfo& ShaderStageCreateInfo);

private:
	static uint32_t GetMemoryTypeIndex (vk::MemoryRequirements MemoryRequirements, vk::PhysicalDeviceMemoryProperties MemoryProperties, vk::MemoryPropertyFlags RequiredMemoryTypes);
	static void SubmitOneTimeCmd (vk::Queue GraphicsQueue, vk::CommandBuffer CommandBuffer);
};

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
		void* data_source
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