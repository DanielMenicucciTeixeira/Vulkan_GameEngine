#include "FVector3.h"
#include <cmath>

///Constructors

//Constructor using initial values for each component.
FVector3::FVector3(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

//Constructor initializing all components to a single float value.
FVector3::FVector3(float Float)
{
	X = Y = Z = Float;
}

inline FVector3::FVector3(const FVector3 & InitilizerVector)
{
	Load(InitilizerVector.X, InitilizerVector.Y, InitilizerVector.Z);
}

FVector3::FVector3(float values[3])
{
	X = values[0];
	X = values[1];
	X = values[2];
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
FVector3 FVector3::operator+(FVector3 Vector)
{
	FVector3 Result;

	Result.X = X + Vector.X;
	Result.Y = Y + Vector.Y;
	Result.Z = Z + Vector.Z;

	return Result;
}

//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
void FVector3::operator+=(FVector3 Vector)
{
	X += Vector.X;
	Y += Vector.Y;
	Z += Vector.Z;
}

//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
void FVector3::operator-=(FVector3 Vector)
{
	X -= Vector.X;
	Y -= Vector.Y;
	Z -= Vector.Z;
}

bool FVector3::operator==(FVector3 Vector)
{
	return X == Vector.X && Y == Vector.Y && Z == Vector.Z;
}

//Overload of the "-" operator, makes a vector minus vector subtraction.
FVector3 FVector3::operator-(FVector3 Vector)
{

	FVector3 Result;

	Result.X = X - Vector.X;
	Result.Y = Y - Vector.Y;
	Result.Z = Z - Vector.Z;

	return Result;
}

//Overload of the "*" operator, makes a vector times scalar multiplication.
FVector3 FVector3::operator*(float Multiplier)
{
	return FVector3(X * Multiplier, Y * Multiplier, Z * Multiplier);
}

//Overload of the "/" operator, makes a vector divided by scalar division.
FVector3 FVector3::operator/(float Divisor)
{
	return FVector3(X / Divisor, Y / Divisor, Z / Divisor);
}

//Overload of the "*" operator, makes a vector times vector dot product.
float FVector3::operator*(FVector3 Vector)
{
	return (X*Vector.X + Y*Vector.Y + Z*Vector.Z);
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void FVector3::operator=(FVector3 Vector)
{
	X = Vector.X;
	Y = Vector.Y;
	Z = Vector.Z;
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void FVector3::operator=(FVector3 * Vector)
{
	if (Vector == nullptr) return;

	X = Vector->X;
	Y = Vector->Y;
	Z = Vector->Z;
}

void FVector3::operator=(float values[3])
{
	X = values[0];
	Y = values[1];
	Z = values[2];
}

///Functions

//Returns the magnitude of the Vector (or Vector's scalar lenght)
float FVector3::Length()
{
	return sqrt(X*X + Y*Y + Z*Z);
}

//Returns the angle between this vector and another given vector in degrees.
float FVector3::GetAngle(FVector3 Vector)
{
	FVector3 thisVector;
	thisVector.X = X;
	thisVector.Y = Y;
	thisVector.Z = Z;
	
	return acos((thisVector * Vector)/(thisVector.Length() * Vector.Length())) * 180.0f/ 3.14159265f;
}

//Returns the angle between this vector and another given vector in radians.
float FVector3::GetRadAngle(FVector3 Vector)
{
	FVector3 thisVector;
	thisVector.X = X;
	thisVector.Y = Y;
	thisVector.Z = Z;

	return acos((thisVector * Vector) / (Length() * Vector.Length()));
}

//Returns the Cross product of this vector and another given vector.
FVector3 FVector3::CrossProduct(FVector3 Vector)
{
	return FVector3(Y*Vector.Z - Z*Vector.Y, Z*Vector.X - X*Vector.Z, X*Vector.Y - Y*Vector.X);
}

//Gets the normalized form of this Vector
FVector3 FVector3::GetNormal()
{

	return *this/Length();
}

//Divides the vector by its Magnitude to get the normalized unit vector.
void FVector3::Normalize()
{
	*this = *this/Length();
}

//Rotates the vector by X degrees in the Z axis
void FVector3::RotateZ(float Degrees)
{
	Degrees *= 0.0174533f;

	X = X * cos(Degrees) - Y * sin(Degrees);
	Y = X * sin(Degrees) + Y * cos(Degrees);

}
//Rotates the vector by disered amount in radians in the Z axis
void FVector3::RotateZRad(float Degrees)
{
	X = X * cos(Degrees) - Y * sin(Degrees);
	Y = X * sin(Degrees) + Y * cos(Degrees);
}

inline void FVector3::Load(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}
