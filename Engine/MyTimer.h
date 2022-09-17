#pragma once
#include <chrono>
class MyTimer
{
public:
	std::chrono::time_point<std::chrono::steady_clock> StartTime;
	MyTimer();
	bool exceeded(float seconds);
	void start();
};

