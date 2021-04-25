#include "FVector2.h"
#include "IVector2.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

///Constructors

//Constructor using initial values for each component.
FVector2::FVector2(float x, float y)
{
	X = x;
	Y = y;
}

//Constructor initializing all components to a single float value.
FVector2::FVector2(float value)
{
	X = value;
	Y = value;
}

inline FVector2::FVector2(const FVector2& vector)
{
	X = vector.X;
	Y = vector.Y;
}

FVector2::FVector2(const IVector2& vector)
{
	X = vector.X;
	Y = vector.Y;
}

FVector2::FVector2(float values[2])
{
	X = values[0];
	Y = values[1];
}

//Default Constructor, initializes all values to 0;
FVector2::FVector2()
{
	X = 0.0f;
	Y = 0.0f;
}

///Destructors

FVector2::~FVector2()
{
}

///Operators

//Overload of the "+" operator, makes a vector plus vector addition.
FVector2 FVector2::operator+(const FVector2& vector) const
{
	FVector2 result;

	result.X = X + vector.X;
	result.Y = Y + vector.Y;

	return result;
}

//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
void FVector2::operator+=(const FVector2& vector)
{
	X += vector.X;
	Y += vector.Y;
}

//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
void FVector2::operator-=(const FVector2& vector)
{
	X -= vector.X;
	Y -= vector.Y;
}

bool FVector2::operator==(const FVector2& vector) const
{
	return X == vector.X && Y == vector.Y;
}

//Overload of the "-" operator, makes a vector minus vector subtraction.
FVector2 FVector2::operator-(const FVector2& vector) const
{

	FVector2 Result;

	Result.X = X - vector.X;
	Result.Y = Y - vector.Y;

	return Result;
}

//Overload of the "*" operator, makes a vector times scalar multiplication.
FVector2 FVector2::operator*(float multiplier) const
{
	return FVector2(X * multiplier, Y * multiplier);
}

//Overload of the "/" operator, makes a vector divided by scalar division.
FVector2 FVector2::operator/(float divisor) const
{
	return FVector2(X / divisor, Y / divisor);
}

//Overload of the "*" operator, makes a vector times vector dot product.
float FVector2::operator*(const FVector2& vector) const
{
	return (X * vector.X + Y * vector.Y);
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void FVector2::operator=(const FVector2& vector)
{
	X = vector.X;
	Y = vector.Y;
}

void FVector2::operator=(float values[3])
{
	X = values[0];
	Y = values[1];
}

///Functions

//Returns the magnitude of the vector (or vector's scalar lenght)
float FVector2::Length() const
{
	return sqrt(X * X + Y * Y);
}

//Returns the angle between this vector and another given vector in degrees.
float FVector2::GetAngle(const FVector2& vector, bool inRadians)
{
	if (inRadians)
	{
		return acos((*this * vector) / (Length() * vector.Length()));
	}
	else
	{
		return acos((*this * vector) / (Length() * vector.Length())) * 180.0f / M_PI;
	}
}

//Returns the Cross product of this vector and another given vector.
FVector2 FVector2::CrossProduct(const FVector2& vector)
{
	return FVector2(X * vector.Y - Y * vector.X);
}

//Gets the normalized form of this vector
FVector2 FVector2::GetNormal() const
{

	return *this / Length();
}

//Divides the vector by its Magnitude to get the normalized unit vector.
void FVector2::Normalize()
{
	*this = *this / Length();
}

void FVector2::Rotate(float angle, bool isRadian)
{
	if (!isRadian) angle *= M_PI / 180.0f;
	float x = X * cos(angle) - Y * sin(angle);
	float y =X * sin(angle) + Y * cos(angle);
	X = x;
	Y = y;
}

FVector2 FVector2::GetVectorByAngle(float angle, bool isRadian)
{
	FVector2 returnVector(0.0f, 1.0f);
	returnVector.Rotate(angle, isRadian);
	return returnVector.GetNormal();
}

float FVector2::GetDistance(FVector2 vector) const
{
	return (*this - vector).Length();
}

float FVector2::GetDistance(FVector2 vector1, FVector2 vector2)
{
	return vector1.GetDistance(vector2);
}

void FVector2::Print()
{
	printf("[ %f,\t%f]\n", X, Y);
}

size_t HASH_FVector2::operator()(const FVector2& vector) const
{
	std::size_t vectorX = std::hash<float>()(vector.X);
	std::size_t vectorY = std::hash<float>()(vector.Y);

	return vectorX ^ vectorY;
}