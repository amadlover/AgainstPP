#include "image.hpp"

#include "tiny_gltf.h"

namespace image
{
	void import_gltf_images (
		const std::string& file_path, 
		std::vector<gltf_image>& gltf_images
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
			gltf_image tmp_image;

			tmp_image.name = model_image.name;
			tmp_image.width = model_image.width;
			tmp_image.height = model_image.height;
			tmp_image.bpp = model_image.bits / 8;
			tmp_image.pixels = model_image.image;

			gltf_images.push_back (tmp_image);
		}
	}

	void destroy_gltf_images (
		std::vector<gltf_image>& gltf_images
	)
	{
		gltf_images.clear ();
	}
}