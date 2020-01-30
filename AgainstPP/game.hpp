#pragma once

#include <Windows.h>
#include <memory>

#include "common_graphics.hpp"
#include "splash_screen.hpp"
#include "main_menu.hpp"

/*namespace game
{
	void init (HINSTANCE HInstance, HWND HWnd);
	void draw ();
	void exit ();

	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void process_left_mouse_click ();
	void process_middle_mouse_click ();
	void process_right_mouse_click ();
	void process_mouse_movement (WPARAM wParam, LPARAM lParam);

	void process_window_destroy ();
};*/

class game
{
public:
	static game* get_instance (HINSTANCE hInstance, int cmd_show);
	
	static LRESULT CALLBACK window_proc (
		HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	);

	void main_loop ();
	~game ();

private:
	game (HINSTANCE hInstance, int cmd_show);
	
	HWND hWnd;
	static game* ptr;
};