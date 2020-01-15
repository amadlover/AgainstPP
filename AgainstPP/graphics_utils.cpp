#include "graphics_utils.hpp"

#include <fstream>

namespace graphics_utils
{
	void SubmitOneTimeCmd (vk::Queue GraphicsQueue, vk::CommandBuffer CommandBuffer)
	{
		vk::SubmitInfo SubmitInfo (0, nullptr, nullptr, 1, &CommandBuffer, 0, nullptr);
		vk::ArrayProxy<const vk::SubmitInfo> SubmitInfos (1, &SubmitInfo);
		GraphicsQueue.submit (SubmitInfos, nullptr);
		GraphicsQueue.waitIdle ();
	}

	uint32_t get_memory_type_index (vk::MemoryRequirements MemoryRequirements, vk::PhysicalDeviceMemoryProperties MemoryProperties, vk::MemoryPropertyFlags RequiredMemoryTypes)
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

	void create_buffer_and_buffer_memory (BaseGraphics* G, vk::DeviceSize Size, vk::BufferUsageFlags Usage, vk::SharingMode SharingMode, const std::vector<uint32_t>& QueueFamilies, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Buffer& OutBuffer, vk::DeviceMemory& OutBufferMemory)
	{
		const vk::BufferCreateInfo BufferCreateInfo ({}, Size, Usage, SharingMode, QueueFamilies.size (), QueueFamilies.data ());

		OutBuffer = G->GraphicsDevice.createBuffer (BufferCreateInfo);
	}

	void allocate_bind_memory (vk::Device graphics_device, const vk::Buffer& Buffer, vk::PhysicalDeviceMemoryProperties physical_device_memory_properties, vk::MemoryPropertyFlags required_types, vk::DeviceMemory& out_buffer_memory)
	{
		vk::MemoryRequirements buffer_memory_requirements = graphics_device.getBufferMemoryRequirements (Buffer);
		vk::MemoryAllocateInfo BufferMemoryAllocateInfo (buffer_memory_requirements.size, get_memory_type_index (buffer_memory_requirements, physical_device_memory_properties, required_types));

		out_buffer_memory = graphics_device.allocateMemory (BufferMemoryAllocateInfo);
		graphics_device.bindBufferMemory (Buffer, out_buffer_memory, 0);
	}

	void CreateImageAndImageMemory (BaseGraphics* G, vk::ImageType ImageType, vk::Format Format, vk::Extent3D Extent, uint32_t MipLevels, uint32_t ArrayLayers, vk::SampleCountFlagBits Samples, vk::ImageTiling Tiling, vk::ImageUsageFlags Usage, vk::SharingMode SharingMode, std::vector<uint32_t> QueueFamilies, vk::ImageLayout InitialLayout, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Image& OutImage, vk::DeviceMemory& OutImageMemory)
	{
		vk::ImageCreateInfo ImageCreateInfo ({}, ImageType, Format, Extent, MipLevels, ArrayLayers, Samples, Tiling, Usage, SharingMode, QueueFamilies[0], QueueFamilies.data (), InitialLayout);
		OutImage = G->GraphicsDevice.createImage (ImageCreateInfo);

		vk::MemoryRequirements ImageMemoryRequirements = G->GraphicsDevice.getImageMemoryRequirements (OutImage);
		vk::MemoryAllocateInfo ImageMemoryAllocateInfo (ImageMemoryRequirements.size, get_memory_type_index (ImageMemoryRequirements, G->PhysicalDeviceMemoryProperties, RequiredMemoryTypes));

		OutImageMemory = G->GraphicsDevice.allocateMemory (ImageMemoryAllocateInfo);
		G->GraphicsDevice.bindImageMemory (OutImage, OutImageMemory, 0);
	}

