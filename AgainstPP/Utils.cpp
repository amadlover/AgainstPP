#include "Utils.hpp"

#include <Windows.h>
#include <Shlwapi.h>
#include <locale>
#include <codecvt>

namespace utils
{
	std::string get_full_path (std::string PartialFilepath)
	{
		TCHAR Path[MAX_PATH];
		HMODULE Module = GetModuleHandle (NULL);
		GetModuleFileName (Module, Path, MAX_PATH);
		PathRemoveFileSpec (Path);

		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		return converter.to_bytes (std::wstring (Path)) + PartialFilepath;
	}
}