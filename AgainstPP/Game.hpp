#pragma once

#include <Windows.h>
#include <memory>

#include "BaseGraphics.hpp"
#include "SplashScreenScene.hpp"
#include "MainMenuScene.hpp"

enum class SceneType
{
	SplashScreen,
	MainMenu,
	MainGame
};

class Game
{
public:
	Game (HINSTANCE HInstance, HWND HWnd);
	~Game ();

	void Run ();

	void ProcessKeyboardInput (WPARAM wParam, LPARAM lParam);
	void ProcessLeftMouseClick ();
	void ProcessMiddleMouseClick ();
	void ProcessRightMouseClick ();
	void ProcessMouseMovement (WPARAM wParam, LPARAM lParam);
	void ProcessWindowDestroy ();

private:
	std::unique_ptr<BaseGraphics> _G;
	std::shared_ptr<Scene> _CurrentScene;

	std::shared_ptr<SplashScreenScene> _SplashScreen;
	std::shared_ptr<MainMenuScene> _MainMenu;

	SceneType _CurrentSceneType;
};

namespace game
{
	void init (HINSTANCE HInstance, HWND HWnd);
	void run ();
	void exit ();

	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	void process_left_mouse_click ();
	void process_middle_mouse_click ();
	void process_right_mouse_click ();
	void process_mouse_movement (WPARAM wParam, LPARAM lParam);

	void process_window_destroy ();
};