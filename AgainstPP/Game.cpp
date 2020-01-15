#include "Game.hpp"

#include <Windows.h>

#include <memory>

#include "SplashScreenScene.hpp"
#include "MainMenuScene.hpp"

Game::Game (HINSTANCE HInstance, HWND HWnd)
{
	OutputDebugString (L"Game::Game\n");

	_G = std::make_unique<BaseGraphics> (HInstance, HWnd);

	_CurrentScene = std::make_shared<SplashScreenScene> (_G);

	_CurrentSceneType = SceneType::SplashScreen;
}

Game::~Game ()
{
	OutputDebugString (L"Game::~Game\n");
}

void Game::ProcessKeyboardInput (WPARAM wParam, LPARAM lParam)
{
	switch (_CurrentSceneType)
	{
	case SceneType::SplashScreen:
		switch (wParam)
		{
		case VK_ESCAPE:
			_CurrentScene = std::make_shared<MainMenuScene> ();
			
			_CurrentSceneType = SceneType::MainMenu;

			break;

		default:
			break;
		}

		break;

	case SceneType::MainMenu:
		switch (wParam)
		{
		case VK_ESCAPE:
			_CurrentScene = std::make_shared<SplashScreenScene> (_G);

			_CurrentSceneType = SceneType::SplashScreen;

			break;

		default:
			break;
		}

		break;

	default:
		break;
	}
}

void Game::ProcessLeftMouseClick ()
{
}

void Game::ProcessMiddleMouseClick ()
{
}

void Game::ProcessRightMouseClick ()
{
}

void Game::ProcessMouseMovement (WPARAM wParam, LPARAM lParam)
{
}

void Game::ProcessWindowDestroy ()
{
}

void Game::Run ()
{
	if (_CurrentScene)
	{
		_CurrentScene->Draw ();
	}
}

namespace game 
{
	enum class ecurrent_scene
	{
		splash_screen,
		main_menu,
	};

	enum class escene_state
	{
		inited,
		exited
	};

	ecurrent_scene current_scene = ecurrent_scene::splash_screen;

	escene_state splash_screen_state = escene_state::exited;
	escene_state main_menu_state = escene_state::exited;

	void init (HINSTANCE hInstance, HWND hWnd)
	{
		graphics::init (hInstance, hWnd);
	}

	void run ()
	{
		switch (current_scene)
		{
		case ecurrent_scene::splash_screen:
			OutputDebugString (L"running splash screen\n");
			break;

		case ecurrent_scene::main_menu:
			OutputDebugString (L"running main menu\n");
			break;
		}
	}

	void exit ()
	{
	}

	void process_keyboard_input (WPARAM wParam, LPARAM lParam)
	{
		switch (current_scene)
		{
		case ecurrent_scene::splash_screen:

			switch (wParam)
			{
			case VK_ESCAPE:
				splash_screen_state = escene_state::exited;
				current_scene = ecurrent_scene::main_menu;

				break;

			default:
				break;
			}

			break;

		case ecurrent_scene::main_menu:

			switch (wParam)
			{
			case VK_ESCAPE:
				main_menu_state = escene_state::exited;
				current_scene = ecurrent_scene::splash_screen;
				
				break;

			default:
				break;
			}

			break;

		default:
			break;
		}
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
}