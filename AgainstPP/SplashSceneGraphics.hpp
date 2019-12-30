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
	void _CreateDeviceTextureImage ();

	BaseGraphics* _G;

	vk::DescriptorPool _DescriptorPool;
	vk::DescriptorSetLayout _DescriptorSetLayout;
	vk::DescriptorSet _DescriptorSet;
	vk::Sampler _Sampler;
	vk::ImageView _TextureImageView;
	vk::RenderPass _RenderPass;
};

