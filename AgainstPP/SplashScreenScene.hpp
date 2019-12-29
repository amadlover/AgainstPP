#pragma once

#include "BaseGraphics.hpp"
#include "SplashSceneGraphics.hpp"
#include "Scene.hpp"
#include "MeshEntity.hpp"

#include <memory>

class SplashScreenScene : public Scene
{
public:
	SplashScreenScene (const std::unique_ptr<BaseGraphics>& G);
	~SplashScreenScene ();

	void Draw () override;

	std::unique_ptr<_SplashSceneGraphics> Graphics;

private:
	void _CreateBackground ();

	std::unique_ptr<MeshEntity> _Background;
};