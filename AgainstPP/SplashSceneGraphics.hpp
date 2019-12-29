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

	BaseGraphics* _G;

	VkDescriptorPool _DescriptorPool;
	VkDescriptorSetLayout _DescriptorSetLayout;
};

