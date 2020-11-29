#include "MovementComponent.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "GameObject.h"

C_MovementComponent::C_MovementComponent(O_GameObject* owner) : O_Component(owner)
{
	Acceleration = new FVector3();
	Velocity = new FVector3();
	AlwaysFaceForward = false;
}

C_MovementComponent::~C_MovementComponent()
{
	if (Acceleration) delete(Acceleration);
	if (Velocity) delete(Velocity);
}

void C_MovementComponent::Update(float deltaTime)//TODO implement rotation to always face forward
{
	FVector3 displacement = *Velocity * deltaTime + ((*Acceleration * (deltaTime * deltaTime)) / 2.0f);
	Owner->SetPosition(Owner->GetPosition() + displacement);
	*Velocity = *Velocity + (*Acceleration * deltaTime);
}

FVector3 C_MovementComponent::GetAcceleration()
{
	return FVector3(*Acceleration);
}

FVector3 C_MovementComponent::GetVelocity()
{
	return FVector3(*Velocity);
}

void C_MovementComponent::SetAcceleration(FVector3 acceleration)
{
	*Acceleration = acceleration;
}

void C_MovementComponent::SetVelocity(FVector3 velocity)
{
	*Velocity = velocity;
}
