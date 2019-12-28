#pragma once

#include <Windows.h>

class Scene
{
public:
	Scene () { OutputDebugString (L"Scene::Scene\n"); }
	~Scene () { OutputDebugString (L"Scene::~Scene\n"); }

	virtual void Draw () = 0;

	static int NumberTimes;
};