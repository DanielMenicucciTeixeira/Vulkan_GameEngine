#include "Plane.h"
#include <stdexcept>
#include "Ray.h"

Plane::Plane(const FVector3& Vector0, const FVector3& Vector1, const FVector3& Vector2)
{
	FVector3 point0 = Vector0;
	FVector3 point1 = Vector1;
	FVector3 point2 = Vector2;

	if (point0 == point1 || point0 == point2 || point1 == point2)
	{
		throw std::runtime_error("Could not create plane, all three given points must be different");
	}
	else
	{
		FVector3 vectorA = point0 - point1;
		FVector3 vectorB = point0 - point2;
		normal = vectorA.CrossProduct(vectorB).GetNormal();

		X = normal.X;
		Y = normal.Y;
		Z = normal.Z;
		W = normal * point0;
	}
}

bool Plane::InterssectionPoint(const Ray& ray, FVector3& interssectionPoint) const
{
	if (ray.GetDirection() * normal != 0.0f)
	{
		//float displacement = (W - (normal * ray.GetOrigin())) / (normal * ray.GetDirection());
		float displacement = (normal * ray.GetOrigin() + W) / (normal * (ray.GetDirection() -  ray.GetOrigin()));
		if (ray.IsInfinit() || abs(displacement) <= ray.GetLength())
		{
			interssectionPoint = ray.GetPositionAtLength(displacement);
			return true;
		}
		else return false;
	}
	else return false;
}

FVector3 Plane::GetRandomPointInPlane() const
{
	if (X != 0) return FVector3(W / X, 0, 0);
	else if (Y != 0) return FVector3(0, W / Y, 0);
	else if (Z != 0) return FVector3(0, 0, Z / W);
	else return FVector3();
}

void Plane::NormalizePlane()
{
	FVector3 normal(*this);
	normal.Normalize();
	X = normal.X;
	Y = normal.Y;
	Z = normal.Z;
}
