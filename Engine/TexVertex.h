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


	// tc - texture
	Vec3 pos;
	Vec2 tc_pos;

};

