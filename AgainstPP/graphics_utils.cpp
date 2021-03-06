#include "graphics_utils.hpp"

#include <fstream>

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
	VkSubmitInfo submit_info = {};

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
	VkCommandBufferAllocateInfo command_buffer_allocate_info = {};

	command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool = command_pool;
	command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount = 1;

	VkCommandBuffer command_buffer;

	if (vkAllocateCommandBuffers (graphics_device, &command_buffer_allocate_info, &command_buffer) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_command_buffer;
	}

	VkCommandBufferBeginInfo command_buffer_begin_info = {};
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer (command_buffer, &command_buffer_begin_info) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_begin_command_buffer;
	}

	*out_buffer = command_buffer;

	return egraphics_result::success;
}

egraphics_result graphics_utils::create_vulkan_handles_for_meshes (
	std::vector<asset::mesh>& meshes,
	VkCommandPool command_pool,
	VkBuffer* staging_vertex_index_buffer,
	VkDeviceMemory* staging_vertex_index_memory,
	VkBuffer* vertex_index_buffer,
	VkDeviceMemory* vertex_index_memory,
	VkBuffer* staging_image_buffer,
	VkDeviceMemory* staging_image_memory,
	std::vector<VkImage>& scene_images,
	VkDeviceMemory* scene_images_memory
)
{
	VkDeviceSize total_size = 0;

	for (auto& mesh : meshes)
	{
		for (auto& graphics_primitive : mesh.graphics_primitves)
		{
			total_size +=
				(VkDeviceSize)graphics_primitive.positions.size () +
				(VkDeviceSize)graphics_primitive.uv0s.size () +
				(VkDeviceSize)graphics_primitive.uv1s.size () +
				(VkDeviceSize)graphics_primitive.normals.size () +
				(VkDeviceSize)graphics_primitive.indices.size ();
		}
	}

	CHECK_AGAINST_RESULT (graphics_utils::create_buffer (common_graphics::graphics_device, total_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, common_graphics::graphics_queue_family_index, staging_vertex_index_buffer));
	CHECK_AGAINST_RESULT (graphics_utils::allocate_bind_buffer_memory (common_graphics::graphics_device, staging_vertex_index_buffer, 1, common_graphics::physical_device_memory_properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, staging_vertex_index_memory));

	VkDeviceSize offset = 0;

	for (auto& mesh : meshes)
	{
		for (auto& graphics_primitive : mesh.graphics_primitves)
		{
			if (graphics_primitive.positions.size () > 0)
			{
				CHECK_AGAINST_RESULT (graphics_utils::map_data_to_device_memory (common_graphics::graphics_device, *staging_vertex_index_memory, offset, graphics_primitive.positions.size (), graphics_primitive.positions.data ()));
				graphics_primitive.positions_offset = offset;
				offset += graphics_primitive.positions.size ();
			}

			if (graphics_primitive.uv0s.size () > 0)
			{
				CHECK_AGAINST_RESULT (graphics_utils::map_data_to_device_memory (common_graphics::graphics_device, *staging_vertex_index_memory, offset, graphics_primitive.uv0s.size (), graphics_primitive.uv0s.data ()));
				graphics_primitive.uv0s_offset = offset;
				offset += graphics_primitive.uv0s.size ();
			}

			if (graphics_primitive.uv1s.size () > 0)
			{
				CHECK_AGAINST_RESULT (graphics_utils::map_data_to_device_memory (common_graphics::graphics_device, *staging_vertex_index_memory, offset, graphics_primitive.uv1s.size (), graphics_primitive.uv1s.data ()));
				graphics_primitive.uv1s_offset = offset;
				offset += graphics_primitive.uv1s.size ();
			}

			if (graphics_primitive.normals.size () > 0)
			{
				CHECK_AGAINST_RESULT (graphics_utils::map_data_to_device_memory (common_graphics::graphics_device, *staging_vertex_index_memory, offset, graphics_primitive.normals.size (), graphics_primitive.normals.data ()));
				graphics_primitive.normals_offset = offset;
				offset += graphics_primitive.normals.size ();
			}

			if (graphics_primitive.indices.size () > 0)
			{
				CHECK_AGAINST_RESULT (graphics_utils::map_data_to_device_memory (common_graphics::graphics_device, *staging_vertex_index_memory, offset, graphics_primitive.indices.size (), graphics_primitive.indices.data ()));
				graphics_primitive.indices_offset = offset;
				offset += graphics_primitive.indices.size ();
			}
		}
	}

	CHECK_AGAINST_RESULT (graphics_utils::create_buffer (common_graphics::graphics_device, total_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, common_graphics::graphics_queue_family_index, vertex_index_buffer));
	CHECK_AGAINST_RESULT (graphics_utils::allocate_bind_buffer_memory (common_graphics::graphics_device, vertex_index_buffer, 1, common_graphics::physical_device_memory_properties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertex_index_memory));
	CHECK_AGAINST_RESULT (graphics_utils::copy_buffer_to_buffer (common_graphics::graphics_device, command_pool, common_graphics::graphics_queue, *staging_vertex_index_buffer, *vertex_index_buffer, total_size));

	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, staging_vertex_index_buffer, 1, staging_vertex_index_memory);

	total_size = 0;
	std::vector<asset::image> all_images;
	for (auto& mesh : meshes)
	{
		for (auto& graphics_primitive : mesh.graphics_primitves)
		{
			all_images.push_back (graphics_primitive.material.base_color_texture.texture_image);
			total_size += graphics_primitive.material.base_color_texture.texture_image.pixels.size ();
		}
	}

	CHECK_AGAINST_RESULT (
		graphics_utils::create_buffer (
			common_graphics::graphics_device,
			total_size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			common_graphics::graphics_queue_family_index,
			staging_image_buffer
		)
	);

	std::vector<std::vector<asset::image>> all_similar_images;
	for (uint32_t i = 0; i < all_images.size (); i++)
	{
		auto image = all_images.at (i);
		bool is_image_present = false;

		for (auto& similar_images : all_similar_images)
		{
			for (auto& similar_image : similar_images)
			{
				if (image.name.compare (similar_image.name) == 0)
				{
					is_image_present = true;
					break;
				}
			}

			if (is_image_present)
			{
				break;
			}
		}

		if (is_image_present)
		{
			continue;
		}

		std::vector<asset::image> similar_images;
		similar_images.push_back (image);

		for (uint32_t j = i + 1; j < all_images.size (); j++)
		{
			auto image_to_be_checked = all_images.at (j);
			if (image.width == image_to_be_checked.width && image.height == image_to_be_checked.height)
			{
				similar_images.push_back (image_to_be_checked);
			}
		}

		all_similar_images.push_back (similar_images);
	}

	scene_images.reserve (all_similar_images.size ());

	for (auto& similar_images : all_similar_images)
	{
		VkImage scene_image = VK_NULL_HANDLE;
		CHECK_AGAINST_RESULT (
			graphics_utils::create_image (
				common_graphics::graphics_device,
				common_graphics::graphics_queue_family_index,
				VkExtent3D{ (uint32_t)similar_images[0].width, (uint32_t)similar_images[0].height, 1 },
				similar_images.size (),
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_TYPE_2D,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				&scene_image
			)
		);
		scene_images.push_back (scene_image);
	}

	CHECK_AGAINST_RESULT (
		graphics_utils::allocate_bind_buffer_memory (
			common_graphics::graphics_device,
			staging_image_buffer,
			1,
			common_graphics::physical_device_memory_properties,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			staging_image_memory
		)
	);

	offset = 0;
	std::vector<VkDeviceSize> buffer_image_offsets (all_similar_images.size ());

	for (uint32_t as = 0; as < all_similar_images.size (); as++)
	{
		VkDeviceSize buffer_image_offset = 0;
		for (uint32_t s = 0; s < all_similar_images[as].size (); s++)
		{
			CHECK_AGAINST_RESULT (
				graphics_utils::map_data_to_device_memory (
					common_graphics::graphics_device,
					*staging_image_memory,
					offset,
					all_similar_images[as][s].pixels.size (),
					all_similar_images[as][s].pixels.data ()
				)
			);
			offset += all_similar_images[as][s].pixels.size ();
			buffer_image_offset += all_similar_images[as][s].pixels.size ();
		}
		buffer_image_offsets.push_back (buffer_image_offset);
	}

	CHECK_AGAINST_RESULT (
		graphics_utils::allocate_bind_image_memory (
			common_graphics::graphics_device,
			scene_images.data (),
			scene_images.size (),
			common_graphics::physical_device_memory_properties,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			scene_images_memory
		)
	);

	for (uint32_t s = 0; s < all_similar_images.size (); s++)
	{
		CHECK_AGAINST_RESULT (
			graphics_utils::change_image_layout (
				common_graphics::graphics_device,
				command_pool,
				common_graphics::graphics_queue_family_index,
				scene_images[s],
				all_similar_images[s].size (),
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				0,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT
			)
		);

		CHECK_AGAINST_RESULT (
			graphics_utils::copy_buffer_to_image (
				common_graphics::graphics_device,
				command_pool,
				common_graphics::graphics_queue,
				buffer_image_offsets[s],
				*staging_image_buffer,
				&scene_images[s],
				VkExtent3D{ (uint32_t)all_similar_images[s][0].width, (uint32_t)all_similar_images[s][0].height, 1 },
				all_similar_images[s].size ()
			)
		);

		CHECK_AGAINST_RESULT (
			graphics_utils::change_image_layout (
				common_graphics::graphics_device,
				command_pool,
				common_graphics::graphics_queue_family_index,
				scene_images[s],
				all_similar_images[s].size (),
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_ACCESS_SHADER_READ_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			)
		);
	}

	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, staging_image_buffer, 1, staging_image_memory);

	VkComponentMapping components = {};
	VkImageViewCreateInfo image_view_create_info = {};
	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.components = components;
	image_view_create_info.format = VK_FORMAT_R8G8B8A8_UNORM;
	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;

	VkImageSubresourceRange subresource_range = {};
	subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource_range.layerCount = 1;
	subresource_range.levelCount = 1;

	image_view_create_info.subresourceRange = subresource_range;

	for (uint32_t as = 0; as < all_similar_images.size (); ++as)
	{
		for (uint32_t a = 0; a < all_similar_images[as].size (); ++a)
		{
			for (auto& mesh : meshes)
			{
				for (auto& graphics_primitve : mesh.graphics_primitves)
				{
					if (graphics_primitve.material.base_color_texture.texture_image.name.compare (all_similar_images[as][a].name) == 0)
					{
						image_view_create_info.image = scene_images[as];
						image_view_create_info.subresourceRange.baseArrayLayer = a;

						if (vkCreateImageView (
							common_graphics::graphics_device,
							&image_view_create_info,
							nullptr,
							&graphics_primitve.material.base_color_texture.texture_image.image_view) != VK_SUCCESS)
						{
							return egraphics_result::e_against_error_graphics_create_image_view;
						}
					}
				}
			}
		}
	}

	return egraphics_result::success;
}

