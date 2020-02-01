#pragma once

#define CHECK_AGAINST_RESULT(result) if (result != egraphics_result::e_success) return result;

enum egraphics_result
{
	e_success,
	e_against_error_populate_instance_layers_and_extensions,
	e_against_error_create_instance,
	e_against_error_setup_debug_utils_messenger,
	e_against_error_create_debug_utils_messenger,
	e_against_error_destroy_debug_utils_messenger,
	e_against_error_create_surface,
	e_against_error_get_physical_device,
	e_against_error_populate_device_layers_and_extensions,
	e_against_error_create_graphics_device,
	e_against_error_surface_support,
	e_against_error_create_swapchain,
	e_against_error_create_image_view,
	e_against_error_create_buffer,
	e_against_error_allocate_memory,
	e_against_error_bind_buffer_memory,
	e_against_error_create_image,
	e_against_error_bind_image_memory,
	e_against_error_map_memory,
	e_against_error_create_descriptor_set_layout,
	e_against_error_create_pipeline_layout,
	e_against_error_create_descriptor_pool,
	e_against_error_allocate_descriptor_set,
	e_against_error_create_render_pass,
	e_against_error_create_shader_module,
	e_against_error_create_framebuffers,
	e_against_error_begin_command_buffer,
	e_against_error_end_command_buffer,
	e_against_error_reset_command_buffer,
	e_against_error_create_command_pool,
	e_against_error_allocate_command_buffer,
	e_against_error_create_graphics_pipeline,
	e_against_error_create_semaphore,
	e_against_error_acquire_next_image,
	e_against_error_wait_for_fences,
	e_against_error_create_fence,
	e_against_error_reset_fence,
	e_against_error_queue_submit,
	e_against_error_queue_present,
	e_against_error_update_uniform_buffer,
	e_against_error_create_sampler,
};

void log_error (egraphics_result Err);

