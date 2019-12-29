#include "SplashSceneGraphics.hpp"
#include "Error.hpp"

_SplashSceneGraphics::_SplashSceneGraphics (const std::unique_ptr<BaseGraphics>& G)
{
	_G = G.get ();

	_CreateDescriptorPool ();
	_CreateDescriptorSetLayout ();
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
