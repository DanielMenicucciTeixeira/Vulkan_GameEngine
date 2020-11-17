#include "FVector4.h"
#include "FVector3.h"
#include <cmath>


FVector4::FVector4(const FVector4& InitilizerVector)
{
	VectorArray[0] = InitilizerVector.VectorArray[0];
	VectorArray[1] = InitilizerVector.VectorArray[1];
	VectorArray[2] = InitilizerVector.VectorArray[2];
	VectorArray[3] = InitilizerVector.VectorArray[3];
}

FVector4::~FVector4()
{
}

FVector4 FVector4::operator+(const FVector4& Vector)
{
	return FVector4(VectorArray[0] + Vector.VectorArray[0], VectorArray[1] + Vector.VectorArray[1], VectorArray[2] + Vector.VectorArray[2], VectorArray[3] + Vector.VectorArray[3]);
}

FVector4 FVector4::operator-(const FVector4& Vector)
{
	return FVector4(VectorArray[0] - Vector.VectorArray[0], VectorArray[1] - Vector.VectorArray[1], VectorArray[2] - Vector.VectorArray[2], VectorArray[3] - Vector.VectorArray[3]);
}

FVector4 FVector4::operator*(float Multiplier)
{
	return FVector4(VectorArray[0] * Multiplier, VectorArray[1] * Multiplier, VectorArray[2] * Multiplier, VectorArray[3] * Multiplier);
}

FVector4 FVector4::operator/(float Divisor)
{
	return FVector4(VectorArray[0] / Divisor, VectorArray[1] / Divisor, VectorArray[2] / Divisor, VectorArray[3] / Divisor);
}

float FVector4::operator*(const FVector4& Vector)
{
	return (VectorArray[0] * Vector.VectorArray[0]) + (VectorArray[1] * Vector.VectorArray[1]) + (VectorArray[2] * Vector.VectorArray[2]) + (VectorArray[3] * Vector.VectorArray[3]);
}

void FVector4::operator=(const FVector4& Vector)
{
	VectorArray[0] = Vector.VectorArray[0];
	VectorArray[1] = Vector.VectorArray[1];
	VectorArray[2] = Vector.VectorArray[2];
	VectorArray[3] = Vector.VectorArray[3];
}

void FVector4::operator=(FVector4* Vector)
{
	VectorArray[0] = Vector->VectorArray[0];
	VectorArray[1] = Vector->VectorArray[1];
	VectorArray[2] = Vector->VectorArray[2];
	VectorArray[3] = Vector->VectorArray[3];
}

void FVector4::operator+=(const FVector4& Vector)
{
	VectorArray[0] += Vector.VectorArray[0];
	VectorArray[1] += Vector.VectorArray[1];
	VectorArray[2] += Vector.VectorArray[2];
	VectorArray[3] += Vector.VectorArray[3];
}

void FVector4::operator-=(const FVector4& Vector)
{
	VectorArray[0] -= Vector.VectorArray[0];
	VectorArray[1] -= Vector.VectorArray[1];
	VectorArray[2] -= Vector.VectorArray[2];
	VectorArray[3] -= Vector.VectorArray[3];
}

FVector3 FVector4::CrossProduct(const FVector4& Vector)
{
	return FVector3(VectorArray[1] * Vector.VectorArray[2] - VectorArray[2] * Vector.VectorArray[1], VectorArray[2] * Vector.VectorArray[0] - VectorArray[0] * Vector.VectorArray[2], VectorArray[0] * Vector.VectorArray[1] - VectorArray[1] * Vector.VectorArray[0]);
}

float FVector4::Length()
{
	return sqrt(pow(VectorArray[0], 2) + pow(VectorArray[1], 2) + pow(VectorArray[2], 2) + pow(VectorArray[3], 2));
}

FVector4 FVector4::GetNormal()
{
	int length = Length();
	return FVector4(VectorArray[0] / length, VectorArray[1] / length, VectorArray[2] / length, VectorArray[3] / length);
}

void FVector4::Normalize()
{
	int length = Length();

	VectorArray[0] = VectorArray[0] / length;
	VectorArray[1] = VectorArray[1] / length;
	VectorArray[2] = VectorArray[2] / length;
	VectorArray[3] = VectorArray[3] / length;
}