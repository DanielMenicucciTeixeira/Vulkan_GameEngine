#include "FTransform.h"

FTransform::FTransform()
{
	Position = FVector3(0, 0, 0);
	Scale = FVector3(0, 0, 0);
	Rotation = FQuaternion(0, 0, 0, 0);
}

FTransform::FTransform(FVector3 InPosition, FVector3 InScale, FQuaternion InRotation)
{
	Position = InPosition;
	Scale = InScale;
	Rotation = InRotation;
}

FTransform::~FTransform()
{
}
