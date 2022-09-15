#pragma once
#include "Vec2.h"
#include "Vec3.h"

class TexVertex
{
public:
	TexVertex(const Vec3 & v_pos, const Vec2 & vtc_pos) :pos(v_pos), tc_pos(vtc_pos) {};

	TexVertex InterpolateTo(const TexVertex & end_of_interp, float extend) const
	{
		return{
			pos.InterpolateTo(end_of_interp.pos, extend),
			tc_pos.InterpolateTo(end_of_interp.tc_pos, extend),
		};
	}

	TexVertex& operator+=(const TexVertex& rhs)
	{
		pos += rhs.pos;
		tc_pos += rhs.tc_pos;
		return *this;
	}
	TexVertex operator+(const TexVertex& rhs) const
	{
		return TexVertex(*this) += rhs;
	}
	TexVertex& operator-=(const TexVertex& rhs)
	{
		pos -= rhs.pos;
		tc_pos -= rhs.tc_pos;
		return *this;
	}
	TexVertex operator-(const TexVertex& rhs) const
	{
		return TexVertex(*this) -= rhs;
	}
	TexVertex& operator*=(float rhs)
	{
		pos *= rhs;
		tc_pos *= rhs;
		return *this;
	}
	TexVertex operator*(float rhs) const
	{
		return TexVertex(*this) *= rhs;
	}
	TexVertex& operator/=(float rhs)
	{
		pos /= rhs;
		tc_pos /= rhs;
		return *this;
	}
	TexVertex operator/(float rhs) const
	{
		return TexVertex(*this) /= rhs;
	}

	TexVertex& operator=(const TexVertex& rhs)
	{
		pos = rhs.pos;
		tc_pos = rhs.tc_pos;
		return *this;
	}
	// tc - texture
	Vec3 pos;
	Vec2 tc_pos;

};

