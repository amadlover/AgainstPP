#pragma once

#include <Windows.h>
#include <memory>
#include <vector>

#include "Asset.hpp"
#include "Actor.hpp"

class Scene
{
public:
	Scene () { OutputDebugString (L"Scene::Scene\n"); }
	~Scene () { OutputDebugString (L"Scene::~Scene\n"); }

	virtual void Update () {}
	virtual void Draw () {}

protected:
	std::vector<Asset> Assets;
	std::vector<Actor> Actors;
};