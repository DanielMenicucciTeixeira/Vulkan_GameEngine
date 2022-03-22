#include "PhysicsComponent.h"
#include "MovementComponent.h"
#include "Objects/GameObjects/GameObject.h"
#include "Physics/PhysicsLib.h"
#include "Math/FQuaternion.h"
#include "Math/FTransform.h"
#include "Objects/Components/TransformComponent.h"
#include "Objects/Components/Colliders/BoundingBox.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Physics/CollisionDetection.h"
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
	Translate(velocityBuffer * deltaTime + ((accelerationBuffer * (deltaTime * deltaTime)) / 2.0f));

	Rotate((
		FQuaternion(angularVelocityBuffer.X * (M_PI / 180.0f), angularVelocityBuffer.Y * (M_PI / 180.0f), angularVelocityBuffer.Z * (M_PI / 180.0f), 0.0f) * Owner->GetRotation() * 0.5f * deltaTime +
		FQuaternion(angularAccelerationBuffer.X * (M_PI / 180.0f), angularAccelerationBuffer.Y * (M_PI / 180.0f), angularAccelerationBuffer.Z * (M_PI / 180.0f), 0.0f) * Owner->GetRotation() * 0.25f * deltaTime * deltaTime
		).GetNormal());

	SlowDown(velocityBuffer);
	SlowDown(accelerationBuffer);

	//SingleComponent
	SetVelocity(velocityBuffer + (accelerationBuffer * deltaTime));
	acceleration = accelerationBuffer;
	angularAcceleration = angularAccelerationBuffer;
	AddAngularVelocity(angularAcceleration * deltaTime);
	angularVelocity = angularVelocityBuffer;
}

void C_PhysicsComponent::SlowDown(FVector3 vector)
{
	float x = 0.1;

	//Slow down?
	if (vector.X != 0) {
		if (vector.X < 0) {
			x *= -1;
		}
		vector.X += x;
	}

	float y = 0.1;

	if (vector.Y != 0) {
		if (vector.Y < 0) {
			y *= -1;
		}
		vector.Y += y;
	}

	float z = 0.1;

	if (vector.Z != 0) {
		if (vector.Z < 0) {
			z *= -1;
		}
		vector.Z += z;
	}
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
	accelerationBuffer = acceleration_;
}

void C_PhysicsComponent::SetVelocity(FVector3 velocity_)
{
	velocity = velocity_;
	velocityBuffer = velocity_;
}

void C_PhysicsComponent::SetAngularAcceleration(FVector3 angularAcceleration_)
{
	angularAcceleration = angularAcceleration_;
	angularAccelerationBuffer = angularAcceleration_;
}

void C_PhysicsComponent::SetAngularVelocity(FVector3 angularVelocity_)
{
	angularVelocity = angularVelocity_;
	angularVelocityBuffer = angularVelocity_;
}

