#include "Utils.hpp"

#include <Windows.h>
#include <Shlwapi.h>
#include <locale>
#include <codecvt>

std::string utils::get_full_path (std::string partial_file_path)
{
	TCHAR path[MAX_PATH];
	HMODULE Module = GetModuleHandle (NULL);
	GetModuleFileName (Module, path, MAX_PATH);
	PathRemoveFileSpec (path);

	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	return converter.to_bytes (std::wstring (path)) + partial_file_path;
}