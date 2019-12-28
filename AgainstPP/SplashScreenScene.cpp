#include "SplashScreenScene.hpp"

SplashScreenScene::SplashScreenScene (const std::unique_ptr<Graphics>& G)
{ 
	OutputDebugString (L"SplashScreenScene::SplashScreenScene\n");

	_CreateBackground ();
}

void SplashScreenScene::_CreateBackground ()
{
	OutputDebugString (L"SplashScreenScene::_CreateBackground\n");

	Background = std::make_unique<MeshEntity> ();
}

SplashScreenScene::~SplashScreenScene ()
{
	OutputDebugString (L"SplashScreenScene::~SplashScreenScene\n");
}