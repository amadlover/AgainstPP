#include "SplashScreenScene.hpp"

SplashScreenScene::SplashScreenScene (const std::unique_ptr<BaseGraphics>& G)
{ 
	OutputDebugString (L"SplashScreenScene::SplashScreenScene\n");

	Graphics = std::make_unique<_SplashSceneGraphics> (G);
	GLTFData = std::make_unique<_GLTFData> ("HH");
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