#include "utils.hpp"
#include "mesh.hpp"

#include <Windows.h>
#include <Shlwapi.h>
#include <locale>
#include <codecvt>

#include <tiny_gltf.h>

std::string utils::get_full_path (std::string relative_file_path)
{
	TCHAR path[MAX_PATH];
	HMODULE Module = GetModuleHandle (NULL);
	GetModuleFileName (Module, path, MAX_PATH);
	PathRemoveFileSpec (path);

	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	return converter.to_bytes (std::wstring (path)) + relative_file_path;
}

egraphics_result utils::import_gltf (std::string relative_file_path, std::vector<mesh>& meshes)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;

	std::string error, warning;

	if (!loader.LoadASCIIFromFile (&model, &error, &warning, utils::get_full_path (relative_file_path)))
	{
		egraphics_result::e_against_error_gltf_import;
	}

	return egraphics_result::success;
}