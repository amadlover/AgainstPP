#include "SplashSceneGraphics.hpp"
#include "Error.hpp"
#include "GraphicUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_TGA
#include <stb_image.h>

_SplashSceneGraphics::_SplashSceneGraphics (const std::unique_ptr<BaseGraphics>& G)
{
	_G = G.get ();

	_CreateCommandBuffers ();
	_CreateDeviceTextureImage ();
	_CreateDescriptorPool ();
	_CreateDescriptorSetLayout ();
	_CreateDescriptorSet ();
	_CreateShaders ();
	_CreateRenderPass ();
	_CreateFramebuffers ();
}

void _SplashSceneGraphics::_CreateCommandBuffers ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateCommandBuffers\n");

	vk::CommandPoolCreateInfo CommandPoolCreateInfo ({}, _G->GraphicsQueueFamilies[0]);
	_CommandPool = _G->GraphicsDevice.createCommandPool (CommandPoolCreateInfo);

	const vk::CommandBufferAllocateInfo CommandBufferAllocateInfo (_CommandPool, vk::CommandBufferLevel::ePrimary, _G->SwapchainImages.size ());
	SwapchainCommandBuffers = _G->GraphicsDevice.allocateCommandBuffers (CommandBufferAllocateInfo);
}

void _SplashSceneGraphics::_CreateDeviceTextureImage ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateDeviceTextureImage\n");

	vk::DeviceMemory StagingBufferMemory;
	vk::Buffer StagingBuffer;

	std::string TexturePath = GraphicUtils::GetFullPath ("\\Images\\SplashScreen\\SplashScreen.tga");

	int Width; int Height; int Components;
	uint8_t* Pixels = stbi_load (TexturePath.c_str (), &Width, &Height, &Components, 4);

	GraphicUtils::CreateBufferAndBufferMemory (_G, (vk::DeviceSize)(Width * Height * Components), vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, StagingBuffer, StagingBufferMemory);
	HANDLE Data = _G->GraphicsDevice.mapMemory (StagingBufferMemory, 0, (vk::DeviceSize)(Width * Height * Components));
	memcpy (Data, Pixels, (Width * Height * Components));
	_G->GraphicsDevice.unmapMemory (StagingBufferMemory);

	GraphicUtils::CreateImageAndImageMemory (_G, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm, vk::Extent3D (Width, Height, 1), 1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::ImageLayout::eUndefined, vk::MemoryPropertyFlagBits::eDeviceLocal, _TextureImage, _TextureImageMemory);

	GraphicUtils::ChangeImageLayout (_G, _CommandPool, _TextureImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, vk::AccessFlagBits::eTransferWrite);
	GraphicUtils::CopyBufferToImage (_G, _CommandPool, StagingBuffer, _TextureImage, vk::Extent3D (Width, Height, 1), vk::ImageLayout::eTransferDstOptimal);
	GraphicUtils::ChangeImageLayout (_G, _CommandPool, _TextureImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eShaderRead);

	_G->GraphicsDevice.freeMemory (StagingBufferMemory);
	_G->GraphicsDevice.destroyBuffer (StagingBuffer);

	vk::SamplerCreateInfo SamplerCreateInfo ({}, vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, 0, VK_FALSE, 0, VK_FALSE, {}, {}, {}, vk::BorderColor::eFloatOpaqueBlack);
	_TextureSampler = _G->GraphicsDevice.createSampler (SamplerCreateInfo);

	vk::ImageSubresourceRange ImageSubresourceRange (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	vk::ImageViewCreateInfo ImageViewCreateInfo ({}, _TextureImage, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Unorm, vk::ComponentSwizzle::eIdentity, ImageSubresourceRange);
	_TextureImageView = _G->GraphicsDevice.createImageView (ImageViewCreateInfo);
}

void _SplashSceneGraphics::_CreateFramebuffers ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateFramebuffers\n");

	vk::FramebufferCreateInfo FramebufferCreateInfo ({}, _RenderPass, 1, {}, _G->SurfaceExtent.width, _G->SurfaceExtent.height, 1);

	for (auto& SwapchainImageView : _G->SwapchainImageViews)
	{
		FramebufferCreateInfo.setPAttachments (&SwapchainImageView);

		SwapchainFramebuffers.push_back (_G->GraphicsDevice.createFramebuffer (FramebufferCreateInfo));
	}
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

	vk::DescriptorImageInfo DescriptorImageInfo (_TextureSampler, _TextureImageView, vk::ImageLayout::eShaderReadOnlyOptimal);
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

	PipelineShaderStageCreateInfos = std::vector<vk::PipelineShaderStageCreateInfo> (2);

	std::string VertFilePath = GraphicUtils::GetFullPath ("\\Shaders\\SplashScreen\\vert.spv");
	std::string FragFilePath = GraphicUtils::GetFullPath ("\\Shaders\\SplashScreen\\frag.spv");

	GraphicUtils::CreateShader (_G->GraphicsDevice, VertFilePath, vk::ShaderStageFlagBits::eVertex, PipelineShaderStageCreateInfos);
	GraphicUtils::CreateShader (_G->GraphicsDevice, FragFilePath, vk::ShaderStageFlagBits::eFragment, PipelineShaderStageCreateInfos);
}

void _SplashSceneGraphics::Draw (const std::unique_ptr<MeshEntity>& Background)
{
	Background->Draw ();
}

_SplashSceneGraphics::~_SplashSceneGraphics ()
{
	if (_RenderPass != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyRenderPass (_RenderPass);
	}

	for (auto SwapchainFramebuffer : SwapchainFramebuffers)
	{
		if (SwapchainFramebuffer != VK_NULL_HANDLE)
		{
			_G->GraphicsDevice.destroyFramebuffer (SwapchainFramebuffer);
		}
	}

	if (_TextureSampler != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroySampler (_TextureSampler);
	}

	_G->GraphicsDevice.freeCommandBuffers (_CommandPool, SwapchainCommandBuffers);	
	
	if (_CommandPool != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyCommandPool (_CommandPool);
	}

	if (_TextureImageMemory != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.freeMemory (_TextureImageMemory);
	}

	if (_TextureImage != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyImage (_TextureImage);
	}

	if (_TextureImageView != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyImageView (_TextureImageView);
	}

	if (_DescriptorSetLayout != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyDescriptorSetLayout (_DescriptorSetLayout);
	}

	if (_DescriptorPool != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyDescriptorPool (_DescriptorPool);
	}
}
