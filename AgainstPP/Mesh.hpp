#pragma once

#include <string>
#include <vector>

#include "Primitive.hpp"

class Mesh
{
public:
	Mesh ();
	~Mesh ();

	void Init ();

	void Update ();
	void Draw ();

	std::string Name;
	uint32_t ID;

	std::vector<Primitive> Primitives;
};