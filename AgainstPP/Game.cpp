#include "Game.hpp"

#include <Windows.h>

#include <memory>

#include "SplashScreenScene.hpp"
#include "MainMenuScene.hpp"

Game::Game ()
{
	OutputDebugString (L"Game::Game\n");

	_G = std::make_unique<Graphics> ();

	_SplashScreen = std::make_shared<SplashScreenScene> ();
	_MainMenu = std::make_shared<MainMenuScene> ();

	_CurrentScene = _SplashScreen;
	_CurrentScene->Init ();

	_CurrentSceneType = SceneType::SplashScreen;
}

void Game::ProcessKeyboardInput (WPARAM wParam, LPARAM lParam)
{
	switch (_CurrentSceneType)
	{
	case SceneType::SplashScreen:
		switch (wParam)
		{
		case VK_ESCAPE:
			_CurrentScene->Exit ();
			
			_CurrentScene = _MainMenu;
			_CurrentScene->Init ();
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
			_CurrentScene->Exit ();

			_CurrentScene = _SplashScreen;
			_CurrentScene->Init ();

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

void Game::Run ()
{
	if (_CurrentScene)
	{
		_CurrentScene->Draw ();
	}
}

Game::~Game ()
{
	OutputDebugString (L"Game::~Game\n");
}
