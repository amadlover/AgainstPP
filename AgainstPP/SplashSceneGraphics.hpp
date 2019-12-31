#pragma once

#include "BaseGraphics.hpp"
#include "MeshEntity.hpp"

#include <memory>

class _SplashSceneGraphics
{
public:
	_SplashSceneGraphics (const std::unique_ptr<BaseGraphics>& G);
	~_SplashSceneGraphics ();

	void Draw (const std::unique_ptr<MeshEntity>& Background);

private:
	void _CreateDescriptorPool ();
	void _CreateDescriptorSetLayout ();
	void _CreateDescriptorSet ();
	void _CreateRenderPass ();
	void _CreateShaders ();
	void _CreateCommandBuffers ();
	void _CreateDeviceTextureImage ();
	void _CreateFramebuffers ();

	BaseGraphics* _G;

	vk::DescriptorPool _DescriptorPool;
	vk::DescriptorSetLayout _DescriptorSetLayout;
	vk::DescriptorSet _DescriptorSet;
	vk::CommandPool _CommandPool;
	vk::Sampler _TextureSampler;
	vk::Image _TextureImage;
	vk::DeviceMemory _TextureImageMemory;
	vk::ImageView _TextureImageView;
	vk::RenderPass _RenderPass;

	std::vector<vk::CommandBuffer> SwapchainCommandBuffers;
	std::vector<vk::Framebuffer> SwapchainFramebuffers;
	std::vector<vk::PipelineShaderStageCreateInfo> PipelineShaderStageCreateInfos;
};

