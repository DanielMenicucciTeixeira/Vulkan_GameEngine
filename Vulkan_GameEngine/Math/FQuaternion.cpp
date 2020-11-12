#include "FQuaternion.h"
#include<cmath>
#include "FMatrix.h"
#include <stdio.h>

FQuaternion::FQuaternion(FVector3 axis, float angle, bool isRotation, bool inRadians)
{
	if (isRotation)
	{
		FVector3 vector;

		if (inRadians)
		{
			vector = axis * (sinf(angle * 0.5f));
			w = cosf(angle * 0.5f);
		}
		else
		{
			vector = axis * sinf(angle * (3.14159265359 / 180.0f) * 0.5f);
			w = cosf(angle * (3.14159265359 / 180.0f) * 0.5f);
		}

		X = vector.X;
		Y = vector.Y;
		Z = vector.Z;
	}
	else
	{
		X = axis.X;
		Y = axis.Y;
		Z = axis.Z;
		w = angle;
	}
}

FQuaternion FQuaternion::operator*(const FQuaternion& Quaternion) const
{
	FVector3 vector0 = FVector3(X, Y, Z);
	FVector3 vector1 = FVector3(Quaternion.X, Quaternion.Y, Quaternion.Z);

	FVector3 vectorFinal = (vector1 * w) + (vector0 * Quaternion.w) + (vector0.CrossProduct(vector1));
	float scalar = (w * Quaternion.w) - (vector0 * vector1);

	return FQuaternion(vectorFinal, scalar, false);
}

void FQuaternion::operator=(const FQuaternion& Quaternion)
{
	X = Quaternion.X;
	Y = Quaternion.Y;
	Z = Quaternion.Z;
	w = Quaternion.w;
}

void FQuaternion::Invert()
{
	Conjugate();
	X = X / Length();
	Y = Y / Length();
	Z = Z / Length();
	w = w / Length();
}

FMatrix FQuaternion::GetRotationMatrix()
{
	FVector3 row0 = FVector3(1 - 2 * pow(Y,2) - 2 * pow(Z,2), 2 * X * Y - 2 * Z * w, 2 * X * Z + 2 * Y * w);
	FVector3 row1 = FVector3(2 * X * Y + 2 * Z * w, 1 - 2 * pow(X,2) - 2 * pow(Z,2), 2 * Y * Z - 2 * X * w);
	FVector3 row2 = FVector3(2 * X * Z - 2 * Y * w, 2 * Y * Z + 2 * X * w, 1 - 2 * pow(X,2) - 2 * pow(Y,2));
	return FMatrix(row0, row1, row2);
}

FVector3 FQuaternion::GetEulerAngle()
{
	float a = atan2(2 * (w * X + Y * Z), 1 - 2 * (pow(X, 2) + pow(Y, 2)));
	float b = asin(2 * (w * Y - Z * X));
	float c = atan2(2 * (w * Z + X * Y), 1 - 2 * (pow(Y, 2) + pow(Z, 2)));
	return FVector3(a, b, c);
}

FVector3 FQuaternion::RotateVector(FVector3 vector)
{
	FQuaternion pureQuat = FQuaternion(vector, 0, false);
	pureQuat = *this * pureQuat * GetConjugated();
	return FVector3(pureQuat.X, pureQuat.Y, pureQuat.Z);
}

void FQuaternion::PrintQuaternion()
{
	printf("\n[%f, %f, %f, %f]", X, Y, Z, w);
}

FQuaternion::~FQuaternion()
{
}
