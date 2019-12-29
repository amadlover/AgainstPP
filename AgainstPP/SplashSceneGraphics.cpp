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

	/*VkDescriptorPoolSize DescriptorPoolSize = {};

	DescriptorPoolSize.descriptorCount = _G->SwapchainImageCount;
	DescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo = {};

	DescriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	DescriptorPoolCreateInfo.poolSizeCount = 1;
	DescriptorPoolCreateInfo.pPoolSizes = &DescriptorPoolSize;
	DescriptorPoolCreateInfo.maxSets = _G->SwapchainImageCount;

	if (vkCreateDescriptorPool (_G->GraphicsDevice, &DescriptorPoolCreateInfo, NULL, &_DescriptorPool) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_DESCRIPTOR_POOL;
	}*/
}

void _SplashSceneGraphics::_CreateDescriptorSetLayout ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateDescriptorSetLayout\n");

	VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding = {};

	DescriptorSetLayoutBinding.binding = 0;
	DescriptorSetLayoutBinding.descriptorCount = 1;
	DescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo = {};

	DescriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescriptorSetLayoutCreateInfo.bindingCount = 1;
	DescriptorSetLayoutCreateInfo.pBindings = &DescriptorSetLayoutBinding;

	if (vkCreateDescriptorSetLayout (_G->GraphicsDevice, &DescriptorSetLayoutCreateInfo, NULL, &_DescriptorSetLayout) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_DESCRIPTOR_SET_LAYOUT;
	}
}

void _SplashSceneGraphics::Draw (const std::unique_ptr<MeshEntity>& Background)
{
	Background->Draw ();
}

_SplashSceneGraphics::~_SplashSceneGraphics ()
{
	if (_DescriptorSetLayout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout (_G->GraphicsDevice, _DescriptorSetLayout, NULL);
	}

	if (_DescriptorPool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool (_G->GraphicsDevice, _DescriptorPool, NULL);
	}
}