egraphics_result graphics_utils::create_buffer (VkDevice graphics_device, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharing_mode, uint32_t graphics_queue_family_index, VkBuffer * out_buffer)
{
	VkBufferCreateInfo create_info = {};

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
	VkMemoryAllocateInfo memory_allocation = {};
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
	VkImageCreateInfo create_info = {};

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
	VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	std::vector<VkDeviceSize> offsets (image_count);

	for (uint32_t i = 0; i < image_count; i++)
	{
		offsets[i] = memory_allocate_info.allocationSize;

		VkMemoryRequirements memory_requirements = {};
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
	VkCommandPool command_pool,
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
	VkCommandBufferAllocateInfo allocate_info = {};
	allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocate_info.commandPool = command_pool;
	allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocate_info.commandBufferCount = 1;
	
	VkCommandBuffer command_buffer;

	if (vkAllocateCommandBuffers (common_graphics::graphics_device, &allocate_info, &command_buffer) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_command_buffer;
	}

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer (command_buffer, &begin_info) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_begin_command_buffer;
	}

	VkImageSubresourceRange subresource_range = {};
	subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource_range.baseMipLevel = 0;
	subresource_range.levelCount = 1;
	subresource_range.baseArrayLayer = 0;
	subresource_range.layerCount = layer_count;

	VkImageMemoryBarrier image_memory_barrier = {};
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
	vkFreeCommandBuffers (common_graphics::graphics_device, command_pool, 1, &command_buffer);

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

	VkBufferCopy buffer_copy = {};
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

	VkImageSubresourceLayers subresource_layers = {};
	subresource_layers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource_layers.layerCount = layer_count;
	
	VkBufferImageCopy buffer_image_copy = {};
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

	VkImageCopy image_copy = {};
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

	std::vector<char>buff (size);

	file.seekg (0, std::ios::beg);
	file.read (buff.data (), size);
	file.close ();

	VkShaderModuleCreateInfo shader_module_create_info = {};
	shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_module_create_info.codeSize = buff.size ();
	shader_module_create_info.pCode = reinterpret_cast<uint32_t*> (buff.data ());

	if (vkCreateShaderModule (graphics_device, &shader_module_create_info, nullptr, shader_module) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_shader_module;
	}

	VkPipelineShaderStageCreateInfo shader_stage_c_i = {};

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
		vkDestroyBuffer (graphics_device, buffers[b], NULL);
		buffers[b] = VK_NULL_HANDLE;
	}

	vkFreeMemory (graphics_device, *buffer_memory, NULL);
	*buffer_memory = VK_NULL_HANDLE;
}

void graphics_utils::destroy_images_and_image_memory (VkDevice graphics_device, VkImage* images, uint32_t image_count, VkDeviceMemory* image_memory)
{
	for (uint32_t i = 0; i < image_count; i++)
	{
		vkDestroyImage (graphics_device, images[i], nullptr);
		images[i] = VK_NULL_HANDLE;
	}

	vkFreeMemory (graphics_device, *image_memory, nullptr);
	*image_memory = VK_NULL_HANDLE;
}