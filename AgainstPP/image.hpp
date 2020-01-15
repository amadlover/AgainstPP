#pragma once

#include <vulkan.hpp>

namespace image
{
	struct image
	{
	};

	void import_images (const std::string& file_path, std::vector<image>& images);
}
