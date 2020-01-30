#pragma once

#include "enums.hpp"
#include <functional>

/*namespace event
{
	typedef void (*go_to_scene_fp)(e_scene_type scene);

	struct event
	{
		go_to_scene_fp go_to_scene;
	};
}
*/

class event
{
public:
	event () { OutputDebugString (L"event::event\n"); }
	~event () { OutputDebugString (L"event::~event\n"); }

	std::function<void (e_scene_type)> go_to_scene;
};