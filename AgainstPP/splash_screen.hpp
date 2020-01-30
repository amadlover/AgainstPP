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
	splash_screen () { OutputDebugString (L"splash_screen::splash_screen\n"); }
	splash_screen (event* event_ptr) { this->event_ptr = event_ptr; }
	~splash_screen () { OutputDebugString (L"splash_screen::~splash_screen\n"); }

	void init () override { OutputDebugString (L"splash_screen::init\n"); };
	void process_keyboard_input (WPARAM wParam, LPARAM lParam) override;

	void main_loop () override
	{
	}

	void exit () override { OutputDebugString (L"splash_screen::exit\n"); }
};