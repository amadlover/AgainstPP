#pragma once

#include "Entity.hpp"
#include "MeshComponent.hpp"

class MeshEntity : public Entity
{
public:
	MeshEntity ();
	~MeshEntity ();

	void Update () override;
	void Draw () override;
	
	std::unique_ptr<MeshComponent> MeshComponent;
};

