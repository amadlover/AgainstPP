#include "Asset.hpp"

#include "tiny_gltf.h"

Asset::Asset ()
{

}

Asset::~Asset ()
{

}

namespace asset
{
	void import_gltf_attributes (const tinygltf::Model& model, const tinygltf::Primitive& primitive, gltf_graphics_primitive& graphics_primitive)
	{
		auto position_attribute = primitive.attributes.find ("POSITION");
		if (position_attribute != primitive.attributes.end ())
		{
			auto accessor = model.accessors[position_attribute->second];
			auto buffer_view = model.bufferViews[accessor.bufferView];

			graphics_primitive.positions.reserve (buffer_view.byteLength);
			for (uint32_t i = 0; i < buffer_view.byteLength; i++)
			{
				uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
				graphics_primitive.positions.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
			}
		}

		auto normal_attribute = primitive.attributes.find ("NORMAL");
		if (normal_attribute != primitive.attributes.end ())
		{
			auto accessor = model.accessors[normal_attribute->second];
			auto buffer_view = model.bufferViews[accessor.bufferView];

			graphics_primitive.normals.reserve (buffer_view.byteLength);
			for (uint32_t i = 0; i < buffer_view.byteLength; i++)
			{
				uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
				graphics_primitive.normals.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
			}
		}

		auto uv0_attribute = primitive.attributes.find ("TEXCOORD_0");
		if (uv0_attribute != primitive.attributes.end ())
		{
			auto accessor = model.accessors[uv0_attribute->second];
			auto buffer_view = model.bufferViews[accessor.bufferView];

			graphics_primitive.uv0s.reserve (buffer_view.byteLength);
			for (uint32_t i = 0; i < buffer_view.byteLength; i++)
			{
				uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
				graphics_primitive.uv0s.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
			}
		}

		auto uv1_attribute = primitive.attributes.find ("TEXCOORD_1");
		if (uv1_attribute != primitive.attributes.end ())
		{
			auto accessor = model.accessors[uv1_attribute->second];
			auto buffer_view = model.bufferViews[accessor.bufferView];

			graphics_primitive.uv1s.reserve (buffer_view.byteLength);
			for (uint32_t i = 0; i < buffer_view.byteLength; i++)
			{
				uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
				graphics_primitive.uv1s.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
			}
		}
	}

	void import_gltf_indices (const tinygltf::Model& model, const tinygltf::Primitive& primitive, gltf_graphics_primitive& graphics_primitive)
	{
		auto accessor = model.accessors[primitive.indices];
		auto buffer_view = model.bufferViews[accessor.bufferView];

		if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* data_start = reinterpret_cast<const uint8_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

			for (uint32_t i = 0; i < accessor.count; i++)
			{
				graphics_primitive.indices.push_back (data_start[i]);
			}
		}
		else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* data_start = reinterpret_cast<const uint16_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

			for (uint32_t i = 0; i < accessor.count; i++)
			{
				graphics_primitive.indices.push_back (data_start[i]);
			}
		}
		else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
		{
			const uint32_t* data_start = reinterpret_cast<const uint32_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

			for (uint32_t i = 0; i < accessor.count; i++)
			{
				graphics_primitive.indices.push_back (data_start[i]);
			}
		}
	}

	void import_gltf_materials (const tinygltf::Model& model, const tinygltf::Primitive& primitive, gltf_graphics_primitive& tmp_graphics_primitive)
	{
		tmp_graphics_primitive.material.name = model.materials[primitive.material].name;
		tmp_graphics_primitive.material.base_color_texture.name = model.textures[model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index].name;
		tmp_graphics_primitive.material.base_color_texture.gltf_image_index = model.textures[model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index].source;
	}

	void import_gltf_graphics_primitives (const tinygltf::Model& model, std::vector<gltf_asset>& gltf_assets)
	{
		for (auto node : model.nodes)
		{
			if (node.mesh < 0)
			{
				continue;
			}

			if (node.name.find ("CS_") != std::string::npos)
			{
				continue;
			}

			gltf_asset tmp_asset;
			tmp_asset.name = node.name;

			auto mesh = model.meshes[node.mesh];

			for (auto primitive : mesh.primitives)
			{
				gltf_graphics_primitive tmp_graphics_primitive;
			
				import_gltf_attributes (model, primitive, tmp_graphics_primitive);
				import_gltf_materials (model, primitive, tmp_graphics_primitive);
				import_gltf_indices (model, primitive, tmp_graphics_primitive);
				
				tmp_asset.graphics_primitves.push_back (tmp_graphics_primitive);
			}

			gltf_assets.push_back (tmp_asset);
		}
	}

	void import_gltf_physics_primitive (const tinygltf::Model& model, std::vector<gltf_asset>& gltf_assets)
	{

	}

	void import_gltf_assets (
		const std::string& file_path, 
		std::vector<gltf_asset>& gltf_assets
	)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;

		std::string error; std::string warning;

		if (!loader.LoadASCIIFromFile (&model, &error, &warning, file_path))
		{
			throw std::runtime_error (error);
		}

		import_gltf_graphics_primitives (model, gltf_assets);
		import_gltf_physics_primitive (model, gltf_assets);
	}

	void destroy_gltf_assets (
		std::vector<gltf_asset>& gltf_assets
	)
	{
		gltf_assets.clear ();
	}
}