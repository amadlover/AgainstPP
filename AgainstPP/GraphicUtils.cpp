#include "GraphicUtils.hpp"

std::string GraphicUtils::GetFullPath (std::string PartialFilepath)
{
	TCHAR Path[MAX_PATH];
	HMODULE Module = GetModuleHandle (NULL);
	GetModuleFileName (Module, Path, MAX_PATH);
	PathRemoveFileSpec (Path);

	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	return converter.to_bytes (std::wstring (Path)) + PartialFilepath;
}

void GraphicUtils::CreateBufferAndBufferMemory (BaseGraphics* G, vk::DeviceSize Size, vk::BufferUsageFlags Usage, vk::SharingMode SharingMode, const std::vector<uint32_t>& QueueFamilies, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Buffer& OutBuffer, vk::DeviceMemory& OutBufferMemory)
{
	const vk::BufferCreateInfo BufferCreateInfo ({}, Size, Usage, SharingMode, QueueFamilies.size (), QueueFamilies.data ());

	OutBuffer = G->GraphicsDevice.createBuffer (BufferCreateInfo);

	vk::MemoryRequirements BufferMemoryRequirements = G->GraphicsDevice.getBufferMemoryRequirements (OutBuffer);
	vk::MemoryAllocateInfo BufferMemoryAllocateInfo (BufferMemoryRequirements.size,	GetMemoryTypeIndex (BufferMemoryRequirements, G->PhysicalDeviceMemoryProperties, RequiredMemoryTypes));

	OutBufferMemory = G->GraphicsDevice.allocateMemory (BufferMemoryAllocateInfo);
	G->GraphicsDevice.bindBufferMemory (OutBuffer, OutBufferMemory, 0);
}

void GraphicUtils::CreateImageAndImageMemory (BaseGraphics* G, vk::ImageType ImageType, vk::Format Format, vk::Extent3D Extent, uint32_t MipLevels, uint32_t ArrayLayers, vk::SampleCountFlagBits Samples, vk::ImageTiling Tiling, vk::ImageUsageFlags Usage, vk::SharingMode SharingMode, std::vector<uint32_t> QueueFamilies, vk::ImageLayout InitialLayout, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Image& OutImage, vk::DeviceMemory& OutImageMemory)
{
	vk::ImageCreateInfo ImageCreateInfo ({}, ImageType, Format, Extent, MipLevels, ArrayLayers, Samples, Tiling, Usage, SharingMode, QueueFamilies[0], QueueFamilies.data (), InitialLayout);
	OutImage = G->GraphicsDevice.createImage (ImageCreateInfo);

	vk::MemoryRequirements ImageMemoryRequirements = G->GraphicsDevice.getImageMemoryRequirements (OutImage);
	vk::MemoryAllocateInfo ImageMemoryAllocateInfo (ImageMemoryRequirements.size, GetMemoryTypeIndex (ImageMemoryRequirements, G->PhysicalDeviceMemoryProperties, RequiredMemoryTypes));

	OutImageMemory = G->GraphicsDevice.allocateMemory (ImageMemoryAllocateInfo);
	G->GraphicsDevice.bindImageMemory (OutImage, OutImageMemory, 0);
}

uint32_t GraphicUtils::GetMemoryTypeIndex (vk::MemoryRequirements MemoryRequirements, vk::PhysicalDeviceMemoryProperties MemoryProperties, vk::MemoryPropertyFlags RequiredMemoryTypes)
{
	for (uint32_t i = 0; i < MemoryProperties.memoryTypeCount; i++)
	{
		if (MemoryRequirements.memoryTypeBits & (1 << i) && RequiredMemoryTypes & MemoryProperties.memoryTypes[i].propertyFlags)
		{
			return i;
		}
	}

	return 0;
}
