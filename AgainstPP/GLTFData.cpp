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
}