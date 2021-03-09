#include "Sphere.h"
#include "Math/FVector3.h"

Sphere::Sphere()
{
	position = new FVector3(0.0f);
	radius = 1.0f;
}

Sphere::Sphere(FVector3 Position, float Radius)
{
	position = Position;
	radius = Radius;
}
