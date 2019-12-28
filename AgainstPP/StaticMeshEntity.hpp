#pragma once

#include "Entity.hpp"
#include "StaticMeshComponent.hpp"

class StaticMeshEntity : public Entity
{
public:
	StaticMeshEntity ();
	~StaticMeshEntity ();

	void Update () override;
	void Draw () override;
	
	std::unique_ptr<StaticMeshComponent> MeshComponent;
};

