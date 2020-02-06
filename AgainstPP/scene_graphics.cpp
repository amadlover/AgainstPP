#include "scene_graphics.hpp"
#include "common_graphics.hpp"

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_win32.h"

scene_graphics::scene_graphics ()
{
	render_pass = VK_NULL_HANDLE;
	graphics_pipeline_layout = VK_NULL_HANDLE;
	graphics_pipeline = VK_NULL_HANDLE;
	vertex_shader_module = VK_NULL_HANDLE;
	fragment_shader_module = VK_NULL_HANDLE;
	descriptor_pool = VK_NULL_HANDLE;
}

scene_graphics::~scene_graphics ()
{
}

egraphics_result scene_graphics::create_renderpass ()
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

egraphics_result scene_graphics::create_framebuffers ()
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

egraphics_result scene_graphics::create_shaders ()
{
	return egraphics_result::success;
}

egraphics_result scene_graphics::create_descriptor_sets (std::vector<asset::mesh>& meshes)
{
	return egraphics_result::success;
}

egraphics_result scene_graphics::create_graphics_pipeline_layout ()
{
	return egraphics_result::success;
}

egraphics_result scene_graphics::create_graphics_pipeline ()
{
	return egraphics_result::success;
}

egraphics_result scene_graphics::update_command_buffers (const std::vector<asset::mesh>& meshes)
{
	return egraphics_result::success;
}

egraphics_result scene_graphics::init (std::vector<asset::mesh>& meshes)
{
	ImGui_ImplVulkan_InitInfo imgui_init_info = { 0 };
	imgui_init_info.PhysicalDevice = common_graphics::physical_device;
	imgui_init_info.Device = common_graphics::graphics_device;
	imgui_init_info.Queue = common_graphics::graphics_queue;
	imgui_init_info.QueueFamily = common_graphics::graphics_queue_family_index;
	imgui_init_info.DescriptorPool = descriptor_pool;
	imgui_init_info.ImageCount = common_graphics::swapchain_image_count;
	imgui_init_info.Instance = common_graphics::instance;
	imgui_init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	return egraphics_result::success;
}

egraphics_result scene_graphics::draw (const std::vector<asset::mesh>& meshes) const
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

void scene_graphics::exit (std::vector<asset::mesh>& meshes)
{
	ImGui_ImplVulkan_Shutdown ();
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
}
