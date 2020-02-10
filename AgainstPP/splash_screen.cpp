#include "splash_screen.hpp"
#include "utils.hpp"
#include "enums.hpp"

splash_screen::splash_screen () : scene ()
{
	OutputDebugString (L"splash_screen::splash_screen\n");
}

splash_screen::~splash_screen ()
{
	OutputDebugString (L"splash_screen::~splash_screen\n");
}

egraphics_result splash_screen::create_render_pass ()
{
	VkAttachmentDescription attachment_description = { 0 };

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

	VkSubpassDescription subpass_description = { 0 };

	subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.inputAttachmentCount = 0;
	subpass_description.preserveAttachmentCount = 0;
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &color_reference;

	VkRenderPassCreateInfo create_info = { 0 };

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

egraphics_result splash_screen::create_framebuffers ()
{
	VkFramebufferCreateInfo create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	create_info.renderPass = render_pass;
	create_info.attachmentCount = 1;
	create_info.width = common_graphics::surface_extent.width;
	create_info.height = common_graphics::surface_extent.height;
	create_info.layers = 1;

	swapchain_framebuffers.resize (common_graphics::swapchain_image_count);

	VkImageView attachment;
	for (uint32_t i = 0; i < common_graphics::swapchain_image_count; i++)
	{
		attachment = common_graphics::swapchain_imageviews[i];
		create_info.pAttachments = &attachment;

		if (vkCreateFramebuffer (common_graphics::graphics_device, &create_info, NULL, &swapchain_framebuffers[i]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_create_framebuffers;
		}
	}

	return egraphics_result::success;
}

egraphics_result splash_screen::create_command_pool ()
{
	VkCommandPoolCreateInfo command_pool_create_info = { 0 };
	command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.queueFamilyIndex = common_graphics::graphics_queue_family_index;
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool (common_graphics::graphics_device, &command_pool_create_info, nullptr, &command_pool) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_command_pool;
	}

	return egraphics_result::success;
}

egraphics_result splash_screen::create_sync_objects ()
{
	VkSemaphoreCreateInfo create_info = { 0 };
	create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	swapchain_signal_semaphores.resize (common_graphics::swapchain_image_count);
	for (uint32_t i = 0; i < common_graphics::swapchain_image_count; i++)
	{
		if (vkCreateSemaphore (common_graphics::graphics_device, &create_info, nullptr, &swapchain_signal_semaphores[i]) != VK_SUCCESS)
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

egraphics_result splash_screen::allocate_command_buffers ()
{
	VkCommandBufferAllocateInfo allocate_info = { 0 };
	allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocate_info.commandPool = command_pool;
	allocate_info.commandBufferCount = common_graphics::swapchain_image_count;

	swapchain_command_buffers.resize (common_graphics::swapchain_image_count);

	if (vkAllocateCommandBuffers (common_graphics::graphics_device, &allocate_info, swapchain_command_buffers.data ()) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_allocate_command_buffer;
	}

	return egraphics_result::success;
}


egraphics_result splash_screen::update_command_buffers ()
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
		if (vkBeginCommandBuffer (swapchain_command_buffers[i], &command_buffer_begin_info) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_begin_command_buffer;
		}

		vkCmdBeginRenderPass (swapchain_command_buffers[i], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdEndRenderPass (swapchain_command_buffers[i]);

		if (vkEndCommandBuffer (swapchain_command_buffers[i]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_end_command_buffer;
		}
	}

	return egraphics_result::success;
}

egraphics_result splash_screen::init ()
{
	OutputDebugString (L"splash_screen::init\n");

	CHECK_AGAINST_RESULT (asset::import_meshes (utils::get_full_path ("\\UIElements\\SplashScreen\\SplashScreen.gltf"), meshes));

	CHECK_AGAINST_RESULT (create_render_pass ());
	CHECK_AGAINST_RESULT (create_framebuffers ());
	CHECK_AGAINST_RESULT (create_command_pool ());
	CHECK_AGAINST_RESULT (allocate_command_buffers ());
	CHECK_AGAINST_RESULT (create_sync_objects ());
	CHECK_AGAINST_RESULT (update_command_buffers ());

	state = e_scene_state::inited;

	return egraphics_result::success;
}

void splash_screen::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	OutputDebugString (L"splash_screen::process_keyboard_input\n");
	switch (wParam)
	{
	case VK_ESCAPE:
		go_to_scene_event.broadcast (e_scene_type::main_menu);
		break;

	case 0x53:
		break;

	default:
		break;
	}
}

egraphics_result splash_screen::update ()
{
	return egraphics_result::success;
}

egraphics_result splash_screen::draw () const
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
	VkSubmitInfo submit_info = { 0 };
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &swapchain_command_buffers[image_index];
	submit_info.pWaitDstStageMask = &wait_dst_stage_mask;
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &swapchain_signal_semaphores[image_index];
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &wait_semaphore;

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
	present_info.pWaitSemaphores = &swapchain_signal_semaphores[image_index];

	if (vkQueuePresentKHR (common_graphics::graphics_queue, &present_info) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_queue_present;
	}

	return egraphics_result::success;
}

egraphics_result splash_screen::main_loop ()
{
	CHECK_AGAINST_RESULT (update ());
	CHECK_AGAINST_RESULT (draw ());

	return egraphics_result::success;
}

void splash_screen::exit ()
{
	OutputDebugString (L"splash_screen::exit\n");

	vkQueueWaitIdle (common_graphics::graphics_queue);

	for (auto& swapchain_signal_semaphore : swapchain_signal_semaphores)
	{
		vkDestroySemaphore (common_graphics::graphics_device, swapchain_signal_semaphore, nullptr);
	}
	swapchain_signal_semaphores.clear ();
	vkDestroySemaphore (common_graphics::graphics_device, wait_semaphore, nullptr);

	vkFreeCommandBuffers (common_graphics::graphics_device, command_pool, common_graphics::swapchain_image_count, swapchain_command_buffers.data ());
	vkDestroyCommandPool (common_graphics::graphics_device, command_pool, nullptr);

	vkDestroyRenderPass (common_graphics::graphics_device, render_pass, nullptr);
	render_pass = VK_NULL_HANDLE;

	for (auto& swapchain_framebuffer : swapchain_framebuffers)
	{
		vkDestroyFramebuffer (common_graphics::graphics_device, swapchain_framebuffer, nullptr);
		swapchain_framebuffer = VK_NULL_HANDLE;
	}
	swapchain_framebuffers.clear ();

	for (auto& mesh : meshes)
	{
		for (auto& graphics_primitive : mesh.graphics_primitves)
		{
			vkDestroyImageView (common_graphics::graphics_device, graphics_primitive.material.base_color_texture.texture_image.image_view, nullptr);
			graphics_primitive.material.base_color_texture.texture_image.image_view = VK_NULL_HANDLE;
		}
	}

	asset::destroy_meshes (meshes);
	state = e_scene_state::exited;
}