#include "FQuaternion.h"

#include "FMatrix4.h"
#include "FVector3.h"

#define _USE_MATH_DEFINES
#include<cmath>
#include <stdio.h>

FQuaternion::FQuaternion(const FVector3& axis, float angle, bool isRotation, bool inRadians)
{
	if (isRotation)
	{
		FVector3 vector;

		if (!inRadians) angle *= M_PI / 180;
		vector = axis * (sinf(angle * 0.5f));
		W = cosf(angle * 0.5f);
		X = vector.X;
		Y = vector.Y;
		Z = vector.Z;
	}
	else
	{
		X = axis.X;
		Y = axis.Y;
		Z = axis.Z;
		W = angle;
	}
}

FQuaternion::FQuaternion(const FVector3& euler, bool inRadians)
{
	FVector3 vector;
	if (!inRadians) vector = euler * M_PI / 180;
	else vector = euler;
	// Abbreviations for the various angular functions
	double cy = cos(vector.Z * 0.5);
	double sy = sin(vector.Z * 0.5);
	double cp = cos(vector.Y * 0.5);
	double sp = sin(vector.Y * 0.5);
	double cr = cos(vector.X * 0.5);
	double sr = sin(vector.X * 0.5);

	W = cr * cp * cy + sr * sp * sy;
	X = sr * cp * cy - cr * sp * sy;
	Y = cr * sp * cy + sr * cp * sy;
	Z = cr * cp * sy - sr * sp * cy;

	Normalize();
}

FQuaternion FQuaternion::operator*(const FQuaternion& quaternion) const
{
	FVector3 vector0 = FVector3(X, Y, Z);
	FVector3 vector1 = FVector3(quaternion.X, quaternion.Y, quaternion.Z);

	FVector3 vectorFinal = (vector1 * W) + (vector0 * quaternion.W) + (vector0.CrossProduct(vector1));
	float scalar = (W * quaternion.W) - (vector0 * vector1);

	return FQuaternion(vectorFinal, scalar, false);
}

FQuaternion FQuaternion::operator*(const float& scalar) const
{
	//float w = cosf(acosf(W) * scalar);
	return FQuaternion(X * scalar, Y * scalar, Z * scalar, W * scalar);
}

FQuaternion FQuaternion::MultiplyRotation(const float& scalar) const
{
	float w = cosf(acosf(W) * scalar);
	return FQuaternion(X, Y, Z, w).GetNormal();
}

void FQuaternion::operator=(const FQuaternion& quaternion)
{
	X = quaternion.X;
	Y = quaternion.Y;
	Z = quaternion.Z;
	W = quaternion.W;
}

void FQuaternion::Invert()
{
	Conjugate();
	X = X / Length();
	Y = Y / Length();
	Z = Z / Length();
	W = W / Length();
}

FVector3 FQuaternion::GetEulerAngle() const
{
	float a = atan2(2 * (W * X + Y * Z), 1 - 2 * (pow(X, 2) + pow(Y, 2)));
	float b = asin(2 * (W * Y - Z * X));
	float c = atan2(2 * (W * Z + X * Y), 1 - 2 * (pow(Y, 2) + pow(Z, 2)));
	return FVector3(a, b, c);
}

FVector3 FQuaternion::GetRotatedVector(const FVector3& vector, const FQuaternion& quaternion)
{
	return vector.GetRotatedVector(quaternion);
}

FQuaternion FQuaternion::SLerp(FQuaternion quaternion0, FQuaternion quaternion1, float deltaTime)
{
	// Only unit quaternions are valid rotations.
   // Normalize to avoid undefined behavior.
	quaternion0.Normalize();
	quaternion1.Normalize();

	// Compute the cosine of the angle between the two vectors.
	float dot = DotProduct(quaternion0, quaternion1);

	// If the dot product is negative, slerp won't take
	// the shorter path. Note that v1 and -v1 are equivalent when
	// the negation is applied to all four components. Fix by 
	// reversing one quaternion.
	if (dot < 0.0f) {
		quaternion1 = quaternion1 * -1;
		dot = -dot;
	}

	const float DOT_THRESHOLD = 0.9995;
	if (dot > DOT_THRESHOLD) {
		// If the inputs are too close for comfort, linearly interpolate
		// and normalize the result.

		FQuaternion result = quaternion0 + (quaternion1 - quaternion0) * deltaTime;
		result;
		return result;
	}

	// Since dot is in range [0, DOT_THRESHOLD], acos is safe
	float theta0 = acos(dot);        // theta_0 = angle between input vectors
	float theta = theta0 * deltaTime;          // theta = angle between v0 and result
	float sinTheta = sin(theta);     // compute this value only once
	float sinTheta0 = sin(theta0); // compute this value only once

	float scalar0 = cos(theta) - dot * sinTheta / sinTheta0;  // == sin(theta_0 - theta) / sin(theta_0)
	float scalar1 = sinTheta / sinTheta0;
	FQuaternion result = (quaternion0 * scalar0) + (quaternion1 * scalar1);
	return ((quaternion0 * scalar0) + (quaternion1 * scalar1));
}

float FQuaternion::DotProduct(const FQuaternion& quaternion0, const FQuaternion& quaternion1)
{
	float x = quaternion0.X * quaternion1.X;
	float y = quaternion0.Y * quaternion1.Y;
	float z = quaternion0.Z * quaternion1.Z;
	float w = quaternion0.W * quaternion1.W;
	return x + y + z + w;
}

FVector3 FQuaternion::GetUpVector()
{
	return GetRotatedVector(FVector3(0.0f, 0.0f, 1.0f), *this);
}

FVector3 FQuaternion::GetForwardVector()
{
	return GetRotatedVector(FVector3(0.0f, -1.0f, 0.0f), *this);
}

FVector3 FQuaternion::GetRightVector()
{
	return GetRotatedVector(FVector3(1.0f, 0.0f, 0.0f), *this);
}

FQuaternion::~FQuaternion()
{
}
