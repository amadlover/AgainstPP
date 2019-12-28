#pragma once

#include "Graphics.hpp"
#include "Scene.hpp"
#include "MeshEntity.hpp"

#include <memory>

class SplashScreenScene : public Scene
{
public:
	SplashScreenScene (const std::unique_ptr<Graphics>& G);
	~SplashScreenScene ();

	void Draw () override;

private:
	void _CreateBackground ();

	std::unique_ptr<MeshEntity> Background;
};