#include "FQuaternion.h"

#include "FMatrix.h"
#include "FVector3.h"

#include<cmath>
#include <stdio.h>

FQuaternion::FQuaternion(FVector3 axis, float angle, bool isRotation, bool inRadians)
{
	if (isRotation)
	{
		FVector3 vector;

		if (inRadians)
		{
			vector = axis * (sinf(angle * 0.5f));
			VectorArray[3] = cosf(angle * 0.5f);
		}
		else
		{
			vector = axis * sinf(angle * (3.14159265359 / 180.0f) * 0.5f);
			VectorArray[3] = cosf(angle * (3.14159265359 / 180.0f) * 0.5f);
		}

		VectorArray[0] = vector.VectorArray[0];
		VectorArray[1] = vector.VectorArray[1];
		VectorArray[2] = vector.VectorArray[2];
	}
	else
	{
		VectorArray[0] = axis.VectorArray[0];
		VectorArray[1] = axis.VectorArray[1];
		VectorArray[2] = axis.VectorArray[2];
		VectorArray[3] = angle;
	}
}

FQuaternion FQuaternion::operator*(const FQuaternion& Quaternion) const
{
	FVector3 vector0 = FVector3(VectorArray[0], VectorArray[1], VectorArray[2]);
	FVector3 vector1 = FVector3(Quaternion.VectorArray[0], Quaternion.VectorArray[1], Quaternion.VectorArray[2]);

	FVector3 vectorFinal = (vector1 * VectorArray[3]) + (vector0 * Quaternion.VectorArray[3]) + (vector0.CrossProduct(vector1));
	float scalar = (VectorArray[3] * Quaternion.VectorArray[3]) - (vector0 * vector1);

	return FQuaternion(vectorFinal, scalar, false);
}

void FQuaternion::operator=(const FQuaternion& Quaternion)
{
	VectorArray[0] = Quaternion.VectorArray[0];
	VectorArray[1] = Quaternion.VectorArray[1];
	VectorArray[2] = Quaternion.VectorArray[2];
	VectorArray[3] = Quaternion.VectorArray[3];
}

void FQuaternion::Invert()
{
	Conjugate();
	VectorArray[0] = VectorArray[0] / Length();
	VectorArray[1] = VectorArray[1] / Length();
	VectorArray[2] = VectorArray[2] / Length();
	VectorArray[3] = VectorArray[3] / Length();
}

FMatrix FQuaternion::GetRotationMatrix()
{
	FVector3 row0 = FVector3(1 - 2 * pow(VectorArray[1],2) - 2 * pow(VectorArray[2],2), 2 * VectorArray[0] * VectorArray[1] - 2 * VectorArray[2] * VectorArray[3], 2 * VectorArray[0] * VectorArray[2] + 2 * VectorArray[1] * VectorArray[3]);
	FVector3 row1 = FVector3(2 * VectorArray[0] * VectorArray[1] + 2 * VectorArray[2] * VectorArray[3], 1 - 2 * pow(VectorArray[0],2) - 2 * pow(VectorArray[2],2), 2 * VectorArray[1] * VectorArray[2] - 2 * VectorArray[0] * VectorArray[3]);
	FVector3 row2 = FVector3(2 * VectorArray[0] * VectorArray[2] - 2 * VectorArray[1] * VectorArray[3], 2 * VectorArray[1] * VectorArray[2] + 2 * VectorArray[0] * VectorArray[3], 1 - 2 * pow(VectorArray[0],2) - 2 * pow(VectorArray[1],2));
	return FMatrix(row0, row1, row2);
}

FVector3 FQuaternion::GetEulerAngle()
{
	float a = atan2(2 * (VectorArray[3] * VectorArray[0] + VectorArray[1] * VectorArray[2]), 1 - 2 * (pow(VectorArray[0], 2) + pow(VectorArray[1], 2)));
	float b = asin(2 * (VectorArray[3] * VectorArray[1] - VectorArray[2] * VectorArray[0]));
	float c = atan2(2 * (VectorArray[3] * VectorArray[2] + VectorArray[0] * VectorArray[1]), 1 - 2 * (pow(VectorArray[1], 2) + pow(VectorArray[2], 2)));
	return FVector3(a, b, c);
}

FVector3 FQuaternion::RotateVector(FVector3 vector)
{
	FQuaternion pureQuat = FQuaternion(vector, 0, false);
	pureQuat = *this * pureQuat * GetConjugated();
	return FVector3(pureQuat.VectorArray[0], pureQuat.VectorArray[1], pureQuat.VectorArray[2]);
}

void FQuaternion::PrintQuaternion()
{
	printf("\n[%f, %f, %f, %f]", VectorArray[0], VectorArray[1], VectorArray[2], VectorArray[3]);
}

FQuaternion::~FQuaternion()
{
}
