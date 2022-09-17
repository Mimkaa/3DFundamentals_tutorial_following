#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "AnimatedTextureEffect.h"
#include "MyTimer.h"

// scene demonstrating skinned cube
class AnimatedCubeSkinScene : public Scene
{
public:
	typedef Pipeline<AnimatedTextureEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	AnimatedCubeSkinScene(Graphics& gfx, const std::vector<std::wstring>& filenames)
		:
		itlist(Cube::GetAnimated<Vertex>()),
		pipeline(gfx)
	{

		pipeline.effect.ps.BindTextures(filenames);
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
		if (kbd.KeyIsPressed('R'))
		{
			offset_z += 2.0f * dt;
		}
		if (kbd.KeyIsPressed('F'))
		{
			offset_z -= 2.0f * dt;
		}
		// loop through animation frames
		if (myTimer.exceeded(0.05))
		{
			pipeline.effect.ps.current_tex = (pipeline.effect.ps.current_tex + 1) % 199;
			myTimer.start();
		}
	}
	virtual void Draw() override
	{
		// clear z-buffer
		pipeline.BeginFrame();
		// generate rotation matrix from euler angles
		// translation from offset
		const Mat3 rot =
			Mat3::RotationX(theta_x) *
			Mat3::RotationY(theta_y) *
			Mat3::RotationZ(theta_z);
		const Vec3 trans = { 0.0f,0.0f,offset_z };
		// set pipeline transform
		pipeline.BindRotation(rot);
		pipeline.BindTranslation(trans);
		// render triangles
		pipeline.Draw(itlist);
	}
private:
	IndexedTriangleList<Vertex> itlist;
	Pipeline pipeline;
	static constexpr float dTheta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	MyTimer myTimer;
};

