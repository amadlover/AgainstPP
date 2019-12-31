#pragma once

#include <Windows.h>
#include <string>
#include <Shlwapi.h>

#include <locale>
#include <codecvt>
#include <fstream>

#include <vulkan.hpp>

#include "BaseGraphics.hpp"

class GraphicUtils
{
public:
	static std::string GetFullPath (std::string PartialFilepath);

	static void CreateBufferAndBufferMemory (BaseGraphics* G, vk::DeviceSize Size, vk::BufferUsageFlags Usage, vk::SharingMode SharingMode, const std::vector<uint32_t>& QueueFamilies, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Buffer& OutBuffer, vk::DeviceMemory& OutBufferMemory);
	static void CreateImageAndImageMemory (BaseGraphics* G, vk::ImageType ImageType, vk::Format Format, vk::Extent3D Extent, uint32_t MipLevels, uint32_t ArrayLayers, vk::SampleCountFlagBits Samples, vk::ImageTiling Tiling, vk::ImageUsageFlags Usage, vk::SharingMode SharingMode, std::vector<uint32_t> QueueFamilies, vk::ImageLayout InitialLayout, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Image& OutImage, vk::DeviceMemory& OutImageMemory);
	static void ChangeImageLayout (BaseGraphics* G, vk::CommandPool CommandPool, vk::Image& Image, vk::ImageLayout OldLayout, vk::ImageLayout NewLayout, vk::PipelineStageFlags SrcStageMask, vk::PipelineStageFlags DstStageMask, vk::AccessFlags SrcAccessMask, vk::AccessFlags DstAccessMask);
	static void CopyBufferToImage (BaseGraphics* G, vk::CommandPool CommandPool, vk::Buffer Buffer, vk::Image& Image, vk::Extent3D ImageExtent, vk::ImageLayout DstImageLayout);

	static void CreateShader (vk::Device GraphicsDevice, std::string FilePath, vk::ShaderStageFlagBits ShaderStage, vk::ShaderModule& ShaderModule, vk::PipelineShaderStageCreateInfo& ShaderStageCreateInfo);

private:
	static uint32_t GetMemoryTypeIndex (vk::MemoryRequirements MemoryRequirements, vk::PhysicalDeviceMemoryProperties MemoryProperties, vk::MemoryPropertyFlags RequiredMemoryTypes);
	static void SubmitOneTimeCmd (vk::Queue GraphicsQueue, vk::CommandBuffer CommandBuffer);
};