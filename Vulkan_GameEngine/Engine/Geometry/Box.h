#ifndef BOX_H
#define BOX_H

#include "../Math/FVector3.h"
#include "../Math/FMatrix4.h"

struct Box
{
	FVector3 Min;
	FVector3 Max;
	FMatrix4 model;
};
#endif

