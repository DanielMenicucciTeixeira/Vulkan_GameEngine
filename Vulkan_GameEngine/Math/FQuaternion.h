#ifndef FQUATERNION_H
#define FQUATERNION_H

#include "FVector4.h"

class FMatrix4;

class FQuaternion : public FVector4
{
public:

	inline FQuaternion() { X = Y = Z = 0; W = 1; }
	inline FQuaternion(const FVector4& vector) : FVector4(vector) {}
	inline FQuaternion(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w; }
	FQuaternion(const FVector3& axis, float angle, bool isRotation = true, bool inRadians = false);

	//Overloaded Operators
	FQuaternion operator* (const FQuaternion& Quaternion) const;
	void operator= (const FQuaternion& Quaternion);

	//Functions
	inline FQuaternion GetConjugated() const { return FQuaternion(-X, -Y, -Z, W); }
	inline void Conjugate() { X = -X; Y = -Y; Z = -Z; }
	inline FQuaternion GetInverse() const { return FQuaternion(GetConjugated()/Length()); }
	void Invert();
	FVector3 GetEulerAngle() const;
	static FVector3 GetRotatedVector(const FVector3& vector, const FQuaternion& quaternion);

	~FQuaternion();
};

#endif // !FQUATERNION_H

