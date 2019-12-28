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
	Background->MeshComponent->Mesh->ID = 0;
	Background->MeshComponent->Mesh->Name = "Background";
	
	_Primitive Prim;
	Prim.Positions.push_back (Float3{ 1, 1, 1 });
	Prim.Positions.push_back (Float3{ -1,1,1 });
	Prim.Positions.push_back (Float3{ -1,-1,1 });
	Prim.Positions.push_back (Float3{ 1,-1,1 });

	Prim.PositionSize = Prim.Positions.size () * sizeof (Float3);

	Prim.Indices.push_back (0); Prim.Indices.push_back (1);	Prim.Indices.push_back (2);
	Prim.Indices.push_back (0);	Prim.Indices.push_back (2);	Prim.Indices.push_back (3);

	Prim.IndexCount = 6;
	Prim.IndexSize = Prim.Indices.size () * sizeof (uint32_t);

	Prim.UV0s.push_back (Float2{ 1,1 });
	Prim.UV0s.push_back (Float2{ 0,1 });
	Prim.UV0s.push_back (Float2{ 0,0 });
	Prim.UV0s.push_back (Float2{ 1,0 });

	Prim.UV0Size = Prim.UV0s.size () * sizeof (Float2);

	Background->MeshComponent->Mesh->Primitives.push_back (Prim);
}

void SplashScreenScene::Draw ()
{
	Background->Draw ();
}

SplashScreenScene::~SplashScreenScene ()
{
	OutputDebugString (L"SplashScreenScene::~SplashScreenScene\n");
}