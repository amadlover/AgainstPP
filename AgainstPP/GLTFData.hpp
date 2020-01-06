#pragma once

#include <string>
#include <Windows.h>

class _GLTFData
{
public:
	_GLTFData (std::string FilePath) { OutputDebugString (L"_GLTFData::_GLTFData\n"); }
	~_GLTFData () { OutputDebugString (L"_GLTFData::~_GLTFData\n"); }
};

