#ifndef BOXBOUNDS_H
#define BOXBOUNDS_H

#include "Math/FVector3.h"
#include "Math/FMatrix4.h"

struct S_BoxBounds
{
	FVector3 Max;
	FVector3 Min;
	FMatrix4 Model;

	FVector3 GetTransformedMax()
	{
		FVector4 newMax(Max.X, Max.Y, Max.Z, 1);
		newMax = Model * newMax;
		newMax = newMax / newMax.W;
		return newMax;
	}

	FVector3 GetTransformedMin()
	{
		FVector4 newMin(Min.X, Min.Y, Min.Z, 1);
		newMin = Model * newMin;
		newMin = newMin / newMin.W;
		return newMin;
	}
};

#endif
