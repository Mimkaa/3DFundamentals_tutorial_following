#pragma once

template<class V>
class Triangle
{
public:
	Triangle(const V& v0i, const V& v1i, const V& v2i)
	{
		v0 = v0i;
		v1 = v1i;
		v2 = v2i;
	}
	V v0;
	V v1;
	V v2;
};