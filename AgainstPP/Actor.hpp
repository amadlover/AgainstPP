#pragma once

#include <string>

class Actor
{
public:
	Actor ();
	~Actor ();

	float TransformationMatrix[16];

	float Translation[3];
	float Rotation[4];
	float Scale[3];

	std::string Name;
};