#pragma once

#include <limits>
#include <cassert>

class ZBuffer
{
public:
	ZBuffer(int width, int height)
		:
		width(width),
		height(height),
		// allocated on the heap because stack may not contain enough memory for the task
		pBuffer(new float[width * height])
	{}
	~ZBuffer()
	{
		delete[] pBuffer;
		pBuffer = nullptr;
	}
	ZBuffer(const ZBuffer&) = delete;
	ZBuffer& operator=(const ZBuffer&) = delete;
	void Clear()
	{
		const int nDepths = width * height;
		for (int i = 0; i < nDepths; i++)
		{
			pBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}
	float& At(int x, int y)
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);
		return pBuffer[y * width + x];
	}
	const float& At(int x, int y) const
	{
		return const_cast<ZBuffer*>(this)->At(x, y);
	}
	bool TestAndSet(int x, int y, float depth)
	{
		float& depthInBuffer = At(x, y);
		if (depth < depthInBuffer)
		{
			depthInBuffer = depth;
			return true;
		}
		return false;
	}
	auto GetMinMax() const
	{
		return std::minmax_element(pBuffer, pBuffer + width * height);
	}
public:
	int width;
	int height;
private:
	float* pBuffer = nullptr;
};

