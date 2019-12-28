#pragma once

#include "Mesh.hpp"

#include <memory>

class MeshComponent
{
public:
	MeshComponent ();
	~MeshComponent ();

	void Init ();

	void Update ();
	void Draw ();

private:
	std::unique_ptr<Mesh> Mesh;
};

