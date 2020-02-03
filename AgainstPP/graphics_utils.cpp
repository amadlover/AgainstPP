#include "graphics_utils.hpp"

#include <fstream>
/*
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

	vk::Buffer create_buffer (
		vk::DeviceSize size,
		vk::BufferUsageFlags usage,
		vk::SharingMode sharing_mode
	)
	{
		vk::BufferCreateInfo buffer_create_info (
			{},
			size,
			usage,
			sharing_mode, common_graphics_obj_ptr->graphics_queue_family_indices.size (),
			common_graphics_obj_ptr->graphics_queue_family_indices.data ()
		);

		return common_graphics_obj_ptr->graphics_device.createBuffer (buffer_create_info);
	}

	vk::DeviceMemory allocate_bind_buffer_memory (
		const vk::Buffer& buffer,
		vk::MemoryPropertyFlags required_types
	)
	{
		vk::MemoryRequirements buffer_memory_requirements = common_graphics_obj_ptr->graphics_device.getBufferMemoryRequirements (buffer);
		vk::MemoryAllocateInfo buffer_memory_allocate_info (buffer_memory_requirements.size, get_memory_type_index (buffer_memory_requirements, common_graphics_obj_ptr->physical_device_memory_properties, required_types));

		vk::DeviceMemory out_buffer_memory = common_graphics_obj_ptr->graphics_device.allocateMemory (buffer_memory_allocate_info);
		common_graphics_obj_ptr->graphics_device.bindBufferMemory (buffer, out_buffer_memory, 0);

		return out_buffer_memory;
	}

	void map_data_to_device_memory (
		vk::DeviceMemory& memory,
		vk::DeviceSize offset,
		vk::DeviceSize size,
		const void* data_source
	)
	{
		HANDLE data = common_graphics_obj_ptr->graphics_device.mapMemory (memory, offset, size);
		memcpy (data, data_source, (size_t)size);
		common_graphics_obj_ptr->graphics_device.unmapMemory (memory);
	}

	void copy_buffer_to_buffer (
		vk::CommandPool& command_pool,
		const vk::Buffer& src_buffer,
		const vk::Buffer& dst_buffer,
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

	vk::Image create_image (
		vk::Extent3D extent,
		uint32_t array_layers,
		vk::Format format,
		vk::ImageLayout initial_layout,
		vk::ImageUsageFlags usage,
		vk::SharingMode sharing_mode
	)
	{
		vk::ImageCreateInfo image_create_info (
			{},
			vk::ImageType::e2D,
			format,
			extent,
			1,
			array_layers,
			vk::SampleCountFlagBits::e1,
			vk::ImageTiling::eOptimal,
			usage,
			sharing_mode,
			common_graphics_obj_ptr->graphics_queue_family_indices.size (),
			common_graphics_obj_ptr->graphics_queue_family_indices.data (),
			initial_layout
		);

		return common_graphics_obj_ptr->graphics_device.createImage (image_create_info);
	}

	vk::DeviceMemory allocate_bind_image_memory (
		const std::vector<vk::Image>& images,
		vk::MemoryPropertyFlags required_types
	)
	{
		vk::MemoryAllocateInfo memory_allocate_info ({}, {});
		vk::DeviceSize total_size = 0;
		std::vector<vk::DeviceSize> offsets (images.size ());

		for (auto image : images)
		{
			offsets.push_back (total_size);
			vk::MemoryRequirements memory_requirements = common_graphics_obj_ptr->graphics_device.getImageMemoryRequirements (image);
			total_size += memory_requirements.size;
			memory_allocate_info.memoryTypeIndex = get_memory_type_index (memory_requirements, common_graphics_obj_ptr->physical_device_memory_properties, required_types);
		}

		memory_allocate_info.allocationSize = total_size;

		vk::DeviceMemory out_image_memory = common_graphics_obj_ptr->graphics_device.allocateMemory (memory_allocate_info);

		uint32_t image_counter = 0;
		for (auto image : images)
		{
			common_graphics_obj_ptr->graphics_device.bindImageMemory (image, out_image_memory, offsets.at (image_counter++));
		}

		return out_image_memory;
	}

	void copy_image_to_image ()
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
}*/

