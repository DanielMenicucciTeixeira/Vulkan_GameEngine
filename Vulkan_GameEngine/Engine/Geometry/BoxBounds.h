#ifndef BOXBOUNDS_H
#define BOXBOUNDS_H

#include "../Math/FMatrix4.h"
#include "../Math/FVector3.h"

class S_BoxBounds
{
	FVector3 Max = FVector3();
	FVector3 Min = FVector3();
	FMatrix4 Model = FMatrix4();
};

#endif
