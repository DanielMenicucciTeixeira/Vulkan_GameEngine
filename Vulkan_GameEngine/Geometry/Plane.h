#pragma once
#include "Math/FVector4.h"
class Ray;

class Plane : public FVector4
{
public:
	inline Plane() { X = Y = Z = 1; w = 0; }
	Plane(float X, float Y, float Z, float W) : FVector4{ X, Y, Z, W } { normal = FVector3(X, Y, Z); }

	//Creates a plane given 3 points on that plane. All of the points must be different
	Plane(const FVector3& Vector0, const FVector3& Vector1, const FVector3& Vector2);

	bool InterssectionPoint(Ray* ray, FVector3& interssectionPoint);
	
	inline FVector3 GetPlaneNormal() { return FVector3(X, Y, Z).GetNormal(); }
	FVector3 GetRandomPointInPlane();

private:
	FVector3 normal;
};