egraphics_result get_memory_type_index (
	VkMemoryRequirements memory_requirements,
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties,
	VkMemoryPropertyFlags required_memory_types,
	uint32_t* memory_type_index
)
{
	for (uint32_t i = 0; i < physical_device_memory_properties.memoryTypeCount; i++)
	{
		if (memory_requirements.memoryTypeBits & (1 << i) && required_memory_types & physical_device_memory_properties.memoryTypes[i].propertyFlags)
		{
			*memory_type_index = i;
			break;
		}
	}
		
	return egraphics_result::success;
}

egraphics_result submit_one_time_cmd (VkQueue graphics_queue, VkCommandBuffer command_buffer)
{
	VkSubmitInfo submit_info = { 0 };

	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;

	if (vkQueueSubmit (graphics_queue, 1, &submit_info, 0) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_queue_submit;
	}

	vkQueueWaitIdle (graphics_queue);

	return egraphics_result::success;
}

egraphics_result get_one_time_command_buffer (VkDevice graphics_device, VkCommandPool command_pool, VkCommandBuffer* out_buffer)
{
	VkCommandBufferAllocateInfo command_buffer_allocate_info = { 0 };

	command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool = command_pool;
	command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount = 1;

	VkCommandBuffer command_buffer;

	if (vkAllocateCommandBuffers (graphics_device, &command_buffer_allocate_info, &command_buffer) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_command_buffer;
	}

	VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer (command_buffer, &command_buffer_begin_info) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_begin_command_buffer;
	}

	*out_buffer = command_buffer;

	return egraphics_result::success;
}

egraphics_result graphics_utils::create_buffer (VkDevice graphics_device, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharing_mode, uint32_t graphics_queue_family_index, VkBuffer * out_buffer)
{
	VkBufferCreateInfo create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	create_info.size = size;
	create_info.queueFamilyIndexCount = 1;
	create_info.pQueueFamilyIndices = &graphics_queue_family_index;
	create_info.usage = usage;

	if (vkCreateBuffer (graphics_device, &create_info, NULL, out_buffer) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_buffer;
	}

	return egraphics_result::success;
}

