#include "MyTimer.h"


bool MyTimer::exceeded(float seconds) {

	unsigned int delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->StartTime).count();
	return ((float)delta_time / 1000) > seconds;
}

void MyTimer::start() {
	this->StartTime = std::chrono::steady_clock::now();
}

MyTimer::MyTimer()
{
	this->StartTime = std::chrono::steady_clock::now();
}