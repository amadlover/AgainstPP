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

			auto mesh = model.meshes[node.mesh];

			for (auto primitive : mesh.primitives)
			{
				gltf_graphics_primitve tmp_graphics_primitive;
			
				auto position_attribute = primitive.attributes.find ("POSITION");
				if (position_attribute != primitive.attributes.end ())
				{
					tmp_graphics_primitive.positions = model.buffers[model.bufferViews[model.accessors[position_attribute->second].bufferView].buffer].data;
				}

				auto normal_attribute = primitive.attributes.find ("NORMAL");
				if (normal_attribute != primitive.attributes.end ())
				{
					tmp_graphics_primitive.normals = model.buffers[model.bufferViews[model.accessors[normal_attribute->second].bufferView].buffer].data;
				}

				auto uv0_attribute = primitive.attributes.find ("TEXCOORD_0");
				if (uv0_attribute != primitive.attributes.end ())
				{
					tmp_graphics_primitive.uv0s = model.buffers[model.bufferViews[model.accessors[uv0_attribute->second].bufferView].buffer].data;
				}

				auto uv1_attribute = primitive.attributes.find ("TEXCOORD_1");
				if (uv1_attribute != primitive.attributes.end ())
				{
					tmp_graphics_primitive.uv1s = model.buffers[model.bufferViews[model.accessors[uv1_attribute->second].bufferView].buffer].data;
				}
			}
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