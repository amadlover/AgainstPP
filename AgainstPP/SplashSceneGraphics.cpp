#include "SplashSceneGraphics.hpp"
#include "Error.hpp"
#include "GraphicUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_TGA
#include <stb_image.h>

_SplashSceneGraphics::_SplashSceneGraphics (const std::unique_ptr<BaseGraphics>& G)
{
	_G = G.get ();

	_CreateDeviceTextureImage ();
	_CreateDescriptorPool ();
	_CreateDescriptorSetLayout ();
	_CreateDescriptorSet ();
	_CreateShaders ();
}

void _SplashSceneGraphics::_CreateDeviceTextureImage ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateDeviceTextureImage\n");

	std::string TexturePath = GraphicUtils::GetFullPath ("\\Images\\SplashScreen\\SplashScreen.tga");

	int Width; int Height; int Components;
	uint8_t* Pixels = stbi_load (TexturePath.c_str (), &Width, &Height, &Components, 4);

	vk::DeviceMemory StagingBufferMemory;
	vk::Buffer StagingBuffer;

	GraphicUtils::CreateBufferAndBufferMemory (_G, (vk::DeviceSize)(Width * Height * Components), vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, StagingBuffer, StagingBufferMemory);

	_G->GraphicsDevice.freeMemory (StagingBufferMemory);
	_G->GraphicsDevice.destroyBuffer (StagingBuffer);
}

void _SplashSceneGraphics::_CreateDescriptorPool ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateDescriptorPool\n");

	vk::DescriptorPoolSize DescriptorPoolSize (vk::DescriptorType::eCombinedImageSampler, 1);
	vk::DescriptorPoolCreateInfo DesriptorPoolCreateInfo ({}, _G->SwapchainImages.size (), 1, &DescriptorPoolSize);
	_DescriptorPool = _G->GraphicsDevice.createDescriptorPool (DesriptorPoolCreateInfo);
}

void _SplashSceneGraphics::_CreateDescriptorSetLayout ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateDescriptorSetLayout\n");

	vk::DescriptorSetLayoutBinding DescriptorSetLayoutBinding (0, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment);
	vk::DescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo ({}, 1, &DescriptorSetLayoutBinding);
	_DescriptorSetLayout = _G->GraphicsDevice.createDescriptorSetLayout (DescriptorSetLayoutCreateInfo);
}

void _SplashSceneGraphics::_CreateDescriptorSet ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateDescriptorSet\n");

	vk::DescriptorSetAllocateInfo DescriptorSetAllocateInfo (_DescriptorPool, 1, &_DescriptorSetLayout);
	_DescriptorSet = _G->GraphicsDevice.allocateDescriptorSets (DescriptorSetAllocateInfo).front ();

	vk::DescriptorImageInfo DescriptorImageInfo (_Sampler, _TextureImageView, vk::ImageLayout::eShaderReadOnlyOptimal);
	vk::WriteDescriptorSet WriteDescriptorSet (_DescriptorSet, 0, 0, 1, vk::DescriptorType::eCombinedImageSampler, &DescriptorImageInfo);

	vk::ArrayProxy<const vk::WriteDescriptorSet> WriteDescriptorSets (1, &WriteDescriptorSet);

	_G->GraphicsDevice.updateDescriptorSets (WriteDescriptorSets, nullptr);
}

void _SplashSceneGraphics::_CreateRenderPass ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateRenderPass\n");

	vk::AttachmentDescription ColorAttachment ({}, _G->ChosenSurfaceFormat.format, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
	vk::AttachmentReference ColorAttachmentReference (0, vk::ImageLayout::eColorAttachmentOptimal);
	vk::SubpassDescription ColorAttachmentSubpass ({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &ColorAttachmentReference);

	vk::RenderPassCreateInfo RenderPassCreateInfo ({}, 1, &ColorAttachment, 1, &ColorAttachmentSubpass);

	_RenderPass = _G->GraphicsDevice.createRenderPass (RenderPassCreateInfo);
}

void _SplashSceneGraphics::_CreateShaders ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateShaders\n");

	std::string VertFullFilePath = GraphicUtils::GetFullPath ("\\Shaders\\SplashScreen\\vert.spv");
}

void _SplashSceneGraphics::Draw (const std::unique_ptr<MeshEntity>& Background)
{
	Background->Draw ();
}

_SplashSceneGraphics::~_SplashSceneGraphics ()
{
	if (_DescriptorSetLayout != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyDescriptorSetLayout (_DescriptorSetLayout);
	}

	if (_DescriptorPool != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyDescriptorPool (_DescriptorPool);
	}
}
