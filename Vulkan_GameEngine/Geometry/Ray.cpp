#include "Ray.h"

#include "Math/FVector3.h"

#include <stdexcept>

Ray::Ray()
{
	StartPosition = new FVector3();
	Direction = new FVector3(1.0f, 0.0f, 0.0f);
	Lenght = 0;
	Infinit = true;
}

Ray::Ray(FVector3* Start, FVector3* Direction, int Length, bool Infinit)
{
	StartPosition = Start;
	Direction = new FVector3(Direction->GetNormal());//Direction should always be normalized
	Lenght = Length;
	Infinit = Infinit;
}

Ray::~Ray()
{
	if (StartPosition) delete(StartPosition);
	if (Direction) delete(Direction);
}

FVector3 Ray::GetPosition(float displacement)
{
	if (Infinit || abs(displacement) <= Lenght)
	{
		return *StartPosition + (*Direction * displacement);
	}
	else
	{
		throw std::runtime_error("Ray::GetPosition() tried to acess position beyond ray lenght");
	}
}
