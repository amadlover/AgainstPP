#pragma once

#include "Assets.hpp"
#include "EntityComponent.hpp"

#include <memory>

class StaticMeshComponent : EntityComponent
{
public:
	StaticMeshComponent ();
	~StaticMeshComponent ();

	void Update () override;
	void Draw () override;

private:
	std::unique_ptr<Mesh> Mesh;
};

