#pragma once

#include <cstdint>
#include "enums.hpp"

namespace event
{
	typedef void (*go_to_scene_fp)(e_scene_type scene);
	
	struct event
	{
		go_to_scene_fp go_to_scene;
	};
}
