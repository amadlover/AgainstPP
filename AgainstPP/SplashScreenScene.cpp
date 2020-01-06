#include "SplashScreenScene.hpp"

SplashScreenScene::SplashScreenScene (const std::unique_ptr<BaseGraphics>& G)
{ 
	OutputDebugString (L"SplashScreenScene::SplashScreenScene\n");

	Graphics = std::make_unique<_SplashSceneGraphics> (G);

	_CreateBackground ();
}

void SplashScreenScene::_CreateBackground ()
{
	OutputDebugString (L"SplashScreenScene::_CreateBackground\n");
}

void SplashScreenScene::Draw ()
{
	for (auto SceneActor : SceneActors)
	{
		SceneActor.Draw ();
	}
}

SplashScreenScene::~SplashScreenScene ()
{
	OutputDebugString (L"SplashScreenScene::~SplashScreenScene\n");
}