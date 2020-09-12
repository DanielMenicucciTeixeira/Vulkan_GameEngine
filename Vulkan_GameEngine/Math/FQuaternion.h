#ifndef FQuaternion_
#define FQuaternion_
#include "FVector3.h"

class FQuaternion
{
public:

	//Axis Vector component x.
	float x;

	//Axis Vector component y.
	float y;

	//Axis Vector component z.
	float z;

	//Quaternion Scalar component;
	float w;

	///Constructors

	//Default constructor, initilizes all components to 0.
	FQuaternion();

	//Initializes each component to a given value.
	FQuaternion(float X, float Y, float Z, float W);

	//Initializes x, y and z to according to given FVector3 compoents and w to a given value.
	FQuaternion(FVector3 Axis, float W);

	//Initializes x, y and z to according to given FVector3 compoents and w to 0.
	FQuaternion(FVector3 Axis);

	///Destructors

	~FQuaternion();

	///Operators
	//TODO implement operators (FQuaternionClass)

	//Adds two quaternions
	FQuaternion operator+ (FQuaternion Quaternion);

	//Subtracts two quaternions
	FQuaternion operator- (FQuaternion Quaternion);

	//Multiplies two quaternions
	FQuaternion operator* (FQuaternion Quaternion);

	///Functions

	//Returns the conjugate Quaternion ([w, -v]).
	FQuaternion Conjugate();//TODO implement Conjugate function (FQuaternionClass)

	//Returns this quaternions norm (w2 + x2 + y2 + z2).
	float Norm();//TODO implement Norm function (FQuaternionClass)

	//Returns this quaternions inverse (Conjugate/Norm).
	FQuaternion Inverse();//TODO implement Inverse function (FQuaternionClass)

};
#endif
