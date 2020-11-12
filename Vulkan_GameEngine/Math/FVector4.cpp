#include "FVector4.h"
#include <cmath>


inline FVector4::FVector4(const FVector4& InitilizerVector)
{
	X = InitilizerVector.X;
	Y = InitilizerVector.Y;
	Z = InitilizerVector.Z;
	w = InitilizerVector.w;
}

FVector4::~FVector4()
{
}

FVector4 FVector4::operator+(const FVector4& Vector)
{
	return FVector4(X + Vector.X, Y + Vector.Y, Z + Vector.Z, w + Vector.w);
}

FVector4 FVector4::operator-(const FVector4& Vector)
{
	return FVector4(X - Vector.X, Y - Vector.Y, Z - Vector.Z, w - Vector.w);
}

FVector4 FVector4::operator*(float Multiplier)
{
	return FVector4(X * Multiplier, Y * Multiplier, Z * Multiplier, w * Multiplier);
}

FVector4 FVector4::operator/(float Divisor)
{
	return FVector4(X / Divisor, Y / Divisor, Z / Divisor, w / Divisor);
}

float FVector4::operator*(const FVector4& Vector)
{
	return (X * Vector.X) + (Y * Vector.Y) + (Z * Vector.Z) + (w * Vector.w);
}

void FVector4::operator=(const FVector4& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
	Z = Vector.Z;
	w = Vector.w;
}

void FVector4::operator=(FVector4* Vector)
{
	X = Vector->X;
	Y = Vector->Y;
	Z = Vector->Z;
	w = Vector->w;
}

void FVector4::operator+=(const FVector4& Vector)
{
	X += Vector.X;
	Y += Vector.Y;
	Z += Vector.Z;
	w += Vector.w;
}

void FVector4::operator-=(const FVector4& Vector)
{
	X -= Vector.X;
	Y -= Vector.Y;
	Z -= Vector.Z;
	w -= Vector.w;
}

float FVector4::Length()
{
	return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2) + pow(w, 2));
}

FVector4 FVector4::GetNormal()
{
	int length = Length();
	return FVector4(X / length, Y / length, Z / length, w / length);
}

void FVector4::Normalize()
{
	int length = Length();

	X = X / length;
	Y = Y / length;
	Z = Z / length;
	w = w / length;
}

void FVector4::test()
{
}
