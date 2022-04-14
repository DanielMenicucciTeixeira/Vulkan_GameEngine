#include "Ray.h"

#include "Physics/CollisionDetection.h"

#include <stdexcept>

Ray::Ray()
{
	Origin = new FVector3();
	Direction = new FVector3(0.0f, 0.0f, -1.0f);
	Length = 0;
	Infinit = true;
}

Ray::Ray(const FVector3 start, const FVector3 direction, float length, bool infinit)
{
	Origin = start;
	Direction = direction.GetNormal();//Direction should always be normalized
	Length = length;
	Infinit = infinit;
}

Ray::~Ray()
{
}

FVector3 Ray::GetPositionAtLength(float displacement) const
{
	if (Infinit || abs(displacement) <= Length)
	{
		return Origin + (Direction * displacement);
	}
	else
	{
		throw std::runtime_error("Ray::GetPosition() tried to acess position beyond ray length");
	}
}

FVector3 Ray::GetOrigin() const
{
	return Origin;
}

FVector3 Ray::GetDirection() const
{
	return Direction;
}

void Ray::SetOrigin(const FVector3& position)
{
	Origin = position;
}

void Ray::SetDirection(const FVector3& direction)
{
	Direction = direction;
}

Ray& Ray::operator=(const Ray& ray)
{
	Origin = ray.Origin;
	Direction = ray.Direction;
	Length = ray.Length;
	Infinit = ray.Infinit;
	return *this;
}

void Ray::SetIntersectDistance(float dis)
{
	intersectDistance = dis;
}

float Ray::GetDistanceFromOrigin(FVector3 point)
{
	return ((Origin.X - point.X) * (Origin.X - point.X) +
		(Origin.Y - point.Y) * (Origin.Y - point.Y) +
		(Origin.Z - point.Z) * (Origin.Z - point.Z));
}
