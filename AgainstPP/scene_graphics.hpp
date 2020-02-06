#pragma once

#include "error.hpp"
#include <vector>
#include <vulkan/vulkan.h>

class scene_graphics
{
public:
    scene_graphics () {}
    ~scene_graphics () {}

    egraphics_result create_basic_framebuffers (std::vector<VkFramebuffer>& swapchain_framebuffers, VkRenderPass* render_pass);
    egraphics_result create_basic_renderpass (VkRenderPass* render_pass);
};