void C_PhysicsComponent::SetMass(float mass)
{
	Mass = mass;
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

float C_PhysicsComponent::GetMass()
{
	return Mass;
}

void C_PhysicsComponent::AABBResponse(C_BoundingBox* coll1, C_BoundingBox* coll2)
{
	//First determine which one is farther left.

	S_BoxBounds min1 = coll1->GetBoxBounds();
	S_BoxBounds min2 = coll2->GetBoxBounds();

	FVector3 depthPenetration;

	//Is the second collider static
	bool isSecondStatic = coll2->GetIsStatic();

	//three spliting points
	//1. is coll1 to the left or right of the other.
	//2. is coll2 static or not
	//3. does coll2 have a parent or not.

	//Notes
	//if coll2 is static it does not matter if it has a parent or not.
	//By making the depthPenetration negative if coll1 is on the right, the translation can be negated allowing for the bypassing of another nesting.
	//A collider without a object is by definition static. so 3 being false = 2 

	//Push X

	if (min1.Min.X <= min2.Min.X) { depthPenetration.X = min1.Max.X - min2.Min.X; }
	else { depthPenetration.X = -(min2.Max.X - min1.Min.X); }

	//Push Y

	if (min1.Min.Y <= min2.Min.Y) { depthPenetration.Y = min1.Max.Y - min2.Min.Y; }
	else { depthPenetration.Y = -(min2.Max.Y - min1.Min.Y); }

	//Push Z

	if (min1.Min.Z <= min2.Min.Z) { depthPenetration.Z = min1.Max.Z - min2.Min.Z; }
	else { depthPenetration.Z = -(min2.Max.Z - min1.Min.Z); }

	if (isSecondStatic) {
		Translate(-depthPenetration);
	}
	else {
		depthPenetration = depthPenetration / 2.0f;
		Translate(-depthPenetration);

		C_PhysicsComponent* physicsComp = coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();
		physicsComp->Translate(depthPenetration);
		physicsComp = nullptr;
	}
}

void C_PhysicsComponent::AABBSphereResponse(C_BoundingBox* coll1, C_SphereCollider* coll2)
{
	//Split objects
	FVector3 displacement = CollisionDetection::GetCollisionData().CollisionPoint;

	//Translation
	FVector3 displacementVector = (displacement.GetNormal() * (coll2->GetCollisionSphere().radius + displacement.Length())) * -1;


	C_PhysicsComponent* physicsComp = coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();
	physicsComp->Translate(displacementVector);
	physicsComp = nullptr;

	//TODO: Reflect the ball using angle
}

void C_PhysicsComponent::AABBOBBResponse(C_BoundingBox* coll1, C_BoxCollider* coll2)
{
	//Determine the penetrative depth 
}

void C_PhysicsComponent::SphereSphereResponse(C_SphereCollider* coll1, C_SphereCollider* coll2)
{

	FVector3 vecP = coll1->GetCollisionSphere().position - coll2->GetCollisionSphere().position;

	C_PhysicsComponent* physicsComp = coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();
	//TODO: Does this split?

	if (vecP < 0) { vecP = -vecP; }

	vecP.Normalize();

	//Then we calculate the x - direction velocity vector and the perpendicular y - vector.
	
	float x1 = vecP.GetDotProduct(velocity);
	FVector3 v1x = vecP * x1;
	FVector3 v1y = velocity - v1x;

	vecP = vecP * -1;

	FVector3 v2;
	float mass2 = 0;

	if (physicsComp != nullptr) {
		v2 = physicsComp->GetVelocity();
		mass2 = physicsComp->GetMass();
	}
	
	FVector3 x2 = vecP.GetDotProduct(v2);
	FVector3 v2x = vecP * x2;
	FVector3 v2y = v2 - v2x;

	SetVelocity(v1x * (Mass - mass2) / (Mass - mass2) + v2x * (2 * mass2) / (Mass + mass2) + v1y);
	if (physicsComp != nullptr) {
		physicsComp->SetVelocity(v1x * (2 * Mass) / (Mass + mass2) + v2x * (mass2 - Mass) / (Mass + mass2) + v2y);
	}

	physicsComp = nullptr;
}

void C_PhysicsComponent::SphereOBBResponse(C_SphereCollider* coll1, C_BoxCollider* coll2)
{
	//Split objects
	FVector3 displacement = CollisionDetection::GetCollisionData().CollisionPoint;

	//Translation
	FVector3 displacementVector = (displacement.GetNormal() * (coll1->GetCollisionSphere().radius + displacement.Length())) * -1;


	C_PhysicsComponent* physicsComp = coll1->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();
	physicsComp->Translate(displacementVector);
	physicsComp = nullptr;

	//TODO: Reflect the ball using angle
}

void C_PhysicsComponent::OBBResponse(C_BoxCollider* coll1, C_BoxCollider* coll2)
{
	
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

C_PhysicsComponent::C_PhysicsComponent(O_GameObject* owner, float mass, bool useCenterOfMass, float angularInertia, bool useCalculatedAngularIntertia) : C_MovementComponent(owner)
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
