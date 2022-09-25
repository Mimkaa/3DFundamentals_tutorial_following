#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "GouraudPointLightEffect.h"
#include "Sphere.h"
#include "SolidCubeEffect.h"

class GouraudPointLightScene : public Scene
{
public:
	typedef ::Pipeline<GouraudPointLightEffect> Pipeline;
	typedef ::Pipeline<SolidEffect> PipelineLight;
	typedef Pipeline::Vertex Vertex;
public:
	GouraudPointLightScene(Graphics& gfx, IndexedTriangleList<Vertex> it)
		:
		itlist(std::move(it)),
		pipeline(gfx),
		Lightpipeline(gfx)

	{

		itlistLightPoint = Sphere::GetPlain<SolidEffect::Vertex>(20, 20, 0.05f);
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
		// generate rotation matrix from euler angles
		// translation from offset
		const Mat3 rot =
			Mat3::RotationX(theta_x) *
			Mat3::RotationY(theta_y) *
			Mat3::RotationZ(theta_z);
	
		const Vec3 trans = { 0.0f,0.0f,offset_z };
		// set pipeline transform
		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(trans);
		pipeline.effect.vs.SetLightPosition({ loffset_x,loffset_y,loffset_z });
		//pipeline.effect.vs.SetLightPosition(light_pos);
		// render triangles
		pipeline.Draw(itlist);

		Lightpipeline.BeginFrame();
		
		const Vec3 transLight = { loffset_x,loffset_y,loffset_z };
		// set pipeline transform
		Lightpipeline.effect.vs.BindRotation(Mat3::Identity());
		Lightpipeline.effect.vs.BindTranslation(transLight);
		//Lightpipeline.effect.vs.SetLightPosition({ loffset_x,loffset_y,loffset_z });
		//pipeline.effect.vs.SetLightPosition(light_pos);
		// render triangles
		Lightpipeline.Draw(itlistLightPoint);
	}
private:
	IndexedTriangleList<SolidEffect::Vertex> itlistLightPoint;
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


