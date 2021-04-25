#include "Ray.h"

#include "Math/FVector3.h"

#include <stdexcept>

Ray::Ray()
{
	Origin = new FVector3();
	Direction = new FVector3(0.0f, 0.0f, -1.0f);
	Lenght = 0;
	Infinit = true;
}

Ray::Ray(const FVector3& start, const FVector3& direction, float length, bool infinit)
{
	Origin = new FVector3(start);
	Direction = new FVector3(direction.GetNormal());//Direction should always be normalized
	Lenght = length;
	Infinit = infinit;
}

Ray::~Ray()
{
	if (Origin) delete(Origin);
	if (Direction) delete(Direction);
}

FVector3 Ray::GetPositionAtLenght(float displacement) const
{
	if (Infinit || abs(displacement) <= Lenght)
	{
		return *Origin + (*Direction * displacement);
	}
	else
	{
		throw std::runtime_error("Ray::GetPosition() tried to acess position beyond ray lenght");
	}
}

FVector3 Ray::GetOrigin() const
{
	return *Origin;
}

FVector3 Ray::GetDirection() const
{
	return *Direction;
}

void Ray::SetOrigin(const FVector3& position)
{
	*Origin = position;
}

void Ray::SetDirection(const FVector3& direction)
{
	*Direction = direction;
}

Ray& Ray::operator=(const Ray& ray)
{
	Origin = ray.Origin;
	Direction = ray.Direction;
	Lenght = ray.Lenght;
	Infinit = ray.Infinit;
	return *this;
}

bool Ray::IsColliding(C_BoundingBox* box)
{
	return true;
}
