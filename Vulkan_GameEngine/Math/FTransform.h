#ifndef FTransform_
#define FTransform_

#include "FQuaternion.h"
#include "FVector3.h"

class FTransform
{
public:
	//Constructors

	FTransform();

	FTransform(FVector3 InPosition, FVector3 InScale = FVector3(0, 0, 0), FQuaternion InRotation = FQuaternion(0, 0, 0, 0));

	~FTransform();

	FVector3 Position;
	FVector3  Scale;
	FQuaternion Rotation;

};
#endif