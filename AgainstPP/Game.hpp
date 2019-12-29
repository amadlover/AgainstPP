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