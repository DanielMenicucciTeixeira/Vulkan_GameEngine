#include "FTransform.h"
#include "FVector3.h"
#include "FQuaternion.h"
#include "FMatrix4.h"

FTransform::FTransform()
{
	Position = new FVector3();
	Rotation = new FQuaternion();
	Scale = new FVector3(1.0f);
}

FTransform::FTransform(const FTransform& transform)
{
	Position = new FVector3();
	Rotation = new FQuaternion();
	Scale = new FVector3(1.0f);
	*this = transform;
}

FTransform::FTransform(const FVector3& position, const FQuaternion& rotation, const FVector3& scale)
{
	Position = new FVector3(position);
	Rotation = new FQuaternion(rotation);
	Scale = new FVector3(scale);
}

FTransform::~FTransform()
{
	if (Position) delete(Position);
	if (Rotation) delete(Rotation);
	if (Scale) delete(Scale);
}

FMatrix4 FTransform::GetModelMatrix() const
{
	return (FMatrix4::GetTranslationMatrix(*Position) * FMatrix4::GetRotationMatrix(*Rotation) * FMatrix4::GetScalingMatrix(*Scale));
	//return (FMatrix4::GetScalingMatrix(*Scale) * FMatrix4::GetTranslationMatrix(*Position) * FMatrix4::GetRotationMatrix(*Rotation) );
}

void FTransform::operator=(const FTransform& transform)
{
	*Position = *transform.Position;
	*Rotation = *transform.Rotation;
	*Scale = *transform.Scale;
}

bool FTransform::operator==(const FTransform& transform) const
{
	return *Position == *transform.Position && *Rotation == *transform.Rotation && *Scale == *transform.Scale;
}

FTransform FTransform::operator+(const FTransform& transform) const
{
	return FTransform
	(
		FVector3(*Position + *transform.Position),
		FQuaternion((*Rotation * *transform.Rotation).GetNormal()),
		FVector3
		(
			Scale->X * transform.Scale->X,
			Scale->Y * transform.Scale->Y,
			Scale->Z * transform.Scale->Z
		)

	);
}

void FTransform::SetPosition(const FVector3& position)
{
	*Position = position;
}

void FTransform::SetRotation(const FQuaternion& rotation)
{
	*Rotation = rotation;
}

void FTransform::SetScale(const FVector3& scale)
{
	*Scale = scale;
}
