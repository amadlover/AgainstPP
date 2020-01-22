#include "splash_screen_graphics.hpp"
#include "Error.hpp"
#include "Utils.hpp"
#include "graphics_utils.hpp"
#include "glm/vec2.hpp"

#include <map>

namespace splash_screen_graphics
{
	std::unique_ptr<splash_screen_graphics> splash_screen_graphics_obj_ptr (new splash_screen_graphics ());
	common_graphics::common_graphics* common_graphics_obj_ptr;
	vk::Device graphics_device;
	vk::RenderPass renderpass;
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

	void check_for_similar_image_indices (
		uint32_t from_index, 
		const asset::image::image& current_image, 
		const std::vector<asset::image::image>& images, 
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

	void create_vulkan_handles_for_images (const std::vector<asset::image::image>& images, std::vector<asset::mesh::mesh>& meshes)
	{
		std::map<vk::Image, std::vector<uint32_t>> similar_indices;

		for (uint32_t index = 0; index < images.size(); index++)
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

			asset::image::image image = images.at (index);
			std::vector<uint32_t> similar_images_indices;
			similar_images_indices.reserve (images.size ());
			similar_images_indices.push_back (index);
			
			uint32_t start_index = index + 1;
			check_for_similar_image_indices (
				start_index,
				image,
				images,
				similar_images_indices
			);

			vk::DeviceSize similar_image_size = 0;

			for (const auto& similar_index : similar_images_indices)
			{
				const asset::image::image similar_image = images.at (similar_index);
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

		splash_screen_graphics_obj_ptr->image_memory = graphics_utils::allocate_bind_image_memory (new_images, vk::MemoryPropertyFlagBits::eDeviceLocal);
		splash_screen_graphics_obj_ptr->images = new_images;

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

				splash_screen_graphics_obj_ptr->image_views.push_back (image_view);
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
		graphics_utils::map_data_to_buffer (device_memory, offset, data.size (), data.data ());
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

		vk::Buffer staging_buffer = graphics_utils::create_buffer (total_size, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive);
		vk::DeviceMemory staging_buffer_memory = graphics_utils::allocate_bind_buffer_memory (staging_buffer, vk::MemoryPropertyFlagBits::eHostVisible);

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

		splash_screen_graphics_obj_ptr->vertex_index_buffer = graphics_utils::create_buffer (
			total_size, 
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer, 
			vk::SharingMode::eExclusive
		);

		splash_screen_graphics_obj_ptr->vertex_index_buffer_memory = graphics_utils::allocate_bind_buffer_memory (
			splash_screen_graphics_obj_ptr->vertex_index_buffer, 
			vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		graphics_utils::copy_buffer_to_buffer (
			common_graphics_obj_ptr->command_pool, 
			staging_buffer, 
			splash_screen_graphics_obj_ptr->vertex_index_buffer, 
			total_size
		);

		for (auto& mesh : meshes)
		{
			for (auto& graphics_primitive : mesh.graphics_primitves)
			{
				graphics_primitive.buffer = &splash_screen_graphics_obj_ptr->vertex_index_buffer;
				graphics_primitive.buffer_memory = &splash_screen_graphics_obj_ptr->vertex_index_buffer_memory;
			}
		}

		graphics_device.destroyBuffer (staging_buffer);
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

		renderpass = graphics_device.createRenderPass (renderpass_create_info);
	}

	void create_framebuffers ()
	{
		vk::FramebufferCreateInfo framebuffer_create_info (
			{},
			renderpass,
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
			renderpass,
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

	void init (
		std::vector<asset::mesh::mesh>& meshes, 
		std::vector<asset::image::image>& images, 
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
	}

	void create_command_buffers ()
	{
	}

	void draw ()
	{
	}

	void exit ()
	{
		OutputDebugString (L"splash_screen_graphics::exit\n");

		graphics_device.waitForFences (swapchain_fences.size (), swapchain_fences.data (), VK_TRUE, UINT64_MAX);

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

		if (renderpass != VK_NULL_HANDLE)
		{
			graphics_device.destroyRenderPass (renderpass);
		}

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

		if (splash_screen_graphics_obj_ptr->vertex_index_buffer != VK_NULL_HANDLE)
		{
			graphics_device.destroyBuffer (splash_screen_graphics_obj_ptr->vertex_index_buffer);
		}

		if (splash_screen_graphics_obj_ptr->vertex_index_buffer_memory != VK_NULL_HANDLE)
		{
			graphics_device.freeMemory (splash_screen_graphics_obj_ptr->vertex_index_buffer_memory);
		}

		for (auto& image_view : splash_screen_graphics_obj_ptr->image_views)
		{
			if (image_view != VK_NULL_HANDLE)
			{
				graphics_device.destroyImageView (image_view);
			}
		}
		splash_screen_graphics_obj_ptr->image_views.clear ();

		for (auto& image : splash_screen_graphics_obj_ptr->images)
		{
			if (image != VK_NULL_HANDLE)
			{
				graphics_device.destroyImage (image);
			}
		}
		splash_screen_graphics_obj_ptr->images.clear ();

		if (splash_screen_graphics_obj_ptr->image_memory != VK_NULL_HANDLE)
		{
			graphics_device.freeMemory (splash_screen_graphics_obj_ptr->image_memory);
		}
	}
}