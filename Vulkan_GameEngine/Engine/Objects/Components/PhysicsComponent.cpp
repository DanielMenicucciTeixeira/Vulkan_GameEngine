#include "PhysicsComponent.h"
#include "Objects/GameObjects/GameObject.h"
#include "Physics/PhysicsLib.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Math/FTransform.h"

void C_PhysicsComponent::Update(const float deltaTime)
{
	C_MovementComponent::Update(deltaTime);
	*Acceleration = FVector3();
	*AngularAcceleration = FQuaternion();
}

void C_PhysicsComponent::CalculateAngularInertia()
{
	if (!MeshComponent) return;
}

void C_PhysicsComponent::CalculateCenterOfMass()
{
	if (!MeshComponent)
	{
		UseCenterOfMass = false;
		CenterOfMass = Owner->GetTransformReference()->Position;
	}
}

FVector3 C_PhysicsComponent::GetCenterOfMass()
{
	return *CenterOfMass;
}

C_PhysicsComponent::C_PhysicsComponent(O_GameObject* owner, float mass, bool useCenterOfMass, float angularInertia, bool useCalculatedAngularIntertia) : C_MovementComponent(owner)
{
	Mass = mass;
	if (UseCenterOfMass) CalculateCenterOfMass();
	else CenterOfMass = Owner->GetTransformReference()->Position;
	if (useCalculatedAngularIntertia && MeshComponent) CalculateAngularInertia();
	else AngularInertia = angularInertia;

}

C_PhysicsComponent::~C_PhysicsComponent()
{
	if (UseCenterOfMass) delete(CenterOfMass);
}
