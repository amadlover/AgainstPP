#pragma once

#include <Windows.h>

class Scene
{
public:
	Scene () { OutputDebugString (L"Scene::Scene\n"); Init (); }
	~Scene () { OutputDebugString (L"Scene::~Scene\n"); Exit (); }

	virtual void Init () {}
	virtual void Draw () { OutputDebugString (L"Scene::Draw\n"); }
	virtual void Exit () {}

	static int NumberTimes;
};