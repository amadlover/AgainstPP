#include "main_menu.hpp"
#include "graphics_utils.hpp"
#include "utils.hpp"

main_menu::main_menu () : scene ()
{
	OutputDebugString (L"main_menu::main_menu\n");

	render_pass = VK_NULL_HANDLE;
	command_pool = VK_NULL_HANDLE;
	descriptor_pool = VK_NULL_HANDLE;
	skybox_descriptor_set_layout = VK_NULL_HANDLE;
	skybox_descriptor_set = VK_NULL_HANDLE;
	skybox_vertex_shader_module = VK_NULL_HANDLE;
	skybox_fragment_shader_module = VK_NULL_HANDLE;

	staging_vertex_index_buffer = VK_NULL_HANDLE;
	staging_vertex_index_memory = VK_NULL_HANDLE;
	vertex_index_buffer = VK_NULL_HANDLE;
	vertex_index_memory = VK_NULL_HANDLE;

	staging_image_buffer = VK_NULL_HANDLE;
	staging_image_memory = VK_NULL_HANDLE;
	scene_images_memory = VK_NULL_HANDLE;

	uniform_buffer = VK_NULL_HANDLE;
	uniform_buffer_memory = VK_NULL_HANDLE;

	graphics_pipeline_layout = VK_NULL_HANDLE;
	graphics_pipeline = VK_NULL_HANDLE;

	wait_semaphore = VK_NULL_HANDLE;
}

main_menu::~main_menu ()
{
	OutputDebugString (L"main_menu::~main_menu\n");
}

egraphics_result main_menu::create_uniform_buffer ()
{
	CHECK_AGAINST_RESULT (
		graphics_utils::create_buffer (
			common_graphics::graphics_device,
			sizeof (glm::mat4),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			common_graphics::graphics_queue_family_index,
			&uniform_buffer
		)
	);

	CHECK_AGAINST_RESULT (
		graphics_utils::allocate_bind_buffer_memory (
			common_graphics::graphics_device,
			&uniform_buffer,
			1,
			common_graphics::physical_device_memory_properties,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&uniform_buffer_memory
		);
	);

	if (vkMapMemory (common_graphics::graphics_device, uniform_buffer_memory, 0, sizeof (glm::mat4), 0, &uniform_buffer_data_ptr) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_map_memory;
	}

	return egraphics_result::success;
}

egraphics_result main_menu::create_command_pool ()
{
	VkCommandPoolCreateInfo command_pool_create_info = {};
	command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.queueFamilyIndex = common_graphics::graphics_queue_family_index;
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool (common_graphics::graphics_device, &command_pool_create_info, nullptr, &command_pool) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_command_pool;
	}

	return egraphics_result::success;
}

