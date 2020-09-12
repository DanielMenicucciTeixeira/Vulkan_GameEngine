#include "FVector3.h"
#include <cmath>

///Constructors

//Constructor using initial values for each component.
FVector3::FVector3(float inX, float inY, float inZ)
{
	x = inX;
	y = inY;
	z = inZ;
}

//Constructor initializing all components to a single float value.
FVector3::FVector3(float Float)
{
	x = y = z = Float;
}

inline FVector3::FVector3(const FVector3 & InitilizerVector)
{
	Load(InitilizerVector.x, InitilizerVector.y, InitilizerVector.z);
}

//Default Constructor, initializes all values to 0;
FVector3::FVector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
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

	Result.x = x + Vector.x;
	Result.y = y + Vector.y;
	Result.z = z + Vector.z;

	return Result;
}

//Overload of the "+=" operator, makes a vector plus vector addition changing the current vector to equal to sum.
void FVector3::operator+=(FVector3 Vector)
{
	x += Vector.x;
	y += Vector.y;
	z += Vector.z;
}

//Overload of the "-=" operator, makes a vector minus vector subtraction. changing the current vector to equal the result.
void FVector3::operator-=(FVector3 Vector)
{
	x -= Vector.x;
	y -= Vector.y;
	z -= Vector.z;
}

//Overload of the "-" operator, makes a vector minus vector subtraction.
FVector3 FVector3::operator-(FVector3 Vector)
{

	FVector3 Result;

	Result.x = x - Vector.x;
	Result.y = y - Vector.y;
	Result.z = z - Vector.z;

	return Result;
}

//Overload of the "*" operator, makes a vector times scalar multiplication.
FVector3 FVector3::operator*(float Multiplier)
{
	return FVector3(x * Multiplier, y * Multiplier, z * Multiplier);
}

//Overload of the "/" operator, makes a vector divided by scalar division.
FVector3 FVector3::operator/(float Divisor)
{
	return FVector3(x / Divisor, y / Divisor, z / Divisor);
}

//Overload of the "*" operator, makes a vector times vector dot product.
float FVector3::operator*(FVector3 Vector)
{
	return (x*Vector.x + y*Vector.y + z*Vector.z);
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void FVector3::operator=(FVector3 Vector)
{
	x = Vector.x;
	y = Vector.y;
	z = Vector.z;
}

//Overload of the "=" operator, makes each component of the vector equal to the equivalent component of a given vector.
void FVector3::operator=(FVector3 * Vector)
{
	if (Vector == nullptr) return;

	x = Vector->x;
	y = Vector->y;
	z = Vector->z;
}

/*inline FVector3 & FVector3::operator=(const FVector3 & Vector)
{
	Load(Vector.x, Vector.y, Vector.z);
	return *this;
}*/

///Functions

//Returns the magnitude of the Vector (or Vector's scalar lenght)
float FVector3::Length()
{
	return sqrt(x*x + y * y + z * z);
}

//Returns the angle between this vector and another given vector in degrees.
float FVector3::GetAngle(FVector3 Vector)
{
	FVector3 thisVector;
	thisVector.x = x;
	thisVector.y = y;
	thisVector.z = z;
	
	return acos((thisVector * Vector)/(thisVector.Length() * Vector.Length())) * 180.0f/ 3.14159265f;
}

//Returns the angle between this vector and another given vector in radians.
float FVector3::GetRadAngle(FVector3 Vector)
{
	FVector3 thisVector;
	thisVector.x = x;
	thisVector.y = y;
	thisVector.z = z;

	return acos((thisVector * Vector) / (Length() * Vector.Length()));
}

//Returns the Cross product of this vector and another given vector.
FVector3 FVector3::CrossProduct(FVector3 Vector)
{
	return FVector3(y*Vector.z - z*Vector.y, z*Vector.x - x*Vector.z, x*Vector.y - y*Vector.x);
}

//Gets the normalized form of this Vector
FVector3 FVector3::GetNormal()
{
	FVector3 ReturnVector(x, y, z);

	return ReturnVector/ReturnVector.Length();
}

//Divides the vector by its Magnitude to get the normalized unit vector.
void FVector3::Normalize()
{
	FVector3 ReturnVector(x, y, z);

	ReturnVector = ReturnVector / ReturnVector.Length();

	x = ReturnVector.x;
	y = ReturnVector.y;
	z = ReturnVector.z;
}

//Rotates the vector by X degrees in the Z axis
void FVector3::RotateZ(float Degrees)
{
	Degrees *= 0.0174533f;

	x = x * cos(Degrees) - y * sin(Degrees);
	y = x * sin(Degrees) + y * cos(Degrees);

}
//Rotates the vector by disered amount in radians in the Z axis
void FVector3::RotateZRad(float Degrees)
{
	x = x * cos(Degrees) - y * sin(Degrees);
	y = x * sin(Degrees) + y * cos(Degrees);
}

inline void FVector3::Load(float InX, float InY, float InZ)
{
	x = InX;
	y = InY;
	z = InZ;
}
