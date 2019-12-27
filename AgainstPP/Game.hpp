#pragma once

#include <Windows.h>
#include <memory>

#include "Graphics.hpp"
#include "SplashScreenScene.hpp"
#include "MainMenuScene.hpp"

enum SceneType
{
	SplashScreen,
	MainMenu,
	MainGame
};

class Game
{
public:
	Game ();
	~Game ();

	void Run ();

	void ProcessKeyboardInput (WPARAM wParam, LPARAM lParam);
	void ProcessLeftMouseClick ();
	void ProcessMiddleMouseClick ();
	void ProcessRightMouseClick ();
	void ProcessMouseMovement (WPARAM wParam, LPARAM lParam);

private:
	std::unique_ptr<Graphics> _G;
	std::shared_ptr<Scene> _CurrentScene;

	std::shared_ptr<SplashScreenScene> _SplashScreen;
	std::shared_ptr<MainMenuScene> _MainMenu;

	SceneType _CurrentSceneType;
};