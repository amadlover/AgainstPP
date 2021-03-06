#include "asset.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"
#include <glm/gtc/type_ptr.hpp>	
#include <glm/gtx/quaternion.hpp>

namespace asset
{
	egraphics_result import_attributes (
		const tinygltf::Model& model,
		const tinygltf::Primitive& primitive,
		physics_primitive& physics_primitive
	)
	{
		auto position_attribute = primitive.attributes.find ("POSITION");
		if (position_attribute != primitive.attributes.end ())
		{
			auto accessor = model.accessors[position_attribute->second];
			auto buffer_view = model.bufferViews[accessor.bufferView];

			physics_primitive.positions.reserve (buffer_view.byteLength);
			for (uint32_t i = 0; i < buffer_view.byteLength; i++)
			{
				uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
				physics_primitive.positions.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
			}
		}

		return egraphics_result::success;
	}

	egraphics_result import_attributes (
		const tinygltf::Model& model,
		const tinygltf::Primitive& primitive,
		graphics_primitive& graphics_primitive
	)
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

		return egraphics_result::success;
	}

	egraphics_result import_indices (
		const tinygltf::Model& model,
		const tinygltf::Primitive& primitive,
		physics_primitive& physics_primitive
	)
	{
		auto accessor = model.accessors[primitive.indices];
		auto buffer_view = model.bufferViews[accessor.bufferView];

		physics_primitive.indices_count = accessor.count;
		physics_primitive.indices.reserve (buffer_view.byteLength);
		for (uint32_t i = 0; i < buffer_view.byteLength; i++)
		{
			uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
			physics_primitive.indices.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
		}

		if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
		{
			physics_primitive.indices_type = VK_INDEX_TYPE_UINT8_EXT;
		}
		else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
		{
			physics_primitive.indices_type = VK_INDEX_TYPE_UINT16;
		}
		else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
		{
			physics_primitive.indices_type = VK_INDEX_TYPE_UINT32;
		}

		return egraphics_result::success;
	}

	egraphics_result import_indices (
		const tinygltf::Model& model,
		const tinygltf::Primitive& primitive,
		graphics_primitive& graphics_primitive
	)
	{
		auto accessor = model.accessors[primitive.indices];
		auto buffer_view = model.bufferViews[accessor.bufferView];

		graphics_primitive.indices_count = accessor.count;
		graphics_primitive.indices.reserve (buffer_view.byteLength);
		for (uint32_t i = 0; i < buffer_view.byteLength; i++)
		{
			uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
			graphics_primitive.indices.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
		}

		if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
		{
			graphics_primitive.indices_type = VK_INDEX_TYPE_UINT8_EXT;
			/*const uint8_t* data_start = reinterpret_cast<const uint8_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

			for (uint32_t i = 0; i < accessor.count; i++)
			{
				graphics_primitive.indices.push_back (data_start[i]);
			}*/
		}
		else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
		{
			graphics_primitive.indices_type = VK_INDEX_TYPE_UINT16;
		}
		else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
		{
			graphics_primitive.indices_type = VK_INDEX_TYPE_UINT32;
		}

		return egraphics_result::success;
	}

	egraphics_result import_materials (
		const tinygltf::Model& model,
		const tinygltf::Primitive& primitive,
		graphics_primitive& tmp_graphics_primitive
	)
	{
		tmp_graphics_primitive.material.name = model.materials[primitive.material].name;
		tmp_graphics_primitive.material.base_color_texture.name = model.textures[model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index].name;
		auto model_image = model.images[model.textures[model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index].source];

		image tmp_image;

		tmp_image.name = model_image.name;
		tmp_image.width = model_image.width;
		tmp_image.height = model_image.height;
		tmp_image.bpp = model_image.bits / 8;
		tmp_image.pixels = model_image.image;

		tmp_graphics_primitive.material.base_color_texture.texture_image = tmp_image;

		return egraphics_result::success;
	}

	egraphics_result import_physics_primitive (
		const tinygltf::Model& model,
		std::vector<mesh>& meshes
	)
	{
		for (auto node : model.nodes)
		{
			if (node.mesh < 0)
			{
				continue;
			}

			if (node.name.find ("CS_") == std::string::npos)
			{
				continue;
			}

			char* name = (char*)(node.name.c_str ());
			char* first_ = strtok (name, "_");
			char* second = strtok (nullptr, "_");
			char* third = strtok (nullptr, "_");

			for (auto& current_mesh : meshes)
			{
				if (current_mesh.name.compare (second) != 0)
				{
					continue;
				}

				auto node_mesh = model.meshes[node.mesh];

				current_mesh.physics_primitives.reserve (node_mesh.primitives.size ());

				for (const auto& node_mesh_primitive : node_mesh.primitives)
				{
					physics_primitive tmp_primitive;

					import_attributes (model, node_mesh_primitive, tmp_primitive);
					import_indices (model, node_mesh_primitive, tmp_primitive);

					current_mesh.physics_primitives.push_back (tmp_primitive);
				}
			}
		}

		return egraphics_result::success;
	}

	egraphics_result import_graphics_primitives (
		const tinygltf::Model& model,
		std::vector<mesh>& meshes
	)
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

			mesh tmp_mesh;
			tmp_mesh.name = node.name;
			if (node.matrix.size () > 0)
			{
				tmp_mesh.transformation_matrix = glm::make_mat4 (node.matrix.data ());
			}
			else 
			{
				glm::mat4 transformation_matrix = glm::mat4 (1.f);
				if (node.translation.size () > 0)
				{
					glm::vec3 t = glm::make_vec3 (node.translation.data ());
					transformation_matrix = glm::translate (transformation_matrix, t);
				}
				
				if (node.rotation.size () > 0)
				{
					glm::quat q = glm::make_quat (node.rotation.data ());
					transformation_matrix *= glm::toMat4 (q);
				}

				if (node.scale.size () > 0) 
				{
					glm::vec3 s = glm::make_vec3 (node.scale.data ());
					transformation_matrix = glm::scale (transformation_matrix, s);
				}

				tmp_mesh.transformation_matrix = transformation_matrix;
			}

			auto asset = model.meshes[node.mesh];

			for (auto primitive : asset.primitives)
			{
				graphics_primitive tmp_graphics_primitive;

				import_attributes (model, primitive, tmp_graphics_primitive);
				import_materials (model, primitive, tmp_graphics_primitive);
				import_indices (model, primitive, tmp_graphics_primitive);

				tmp_mesh.graphics_primitves.push_back (tmp_graphics_primitive);
			}

			meshes.push_back (tmp_mesh);
		}

		return egraphics_result::success;
	}

	egraphics_result import_meshes (
		const std::string& file_path,
		std::vector<mesh>& meshes
	)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;

		std::string error; std::string warning;

		if (!loader.LoadASCIIFromFile (&model, &error, &warning, file_path))
		{
			return egraphics_result::e_against_error_gltf_import;
		}
		
		import_graphics_primitives (model, meshes);
		import_physics_primitive (model, meshes);

		return egraphics_result::success;
	}
}