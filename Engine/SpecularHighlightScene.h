#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat.h"
#include "Pipeline.h"
#include "SpecularHighlightEffect.h"
#include "Sphere.h"
#include "SolidCubeEffect.h"
#include "MouseHandler.h"

class SpecularHighlightScene : public Scene
{
public:
	// :: was used to access the global scope 
	typedef ::Pipeline<SpecularHighlightEffect> Pipeline;
	typedef ::Pipeline<SolidEffect> PipelineLight;
	typedef Pipeline::Vertex Vertex;
public:
	SpecularHighlightScene(Graphics& gfx, IndexedTriangleList<Vertex> it)
		:
		pZb(std::make_shared<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		itlist(std::move(it)),
		pipeline(gfx, pZb),
		Lightpipeline(gfx, pZb)

	{

		itlist.AdjustToTrueCenter();
		offset_z = itlist.GetRadius() * 1.6f;
		for (auto& v : itlistLightPoint.vertices)
		{
			v.color = Colors::White;
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
		
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();
		const auto proj = Mat4::ProjectionHFOV(100.0f, 1.33333f, 0.5f, 4.0f);
		const auto view = Mat4::Translation(-cam_pos) * cam_rot;
		pipeline.effect.vs.BindWorld(
			Mat4::RotationX(theta_x) *
			Mat4::RotationY(theta_y) *
			Mat4::RotationZ(theta_z) *
			Mat4::Translation(0.0f, 0.0f, offset_z)
		);
		pipeline.effect.vs.BindProjection(proj);
		pipeline.effect.vs.BindView(view);
		pipeline.effect.ps.SetLightPosition({ light_pos });
		//pipeline.effect.vs.SetLightPosition(light_pos);
		// render triangles
		pipeline.Draw(itlist);




		// set pipeline transform
		Lightpipeline.effect.vs.BindWorldView(Mat4::Translation(light_pos) * view);

		Lightpipeline.effect.vs.BindProjection(proj);

		//pipeline.effect.vs.SetLightPosition(light_pos);
		// render triangles
		Lightpipeline.Draw(itlistLightPoint);
	}
private:
	MouseHandler mt;
	std::shared_ptr<ZBuffer> pZb;
	IndexedTriangleList<SolidEffect::Vertex> itlistLightPoint = Sphere::GetPlain<SolidEffect::Vertex>(20, 20, 0.05f);
	IndexedTriangleList<Vertex> itlist;
	Pipeline pipeline;
	PipelineLight Lightpipeline;
	static constexpr float dTheta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	
	Vec3 light_pos = { 0.0f,0.0f,0.7f };
	// camera
	static constexpr float hfov = 95.0f;//x fow
	static constexpr float aspect_ratio = 1.333f;
	static constexpr float vfov = hfov/ aspect_ratio;// y fow
	// amount of rotation per pixel of movement on the screen
	static constexpr float htrack = to_rad(hfov) / (float)Graphics::ScreenWidth;
	static constexpr float vtrack = to_rad(vfov) / (float)Graphics::ScreenHeight;
	Vec3 cam_pos = { 0.0f, 0.0f, 0.0f };
	Vec3 cam_dir = { 0.0f, 0.0f, 1.0f };
	Mat4 cam_rot = Mat4::Identity();
	static constexpr float cam_speed = 1.0f;
};

