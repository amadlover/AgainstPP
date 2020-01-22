#pragma once

#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

#include "image.hpp"

struct PrimitivePhysicsData
{
};

struct PrimitiveGraphicsData
{
};

class Asset
{
public:
	Asset ();
	~Asset ();
	
	std::vector<PrimitiveGraphicsData> PrimitiveGraphicsDatas;
	std::vector<PrimitivePhysicsData> PrimitivePhysicsDatas;

	glm::mat4 TransformationMatrix;

	glm::vec3 Translation;
	glm::quat Rotation;
	glm::vec3 Scale;

	std::string Name;
};

namespace asset
{
	namespace mesh
	{

		struct vk_texture
		{
			uint32_t vk_image_view_index;
			vk::ImageView image_view;
			vk::DescriptorSet descriptor_set;
		};

		struct vk_material
		{
			vk_texture base_color_texture;
		};

		struct vk_graphics_primitive
		{
			vk::Buffer* buffer;
			vk::DeviceMemory* buffer_memory;

			vk::DeviceSize positions_offset;
			vk::DeviceSize normals_offset;
			vk::DeviceSize uv0s_offset;
			vk::DeviceSize uv1s_offset;
			vk::DeviceSize indices_offset;

			std::vector<uint8_t> indices;
			vk::IndexType indices_type;

			vk_material material;
		};

		struct vk_physics_primitive
		{
			vk::Buffer* buffer;
			vk::DeviceMemory* buffer_memory;

			vk::DeviceSize positions_offset;
			vk::DeviceSize indices_offset;

			std::vector<uint32_t> indices;
		};

		struct vk_mesh
		{
			std::vector<vk_graphics_primitive> graphics_primitives;
			std::vector<vk_physics_primitive> physics_primitives;
		};

		struct texture
		{
			std::string name;
			uint32_t image_index;
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
			vk::IndexType indices_type;
			material material;

			vk::Buffer* buffer;
			vk::DeviceMemory* buffer_memory;

			vk::DeviceSize positions_offset;
			vk::DeviceSize normals_offset;
			vk::DeviceSize uv0s_offset;
			vk::DeviceSize uv1s_offset;
			vk::DeviceSize indices_offset;
		};

		struct physics_primitive
		{
			std::vector<uint8_t> positions;
			std::vector<uint8_t> indices;
			vk::IndexType indices_type;

			vk::Buffer* buffer;
			vk::DeviceMemory* buffer_memory;

			vk::DeviceSize positions_offset;
			vk::DeviceSize indices_offset;
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
		struct vk_image
		{
			vk::ImageView* image_view;
			vk::DescriptorSet* descriptor_set;
		};

		struct image
		{
			std::string name;

			vk::DeviceSize width;
			vk::DeviceSize height;
			vk::DeviceSize bpp;

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