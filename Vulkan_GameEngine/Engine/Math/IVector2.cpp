#include "IVector2.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

///Constructors

//Constructor using initial values for each component.
IVector2::IVector2(int x, int y)
{
	X = x;
	Y = y;
}

//Constructor initializing all components to a single int value.
IVector2::IVector2(int value)
{
	X = value;
	Y = value;
}

inline IVector2::IVector2(const IVector2& vector)
{
	X = vector.X;
	Y = vector.Y;
}

IVector2::IVector2(int values[2])
{
	X = values[0];
	Y = values[1];
}

//Default Constructor, initializes all values to 0;
IVector2::IVector2()
{
	X = 0.0f;
	Y = 0.0f;
}

///Destructors

IVector2::~IVector2()
{
}

///Operators

//Overload of the "+" operator, makes a vector plus vector addition.
IVector2 IVector2::operator+(const IVector2& vector) const
{
	IVector2 result;

	result.X = X + vector.X;
	result.Y = Y + vector.Y;

	return result;
}

//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
void IVector2::operator+=(const IVector2& vector)
{
	X += vector.X;
	Y += vector.Y;
}

//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
void IVector2::operator-=(const IVector2& vector)
{
	X -= vector.X;
	Y -= vector.Y;
}

bool IVector2::operator==(const IVector2& vector) const
{
	return X == vector.X && Y == vector.Y;
}

//Overload of the "-" operator, makes a vector minus vector subtraction.
IVector2 IVector2::operator-(const IVector2& vector) const
{

	IVector2 Result;

	Result.X = X - vector.X;
	Result.Y = Y - vector.Y;

	return Result;
}

//Overload of the "*" operator, makes a vector times scalar multiplication.
IVector2 IVector2::operator*(int multiplier) const
{
	return IVector2(X * multiplier, Y * multiplier);
}

//Overload of the "/" operator, makes a vector divided by scalar division.
IVector2 IVector2::operator/(int divisor) const
{
	return IVector2(X / divisor, Y / divisor);
}

//Overload of the "*" operator, makes a vector times vector dot product.
int IVector2::operator*(const IVector2& vector) const
{
	return (X * vector.X + Y * vector.Y);
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void IVector2::operator=(const IVector2& vector)
{
	X = vector.X;
	Y = vector.Y;
}

void IVector2::operator=(int values[3])
{
	X = values[0];
	Y = values[1];
}

///Functions

//Returns the magnitude of the vector (or vector's scalar lenght)
int IVector2::Length() const
{
	return sqrt(X * X + Y * Y);
}

//Returns the angle between this vector and another given vector in degrees.
int IVector2::GetAngle(const IVector2& vector, bool inRadians)
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
IVector2 IVector2::CrossProduct(const IVector2& vector)
{
	return IVector2(X * vector.Y - Y * vector.X);
}

//Gets the normalized form of this vector
IVector2 IVector2::GetNormal() const
{

	return *this / Length();
}

//Divides the vector by its Magnitude to get the normalized unit vector.
void IVector2::Normalize()
{
	*this = *this / Length();
}

void IVector2::Rotate(int angle, bool isRadian)
{
	if (!isRadian) angle *= M_PI / 180.0f;
	int x = X * cos(angle) - Y * sin(angle);
	int y = X * sin(angle) + Y * cos(angle);
	X = x;
	Y = y;
}

IVector2 IVector2::GetVectorByAngle(int angle, bool isRadian)
{
	IVector2 returnVector(0.0f, 1.0f);
	returnVector.Rotate(angle, isRadian);
	return returnVector.GetNormal();
}

int IVector2::GetDistance(IVector2 vector) const
{
	return (*this - vector).Length();
}

int IVector2::GetDistance(IVector2 vector1, IVector2 vector2)
{
	return vector1.GetDistance(vector2);
}

void IVector2::Print()
{
	printf("[ %f,\t%f]\n", X, Y);
}

size_t HASH_IVector2::operator()(const IVector2& vector) const
{
	std::size_t vectorX = std::hash<int>()(vector.X);
	std::size_t vectorY = std::hash<int>()(vector.Y);

	return vectorX ^ vectorY;
}