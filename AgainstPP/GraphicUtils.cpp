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
	vk::MemoryAllocateInfo BufferMemoryAllocateInfo (BufferMemoryRequirements.size);

	for (uint32_t i = 0; i < G->PhysicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if (BufferMemoryRequirements.memoryTypeBits & (i << 1) && RequiredMemoryTypes & G->PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags)
		{
			BufferMemoryAllocateInfo.memoryTypeIndex = i;
			break;
		}
	}

	OutBufferMemory = G->GraphicsDevice.allocateMemory (BufferMemoryAllocateInfo);

	G->GraphicsDevice.bindBufferMemory (OutBuffer, OutBufferMemory, 0);
}