egraphics_result graphics_utils::allocate_bind_buffer_memory (VkDevice graphics_device, VkBuffer* buffers, uint32_t buffer_count, VkPhysicalDeviceMemoryProperties physical_device_memory_properties, VkMemoryPropertyFlags required_types, VkDeviceMemory* out_memory)
{
	VkMemoryAllocateInfo memory_allocation = { 0 };
	memory_allocation.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

	std::vector<VkDeviceSize> offsets (buffer_count);

	for (uint32_t b = 0; b < buffer_count; b++)
	{
		offsets[b] = memory_allocation.allocationSize;

		VkMemoryRequirements memory_requirements;
		vkGetBufferMemoryRequirements (graphics_device, buffers[b], &memory_requirements);

		memory_allocation.allocationSize += memory_requirements.size;
		CHECK_AGAINST_RESULT (get_memory_type_index (memory_requirements, physical_device_memory_properties, required_types, &memory_allocation.memoryTypeIndex));
	}

	if (vkAllocateMemory (graphics_device, &memory_allocation, NULL, out_memory) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_memory;
	}

	for (uint32_t b = 0; b < buffer_count; b++)
	{
		if (vkBindBufferMemory (graphics_device, buffers[b], *out_memory, offsets[b]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_bind_buffer_memory;
		}
	}

	return egraphics_result::success;
}

egraphics_result graphics_utils::map_data_to_device_memory (VkDevice graphics_device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, void* data_source)
{
	void* Data;
	if (vkMapMemory (graphics_device, memory, offset, size, 0, &Data) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_map_memory;
	}

	std::memcpy (Data, data_source, (size_t)size);
	vkUnmapMemory (graphics_device, memory);

	return egraphics_result::success;
}

egraphics_result graphics_utils::create_image (
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
)
{
	VkImageCreateInfo create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	create_info.arrayLayers = array_layers;
	create_info.extent = extent;
	create_info.format = format;
	create_info.mipLevels = 1;
	create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	create_info.initialLayout = initial_layout;
	create_info.sharingMode = sharing_mode;
	create_info.queueFamilyIndexCount = 1;
	create_info.pQueueFamilyIndices = &graphics_queue_family_index;
	create_info.usage = usage;
	create_info.imageType = image_type;
	create_info.tiling = tiling;

	if (vkCreateImage (graphics_device, &create_info, NULL, out_image) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_image;
	}

	return egraphics_result::success;
}

egraphics_result graphics_utils::allocate_bind_image_memory (VkDevice graphics_device, VkImage* images, uint32_t image_count, VkPhysicalDeviceMemoryProperties physical_device_memory_properties, VkMemoryPropertyFlags required_types, VkDeviceMemory* out_memory)
{
	VkMemoryAllocateInfo memory_allocate_info = { 0 };
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	std::vector<VkDeviceSize> offsets (image_count);

	for (uint32_t i = 0; i < image_count; i++)
	{
		offsets[i] = memory_allocate_info.allocationSize;

		VkMemoryRequirements memory_requirements = { 0 };
		vkGetImageMemoryRequirements (graphics_device, images[i], &memory_requirements);
		memory_allocate_info.allocationSize += memory_requirements.size;
		CHECK_AGAINST_RESULT (get_memory_type_index (memory_requirements, physical_device_memory_properties, required_types, &memory_allocate_info.memoryTypeIndex));
	}

	if (vkAllocateMemory (graphics_device, &memory_allocate_info, NULL, out_memory) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_memory;
	}

	for (uint32_t i = 0; i < image_count; i++)
	{
		if (vkBindImageMemory (graphics_device, images[i], *out_memory, offsets[i]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_bind_image_memory;
		}
	}

	return egraphics_result::success;
}

egraphics_result graphics_utils::change_image_layout (
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
)
{
	VkCommandBufferAllocateInfo allocate_info = { 0 };
	allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocate_info.commandPool = common_graphics::command_pool;
	allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocate_info.commandBufferCount = 1;
	
	VkCommandBuffer command_buffer;

	if (vkAllocateCommandBuffers (common_graphics::graphics_device, &allocate_info, &command_buffer) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_command_buffer;
	}

	VkCommandBufferBeginInfo begin_info = { 0 };
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer (command_buffer, &begin_info) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_begin_command_buffer;
	}

	VkImageSubresourceRange subresource_range = { 0 };
	subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource_range.baseMipLevel = 0;
	subresource_range.levelCount = 1;
	subresource_range.baseArrayLayer = 0;
	subresource_range.layerCount = layer_count;

	VkImageMemoryBarrier image_memory_barrier = { 0 };
	image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	image_memory_barrier.image = image;
	image_memory_barrier.srcQueueFamilyIndex = graphics_queue_family_index;
	image_memory_barrier.dstQueueFamilyIndex = graphics_queue_family_index;
	image_memory_barrier.oldLayout = old_layout;
	image_memory_barrier.newLayout = new_layout;
	image_memory_barrier.srcAccessMask = src_access;
	image_memory_barrier.dstAccessMask = dst_access;
	image_memory_barrier.subresourceRange = subresource_range;
	
	vkCmdPipelineBarrier (command_buffer, src_stage, dst_stage, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
	if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_end_command_buffer;
	}
	CHECK_AGAINST_RESULT (submit_one_time_cmd (common_graphics::graphics_queue, command_buffer));
	vkFreeCommandBuffers (common_graphics::graphics_device, common_graphics::command_pool, 1, &command_buffer);

	return egraphics_result::success;
}

egraphics_result graphics_utils::copy_buffer_to_buffer (
	VkDevice graphics_device, 
	VkCommandPool command_pool, 
	VkQueue graphics_queue, 
	VkBuffer src_buffer, 
	VkBuffer dst_buffer, 
	VkDeviceSize size
)
{
	VkCommandBuffer command_buffer;
	CHECK_AGAINST_RESULT (get_one_time_command_buffer (graphics_device, command_pool, &command_buffer));

	VkBufferCopy buffer_copy = { 0 };
	buffer_copy.size = size;

	vkCmdCopyBuffer (command_buffer, src_buffer, dst_buffer, 1, &buffer_copy);
	if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_end_command_buffer;
	}
	CHECK_AGAINST_RESULT (submit_one_time_cmd (graphics_queue, command_buffer));
	vkFreeCommandBuffers (graphics_device, command_pool, 1, &command_buffer);

	return egraphics_result::success;
}

