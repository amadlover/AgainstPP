#include "graphics_utils.hpp"

#include <fstream>

void GraphicUtils::CreateBufferAndBufferMemory (BaseGraphics* G, vk::DeviceSize Size, vk::BufferUsageFlags Usage, vk::SharingMode SharingMode, const std::vector<uint32_t>& QueueFamilies, vk::MemoryPropertyFlags RequiredMemoryTypes, vk::Buffer& OutBuffer, vk::DeviceMemory& OutBufferMemory)
{
	const vk::BufferCreateInfo BufferCreateInfo ({}, Size, Usage, SharingMode, QueueFamilies.size (), QueueFamilies.data ());

	OutBuffer = G->GraphicsDevice.createBuffer (BufferCreateInfo);

	vk::MemoryRequirements BufferMemoryRequirements = G->GraphicsDevice.getBufferMemoryRequirements (OutBuffer);
	vk::MemoryAllocateInfo BufferMemoryAllocateInfo (BufferMemoryRequirements.size, GetMemoryTypeIndex (BufferMemoryRequirements, G->PhysicalDeviceMemoryProperties, RequiredMemoryTypes));

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

void GraphicUtils::ChangeImageLayout (BaseGraphics* G, vk::CommandPool CommandPool, vk::Image& Image, vk::ImageLayout OldLayout, vk::ImageLayout NewLayout, vk::PipelineStageFlags SrcStageMask, vk::PipelineStageFlags DstStageMask, vk::AccessFlags SrcAccessMask, vk::AccessFlags DstAccessMask)
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

void GraphicUtils::CopyBufferToImage (BaseGraphics* G, vk::CommandPool CommandPool, vk::Buffer Buffer, vk::Image& Image, vk::Extent3D ImageExtent, vk::ImageLayout DstImageLayout)
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

void GraphicUtils::CopyBufferToBuffer (BaseGraphics* G, vk::CommandPool CommandPool, vk::Buffer SrcBuffer, vk::Buffer DstBuffer, vk::DeviceSize Size)
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

void GraphicUtils::CreateShader (vk::Device GraphicsDevice, std::string FilePath, vk::ShaderStageFlagBits ShaderStage, vk::ShaderModule& ShaderModule, vk::PipelineShaderStageCreateInfo& ShaderStageCreateInfo)
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

void GraphicUtils::SubmitOneTimeCmd (vk::Queue GraphicsQueue, vk::CommandBuffer CommandBuffer)
{
	vk::SubmitInfo SubmitInfo (0, nullptr, nullptr, 1, &CommandBuffer, 0, nullptr);
	GraphicsQueue.submit (1, &SubmitInfo, nullptr);
	GraphicsQueue.waitIdle ();
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


namespace graphics_utils
{
	common_graphics::common_graphics* common_graphics_obj_ptr;

	void init (common_graphics::common_graphics* ptr)
	{
		common_graphics_obj_ptr = ptr;
	}

	void submit_one_time_cmd (vk::CommandBuffer command_buffer)
	{
		vk::SubmitInfo submit_info (0, nullptr, nullptr, 1, &command_buffer, 0, nullptr);
		common_graphics_obj_ptr->graphics_queue.submit (1, &submit_info, nullptr);
		common_graphics_obj_ptr->graphics_queue.waitIdle ();
	}

	uint32_t get_memory_type_index (vk::MemoryRequirements memory_requirements, vk::PhysicalDeviceMemoryProperties memory_properties, vk::MemoryPropertyFlags required_types)
	{
		for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
		{
			if (memory_requirements.memoryTypeBits & (1 << i) && required_types & memory_properties.memoryTypes[i].propertyFlags)
			{
				return i;
			}
		}

		return 0;
	}

	void allocate_bind_memory (
		vk::Buffer buffer, 
		vk::MemoryPropertyFlags required_types, 
		vk::DeviceMemory& out_buffer_memory
	)
	{
		vk::MemoryRequirements buffer_memory_requirements = common_graphics_obj_ptr->graphics_device.getBufferMemoryRequirements (buffer);
		vk::MemoryAllocateInfo buffer_memory_allocate_info (buffer_memory_requirements.size, get_memory_type_index (buffer_memory_requirements, common_graphics_obj_ptr->physical_device_memory_properties, required_types));

		out_buffer_memory = common_graphics_obj_ptr->graphics_device.allocateMemory (buffer_memory_allocate_info);
		common_graphics_obj_ptr->graphics_device.bindBufferMemory (buffer, out_buffer_memory, 0);
	}

	void map_data_to_buffer (
		vk::DeviceMemory memory,
		vk::DeviceSize offset,
		vk::DeviceSize size,
		void* data_source
	)
	{
		HANDLE data = common_graphics_obj_ptr->graphics_device.mapMemory (memory, offset, size);
		memcpy (data, data_source, (size_t)size);
		common_graphics_obj_ptr->graphics_device.unmapMemory (memory);
	}

	void copy_buffer_to_buffer (
		vk::CommandPool command_pool, 
		vk::Buffer src_buffer, 
		vk::Buffer dst_buffer, 
		vk::DeviceSize size
	)
	{
		vk::CommandBufferAllocateInfo command_buffer_allocate_info (command_pool, vk::CommandBufferLevel::ePrimary, 1);
		vk::CommandBuffer command_buffer = common_graphics_obj_ptr->graphics_device.allocateCommandBuffers (command_buffer_allocate_info).front ();

		vk::CommandBufferBeginInfo command_buffer_begin_info (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		vk::BufferCopy buffer_copy (0, 0, size);

		command_buffer.begin (command_buffer_begin_info);
		command_buffer.copyBuffer (src_buffer, dst_buffer, buffer_copy);
		command_buffer.end ();

		submit_one_time_cmd (command_buffer);

		common_graphics_obj_ptr->graphics_device.freeCommandBuffers (command_pool, 1, &command_buffer);
	}

	void allocate_bind_image_memory ()
	{

	}
	void copy_buffer_to_image ()
	{

	}

	void create_shader (std::string file_path,
		vk::ShaderStageFlagBits shader_stage,
		vk::ShaderModule& shader_module,
		vk::PipelineShaderStageCreateInfo& shader_shader_create_info
	)
	{
		std::ifstream file (file_path, std::ios::in | std::ios::binary | std::ios::ate);

		if (!file.good ())
		{
			char Buff[] = "Could not open file ";
			throw std::runtime_error (reinterpret_cast<const char*>(strcat_s (Buff, 21, file_path.c_str ())));
		}

		if (!file.is_open ())
		{
			char buff[] = "Could not open file ";
			throw std::runtime_error (reinterpret_cast<const char*>(strcat_s (buff, 21, file_path.c_str ())));
		}

		uint32_t size = (uint32_t)(file.tellg ());

		std::unique_ptr<char[]> buff (new char[size * sizeof (uint32_t)]);

		file.seekg (0, std::ios::beg);
		file.read ((char*)buff.get (), size);
		file.close ();

		vk::ShaderModuleCreateInfo shader_module_c_i ({}, size, reinterpret_cast<uint32_t*>(buff.get ()));
		shader_module = common_graphics_obj_ptr->graphics_device.createShaderModule (shader_module_c_i);

		vk::PipelineShaderStageCreateInfo shader_stage_c_i ({}, shader_stage, shader_module, "main");
		shader_shader_create_info = shader_stage_c_i;
	}
}