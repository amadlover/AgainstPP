#pragma once

#include <vector>
#include <memory>

#include "shader_path.hpp"

class mesh
{
public:
    mesh ();
    ~mesh ();

    std::vector<std::unique_ptr<shader_path>> shader_paths;
};