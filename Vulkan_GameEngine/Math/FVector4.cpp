#include "FVector4.h"
#include "FVector3.h"
#include <cmath>
#include <iostream>


FVector4::FVector4(const FVector4& InitilizerVector)
{
	X = InitilizerVector.X;
	Y = InitilizerVector.Y;
	Z = InitilizerVector.Z;
	W = InitilizerVector.W;
}

FVector4::~FVector4()
{
}

FVector4 FVector4::operator+(const FVector4& Vector) const
{
	return FVector4(X + Vector.X, Y + Vector.Y, Z + Vector.Z, W + Vector.W);
}

FVector4 FVector4::operator-(const FVector4& Vector) const
{
	return FVector4(X - Vector.X, Y - Vector.Y, Z - Vector.Z, W - Vector.W);
}

FVector4 FVector4::operator*(float Multiplier) const
{
	return FVector4(X * Multiplier, Y * Multiplier, Z * Multiplier, W * Multiplier);
}

FVector4 FVector4::operator/(float Divisor) const
{
	return FVector4(X / Divisor, Y / Divisor, Z / Divisor, W / Divisor);
}

float FVector4::operator*(const FVector4& Vector) const
{
	return (X * Vector.X) + (Y * Vector.Y) + (Z * Vector.Z) + (W * Vector.W);
}

void FVector4::operator=(const FVector4& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
	Z = Vector.Z;
	W = Vector.W;
}

void FVector4::operator=(FVector4* Vector)
{
	X = Vector->X;
	Y = Vector->Y;
	Z = Vector->Z;
	W = Vector->W;
}

void FVector4::operator=(float values[4])
{
	X = values[0];
	Y = values[1];
	Z = values[2];
	W = values[3];
}

void FVector4::operator+=(const FVector4& Vector)
{
	X += Vector.X;
	Y += Vector.Y;
	Z += Vector.Z;
	W += Vector.W;
}

void FVector4::operator-=(const FVector4& Vector)
{
	X -= Vector.X;
	Y -= Vector.Y;
	Z -= Vector.Z;
	W -= Vector.W;
}

void FVector4::operator*=(const FVector4& vector)
{
	*this = *this * vector;
}

FVector3 FVector4::CrossProduct(const FVector4& Vector) const
{
	return FVector3(Y * Vector.Z - Z * Vector.Y, Z * Vector.X - X * Vector.Z, X * Vector.Y - Y * Vector.X);
}

float FVector4::Length() const
{
	return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2) + pow(W, 2));
}

FVector4 FVector4::GetNormal() const
{
	int length = Length();
	return FVector4(X / length, Y / length, Z / length, W / length);
}

void FVector4::Normalize()
{
	int length = Length();

	X = X / length;
	Y = Y / length;
	Z = Z / length;
	W = W / length;
}

void FVector4::Print() const
{
	printf("[ %f,\t%f,\t%f,\t%f ]\n", X, Y, Z, W);
}
