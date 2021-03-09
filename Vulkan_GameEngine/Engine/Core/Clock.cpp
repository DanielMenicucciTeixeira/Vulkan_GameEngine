#include "Clock.h"

float Clock::GetDeltaTimeSeconds()
{
	currentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
	previousTime = currentTime;
	return deltaTime;
}

float Clock::GetTimeSeconds()
{
	currentTime = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
}

Clock::Clock()
{
	startTime = previousTime = currentTime = std::chrono::high_resolution_clock::now();
}
