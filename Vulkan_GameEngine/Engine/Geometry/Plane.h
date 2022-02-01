#pragma once
#include "../Math/FVector4.h"
#include "../Math/FVector3.h"
class Ray;

class Plane : public FVector4
{
public:
	inline Plane() { X = Y = Z = 1; W = 0; }
	Plane(float X, float Y, float Z, float W) : FVector4{ X, Y, Z, W } { normal = FVector3(X, Y, Z); }

	//Creates a plane given 3 points on that plane. All of the points must be different
	Plane(const FVector3& Vector0, const FVector3& Vector1, const FVector3& Vector2);

	bool InterssectionPoint(const Ray& ray, FVector3& interssectionPoint) const;
	
	inline FVector3 GetPlaneNormal() const { return FVector3(X, Y, Z).GetNormal(); }
	FVector3 GetRandomPointInPlane() const;

	void NormalizePlane();

protected:
	FVector3 normal;
};

