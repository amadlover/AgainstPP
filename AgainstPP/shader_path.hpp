#pragma once

#include "material.hpp"
#include <memory>
#include <vector>

class shader_path
{
public:
    shader_path ();
    ~shader_path ();

    std::vector<std::unique_ptr<material>> materials;

    std::unique_ptr<shader> vertex_shader;
    std::unique_ptr<shader> fragment_shader;
};