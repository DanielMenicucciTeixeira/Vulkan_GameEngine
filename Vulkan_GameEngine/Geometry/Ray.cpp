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

Ray::Ray(const FVector3& start, const FVector3& direction, float length, bool infinit)
{
	StartPosition = new FVector3(start);
	Direction = new FVector3(direction.GetNormal());//Direction should always be normalized
	Lenght = length;
	Infinit = infinit;
}

Ray::~Ray()
{
	if (StartPosition) delete(StartPosition);
	if (Direction) delete(Direction);
}

FVector3 Ray::GetPosition(float displacement) const
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

FVector3 Ray::GetStartPosition() const
{
	return *StartPosition;
}

FVector3 Ray::GetDirection() const
{
	return *Direction;
}

void Ray::SetStartPosition(const FVector3& position)
{
	*StartPosition = position;
}

void Ray::SetDirection(const FVector3& direction)
{
	*Direction = direction;
}
