#include "PhysicsComponent.h"
#include "MovementComponent.h"
#include "Objects/GameObjects/GameObject.h"
#include "Physics/PhysicsLib.h"
#include "Math/FQuaternion.h"
#include "Math/FTransform.h"
#define _USE_MATH_DEFINES
#include <math.h>

void C_PhysicsComponent::Update(const float deltaTime)
{
	//C_MovementComponent::Update(deltaTime);
	//*Acceleration = FVector3();
	//*AngularAcceleration = FQuaternion();

	//FVector3 displacement = velocityBuffer * deltaTime + ((accelerationBuffer * (deltaTime * deltaTime)) / 2.0f);

	//FQuaternion VelocityQuat = FQuaternion(angularVelocityBuffer.X * (M_PI / 180.0f), angularVelocityBuffer.Y * (M_PI / 180.0f), angularVelocityBuffer.Z * (M_PI / 180.0f), 0.0f);
	//FQuaternion AccelerationQuat = FQuaternion(angularAccelerationBuffer.X * (M_PI / 180.0f), angularAccelerationBuffer.Y * (M_PI / 180.0f), angularAccelerationBuffer.Z * (M_PI / 180.0f), 0.0f);


	//If owner has movement as well translate them accordingly.
	for (C_MovementComponent * c : Owner->GetComponentsOfClass<C_MovementComponent>()) {
		c->Translate(velocityBuffer * deltaTime + ((accelerationBuffer * (deltaTime * deltaTime)) / 2.0f));

		c->Rotate((
			Owner->GetRotation() +
			FQuaternion(angularVelocityBuffer.X * (M_PI / 180.0f), angularVelocityBuffer.Y * (M_PI / 180.0f), angularVelocityBuffer.Z * (M_PI / 180.0f), 0.0f) * Owner->GetRotation() * 0.5f * deltaTime +
			FQuaternion(angularAccelerationBuffer.X * (M_PI / 180.0f), angularAccelerationBuffer.Y * (M_PI / 180.0f), angularAccelerationBuffer.Z * (M_PI / 180.0f), 0.0f) * Owner->GetRotation() * 0.25f * deltaTime * deltaTime
			).GetNormal());
	}
	velocity = velocityBuffer + (accelerationBuffer * deltaTime);
	acceleration = accelerationBuffer;
	angularAcceleration = angularAccelerationBuffer;
	AddAngularVelocity(angularAcceleration * deltaTime);
	angularVelocity = angularVelocityBuffer;
}

void C_PhysicsComponent::AddAcceleration(FVector3 acceleration_)
{
	accelerationBuffer += acceleration_;
}

void C_PhysicsComponent::AddVelocity(FVector3 velocity_)
{
	velocityBuffer += velocity_;
}

void C_PhysicsComponent::AddAngularAcceleration(FVector3 angularAcceleration_)
{
	angularAccelerationBuffer += angularAcceleration_;
}

void C_PhysicsComponent::AddAngularVelocity(FVector3 angularVelocity_)
{
	angularVelocityBuffer += angularVelocity_;
}

void C_PhysicsComponent::SetAcceleration(FVector3 acceleration_)
{
	acceleration = acceleration_;
}

void C_PhysicsComponent::SetVelocity(FVector3 velocity_)
{
	velocity = velocity_;
}

void C_PhysicsComponent::SetAngularAcceleration(FVector3 angularAcceleration_)
{
	angularAcceleration += angularAcceleration_;
}

void C_PhysicsComponent::SetAngularVelocity(FVector3 angularVelocity_)
{
	angularVelocity = angularVelocity_;
}

FVector3 C_PhysicsComponent::GetAcceleration()
{
	return acceleration;
}

FVector3 C_PhysicsComponent::GetVelocity()
{
	return velocity;
}

FVector3 C_PhysicsComponent::GetAngularAcceleration()
{
	return angularAcceleration;
}

FVector3 C_PhysicsComponent::GetAngularVelocity()
{
	return angularVelocity;
}

void C_PhysicsComponent::CalculateAngularInertia()
{
	//if (!MeshComponent) return;
}

void C_PhysicsComponent::CalculateCenterOfMass()
{
	//if (!MeshComponent)
	//{
	//	UseCenterOfMass = false;
	//	CenterOfMass = Owner->GetTransformReference()->Position;
	//}
}

FVector3 C_PhysicsComponent::GetCenterOfMass()
{
	return *CenterOfMass;
}

C_PhysicsComponent::C_PhysicsComponent(O_GameObject* owner, float mass, bool useCenterOfMass, float angularInertia, bool useCalculatedAngularIntertia) : O_Component(owner)
{
	Mass = mass;
	if (UseCenterOfMass) CalculateCenterOfMass();
	else CenterOfMass = Owner->GetTransformReference()->Position;

	//if (useCalculatedAngularIntertia && MeshComponent) CalculateAngularInertia();
	//else AngularInertia = angularInertia;
}

C_PhysicsComponent::~C_PhysicsComponent()
{
	if (UseCenterOfMass) delete(CenterOfMass);
}
