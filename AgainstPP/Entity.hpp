#pragma once

#include "Assets.hpp"

#include <string>

class Entity
{
public:
	Entity ();
	~Entity ();

	virtual void Update () = 0;
	virtual void Draw () = 0;

protected:
	float TransformationMatrix[16];

	float Translation[3];
	float Rotation[4];
	float Scale[3];

	std::string Name;
};