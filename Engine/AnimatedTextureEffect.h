#pragma once

#include "Pipeline.h"

// basic texture effect
class AnimatedTextureEffect
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
			t(src.t),
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vec2& t)
			:
			t(t),
			pos(pos)
		{}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			t += rhs.t;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			t -= rhs.t;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs)
		{
			pos *= rhs;
			t *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			t /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec2 t;
	};
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
			return pTexFrames[current_tex]->GetPixel(
				(unsigned int)std::min(in.t.x * tex_width + 0.5f, tex_xclamp),
				(unsigned int)std::min(in.t.y * tex_height + 0.5f, tex_yclamp)
			);
		}
		
		void BindTextures(const std::vector<std::wstring>& filenames)
		{

			for (auto& name : filenames)
			{
				pTexFrames.emplace_back(std::make_unique<Surface>(Surface::FromFile(name)));	
			}
			tex_width = float(pTexFrames[0]->GetWidth());
			tex_height = float(pTexFrames[0]->GetHeight());
			tex_xclamp = tex_width - 1.0f;
			tex_yclamp = tex_height - 1.0f;
			current_tex = 0;
		}
	public:
		int current_tex;
	private:
		std::vector<std::unique_ptr<Surface>> pTexFrames;
		float tex_width;
		float tex_height;
		float tex_xclamp;
		float tex_yclamp;
		
	};
public:
	PixelShader ps;
	
};
