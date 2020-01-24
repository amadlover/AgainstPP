#include "graphics_utils.hpp"

#include <fstream>

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

	void map_data_to_buffer (
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