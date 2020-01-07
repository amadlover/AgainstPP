#pragma once

#include <string>
#include <Windows.h>

class _GLTFData
{
public:
	_GLTFData (std::string FilePath);
	~_GLTFData () { OutputDebugString (L"_GLTFData::~_GLTFData\n"); }
};

