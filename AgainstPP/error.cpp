#include "error.hpp"
#include <windows.h>

void log_error (egraphics_result err)
{
	switch (err)
	{
	case egraphics_result::e_against_error_graphics_populate_instance_layers_and_extensions:
		OutputDebugString (L"graphics error: populating instance layers and extensions\n");
		break;

	case egraphics_result::e_against_error_graphics_create_instance:
		OutputDebugString (L"graphics error: create instance\n");
		break;

	case egraphics_result::e_against_error_graphics_setup_debug_utils_messenger:
		OutputDebugString (L"graphics error: setup debug utils messenger\n");
		break;

	case egraphics_result::e_against_error_graphics_create_debug_utils_messenger:
		OutputDebugString (L"graphics error: create debug utils messenger\n");
		break;

	case egraphics_result::e_against_error_graphics_destroy_debug_utils_messenger:
		OutputDebugString (L"graphics error: destroy debud utils messenger\n");
		break;

	case egraphics_result::e_against_error_graphics_create_surface:
		OutputDebugString (L"graphics error: create surface\n");
		break;

	case egraphics_result::e_against_error_graphics_get_physical_device:
		OutputDebugString (L"graphics error: get physical device\n");
		break;

	case egraphics_result::e_against_error_graphics_populate_device_layers_and_extensions:
		OutputDebugString (L"graphics error: populate device layers and extensions\n");
		break;

	case egraphics_result::e_against_error_graphics_create_graphics_device:
		OutputDebugString (L"graphics error: create basegraphics device\n");
		break;

	case egraphics_result::e_against_error_graphics_surface_support:
		OutputDebugString (L"graphics error: surface support\n");
		break;

	case egraphics_result::e_against_error_graphics_create_swapchain:
		OutputDebugString (L"graphics error: create swapchain\n");
		break;

	case egraphics_result::e_against_error_graphics_create_image_view:
		OutputDebugString (L"graphics error: create image view\n");
		break;

	case egraphics_result::e_against_error_graphics_create_buffer:
		OutputDebugString (L"graphics error: create buffer\n");
		break;

	case egraphics_result::e_against_error_graphics_allocate_memory:
		OutputDebugString (L"graphics error: allocate buffer memory\n");
		break;

	case egraphics_result::e_against_error_graphics_bind_buffer_memory:
		OutputDebugString (L"graphics error: bind buffer memory\n");
		break;

	case egraphics_result::e_against_error_graphics_create_image:
		OutputDebugString (L"graphics error: create image\n");
		break;

	case egraphics_result::e_against_error_graphics_bind_image_memory:
		OutputDebugString (L"graphics error: bind image memory\n");
		break;

	case egraphics_result::e_against_error_graphics_map_memory:
		OutputDebugString (L"graphics error: map image memory\n");
		break;

	case egraphics_result::e_against_error_graphics_create_descriptor_set_layout:
		OutputDebugString (L"graphics error: create descriptor set layout\n");
		break;

	case egraphics_result::e_against_error_graphics_create_graphics_pipeline_layout:
		OutputDebugString (L"graphics error: create pipeline layout\n");
		break;

	case egraphics_result::e_against_error_graphics_create_descriptor_pool:
		OutputDebugString (L"graphics error: create descriptor pool\n");
		break;

	case egraphics_result::e_against_error_graphics_allocate_descriptor_set:
		OutputDebugString (L"graphics error: allocate descriptor set\n");
		break;

	case egraphics_result::e_against_error_graphics_create_render_pass:
		OutputDebugString (L"graphics error: create render pass\n");
		break;

	case egraphics_result::e_against_error_graphics_create_shader_module:
		OutputDebugString (L"graphics error: create shader module\n");
		break;

	case egraphics_result::e_against_error_graphics_create_framebuffers:
		OutputDebugString (L"graphics error: create framebuffer\n");
		break;

	case egraphics_result::e_against_error_graphics_begin_command_buffer:
		OutputDebugString (L"graphics error: begin command buffer\n");
		break;

	case egraphics_result::e_against_error_graphics_end_command_buffer:
		OutputDebugString (L"graphics error: end command buffer\n");
		break;

	case egraphics_result::e_against_error_graphics_create_command_pool:
		OutputDebugString (L"graphics error: create command pool\n");
		break;

	case egraphics_result::e_against_error_graphics_allocate_command_buffer:
		OutputDebugString (L"graphics error: allocate command buffer\n");
		break;

	case egraphics_result::e_against_error_graphics_create_graphics_pipeline:
		OutputDebugString (L"graphics error: create basegraphics pipeline\n");
		break;

	case egraphics_result::e_against_error_graphics_create_semaphore:
		OutputDebugString (L"graphics error: create semaphore\n");
		break;

	case egraphics_result::e_against_error_graphics_acquire_next_image:
		OutputDebugString (L"graphics error: acquire next image\n");
		break;

	case egraphics_result::e_against_error_graphics_wait_for_fences:
		OutputDebugString (L"graphics error: wait for fence\n");
		break;

	case egraphics_result::e_against_error_graphics_create_fence:
		OutputDebugString (L"graphics error: create fence\n");
		break;

	case egraphics_result::e_against_error_graphics_reset_fence:
		OutputDebugString (L"graphics error: reset fence\n");
		break;

	case egraphics_result::e_against_error_graphics_queue_submit:
		OutputDebugString (L"graphics error: queue submit\n");
		break;

	case egraphics_result::e_against_error_graphics_queue_wait_idle:
		OutputDebugString (L"graphics error: queue wait idle\n");
		break;

	case egraphics_result::e_against_error_graphics_queue_present:
		OutputDebugString (L"graphics error: queue present\n");
		break;

	case egraphics_result::e_against_error_graphics_update_uniform_buffer:
		OutputDebugString (L"graphics error: update uniform buffer\n");
		break;

	case egraphics_result::e_against_error_graphics_create_sampler:
		OutputDebugString (L"graphics error: create texture splashscreensampler\n");
		break;

	case egraphics_result::e_against_error_imgui_init:
		OutputDebugString (L"graphics error:imgui init\n");
		break;

	case egraphics_result::e_against_error_system_allocate_memory:
		OutputDebugString (L"system error: allocate memory\n");
		break;

	case egraphics_result::e_against_error_system_create_file:
		OutputDebugString (L"system error: create file\n");
		break;

	case egraphics_result::e_against_error_system_open_file:
		OutputDebugString (L"system error: open file\n");
		break;

	default:
		break;
	}
}