	void ChangeImageLayout (BaseGraphics* G, vk::CommandPool CommandPool, vk::Image& Image, vk::ImageLayout OldLayout, vk::ImageLayout NewLayout, vk::PipelineStageFlags SrcStageMask, vk::PipelineStageFlags DstStageMask, vk::AccessFlags SrcAccessMask, vk::AccessFlags DstAccessMask)
	{
		vk::CommandBufferAllocateInfo CommandBufferAllocateInfo (CommandPool, vk::CommandBufferLevel::ePrimary, 1);
		vk::CommandBuffer CommandBuffer = G->GraphicsDevice.allocateCommandBuffers (CommandBufferAllocateInfo).front ();

		vk::CommandBufferBeginInfo CommandBufferBeginInfo (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		vk::ImageSubresourceRange ImageSubresourceRange (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
		vk::ImageMemoryBarrier ImageMemoryBarrier (SrcAccessMask, DstAccessMask, OldLayout, NewLayout, G->GraphicsQueueFamilies[0], G->GraphicsQueueFamilies[0], Image, ImageSubresourceRange);
		vk::ArrayProxy<const vk::ImageMemoryBarrier> ImageMemoryBarriers (1, &ImageMemoryBarrier);

		CommandBuffer.begin (CommandBufferBeginInfo);
		CommandBuffer.pipelineBarrier (SrcStageMask, DstStageMask, {}, {}, {}, ImageMemoryBarriers);
		CommandBuffer.end ();

		SubmitOneTimeCmd (G->GraphicsQueue, CommandBuffer);

		G->GraphicsDevice.freeCommandBuffers (CommandPool, CommandBuffer);
	}

	void CopyBufferToImage (BaseGraphics* G, vk::CommandPool CommandPool, vk::Buffer Buffer, vk::Image& Image, vk::Extent3D ImageExtent, vk::ImageLayout DstImageLayout)
	{
		vk::CommandBufferAllocateInfo CommandBufferAllocateInfo (CommandPool, vk::CommandBufferLevel::ePrimary, 1);
		vk::CommandBuffer CommandBuffer = G->GraphicsDevice.allocateCommandBuffers (CommandBufferAllocateInfo).front ();

		vk::CommandBufferBeginInfo CommandBufferBeginInfo (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		vk::ImageSubresourceLayers ImageSubresourceLayers (vk::ImageAspectFlagBits::eColor, 0, 0, 1);
		vk::BufferImageCopy BufferImageCopy (0, 0, 0, ImageSubresourceLayers, {}, ImageExtent);
		vk::ArrayProxy<const vk::BufferImageCopy> BufferImageCopies (1, &BufferImageCopy);

		CommandBuffer.begin (CommandBufferBeginInfo);
		CommandBuffer.copyBufferToImage (Buffer, Image, DstImageLayout, BufferImageCopies);
		CommandBuffer.end ();

		SubmitOneTimeCmd (G->GraphicsQueue, CommandBuffer);

		G->GraphicsDevice.freeCommandBuffers (CommandPool, CommandBuffer);
	}

	void CopyBufferToBuffer (BaseGraphics* G, vk::CommandPool CommandPool, vk::Buffer SrcBuffer, vk::Buffer DstBuffer, vk::DeviceSize Size)
	{
		vk::CommandBufferAllocateInfo CommandBufferAllocateInfo (CommandPool, vk::CommandBufferLevel::ePrimary, 1);
		vk::CommandBuffer CommandBuffer = G->GraphicsDevice.allocateCommandBuffers (CommandBufferAllocateInfo).front ();

		vk::CommandBufferBeginInfo CommandBufferBeginInfo (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		vk::BufferCopy BufferCopy (0, 0, Size);

		CommandBuffer.begin (CommandBufferBeginInfo);
		CommandBuffer.copyBuffer (SrcBuffer, DstBuffer, BufferCopy);
		CommandBuffer.end ();

		SubmitOneTimeCmd (G->GraphicsQueue, CommandBuffer);

		G->GraphicsDevice.freeCommandBuffers (CommandPool, CommandBuffer);
	}

	void CreateShader (vk::Device GraphicsDevice, std::string FilePath, vk::ShaderStageFlagBits ShaderStage, vk::ShaderModule& ShaderModule, vk::PipelineShaderStageCreateInfo& ShaderStageCreateInfo)
	{
		std::ifstream File (FilePath, std::ios::in | std::ios::binary | std::ios::ate);

		if (!File.good ())
		{
			char Buff[] = "Could not open file ";
			throw std::runtime_error (reinterpret_cast<const char*>(strcat_s (Buff, 21, FilePath.c_str ())));
		}

		if (!File.is_open ())
		{
			char Buff[] = "Could not open file ";
			throw std::runtime_error (reinterpret_cast<const char*>(strcat_s (Buff, 21, FilePath.c_str ())));
		}

		uint32_t Size = (uint32_t)(File.tellg ());

		std::unique_ptr<char[]> Buff (new char[Size * sizeof (uint32_t)]);

		File.seekg (0, std::ios::beg);
		File.read ((char*)Buff.get (), Size);
		File.close ();

		vk::ShaderModuleCreateInfo ShaderModuleCI ({}, Size, reinterpret_cast<uint32_t*>(Buff.get ()));
		ShaderModule = GraphicsDevice.createShaderModule (ShaderModuleCI);

		vk::PipelineShaderStageCreateInfo ShaderStageCI ({}, ShaderStage, ShaderModule, "main");
		ShaderStageCreateInfo = ShaderStageCI;
	}
}