#pragma once

#include <Windows.h>
#include <memory>
#include <vector>

#include "Actor.hpp"
#include "GLTFData.hpp"

class Scene
{
public:
	Scene () { OutputDebugString (L"Scene::Scene\n"); }
	~Scene () { OutputDebugString (L"Scene::~Scene\n"); }

	virtual void Update () {}
	virtual void Draw () {}

protected:
	std::vector<Actor> SceneActors;
	std::unique_ptr<_GLTFData> GLTFData;
};