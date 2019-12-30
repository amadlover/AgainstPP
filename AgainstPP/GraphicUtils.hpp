#pragma once

#include <Windows.h>
#include <string>
#include <Shlwapi.h>

#include <locale>
#include <codecvt>

#include <vulkan.hpp>

#include "BaseGraphics.hpp"

class GraphicUtils
{
public:
	static std::string GetFullPath (std::string PartialFilepath);

	static void CreateBufferAndBufferMemory (BaseGraphics* G, vk::DeviceSize Size, vk::BufferUsageFlags Usage, vk::SharingMode SharingMode, const std::vector<uint32_t>& QueueFamilies, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Buffer& OutBuffer, vk::DeviceMemory& OutBufferMemory);
	static void CreateImageAndImageMemory () {}
};

