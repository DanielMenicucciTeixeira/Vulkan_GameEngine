#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "MovementComponent.h"
#include "Math/FVector3.h"
class O_GameObject;

class FMatrix;
class C_StaticMeshComponent;
class C_BoundingBox;
class C_SphereCollider;
class C_BoxCollider;


class C_PhysicsComponent : public C_MovementComponent
{

private:
	FVector3 acceleration;
	FVector3 angularAcceleration;
	FVector3 angularVelocity;

	FVector3 accelerationBuffer;
	FVector3 angularAccelerationBuffer;
	FVector3 angularVelocityBuffer;

	float Mass;

	bool applyGravity;

protected:

	//requires a C_StaticMeshComponent set as MeshComponent
	void CalculateAngularInertia();

	//requires a C_StaticMeshComponent set as MeshComponent
	void CalculateCenterOfMass();

	FVector3* CenterOfMass;
	bool UseCenterOfMass;

	//used to determine how much the object bounces. 1 = high, 0 = low.
	float rubberness;

public:
	float AngularInertia;
	FVector3 GetCenterOfMass();
	C_PhysicsComponent(O_GameObject* owner, float mass = 1.0f, bool useCenterOfMass = false, float angularInertia = 1.0f, float rubberness = 1.0f, bool useCalculatedAngularIntertia = false);
	virtual ~C_PhysicsComponent();

	void Update(const float deltaTime) override;

	//AKA stupid stop gap for friction.
	void SlowDown(FVector3& vector);


	//Add Functions

	void AddAcceleration(FVector3 acceleration_);
	void AddVelocity(FVector3 velocity_);
	void AddAngularAcceleration(FVector3 angularAcceleration_);
	void AddAngularVelocity(FVector3 angularVelocity_);
	void ApplyForce(FVector3 force, FVector3 location);

	//Setters

	void SetAcceleration(FVector3 acceleration_);
	void SetVelocity(FVector3 velocity_);
	void SetAngularAcceleration(FVector3 angularAcceleration_);
	void SetAngularVelocity(FVector3 angularVelocity_);
	void SetMass(float mass);
	void SetApplyGravity(bool applyGravity_);
	void SetRubberness(float rubber);

	//Getters

	FVector3 GetAcceleration();
	FVector3 GetVelocity();
	FVector3 GetAngularAcceleration();
	FVector3 GetAngularVelocity();
	float GetMass();
	bool GetApplyGravity();
	float GetRubberness();


	//Physics Response. If the other object does not have physics values ausumed to be 0.

	//AABB v AABB
	void AABBResponse(C_BoundingBox* coll1, C_BoundingBox* coll2);
	
	//AABB v Sphere
	void AABBSphereResponse(C_BoundingBox* coll1, C_SphereCollider* coll2);


	//AABB v OBB
	void AABBOBBResponse(C_BoundingBox* coll1, C_BoxCollider* coll2);

	
	//Sphere v Sphere
	void SphereSphereResponse(C_SphereCollider* coll1, C_SphereCollider* coll2);

	//Sphere v OBB
	void SphereOBBResponse(C_SphereCollider* coll1, C_BoxCollider* coll2);


	//OBB v OBB
	void OBBResponse(C_BoxCollider* coll1, C_BoxCollider* coll2);
};
#endif
