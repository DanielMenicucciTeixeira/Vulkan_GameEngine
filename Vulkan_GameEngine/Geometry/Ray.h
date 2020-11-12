#ifndef RAY_H
#define RAY_H

#include "Math/FVector3.h"

class Ray
{
public:
	//Contructors
	inline Ray() { startPosition = FVector3(0); direction = FVector3(1.0f, 0.0f, 0.0f); lenght = 0; infinit = true; }
	Ray(FVector3 Start, FVector3 Direction, int Length = 0, bool Infinit = true);

	~Ray();

private:
	FVector3 startPosition;
	FVector3 direction;
	float lenght;
	bool infinit;

public:
	FVector3 GetPosition(float displacement);

	inline FVector3 GetStartPosition() { return startPosition; }
	inline FVector3 GetDirection() { return direction; }
	inline float GetLenght() { return lenght; }
	inline bool IsInfinit() { return infinit; }
};

#endif
