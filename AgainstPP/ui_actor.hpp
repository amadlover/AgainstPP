#pragma once

#include "asset.hpp"

#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>

class ui_actor
{
public:
    ui_actor ();
    ui_actor (asset::mesh* mesh);
    ~ui_actor ();

    glm::mat4 transformation_matrix;

    VkDeviceSize transform_buffer_aligned_size;
    VkDeviceSize transform_buffer_aligned_offset;

    void* transform_buffer_data_ptr;

    asset::mesh* mesh;
};