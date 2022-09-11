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

	TexVertex& operator += (const TexVertex other)
	{
		pos += other.pos;
		tc_pos += other.tc_pos;
		return *this;
	}
	TexVertex operator + (const TexVertex other) const
	{
		return TexVertex(*this) += other;
	}
	TexVertex& operator -= (const TexVertex other)
	{
		pos -= other.pos;
		tc_pos -= other.tc_pos;
		return *this;
	}
	TexVertex operator - (const TexVertex other) const
	{
		return TexVertex(*this) -= other;
	}
	TexVertex& operator *= (const TexVertex other)
	{
		pos *= other.pos;
		tc_pos *= other.tc_pos;
		return *this;
	}
	TexVertex& operator *= (const float num)
	{
		pos *= num;
		tc_pos *= num;
		return *this;
	}
	TexVertex operator * (const TexVertex other) const
	{
		return TexVertex(*this) *= other;
	}
	TexVertex operator * (const float num) const
	{
		return TexVertex(*this) *= num;
	}
	TexVertex& operator /= (const TexVertex other)
	{
		pos /= other.pos;
		tc_pos /= other.tc_pos;
		return *this;
	}
	TexVertex& operator /= (const float num)
	{
		pos /= num;
		tc_pos /= num;
		return *this;
	}
	TexVertex operator / (const TexVertex other) const
	{
		return TexVertex(*this) /= other;
	}
	TexVertex operator / (const float num) const
	{
		return TexVertex(*this) /= num;
	}
	// tc - texture
	Vec3 pos;
	Vec2 tc_pos;

};

