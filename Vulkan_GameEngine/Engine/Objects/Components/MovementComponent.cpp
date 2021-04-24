#include "MovementComponent.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Objects/GameObjects/GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
C_MovementComponent::C_MovementComponent(O_GameObject* owner) : O_Component(owner)
{
	Acceleration = new FVector3();
	Velocity = new FVector3();
	AngularAcceleration = new FVector3();
	AngularVelocity = new FVector3();

	AccelerationBuffer = new FVector3();
	VelocityBuffer = new FVector3();
	AngularAccelerationBuffer = new FVector3();
	AngularVelocityBuffer = new FVector3();

	AlwaysFaceForward = false;
}

C_MovementComponent::~C_MovementComponent()
{
	if (Acceleration) delete(Acceleration);
	if (Velocity) delete(Velocity);
	if (AngularAcceleration) delete(AngularAcceleration);
	if (AngularVelocity) delete(AngularVelocity);

	if (AccelerationBuffer) delete(AccelerationBuffer);
	if (VelocityBuffer) delete(VelocityBuffer);
	if (AngularAccelerationBuffer) delete(AngularAccelerationBuffer);
	if (AngularVelocityBuffer) delete(AngularVelocityBuffer);
}

void C_MovementComponent::Update(const float deltaTime)//TODO implement rotation to always face forward
{
	FVector3 displacement = *VelocityBuffer * deltaTime + ((*AccelerationBuffer * (deltaTime * deltaTime)) / 2.0f);
	Owner->SetPosition(Owner->GetPosition() + displacement);
	*VelocityBuffer = *VelocityBuffer + (*AccelerationBuffer * deltaTime);

	FVector3 rotation = *AngularVelocityBuffer * deltaTime + ((*AngularAccelerationBuffer * (deltaTime * deltaTime))/ 2.0f);
	if (rotation != FVector3())
	{
		FQuaternion result = FQuaternion(rotation.GetNormal(), rotation.Length() / 2.0 * M_PI, true, true);
		result.Normalize();
		Owner->SetRotation((Owner->GetRotation() * result).GetNormal());
	}

	*Velocity = *VelocityBuffer;
	*Acceleration = *AccelerationBuffer;
	*AngularVelocity = *AngularVelocityBuffer;
	*AngularAcceleration = *AngularAccelerationBuffer;

}

void C_MovementComponent::SetAcceleration(FVector3 acceleration)
{
	*AccelerationBuffer = acceleration;
}

void C_MovementComponent::SetVelocity(FVector3 velocity)
{
	*VelocityBuffer = velocity;
}

void C_MovementComponent::SetAngularAcceleration(FVector3 angularAcceleration)
{
	*AngularAccelerationBuffer = angularAcceleration;
}

void C_MovementComponent::SetAngularVelocity(FVector3 angularVelocity)
{
	*AngularVelocityBuffer = angularVelocity;
}

FVector3 C_MovementComponent::GetAcceleration()
{
	return *Acceleration;
}

FVector3 C_MovementComponent::GetVelocity()
{
	return *Velocity;
}

FVector3 C_MovementComponent::GetAngularAcceleration()
{
	return *AngularAcceleration;
}

FVector3 C_MovementComponent::GetAngularVelocity()
{
	return *AngularVelocity;
}
