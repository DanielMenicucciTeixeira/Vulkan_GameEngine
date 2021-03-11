#include "Clock.h"
#include "Objects/Object.h"


float Clock::GetInstantDeltaTimeMilliseconds() const
{
	return std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::high_resolution_clock::now() - CurrentTime).count();
}

void Clock::StartClock()
{
	StartTime = PreviousTime = CurrentTime = std::chrono::high_resolution_clock::now();
}
void Clock::UpdateClock()
{
	PreviousTime = CurrentTime;
	CurrentTime = std::chrono::high_resolution_clock::now();
}

unsigned int Clock::GetSleepTime(const unsigned int framesPerSecond) const
{
	unsigned int millisecondsPerFrame = MILLISECONDS_TO_SECONDS / framesPerSecond;
	if (millisecondsPerFrame == 0)
	{
		return 0;
	}
	
	unsigned int sleepTime = millisecondsPerFrame - GetInstantDeltaTimeMilliseconds();
	return sleepTime < millisecondsPerFrame ? sleepTime : millisecondsPerFrame;
}

float Clock::GetDeltaTimeSeconds() const
{
	return std::chrono::duration<float, std::chrono::seconds::period>(CurrentTime - PreviousTime).count();
}

float Clock::GetDeltaTimeMiliSecods() const
{
	return std::chrono::duration<float, std::chrono::milliseconds::period>(CurrentTime - PreviousTime).count();
}

float Clock::GetTimeSeconds() const
{
	return std::chrono::duration<float, std::chrono::seconds::period>(CurrentTime - StartTime).count();
}

Clock::Clock()
{
}
