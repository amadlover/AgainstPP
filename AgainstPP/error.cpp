#include "Error.hpp"
#include <Windows.h>

void log_error (egraphics_error Err)
{
	switch (Err)
	{
	case egraphics_error::populate_instance_layers_and_extensions:
		OutputDebugString (L"BaseGraphics Error: Populating Instance Layers and Extensions\n");
		break;

	case egraphics_error::create_instance:
		OutputDebugString (L"BaseGraphics Error: Create Instance\n");
		break;

	case egraphics_error::setup_debug_utils_messenger:
		OutputDebugString (L"BaseGraphics Error: Setup Debug Utils Messenger\n");
		break;

	case egraphics_error::create_debug_utils_messenger:
		OutputDebugString (L"BaseGraphics Error: Create Debug Utils Messenger\n");
		break;

	case egraphics_error::destroy_debug_utils_messenger:
		OutputDebugString (L"BaseGraphics Error: Destroy Debud Utils Messenger\n");
		break;

	case egraphics_error::create_surface:
		OutputDebugString (L"BaseGraphics Error: Create Surface\n");
		break;

	case egraphics_error::get_physical_device:
		OutputDebugString (L"BaseGraphics Error: Get Physical Device\n");
		break;

	case egraphics_error::populate_device_layers_and_extensions:
		OutputDebugString (L"BaseGraphics Error: Populate Device Layers and Extensions\n");
		break;

	case egraphics_error::create_graphics_device:
		OutputDebugString (L"BaseGraphics Error: Create BaseGraphics Device\n");
		break;

	case egraphics_error::surface_support:
		OutputDebugString (L"BaseGraphics Error: Surface Support\n");
		break;

	case egraphics_error::create_swapchain:
		OutputDebugString (L"BaseGraphics Error: Create Swapchain\n");
		break;

	case egraphics_error::create_image_view:
		OutputDebugString (L"BaseGraphics Error: Create Image View\n");
		break;

	case egraphics_error::create_buffer:
		OutputDebugString (L"BaseGraphics Error: Create Buffer\n");
		break;

	case egraphics_error::allocate_memory:
		OutputDebugString (L"BaseGraphics Error: Allocate Buffer Memory\n");
		break;

	case egraphics_error::bind_buffer_memory:
		OutputDebugString (L"BaseGraphics Error: Bind Buffer Memory\n");
		break;

	case egraphics_error::create_image:
		OutputDebugString (L"BaseGraphics Error: Create Image\n");
		break;

	case egraphics_error::bind_image_memory:
		OutputDebugString (L"BaseGraphics Error: Bind Image Memory\n");
		break;

	case egraphics_error::map_memory:
		OutputDebugString (L"BaseGraphics Error: Map Image Memory\n");
		break;

	case egraphics_error::create_descriptor_set_layout:
		OutputDebugString (L"BaseGraphics Error: Create Descriptor Set Layout\n");
		break;

	case egraphics_error::create_pipeline_layout:
		OutputDebugString (L"BaseGraphics Error: Create Pipeline Layout\n");
		break;

	case egraphics_error::create_descriptor_pool:
		OutputDebugString (L"BaseGraphics Error: Create Descriptor Pool\n");
		break;

	case egraphics_error::allocate_descriptor_set:
		OutputDebugString (L"BaseGraphics Error: Allocate Descriptor Set\n");
		break;

	case egraphics_error::create_render_pass:
		OutputDebugString (L"BaseGraphics Error: Create Render Pass\n");
		break;

	case egraphics_error::create_shader_module:
		OutputDebugString (L"BaseGraphics Error: Create Shader Module\n");
		break;

	case egraphics_error::create_framebuffers:
		OutputDebugString (L"BaseGraphics Error: Create Framebuffer\n");
		break;

	case egraphics_error::begin_command_buffer:
		OutputDebugString (L"BaseGraphics Error: Begin Command Buffer\n");
		break;

	case egraphics_error::end_command_buffer:
		OutputDebugString (L"BaseGraphics Error: End Command Buffer\n");
		break;

	case egraphics_error::create_command_pool:
		OutputDebugString (L"BaseGraphics Error: Create Command Pool\n");
		break;

	case egraphics_error::allocate_command_buffer:
		OutputDebugString (L"BaseGraphics Error: Allocate Command Buffer\n");
		break;

	case egraphics_error::create_graphics_pipeline:
		OutputDebugString (L"BaseGraphics Error: Create BaseGraphics Pipeline\n");
		break;

	case egraphics_error::create_semaphore:
		OutputDebugString (L"BaseGraphics Error: Create Semaphore\n");
		break;

	case egraphics_error::acquire_next_image:
		OutputDebugString (L"BaseGraphics Error: Acquire Next Image\n");
		break;

	case egraphics_error::wait_for_fences:
		OutputDebugString (L"BaseGraphics Error: Wait for Fence\n");
		break;

	case egraphics_error::create_fence:
		OutputDebugString (L"BaseGraphics Error: Create Fence\n");
		break;

	case egraphics_error::reset_fence:
		OutputDebugString (L"BaseGraphics Error: Reset Fence\n");
		break;

	case egraphics_error::queue_submit:
		OutputDebugString (L"BaseGraphics Error: Queue Submit\n");
		break;

	case egraphics_error::queue_present:
		OutputDebugString (L"BaseGraphics Error: Queue Present\n");
		break;

	case egraphics_error::update_uniform_buffer:
		OutputDebugString (L"BaseGraphics Error: Update Uniform Buffer\n");
		break;

	case egraphics_error::create_sampler:
		OutputDebugString (L"BaseGraphics Error: Create Texture SplashScreenSampler\n");
		break;

	default:
		break;
	}
}