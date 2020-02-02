#pragma once

template <typename T, size_t N>
class array
{
public:
	array ()
	{
		memset (elements, 0, sizeof (T) * N);
	}
	~array () {}

	T& operator[] (size_t i)
	{
		return elements[i];
	}

	T* data ()
	{
		return elements;
	}

	T* begin ()
	{
		return N > 0 ? elements : nullptr;
	}

	T* end ()
	{
		return N > 0 ? elements + (N - 1) : nullptr;
	}

private:
	T elements[N];
};