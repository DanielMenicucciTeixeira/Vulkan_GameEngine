#include "IVector3.h"
#include "FQuaternion.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <type_traits>

///Constructors

//Constructor using initial values for each component.
IVector3::IVector3(int x, int y, int z)
{
	X = x;
	Y = y;
	Z = z;
}

//Constructor initializing all components to a single int value.
IVector3::IVector3(int value)
{
	X = value;
	Y = value;
	Z = value;
}

inline IVector3::IVector3(const IVector3& vector)
{
	Load(vector.X, vector.Y, vector.Z);
}

IVector3::IVector3(int values[3])
{
	X = values[0];
	Y = values[1];
	Z = values[2];
}

//Default Constructor, initializes all values to 0;
IVector3::IVector3()
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

///Destructors

IVector3::~IVector3()
{
}

///Operators

//Overload of the "+" operator, makes a vector plus vector addition.
IVector3 IVector3::operator+(const IVector3& vector) const
{
	IVector3 result;

	result.X = X + vector.X;
	result.Y = Y + vector.Y;
	result.Z = Z + vector.Z;

	return result;
}

//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
void IVector3::operator+=(const IVector3& vector)
{
	X += vector.X;
	Y += vector.Y;
	Z += vector.Z;
}

//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
void IVector3::operator-=(const IVector3& vector)
{
	X -= vector.X;
	Y -= vector.Y;
	Z -= vector.Z;
}

bool IVector3::operator==(const IVector3& vector) const
{
	return X == vector.X && Y == vector.Y && Z == vector.Z;
}

//Overload of the "-" operator, makes a vector minus vector subtraction.
IVector3 IVector3::operator-(const IVector3& vector) const
{

	IVector3 Result;

	Result.X = X - vector.X;
	Result.Y = Y - vector.Y;
	Result.Z = Z - vector.Z;

	return Result;
}

//Overload of the "*" operator, makes a vector times scalar multiplication.
IVector3 IVector3::operator*(int multiplier) const
{
	return IVector3(X * multiplier, Y * multiplier, Z * multiplier);
}

//Overload of the "/" operator, makes a vector divided by scalar division.
IVector3 IVector3::operator/(int divisor) const
{
	return IVector3(X / divisor, Y / divisor, Z / divisor);
}

//Overload of the "*" operator, makes a vector times vector dot product.
int IVector3::operator*(const IVector3& vector) const
{
	return (X * vector.X + Y * vector.Y + Z * vector.Z);
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void IVector3::operator=(const IVector3& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
}

void IVector3::operator=(int values[3])
{
	X = values[0];
	Y = values[1];
	Z = values[2];
}

///Functions

//Returns the magnitude of the vector (or vector's scalar lenght)
int IVector3::Length() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

//Returns the angle between this vector and another given vector in degrees.
int IVector3::GetAngle(const IVector3& vector, bool inRadians)
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
IVector3 IVector3::CrossProduct(const IVector3& vector)
{
	return IVector3(Y * vector.Z - Z * vector.Y, Z * vector.X - X * vector.Z, X * vector.Y - Y * vector.X);
}

//Gets the normalized form of this vector
IVector3 IVector3::GetNormal() const
{

	return *this / Length();
}

//Divides the vector by its Magnitude to get the normalized unit vector.
void IVector3::Normalize()
{
	*this = *this / Length();
}

void IVector3::Rotate(const FQuaternion& rotation)
{
	// Extract the vector part of the quaternion
	IVector3 vector(rotation.X, rotation.Y, rotation.Z);

	// Extract the scalar part of the quaternion
	int scalar = rotation.W;

	// Do the math
	*this = (vector * (2.0f * (vector * *this)))
		+ (*this * (scalar * scalar - (vector * vector)))
		+ (vector.CrossProduct(*this) * 2.0f * scalar);
}

IVector3 IVector3::GetRotatedVector(const FQuaternion& rotation) const
{
	IVector3 temp = *this;
	temp.Rotate(rotation);
	return temp;
}

inline void IVector3::Load(int x, int y, int z)
{
	X = x;
	Y = y;
	Z = z;
}

void IVector3::Print()
{
	printf("[ %f,\t%f,\t%f ]\n", X, Y, Z);
}

size_t HASH_FVector3::operator()(const IVector3& vector) const
{
	std::size_t vectorX = std::hash<int>()(vector.X);
	std::size_t vectorY = std::hash<int>()(vector.Y);
	std::size_t vectorZ = std::hash<int>()(vector.Z);

	return vectorX ^ vectorY ^ vectorZ;
}
