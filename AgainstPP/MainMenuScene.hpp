#pragma once

#include "Scene.hpp"

class MainMenuScene : public Scene
{
public:
	MainMenuScene () { OutputDebugString (L"MainMenu::MainMenu\n"); }
	~MainMenuScene () { OutputDebugString (L"MainMenu::~MainMenu\n"); }

	void Draw () override {}
};