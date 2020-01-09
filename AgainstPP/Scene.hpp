#pragma once

#include <Windows.h>
#include <memory>
#include <vector>

#include "Actor.hpp"

class Scene
{
public:
	Scene () { OutputDebugString (L"Scene::Scene\n"); }
	~Scene () { OutputDebugString (L"Scene::~Scene\n"); }

	virtual void Update () {}
	virtual void Draw () {}

	void LoadGLTFData (std::string FilePath);

protected:
	std::vector<Actor> Actors;
};