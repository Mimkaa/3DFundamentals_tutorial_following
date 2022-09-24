/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Mat3.h"
#include "CubeSkinScene.h"
#include "CubeVertexColorScene.h"
#include "SolidCubeScene.h"
#include "AnimatesTextureScene.h"
#include "DoubleCubeScene.h"
#include "VertexWaveScene.h"
#include "CubeVertexPositionColorScene.h"
#include "SolidCobeGeometryScene.h"
#include "CubeFlatIndependentLighting.h"
#include "ProceduralNormalScene.h"
#include "SmoothBunnyScene.h"
#include "RotatingFaceScene.h"
#include "GeometryFlatScene.h"
#include "SphereScene.h"
#include "GouraudScene.h"
#include <filesystem>
#include <sstream>
#include "Sphere.h"

Game::Game( MainWindow& wnd ):wnd( wnd ),gfx( wnd )
{
	scenes.push_back(std::make_unique<SmooothBunnyScene>(gfx, IndexedTriangleList<SmooothBunnyScene::Vertex>::GeneratedNormals("models\\model.obj")));
	scenes.push_back(std::make_unique<GeometryFlatScene>(gfx, IndexedTriangleList<GeometryFlatScene::Vertex>::LoadMyVersion("models\\model.obj")));
	scenes.push_back(std::make_unique<GouraudScene>(gfx, IndexedTriangleList<GouraudScene::Vertex>::LoadNormals("models\\suzanne.obj")));
	scenes.push_back(std::make_unique<GouraudScene>(gfx, Sphere::GetPlainNormals<GouraudScene::Vertex>(20, 20, 1.0f)));
	scenes.push_back(std::make_unique<SphereScene>(gfx, Sphere::GetPlain<GeometryFlatScene::Vertex>(20, 20, 1.0f)));
	scenes.push_back(std::make_unique<SphereScene>(gfx, Sphere::GetPlain<GeometryFlatScene::Vertex>(20,20,1.0f)));
	
	scenes.push_back(std::make_unique<GeometryFlatScene>(gfx, IndexedTriangleList<GeometryFlatScene::Vertex>::Load("models\\model.obj")));
	scenes.push_back(std::make_unique<CubeFlatIndependentScene>(gfx));
	scenes.push_back(std::make_unique<RotatingFacesScene>(gfx));
	scenes.push_back(std::make_unique<CubeSkinScene>(gfx, L"images\\soil.png"));
	scenes.push_back(std::make_unique<CubeVertexColorScene>(gfx));
	scenes.push_back(std::make_unique<CubeSolidScene>(gfx));
	scenes.push_back(std::make_unique<AnimatedCubeSkinScene>(gfx, ScanDirectory("Images\\Uta_dance_frames")));
	scenes.push_back(std::make_unique<DoubleCubeScene>(gfx));
	scenes.push_back(std::make_unique<VertexWaveScene>(gfx));
	scenes.push_back(std::make_unique<CubeVertexPositionColorScene>(gfx));
	scenes.push_back(std::make_unique<CubeSolidGeometryScene>(gfx));
	
	curScene = scenes.begin();

}

std::vector<std::wstring> Game::ScanDirectory(const char* dirname)
{
	std::vector<std::wstring> names;
	for (const auto& entry : std::filesystem::directory_iterator(dirname))
	{
		auto path = entry.path();
		names.emplace_back(path.wstring());
	}
		
	return names;

}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();
	while (!wnd.kbd.KeyIsEmpty())
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.GetCode() == VK_TAB && e.IsPress())
		{
			CycleScenes();
		}
	}
	// update scene
	(*curScene)->Update(wnd.kbd, wnd.mouse, dt);
}

void Game::CycleScenes()
{
	if (++curScene == scenes.end())
	{
		curScene = scenes.begin();
	}
}



void Game::ComposeFrame()
{
	// draw scene
	(*curScene)->Draw();
}