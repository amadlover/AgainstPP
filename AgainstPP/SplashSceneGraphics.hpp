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
	void _CreateCommandPool ();
	void _CreateDeviceTextureImage ();
	void _CreateFramebuffers ();
	void _CreateGraphicsPipeline ();
	void _CreateVBIB ();
	void _CreateCommandBuffers ();
	void _CreateSyncObjects ();

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
	vk::Pipeline _GraphicsPipeline;
	vk::PipelineLayout _GraphicsPipelineLayout;
	vk::Semaphore _SignalSemaphore;
	vk::Semaphore _WaitSemaphore;

	std::vector<vk::CommandBuffer> _SwapchainCommandBuffers;
	std::vector<vk::Framebuffer> _SwapchainFramebuffers;
	std::vector<vk::ShaderModule> _ShaderModules;
	std::vector<vk::PipelineShaderStageCreateInfo> _PipelineShaderStageCreateInfos;
	std::vector<vk::Fence> _SwapchainFences;
};

