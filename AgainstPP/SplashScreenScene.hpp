#pragma once

#include "Scene.hpp"

class SplashScreenScene : public Scene
{
public:
	SplashScreenScene () { OutputDebugString (L"SplashScreenScene::SplashScreenScene\n"); Init (); }
	~SplashScreenScene () { OutputDebugString (L"SplashScreenScene::~SplashScreenScene\n"); Exit (); }

	void Init () override { OutputDebugString (L"SplashScreenScene::Init\n"); }
	void Draw () override {}
	void Exit () override { OutputDebugString (L"SplashScreenScene::Exit\n"); }
};