#include "FQuaternion.h"

///Constructors

//Default constructor, initilizes all components to 0.
FQuaternion::FQuaternion()
{
	x = y = z = w = 0.0f;
}

//Initializes each component to a given value.
FQuaternion::FQuaternion(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

//Initializes x, y and z to according to given FVector3 compoents and w to a given value.
FQuaternion::FQuaternion(FVector3 Axis, float W)
{
	x = Axis.x;
	y = Axis.y;
	z = Axis.z;
	w = W;
}

//Initializes x, y and z to according to given FVector3 compoents and w to 0.
FQuaternion::FQuaternion(FVector3 Axis)
{
	x = Axis.x;
	y = Axis.y;
	z = Axis.z;

	w = 0.0f;
}

///Destructors

FQuaternion::~FQuaternion()
{
}

///Operators

//Adds two quaternions
FQuaternion FQuaternion::operator+(FQuaternion Quaternion)
{
	return FQuaternion();
}

//Subtracts two quaternions
FQuaternion FQuaternion::operator-(FQuaternion Quaternion)
{
	return FQuaternion();
}

//Multiplies two quaternions
FQuaternion FQuaternion::operator*(FQuaternion Quaternion)
{
	return FQuaternion();
}

///Functions

//Returns the conjugate Quaternion ([w, -v]).
FQuaternion FQuaternion::Conjugate()
{
	return FQuaternion();
}

//Returns this quaternions norm (w2 + x2 + y2 + z2).
float FQuaternion::Norm()
{
	return 0.0f;
}

//Returns this quaternions inverse (Conjugate/Norm).
FQuaternion FQuaternion::Inverse()
{
	return FQuaternion();
}
