#ifndef FQUATERNION_H
#define FQUATERNION_H

#include "FVector4.h"

class FMatrix;

class FQuaternion : public FVector4
{
public:

	inline FQuaternion() { X = Y = Z = 0; w = 1; }
	inline FQuaternion(FVector4 vector) : FVector4(vector) {}
	inline FQuaternion(float X, float Y, float Z, float W) { X = X; Y = Y; Z = Z; w = W; }
	FQuaternion(FVector3 axis, float angle, bool isRotation = true, bool inRadians = false);

	//Overloaded Operators
	FQuaternion operator* (const FQuaternion& Quaternion) const;
	void operator= (const FQuaternion& Quaternion);

	//Functions
	inline FQuaternion GetConjugated() { return FQuaternion(-X, -Y, -Z, w); }
	inline void Conjugate() { X = -X; Y = -Y; Z = -Z; }
	inline FQuaternion GetInverse() { return FQuaternion(GetConjugated()/Length()); }
	void Invert();
	FMatrix GetRotationMatrix();
	FVector3 GetEulerAngle();
	FVector3 RotateVector(FVector3 vector);

	void PrintQuaternion();

	~FQuaternion();
};

#endif // !FQUATERNION_H

