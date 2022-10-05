#pragma once
#include "Vec2.h"
class MouseHandler
{
public:
	void Engage(const Vei2& v)
	{
		recorded = v;
		engaged = true;
	}
	void Release()
	{
		engaged = false;
	}
	bool Engaged()const
	{
		return engaged;
	}
	Vei2 Move(const Vei2& pos)
	{
		const auto delta = pos - recorded;
		recorded = pos;
		return delta;
	}
private :
	Vei2 recorded;
	bool engaged;
};