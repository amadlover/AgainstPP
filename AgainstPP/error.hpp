#pragma once

enum class egraphics_error
{
	populate_instance_layers_and_extensions,
	create_instance,
	setup_debug_utils_messenger,
	create_debug_utils_messenger,
	destroy_debug_utils_messenger,
	create_surface,
	get_physical_device,
	populate_device_layers_and_extensions,
	create_graphics_device,
	surface_support,
	create_swapchain,
	create_image_view,
	create_buffer,
	allocate_memory,
	bind_buffer_memory,
	create_image,
	bind_image_memory,
	map_memory,
	create_descriptor_set_layout,
	create_pipeline_layout,
	create_descriptor_pool,
	allocate_descriptor_set,
	create_render_pass,
	create_shader_module,
	create_framebuffers,
	begin_command_buffer,
	end_command_buffer,
	reset_command_buffer,
	create_command_pool,
	allocate_command_buffer,
	create_graphics_pipeline,
	create_semaphore,
	acquire_next_image,
	wait_for_fences,
	create_fence,
	reset_fence,
	queue_submit,
	queue_present,
	update_uniform_buffer,
	create_sampler,
};

void log_error (egraphics_error Err);
