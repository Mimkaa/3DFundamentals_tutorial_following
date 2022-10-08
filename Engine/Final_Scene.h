#pragma once
#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat.h"
#include "Pipeline.h"
#include "SpecularHighlightEffect.h"
#include "Sphere.h"
#include "SolidCubeEffect.h"
#include "MouseHandler.h"
#include "TextureEffect.h"
#include "Plane.h"
#include "WaveVertexTextureEffect.h"
#include "NormiePipe.h"

class FinalScene : public Scene
{
	template<class Vertex>
	class Wall
	{
	public:
		
		Wall(const Mat3& rotMat, const Vec3& offset_in, IndexedTriangleList<Vertex> it, float scale_in = 1.0)
		{
			offset = offset_in;
			rot_mat = rotMat;
			//itList(std::move(it));
			itList.vertices = it.vertices;
			itList.indices = it.indices;
			scale = scale_in;
			// tansform pos
			for (auto& v : itList.vertices)
			{
				v.pos *= scale;
				v.pos *= rotMat;
				v.pos += offset;
				v.n *= rotMat;
				


			}
		}
	public:
		IndexedTriangleList<Vertex> itList;
		Mat3 rot_mat;
		Vec3 offset;
		float scale;
	
	};
public:
	// :: was used to access the global scope 
	typedef ::Pipeline<SpecularHighlightEffect> Pipeline;
	typedef ::Pipeline<SolidEffect> PipelineLight;
	typedef ::Pipeline<TextureEffect> WallsPipeline;
	typedef ::Pipeline< WaveVertexTextureEffect> WavePipeline;
	typedef Pipeline::Vertex Vertex;
public:
	FinalScene(Graphics& gfx, IndexedTriangleList<Vertex> it)
		:
		pZb(std::make_shared<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		itlist(std::move(it)),
		pipeline(gfx, pZb),
		Lightpipeline(gfx, pZb),
		WallsPipeLine(gfx, pZb),
		wavePipe(gfx, pZb)
	{

		itlist.AdjustToTrueCenter();
		offset_z = itlist.GetRadius() * 1.6f;
		for (auto& v : itlistLightPoint.vertices)
		{
			v.color = Colors::White;
		}
		// init walls
		float padding = 5.0f;
		// back, front
		walls.push_back(Wall<TextureEffect::Vertex>(Mat3::RotationX(0.0f), Vec3(0.0f, 0.0f, padding), Plane::GetSkinnedNormals<TextureEffect::Vertex>(20), padding*2));
		walls.push_back(Wall<TextureEffect::Vertex>(Mat3::RotationX(0.0f), Vec3(0.0f, 0.0f, -padding), Plane::GetSkinnedNormals<TextureEffect::Vertex>(7, 1.0f, true), padding * 2));
		// top, bottom
		walls.push_back(Wall<TextureEffect::Vertex>(Mat3::RotationX(PI/2), Vec3(0.0f, -padding, 0.0f), Plane::GetSkinnedNormals<TextureEffect::Vertex>(), padding * 2));
		walls.push_back(Wall<TextureEffect::Vertex>(Mat3::RotationX(PI / 2), Vec3(0.0f, +padding, 0.0f), Plane::GetSkinnedNormals<TextureEffect::Vertex>(7,1.0f, true), padding * 2));
		// left right
		walls.push_back(Wall<TextureEffect::Vertex>(Mat3::RotationY(PI / 2), Vec3(-padding, 0.0f, 0.0f), Plane::GetSkinnedNormals<TextureEffect::Vertex>(7, 1.0f, true), padding * 2));
		walls.push_back(Wall<TextureEffect::Vertex>(Mat3::RotationY(PI / 2), Vec3(padding, 0.0f, 0.0f), Plane::GetSkinnedNormals<TextureEffect::Vertex>(7, 1.0f, false), padding * 2));

		// init wave
		Waveitlist = Plane::GetSkinnedNormals<WaveVertexTextureEffect::Vertex>(30, 1.0f, false);
		for (auto& v : Waveitlist.vertices)
		{
			v.pos *= Mat3::RotationX(PI / 2);
			v.n *= Mat3::RotationX(PI / 2);
		}
	}
	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override
	{

		if (kbd.KeyIsPressed('W'))
		{
			cam_pos += Vec4{ 0.0f,0.0f,1.0f,0.0f } *!cam_rot * cam_speed * dt;
		}
		if (kbd.KeyIsPressed('S'))
		{
			cam_pos += Vec4{ 0.0f,0.0f,-1.0f,0.0f } *!cam_rot * cam_speed * dt;
		}
		if (kbd.KeyIsPressed('D'))
		{
			cam_pos += Vec4{ 1.0f,0.0f,0.0f,0.0f } *!cam_rot * cam_speed * dt;
		}
		if (kbd.KeyIsPressed('A'))
		{
			cam_pos += Vec4{ -1.0f,0.0f,0.0f,0.0f } *!cam_rot * cam_speed * dt;
		}
		if (kbd.KeyIsPressed('R'))
		{
			cam_pos += Vec4{ 0.0f,1.0f,0.0f,0.0f } *!cam_rot * cam_speed * dt;
		}
		if (kbd.KeyIsPressed('F'))
		{
			cam_pos += Vec4{ 0.0f,-1.0f,0.0f,0.0f } *!cam_rot * cam_speed * dt;
		}
		// mouse events
		while (!mouse.IsEmpty())
		{
			const auto e = mouse.Read();
			switch (e.GetType())
			{
			case Mouse::Event::Type::LPress:
				mt.Engage(e.GetPos());
				break;
			case Mouse::Event::Type::LRelease:
				mt.Release();
				break;
			case Mouse::Event::Type::Move:
				if (mt.Engaged())
				{
					const auto delta = mt.Move(e.GetPos());
					cam_rot = cam_rot
						* Mat4::RotationY((float)-delta.x * htrack)
						* Mat4::RotationX((float)-delta.y * vtrack);
				}
				break;
			}
		}



		if (kbd.KeyIsPressed('U'))
		{
			light_pos.x += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('I'))
		{
			light_pos.y += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('O'))
		{
			light_pos.z += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('J'))
		{
			light_pos.x -= 0.2f * dt;
		}
		if (kbd.KeyIsPressed('K'))
		{
			light_pos.y -= 0.2f * dt;
		}
		if (kbd.KeyIsPressed('L'))
		{
			light_pos.z -= 0.2f * dt;
		}
		time += dt;
		light_pos.y += sin(time)/20;
		pipeline.effect.vs.SetTime(time/2);
		wavePipe.effect.vs.SetTime(time);

	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();
		const auto proj = Mat4::ProjectionHFOV(hfov, 1.33333f, 0.5f, 15.0f);
		const auto view = Mat4::Translation(-cam_pos) * cam_rot;
		
		/*Vec4 lightV4 = Vec4(light_pos) * view;
		Vec3 light_pos_view = { lightV4.x,lightV4.y ,lightV4.z };*/

		pipeline.effect.vs.BindWorldView(
			Mat4::RotationX(theta_x) *
			Mat4::RotationY(theta_y) *
			Mat4::RotationZ(theta_z) *
			Mat4::Translation(Vec3{ 0.0f,0.0f,2.5f }) * view
		);
		pipeline.effect.vs.BindProjection(proj);
		// light indicne.effect.vs.BindProjection(proj);
		pipeline.effect.ps.SetLightPosition(light_pos*view );
		//pipeline.effect.vs.SetLightPosition(light_pos);
		// render triangles
		pipeline.Draw(itlist);

		Lightpipeline.effect.vs.BindWorldView(Mat4::Translation( light_pos) * view);
		Lightpipeline.effect.vs.BindProjection(proj);
		// render triangles
		Lightpipeline.Draw(itlistLightPoint);

		// walls
		for (auto& w : walls)
		{
			WallsPipeLine.effect.vs.BindWorldView(
				Mat4::RotationX(theta_x) *
				Mat4::RotationY(theta_y) *
				Mat4::RotationZ(theta_z) *
				Mat4::Translation(0.0f, 0.0f, 0.0f) * view
				
			);
			WallsPipeLine.effect.ps.BindTexture(L"Images\\Small_wall.png");
			WallsPipeLine.effect.vs.BindProjection(proj);
			WallsPipeLine.effect.ps.SetLightPosition(light_pos * view);
			WallsPipeLine.effect.ps.SetScale(3.0f);
			WallsPipeLine.Draw(w.itList);
		}
		// wave
		wavePipe.effect.vs.BindWorldView(
			Mat4::RotationX(theta_x) *
			Mat4::RotationY(theta_y) *
			Mat4::RotationZ(theta_z) *
			Mat4::Translation(WavePos) * view

		);
		wavePipe.effect.vs.BindProjection(proj);
		wavePipe.effect.ps.SetLightPosition(light_pos * view);
		wavePipe.effect.ps.BindTexture(L"Images\\Uta.png");
		wavePipe.Draw(Waveitlist);
	}
private:
	MouseHandler mt;
	std::shared_ptr<ZBuffer> pZb;
	IndexedTriangleList<SolidEffect::Vertex> itlistLightPoint = Sphere::GetPlain<SolidEffect::Vertex>(20, 20, 0.1f);
	IndexedTriangleList<Vertex> itlist;
	Pipeline pipeline;
	PipelineLight Lightpipeline;
	WallsPipeline WallsPipeLine;
	static constexpr float dTheta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	Vec4 light_pos = { 0.0f,-1.5f,0.3f, 1.0f };
	// camera
	static constexpr float hfov = 95.0f;//x fow
	static constexpr float aspect_ratio = 1.333f;
	static constexpr float vfov = hfov / aspect_ratio;// y fow
	// amount of rotation per pixel of movement on the screen
	static constexpr float htrack = to_rad(hfov) / (float)Graphics::ScreenWidth;
	static constexpr float vtrack = to_rad(vfov) / (float)Graphics::ScreenHeight;
	Vec3 cam_pos = { 0.0f, 0.0f, 0.0f };
	Vec3 cam_dir = { 0.0f, 0.0f, 1.0f };
	Mat4 cam_rot = Mat4::Identity();
	static constexpr float cam_speed = 1.0f;

	// walls
	std::vector<Wall<TextureEffect::Vertex>> walls;
	//time
	float time = 0.0f;
	// wave
	WavePipeline wavePipe;
	IndexedTriangleList< WaveVertexTextureEffect::Vertex> Waveitlist;
	Vec3 WavePos = { 0.0f, -1.5f, -0.5f };
};
