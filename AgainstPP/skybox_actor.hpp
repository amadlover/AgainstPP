#pragma once

#include "asset.hpp"

#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>

class skybox_actor
{
public:
    skybox_actor ();
    skybox_actor (asset::mesh* mesh);
    ~skybox_actor ();

    glm::mat4 transformation_matrix;

    VkDeviceSize transform_buffer_aligned_size;
    VkDeviceSize transform_buffer_aligned_offset;

    glm::mat4* transform_buffer_data_ptr;

    asset::mesh* mesh;
};