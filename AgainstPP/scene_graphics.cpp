#include "scene_graphics.hpp"
#include "common_graphics.hpp"

egraphics_result scene_graphics::create_basic_renderpass (VkRenderPass* render_pass)
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

	if (vkCreateRenderPass (common_graphics::graphics_device, &create_info, NULL, render_pass) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_render_pass;
	}

	return egraphics_result::success;
}

egraphics_result scene_graphics::create_basic_framebuffers (std::vector<VkFramebuffer>& swapchain_framebuffers, VkRenderPass* render_pass)
{
	VkFramebufferCreateInfo create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	create_info.renderPass = *render_pass;
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
