#include "MovementComponent.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Objects/GameObjects/GameObject.h"
#include "CollisionHandler.h"
#define _USE_MATH_DEFINES
#include <math.h>
C_MovementComponent::C_MovementComponent(O_GameObject* owner) : O_Component(owner)
{
	//AccelerationBuffer = new FVector3();
	velocityBuffer = new FVector3();
	//AngularAccelerationBuffer = new FVector3();
	//AngularVelocityBuffer = new FVector3();

	AlwaysFaceForward = false;
}

C_MovementComponent::~C_MovementComponent()
{
	//if (acceleration) delete(Acceleration);
	//if (velocity) delete(Velocity);
	//if (angularAcceleration) delete(AngularAcceleration);
	//if (angularVelocity) delete(AngularVelocity);
}

void C_MovementComponent::Update(const float deltaTime)
{
	//Add basic velocity movement here?
}

void C_MovementComponent::Translate(FVector3 translation_)
{
	if (!Owner->GetRoot()->GetIsStatic()) {
		Owner->SetPosition(Owner->GetPosition() + translation_);
	}
}

void C_MovementComponent::Rotate(FQuaternion rotation_)
{
	if (!Owner->GetRoot()->GetIsStatic()) {
		Owner->SetRotation(rotation_);
	}
}
