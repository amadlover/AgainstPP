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
	GraphicUtils::CreateBufferAndBufferMemory (_G, (ImageMemorySize + VertexIndexMemorySize), vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, StagingBuffer, StagingBufferMemory);

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

	GraphicUtils::CreateBufferAndBufferMemory (_G, (vk::DeviceSize)(Width * Height * Components), vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, StagingBuffer, StagingBufferMemory);
	HANDLE Data = _G->GraphicsDevice.mapMemory (StagingBufferMemory, 0, (vk::DeviceSize)(Width * Height * Components));
	memcpy (Data, Pixels, (Width * Height * Components));
	_G->GraphicsDevice.unmapMemory (StagingBufferMemory);

	GraphicUtils::CreateImageAndImageMemory (_G, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm, vk::Extent3D (Width, Height, 1), 1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::SharingMode::eExclusive, _G->GraphicsQueueFamilies, vk::ImageLayout::eUndefined, vk::MemoryPropertyFlagBits::eDeviceLocal, _TextureImage, _TextureImageMemory);

	GraphicUtils::ChangeImageLayout (_G, _CommandPool, _TextureImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, vk::AccessFlagBits::eTransferWrite);
	GraphicUtils::CopyBufferToImage (_G, _CommandPool, StagingBuffer, _TextureImage, vk::Extent3D (Width, Height, 1), vk::ImageLayout::eTransferDstOptimal);
	GraphicUtils::ChangeImageLayout (_G, _CommandPool, _TextureImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eShaderRead);

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

	GraphicUtils::CreateShader (_G->GraphicsDevice, VertFilePath, vk::ShaderStageFlagBits::eVertex, _ShaderModules[0], _PipelineShaderStageCreateInfos[0]);
	GraphicUtils::CreateShader (_G->GraphicsDevice, FragFilePath, vk::ShaderStageFlagBits::eFragment, _ShaderModules[1], _PipelineShaderStageCreateInfos[1]);
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
	std::unique_ptr<splash_screen_graphics> splash_screen_graphics_obj_ptr (new splash_screen_graphics ());
	common_graphics::common_graphics* common_graphics_obj_ptr;

	void check_for_similar_image_indices (
		uint32_t from_index, 
		const image::gltf_image& current_image, 
		const std::vector<image::gltf_image>& gltf_images, 
		std::vector<uint32_t>& similar_image_indices
	)
	{
		while (from_index < gltf_images.size ())
		{
			if (current_image.width == gltf_images.at (from_index).width &&
				current_image.height == gltf_images.at (from_index).height)
			{
				similar_image_indices.push_back (from_index);
			}

			++from_index;
		}
	}

	void create_vk_images (const std::vector<image::gltf_image>& gltf_images)
	{
		std::map<vk::Image, std::vector<uint32_t>> vkimage_similarindices;

		for (uint32_t index = 0; index < gltf_images.size(); index++)
		{
			bool is_index_already_processed = false;
			for (const auto& vk_image_similar_indices : vkimage_similarindices)
			{
				for (const auto& processed_index : vk_image_similar_indices.second)
				{
					if (processed_index == index)
					{
						is_index_already_processed = true;
						break;
					}
				}
				if (is_index_already_processed)
				{
					break;
				}
			}

			if (is_index_already_processed) 
			{
				continue;
			}

			image::gltf_image image = gltf_images.at (index);
			std::vector<uint32_t> similar_images_indices;
			similar_images_indices.reserve (gltf_images.size ());
			similar_images_indices.push_back (index);
			
			uint32_t start_index = index + 1;
			check_for_similar_image_indices (
				start_index,
				image,
				gltf_images,
				similar_images_indices
			);

			vk::DeviceSize similar_image_size = 0;

			for (const auto& similar_index : similar_images_indices)
			{
				const image::gltf_image similar_image = gltf_images.at (similar_index);
				similar_image_size += similar_image.pixels.size ();
			}

			vk::Extent3D image_extent{
				(uint32_t)gltf_images.at (similar_images_indices.at (0)).width,
				(uint32_t)gltf_images.at (similar_images_indices.at (0)).height,
				(uint32_t)1
			};

			vk::Image vkimage = graphics_utils::create_image (
				image_extent,
				similar_images_indices.size (),
				vk::Format::eR8G8B8A8Unorm,
				vk::ImageLayout::eUndefined,
				vk::ImageUsageFlagBits::eSampled,
				vk::SharingMode::eExclusive
			);

			vkimage_similarindices.insert (std::make_pair (vkimage, similar_images_indices));
		}
		
		std::vector<vk::Image> images;

		for (auto vk_image_similar_indices : vkimage_similarindices)
		{
			images.push_back (vk_image_similar_indices.first);
		}

		splash_screen_graphics_obj_ptr->image_memory = graphics_utils::allocate_bind_image_memory (images, vk::MemoryPropertyFlagBits::eDeviceLocal);
		splash_screen_graphics_obj_ptr->images = images;

		for (const auto& vk_image_similar_indices : vkimage_similarindices)
		{
			vk::ImageSubresourceRange image_subresource_range (
				vk::ImageAspectFlagBits::eColor, 
				0, 
				1, 
				0, 
				1
			);

			vk::ImageViewCreateInfo image_view_create_info (
				{},
				vk_image_similar_indices.first,
				vk::ImageViewType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				{
					vk::ComponentSwizzle::eIdentity,
					vk::ComponentSwizzle::eIdentity,
					vk::ComponentSwizzle::eIdentity,
					vk::ComponentSwizzle::eIdentity
				},
				{}
			);

			uint32_t similar_index_counter = 0;
			for (const auto& similar_index : vk_image_similar_indices.second)
			{
				image_subresource_range.baseArrayLayer = similar_index_counter;
				image_view_create_info.subresourceRange = image_subresource_range;

				vk::ImageView image_view = common_graphics_obj_ptr->graphics_device.createImageView (image_view_create_info);

				for (auto& vk_mesh : splash_screen_graphics_obj_ptr->vk_meshes)
				{
					for (auto& graphics_primitive : vk_mesh.graphics_primitives)
					{
						if (graphics_primitive.material.base_color_texture.vk_image_view_index == similar_index)
						{
							graphics_primitive.material.base_color_texture.image_view = &image_view;
						}
					}
				}

				splash_screen_graphics_obj_ptr->image_views.push_back (image_view);
				++similar_index_counter;
			}
		}
	}

	void map_mesh_data (
		vk::DeviceMemory& device_memory, 
		vk::DeviceSize& offset, 
		vk::DeviceSize& data_offset_for_memory, 
		const std::vector<uint8_t>& data
	)
	{
		graphics_utils::map_data_to_buffer (device_memory, offset, data.size (), data.data ());
		data_offset_for_memory = offset;
		offset += data.size ();
	}

	void create_vk_meshes (const std::vector<mesh::gltf_mesh>& gltf_meshes)
	{
		vk::DeviceSize total_size = 0;

		for (const auto& mesh : gltf_meshes)
		{
			for (const auto& graphics_primitive : mesh.graphics_primitves)
			{
				total_size += (vk::DeviceSize)graphics_primitive.positions.size () + 
							(vk::DeviceSize)graphics_primitive.normals.size () + 
							(vk::DeviceSize)graphics_primitive.uv0s.size () + 
							(vk::DeviceSize)graphics_primitive.uv1s.size () + 
							(vk::DeviceSize)(graphics_primitive.indices.size ());
			}
		}

		vk::Buffer staging_buffer = graphics_utils::create_buffer (total_size, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive);
		vk::DeviceMemory staging_buffer_memory = graphics_utils::allocate_bind_buffer_memory (staging_buffer, vk::MemoryPropertyFlagBits::eHostVisible);

		splash_screen_graphics_obj_ptr->vk_meshes.reserve (gltf_meshes.size ());
		vk::DeviceSize offset = 0;

		for (const auto& mesh : gltf_meshes)
		{
			mesh::vk_mesh vk_mesh = {};
			vk_mesh.graphics_primitives.reserve (mesh.graphics_primitves.size ());
			
			for (const auto& graphics_primitive : mesh.graphics_primitves)
			{
				mesh::vk_graphics_primitive vk_graphics_primitive = {};
			
				if (graphics_primitive.positions.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory, 
						offset, 
						vk_graphics_primitive.positions_offset, 
						graphics_primitive.positions
					);
				}

				if (graphics_primitive.normals.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory, 
						offset, 
						vk_graphics_primitive.normals_offset, 
						graphics_primitive.normals
					);
				}

				if (graphics_primitive.uv0s.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory, 
						offset, 
						vk_graphics_primitive.uv0s_offset, 
						graphics_primitive.uv0s
					);
				}

				if (graphics_primitive.uv1s.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory, 
						offset, 
						vk_graphics_primitive.uv1s_offset, 
						graphics_primitive.uv1s
					);
				}

				if (graphics_primitive.indices.size () > 0)
				{
					map_mesh_data (
						staging_buffer_memory, 
						offset, 
						vk_graphics_primitive.indices_offset, 
						graphics_primitive.indices
					);
				}

				vk_graphics_primitive.indices_type = graphics_primitive.indices_type;
				vk_graphics_primitive.indices = graphics_primitive.indices;
				vk_mesh.graphics_primitives.push_back (vk_graphics_primitive);
			}

			splash_screen_graphics_obj_ptr->vk_meshes.push_back (vk_mesh);
		}

		splash_screen_graphics_obj_ptr->vertex_index_buffer = graphics_utils::create_buffer (
			total_size, 
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer, 
			vk::SharingMode::eExclusive
		);

		splash_screen_graphics_obj_ptr->vertex_index_buffer_memory = graphics_utils::allocate_bind_buffer_memory (
			splash_screen_graphics_obj_ptr->vertex_index_buffer, 
			vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		graphics_utils::copy_buffer_to_buffer (
			common_graphics_obj_ptr->command_pool, 
			staging_buffer, 
			splash_screen_graphics_obj_ptr->vertex_index_buffer, 
			total_size
		);

		uint32_t mesh_counter = 0;
		for (auto& vk_mesh : splash_screen_graphics_obj_ptr->vk_meshes)
		{
			uint32_t primitive_counter = 0;
			for (auto& graphics_primitive : vk_mesh.graphics_primitives)
			{
				graphics_primitive.buffer = &splash_screen_graphics_obj_ptr->vertex_index_buffer;
				graphics_primitive.buffer_memory = &splash_screen_graphics_obj_ptr->vertex_index_buffer_memory;

				graphics_primitive.material.base_color_texture.vk_image_view_index = gltf_meshes.at (mesh_counter).graphics_primitves.at (primitive_counter).material.base_color_texture.gltf_image_index;
			
				++primitive_counter;
			}
			++mesh_counter;
		}

		common_graphics_obj_ptr->graphics_device.destroyBuffer (staging_buffer);
		common_graphics_obj_ptr->graphics_device.freeMemory (staging_buffer_memory);
	}

	void init (
		std::vector<mesh::gltf_mesh>& gltf_meshes, 
		std::vector<image::gltf_image>& gltf_images, 
		common_graphics::common_graphics* ptr
	)
	{
		OutputDebugString (L"splash_screen_graphics::init\n");
		common_graphics_obj_ptr = ptr;

		create_vk_meshes (gltf_meshes);
		create_vk_images (gltf_images);

		gltf_images.clear ();
		gltf_meshes.clear ();
	}

	void run ()
	{
	}

	void exit ()
	{
		OutputDebugString (L"splash_screen_graphics::exit\n");

		if (splash_screen_graphics_obj_ptr->vertex_index_buffer != VK_NULL_HANDLE)
		{
			common_graphics_obj_ptr->graphics_device.destroyBuffer (splash_screen_graphics_obj_ptr->vertex_index_buffer);
		}

		if (splash_screen_graphics_obj_ptr->vertex_index_buffer_memory != VK_NULL_HANDLE)
		{
			common_graphics_obj_ptr->graphics_device.freeMemory (splash_screen_graphics_obj_ptr->vertex_index_buffer_memory);
		}

		for (auto& image_view : splash_screen_graphics_obj_ptr->image_views)
		{
			if (image_view != VK_NULL_HANDLE)
			{
				common_graphics_obj_ptr->graphics_device.destroyImageView (image_view);
			}
		}

		for (auto& image : splash_screen_graphics_obj_ptr->images)
		{
			if (image != VK_NULL_HANDLE)
			{
				common_graphics_obj_ptr->graphics_device.destroyImage (image);
			}
		}

		if (splash_screen_graphics_obj_ptr->image_memory != VK_NULL_HANDLE)
		{
			common_graphics_obj_ptr->graphics_device.freeMemory (splash_screen_graphics_obj_ptr->image_memory);
		}

		splash_screen_graphics_obj_ptr->images.clear ();
		splash_screen_graphics_obj_ptr->image_views.clear ();
		splash_screen_graphics_obj_ptr->vk_images.clear ();
		splash_screen_graphics_obj_ptr->vk_meshes.clear ();
	}
}