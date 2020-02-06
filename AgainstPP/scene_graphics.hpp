#pragma once

#include "error.hpp"
#include "asset.hpp"
#include <vector>
#include <vulkan/vulkan.h>

class scene_graphics
{
public:
	scene_graphics ();
	~scene_graphics ();

	virtual egraphics_result init (std::vector<asset::mesh>& meshes);
	virtual egraphics_result draw (const std::vector<asset::mesh>& meshes) const;
	virtual void exit (std::vector<asset::mesh>& meshes);

protected:
    virtual egraphics_result create_renderpass ();
	virtual egraphics_result create_framebuffers ();
	virtual egraphics_result create_shaders ();
	virtual egraphics_result create_descriptor_sets (std::vector<asset::mesh>& meshes);
	virtual egraphics_result create_graphics_pipeline_layout ();
	virtual egraphics_result create_graphics_pipeline ();
	virtual egraphics_result update_command_buffers (const std::vector<asset::mesh>& meshes);

	VkRenderPass render_pass;
	std::vector<VkFramebuffer> swapchain_framebuffers;
	VkPipelineLayout graphics_pipeline_layout;
	VkPipeline graphics_pipeline;
	std::vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stage_create_infos;
	VkShaderModule vertex_shader_module;
	VkShaderModule fragment_shader_module;
	VkDescriptorPool descriptor_pool;
	std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
};