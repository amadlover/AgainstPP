#pragma once

#include "graphics.hpp"
#include "asset.hpp"
#include "actor.hpp"

#include <memory>

class _SplashSceneGraphics
{
public:
	_SplashSceneGraphics (const std::unique_ptr<BaseGraphics>& G, std::string FilePath, std::vector<Asset>& Assets);
	~_SplashSceneGraphics ();

	void Draw ();

private:
	void _LoadGLTFData (std::string FilePath, std::vector<Asset>& Assets);

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
	vk::Buffer _VBIB;
	vk::Buffer _VBIBMemory;

	std::vector<vk::CommandBuffer> _SwapchainCommandBuffers;
	std::vector<vk::Framebuffer> _SwapchainFramebuffers;
	std::vector<vk::ShaderModule> _ShaderModules;
	std::vector<vk::PipelineShaderStageCreateInfo> _PipelineShaderStageCreateInfos;
	std::vector<vk::Fence> _SwapchainFences;
};

namespace splash_screen_graphics
{
	struct splash_screen_graphics
	{

	};
}