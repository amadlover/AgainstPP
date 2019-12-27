#pragma once

#include <Windows.h>

#include "Scene.hpp"

class SplashScreenScene : public Scene
{
public:
	SplashScreenScene () { OutputDebugString (L"SplashScreenScene::SplashScreenScene\n"); }
	~SplashScreenScene () { OutputDebugString (L"SplashScreenScene::~SplashScreenScene\n"); }

	void Init () override { OutputDebugString (L"SplashScreenScene::Init\n"); }
	void Draw () override {}
	void Exit () override { OutputDebugString (L"SplashScreenScene::Exit\n"); }
};