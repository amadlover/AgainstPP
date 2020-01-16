#pragma once

#include <string>

#include <vulkan.hpp>

#include "graphics.hpp"

namespace graphics_utils
{
	void create_buffer_and_buffer_memory 
	(
		BaseGraphics* G, 
		vk::DeviceSize Size, 
		vk::BufferUsageFlags Usage, 
		vk::SharingMode SharingMode, 
		const std::vector<uint32_t>& QueueFamilies, 
		vk::MemoryPropertyFlags RequiredMemoryTypes, 
		vk::Buffer& OutBuffer, 
		vk::DeviceMemory& OutBufferMemory
	);

	void allocate_bind_memory 
	(
		vk::Device graphics_device, 
		const vk::Buffer Buffers, 
		vk::PhysicalDeviceMemoryProperties physical_device_memory_properties, 
		vk::MemoryPropertyFlags required_types, 
		vk::DeviceMemory& out_buffer_memory
	);

	void CreateImageAndImageMemory 
	(
		BaseGraphics* G, 
		vk::ImageType ImageType, 
		vk::Format Format, 
		vk::Extent3D Extent, 
		uint32_t MipLevels, 
		uint32_t ArrayLayers, 
		vk::SampleCountFlagBits Samples, 
		vk::ImageTiling Tiling, 
		vk::ImageUsageFlags Usage, 
		vk::SharingMode SharingMode, 
		std::vector<uint32_t> QueueFamilies, 
		vk::ImageLayout InitialLayout, 
		vk::MemoryPropertyFlags RequiredMemoryTypes, 
		vk::Image& OutImage, 
		vk::DeviceMemory& OutImageMemory
	);

	void ChangeImageLayout 
	(
		BaseGraphics* G, 
		vk::CommandPool CommandPool, 
		vk::Image& Image, 
		vk::ImageLayout OldLayout, 
		vk::ImageLayout NewLayout, 
		vk::PipelineStageFlags SrcStageMask, 
		vk::PipelineStageFlags DstStageMask, 
		vk::AccessFlags SrcAccessMask, 
		vk::AccessFlags DstAccessMask
	);

	void CopyBufferToImage 
	(
		BaseGraphics* G, 
		vk::CommandPool CommandPool, 
		vk::Buffer Buffer, 
		vk::Image& Image, 
		vk::Extent3D ImageExtent, 
		vk::ImageLayout DstImageLayout
	);

	void CopyBufferToBuffer 
	(
		BaseGraphics* G, 
		vk::CommandPool CommandPool, 
		vk::Buffer SrcBuffer, 
		vk::Buffer DstBuffer, 
		vk::DeviceSize Size
	);

	void CreateShader 
	(
		vk::Device GraphicsDevice, 
		std::string FilePath, 
		vk::ShaderStageFlagBits ShaderStage, 
		vk::ShaderModule& ShaderModule, 
		vk::PipelineShaderStageCreateInfo& ShaderStageCreateInfo
	);

	//uint32_t get_memory_type_index (vk::MemoryRequirements MemoryRequirements, vk::PhysicalDeviceMemoryProperties MemoryProperties, vk::MemoryPropertyFlags RequiredMemoryTypes);
	//void SubmitOneTimeCmd (vk::Queue GraphicsQueue, vk::CommandBuffer CommandBuffer);
};