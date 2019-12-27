#include "SplashScreenScene.hpp"

SplashScreenScene::SplashScreenScene (const std::unique_ptr<Graphics>& G)
{ 
	OutputDebugString (L"SplashScreenScene::SplashScreenScene\n");
}

void SplashScreenScene::_CreateMesh ()
{
}

SplashScreenScene::~SplashScreenScene ()
{
	OutputDebugString (L"SplashScreenScene::~SplashScreenScene\n");
}