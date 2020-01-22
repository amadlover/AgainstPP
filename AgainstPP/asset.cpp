#include "asset.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

namespace asset
{
	namespace mesh
	{
		void import_attributes (
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
		}

		void import_attributes (
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
		}

		void import_indices (
			const tinygltf::Model& model,
			const tinygltf::Primitive& primitive,
			physics_primitive& physics_primitive
		)
		{
			auto accessor = model.accessors[primitive.indices];
			auto buffer_view = model.bufferViews[accessor.bufferView];

			if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
			{
				physics_primitive.indices_type = vk::IndexType::eUint8EXT;
				/*const uint8_t* data_start = reinterpret_cast<const uint8_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

				for (uint32_t i = 0; i < accessor.count; i++)
				{
					graphics_primitive.indices.push_back (data_start[i]);
				}*/
			}
			else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
			{
				physics_primitive.indices_type = vk::IndexType::eUint16;
				/*const uint16_t* data_start = reinterpret_cast<const uint16_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

				for (uint32_t i = 0; i < accessor.count; i++)
				{
					graphics_primitive.indices.push_back (data_start[i]);
				}*/
			}
			else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
			{
				physics_primitive.indices_type = vk::IndexType::eUint32;
				/*const uint32_t* data_start = reinterpret_cast<const uint32_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

				for (uint32_t i = 0; i < accessor.count; i++)
				{
					graphics_primitive.indices.push_back (data_start[i]);
				}*/
			}

			for (uint32_t i = 0; i < buffer_view.byteLength; i++)
			{
				uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
				physics_primitive.indices.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
			}
		}

		void import_indices (
			const tinygltf::Model& model,
			const tinygltf::Primitive& primitive,
			graphics_primitive& graphics_primitive
		)
		{
			auto accessor = model.accessors[primitive.indices];
			auto buffer_view = model.bufferViews[accessor.bufferView];

			if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
			{
				graphics_primitive.indices_type = vk::IndexType::eUint8EXT;
				/*const uint8_t* data_start = reinterpret_cast<const uint8_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

				for (uint32_t i = 0; i < accessor.count; i++)
				{
					graphics_primitive.indices.push_back (data_start[i]);
				}*/
			}
			else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
			{
				graphics_primitive.indices_type = vk::IndexType::eUint16;
				/*const uint16_t* data_start = reinterpret_cast<const uint16_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

				for (uint32_t i = 0; i < accessor.count; i++)
				{
					graphics_primitive.indices.push_back (data_start[i]);
				}*/
			}
			else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
			{
				graphics_primitive.indices_type = vk::IndexType::eUint32;
				/*const uint32_t* data_start = reinterpret_cast<const uint32_t*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

				for (uint32_t i = 0; i < accessor.count; i++)
				{
					graphics_primitive.indices.push_back (data_start[i]);
				}*/
			}

			for (uint32_t i = 0; i < buffer_view.byteLength; i++)
			{
				uint32_t offset_index = accessor.byteOffset + buffer_view.byteOffset + i;
				graphics_primitive.indices.push_back (model.buffers[buffer_view.buffer].data[offset_index]);
			}
		}

		void import_materials (
			const tinygltf::Model& model,
			const tinygltf::Primitive& primitive,
			graphics_primitive& tmp_graphics_primitive
		)
		{
			tmp_graphics_primitive.material.name = model.materials[primitive.material].name;
			tmp_graphics_primitive.material.base_color_texture.name = model.textures[model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index].name;
			tmp_graphics_primitive.material.base_color_texture.image_index = model.textures[model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index].source;
		}

		void import_graphics_primitives (
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
		}

		void import_physics_primitive (
			const tinygltf::Model& model,
			std::vector<asset::mesh::mesh>& meshes
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
						asset::mesh::physics_primitive tmp_primitive;

						import_attributes (model, node_mesh_primitive, tmp_primitive);
						import_indices (model, node_mesh_primitive, tmp_primitive);

						current_mesh.physics_primitives.push_back (tmp_primitive);
					}
				}
			}
		}

		void import_meshes (
			const std::string& file_path,
			std::vector<asset::mesh::mesh>& meshes
		)
		{
			tinygltf::Model model;
			tinygltf::TinyGLTF loader;

			std::string error; std::string warning;

			if (!loader.LoadASCIIFromFile (&model, &error, &warning, file_path))
			{
				throw std::runtime_error (error);
			}

			import_graphics_primitives (model, meshes);
			import_physics_primitive (model, meshes);
		}

		void destroy_meshes (
			std::vector<asset::mesh::mesh>& meshes
		)
		{
			meshes.clear ();
		}
	}

	namespace image
	{
		void import_images (
			const std::string& file_path,
			std::vector<image>& images
		)
		{
			tinygltf::Model model;
			tinygltf::TinyGLTF loader;

			std::string error; std::string warning;

			if (!loader.LoadASCIIFromFile (&model, &error, &warning, file_path))
			{
				throw std::runtime_error (error);
			}

			for (const auto& model_image : model.images)
			{
				image tmp_image;

				tmp_image.name = model_image.name;
				tmp_image.width = model_image.width;
				tmp_image.height = model_image.height;
				tmp_image.bpp = model_image.bits / 8;
				tmp_image.pixels = model_image.image;

				images.push_back (tmp_image);
			}
		}

		void destroy_images (
			std::vector<image>& images
		)
		{
			images.clear ();
		}
	}
}