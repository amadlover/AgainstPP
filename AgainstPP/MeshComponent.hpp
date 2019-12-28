#pragma once

#include "Mesh.hpp"

#include <memory>

class _MeshComponent
{
public:
	_MeshComponent ();
	~_MeshComponent ();

	void Update ();
	void Draw ();

	std::unique_ptr<_Mesh> Mesh;
};

