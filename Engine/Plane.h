#pragma once
#include <array>
#include "IndexedTriangleList.h"
class Plane
{
public:
	template<class V>
	static IndexedTriangleList<V> GetPlain(int divisions = 7, float size = 1.0f)
	{
		const int VerticesPerDevSide = divisions + 1;
		std::vector<V> vertices(sq(VerticesPerDevSide + 1));
	
		// additional scope to destroy everything inside as soon as the program reaches the end of it
		{
			const float side = size / 2.0f;
			const float DevidedSideLength = size / (float)divisions;
			const Vec3 BottomLeft = { -side, -side, 0.0f };
			

			for (int y = 0, i = 0; y < VerticesPerDevSide; y++)
			{
				const float y_pos = float(y) * DevidedSideLength;
				for (int x = 0; x < VerticesPerDevSide; x++, i++)
				{
					vertices[i].pos= BottomLeft + Vec3{ float(x) * DevidedSideLength, y_pos,0.0f};
				}
			}
		}

		std::vector<size_t> indices;
		// reserve does not initialize default values
		indices.reserve(sq(divisions ) * 6);
		{
			const auto vxy2i = [VerticesPerDevSide](size_t x, size_t y)
			{
				// we are indexing in vertices
				return y * VerticesPerDevSide + x;
			};

			for (int y = 0; y < divisions; y++)
			{
				
				for (int x = 0; x < divisions; x++)
				{

					const std::array<size_t, 4> indexArray =
					{ vxy2i(x,y),vxy2i(x + 1,y),vxy2i(x,y + 1),vxy2i(x + 1,y + 1) };
					indices.push_back(indexArray[0]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[3]);
				}
			}

		}
		return{ std::move(vertices),std::move(indices) };
	}
	template<class V>
	static IndexedTriangleList<V> GetSkinned(int divisions = 7, float size = 1.0f)
	{
		auto itlist = GetPlain<V>(divisions, size);
		{
			const int nVerticesSide = divisions + 1;
			const float tDivisionSize = 1.0f / float(divisions);
			const Vec2 tBottomLeft = { 0.0f,1.0f };

			for (int y = 0, i = 0; y < nVerticesSide; y++)
			{
				const float y_t = -float(y) * tDivisionSize;
				for (int x = 0; x < nVerticesSide; x++, i++)
				{
					itlist.vertices[i].t = tBottomLeft + Vec2{ float(x) * tDivisionSize,y_t };
				}
			}

		}

		return itlist;
	}

	template<class V>
	static IndexedTriangleList<V> GetNormals(int divisions = 7, float size = 1.0f)
	{
		auto tl = GetPlain<V>(divisions, size);
		for (auto& v : tl.vertices)
		{
			v.n = { 0.0f, 0.0f, -1.0f };
		}
		return tl;
	}

	
};