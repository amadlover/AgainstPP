#pragma once

#include "Scene.hpp"

class MainMenuScene : public Scene
{
public:
	MainMenuScene () { OutputDebugString (L"MainMenu::MainMenu\n"); }
	~MainMenuScene () { OutputDebugString (L"MainMenu::~MainMenu\n"); }

	void Init () override { OutputDebugString (L"MainMenuScene::Init\n"); }
	void Draw () override { }
	void Exit () override { OutputDebugString (L"MainMenuScene::Exit\n"); }
};