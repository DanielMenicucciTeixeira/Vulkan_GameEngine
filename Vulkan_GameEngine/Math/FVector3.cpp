#include "FVector3.h"
#include <cmath>

///Constructors

//Constructor using initial values for each component.
FVector3::FVector3(float x, float y, float z)
{
	VectorArray[0] = x;
	VectorArray[1] = y;
	VectorArray[2] = z;
}

//Constructor initializing all components to a single float value.
FVector3::FVector3(float Float)
{
	VectorArray[0] = VectorArray[1] = VectorArray[2] = Float;
}

inline FVector3::FVector3(const FVector3 & InitilizerVector)
{
	Load(InitilizerVector.VectorArray[0], InitilizerVector.VectorArray[1], InitilizerVector.VectorArray[2]);
}

FVector3::FVector3(float array[3])
{
	VectorArray[0] = array[0];
	VectorArray[0] = array[1];
	VectorArray[0] = array[2];
}

//Default Constructor, initializes all values to 0;
FVector3::FVector3()
{
	VectorArray[0] = 0.0f;
	VectorArray[1] = 0.0f;
	VectorArray[2] = 0.0f;
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

	Result.VectorArray[0] = VectorArray[0] + Vector.VectorArray[0];
	Result.VectorArray[1] = VectorArray[1] + Vector.VectorArray[1];
	Result.VectorArray[2] = VectorArray[2] + Vector.VectorArray[2];

	return Result;
}

//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
void FVector3::operator+=(FVector3 Vector)
{
	VectorArray[0] += Vector.VectorArray[0];
	VectorArray[1] += Vector.VectorArray[1];
	VectorArray[2] += Vector.VectorArray[2];
}

//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
void FVector3::operator-=(FVector3 Vector)
{
	VectorArray[0] -= Vector.VectorArray[0];
	VectorArray[1] -= Vector.VectorArray[1];
	VectorArray[2] -= Vector.VectorArray[2];
}

bool FVector3::operator==(FVector3 Vector)
{
	return VectorArray[0] == Vector.VectorArray[0] && VectorArray[1] == Vector.VectorArray[1] && VectorArray[2] == Vector.VectorArray[2];
}

//Overload of the "-" operator, makes a vector minus vector subtraction.
FVector3 FVector3::operator-(FVector3 Vector)
{

	FVector3 Result;

	Result.VectorArray[0] = VectorArray[0] - Vector.VectorArray[0];
	Result.VectorArray[1] = VectorArray[1] - Vector.VectorArray[1];
	Result.VectorArray[2] = VectorArray[2] - Vector.VectorArray[2];

	return Result;
}

//Overload of the "*" operator, makes a vector times scalar multiplication.
FVector3 FVector3::operator*(float Multiplier)
{
	return FVector3(VectorArray[0] * Multiplier, VectorArray[1] * Multiplier, VectorArray[2] * Multiplier);
}

//Overload of the "/" operator, makes a vector divided by scalar division.
FVector3 FVector3::operator/(float Divisor)
{
	return FVector3(VectorArray[0] / Divisor, VectorArray[1] / Divisor, VectorArray[2] / Divisor);
}

//Overload of the "*" operator, makes a vector times vector dot product.
float FVector3::operator*(FVector3 Vector)
{
	return (VectorArray[0]*Vector.VectorArray[0] + VectorArray[1]*Vector.VectorArray[1] + VectorArray[2]*Vector.VectorArray[2]);
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void FVector3::operator=(FVector3 Vector)
{
	VectorArray[0] = Vector.VectorArray[0];
	VectorArray[1] = Vector.VectorArray[1];
	VectorArray[2] = Vector.VectorArray[2];
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void FVector3::operator=(FVector3 * Vector)
{
	if (Vector == nullptr) return;

	VectorArray[0] = Vector->VectorArray[0];
	VectorArray[1] = Vector->VectorArray[1];
	VectorArray[2] = Vector->VectorArray[2];
}

///Functions

//Returns the magnitude of the Vector (or Vector's scalar lenght)
float FVector3::Length()
{
	return sqrt(VectorArray[0]*VectorArray[0] + VectorArray[1] * VectorArray[1] + VectorArray[2] * VectorArray[2]);
}

//Returns the angle between this vector and another given vector in degrees.
float FVector3::GetAngle(FVector3 Vector)
{
	FVector3 thisVector;
	thisVector.VectorArray[0] = VectorArray[0];
	thisVector.VectorArray[1] = VectorArray[1];
	thisVector.VectorArray[2] = VectorArray[2];
	
	return acos((thisVector * Vector)/(thisVector.Length() * Vector.Length())) * 180.0f/ 3.14159265f;
}

//Returns the angle between this vector and another given vector in radians.
float FVector3::GetRadAngle(FVector3 Vector)
{
	FVector3 thisVector;
	thisVector.VectorArray[0] = VectorArray[0];
	thisVector.VectorArray[1] = VectorArray[1];
	thisVector.VectorArray[2] = VectorArray[2];

	return acos((thisVector * Vector) / (Length() * Vector.Length()));
}

//Returns the Cross product of this vector and another given vector.
FVector3 FVector3::CrossProduct(FVector3 Vector)
{
	return FVector3(VectorArray[1]*Vector.VectorArray[2] - VectorArray[2]*Vector.VectorArray[1], VectorArray[2]*Vector.VectorArray[0] - VectorArray[0]*Vector.VectorArray[2], VectorArray[0]*Vector.VectorArray[1] - VectorArray[1]*Vector.VectorArray[0]);
}

//Gets the normalized form of this Vector
FVector3 FVector3::GetNormal()
{
	FVector3 ReturnVector(VectorArray[0], VectorArray[1], VectorArray[2]);

	return ReturnVector/ReturnVector.Length();
}

//Divides the vector by its Magnitude to get the normalized unit vector.
void FVector3::Normalize()
{
	FVector3 ReturnVector(VectorArray[0], VectorArray[1], VectorArray[2]);

	ReturnVector = ReturnVector / ReturnVector.Length();

	VectorArray[0] = ReturnVector.VectorArray[0];
	VectorArray[1] = ReturnVector.VectorArray[1];
	VectorArray[2] = ReturnVector.VectorArray[2];
}

//Rotates the vector by VectorArray[0] degrees in the VectorArray[2] axis
void FVector3::RotateZ(float Degrees)
{
	Degrees *= 0.0174533f;

	VectorArray[0] = VectorArray[0] * cos(Degrees) - VectorArray[1] * sin(Degrees);
	VectorArray[1] = VectorArray[0] * sin(Degrees) + VectorArray[1] * cos(Degrees);

}
//Rotates the vector by disered amount in radians in the VectorArray[2] axis
void FVector3::RotateZRad(float Degrees)
{
	VectorArray[0] = VectorArray[0] * cos(Degrees) - VectorArray[1] * sin(Degrees);
	VectorArray[1] = VectorArray[0] * sin(Degrees) + VectorArray[1] * cos(Degrees);
}

inline void FVector3::Load(float x, float y, float z)
{
	VectorArray[0] = x;
	VectorArray[1] = y;
	VectorArray[2] = z;
}
