#pragma once

#include <vulkan.hpp>

namespace image
{
	struct vk_image
	{
		vk::Image* image;
		uint32_t layer_index;
		vk::ImageView* image_view;
		vk::DescriptorSet descriptor_set;
		vk::DeviceMemory* memory;
		VkDeviceSize offset;
	};

	struct gltf_image
	{
		std::string name;

		vk::DeviceSize width;
		vk::DeviceSize height;
		vk::DeviceSize bpp;

		std::vector<uint8_t> pixels;
	};

	void import_gltf_images (
		const std::string& file_path, 
		std::vector<gltf_image>& gltf_images
	);

	void destroy_gltf_images (
		std::vector<gltf_image>& gltf_images
	);
}
