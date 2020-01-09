#include "GLTFData.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

_GLTFData::_GLTFData (std::string FilePath)
{
	OutputDebugString (L"_GLTFData::_GLTFData\n");

	tinygltf::Model Model;
	tinygltf::TinyGLTF Loader;
	std::string Error; std::string Warning;

	if (!Loader.LoadASCIIFromFile (&Model, &Error, &Warning, FilePath))
	{
		throw std::runtime_error (Error);
	}

	for (auto Material : Model.materials)
	{
	}

	for (auto Texture : Model.textures)
	{
	}

	for (auto Image : Model.images)
	{
	}

	for (auto Node : Model.nodes)
	{
		if (Node.mesh > -1)
		{
			MeshNode MNode;

			for (auto GLTFPrimitive : Model.meshes[Node.mesh].primitives)
			{
				Primitive MPrimitive;

				if (GLTFPrimitive.indices > -1)
				{
					tinygltf::Accessor Accessor = Model.accessors[GLTFPrimitive.indices];
					tinygltf::BufferView BufferView = Model.bufferViews[Accessor.bufferView];
					tinygltf::Buffer Buffer = Model.buffers[BufferView.buffer];

					MPrimitive.IndexCount = Accessor.count;
					MPrimitive.IndexSize = BufferView.byteLength;

					if (Accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
					{
						uint8_t* Data = reinterpret_cast<uint8_t*> (&Buffer.data[Accessor.byteOffset + BufferView.byteOffset]);

						for (uint32_t i = 0; i < MPrimitive.IndexCount; i++)
						{
							MPrimitive.Indices.push_back (Data[i]);
						}
					}
					else if (Accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
					{
						uint16_t* Data = reinterpret_cast<uint16_t*> (&Buffer.data[Accessor.byteOffset + BufferView.byteOffset]);

						for (uint32_t i = 0; i < MPrimitive.IndexCount; i++)
						{
							MPrimitive.Indices.push_back (Data[i]);
						}
					}
					else if (Accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
					{
						uint32_t* Data = reinterpret_cast<uint32_t*>(&Buffer.data[Accessor.byteOffset + BufferView.byteOffset]);

						for (uint32_t i = 0; i < MPrimitive.IndexCount; i++)
						{
							MPrimitive.Indices.push_back (Data[i]);
						}
					}
				}

				if (GLTFPrimitive.attributes.find ("POSITION") != GLTFPrimitive.attributes.end ())
				{
					tinygltf::Accessor Accessor = Model.accessors[GLTFPrimitive.attributes.find ("POSITION")->second];
					tinygltf::BufferView BufferView = Model.bufferViews[Accessor.bufferView];
					tinygltf::Buffer Buffer = Model.buffers[BufferView.buffer];

					MPrimitive.PositionSize = BufferView.byteLength;

					float* Data = reinterpret_cast<float*>(&Buffer.data[Accessor.byteOffset + BufferView.byteOffset]);

					for (uint32_t i = 0; i < MPrimitive.PositionSize / sizeof(float); i++)
					{
						MPrimitive.Positions.push_back (Data[i]);
					}
				}
				
				if (GLTFPrimitive.attributes.find ("NORMAL") != GLTFPrimitive.attributes.end ())
				{
					tinygltf::Accessor Accessor = Model.accessors[GLTFPrimitive.attributes.find ("NORMAL")->second];
					tinygltf::BufferView BufferView = Model.bufferViews[Accessor.bufferView];
					tinygltf::Buffer Buffer = Model.buffers[BufferView.buffer];

					MPrimitive.NormalSize = BufferView.byteLength;

					float* Data = reinterpret_cast<float*>(&Buffer.data[Accessor.byteOffset + BufferView.byteOffset]);

					for (uint32_t i = 0; i < MPrimitive.NormalSize / sizeof(float); i++)
					{
						MPrimitive.Normals.push_back (Data[i]);
					}
				}
				
				if (GLTFPrimitive.attributes.find ("TEXCOORD_0") != GLTFPrimitive.attributes.end ())
				{
					tinygltf::Accessor Accessor = Model.accessors[GLTFPrimitive.attributes.find ("TEXCOORD_0")->second];
					tinygltf::BufferView BufferView = Model.bufferViews[Accessor.bufferView];
					tinygltf::Buffer Buffer = Model.buffers[BufferView.buffer];

					MPrimitive.UV0Size = BufferView.byteLength;

					if (Accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
					{
						uint8_t* Data = reinterpret_cast<uint8_t*>(&Buffer.data[Accessor.byteOffset + BufferView.byteOffset]);

						for (uint32_t i = 0; i < MPrimitive.UV0Size / sizeof (uint8_t); i++)
						{
							MPrimitive.UV0.push_back (Data[i]);
						}
					}
					else if (Accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
					{
						uint16_t* Data = reinterpret_cast<uint16_t*>(&Buffer.data[Accessor.byteOffset + BufferView.byteOffset]);

						for (uint32_t i = 0; i < MPrimitive.UV0Size / sizeof (uint16_t); i++)
						{
							MPrimitive.UV0.push_back (Data[i]);
						}
					}
					else if (Accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
					{
						float* Data = reinterpret_cast<float*>(&Buffer.data[Accessor.byteOffset + BufferView.byteOffset]);

						for (uint32_t i = 0; i < MPrimitive.UV0Size / sizeof (float); i++)
						{
							MPrimitive.UV0.push_back (Data[i]);
						}
					}
				}

				MNode.Primitives.push_back (MPrimitive);
			}
		}

		if (Node.camera > -1)
		{

		}

		if (Node.skin > -1)
		{

		}
	}
}