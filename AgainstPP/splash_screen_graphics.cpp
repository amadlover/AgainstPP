#include "splash_screen_graphics.hpp"
#include "Error.hpp"
#include "Utils.hpp"
#include "graphics_utils.hpp"
#include "common_graphics.hpp"

#include <vector>

/*
namespace splash_screen_graphics
{
	common_graphics::common_graphics* common_graphics_obj_ptr;
	vk::Device graphics_device;
	vk::RenderPass render_pass;
	std::vector<vk::Framebuffer> swapchain_framebuffers;
	vk::PipelineLayout graphics_pipeline_layout;
	vk::Pipeline graphics_pipeline;
	std::vector<vk::PipelineShaderStageCreateInfo> pipeline_shader_stage_create_infos;
	vk::ShaderModule vertex_shader_module;
	vk::ShaderModule fragment_shader_module;
	vk::DescriptorPool descriptor_pool;
	std::vector<vk::DescriptorSetLayout> descriptor_set_layouts;
	vk::Semaphore signal_semaphore;
	vk::Semaphore wait_semaphore;
	std::vector<vk::Fence> swapchain_fences;
	std::vector<vk::CommandBuffer> swapchain_command_buffers;

	vk::DeviceMemory vertex_index_buffer_memory;
	vk::Buffer vertex_index_buffer;

	vk::DeviceMemory image_memory;
	std::vector<vk::ImageView> image_views;
	std::vector<vk::Image> splash_screen_graphics_images;
	std::vector<vk::DescriptorSet> descriptor_sets;

	void check_for_similar_image_indices (
		uint32_t from_index,
		const asset::staging_image::staging_image& current_image,
		const std::vector<asset::staging_image::staging_image>& images,
		std::vector<uint32_t>& similar_image_indices
	)
	{
		while (from_index < images.size ())
		{
			if (current_image.width == images.at (from_index).width &&
				current_image.height == images.at (from_index).height)
			{
				similar_image_indices.push_back (from_index);
			}

			++from_index;
		}
	}

	void create_vulkan_handles_for_images (const std::vector<asset::staging_image::staging_image>& images, std::vector<asset::mesh::mesh>& meshes)
	{
		std::map<vk::Image, std::vector<uint32_t>> similar_indices;

		for (uint32_t index = 0; index < images.size (); index++)
		{
			bool is_index_already_processed = false;
			for (const auto& image_similar_indices : similar_indices)
			{
				for (const auto& processed_index : image_similar_indices.second)
				{
					if (processed_index == index)
					{
						is_index_already_processed = true;
						break;
					}
				}
				if (is_index_already_processed)
				{
					break;
				}
			}

			if (is_index_already_processed)
			{
				continue;
			}

			asset::staging_image::staging_image staging_image = images.at (index);
			std::vector<uint32_t> similar_images_indices;
			similar_images_indices.reserve (images.size ());
			similar_images_indices.push_back (index);

			uint32_t start_index = index + 1;
			check_for_similar_image_indices (
				start_index,
				staging_image,
				images,
				similar_images_indices
			);

			vk::DeviceSize similar_image_size = 0;

			for (const auto& similar_index : similar_images_indices)
			{
				const asset::staging_image::staging_image similar_image = images.at (similar_index);
				similar_image_size += similar_image.pixels.size ();
			}

			vk::Extent3D image_extent{
				(uint32_t)images.at (similar_images_indices.at (0)).width,
				(uint32_t)images.at (similar_images_indices.at (0)).height,
				(uint32_t)1
			};

			vk::Image vkimage = graphics_utils::create_image (
				image_extent,
				similar_images_indices.size (),
				vk::Format::eR8G8B8A8Unorm,
				vk::ImageLayout::eUndefined,
				vk::ImageUsageFlagBits::eSampled,
				vk::SharingMode::eExclusive
			);

			similar_indices.insert (std::make_pair (vkimage, similar_images_indices));
		}

		std::vector<vk::Image> new_images;

		for (auto image_similar_indices : similar_indices)
		{
			new_images.push_back (image_similar_indices.first);
		}

		image_memory = graphics_utils::allocate_bind_image_memory (new_images, vk::MemoryPropertyFlagBits::eDeviceLocal);
		splash_screen_graphics_images = new_images;

		for (const auto& image_similar_indices : similar_indices)
		{
			vk::ImageSubresourceRange image_subresource_range (
				vk::ImageAspectFlagBits::eColor,
				0,
				1,
				0,
				1
			);

			vk::ImageViewCreateInfo image_view_create_info (
				{},
				image_similar_indices.first,
				vk::ImageViewType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				{
					vk::ComponentSwizzle::eIdentity,
					vk::ComponentSwizzle::eIdentity,
					vk::ComponentSwizzle::eIdentity,
					vk::ComponentSwizzle::eIdentity
				},
				{}
			);

			uint32_t similar_index_counter = 0;
			for (const auto& similar_index : image_similar_indices.second)
			{
				image_subresource_range.baseArrayLayer = similar_index_counter;
				image_view_create_info.subresourceRange = image_subresource_range;

				vk::ImageView image_view = graphics_device.createImageView (image_view_create_info);

				for (auto& mesh : meshes)
				{
					for (auto& graphics_primitive : mesh.graphics_primitves)
					{
						if (graphics_primitive.material.base_color_texture.image_index == similar_index)
						{
							graphics_primitive.material.base_color_texture.image_view = image_view;
						}
					}
				}

				image_views.push_back (image_view);
				++similar_index_counter;
			}
		}
	}

	void map_mesh_data (
		vk::DeviceMemory& device_memory,
		vk::DeviceSize& offset,
		vk::DeviceSize& data_offset_for_memory,
		const std::vector<uint8_t>& data
	)
	{
		graphics_utils::map_data_to_device_memory (device_memory, offset, data.size (), data.data ());
		data_offset_for_memory = offset;
		offset += data.size ();
	}

	void create_vulkan_handles_for_meshes (std::vector<asset::mesh::mesh>& meshes)
	{
		vk::DeviceSize total_size = 0;

		for (const auto& asset : meshes)
		{
			for (const auto& graphics_primitive : asset.graphics_primitves)
			{
				total_size += (vk::DeviceSize)graphics_primitive.positions.size () +
					(vk::DeviceSize)graphics_primitive.normals.size () +
					(vk::DeviceSize)graphics_primitive.uv0s.size () +
					(vk::DeviceSize)graphics_primitive.uv1s.size () +
					(vk::DeviceSize)(graphics_primitive.indices.size ());
			}
		}

		vk::Buffer staging_vertex_index_buffer = graphics_utils::create_buffer (total_size, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive);
		vk::DeviceMemory staging_buffer_memory = graphics_utils::allocate_bind_buffer_memory (staging_vertex_index_buffer, vk::MemoryPropertyFlagBits::eHostVisible);

		vk::DeviceSize offset = 0;

		for (auto& mesh : meshes)
		{
			for (auto& graphics_primitive : mesh.graphics_primitves)
			{
				if (graphics_primitive.positions.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory,
						offset,
						graphics_primitive.positions_offset,
						graphics_primitive.positions
					);
				}

				if (graphics_primitive.normals.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory,
						offset,
						graphics_primitive.normals_offset,
						graphics_primitive.normals
					);
				}

				if (graphics_primitive.uv0s.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory,
						offset,
						graphics_primitive.uv0s_offset,
						graphics_primitive.uv0s
					);
				}

				if (graphics_primitive.uv1s.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory,
						offset,
						graphics_primitive.uv1s_offset,
						graphics_primitive.uv1s
					);
				}

				if (graphics_primitive.indices.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory,
						offset,
						graphics_primitive.indices_offset,
						graphics_primitive.indices
					);
				}
			}
		}

		vertex_index_buffer = graphics_utils::create_buffer (
			total_size,
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer,
			vk::SharingMode::eExclusive
		);

		vertex_index_buffer_memory = graphics_utils::allocate_bind_buffer_memory (
			vertex_index_buffer,
			vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		graphics_utils::copy_buffer_to_buffer (
			common_graphics_obj_ptr->command_pool,
			staging_vertex_index_buffer,
			vertex_index_buffer,
			total_size
		);

		for (auto& mesh : meshes)
		{
			for (auto& graphics_primitive : mesh.graphics_primitves)
			{
				graphics_primitive.buffer = &vertex_index_buffer;
				graphics_primitive.buffer_memory = &vertex_index_buffer_memory;
			}
		}

		graphics_device.destroyBuffer (staging_vertex_index_buffer);
		graphics_device.freeMemory (staging_buffer_memory);
	}

	void create_descriptor_sets (std::vector<asset::mesh::mesh>& meshes)
	{
		vk::DescriptorPoolSize pool_size (vk::DescriptorType::eCombinedImageSampler, 1);
		vk::DescriptorPoolCreateInfo descriptor_pool_create_info (
			{},
			6,
			1,
			&pool_size);
		descriptor_pool = graphics_device.createDescriptorPool (descriptor_pool_create_info);

		vk::DescriptorSetLayoutBinding descriptor_set_layout_binding (0, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment);
		vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info ({}, 1, &descriptor_set_layout_binding);
		vk::DescriptorSetLayout descriptor_set_layout = graphics_device.createDescriptorSetLayout (descriptor_set_layout_create_info);
		descriptor_set_layouts.push_back (descriptor_set_layout);

		vk::DescriptorSetAllocateInfo descriptor_set_allocate_info (descriptor_pool, 1, &descriptor_set_layout);

		for (auto& mesh : meshes)
		{
			for (auto& graphics_primitive : mesh.graphics_primitves)
			{
				graphics_primitive.material.base_color_texture.descriptor_set = graphics_device.allocateDescriptorSets (descriptor_set_allocate_info).front ();
				vk::DescriptorImageInfo descriptor_image_info (
					common_graphics_obj_ptr->common_sampler,
					graphics_primitive.material.base_color_texture.image_view,
					vk::ImageLayout::eShaderReadOnlyOptimal
				);

				vk::WriteDescriptorSet write_descriptor_set (
					graphics_primitive.material.base_color_texture.descriptor_set,
					0,
					0,
					1,
					vk::DescriptorType::eCombinedImageSampler,
					&descriptor_image_info);

				graphics_device.updateDescriptorSets (1, &write_descriptor_set, 0, nullptr);
			}
		}
	}

	void create_renderpasses ()
	{
		vk::AttachmentDescription color_attachment_description (
			{},
			common_graphics_obj_ptr->chosen_surface_format.format,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::ePresentSrcKHR
		);

		vk::AttachmentReference color_attachment_reference (0, vk::ImageLayout::eColorAttachmentOptimal);

		vk::SubpassDescription subpass_description (
			{},
			vk::PipelineBindPoint::eGraphics,
			{},
			{},
			1,
			&color_attachment_reference
		);

		vk::RenderPassCreateInfo renderpass_create_info (
			{},
			1,
			&color_attachment_description,
			1,
			&subpass_description,
			0,
			nullptr
		);

		render_pass = graphics_device.createRenderPass (renderpass_create_info);
	}

	void create_framebuffers ()
	{
		vk::FramebufferCreateInfo framebuffer_create_info (
			{},
			render_pass,
			1,
			{},
			common_graphics_obj_ptr->surface_extent.width,
			common_graphics_obj_ptr->surface_extent.height,
			1
		);

		for (auto swapchain_imageview : common_graphics_obj_ptr->swapchain_imageviews)
		{
			framebuffer_create_info.pAttachments = &swapchain_imageview;
			swapchain_framebuffers.push_back (graphics_device.createFramebuffer (framebuffer_create_info));
		}
	}

	void create_shaders ()
	{
		pipeline_shader_stage_create_infos.resize (2);

		graphics_utils::create_shader (
			utils::get_full_path ("\\Shaders\\SplashScreen\\vert.spv"),
			vk::ShaderStageFlagBits::eVertex,
			vertex_shader_module,
			pipeline_shader_stage_create_infos[0]
		);
		graphics_utils::create_shader (
			utils::get_full_path ("\\Shaders\\SplashScreen\\frag.spv"),
			vk::ShaderStageFlagBits::eFragment,
			fragment_shader_module,
			pipeline_shader_stage_create_infos[1]
		);
	}

	void create_graphics_pipeline_layout ()
	{
		vk::PipelineLayoutCreateInfo pipeline_layout_create_info (
			{},
			descriptor_set_layouts.size (),
			descriptor_set_layouts.data ()
		);
		graphics_pipeline_layout = graphics_device.createPipelineLayout (pipeline_layout_create_info);
	}

	void create_graphics_pipeline ()
	{
		vk::VertexInputBindingDescription vertex_input_binding_description[2] = {
			{0, sizeof (float) * 3, vk::VertexInputRate::eVertex},
			{1, sizeof (float) * 2, vk::VertexInputRate::eVertex}
		};
		vk::VertexInputAttributeDescription vertex_pos_input_attribute_description[2] = {
			{0, 0, vk::Format::eR32G32B32Sfloat, 0},
			{1, 1, vk::Format::eR32G32Sfloat, 0}
		};
		vk::PipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info (
			{},
			2,
			vertex_input_binding_description,
			2,
			vertex_pos_input_attribute_description
		);
		vk::PipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info (
			{},
			vk::PrimitiveTopology::eTriangleList,
			0
		);
		vk::PipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info (
			{},
			0,
			0,
			vk::PolygonMode::eFill,
			vk::CullModeFlagBits::eBack,
			vk::FrontFace::eCounterClockwise,
			{},
			{},
			{},
			{},
			1
		);
		vk::PipelineColorBlendAttachmentState pipeline_color_blend_attachment_state (
			{}, {}, {}, {}, {}, {}, {},
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
		);
		vk::PipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info (
			{},
			{},
			{},
			1,
			&pipeline_color_blend_attachment_state,
			{ 1,1,1,1 }
		);

		vk::Viewport viewport (0, 0, (float)common_graphics_obj_ptr->surface_extent.width, (float)common_graphics_obj_ptr->surface_extent.height, 0, 1);
		vk::Rect2D scissor ({}, common_graphics_obj_ptr->surface_extent);
		vk::PipelineViewportStateCreateInfo pipeline_viewport_state_create_info (
			{},
			1,
			&viewport,
			1,
			&scissor
		);

		vk::PipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info ({}, vk::SampleCountFlagBits::e1);

		vk::GraphicsPipelineCreateInfo graphics_pipeline_create_info (
			{},
			pipeline_shader_stage_create_infos.size (),
			pipeline_shader_stage_create_infos.data (),
			&pipeline_vertex_input_state_create_info,
			&pipeline_input_assembly_state_create_info,
			{},
			&pipeline_viewport_state_create_info,
			&pipeline_rasterization_state_create_info,
			&pipeline_multisample_state_create_info,
			{},
			&pipeline_color_blend_state_create_info,
			{},
			graphics_pipeline_layout,
			render_pass,
			{},
			{},
			{}
		);

		graphics_pipeline = graphics_device.createGraphicsPipeline ({}, graphics_pipeline_create_info);
	}

	void create_sync_objects ()
	{
		vk::SemaphoreCreateInfo semaphore_create_info;
		signal_semaphore = graphics_device.createSemaphore (semaphore_create_info);
		wait_semaphore = graphics_device.createSemaphore (semaphore_create_info);

		swapchain_fences.resize (common_graphics_obj_ptr->swapchain_images.size ());
		vk::FenceCreateInfo fence_create_info (vk::FenceCreateFlagBits::eSignaled);

		for (auto& swapchain_fence : swapchain_fences)
		{
			swapchain_fence = graphics_device.createFence (fence_create_info);
		}
	}

	void allocate_command_buffers ()
	{
		vk::CommandBufferAllocateInfo command_buffer_allocate_info (
			common_graphics_obj_ptr->command_pool, vk::CommandBufferLevel::ePrimary, common_graphics_obj_ptr->swapchain_images.size ()
		);

		swapchain_command_buffers = graphics_device.allocateCommandBuffers (command_buffer_allocate_info);
	}

	void update_command_buffers (
		const std::vector<asset::mesh::mesh>& meshes
	)
	{
		vk::CommandBufferBeginInfo command_buffer_begin_info (vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		vk::ClearValue clear_value (std::array<float, 4> {0, 0, 0, 1});
		vk::RenderPassBeginInfo render_pass_begin_info (render_pass, {}, vk::Rect2D ({}, common_graphics_obj_ptr->surface_extent), 1, &clear_value);

		uint32_t counter = 0;
		for (auto& swapchain_command_buffer : swapchain_command_buffers)
		{
			render_pass_begin_info.framebuffer = swapchain_framebuffers.at (counter);
			swapchain_command_buffer.begin (command_buffer_begin_info);
			swapchain_command_buffer.beginRenderPass (render_pass_begin_info, vk::SubpassContents::eInline);

			swapchain_command_buffer.endRenderPass ();
			swapchain_command_buffer.end ();

			++counter;
		}
	}

	void init (
		std::vector<asset::mesh::mesh>& meshes,
		std::vector<asset::staging_image::staging_image>& images,
		common_graphics::common_graphics* ptr
	)
	{
		OutputDebugString (L"splash_screen_graphics::init\n");
		common_graphics_obj_ptr = ptr;
		graphics_device = common_graphics_obj_ptr->graphics_device;

		create_vulkan_handles_for_meshes (meshes);
		create_vulkan_handles_for_images (images, meshes);

		create_descriptor_sets (meshes);
		create_renderpasses ();
		create_framebuffers ();
		create_shaders ();
		create_graphics_pipeline_layout ();
		create_graphics_pipeline ();
		create_sync_objects ();
		allocate_command_buffers ();
		update_command_buffers (meshes);
	}

	void draw ()
	{
		vk::ResultValue<uint32_t> swapchain_image_index = graphics_device.acquireNextImageKHR (common_graphics_obj_ptr->swapchain, UINT64_MAX, wait_semaphore, nullptr);
		if (swapchain_image_index.result == vk::Result::eSuboptimalKHR || swapchain_image_index.result == vk::Result::eErrorOutOfDateKHR)
		{
			return;
		}

		graphics_device.waitForFences (1, &swapchain_fences.at (swapchain_image_index.value), VK_TRUE, UINT64_MAX);
		
		vk::PipelineStageFlags wait_dst_stage_mask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		vk::SubmitInfo submit_info (
			1,
			&wait_semaphore,
			&wait_dst_stage_mask,
			1,
			&swapchain_command_buffers.at (swapchain_image_index.value),
			1,
			&signal_semaphore
		);

		graphics_device.resetFences (1, &swapchain_fences.at (swapchain_image_index.value));
		common_graphics_obj_ptr->graphics_queue.submit (1, &submit_info, swapchain_fences.at (swapchain_image_index.value));
		vk::PresentInfoKHR present_info (1, &signal_semaphore, 1, &common_graphics_obj_ptr->swapchain, &swapchain_image_index.value);
		vk::Result result = common_graphics_obj_ptr->graphics_queue.presentKHR (present_info);
		if (result == vk::Result::eSuboptimalKHR || result == vk::Result::eErrorOutOfDateKHR)
		{
			return;
		}
	}

	void exit ()
	{
		OutputDebugString (L"splash_screen_graphics::exit\n");

		graphics_device.waitForFences (swapchain_fences.size (), swapchain_fences.data (), VK_TRUE, UINT64_MAX);

		graphics_device.freeCommandBuffers (common_graphics_obj_ptr->command_pool, swapchain_command_buffers);
		swapchain_command_buffers.clear ();

		for (auto& swapchain_fence : swapchain_fences)
		{
			if (swapchain_fence != VK_NULL_HANDLE)
			{
				graphics_device.destroyFence (swapchain_fence);
			}
		}

		swapchain_fences.clear ();

		if (wait_semaphore != VK_NULL_HANDLE)
		{
			graphics_device.destroySemaphore (wait_semaphore);
		}

		if (signal_semaphore != VK_NULL_HANDLE)
		{
			graphics_device.destroySemaphore (signal_semaphore);
		}

		for (auto& swapchain_framebuffer : swapchain_framebuffers)
		{
			if (swapchain_framebuffer != VK_NULL_HANDLE)
			{
				graphics_device.destroyFramebuffer (swapchain_framebuffer);
			}
		}
		swapchain_framebuffers.clear ();

		if (vertex_shader_module != VK_NULL_HANDLE)
		{
			graphics_device.destroyShaderModule (vertex_shader_module);
		}

		if (fragment_shader_module != VK_NULL_HANDLE)
		{
			graphics_device.destroyShaderModule (fragment_shader_module);
		}

		if (render_pass != VK_NULL_HANDLE)
		{
			graphics_device.destroyRenderPass (render_pass);
		}

		for (auto& descriptor_set : descriptor_sets)
		{
			if (descriptor_set != VK_NULL_HANDLE)
			{
				graphics_device.freeDescriptorSets (descriptor_pool, descriptor_sets);
			}
		}
		descriptor_sets.clear ();

		for (auto& descriptor_set_layout : descriptor_set_layouts)
		{
			if (descriptor_set_layout != VK_NULL_HANDLE)
			{
				graphics_device.destroyDescriptorSetLayout (descriptor_set_layout);
			}
		}
		descriptor_set_layouts.clear ();

		if (graphics_pipeline_layout != VK_NULL_HANDLE)
		{
			graphics_device.destroyPipelineLayout (graphics_pipeline_layout);
		}

		if (graphics_pipeline != VK_NULL_HANDLE)
		{
			graphics_device.destroyPipeline (graphics_pipeline);
		}

		if (descriptor_pool != VK_NULL_HANDLE)
		{
			graphics_device.destroyDescriptorPool (descriptor_pool);
		}

		if (vertex_index_buffer != VK_NULL_HANDLE)
		{
			graphics_device.destroyBuffer (vertex_index_buffer);
		}

		if (vertex_index_buffer_memory != VK_NULL_HANDLE)
		{
			graphics_device.freeMemory (vertex_index_buffer_memory);
		}

		for (auto& image_view : image_views)
		{
			if (image_view != VK_NULL_HANDLE)
			{
				graphics_device.destroyImageView (image_view);
			}
		}
		image_views.clear ();

		for (auto& staging_image : splash_screen_graphics_images)
		{
			if (staging_image != VK_NULL_HANDLE)
			{
				graphics_device.destroyImage (staging_image);
			}
		}
		splash_screen_graphics_images.clear ();

		if (image_memory != VK_NULL_HANDLE)
		{
			graphics_device.freeMemory (image_memory);
		}
	}
}*/

