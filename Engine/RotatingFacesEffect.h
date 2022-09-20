#pragma once

#include "Pipeline.h"
#include "DefaultVertexShader.h"
#include "Mat3.h"
// solid color attribute taken from table in gs and not interpolated
class RotatingFaceEffect
{
public:
	// the vertex type that will be input into the pipeline
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos)
			:
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vertex& src)
			:
			pos(pos)
		{}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs)
		{
			pos *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
	};
	// default vs rotates and translates vertices
	// does not touch attributes
	typedef DefaultVertexShader<Vertex> VertexShader;
	// gs colors vertices using their index from a table
	// every two triangles are colored from the same entry
	class GeometryShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3& pos)
				:
				pos(pos)
			{}
			Output(const Vec3& pos, const Output& src)
				:
				color(src.color),
				pos(pos)
			{}
			Output(const Vec3& pos, const Color& color)
				:
				color(color),
				pos(pos)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				return *this;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec3 pos;
			Color color;
		};
	public:
		Triangle<Output> operator()(const VertexShader::Output& in0, const VertexShader::Output& in1, const VertexShader::Output& in2, unsigned int triangle_index) const
		{
			std::vector<Vec3> result = Rotate(in0.pos, in1.pos, in2.pos, triangle_index);
			return{
				// deveided by 2 in order to perform int-division like ((int)4/(int)2 = 2, (int)5/(int)2=2)
				// rotate pos
				//Rotate(in0.pos,in1.pos,in2.pos, triangle_index);
				{ result[0],triangle_colors[triangle_index / 2]},
				{ result[1],triangle_colors[triangle_index / 2] },
				{ result[2],triangle_colors[triangle_index / 2] }
			};
		};
		void BindColors(std::vector<Color> colors)
		{
			triangle_colors = std::move(colors);
		}
	std::vector<Vec3> Rotate(const Vec3& v0ch, const Vec3& v1ch, const Vec3& v2ch, unsigned int triangle_index) const
	{
		Vec3 v0(v0ch);
		Vec3 v1(v1ch);
		Vec3 v2(v2ch);
		std::vector<Vec3> result;
		unsigned int tri_index = triangle_index / 2;
		result.insert(std::end(result), { v0, v1, v2 });
		Mat3 rot;
		const Mat3 unwind = Mat3::RotationZ(-theta_z) * Mat3::RotationY(-theta_y) * Mat3::RotationX(-theta_x);
		const Mat3 windBack = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		if (tri_index == 0 || tri_index == 1)
		{
			rot = Mat3::RotationZ(PI*2*time);
			v0 -= translation;
			v0 *= unwind * rot * windBack;
			v0 += translation;
			v1 -= translation;
			v1 *= unwind * rot * windBack;
			v1 += translation;
			v2 -= translation;
			v2 *= unwind * rot * windBack;
			v2 += translation;
			result[0] = v0;
			result[1] = v1;
			result[2] = v2;
			
		}
		else if (tri_index == 2 || tri_index == 3)
		{
			rot = Mat3::RotationX(PI * 2 * time);
			v0 -= translation;
			v0 *= unwind * rot * windBack;
			v0 += translation;
			v1 -= translation;
			v1 *= unwind * rot * windBack;
			v1 += translation;
			v2 -= translation;
			v2 *= unwind * rot * windBack;
			v2 += translation;
			result[0] = v0;
			result[1] = v1;
			result[2] = v2;
		}
		else if (tri_index == 4 || tri_index == 5)
		{
			rot = Mat3::RotationY(PI * 2 * time);
			v0 -= translation;
			v0 *= unwind * rot * windBack;
			v0 += translation;
			v1 -= translation;
			v1 *= unwind * rot * windBack;
			v1 += translation;
			v2 -= translation;
			v2 *= unwind * rot * windBack;
			v2 += translation;
			result[0] = v0;
			result[1] = v1;
			result[2] = v2;
		}
		return result;
	}
	void SetTime(float t)
	{
		time = t;
	}
	void SetRotation(Mat3 rot) 
	{
		rotation = rot;
	}
	void SetXRotAngle(float ang)
	{
		theta_x = ang;
	}
	void SetYRotAngle(float ang)
	{
		theta_y = ang;
	}
	void SetZRotAngle(float ang)
	{
		theta_z = ang;
	}
	void SetTranslation(const Vec3& trans)
	{
		translation = trans;
	}
	private:
		std::vector<Color> triangle_colors;
		float time = 0;
		Mat3 rotation;
		float theta_x = 0.0f;
		float theta_y = 0.0f;
		float theta_z = 0.0f;
		Vec3 translation;
	};
	// invoked for each pixel of a triangle
	// takes an input of attributes that are the
	// result of interpolating vertex attributes
	// and outputs a color
	class PixelShader
	{
	public:
		template<class Input>
		Color operator()(const Input& in) const
		{
			return in.color;
		}
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
