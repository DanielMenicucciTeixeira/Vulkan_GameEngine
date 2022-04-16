#include "PhysicsComponent.h"
#include "MovementComponent.h"
#include "Objects/GameObjects/GameObject.h"
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
	C_MovementComponent::Update(deltaTime);
	//*Acceleration = FVector3();
	//*AngularAcceleration = FQuaternion();

	if (applyGravity) {
		//Apply gravitational force's here.

		velocityBuffer += FVector3(0.0f, -0.98f, 0.0f);
	}

	SlowDown(velocityBuffer);
	SlowDown(accelerationBuffer);

	FVector3 displacement = velocityBuffer * deltaTime + ((accelerationBuffer * (deltaTime * deltaTime)) / 2.0f);

	FQuaternion VelocityQuat = FQuaternion(angularVelocityBuffer.X * (M_PI / 180.0f), angularVelocityBuffer.Y * (M_PI / 180.0f), angularVelocityBuffer.Z * (M_PI / 180.0f), 0.0f);
	FQuaternion AccelerationQuat = FQuaternion(angularAccelerationBuffer.X * (M_PI / 180.0f), angularAccelerationBuffer.Y * (M_PI / 180.0f), angularAccelerationBuffer.Z * (M_PI / 180.0f), 0.0f);

	//If owner has movement as well translate them accordingly.
	Translate(displacement);

	Rotate((Owner->GetRotation() + 
		FQuaternion(angularVelocityBuffer.X * (M_PI / 180.0f), angularVelocityBuffer.Y * (M_PI / 180.0f), angularVelocityBuffer.Z * (M_PI / 180.0f), 0.0f) * Owner->GetRotation() * 0.5f * deltaTime +
		FQuaternion(angularAccelerationBuffer.X * (M_PI / 180.0f), angularAccelerationBuffer.Y * (M_PI / 180.0f), angularAccelerationBuffer.Z * (M_PI / 180.0f), 0.0f) * Owner->GetRotation() * 0.25f * deltaTime * deltaTime
		).GetNormal());





	//SingleComponent
	SetVelocity(velocityBuffer + (accelerationBuffer * deltaTime));
	acceleration = accelerationBuffer;
	angularAcceleration = angularAccelerationBuffer;
	AddAngularVelocity(angularAcceleration * deltaTime);
	angularVelocity = angularVelocityBuffer;
}

