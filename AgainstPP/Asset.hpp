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
	struct vk_texture
	{
		image::vk_image image;
	};

	struct vk_material
	{
		vk_texture texture;
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

		std::vector<uint32_t> indices;
	};

	struct vk_physics_primitive
	{
		vk::Buffer* buffer;
		vk::DeviceMemory* buffer_memory;

		vk::DeviceSize positions_offset;
		vk::DeviceSize normals_offset;
		vk::DeviceSize uv0s_offset;
		vk::DeviceSize uv1s_offset;
		vk::DeviceSize indices_offset;

		std::vector<uint32_t> indices;
	};

	struct vk_asset
	{
		std::vector<vk_graphics_primitive> graphics_primitives;
		std::vector<vk_physics_primitive> physics_primitives;
	};

	struct gltf_texture
	{
		std::string name;
		uint32_t gltf_image_index;
	};

	struct gltf_material
	{
		std::string name;
		gltf_texture base_color_texture;
	};

	struct gltf_graphics_primitive
	{
		std::vector<uint8_t> positions;
		std::vector<uint8_t> normals;
		std::vector<uint8_t> uv0s;
		std::vector<uint8_t> uv1s;

		std::vector<uint32_t> indices;

		gltf_material material;
	};

	struct gltf_physics_primitive
	{
		std::vector<uint8_t> positions;
		std::vector<uint32_t> indices;
	};

	struct gltf_asset
	{
		std::string name;
		uint32_t id;

		std::vector<gltf_graphics_primitive> graphics_primitves;
		std::vector<gltf_physics_primitive> physics_primitives;
	};

	void import_gltf_assets (
		const std::string& file_path, 
		std::vector<gltf_asset>& gltf_assets
	);

	void destroy_gltf_assets (
		std::vector<gltf_asset>& gltf_assets
	);
}