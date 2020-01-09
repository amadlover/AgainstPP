#include "Scene.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

void Scene::LoadGLTFData (std::string FilePath)
{
	tinygltf::Model Model;
	tinygltf::TinyGLTF Loader;
	std::string Error; std::string Warning;

	if (!Loader.LoadASCIIFromFile (&Model, &Error, &Warning, FilePath))
	{
		throw std::runtime_error (Error);
	}
}
