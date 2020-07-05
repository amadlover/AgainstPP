#pragma

#include "error.hpp"

#include <string>
#include <vulkan/vulkan.h>

class shader 
{
public:
    shader ();
    ~shader ();

    egraphics_result read_from_file (std::string file_name);
    VkShaderModule shader_module;
};