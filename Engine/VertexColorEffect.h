#pragma once

#include "Pipeline.h"
#include "DefaultVertexShader.h"
// basic texture effect
class VertexColorEffect
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
			color(src.color),
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vec3& t)
			:
			color(t),
			pos(pos)
		{}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			color += rhs.color;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			color -= rhs.color;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs)
		{
			pos *= rhs;
			color *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			color /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec3 color;
	};
	typedef DefaultVertexShader<Vertex> VertexShader;
	// invoked for each pixel of a triangle
	// takes an input of attributes that are the
	// result of interpolating vertex attributes
	// and outputs a color , (it is a functor(a class that acts like a function))
	class PixelShader
	{
	public:
		template<class Input>
		Color operator()(const Input& in) const
		{
			
			return Color(in.color);
			
		}
		void BindTexture(const std::wstring& filename)
		{
			pTex = std::make_unique<Surface>(Surface::FromFile(filename));
			tex_width = float(pTex->GetWidth());
			tex_height = float(pTex->GetHeight());
			tex_xclamp = tex_width - 1.0f;
			tex_yclamp = tex_height - 1.0f;
		}
	private:
		std::unique_ptr<Surface> pTex;
		float tex_width;
		float tex_height;
		float tex_xclamp;
		float tex_yclamp;
	};
public:
	PixelShader ps;
	VertexShader vs;
};

