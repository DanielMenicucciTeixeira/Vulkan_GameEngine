#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <cstdlib>

constexpr float MILLISECONDS_TO_SECONDS = 1000.0f;

using namespace std::chrono;

typedef high_resolution_clock Timer;

class Clock
{
//Uses a define macro to prevent copying of this class
//#define noCopyClass Clock
//#include "Auxiliary/Singleton.h"
//---------------------------------------------------------------------------------------------
protected:
	//Actual time at which the clock has started (StartClock function was called).
	time_point<std::chrono::steady_clock> StartTime;

	//Current absolute time of each frame (update in UpdateClock function every frame).
	time_point<std::chrono::steady_clock> CurrentTime;

	//Absolute time at the last frame (update in UpdateClock function every frame).
	time_point<std::chrono::steady_clock> PreviousTime;

	unsigned int prevTicks, currentTicks;

	//Get the delta time in milliseconds at a given instant, used to calculate time errors withing the same frame.
	float GetInstantDeltaTimeMilliseconds() const;

public:
	Clock();
	~Clock();


	//Set the initial values for the clocks variables.
	void StartClock();

	//Reset current and privious times according to absolute time
	void UpdateClock();

	//Calculates the waiting time required to avoid exeeding a given fps rate
	unsigned int GetSleepTime(const unsigned int framesPerSecond) const;

	//Getters

	float GetDeltaTimeSeconds() const;
	float GetDeltaTimeMiliSecods() const;
	float GetTimeSeconds() const;


};
#endif