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

		VectorArray[0] = normal.VectorArray[0];
		VectorArray[1] = normal.VectorArray[1];
		VectorArray[2] = normal.VectorArray[2];
		VectorArray[3] = normal * point0;
	}
}

bool Plane::InterssectionPoint(Ray* ray, FVector3& interssectionPoint)
{
	if (*ray->GetDirection() * normal != 0.0f)
	{
		//float displacement = (w - (normal * ray->GetStartPosition())) / (normal * ray->GetDirection());
		float displacement = (normal * *ray->GetStartPosition() + VectorArray[3]) / (normal * (*ray->GetDirection() -  *ray->GetStartPosition()));
		if (ray->IsInfinit() || abs(displacement) <= ray->GetLenght())
		{
			interssectionPoint = ray->GetPosition(displacement);
			return true;
		}
		else return false;
	}
	else return false;
}

FVector3 Plane::GetRandomPointInPlane()
{
	if (VectorArray[0] != 0) return FVector3(VectorArray[3] / VectorArray[0], 0, 0);
	else if (VectorArray[1] != 0) return FVector3(0, VectorArray[3] / VectorArray[1], 0);
	else if (VectorArray[2] != 0) return FVector3(0, 0, VectorArray[2] / VectorArray[3]);
	else return FVector3();
}
