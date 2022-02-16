#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "MovementComponent.h"
#include "Math/FVector3.h"
class O_GameObject;

class FMatrix;
//class FVector3;
class C_StaticMeshComponent;


class C_PhysicsComponent : public O_Component
{

private:
	FVector3 acceleration;
	FVector3 velocity;
	FVector3 angularAcceleration;
	FVector3 angularVelocity;

	FVector3 accelerationBuffer;
	FVector3 velocityBuffer;
	FVector3 angularAccelerationBuffer;
	FVector3 angularVelocityBuffer;

protected:

	//requires a C_StaticMeshComponent set as MeshComponent
	void CalculateAngularInertia();

	//requires a C_StaticMeshComponent set as MeshComponent
	void CalculateCenterOfMass();

	FVector3* CenterOfMass;
	bool UseCenterOfMass;

public:
	float Mass;
	float AngularInertia;
	FVector3 GetCenterOfMass();
	C_PhysicsComponent(O_GameObject* owner, float mass = 1.0f, bool useCenterOfMass = false, float angularInertia = 1.0f, bool useCalculatedAngularIntertia = false);
	virtual ~C_PhysicsComponent();

	void Update(const float deltaTime) override;

	//Add Functions

	void AddAcceleration(FVector3 acceleration_);
	void AddVelocity(FVector3 velocity_);
	void AddAngularAcceleration(FVector3 angularAcceleration_);
	void AddAngularVelocity(FVector3 angularVelocity_);

	//Setters

	void SetAcceleration(FVector3 acceleration_);
	void SetVelocity(FVector3 velocity_);
	void SetAngularAcceleration(FVector3 angularAcceleration_);
	void SetAngularVelocity(FVector3 angularVelocity_);

	//Getters

	FVector3 GetAcceleration();
	FVector3 GetVelocity();
	FVector3 GetAngularAcceleration();
	FVector3 GetAngularVelocity();
};
#endif
