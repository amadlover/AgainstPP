#include "splash_screen_graphics.hpp"
#include "Error.hpp"
#include "Utils.hpp"
#include "graphics_utils.hpp"
#include "common_graphics.hpp"

#include <vector>

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
	return egraphics_result::success;
}

egraphics_result splash_screen_graphics::create_graphics_pipeline ()
{
	return egraphics_result::success;
}

splash_screen_graphics::splash_screen_graphics ()
{
	OutputDebugString (L"splash_screen_graphics::splash_screen_graphics\n");

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

	CHECK_AGAINST_RESULT (
		graphics_utils::create_vulkan_handles_for_meshes (
			meshes,
			command_pool
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

	CHECK_AGAINST_RESULT (create_renderpass ());
	CHECK_AGAINST_RESULT (create_framebuffers ());
	CHECK_AGAINST_RESULT (create_shaders ());
	CHECK_AGAINST_RESULT (create_descriptor_sets (meshes));
	CHECK_AGAINST_RESULT (create_graphics_pipeline_layout ());
	CHECK_AGAINST_RESULT (create_graphics_pipeline ());
	CHECK_AGAINST_RESULT (update_command_buffers (meshes));

	swapchain_framebuffers.shrink_to_fit ();
	common_graphics::swapchain_command_buffers.shrink_to_fit ();

	return egraphics_result::success;
}

egraphics_result splash_screen_graphics::draw (const std::vector<asset::mesh>& meshes) const
{

	return egraphics_result::success;
}

void splash_screen_graphics::exit (std::vector<asset::mesh>& meshes)
{
	OutputDebugString (L"splash_screen_graphics::exit\n");

	vkQueueWaitIdle (common_graphics::graphics_queue);

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
