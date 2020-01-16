#include "splash_screen_graphics.hpp"
#include "Error.hpp"
#include "Utils.hpp"
#include "graphics_utils.hpp"
#include "glm/vec2.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

_SplashSceneGraphics::_SplashSceneGraphics (const std::unique_ptr<BaseGraphics>& G, std::string FilePath, std::vector<Asset>& Assets)
{
	_G = G.get ();

	_LoadGLTFData (FilePath, Assets);

	_CreateCommandPool ();
	_CreateDeviceTextureImage ();
	_CreateDescriptorPool ();
	_CreateDescriptorSetLayout ();
	_CreateDescriptorSet ();
	_CreateShaders ();
	_CreateRenderPass ();
	_CreateFramebuffers ();
	_CreateGraphicsPipeline ();
	_CreateVBIB ();
	_CreateCommandBuffers ();
	_CreateSyncObjects ();
}

void _SplashSceneGraphics::_LoadGLTFData (std::string FilePath, std::vector<Asset>& Assets)
{
	OutputDebugString (L"_SplashSceneGraphics::_LoadGLTFData\n");

	tinygltf::Model Model;
	tinygltf::TinyGLTF Loader;

	std::string Error; std::string Warning;

	if (!Loader.LoadASCIIFromFile (&Model, &Error, &Warning, FilePath))
	{
		throw std::runtime_error (Error);
	}

	vk::DeviceSize ImageMemorySize = 0;
	vk::DeviceSize VertexIndexMemorySize = 0;

	for (auto Image : Model.images)
	{
		ImageMemorySize += (vk::DeviceSize)Image.width * (vk::DeviceSize)Image.height * ((vk::DeviceSize)Image.bits / 8);
	}

	for (auto Node : Model.nodes)
	{
		if (Node.name.find ("CS_") != std::string::npos)
		{
			continue;
		}

		if (Node.mesh > -1)
		{
			auto Mesh = Model.meshes[Node.mesh];

			for (auto Primitive : Mesh.primitives)
			{
				VertexIndexMemorySize += Model.bufferViews[Model.accessors[Primitive.indices].bufferView].byteLength;

				for (auto Attribute : Primitive.attributes)
				{
					VertexIndexMemorySize += Model.bufferViews[Model.accessors[Attribute.second].bufferView].byteLength;
				}
			}
		}
	}

	vk::Buffer StagingBuffer; vk::DeviceMemory StagingBufferMemory;
	graphics_utils::create_buffer_and_buffer_memory (_G, (ImageMemorySize + VertexIndexMemorySize), vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, StagingBuffer, StagingBufferMemory);

	_G->GraphicsDevice.destroyBuffer (StagingBuffer);
	_G->GraphicsDevice.freeMemory (StagingBufferMemory);
}

void _SplashSceneGraphics::_CreateCommandPool ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateCommandPool\n");

	vk::CommandPoolCreateInfo CommandPoolCreateInfo ({}, _G->GraphicsQueueFamilies[0]);
	_CommandPool = _G->GraphicsDevice.createCommandPool (CommandPoolCreateInfo);
}

void _SplashSceneGraphics::_CreateDeviceTextureImage ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateDeviceTextureImage\n");

	vk::DeviceMemory StagingBufferMemory;
	vk::Buffer StagingBuffer;

	std::string TexturePath = utils::get_full_path ("\\Images\\SplashScreen\\SplashScreen.tga");

	int Width; int Height; int Components;
	uint8_t* Pixels = stbi_load (TexturePath.c_str (), &Width, &Height, &Components, 4);

	graphics_utils::create_buffer_and_buffer_memory (_G, (vk::DeviceSize)(Width * Height * Components), vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, StagingBuffer, StagingBufferMemory);
	HANDLE Data = _G->GraphicsDevice.mapMemory (StagingBufferMemory, 0, (vk::DeviceSize)(Width * Height * Components));
	memcpy (Data, Pixels, (Width * Height * Components));
	_G->GraphicsDevice.unmapMemory (StagingBufferMemory);

	graphics_utils::CreateImageAndImageMemory (_G, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm, vk::Extent3D (Width, Height, 1), 1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::ImageLayout::eUndefined, vk::MemoryPropertyFlagBits::eDeviceLocal, _TextureImage, _TextureImageMemory);

	graphics_utils::ChangeImageLayout (_G, _CommandPool, _TextureImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, vk::AccessFlagBits::eTransferWrite);
	graphics_utils::CopyBufferToImage (_G, _CommandPool, StagingBuffer, _TextureImage, vk::Extent3D (Width, Height, 1), vk::ImageLayout::eTransferDstOptimal);
	graphics_utils::ChangeImageLayout (_G, _CommandPool, _TextureImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eShaderRead);

	_G->GraphicsDevice.destroyBuffer (StagingBuffer);
	_G->GraphicsDevice.freeMemory (StagingBufferMemory);

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

		_SwapchainFramebuffers.push_back (_G->GraphicsDevice.createFramebuffer (FramebufferCreateInfo));
	}
}

