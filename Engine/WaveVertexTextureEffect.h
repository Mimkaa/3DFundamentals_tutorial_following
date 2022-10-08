#pragma once

#include "Pipeline.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
// basic texture effect
class WaveVertexTextureEffect
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
			pos(pos),
			n(src.n)
		{}
		Vertex(const Vec3& pos, const Vec2& t, const Vec3& n)
			:
			t(t),
			pos(pos),
			n(n)
		{}
	public:
		Vec3 pos;
		Vec2 t;
		Vec3 n;
	};
	// calculate color based on normal to light angle
	// no interpolation of color attribute
	class VertexShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output(const Vec4& pos)
				:
				pos(pos)
			{}
			Output(const Vec4& pos, const Output& src)
				:
				n(src.n),
				worldPos(src.worldPos),
				pos(pos),
				t(src.t),
				nend(src.nend)
			{}
			Output(const Vec4& pos, const Vec3& n, const Vec3& worldPos, const Vec2& t, const Vec4& nend)
				:
				worldPos(worldPos),
				n(n),
				pos(pos),
				t(t),
				nend(nend)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				n += rhs.n;
				worldPos += rhs.worldPos;
				t += rhs.t;
				nend += rhs.nend;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				n -= rhs.n;
				worldPos -= rhs.worldPos;
				t -= rhs.t;
				nend -= rhs.nend;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				n *= rhs;
				worldPos *= rhs;
				t *= rhs;
				nend *= rhs;
				return *this;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				n /= rhs;
				worldPos /= rhs;
				t /= rhs;
				nend /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec4 pos;
			// pristine word position
			Vec3 worldPos;
			Vec3 n;
			Vec2 t;
			Vec4 nend;
		};
	public:
		void BindWorldView(const Mat4& transformation_in)
		{

			worldView = transformation_in;
			worldViewProj = worldView * proj;

		}
		void BindProjection(const Mat4& transformation_in)
		{
			proj = transformation_in;

		}
		const Mat4& GetProj() const
		{
			return proj;
		}
		void SetTime(float time_in)
		{
			time = time_in;
		}


		Output operator()(const Vertex& v) const
		{
			// apply transformations to world space
			auto p4 = Vec4(v.pos);
			p4.y += amplitude * sinf(time * WaveLength + p4.x * freqWave);
			const auto angle = wrap_angle(p4.x * freqWave + time * WaveLength);
			const auto sinx = std::sin(angle);
			const auto cosx = std::cos(angle);
			const auto n = Vec4{
				-freqWave * amplitude * cosx,
				1.0f,
				0.0f,
				0.0f
			};
			const auto nend = p4 + n * 0.02f;
			/*auto normal = Vec4{ v.n,0.0f };
			normal.y += amplitude * sinf(time * WaveLength + p4.x * freqWave);
			normal /= normal.Len();*/
			return { p4 * worldViewProj,n * worldView,p4 * worldView, v.t,nend * worldViewProj };
		}

	private:

		Mat4 proj = Mat4::Identity();
		Mat4 worldView = Mat4::Identity();
		Mat4 worldViewProj = Mat4::Identity();
		float time = 0.0f;
		float freqWave = 10.0f;
		float WaveLength = 5.0f;
		float amplitude = 0.05f;

	};
	// default gs passes vertices through and outputs triangle
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
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
			unsigned int tx = (unsigned int)std::fmod(std::max(in.t.x * tex_width, 0.0f), tex_xclamp);
			unsigned int ty = (unsigned int)std::fmod(std::max(in.t.y * tex_height, 0.0f), tex_yclamp);

			Color pixColor = pTex->GetPixel(
				tx,
				ty
			);
			auto norm_backup = in.n.GetNormalized();
			// get light direction
			const auto v_to_l = light_pos - in.worldPos;
			const auto dist = v_to_l.Len() / scale;
			// opposite of what it is supposed to be (light dir)
			//not to multiply by minus normal
			const auto LightDir = v_to_l / dist;
			// calculate attenuation
			const float attenuation = 1.0f /
				(constant_attenuation + linear_attenuation * dist + quadratic_attenuation * sq(dist));
			// calculate intensity based on angle of incidence
			const auto d = light_diffuse * attenuation * std::max(0.0f, norm_backup * LightDir);

			// specular highlight
			const auto w = norm_backup * (v_to_l * norm_backup);
			// opposite direction of the reflection
			const auto r = w * 2 - v_to_l;
			//specular intensity between view vector an the reflection
			const auto s = light_diffuse * specular_intensity * std::pow(std::max(0.0f, -r.GetNormalized() * in.worldPos.GetNormalized()), specular_power);

			// add diffuse+ambient, filter by material color, saturate and scalead
			Vec3 material_color = (Vec3)pixColor / 255.0f;
			const auto c = material_color.GetHadamard(d + light_ambient + s).Saturate() * 255.0f;
			return Color(c);
		}
		void BindTexture(const std::wstring& filename)
		{
			pTex = std::make_unique<Surface>(Surface::FromFile(filename));
			tex_width = float(pTex->GetWidth());
			tex_height = float(pTex->GetHeight());
			tex_xclamp = tex_width - 1.0f;
			tex_yclamp = tex_height - 1.0f;
		}
		void SetDiffuseLight(const Vec3& c)
		{
			light_diffuse = { c.x,c.y,c.z };
		}
		void SetAmbientLight(const Vec3& c)
		{
			light_ambient = { c.x,c.y,c.z };
		}
		void SetLightPosition(const Vec3& dl)
		{
			assert(dl.LenSq() >= 0.001f);
			light_pos = dl;
		}
		void SetScale(float s)
		{
			assert(s >= 0.001f);
			{
				scale = s;
			}
		}


	private:
		std::unique_ptr<Surface> pTex;
		float tex_width;
		float tex_height;
		float tex_xclamp;
		float tex_yclamp;

		Vec3 light_pos = { 0.0f,0.0f, 0.5f };
		// color of direct light
		Vec3 light_diffuse = { 1.0f,1.0f,1.0f };
		// indirect light
		Vec3 light_ambient = { 0.1f,0.1f,0.1f };

		float linear_attenuation = 1.0f;
		float quadratic_attenuation = 1.619f;
		float constant_attenuation = 0.382f;

		float specular_power = 30.0f;
		float specular_intensity = 0.6f;
		float scale = 1.0f;
	};
public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;
};

