#ifndef FQUATERNION_H
#define FQUATERNION_H

#include "FVector4.h"

class FMatrix;

class FQuaternion : public FVector4
{
public:

	inline FQuaternion() { VectorArray[0] = VectorArray[1] = VectorArray[2] = 0; VectorArray[3] = 1; }
	inline FQuaternion(FVector4 vector) : FVector4(vector) {}
	inline FQuaternion(float X, float Y, float Z, float W) { X = X; Y = Y; Z = Z; VectorArray[3] = W; }
	FQuaternion(FVector3 axis, float angle, bool isRotation = true, bool inRadians = false);

	//Overloaded Operators
	FQuaternion operator* (const FQuaternion& Quaternion) const;
	void operator= (const FQuaternion& Quaternion);

	//Functions
	inline FQuaternion GetConjugated() { return FQuaternion(-VectorArray[0], -VectorArray[1], -VectorArray[2], VectorArray[3]); }
	inline void Conjugate() { VectorArray[0] = -VectorArray[0]; VectorArray[1] = -VectorArray[1]; VectorArray[2] = -VectorArray[2]; }
	inline FQuaternion GetInverse() { return FQuaternion(GetConjugated()/Length()); }
	void Invert();
	FMatrix GetRotationMatrix();
	FVector3 GetEulerAngle();
	FVector3 RotateVector(FVector3 vector);

	void PrintQuaternion();

	~FQuaternion();
};

#endif // !FQUATERNION_H

