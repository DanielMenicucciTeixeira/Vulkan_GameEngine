#ifndef SPHERE_H
#define SPHERE_H
#include "Math/FVector3.h"

class Sphere
{
public:

	Sphere();
	Sphere(FVector3 Position, float Radius);

	FVector3 position;
	float radius;

};

#endif // 

