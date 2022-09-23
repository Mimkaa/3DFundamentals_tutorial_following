#pragma once
#include "IndexedTriangleList.h"
#include "Mat3.h"
class Sphere
{
public:
	template<class V>
	static IndexedTriangleList<V> GetPlain(unsigned int latdivs, unsigned int longdivs, float radius)
	{
		//create verticies
		const Vec3 base = { 0.0f, 0.0f, radius };
		const float LatAng = PI / latdivs;
		const float LongAng = PI * 2 / longdivs;
		std::vector<V> vertices;
		for (int y = 1; y < latdivs; y++)
		{
			// rotate z around x
			const Vec3 stepped = base * Mat3::RotationX(y * LatAng);
			for (int x = 0; x < longdivs; x++)
			{
				// created a vertex with garbage data
				vertices.emplace_back();
				vertices.back().pos = stepped * Mat3::RotationZ(x * LongAng);
			}
		}

		// create indicies
		std::vector<size_t> inds;
		const auto index1d = [latdivs, longdivs](size_t x, size_t y) 
		{
			return y * longdivs + x;
		};
		for (int iLat = 0;  iLat < latdivs-2 ;  iLat++)
		{
			for (int iLong = 0; iLong < longdivs -1 ; iLong++)
			{
				inds.push_back(index1d(iLong, iLat));
				inds.push_back(index1d(iLong, iLat + 1));
				inds.push_back(index1d(iLong + 1, iLat + 1));
				inds.push_back(index1d(iLong + 1, iLat + 1));
				inds.push_back(index1d(iLong + 1, iLat));
				inds.push_back(index1d(iLong, iLat));
			}
		}
		// add poles
		const Vec3 North = base;
		const Vec3 South = - North;
		vertices.push_back(North);
		int startN = 0;
		for (int iLong = 0; iLong < longdivs ; iLong++)
		{
			inds.push_back(vertices.size()-1);
			inds.push_back(startN);
			inds.push_back((startN+1)% longdivs);
			startN +=1;
		}
		vertices.push_back(South);
		int startS = 0;
		for (int iLong = 0; iLong < longdivs ; iLong++)
		{
			inds.push_back(vertices.size() - 1);
			int midd;
			if ((startS + 1 + (longdivs) * (latdivs - 2) )% longdivs == 0)
			{ 
				midd = (longdivs) * (latdivs - 2);
			}
			else
			{
				midd = startS + 1 + (longdivs) * (latdivs - 2);
			}
			inds.push_back(midd);
			inds.push_back(startS + (longdivs) * (latdivs-2));
			
			startS += 1;
		}
		//wrap
	for (int iLat = 0; iLat < latdivs - 2; iLat++)
	{
		inds.push_back(index1d(longdivs - 1, iLat));
		inds.push_back(index1d(longdivs - 1, iLat + 1));
		inds.push_back(index1d(0, iLat + 1));
		inds.push_back(index1d(0, iLat + 1));
		inds.push_back(index1d(0, iLat));
		inds.push_back(index1d(longdivs - 1, iLat));
	}
		

		return{ std::move(vertices),std::move(inds) };
	}

	template<class V>
	static IndexedTriangleList<V> GetPlainNormals(unsigned int latdivs, unsigned int longdivs, float radius)
	{
		auto sphere = GetPlain<V>(latdivs, longdivs, radius);
		for (auto& v : sphere.verticies)
		{
			v.n = v.pos.GetNormalized();
		}
		return sphere;
	}
};