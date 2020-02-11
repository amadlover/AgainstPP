#include "main_menu.hpp"
#include "graphics_utils.hpp"
#include "utils.hpp"

main_menu::main_menu () : scene ()
{
	OutputDebugString (L"main_menu::main_menu\n");
}

main_menu::~main_menu ()
{
	OutputDebugString (L"main_menu::~main_menu\n");
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

void main_menu::exit ()
{
	OutputDebugString (L"main_menu::exit\n");

	vkDestroyCommandPool (common_graphics::graphics_device, command_pool, nullptr);
	command_pool = VK_NULL_HANDLE;

	for (auto& mesh : meshes)
	{
		for (auto& graphics_primitive : mesh.graphics_primitves)
		{
			vkDestroyImageView (common_graphics::graphics_device, graphics_primitive.material.base_color_texture.texture_image.image_view, nullptr);
			graphics_primitive.material.base_color_texture.texture_image.image_view = VK_NULL_HANDLE;
		}
	}

	//vkDestroyDescriptorPool (common_graphics::graphics_device, descriptor_pool, nullptr);
	//descriptor_pool = VK_NULL_HANDLE;

	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &staging_vertex_index_buffer, 1, &staging_vertex_index_memory);
	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &vertex_index_buffer, 1, &vertex_index_memory);
	graphics_utils::destroy_buffers_and_buffer_memory (common_graphics::graphics_device, &staging_image_buffer, 1, &staging_image_memory);
	graphics_utils::destroy_images_and_image_memory (common_graphics::graphics_device, scene_images.data (), scene_images.size (), &scene_images_memory);

	meshes.clear ();
	scene_images.clear ();

	state = e_scene_state::exited;
}