void C_PhysicsComponent::SlowDown(FVector3& vector)
{
	float x = 0.09f;
	float y = 0.5f;
	float z = 0.09f;

	//Slow down?
	if (vector.X != 0) {
		if (vector.X > 0) {
			if (x > vector.X) { x = vector.X; }
			x *= -1;
		}
		else {
			//Push up (negative veloicty)
			if (-x < vector.X) { x = vector.X; }
		}
		vector.X += x;
	}



	if (vector.Y != 0) {

		//Push down (positive velocity)
		if (vector.Y > 0) {
			if (y > vector.Y) { y = vector.Y; }
			y *= -1;
		}
		else {
			//Push up (negative veloicty)
			if (-y < vector.Y) { y = vector.Y; }
		}
		vector.Y += y;
	}



	if (vector.Z != 0) {
		if (vector.Z > 0) {
			if (z > vector.Z) { z = vector.Z; }
			z *= -1;
		}
		else {
			//Push up (negative veloicty)
			if (-z < vector.Z) { z = vector.Z; }
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

void C_PhysicsComponent::ApplyForce(FVector3 force, FVector3 location)
{
	SetVelocity(GetVelocity() + (force / GetMass()));

	FVector3 torque = (location - GetOwner()->GetPosition()).CrossProduct(force);
	if (torque == FVector3()) return;
	SetAngularVelocity(GetAngularVelocity() + torque / AngularInertia);
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

void C_PhysicsComponent::SetApplyGravity(bool applyGravity_)
{
	applyGravity = applyGravity_;
}

void C_PhysicsComponent::SetRubberness(float rubber)
{
	rubberness = rubber;
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

bool C_PhysicsComponent::GetApplyGravity()
{
	return applyGravity;
}

float C_PhysicsComponent::GetRubberness()
{
	return rubberness;
}

void C_PhysicsComponent::AABBResponse(C_BoundingBox* coll1, C_BoundingBox* coll2)
{
	//First determine which one is farther left.

	//Is the first colldier staic
	bool isFirstStatic = coll1->GetIsStatic();

		//Is the second collider static
	bool isSecondStatic = coll2->GetIsStatic();

	//If both are static then neither can move.  This should not happen becuase the static split check but just in case?
	if (isFirstStatic && isSecondStatic) { return; }

	FVector3 min1 = coll1->GetBoxBounds().GetPosition();
	FVector3 min2 = coll2->GetBoxBounds().GetPosition();

	FVector3 max1 = min1 + coll1->GetBoxBounds().GetExtent();
	FVector3 max2 = min2 + coll2->GetBoxBounds().GetExtent();

	FVector3 depthPenetration;

	//three spliting points
	//1. is coll1 to the left or right of the other.
	//2. is coll2 static or not
	//3. does coll2 have a parent or not.

	//Notes
	//if coll2 is static it does not matter if it has a parent or not.
	//By making the depthPenetration negative if coll1 is on the right, the translation can be negated allowing for the bypassing of another nesting section.
	//A collider without a object is by definition static. so 3 being false = 2 

	//Push X

	if (min1.X < min2.X) { 
		depthPenetration.X = max1.X - min2.X; 
	}
	else if (min1.X > min2.X) { 
		depthPenetration.X = max2.X - min1.X; }

	//Push Y

	if (min1.Y < min2.Y) { 
		depthPenetration.Y = max1.Y - min2.Y; }
	else if (min1.Y > min2.Y) { 
		depthPenetration.Y = -(std::abs(max2.Y - min1.Y)); };

	//Push Z

	if (min1.Z < min2.Z) { 
		depthPenetration.Z = std::abs(max1.Z - min2.Z); }
	else if (min1.Z > min2.Z) { 
		depthPenetration.Z = max2.Z - min1.Z; 
	}


	//Push by the smallest distance.

	//Solution to edge collision. (do wish I could figure out a better way but ehh for now)
	depthPenetration += FVector3(0.0000000000001f);

	depthPenetration = depthPenetration.GetSmallestVector();

	//Translation

	if (isSecondStatic) {
		Translate(-depthPenetration);
	}
	else if (isFirstStatic) {
		coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->Translate(depthPenetration);
	}
	else {
		depthPenetration = depthPenetration / 2.0f;
		Translate(-depthPenetration);

		coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->Translate(depthPenetration);
	}

	//Force reflection
	auto physicsComp = coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();

	if (depthPenetration != FVector3(0.0f)) {

		if (!isFirstStatic) {
			FVector3 force =
				(
					(depthPenetration) * 2.0f *
					(GetVelocity() * (depthPenetration))
					/ pow((depthPenetration).Length(), 2)
					) * -1.0f;

			if (force != FVector3(0.0f)) {
				SetVelocity((GetVelocity() + (force / GetMass())) * rubberness);
			}
		}


		if (!isSecondStatic) {
			FVector3 force2 =
				(
					(depthPenetration) * 2.0f *
					(physicsComp->GetVelocity() * (depthPenetration))
					/ pow((depthPenetration).Length(), 2)
					) * -1.0f;

			if (force2 != FVector3(0.0f)) {
				physicsComp->SetVelocity((physicsComp->GetVelocity() + (force2 / physicsComp->GetMass())) * physicsComp->GetRubberness());
			}
		}
	}


	coll1->RefreshBox();
	coll2->RefreshBox();

	physicsComp = nullptr;
}

void C_PhysicsComponent::AABBSphereResponse(C_BoundingBox* coll1, C_SphereCollider* coll2)
{
	//Is the first colldier staic
	bool isFirstStatic = coll1->GetIsStatic();

	//Is the second collider static
	bool isSecondStatic = coll2->GetIsStatic();

	//If both are static then neither can move.  This should not happen becuase the static split check but just in case?
	if (isFirstStatic && isSecondStatic) { return; }

	FVector3 collisionPoint = CollisionDetection::GetCollisionData().CollisionPoint;

	//Split objects
	FVector3 displacement = coll2->GetComponentAbsolutePosition() - collisionPoint;

	//Translation
	FVector3 displacementVector = displacement.GetNormal() * (coll2->GetCollisionSphere().radius - displacement.Length());


	if (collisionPoint == coll2->GetComponentAbsolutePosition() || displacementVector == FVector3(0.0f)) {
		//if the sphere centre is in the box get the closest face and calculate from there using the other equation.
		//This can also be found if the vector is 0,0,0

		FVector3 pos = coll2->GetCollisionSphere().position;

		FVector3 bMin = coll1->GetBoxBounds().GetPosition();
		FVector3 bMax = bMin + coll1->GetBoxBounds().GetExtent();

		//Clamp to nearest face point
		float x = Math::Clamp(bMin.X, bMax.X, pos.X);
		float y = Math::Clamp(bMin.Y, bMax.Y, pos.Y);
		float z = Math::Clamp(bMin.Z, bMax.Z, pos.Z);

		//Penetration = sphere center - this point
		FVector3 penetration = pos - FVector3(x, y, z);

		//should you subtract coll2 absolute here as well?

		displacementVector = penetration.GetNormal() * ((coll2->GetCollisionSphere().radius + penetration.Length()) * -1);

	}

	displacementVector = displacementVector.GetSmallestVector();

	C_PhysicsComponent* physicsComp = coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();

	//Force reflection

	if (displacementVector != FVector3(0.0f)) {

		if (!isFirstStatic) {
			FVector3 force =
				(
					(displacementVector) * 2.0f *
					(GetVelocity() * (displacementVector))
					/ pow((displacementVector).Length(), 2)
					) * -1.0f;

			if (force != FVector3(0.0f)) {
				SetVelocity((GetVelocity() + (force / GetMass())) * rubberness);
			}
		}


		if (!isSecondStatic) {
			FVector3 force2 =
				(
					(displacementVector) * 2.0f *
					(physicsComp->GetVelocity() * (displacementVector))
					/ pow((displacementVector).Length(), 2)
					) * -1.0f;

			if (force2 != FVector3(0.0f)) {
				physicsComp->SetVelocity((physicsComp->GetVelocity() + (force2 / physicsComp->GetMass())) * physicsComp->GetRubberness());
			}
		}
	}

	physicsComp->Translate(displacementVector);

	coll1->RefreshBox();
	coll2->RefreshSphere();

	physicsComp = nullptr;
	
}

void C_PhysicsComponent::AABBOBBResponse(C_BoundingBox* coll1, C_BoxCollider* coll2)
{
	//Determine the penetrative depth 
}

void C_PhysicsComponent::SphereSphereResponse(C_SphereCollider* coll1, C_SphereCollider* coll2)
{
	FVector3 vecP = coll1->GetCollisionSphere().position - coll2->GetCollisionSphere().position;

	C_PhysicsComponent* physicsComp = coll1->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();
	C_PhysicsComponent* physicsComp2 = coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();
	//TODO: Does this split?

	FVector3 displacementVector = vecP;

	//Force 1 

	FVector3 force =
		(
			(vecP) * (2 * physicsComp2->GetMass() / (physicsComp->GetMass() + physicsComp2->GetMass()))
			* ((physicsComp->GetVelocity() - physicsComp2->GetVelocity()) * (vecP))
			/ pow((vecP).Length(), 2)
			) * -1.0f;



	//Force 2

	vecP = -vecP;

	FVector3 force2 =
		(
			(vecP) * (2 * physicsComp->GetMass() / (physicsComp2->GetMass() + physicsComp->GetMass()))
			* ((physicsComp2->GetVelocity() - physicsComp->GetVelocity()) * (vecP))
			/ pow((vecP).Length(), 2)
			) * -1.0f;

	physicsComp->SetVelocity((physicsComp->GetVelocity() + (force / physicsComp->GetMass())) * physicsComp->GetRubberness());
	physicsComp2->SetVelocity((physicsComp2->GetVelocity() + (force2 / physicsComp->GetMass())) * physicsComp2->GetRubberness());


	//TODO: For some reason instead of dividing by 2 I need to multiply by about 6.5 to 6.6 to get the approximate displacement.  
	//Something is obviously wrong here.
	
	//Translate objects

	float distance = CollisionDetection::SphereIntersectionDistance(coll1->GetCollisionSphere(), coll2->GetCollisionSphere());
	float fOverlap = (distance - coll1->GetCollisionSphere().radius - coll2->GetCollisionSphere().radius);
	displacementVector = (displacementVector * fOverlap) / distance;


	if (coll2->GetIsStatic()) {
		physicsComp->Translate(-displacementVector + -(displacementVector * 0.01f));
	}
	else {
		displacementVector; //= displacementVector / 2;
		physicsComp->Translate(-displacementVector * 6.6f);
		physicsComp2->Translate(displacementVector * 6.6f);
	}
	coll1->RefreshSphere();
	coll2->RefreshSphere();

	physicsComp = nullptr;
	physicsComp2 = nullptr;
}

void C_PhysicsComponent::SphereOBBResponse(C_SphereCollider* coll1, C_BoxCollider* coll2)
{
	//Split objects
	FVector3 displacement = coll1->GetComponentAbsolutePosition() - CollisionDetection::GetCollisionData().CollisionPoint;
	//Translation
	FVector3 displacementVector = displacement.GetNormal() * (coll1->GetCollisionSphere().radius - displacement.Length());

	if (displacement == coll1->GetComponentAbsolutePosition()) {
		//if the sphere centre is in the box get the closest face and calculate from there using the other equation.
		//This can also be found if the vector is 0,0,0
	}
	
	C_PhysicsComponent* physicsComp = coll1->GetOwner()->GetComponentOfClass<C_PhysicsComponent>();

	//Reflect objects velocity
	FVector3 force =
		(
			(displacement) * 2.0f *
			(physicsComp->GetVelocity() * (displacement))
			/ pow((displacement).Length(), 2)
			) * -1.0f;
	physicsComp->SetVelocity(physicsComp->GetVelocity() + (force / physicsComp->GetMass()));

	coll2->GetOwner()->GetComponentOfClass<C_PhysicsComponent>()->SetVelocity(FVector3(0.0f));


	physicsComp->Translate(displacementVector + (displacementVector * 0.01f));

	//Test Idea.
	coll1->RefreshSphere();

	physicsComp = nullptr;

	//TODO: Reflect the ball using angle


	//Use the rotation of the nearest plane to calculate how the ball bounces?


	//Also apply force from one object to the other as well. (if the other is not static)
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

C_PhysicsComponent::C_PhysicsComponent(O_GameObject* owner, float mass, bool useCenterOfMass, float angularInertia, float rubberness_, bool useCalculatedAngularIntertia) : C_MovementComponent(owner)
{
	Mass = mass;
	rubberness = rubberness_;
	if (UseCenterOfMass) CalculateCenterOfMass();
	else CenterOfMass = Owner->GetTransformReference()->Position;

	//if (useCalculatedAngularIntertia && MeshComponent) CalculateAngularInertia();
	//else AngularInertia = angularInertia;
}

C_PhysicsComponent::~C_PhysicsComponent()
{
	if (UseCenterOfMass) delete(CenterOfMass);
}
