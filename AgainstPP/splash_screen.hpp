#pragma once

#include "common_graphics.hpp"
#include "splash_screen_graphics.hpp"
#include "actor.hpp"
#include "asset.hpp"
#include "event.hpp"
#include "scene.hpp"

#include <memory>

/*namespace splash_screen
{
	struct splash_screen
	{
		std::vector<asset::mesh::mesh> meshes;
		std::vector<asset::image::image> images;

		std::vector<actor::actor> actors;
	};

	void init (common_graphics::common_graphics* common_graphics_obj_ptr, event::event* ptr);
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void draw ();
	void exit ();
}*/

class splash_screen : public scene
{
public:
	splash_screen ();
	~splash_screen ();

	void init (event* event_ptr) override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam) override;
	void main_loop () override;
	void exit () override;

private:
	std::unique_ptr<splash_screen_graphics> graphics;
};