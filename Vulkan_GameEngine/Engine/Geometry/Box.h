#ifndef BOX_H
#define BOX_H

#include "../Math/FVector3.h"
#include "../Math/FMatrix4.h"


//Min and max are local cords, model is world.
struct S_Box
{
	FVector3 Min;
	FVector3 Max;
	FMatrix4 model;
};
#endif