egraphics_result graphics_utils::copy_buffer_to_image (
	VkDevice graphics_device,
	VkCommandPool command_pool,
	VkQueue graphics_queue,
	VkDeviceSize offset,
	VkBuffer buffer,
	VkImage* image,
	VkExtent3D extent,
	uint32_t layer_count
)
{
	VkCommandBuffer command_buffer;
	CHECK_AGAINST_RESULT (get_one_time_command_buffer (graphics_device, command_pool, &command_buffer));

	VkImageSubresourceLayers subresource_layers = { 0 };
	subresource_layers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource_layers.layerCount = layer_count;
	
	VkBufferImageCopy buffer_image_copy = { 0 };
	buffer_image_copy.bufferOffset = offset;
	buffer_image_copy.imageExtent = extent;
	buffer_image_copy.imageSubresource = subresource_layers;

	vkCmdCopyBufferToImage (command_buffer, buffer, *image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);
	if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_end_command_buffer;
	}
	
	CHECK_AGAINST_RESULT (submit_one_time_cmd (graphics_queue, command_buffer));
	vkFreeCommandBuffers (graphics_device, command_pool, 1, &command_buffer);

	return egraphics_result::success;
}

egraphics_result graphics_utils::copy_image_to_image (
	VkDevice graphics_device,
	VkCommandPool command_pool,
	VkImage src_image,
	VkImageLayout src_image_layout,
	VkImage dst_image,
	VkImageLayout dst_image_layout,
	VkExtent3D extent
)
{
	VkCommandBuffer command_buffer;
	CHECK_AGAINST_RESULT (get_one_time_command_buffer (graphics_device, command_pool, &command_buffer));

	VkImageCopy image_copy = { 0 };
	image_copy.extent = extent;

	vkCmdCopyImage (
		command_buffer,
		src_image,
		src_image_layout,
		dst_image,
		dst_image_layout,
		1,
		&image_copy
	);

	return egraphics_result::success;
}

egraphics_result graphics_utils::create_shader (const char* file_path, VkDevice graphics_device, VkShaderStageFlagBits shader_stage, VkShaderModule* shader_module, VkPipelineShaderStageCreateInfo* shader_stage_create_info)
{
	std::ifstream file (file_path, std::ios::in | std::ios::binary | std::ios::ate);

	if (!file.good ())
	{
		return egraphics_result::e_against_error_system_open_file;
	}

	if (!file.is_open ())
	{
		return egraphics_result::e_against_error_system_open_file;
	}

	uint32_t size = (uint32_t)(file.tellg ());

	std::unique_ptr<char[]> buff (new char[size * sizeof (uint32_t)]);

	file.seekg (0, std::ios::beg);
	file.read ((char*)buff.get (), size);
	file.close ();

	VkPipelineShaderStageCreateInfo shader_stage_c_i = { 0 };

	shader_stage_c_i.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stage_c_i.stage = shader_stage;
	shader_stage_c_i.module = *shader_module;
	shader_stage_c_i.pName = "main";

	if (shader_stage == VK_SHADER_STAGE_VERTEX_BIT)
	{
		*shader_stage_create_info = shader_stage_c_i;
	}
	else if (shader_stage == VK_SHADER_STAGE_FRAGMENT_BIT)
	{
		*shader_stage_create_info = shader_stage_c_i;
	}

	return egraphics_result::success;
}

void graphics_utils::destroy_buffers_and_buffer_memory (VkDevice graphics_device, VkBuffer* buffers, uint32_t buffer_count, VkDeviceMemory* buffer_memory)
{
	for (uint32_t b = 0; b < buffer_count; b++)
	{
		if (buffers[b] != VK_NULL_HANDLE)
		{
			vkDestroyBuffer (graphics_device, buffers[b], NULL);
			buffers[b] = VK_NULL_HANDLE;
		}
	}

	if (buffer_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory (graphics_device, *buffer_memory, NULL);
		*buffer_memory = VK_NULL_HANDLE;
	}
}

void graphics_utils::destroy_images_and_image_memory (VkDevice graphics_device, VkImage* images, uint32_t image_count, VkDeviceMemory* image_memory)
{
	for (uint32_t i = 0; i < image_count; i++)
	{
		if (images[i] != VK_NULL_HANDLE)
		{
			vkDestroyImage (graphics_device, images[i], nullptr);
			images[i] = VK_NULL_HANDLE;
		}
	}

	if (image_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory (graphics_device, *image_memory, nullptr);
		*image_memory = VK_NULL_HANDLE;
	}
}
