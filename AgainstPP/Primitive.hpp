#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

struct VkImageHandles
{
	VkMemoryRequirements MemoryRequirements;
};

struct Image
{
	std::string Name;
	unsigned int Width;
	unsigned int Height;
	unsigned int BPP;

	unsigned char* Pixels;

	uint32_t Size;

	VkImageHandles VkHandles;
};

struct Sampler
{
	int MinFilter;
	int MagFilter;
	int Wrap_S;
	int Wrap_T;
};

struct Texture
{
	std::string Name;
	Image* Image;
	Sampler* Sampler;
};

struct Material
{
	std::string Name;
	Texture* BaseColorTexture;
};

struct VkPrimitiveHandles
{
	//TODO: Pointers or values;
	VkBuffer VBIB;
	VkImage* Image;
	VkImageView* ImageView;
	VkSampler Sampler;
	VkDescriptorSet* DescriptorSet;
	VkMemoryRequirements MemoryRequirements;
};

struct Float4
{
	float x, y, z, w;
};

struct Float3
{
	float x, y, z;
};

struct Float2
{
	float x, y;
};

class Primitive
{
public:
	Primitive () {}
	~Primitive () {}

	void Update () {}
	void Draw () {}

	std::vector<Float3> Positions;
	std::vector<Float2> UV0s;
	std::vector<Float3> Normals;

	uint32_t PositionSize;
	uint32_t UV0Size;
	uint32_t NormalSize;
	uint32_t IndexSize;

	uint32_t IndexCount;
	std::vector<uint32_t> Indices;

	Material* Material;

	VkPrimitiveHandles VkHandles;
};
