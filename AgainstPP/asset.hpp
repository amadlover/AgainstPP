#pragma once

#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

#include <vulkan/vulkan.h>

namespace asset
{
	namespace mesh
	{
		struct texture
		{
			std::string name;
			uint32_t image_index;
			VkImageView image_view;
			VkDescriptorSet descriptor_set;
		};

		struct material
		{
			std::string name;
			texture base_color_texture;
		};

		struct graphics_primitive
		{
			std::vector<uint8_t> positions;
			std::vector<uint8_t> normals;
			std::vector<uint8_t> uv0s;
			std::vector<uint8_t> uv1s;
			std::vector<uint8_t> indices;
			VkIndexType indices_type;
			material material;

			VkBuffer* buffer;
			VkDeviceMemory* buffer_memory;

			VkDeviceSize positions_offset;
			VkDeviceSize normals_offset;
			VkDeviceSize uv0s_offset;
			VkDeviceSize uv1s_offset;
			VkDeviceSize indices_offset;
		};

		struct physics_primitive
		{
			std::vector<uint8_t> positions;
			std::vector<uint8_t> indices;
			VkIndexType indices_type;

			VkBuffer* buffer;
			VkDeviceMemory* buffer_memory;

			VkDeviceSize positions_offset;
			VkDeviceSize indices_offset;
		};

		struct mesh
		{
			std::string name;
			uint32_t id;

			std::vector<graphics_primitive> graphics_primitves;
			std::vector<physics_primitive> physics_primitives;
		};

		void import_meshes (
			const std::string& file_path,
			std::vector<mesh>& meshes
		);

		void destroy_meshes (
			std::vector<mesh>& meshes
		);
	}

	namespace image
	{
		struct image
		{
			std::string name;

			VkDeviceSize width;
			VkDeviceSize height;
			VkDeviceSize bpp;

			std::vector<uint8_t> pixels;
		};

		void import_images (
			const std::string& file_path,
			std::vector<image>& images
		);

		void destroy_images (
			std::vector<image>& images
		);
	}
}