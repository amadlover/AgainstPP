#pragma once

#include <Windows.h>
#include <memory>

#include "common_graphics.hpp"
#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "test_scene.hpp"
#include "entity.hpp"

#include "event.hpp"

class game : public entity
{
public:
	game ();
	~game ();
	
	egraphics_result init (HINSTANCE hInstance, HWND hWnd);
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	egraphics_result set_current_scene (e_scene_type scene_type);
	egraphics_result main_loop ();
	void exit ();

private:
	std::shared_ptr<scene> current_scene_ptr;
};