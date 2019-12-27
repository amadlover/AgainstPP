#pragma once

#include <vulkan/vulkan.h>

struct VkImageHandles
{
	VkMemoryRequirements MemoryRequirements;
};

struct Image
{
	char Name[256];
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
	char Name[256];
	Image* Image;
	Sampler* Sampler;
};

struct Material
{
	char Name[256];
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

struct Primitive
{
	float* Positions;
	float* UV0s;
	float* Normals;

	uint32_t PositionSize;
	uint32_t UV0Size;
	uint32_t NormalSize;
	uint32_t IndexSize;

	uint32_t IndexCount;
	uint32_t* Indices;

	Material* Material;

	VkPrimitiveHandles VkHandles;
};

struct Mesh
{
	char Name[256];
	uint32_t ID;

	Primitive* Primitives;
	uint32_t PrimitiveCount;
};

struct Node
{
	float TransformationMatrix[16];

	float Translation[3];
	float Rotation[4];
	float Scale[3];

	char Name[256];

	Mesh* Mesh;
};
