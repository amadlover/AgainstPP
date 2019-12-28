#include "Mesh.hpp"


_Mesh::_Mesh ()
{

}

_Mesh::~_Mesh ()
{

}

void _Mesh::Update ()
{

}

void _Mesh::Draw ()
{
	for (auto Primitive : Primitives)
	{
		Primitive.Draw ();
	}
}