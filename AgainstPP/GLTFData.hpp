#pragma once

#include <string>
#include <Windows.h>
#include <vector>
#include <memory>

class Material
{
public:
	Material () {}
	~Material () {}
};

class Primitive
{
public:
	Primitive () {}
	~Primitive () {}

	Material Material;

	std::vector<float> Positions;
	std::vector<float> Normals;
	std::vector<float> UV0;

	uint32_t PositionSize;
	uint32_t NormalSize;
	uint32_t UV0Size;
	uint32_t IndexSize;

	std::vector<uint32_t> Indices;
	uint32_t IndexCount;
};

struct MeshNode
{
	std::vector<Primitive> Primitives;
};

class _GLTFData
{
public:
	_GLTFData (std::string FilePath);
	~_GLTFData () { OutputDebugString (L"_GLTFData::~_GLTFData\n"); }

	std::vector <MeshNode> MeshNodes;
};

