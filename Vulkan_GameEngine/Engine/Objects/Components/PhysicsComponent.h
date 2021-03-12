#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "MovementComponent.h"

class O_GameObject;

class FMatrix;
class FVector3;
class C_StaticMeshComponent;

class C_PhysicsComponent : public C_MovementComponent
{
protected:

	//requires a C_StaticMeshComponent set as MeshComponent
	void CalculateAngularInertia();

	//requires a C_StaticMeshComponent set as MeshComponent
	void CalculateCenterOfMass();

	FVector3* CenterOfMass;
	bool UseCenterOfMass;

public:
	C_StaticMeshComponent* MeshComponent;
	float Mass;
	float AngularInertia;
	FVector3 GetCenterOfMass();
	C_PhysicsComponent(O_GameObject* owner, float mass = 1.0f, bool useCenterOfMass = false, float angularInertia = 1.0f, bool useCalculatedAngularIntertia = false);
	~C_PhysicsComponent();

	void Update(const float deltaTime) override;
};
#endif
