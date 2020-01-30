#pragma once

template <typename T>
class template_event
{
public:
	template_event () {}
	~template_event () {}

	void add_bindings (T binding)
	{
		bindings.push_back (binding);
	}

	void broadcast ()
	{
		for (const auto& binding : bindings)
		{
			wchar_t Buff[64];
			swprintf (Buff, 64, L"Executing %hs\n", binding.c_str ());
			OutputDebugString (Buff);
		}
	}

private:
	typename std::vector<T> bindings;
};