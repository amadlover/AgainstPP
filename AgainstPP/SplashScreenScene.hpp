#pragma once

#include "BaseGraphics.hpp"
#include "SplashSceneGraphics.hpp"
#include "Scene.hpp"

#include <memory>

class SplashScreenScene : public Scene
{
public:
	SplashScreenScene (const std::unique_ptr<BaseGraphics>& G);
	~SplashScreenScene ();

	void Update () override {}
	void Draw () override;

	std::unique_ptr<_SplashSceneGraphics> Graphics;
};