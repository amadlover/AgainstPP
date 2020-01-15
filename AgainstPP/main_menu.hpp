#pragma once

#include "Scene.hpp"

class MainMenuScene : public Scene
{
public:
	MainMenuScene () { OutputDebugString (L"MainMenu::MainMenu\n"); }
	~MainMenuScene () { OutputDebugString (L"MainMenu::~MainMenu\n"); }

	void Update () override {}
	void Draw () override {}
};

namespace main_menu
{
	void init ();
	void run ();
	void exit ();
}