egraphics_result main_menu::create_descriptor_set ()
{
	VkDescriptorPoolSize pool_sizes[2] = {};
	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[0].descriptorCount = 1;
	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo pool_create_info = {};
	pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_create_info.poolSizeCount = 2;
	pool_create_info.pPoolSizes = pool_sizes;
	pool_create_info.maxSets = 1;
	pool_create_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	if (vkCreateDescriptorPool (common_graphics::graphics_device, &pool_create_info, nullptr, &descriptor_pool) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_descriptor_pool;
	}

	VkDescriptorSetLayoutBinding set_layout_bindings[2] = {};
	set_layout_bindings[0].binding = 0;
	set_layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	set_layout_bindings[0].descriptorCount = 1;
	set_layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	set_layout_bindings[1].binding = 1;
	set_layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	set_layout_bindings[1].descriptorCount = 1;
	set_layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo set_layout_create_info = {};
	set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	set_layout_create_info.bindingCount = 2;
	set_layout_create_info.pBindings = set_layout_bindings;

	if (vkCreateDescriptorSetLayout (common_graphics::graphics_device, &set_layout_create_info, nullptr, &skybox_descriptor_set_layout) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_descriptor_set_layout;
	}

	VkDescriptorSetAllocateInfo set_allocate_info = {};
	set_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	set_allocate_info.descriptorPool = descriptor_pool;
	set_allocate_info.descriptorSetCount = 1;
	set_allocate_info.pSetLayouts = &skybox_descriptor_set_layout;

	if (vkAllocateDescriptorSets (common_graphics::graphics_device, &set_allocate_info, &skybox_descriptor_set) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_descriptor_set;
	}

	VkDescriptorBufferInfo uniform_buffer_info = {};
	uniform_buffer_info.buffer = uniform_buffer;
	uniform_buffer_info.offset = 0;
	uniform_buffer_info.range = VK_WHOLE_SIZE;

	VkDescriptorImageInfo image_info = {};
	image_info.sampler = common_graphics::common_sampler;
	image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	image_info.imageView = skybox_actor.get_image_view ();

	VkWriteDescriptorSet descriptor_writes[2] = {};
	descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_writes[0].descriptorCount = 1;
	descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptor_writes[0].dstSet = skybox_descriptor_set;
	descriptor_writes[0].dstBinding = 0;
	descriptor_writes[0].pBufferInfo = &uniform_buffer_info;
	descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_writes[1].descriptorCount = 1;
	descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptor_writes[1].dstSet = skybox_descriptor_set;
	descriptor_writes[1].dstBinding = 1;
	descriptor_writes[1].pImageInfo = &image_info;

	vkUpdateDescriptorSets (common_graphics::graphics_device, 2, descriptor_writes, 0, nullptr);

	return egraphics_result::success;
}

