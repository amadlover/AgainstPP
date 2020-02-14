#pragma once

#include "enums.hpp"
#include <vector>
#include <functional>
#include <Windows.h>

#include <functional>

class event_no_param
{
public:
	event_no_param () {}
	~event_no_param () {}

	void add_binding (std::function<void ()> binding)
	{
		bindings.push_back (binding);
	}

	void broadcast ()
	{
		for (auto binding : bindings)
		{
			binding ();
		}
	}

private:
	struct object_function_ptr_no_param
	{
		object_function_ptr_no_param (std::function<void ()> b, size_t id)
		{
			function_ptr = b;
			obj_id = id;
		}

		std::function<void ()> function_ptr;
		size_t obj_id;
	};
	typename std::vector<std::function<void ()>> bindings;
};

template <typename U>
class event_one_param
{
public:
	event_one_param () {}
	~event_one_param () {}

	void add_binding (std::function<void (U)> binding, size_t id)
	{
		bindings.push_back (object_function_ptr_one_param<U> (binding, id));
	}

	void broadcast (U u)
	{
		for (auto binding : bindings)
		{
			binding.function_ptr (u);
		}
	}

	void remove_binding (size_t id)
	{
		auto it = std::find_if (bindings.begin (), bindings.end (), [&](auto b) {return b.obj_id == id; });
		if (it != bindings.end ()) 
		{ 
			bindings.erase (it); 
		}
	}

private:
	template <typename U>
	struct object_function_ptr_one_param
	{
		object_function_ptr_one_param (std::function<void (U)> b, size_t id)
		{
			function_ptr = b;
			obj_id = id;
		}

		std::function<void (U)> function_ptr;
		size_t obj_id;
	};
	typename std::vector<object_function_ptr_one_param<U>> bindings;
};

template <typename U, typename V>
class event_two_param
{
public:
	event_two_param () {}
	~event_two_param () {}

	void add_binding (std::function<void (U, V)> binding, size_t id)
	{
		bindings.push_back (object_function_ptr_two_param <U, V> (binding, id));
	}

	void broadcast (U u, V v)
	{
		for (auto binding : bindings)
		{
			binding.function_ptr (u, v);
		}
	}

	void remove_binding (size_t id)
	{
		auto it = std::find_if (bindings.begin (), bindings.end (), [&](auto b) {return b.obj_id == id; });
		
		if (it != bindings.end ())
		{
			bindings.erase (it);
		}
	}

private:
	template <typename U, typename V>
	struct object_function_ptr_two_param
	{
		object_function_ptr_two_param (std::function<void (U, V)> b, size_t id)
		{
			function_ptr = b;
			obj_id = id;
		}

		std::function<void (U, V)> function_ptr;
		size_t obj_id;
	};
	typename std::vector<object_function_ptr_two_param<U, V>> bindings;
};