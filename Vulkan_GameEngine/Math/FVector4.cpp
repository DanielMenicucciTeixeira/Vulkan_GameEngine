#include "FVector4.h"
#include <cmath>


inline FVector4::FVector4(const FVector4& InitilizerVector)
{
	x = InitilizerVector.x;
	y = InitilizerVector.y;
	z = InitilizerVector.z;
	w = InitilizerVector.w;
}

FVector4::~FVector4()
{
}

FVector4 FVector4::operator+(const FVector4& Vector)
{
	return FVector4(x + Vector.x, y + Vector.y, z + Vector.z, w + Vector.w);
}

FVector4 FVector4::operator-(const FVector4& Vector)
{
	return FVector4(x - Vector.x, y - Vector.y, z - Vector.z, w - Vector.w);
}

FVector4 FVector4::operator*(float Multiplier)
{
	return FVector4(x * Multiplier, y * Multiplier, z * Multiplier, w * Multiplier);
}

FVector4 FVector4::operator/(float Divisor)
{
	return FVector4(x / Divisor, y / Divisor, z / Divisor, w / Divisor);
}

float FVector4::operator*(const FVector4& Vector)
{
	return (x * Vector.x) + (y * Vector.y) + (z * Vector.z) + (w * Vector.w);
}

void FVector4::operator=(const FVector4& Vector)
{
	x = Vector.x;
	y = Vector.y;
	z = Vector.z;
	w = Vector.w;
}

void FVector4::operator=(FVector4* Vector)
{
	x = Vector->x;
	y = Vector->y;
	z = Vector->z;
	w = Vector->w;
}

void FVector4::operator+=(const FVector4& Vector)
{
	x += Vector.x;
	y += Vector.y;
	z += Vector.z;
	w += Vector.w;
}

void FVector4::operator-=(const FVector4& Vector)
{
	x -= Vector.x;
	y -= Vector.y;
	z -= Vector.z;
	w -= Vector.w;
}

float FVector4::Length()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
}

FVector4 FVector4::GetNormal()
{
	int length = Length();
	return FVector4(x / length, y / length, z / length, w / length);
}

void FVector4::Normalize()
{
	int length = Length();

	x = x / length;
	y = y / length;
	z = z / length;
	w = w / length;
}

void FVector4::test()
{
}