egraphics_result main_menu::create_graphics_pipeline ()
{
	CHECK_AGAINST_RESULT (
		graphics_utils::create_shader (
			utils::get_full_path ("\\Shaders\\MainMenu\\skybox.vert.spv").c_str (),
			common_graphics::graphics_device,
			VK_SHADER_STAGE_VERTEX_BIT,
			&skybox_vertex_shader_module,
			&shader_stage_create_infos[0]
		)
	);

	CHECK_AGAINST_RESULT (
		graphics_utils::create_shader (
			utils::get_full_path ("\\Shaders\\MainMenu\\skybox.frag.spv").c_str (),
			common_graphics::graphics_device,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			&skybox_fragment_shader_module,
			&shader_stage_create_infos[1]
		)
	);

	VkPipelineLayoutCreateInfo layout_create_info = {};
	layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layout_create_info.setLayoutCount = 1;
	layout_create_info.pSetLayouts = &skybox_descriptor_set_layout;

	if (vkCreatePipelineLayout (common_graphics::graphics_device, &layout_create_info, nullptr, &graphics_pipeline_layout) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_graphics_pipeline_layout;
	}

	VkPipelineColorBlendAttachmentState color_blend_state_attachment = {};
	color_blend_state_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo color_blend_state = {};
	color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blend_state.attachmentCount = 1;
	color_blend_state.pAttachments = &color_blend_state_attachment;

	VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {};
	depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

	VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
	input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkPipelineMultisampleStateCreateInfo multi_sample_state = {};
	multi_sample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multi_sample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineRasterizationStateCreateInfo rasterization_state = {};
	rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterization_state.lineWidth = 1;
	rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;

	VkVertexInputBindingDescription vertex_binding_descriptions[2] = {};
	vertex_binding_descriptions[0].binding = 0;
	vertex_binding_descriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertex_binding_descriptions[0].stride = sizeof (float) * 3;;

	vertex_binding_descriptions[1].binding = 1;
	vertex_binding_descriptions[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertex_binding_descriptions[1].stride = sizeof (float) * 2;;

	VkVertexInputAttributeDescription vertex_attribute_descriptions[2] = {};
	vertex_attribute_descriptions[0].binding = 0;
	vertex_attribute_descriptions[0].location = 0;
	vertex_attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;

	vertex_attribute_descriptions[1].binding = 1;
	vertex_attribute_descriptions[1].location = 1;
	vertex_attribute_descriptions[1].format = VK_FORMAT_R32G32_SFLOAT;

	VkPipelineVertexInputStateCreateInfo vertex_input_state = {};
	vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_state.pVertexAttributeDescriptions = vertex_attribute_descriptions;
	vertex_input_state.pVertexBindingDescriptions = vertex_binding_descriptions;
	vertex_input_state.vertexAttributeDescriptionCount = 2;
	vertex_input_state.vertexBindingDescriptionCount = 2;

	VkViewport viewport = {};
	viewport.y = (float)common_graphics::surface_extent.height;
	viewport.width = (float)common_graphics::surface_extent.width;
	viewport.height = -(float)common_graphics::surface_extent.height;
	viewport.maxDepth = 1;

	VkRect2D scissor = {};
	scissor.extent = common_graphics::surface_extent;

	VkPipelineViewportStateCreateInfo viewport_state = {};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = &viewport;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = &scissor;

	VkGraphicsPipelineCreateInfo pipeline_create_info = {};
	pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_create_info.layout = graphics_pipeline_layout;
	pipeline_create_info.pColorBlendState = &color_blend_state;
	pipeline_create_info.pDepthStencilState = &depth_stencil_state;
	pipeline_create_info.pDynamicState = nullptr;
	pipeline_create_info.pInputAssemblyState = &input_assembly_state;
	pipeline_create_info.pMultisampleState = &multi_sample_state;
	pipeline_create_info.pRasterizationState = &rasterization_state;
	pipeline_create_info.pTessellationState = nullptr;
	pipeline_create_info.stageCount = 2;
	pipeline_create_info.renderPass = render_pass;
	pipeline_create_info.pVertexInputState = &vertex_input_state;
	pipeline_create_info.pViewportState = &viewport_state;
	pipeline_create_info.pStages = shader_stage_create_infos;

	if (vkCreateGraphicsPipelines (common_graphics::graphics_device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &graphics_pipeline) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_graphics_pipeline;
	}

	return egraphics_result::success;
}

egraphics_result main_menu::create_render_pass ()
{
	VkAttachmentDescription attachment_description = {};

	attachment_description.format = common_graphics::chosen_surface_format.format;
	attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;

	VkAttachmentReference color_reference;
	color_reference.attachment = 0;
	color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass_description = {};

	subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.inputAttachmentCount = 0;
	subpass_description.preserveAttachmentCount = 0;
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &color_reference;

	VkRenderPassCreateInfo create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	create_info.subpassCount = 1;
	create_info.pSubpasses = &subpass_description;
	create_info.attachmentCount = 1;
	create_info.pAttachments = &attachment_description;

	if (vkCreateRenderPass (common_graphics::graphics_device, &create_info, NULL, &render_pass) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_render_pass;
	}

	return egraphics_result::success;
}

egraphics_result main_menu::create_framebuffers ()
{
	VkFramebufferCreateInfo create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	create_info.renderPass = render_pass;
	create_info.attachmentCount = 1;
	create_info.width = common_graphics::surface_extent.width;
	create_info.height = common_graphics::surface_extent.height;
	create_info.layers = 1;

	framebuffers.resize (common_graphics::swapchain_image_count);

	VkImageView attachment;
	for (uint32_t i = 0; i < common_graphics::swapchain_image_count; i++)
	{
		attachment = common_graphics::swapchain_imageviews[i];
		create_info.pAttachments = &attachment;

		if (vkCreateFramebuffer (common_graphics::graphics_device, &create_info, NULL, &framebuffers[i]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_create_framebuffers;
		}
	}

	return egraphics_result::success;
}

egraphics_result main_menu::create_sync_objects ()
{
	VkSemaphoreCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	signal_semaphores.resize (common_graphics::swapchain_image_count);

	for (uint32_t i = 0; i < signal_semaphores.size (); ++i)
	{
		if (vkCreateSemaphore (common_graphics::graphics_device, &create_info, nullptr, &signal_semaphores[i]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_create_semaphore;
		}
	}

	if (vkCreateSemaphore (common_graphics::graphics_device, &create_info, nullptr, &wait_semaphore) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_semaphore;
	}

	return egraphics_result::success;
}

egraphics_result main_menu::allocate_command_buffers ()
{
	VkCommandBufferAllocateInfo allocate_info = {};
	allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocate_info.commandBufferCount = common_graphics::swapchain_image_count;
	allocate_info.commandPool = command_pool;
	allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	command_buffers.resize (common_graphics::swapchain_image_count);

	if (vkAllocateCommandBuffers (common_graphics::graphics_device, &allocate_info, command_buffers.data ()) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_command_buffer;
	}

	return egraphics_result::success;
}

egraphics_result main_menu::update_command_buffers ()
{
	VkCommandBufferBeginInfo command_buffer_begin_info = {};
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VkClearValue clear_value = {};
	clear_value.color = { 1,1,0,1 };

	VkRenderPassBeginInfo render_pass_begin_info = {};
	render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_begin_info.renderPass = render_pass;
	render_pass_begin_info.renderArea = VkRect2D{ {}, common_graphics::surface_extent };
	render_pass_begin_info.clearValueCount = 1;
	render_pass_begin_info.pClearValues = &clear_value;

	for (uint32_t i = 0; i < common_graphics::swapchain_image_count; i++)
	{
		render_pass_begin_info.framebuffer = framebuffers[i];
		if (vkBeginCommandBuffer (command_buffers[i], &command_buffer_begin_info) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_begin_command_buffer;
		}

		vkCmdBeginRenderPass (command_buffers[i], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdEndRenderPass (command_buffers[i]);

		if (vkEndCommandBuffer (command_buffers[i]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_end_command_buffer;
		}
	}

	return egraphics_result::success;
}

egraphics_result main_menu::init ()
{
	OutputDebugString (L"main_menu::init\n");

	CHECK_AGAINST_RESULT (asset::import_meshes (utils::get_full_path ("\\Scenes\\MainMenu\\MainMenu.gltf"), meshes));
	CHECK_AGAINST_RESULT (create_command_pool ());
	CHECK_AGAINST_RESULT (
		graphics_utils::create_vulkan_handles_for_meshes (
			meshes,
			command_pool,
			&staging_vertex_index_buffer,
			&staging_vertex_index_memory,
			&vertex_index_buffer,
			&vertex_index_memory,
			&staging_image_buffer,
			&staging_image_memory,
			scene_images,
			&scene_images_memory
		)
	);

	for (auto& mesh : meshes)
	{
		if (mesh.name.find ("skybox") != std::string::npos)
		{
			skybox_actor = skybox (&mesh);
			break;
		}
	}

	CHECK_AGAINST_RESULT (create_uniform_buffer ());
	CHECK_AGAINST_RESULT (create_render_pass ());
	CHECK_AGAINST_RESULT (create_framebuffers ());
	CHECK_AGAINST_RESULT (create_descriptor_set ());
	CHECK_AGAINST_RESULT (create_graphics_pipeline ());
	CHECK_AGAINST_RESULT (create_sync_objects ());
	CHECK_AGAINST_RESULT (allocate_command_buffers ());
	CHECK_AGAINST_RESULT (update_command_buffers ());

	state = e_scene_state::inited;

	return egraphics_result::success;
}

void main_menu::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	OutputDebugString (L"main_menu::process_keyboard_input\n");
	switch (wParam)
	{
	case VK_ESCAPE:
		go_to_scene_event.broadcast (e_scene_type::splash_screen);
		break;

	case 0x53:
		break;

	default:
		break;
	}
}

egraphics_result main_menu::main_loop ()
{
	CHECK_AGAINST_RESULT (update ());
	CHECK_AGAINST_RESULT (draw ());
	
	return egraphics_result::success;
}

egraphics_result main_menu::update ()
{
	return egraphics_result::success;
}

egraphics_result main_menu::draw () const
{
	uint32_t image_index;
	VkResult result = vkAcquireNextImageKHR (
		common_graphics::graphics_device,
		common_graphics::swapchain,
		UINT64_MAX,
		wait_semaphore,
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
	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffers[image_index];
	submit_info.pWaitDstStageMask = &wait_dst_stage_mask;
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &signal_semaphores[image_index];
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &wait_semaphore;

	if (vkQueueSubmit (common_graphics::graphics_queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_queue_submit;
	}

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &common_graphics::swapchain;
	present_info.pImageIndices = &image_index;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &signal_semaphores[image_index];

	if (vkQueuePresentKHR (common_graphics::graphics_queue, &present_info) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_queue_present;
	}

	return egraphics_result::success;
}

void main_menu::exit ()
{
	OutputDebugString (L"main_menu::exit\n");

	vkQueueWaitIdle (common_graphics::graphics_queue);

	vkDestroyPipelineLayout (common_graphics::graphics_device, graphics_pipeline_layout, nullptr);
	graphics_pipeline_layout = VK_NULL_HANDLE;

	vkDestroyPipeline (common_graphics::graphics_device, graphics_pipeline, nullptr);
	graphics_pipeline = VK_NULL_HANDLE;

	vkDestroyRenderPass (common_graphics::graphics_device, render_pass, nullptr);
	render_pass = VK_NULL_HANDLE;

	for (auto& swapchain_framebuffer : framebuffers)
	{
		vkDestroyFramebuffer (common_graphics::graphics_device, swapchain_framebuffer, nullptr);
		swapchain_framebuffer = VK_NULL_HANDLE;
	}
	framebuffers.clear ();

	for (auto& signal_semaphore : signal_semaphores)
	{
		vkDestroySemaphore (common_graphics::graphics_device, signal_semaphore, nullptr);
		signal_semaphore = VK_NULL_HANDLE;
	}
	signal_semaphores.clear ();

	vkDestroyShaderModule (common_graphics::graphics_device, skybox_vertex_shader_module, nullptr);
	skybox_vertex_shader_module = VK_NULL_HANDLE;

	vkDestroyShaderModule (common_graphics::graphics_device, skybox_fragment_shader_module, nullptr);
	skybox_fragment_shader_module = VK_NULL_HANDLE;

	vkFreeCommandBuffers (common_graphics::graphics_device, command_pool, command_buffers.size (), command_buffers.data ());
	command_buffers.clear ();
	vkDestroyCommandPool (common_graphics::graphics_device, command_pool, nullptr);
	command_pool = VK_NULL_HANDLE;

	vkFreeDescriptorSets (common_graphics::graphics_device, descriptor_pool, 1, &skybox_descriptor_set);
	skybox_descriptor_set = VK_NULL_HANDLE;

	vkDestroyDescriptorSetLayout (common_graphics::graphics_device, skybox_descriptor_set_layout, nullptr);
	skybox_descriptor_set_layout = VK_NULL_HANDLE;

	vkDestroyDescriptorPool (common_graphics::graphics_device, descriptor_pool, nullptr);
	descriptor_pool = VK_NULL_HANDLE;

	for (auto& mesh : meshes)
	{
		for (auto& graphics_primitive : mesh.graphics_primitves)
		{
			vkDestroyImageView (common_graphics::graphics_device, graphics_primitive.material.base_color_texture.texture_image.image_view, nullptr);
			graphics_primitive.material.base_color_texture.texture_image.image_view = VK_NULL_HANDLE;
		}
	}

	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &staging_vertex_index_buffer, 1, &staging_vertex_index_memory);
	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &vertex_index_buffer, 1, &vertex_index_memory);
	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &staging_image_buffer, 1, &staging_image_memory);
	vkUnmapMemory (common_graphics::graphics_device, uniform_buffer_memory);
	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &uniform_buffer, 1, &uniform_buffer_memory);
	graphics_utils::destroy_images_and_image_memory (common_graphics::graphics_device, scene_images.data (), scene_images.size (), &scene_images_memory);

	meshes.clear ();
	scene_images.clear ();

	state = e_scene_state::exited;
}
