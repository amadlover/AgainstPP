#pragma once

#include <string>
#include <vector>

#include "mesh.hpp"
#include "error.hpp"

class utils
{
public:
	static std::string get_full_path (std::string relative_file_path);
	static egraphics_result import_gltf (std::string relative_file_path, std::vector<mesh>& meshes);
};