void _SplashSceneGraphics::_CreateGraphicsPipeline ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateGraphicsPipeline\n");

	vk::PipelineLayoutCreateInfo PipelineLayoutCreateInfo ({}, 1, &_DescriptorSetLayout);
	_GraphicsPipelineLayout = _G->GraphicsDevice.createPipelineLayout (PipelineLayoutCreateInfo);

	std::vector<vk::VertexInputBindingDescription> VertexInputBindingDescriptions (2);
	VertexInputBindingDescriptions[0] = vk::VertexInputBindingDescription (0, sizeof (glm::vec2), vk::VertexInputRate::eVertex);
	VertexInputBindingDescriptions[1] = vk::VertexInputBindingDescription (1, sizeof (glm::vec2), vk::VertexInputRate::eVertex);

	std::vector<vk::VertexInputAttributeDescription> VertexInputAttributeDescriptions (2);
	VertexInputAttributeDescriptions[0] = vk::VertexInputAttributeDescription (0, 0, vk::Format::eR32G32B32Sfloat);
	VertexInputAttributeDescriptions[1] = vk::VertexInputAttributeDescription (1, 1, vk::Format::eR32G32Sfloat);

	vk::PipelineVertexInputStateCreateInfo VertexInputStateCreateInfo ({}, VertexInputBindingDescriptions.size (), VertexInputBindingDescriptions.data (), VertexInputAttributeDescriptions.size (), VertexInputAttributeDescriptions.data ());
	vk::PipelineInputAssemblyStateCreateInfo InputAssemblyStateCreateInfo ({}, vk::PrimitiveTopology::eTriangleList, VK_FALSE);
	vk::PipelineRasterizationStateCreateInfo RasterizationStateCreateInfo ({}, 0, 0, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eCounterClockwise, {}, {}, {}, {}, 1);
	vk::PipelineColorBlendAttachmentState ColorBlendAttachmentState (0, {}, {}, {}, {}, {}, {}, vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
	vk::PipelineColorBlendStateCreateInfo ColorBlendStateCreateInfo ({}, 0, {}, 1, &ColorBlendAttachmentState, std::array<float, 4> {1, 1, 1, 1});
	vk::PipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo ({}, 1,1, vk::CompareOp::eLessOrEqual);

	vk::Viewport Viewport{ 0, 0, (float)_G->SurfaceExtent.width, (float)_G->SurfaceExtent.height, 0, 1 };
	vk::Rect2D Scissor{ vk::Offset2D{0,0}, vk::Extent2D{_G->SurfaceExtent.width, _G->SurfaceExtent.height} };
	vk::PipelineViewportStateCreateInfo ViewportStateCreateInfo ({}, 1, &Viewport, 1, &Scissor);

	vk::PipelineMultisampleStateCreateInfo MultiSampleStateCreateInfo ({}, vk::SampleCountFlagBits::e1);

	vk::GraphicsPipelineCreateInfo PipelineCreateInfo ({}, _PipelineShaderStageCreateInfos.size (), _PipelineShaderStageCreateInfos.data (), &VertexInputStateCreateInfo, &InputAssemblyStateCreateInfo, {}, &ViewportStateCreateInfo, &RasterizationStateCreateInfo, &MultiSampleStateCreateInfo, &DepthStencilStateCreateInfo, &ColorBlendStateCreateInfo, {}, _GraphicsPipelineLayout, _RenderPass);
	_GraphicsPipeline = _G->GraphicsDevice.createGraphicsPipeline ({}, PipelineCreateInfo);
}

void _SplashSceneGraphics::_CreateVBIB ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateVBIB\n");
}

