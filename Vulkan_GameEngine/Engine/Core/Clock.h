#ifndef CLOCK_H
#define CLOCK_H

#include "Objects/Object.h"
#include <chrono>
#include <cstdlib>

class Clock
{
protected:
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> currentTime;
	std::chrono::time_point<std::chrono::steady_clock> previousTime;

public:
	
	float GetDeltaTimeSeconds();
	float GetTimeSeconds();

	Clock();
};
#endif
