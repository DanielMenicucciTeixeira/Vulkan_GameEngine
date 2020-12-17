#include "PhysicsLib.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Math/FTransform.h"
#include "Math/FQuaternion.h"
#include "Math/FVector3.h"
#include "Math/FMatrix.h"

#define _USE_MATH_DEFINES
#include<math.h>

void FPhysicsLib::AddForce(C_PhysicsComponent* target, FVector3 force, FVector3 location)
{
	target->SetVelocity(target->GetVelocity() + (force / target->Mass));

	FVector3 torque = (location - target->GetOwner()->GetPosition()).CrossProduct(force);
	if (torque == FVector3()) return;
	FVector3 resultingVelocity = torque.CrossProduct(force);
	target->SetAngularVelocity(target->GetAngularVelocity() + torque / target->AngularInertia);
}
