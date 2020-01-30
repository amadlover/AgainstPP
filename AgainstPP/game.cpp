#include "game.hpp"

#include <Windows.h>
#include <memory>

#include "splash_screen.hpp"
#include "main_menu.hpp"
#include "graphics_utils.hpp"
#include "event.hpp"

#include "enums.hpp"
/*
namespace game
{
	e_scene_type current_scene = e_scene_type::splash_screen;

	e_scene_state splash_screen_state = e_scene_state::exited;
	e_scene_state main_menu_state = e_scene_state::exited;

	std::unique_ptr<common_graphics::common_graphics> common_graphics_obj_ptr (new common_graphics::common_graphics ());
	std::unique_ptr<event::event> event_obj_ptr (new event::event ());

	void(*current_scene_draw)();
	void(*current_scene_process_keyboard_input)(WPARAM, LPARAM);

	void process_keyboard_input (WPARAM wParam, LPARAM lParam)
	{
		current_scene_process_keyboard_input (wParam, lParam);
	}

	void process_left_mouse_click ()
	{
	}

	void process_middle_mouse_click ()
	{
	}

	void process_right_mouse_click ()
	{
	}

	void process_mouse_movement (WPARAM wParam, LPARAM lParam)
	{
	}

	void process_window_destroy ()
	{
	}

	void go_to_scene (e_scene_type new_scene)
	{
		switch (current_scene)
		{
		case e_scene_type::splash_screen:
			splash_screen::exit ();
			splash_screen_state = e_scene_state::exited;

			break;

		case e_scene_type::main_menu:
			main_menu::exit ();
			main_menu_state = e_scene_state::exited;

			break;

		default:
			break;
		}

		current_scene = new_scene;

		switch (current_scene)
		{
		case e_scene_type::splash_screen:
			splash_screen::init (common_graphics_obj_ptr.get (), event_obj_ptr.get ());
			splash_screen_state = e_scene_state::inited;
			current_scene_draw = splash_screen::draw;
			current_scene_process_keyboard_input = splash_screen::process_keyboard_input;
			break;

		case e_scene_type::main_menu:
			main_menu::init (event_obj_ptr.get ());
			main_menu_state = e_scene_state::inited;
			current_scene_draw = main_menu::draw;
			current_scene_process_keyboard_input = main_menu::process_keyboard_input;
			break;
		}
	}

	void init (HINSTANCE hInstance, HWND hWnd)
	{
		OutputDebugString (L"game::init\n");

		event_obj_ptr->go_to_scene = go_to_scene;

		common_graphics::init (hInstance, hWnd, common_graphics_obj_ptr.get ());
		graphics_utils::init (common_graphics_obj_ptr.get ());
		splash_screen::init (common_graphics_obj_ptr.get (), event_obj_ptr.get ());
		current_scene_draw = splash_screen::draw;
		current_scene_process_keyboard_input = splash_screen::process_keyboard_input;
	}

	void draw ()
	{
		current_scene_draw ();
	}

	void exit ()
	{
		OutputDebugString (L"game::exit\n");

		if (splash_screen_state != e_scene_state::exited)
		{
			splash_screen::exit ();
			splash_screen_state = e_scene_state::exited;
		}

		if (main_menu_state != e_scene_state::exited)
		{
			main_menu::exit ();
			main_menu_state = e_scene_state::exited;
		}

		common_graphics::exit ();
	}
}
*/

game* game::ptr = nullptr;

game::game (HINSTANCE hInstance, int cmd_show)
{
	OutputDebugString (L"game::game\n");
	WNDCLASS WC = { 0 };

	WC.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WC.lpfnWndProc = window_proc;
	WC.hInstance = hInstance;
	WC.lpszClassName = L"Against";
	WC.hCursor = LoadCursor (hInstance, IDC_ARROW);

	if (!RegisterClass (&WC))
	{
		throw std::runtime_error ("Could not register window class");
	}

	hWnd = CreateWindow (
		L"Against",
		L"Against",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1280,
		720,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		throw std::runtime_error ("Could not create Window");
	}

	ShowWindow (hWnd, cmd_show);
}

game::~game ()
{
	OutputDebugString (L"game::~game\n");
}

game* game::get_instance (HINSTANCE hInstance, int cmd_show)
{
	if (ptr != nullptr)
	{
		delete ptr;
	}
	
	ptr = new game (hInstance, cmd_show);

	return ptr;
}

LRESULT game::window_proc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		OutputDebugString (L"Some key down\n");
		switch (wParam)
		{
		case VK_ESCAPE:
			OutputDebugString (L"VK_ESCAPE\n");
			PostQuitMessage (0);
			break;
		default:
			break;
		}

		break;

	case WM_CLOSE:
		OutputDebugString (L"WM_CLOSE\n");
		PostQuitMessage (0);
		break;

	case WM_DESTROY:
		OutputDebugString (L"WM_DESTROY\n");
		PostQuitMessage (0);
		break;

	case WM_QUIT:
		OutputDebugString (L"WM_QUIT\n");
		PostQuitMessage (0);
		break;

	default:
		break;
	}

	return DefWindowProc (hWnd, msg, wParam, lParam);
}

void game::main_loop ()
{
	UpdateWindow (hWnd);

	MSG msg;
	ZeroMemory (&msg, sizeof (msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage (&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}
}