#pragma once

#include "scene.hpp"
#include "event.hpp"
#include "asset.hpp"

#include <Windows.h>
#include <vector>

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
	egraphics_result update () override;
	egraphics_result draw () const override;

	std::vector<asset::mesh> meshes;
};
