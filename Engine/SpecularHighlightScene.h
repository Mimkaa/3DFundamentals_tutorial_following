#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat.h"
#include "Pipeline.h"
#include "SpecularHighlightEffect.h"
#include "Sphere.h"
#include "SolidCubeEffect.h"

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
		if (kbd.KeyIsPressed('Q'))
		{
			theta_x = wrap_angle(theta_x + dTheta * dt);
		}
		if (kbd.KeyIsPressed('W'))
		{
			theta_y = wrap_angle(theta_y + dTheta * dt);
		}
		if (kbd.KeyIsPressed('E'))
		{
			theta_z = wrap_angle(theta_z + dTheta * dt);
		}
		if (kbd.KeyIsPressed('A'))
		{
			theta_x = wrap_angle(theta_x - dTheta * dt);
		}
		if (kbd.KeyIsPressed('S'))
		{
			theta_y = wrap_angle(theta_y - dTheta * dt);
		}
		if (kbd.KeyIsPressed('D'))
		{
			theta_z = wrap_angle(theta_z - dTheta * dt);
		}
		if (kbd.KeyIsPressed('U'))
		{
			loffset_x += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('I'))
		{
			loffset_y += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('O'))
		{
			loffset_z += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('J'))
		{
			loffset_x -= 0.2f * dt;
		}
		if (kbd.KeyIsPressed('K'))
		{
			loffset_y -= 0.2f * dt;
		}
		if (kbd.KeyIsPressed('L'))
		{
			loffset_z -= 0.2f * dt;
		}
		if (kbd.KeyIsPressed('R'))
		{
			offset_z += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('F'))
		{
			offset_z -= 0.2f * dt;
		}
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();
		const auto proj = Mat4::ProjectionHFOV(100.0f, 1.33333f, 0.5f, 4.0f);;
		pipeline.effect.vs.BindWorld(
			Mat4::RotationX(theta_x) *
			Mat4::RotationY(theta_y) *
			Mat4::RotationZ(theta_z) *
			Mat4::Translation(0.0f, 0.0f, offset_z)
		);
		pipeline.effect.vs.BindProjection(proj);
		pipeline.effect.ps.SetLightPosition({ loffset_x,loffset_y,loffset_z });
		//pipeline.effect.vs.SetLightPosition(light_pos);
		// render triangles
		pipeline.Draw(itlist);




		// set pipeline transform
		Lightpipeline.effect.vs.BindWorld(Mat4::Translation(loffset_x, loffset_y, loffset_z));
		Lightpipeline.effect.vs.BindProjection(proj);

		//pipeline.effect.vs.SetLightPosition(light_pos);
		// render triangles
		Lightpipeline.Draw(itlistLightPoint);
	}
private:
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
	float loffset_x = 0.0f;
	float loffset_y = 0.0f;
	float loffset_z = 0.5f;
	Vec3 light_pos = { loffset_x,loffset_y,loffset_z };
};

