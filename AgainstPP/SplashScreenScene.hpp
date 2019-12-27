#pragma once

#include "Graphics.hpp"
#include "Scene.hpp"
#include "Assets.hpp"

#include <memory>

class SplashScreenScene : public Scene
{
public:
	SplashScreenScene (const std::unique_ptr<Graphics>& G);
	~SplashScreenScene ();

	void Init () override { OutputDebugString (L"SplashScreenScene::Init\n"); }
	void Draw () override {}
	void Exit () override { OutputDebugString (L"SplashScreenScene::Exit\n"); }

private:
	void _CreateMesh ();

	Mesh Mesh;
};