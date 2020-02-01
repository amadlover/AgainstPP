#pragma once

#define CHECK_AGAINST_RESULT(result) if (result != egraphics_result::e_success) return result;

enum egraphics_result
{
	e_success,
	e_against_error_graphics_populate_instance_layers_and_extensions,
	e_against_error_graphics_create_instance,
	e_against_error_graphics_setup_debug_utils_messenger,
	e_against_error_graphics_create_debug_utils_messenger,
	e_against_error_graphics_destroy_debug_utils_messenger,
	e_against_error_graphics_create_surface,
	e_against_error_graphics_get_physical_device,
	e_against_error_graphics_populate_device_layers_and_extensions,
	e_against_error_graphics_create_graphics_device,
	e_against_error_graphics_surface_support,
	e_against_error_graphics_create_swapchain,
	e_against_error_graphics_create_image_view,
	e_against_error_graphics_create_buffer,
	e_against_error_graphics_allocate_memory,
	e_against_error_graphics_bind_buffer_memory,
	e_against_error_graphics_create_image,
	e_against_error_graphics_bind_image_memory,
	e_against_error_graphics_map_memory,
	e_against_error_graphics_create_descriptor_set_layout,
	e_against_error_graphics_create_pipeline_layout,
	e_against_error_graphics_create_descriptor_pool,
	e_against_error_graphics_allocate_descriptor_set,
	e_against_error_graphics_create_render_pass,
	e_against_error_graphics_create_shader_module,
	e_against_error_graphics_create_framebuffers,
	e_against_error_graphics_begin_command_buffer,
	e_against_error_graphics_end_command_buffer,
	e_against_error_graphics_reset_command_buffer,
	e_against_error_graphics_create_command_pool,
	e_against_error_graphics_allocate_command_buffer,
	e_against_error_graphics_create_graphics_pipeline,
	e_against_error_graphics_create_semaphore,
	e_against_error_graphics_acquire_next_image,
	e_against_error_graphics_wait_for_fences,
	e_against_error_graphics_create_fence,
	e_against_error_graphics_reset_fence,
	e_against_error_graphics_queue_submit,
	e_against_error_graphics_queue_present,
	e_against_error_graphics_update_uniform_buffer,
	e_against_error_graphics_create_sampler,

	e_against_error_system_allocate_memory,
	e_against_error_system_create_file,
};

void log_error (egraphics_result Err);

