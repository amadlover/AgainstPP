#pragma once

#include "Graphics.hpp"
#include "Scene.hpp"
#include "StaticMeshEntity.hpp"

#include <memory>

class SplashScreenScene : public Scene
{
public:
	SplashScreenScene (const std::unique_ptr<Graphics>& G);
	~SplashScreenScene ();

	void Draw () override {}

private:
	void _CreateEntity ();

	std::unique_ptr<StaticMeshEntity> Background;
};