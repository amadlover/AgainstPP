#include "SplashScreenScene.hpp"
#include "Utils.hpp"

SplashScreenScene::SplashScreenScene (const std::unique_ptr<BaseGraphics>& G)
{ 
	OutputDebugString (L"SplashScreenScene::SplashScreenScene\n");

	Graphics = std::make_unique<_SplashSceneGraphics> (G);
	GLTFData = std::make_unique<_GLTFData> (Utils::GetFullPath ("\\UIElements\\SplashScreen\\SplashScreen.gltf"));
}

void SplashScreenScene::Update ()
{
	
}

void SplashScreenScene::Draw ()
{
}

SplashScreenScene::~SplashScreenScene ()
{
	OutputDebugString (L"SplashScreenScene::~SplashScreenScene\n");
}