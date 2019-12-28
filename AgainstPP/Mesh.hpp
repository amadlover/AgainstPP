#pragma once

#include <string>
#include <vector>

#include "Primitive.hpp"

class _Mesh
{
public:
	_Mesh ();
	~_Mesh ();

	void Update ();
	void Draw ();

	std::string Name;
	uint32_t ID;

	std::vector<_Primitive> Primitives;
};