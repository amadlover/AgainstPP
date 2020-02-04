#pragma once

#include "scene.hpp"
#include "event.hpp"
#include "asset.hpp"

#include <Windows.h>
#include <vector>

/*namespace main_menu
{
	void init (event::event* event_ptr);
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void draw ();
	void exit ();
}*/

class main_menu : public scene
{
public:
	main_menu ();
	~main_menu ();

	egraphics_result init () override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	egraphics_result main_loop () override;
	void exit () override;

protected:
	egraphics_result update (const std::vector<asset::mesh>& meshes) override;
	egraphics_result draw (const std::vector<asset::mesh>& meshes) override;
};
