#include "MeshComponent.hpp"

_MeshComponent::_MeshComponent ()
{
	Mesh = std::make_unique<_Mesh> ();
}

_MeshComponent::~_MeshComponent ()
{

}

void _MeshComponent::Update ()
{

}

void _MeshComponent::Draw ()
{
	Mesh->Draw ();
}
