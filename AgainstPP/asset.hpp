#pragma once

#include "error.hpp"
#include "entity.hpp"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace asset
{
	struct image
	{
		image ()
		{
			width = 0;
			height = 0;
			bpp = 0;
			offset = 0;
			image_view = VK_NULL_HANDLE;
			skybox_descriptor_set = VK_NULL_HANDLE;
		}
		std::string name;

		VkDeviceSize width;
		VkDeviceSize height;
		VkDeviceSize bpp;

		std::vector<uint8_t> pixels;

		VkDeviceSize offset;

		VkImageView image_view;
		VkDescriptorSet skybox_descriptor_set;
	};

	struct texture
	{
		std::string name;
		image texture_image;
	};

	struct material
	{
		std::string name;
		texture base_color_texture;
	};

	struct graphics_primitive
	{
		graphics_primitive ()
		{
			positions_offset = 0;
			normals_offset = 0;
			uv0s_offset = 0;
			uv1s_offset = 0;
			indices_offset = 0;
			
			buffer = VK_NULL_HANDLE;
			buffer_memory = VK_NULL_HANDLE;
			indices_type = VK_INDEX_TYPE_UINT16;
			indices_count = 0;
		}

		std::vector<uint8_t> positions;
		std::vector<uint8_t> normals;
		std::vector<uint8_t> uv0s;
		std::vector<uint8_t> uv1s;
		std::vector<uint8_t> indices;
		VkIndexType indices_type;
		material material;

		uint32_t indices_count;

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
		physics_primitive ()
		{
			positions_offset = 0;
			indices_offset = 0;
			
			buffer = VK_NULL_HANDLE;
			buffer_memory = VK_NULL_HANDLE;
			indices_type = VK_INDEX_TYPE_UINT16;
			indices_count = 0;
		}
		std::vector<uint8_t> positions;
		std::vector<uint8_t> indices;
		VkIndexType indices_type;

		uint32_t indices_count;

		VkBuffer* buffer;
		VkDeviceMemory* buffer_memory;

		VkDeviceSize positions_offset;
		VkDeviceSize indices_offset;
	};

	enum mesh_type
	{
		skybox,
		ui_button,
		static_mesh
	};

	struct mesh : public entity
	{
		mesh () : entity () {}
		~mesh () {}
		std::string name;

		std::vector<graphics_primitive> graphics_primitves;
		std::vector<physics_primitive> physics_primitives;

		mesh_type type;
	};

	egraphics_result import_meshes (const std::string& file_path, std::vector<mesh>& meshes);
}