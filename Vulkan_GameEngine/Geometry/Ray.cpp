#include "Ray.h"
#include <stdexcept>

Ray::Ray(FVector3 Start, FVector3 Direction, int Length, bool Infinit)
{
	startPosition = Start;
	direction = Direction.GetNormal();//Direction should always be normalized
	lenght = Length;
	infinit = Infinit;
}

Ray::~Ray()
{
}

FVector3 Ray::GetPosition(float displacement)
{
	if (infinit || abs(displacement) <= lenght)
	{
		return startPosition + (direction * displacement);
	}
	else
	{
		throw std::runtime_error("Ray::GetPosition() tried to acess position beyond ray lenght");
	}
}