void _SplashSceneGraphics::_CreateCommandBuffers ()
{
	OutputDebugString (L"_SplashSceneGraphics::_CreateCommandBuffers\n");

	const vk::CommandBufferAllocateInfo CommandBufferAllocateInfo (_CommandPool, vk::CommandBufferLevel::ePrimary, _G->SwapchainImages.size ());
	_SwapchainCommandBuffers = _G->GraphicsDevice.allocateCommandBuffers (CommandBufferAllocateInfo);

	vk::CommandBufferBeginInfo CommandBufferBeginInfo (vk::CommandBufferUsageFlagBits::eSimultaneousUse);
	vk::ClearValue ClearValues[2];
	ClearValues[0].color = vk::ClearColorValue (std::array<float, 4>{1, 0, 0, 1});
	ClearValues[1].depthStencil = vk::ClearDepthStencilValue (1, 0);

	vk::RenderPassBeginInfo RenderPassBeginInfo (_RenderPass, {}, vk::Rect2D (vk::Offset2D{ 0,0 }, _G->SurfaceExtent), 2, ClearValues);

	for (uint32_t i = 0; i < _SwapchainCommandBuffers.size (); i++)
	{
		RenderPassBeginInfo.setFramebuffer (_SwapchainFramebuffers[i]);
		_SwapchainCommandBuffers[i].begin (CommandBufferBeginInfo);
		_SwapchainCommandBuffers[i].beginRenderPass (RenderPassBeginInfo, vk::SubpassContents::eInline);

		_SwapchainCommandBuffers[i].endRenderPass ();
		_SwapchainCommandBuffers[i].end ();
	}
}

void _SplashSceneGraphics::_CreateSyncObjects ()
{
	_SignalSemaphore = _G->GraphicsDevice.createSemaphore (vk::SemaphoreCreateInfo ());
	_WaitSemaphore = _G->GraphicsDevice.createSemaphore (vk::SemaphoreCreateInfo ());

	vk::FenceCreateInfo FenceCreateInfo (vk::FenceCreateFlagBits::eSignaled);

	_SwapchainFences = std::vector<vk::Fence> (_SwapchainCommandBuffers.size ());

	for (auto& SwapchainFence : _SwapchainFences)
	{
		SwapchainFence = _G->GraphicsDevice.createFence (FenceCreateInfo);
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

	_PipelineShaderStageCreateInfos = std::vector<vk::PipelineShaderStageCreateInfo> (2);
	_ShaderModules = std::vector<vk::ShaderModule> (2);

	std::string VertFilePath = utils::get_full_path ("\\Shaders\\SplashScreen\\vert.spv");
	std::string FragFilePath = utils::get_full_path ("\\Shaders\\SplashScreen\\frag.spv");

	graphics_utils::CreateShader (_G->GraphicsDevice, VertFilePath, vk::ShaderStageFlagBits::eVertex, _ShaderModules[0], _PipelineShaderStageCreateInfos[0]);
	graphics_utils::CreateShader (_G->GraphicsDevice, FragFilePath, vk::ShaderStageFlagBits::eFragment, _ShaderModules[1], _PipelineShaderStageCreateInfos[1]);
}

void _SplashSceneGraphics::Draw ()
{
}

_SplashSceneGraphics::~_SplashSceneGraphics ()
{
	if (_SignalSemaphore != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroySemaphore (_SignalSemaphore);
	}

	if (_WaitSemaphore != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroySemaphore (_WaitSemaphore);
	}

	for (auto& SwapchainFence : _SwapchainFences)
	{
		if (SwapchainFence != VK_NULL_HANDLE)
		{
			_G->GraphicsDevice.destroyFence (SwapchainFence);
		}
	}

	for (auto ShaderModule : _ShaderModules)
	{
		if (ShaderModule != VK_NULL_HANDLE)
		{
			_G->GraphicsDevice.destroyShaderModule (ShaderModule);
		}
	}

	if (_GraphicsPipelineLayout != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyPipelineLayout (_GraphicsPipelineLayout);
	}

	if (_GraphicsPipeline != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyPipeline (_GraphicsPipeline);
	}

	if (_RenderPass != VK_NULL_HANDLE)
	{
		_G->GraphicsDevice.destroyRenderPass (_RenderPass);
	}

	for (auto& SwapchainFramebuffer : _SwapchainFramebuffers)
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

	_G->GraphicsDevice.freeCommandBuffers (_CommandPool, _SwapchainCommandBuffers);	
	
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

namespace splash_screen_graphics
{
	std::unique_ptr<splash_screen_graphics> graphics_ptr (new splash_screen_graphics ());

	void create_command_pool ()
	{

	}

	void init (const std::vector<asset::gltf_asset>& gltf_assets, const std::vector<image::gltf_image> &images)
	{
		OutputDebugString (L"splash_screen_graphics::init\n");
	}

	void run ()
	{
	}

	void exit ()
	{
		OutputDebugString (L"splash_screen_graphics::exit\n");
	}
}