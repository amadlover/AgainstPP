#include "SplashSceneGraphics.hpp"
#include "Error.hpp"

_SplashSceneGraphics::_SplashSceneGraphics (const std::unique_ptr<BaseGraphics>& G)
{
	_G = G.get ();

	_CreateDescriptorPool ();
	_CreateDescriptorSetLayout ();
	_CreateDescriptorSet ();
}

void _SplashSceneGraphics::_CreateDescriptorPool ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateDescriptorPool\n");

	vk::DescriptorPoolSize DescriptorPoolSize (vk::DescriptorType::eCombinedImageSampler, 1);
	vk::DescriptorPoolCreateInfo DesriptorPoolCreateInfo ({}, _G->SwapchainImageCount, 1, &DescriptorPoolSize);
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