egraphics_result splash_screen_graphics::create_shaders ()
{
	return egraphics_result::success;
}

egraphics_result splash_screen_graphics::create_descriptor_sets (std::vector<asset::mesh>& meshes)
{
	return egraphics_result::success;
}

egraphics_result splash_screen_graphics::create_graphics_pipeline_layout ()
{
	VkPipelineLayoutCreateInfo create_info = { 0 };
	create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	if (vkCreatePipelineLayout (common_graphics::graphics_device, &create_info, nullptr, &graphics_pipeline_layout) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_graphics_pipeline_layout;
	}

	return egraphics_result::success;
}

egraphics_result splash_screen_graphics::create_graphics_pipeline ()
{
	return egraphics_result::success;
}

egraphics_result splash_screen_graphics::update_command_buffers (const std::vector<asset::mesh>& meshes)
{
	VkCommandBufferBeginInfo command_buffer_begin_info = { 0 };
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VkClearValue clear_value = { 0 };
	clear_value.color = { 1,1,0,1 };

	VkRenderPassBeginInfo render_pass_begin_info = { 0 };
	render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_begin_info.renderPass = render_pass;
	render_pass_begin_info.renderArea = VkRect2D{ {}, common_graphics::surface_extent };
	render_pass_begin_info.clearValueCount = 1;
	render_pass_begin_info.pClearValues = &clear_value;

	for (uint32_t i = 0; i < common_graphics::swapchain_image_count; i++)
	{
		render_pass_begin_info.framebuffer = swapchain_framebuffers[i];
		if (vkBeginCommandBuffer (common_graphics::swapchain_command_buffers[i], &command_buffer_begin_info) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_begin_command_buffer;
		}

		vkCmdBeginRenderPass (common_graphics::swapchain_command_buffers[i], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdEndRenderPass (common_graphics::swapchain_command_buffers[i]);

		if (vkEndCommandBuffer (common_graphics::swapchain_command_buffers[i]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_end_command_buffer;
		}
	}

	return egraphics_result::success;
}

splash_screen_graphics::splash_screen_graphics ()
{
	OutputDebugString (L"splash_screen_graphics::splash_screen_graphics\n");
	
	render_pass = VK_NULL_HANDLE;
	graphics_pipeline_layout = VK_NULL_HANDLE;
	graphics_pipeline = VK_NULL_HANDLE;
	vertex_shader_module = VK_NULL_HANDLE;
	fragment_shader_module = VK_NULL_HANDLE;
	descriptor_pool = VK_NULL_HANDLE;

	vertex_index_buffer = VK_NULL_HANDLE;
	vertex_index_memory = VK_NULL_HANDLE;

	staging_vertex_index_buffer = VK_NULL_HANDLE;
	staging_vertex_index_memory = VK_NULL_HANDLE;

	scene_images_memory = VK_NULL_HANDLE;
	staging_image_buffer_memory = VK_NULL_HANDLE;
}

egraphics_result splash_screen_graphics::init (std::vector<asset::mesh>& meshes)
{
	OutputDebugString (L"splash_screen_graphics::init\n");

	vkDestroyBuffer (common_graphics::graphics_device, VK_NULL_HANDLE, nullptr);

	CHECK_AGAINST_RESULT (
		graphics_utils::create_vulkan_handles_for_meshes (
			meshes,
			&staging_vertex_index_buffer,
			&staging_image_buffer_memory,
			&vertex_index_buffer,
			&vertex_index_memory,
			&staging_image_buffer,
			&staging_image_buffer_memory,
			scene_images,
			&scene_images_memory
		)
	);

	CHECK_AGAINST_RESULT (create_basic_renderpass (&render_pass));
	CHECK_AGAINST_RESULT (create_basic_framebuffers (swapchain_framebuffers, &render_pass));
	CHECK_AGAINST_RESULT (create_shaders ());
	CHECK_AGAINST_RESULT (create_descriptor_sets (meshes));
	CHECK_AGAINST_RESULT (create_graphics_pipeline_layout ());
	CHECK_AGAINST_RESULT (create_graphics_pipeline ());
	CHECK_AGAINST_RESULT (update_command_buffers (meshes));

	swapchain_framebuffers.shrink_to_fit ();
	//swapchain_fences.shrink_to_fit ();
	common_graphics::swapchain_command_buffers.shrink_to_fit ();

	return egraphics_result::success;
}

egraphics_result splash_screen_graphics::draw (const std::vector<asset::mesh>& meshes)
{
	uint32_t image_index;
	VkResult result = vkAcquireNextImageKHR (
		common_graphics::graphics_device,
		common_graphics::swapchain,
		UINT64_MAX,
		common_graphics::swapchain_wait_semaphore,
		VK_NULL_HANDLE,
		&image_index
	);

	if (result != VK_SUCCESS)
	{
		if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			return egraphics_result::success;
		}
		else
		{
			return egraphics_result::e_against_error_graphics_acquire_next_image;
		}
	}

	VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submit_info = { 0 };
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &common_graphics::swapchain_command_buffers[image_index];
	submit_info.pWaitDstStageMask = &wait_dst_stage_mask;
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &common_graphics::swapchain_signal_semaphores[image_index];
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &common_graphics::swapchain_wait_semaphore;
	
	if (vkQueueSubmit (common_graphics::graphics_queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_queue_submit;
	}

	VkPresentInfoKHR present_info = { 0 };
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &common_graphics::swapchain;
	present_info.pImageIndices = &image_index;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &common_graphics::swapchain_signal_semaphores[image_index];

	if (vkQueuePresentKHR (common_graphics::graphics_queue, &present_info) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_queue_present;
	}

	return egraphics_result::success;
}

void splash_screen_graphics::exit (std::vector<asset::mesh>& meshes)
{
	OutputDebugString (L"splash_screen_graphics::exit\n");

	vkQueueWaitIdle (common_graphics::graphics_queue);

	pipeline_shader_stage_create_infos.clear ();

	vkDestroyShaderModule (common_graphics::graphics_device, vertex_shader_module, nullptr);
	vertex_shader_module = VK_NULL_HANDLE;
	vkDestroyShaderModule (common_graphics::graphics_device, fragment_shader_module, nullptr);
	fragment_shader_module = VK_NULL_HANDLE;

	vkDestroyRenderPass (common_graphics::graphics_device, render_pass, nullptr);
	render_pass = VK_NULL_HANDLE;

	for (auto& swapchain_framebuffer : swapchain_framebuffers)
	{
		vkDestroyFramebuffer (common_graphics::graphics_device, swapchain_framebuffer, nullptr);
		swapchain_framebuffer = VK_NULL_HANDLE;
	}
	swapchain_framebuffers.clear ();

	vkDestroyPipelineLayout (common_graphics::graphics_device, graphics_pipeline_layout, nullptr);
	vkDestroyPipeline (common_graphics::graphics_device, graphics_pipeline, nullptr);

	/*for (auto& swapchain_fence : swapchain_fences)
	{
		vkDestroyFence (common_graphics::graphics_device, swapchain_fence, nullptr);
		swapchain_fence = VK_NULL_HANDLE;
	}
	swapchain_fences.clear ();

	vkDestroyFence (common_graphics::graphics_device, acquire_next_image_fence, nullptr);
	acquire_next_image_fence = VK_NULL_HANDLE;*/

	for (auto& mesh : meshes)
	{
		for (auto& graphics_primitive : mesh.graphics_primitves)
		{
			vkDestroyImageView (common_graphics::graphics_device, graphics_primitive.material.base_color_texture.texture_image.image_view, nullptr);
			graphics_primitive.material.base_color_texture.texture_image.image_view = VK_NULL_HANDLE;
			
			if (graphics_primitive.material.base_color_texture.texture_image.descriptor_set != VK_NULL_HANDLE)
			{
				vkFreeDescriptorSets (common_graphics::graphics_device, descriptor_pool, 1, &graphics_primitive.material.base_color_texture.texture_image.descriptor_set);
				graphics_primitive.material.base_color_texture.texture_image.descriptor_set = VK_NULL_HANDLE;
			}
		}
	}

	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &staging_vertex_index_buffer, 1, &staging_vertex_index_memory);
	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &staging_image_buffer, 1, &staging_image_buffer_memory);
		
	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &vertex_index_buffer, 1, &vertex_index_memory);
	graphics_utils::destroy_images_and_image_memory (common_graphics::graphics_device, scene_images.data (), scene_images.size (), &scene_images_memory);
	scene_images.clear ();
}

splash_screen_graphics::~splash_screen_graphics ()
{
	OutputDebugString (L"splash_screen_graphics::~splash_screen_graphics\n");
}
