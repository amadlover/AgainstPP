#include "test_scene.hpp"
#include "game.hpp"
#include "utils.hpp"

test_scene::test_scene ()
{
}

test_scene::test_scene (game* ptr) : scene (ptr)
{
}

test_scene::~test_scene ()
{
}

egraphics_result test_scene::init ()
{
	OutputDebugString (L"test_scene::init\n");
	CHECK_AGAINST_RESULT (utils::import_gltf ("\\TestScene\\TestScene.gltf", meshes));
	return egraphics_result::success;
}

void test_scene::process_keyboard_input (WPARAM wParam, LPARAM lParam)
{
	OutputDebugString (L"test_scene::process_keyboard_input\n");

	egraphics_result result = egraphics_result::success;
	switch (wParam)
	{
	case VK_ESCAPE:
		game_ptr->set_current_scene (e_scene_type::splash_screen);
		break;

	case 0x53:
		break;

	case 0x31:
		result = spawn_ship ();

		if (result != egraphics_result::success)
		{
			log_error (result);
		}
		else
		{
			update_command_buffers ();
		}
		break;

	default:
		break;
	}
}

egraphics_result test_scene::main_loop ()
{
    return egraphics_result::success;
}

egraphics_result test_scene::update ()
{
    return egraphics_result::success;
}

egraphics_result test_scene::update_command_buffers () const
{
	for (const auto& actor : actors)
	{
		actor.update_command_buffer (/*command_buffer*/);
	}

    return egraphics_result::success;
}

egraphics_result test_scene::present ()
{
	return egraphics_result::success;
}

egraphics_result test_scene::spawn_ship ()
{
	// create new actor object.
	// go through meshes to find the appropriate mesh.
	// update the trasnform buffer at the correct offset.
	// update command buffers.
	return egraphics_result::success;
}

void test_scene::exit ()
{
	OutputDebugString (L"test_scene::exit\n");
	meshes.clear ();
}
