#pragma once

#include "Scene.hpp"

class MainMenuScene : public Scene
{
public:
	MainMenuScene () { OutputDebugString (L"MainMenu::MainMenu\n"); Init (); }
	~MainMenuScene () { OutputDebugString (L"MainMenu::~MainMenu\n"); Exit (); }

	void Init () override { OutputDebugString (L"MainMenuScene::Init\n"); }
	void Draw () override { }
	void Exit () override { OutputDebugString (L"MainMenuScene::Exit\n"); }
};