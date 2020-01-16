#include "main_menu.hpp"

#include <Windows.h>

namespace main_menu
{
	void init ()
	{
		OutputDebugString (L"main_menu::init\n");
	}

	void run ()
	{
		OutputDebugString (L"main_menu::run\n");
	}

	void exit ()
	{
		OutputDebugString (L"main_menu::exit\n");
	}
}

