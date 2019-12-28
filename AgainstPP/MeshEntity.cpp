#include "MeshEntity.hpp"
#include "MeshComponent.hpp"

#include <Windows.h>
#include <memory>

MeshEntity::MeshEntity ()
{
	OutputDebugString (L"MeshEntity::MeshEntity\n");

	MeshComponent = std::make_unique<_MeshComponent> ();
}

MeshEntity::~MeshEntity ()
{
	OutputDebugString (L"MeshEntity::~MeshEntity\n");
}

void MeshEntity::Update ()
{

}

void MeshEntity::Draw ()
{
	MeshComponent->Draw ();
}