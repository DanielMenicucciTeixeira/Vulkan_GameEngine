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

		x = vector.x;
		y = vector.y;
		z = vector.z;
	}
	else
	{
		x = axis.x;
		y = axis.y;
		z = axis.z;
		w = angle;
	}
}

FQuaternion FQuaternion::operator*(const FQuaternion& Quaternion) const
{
	FVector3 vector0 = FVector3(x, y, z);
	FVector3 vector1 = FVector3(Quaternion.x, Quaternion.y, Quaternion.z);

	FVector3 vectorFinal = (vector1 * w) + (vector0 * Quaternion.w) + (vector0.CrossProduct(vector1));
	float scalar = (w * Quaternion.w) - (vector0 * vector1);

	return FQuaternion(vectorFinal, scalar, false);
}

void FQuaternion::operator=(const FQuaternion& Quaternion)
{
	x = Quaternion.x;
	y = Quaternion.y;
	z = Quaternion.z;
	w = Quaternion.w;
}

void FQuaternion::Invert()
{
	Conjugate();
	x = x / Length();
	y = y / Length();
	z = z / Length();
	w = w / Length();
}

FMatrix FQuaternion::GetRotationMatrix()
{
	FVector3 row0 = FVector3(1 - 2 * pow(y,2) - 2 * pow(z,2), 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w);
	FVector3 row1 = FVector3(2 * x * y + 2 * z * w, 1 - 2 * pow(x,2) - 2 * pow(z,2), 2 * y * z - 2 * x * w);
	FVector3 row2 = FVector3(2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * pow(x,2) - 2 * pow(y,2));
	return FMatrix(row0, row1, row2);
}

FVector3 FQuaternion::GetEulerAngle()
{
	float a = atan2(2 * (w * x + y * z), 1 - 2 * (pow(x, 2) + pow(y, 2)));
	float b = asin(2 * (w * y - z * x));
	float c = atan2(2 * (w * z + x * y), 1 - 2 * (pow(y, 2) + pow(z, 2)));
	return FVector3(a, b, c);
}

FVector3 FQuaternion::RotateVector(FVector3 vector)
{
	FQuaternion pureQuat = FQuaternion(vector, 0, false);
	pureQuat = *this * pureQuat * GetConjugated();
	return FVector3(pureQuat.x, pureQuat.y, pureQuat.z);
}

void FQuaternion::PrintQuaternion()
{
	printf("\n[%f, %f, %f, %f]", x, y, z, w);
}

FQuaternion::~FQuaternion()
{
}
