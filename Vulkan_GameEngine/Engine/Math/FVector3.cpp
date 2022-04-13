#include "FVector3.h"
#include "FQuaternion.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <type_traits>

///Constructors

//Constructor using initial values for each component.
FVector3::FVector3(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

//Constructor initializing all components to a single float value.
FVector3::FVector3(float value)
{
	X = value;
	Y = value;
	Z = value;
}

inline FVector3::FVector3(const FVector3& vector)
{
	Load(vector.X, vector.Y, vector.Z);
}

FVector3::FVector3(const FVector4& vector)
{
	Load(vector.X, vector.Y, vector.Z);
}

FVector3::FVector3(float values[3])
{
	X = values[0];
	Y = values[1];
	Z = values[2];
}

//Default Constructor, initializes all values to 0;
FVector3::FVector3()
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

///Destructors

FVector3::~FVector3()
{
}

///Operators

//Overload of the "+" operator, makes a vector plus vector addition.
FVector3 FVector3::operator+(const FVector3& vector) const
{
	FVector3 result;

	result.X = X + vector.X;
	result.Y = Y + vector.Y;
	result.Z = Z + vector.Z;

	return result;
}

//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
void FVector3::operator+=(const FVector3& vector)
{
	X += vector.X;
	Y += vector.Y;
	Z += vector.Z;
}

//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
void FVector3::operator-=(const FVector3& vector)
{
	X -= vector.X;
	Y -= vector.Y;
	Z -= vector.Z;
}

bool FVector3::operator==(const FVector3& vector) const
{
	return X == vector.X && Y == vector.Y && Z == vector.Z;
}

//Overload of the "-" operator, makes a vector minus vector subtraction.
FVector3 FVector3::operator-(const FVector3& vector) const
{

	FVector3 Result;

	Result.X = X - vector.X;
	Result.Y = Y - vector.Y;
	Result.Z = Z - vector.Z;

	return Result;
}

//Overload of the "*" operator, makes a vector times scalar multiplication.
FVector3 FVector3::operator*(float multiplier) const
{
	return FVector3(X * multiplier, Y * multiplier, Z * multiplier);
}

//Overload of the "/" operator, makes a vector divided by scalar division.
FVector3 FVector3::operator/(float divisor) const
{
	return FVector3(X / divisor, Y / divisor, Z / divisor);
}

//Overload of the "*" operator, makes a vector times vector dot product.
float FVector3::operator*(const FVector3& vector) const
{
	return (X*vector.X + Y*vector.Y + Z*vector.Z);
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void FVector3::operator=(const FVector3& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
}

void FVector3::operator=(const FVector4& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
}

void FVector3::operator=(float values[3])
{
	X = values[0];
	Y = values[1];
	Z = values[2];
}

///Functions

//Returns the magnitude of the vector (or vector's scalar lenght)
float FVector3::Length() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

//Returns the angle between this vector and another given vector in degrees.
float FVector3::GetAngle(const FVector3& vector, bool inRadians)
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
FVector3 FVector3::CrossProduct(const FVector3& vector)
{
	return FVector3(Y*vector.Z - Z*vector.Y, Z*vector.X - X*vector.Z, X*vector.Y - Y*vector.X);
}

//Gets the normalized form of this vector
FVector3 FVector3::GetNormal() const
{

	return *this/Length();
}

//Divides the vector by its Magnitude to get the normalized unit vector.
void FVector3::Normalize()
{
	*this = *this/Length();
}

void FVector3::Rotate(const FQuaternion& rotation)
{
	// Extract the vector part of the quaternion
	FVector3 vector(rotation.X, rotation.Y, rotation.Z);

	// Extract the scalar part of the quaternion
	float scalar = rotation.W;

	// Do the math
	*this =   (vector * (2.0f * (vector * *this )))
			+ (*this * (scalar * scalar - (vector * vector)))
			+ (vector.CrossProduct(*this) * 2.0f * scalar);
}

FVector3 FVector3::GetRotatedVector(const FQuaternion& rotation) const
{
	FVector3 temp = *this;
	temp.Rotate(rotation);
	return temp;
}

inline void FVector3::Load(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

void FVector3::Print()
{
	printf("[ %f,\t%f,\t%f ]\n", X, Y, Z);
}

size_t HASH_FVector3::operator()(const FVector3& vector) const
{
	std::size_t vectorX = std::hash<float>()(vector.X);
	std::size_t vectorY = std::hash<float>()(vector.Y);
	std::size_t vectorZ = std::hash<float>()(vector.Z);

	return vectorX ^ vectorY ^ vectorZ;
}
