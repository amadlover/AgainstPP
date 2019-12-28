#include "SplashScreenScene.hpp"

SplashScreenScene::SplashScreenScene (const std::unique_ptr<Graphics>& G)
{ 
	OutputDebugString (L"SplashScreenScene::SplashScreenScene\n");
}

void SplashScreenScene::_CreateEntity ()
{
	Background = std::make_unique<StaticMeshEntity> ();
}

SplashScreenScene::~SplashScreenScene ()
{
	OutputDebugString (L"SplashScreenScene::~SplashScreenScene\n");
}