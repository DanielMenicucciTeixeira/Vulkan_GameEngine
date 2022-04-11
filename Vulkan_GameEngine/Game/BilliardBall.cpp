#include "BilliardBall.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Math/FVector3.h"
#include "Geometry/Sphere.h"
#include "BilliardTable.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Geometry/Box.h"

void GO_BilliardBall::OnCollision(O_GameObject* self, const S_CollisionData& data)
{
	if (dynamic_cast<GO_BilliardBall*>(data.OtherCollisonComponent->GetOwner()))
	{
		dynamic_cast<GO_BilliardBall*>(self)->BounceOnBall(dynamic_cast<GO_BilliardBall*>(data.OtherCollisonComponent->GetOwner()), data.CollisionPoint);
	}
	else if (dynamic_cast<GO_BilliardTable*>(data.OtherCollisonComponent->GetOwner()))
	{
		if (dynamic_cast<C_BoxCollider*>(data.OtherCollisonComponent))
		{
			dynamic_cast<GO_BilliardBall*>(self)->BounceOnWall(dynamic_cast<C_BoxCollider*>(data.OtherCollisonComponent), data.CollisionPoint);
		}
	}
}

void GO_BilliardBall::BounceOnBall(GO_BilliardBall* otherBall, FVector3 pointOfImpact)
{
	FVector3 distance = (GetPosition() - otherBall->GetPosition());
	FVector3 force =
		(
			  (distance) * (2 * otherBall->Physics->GetMass() / (Physics->GetMass() + otherBall->Physics->GetMass()))
			* ((Physics->GetVelocity() - otherBall->Physics->GetVelocity()) * (distance))
			/ pow((distance).Length(), 2)
		) * -1.0f;

	//FPhysicsLib::AddForce(Physics, force, pointOfImpact);
}

void GO_BilliardBall::BounceOnWall(C_BoxCollider* wall, FVector3 pointOfImpact)
{
	FVector3 distance = (GetPosition() - pointOfImpact);
	FVector3 force =
		(
		      (distance) * 2.0f
			* (Physics->GetVelocity() * (distance))
			/ pow((distance).Length(), 2)
		) * -1.0f;

	//FPhysicsLib::AddForce(Physics, force, pointOfImpact);
}

void GO_BilliardBall::Update(float deltaTime)
{
	O_GameObject::Update(deltaTime);
	Physics->SetAngularAcceleration(Physics->GetAngularVelocity() * -0.1f);
	//FPhysicsLib::AddForce(Physics, (Physics->GetVelocity() * -Drag * deltaTime), GetPosition() + (FVector3(0.0f, 0.0f, -1.0f) * Collider->GetRadius()));
}

GO_BilliardBall::GO_BilliardBall(std::string name) : O_GameObject(name)
{
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("SphereMesh");
	Physics = AddComponentOfClass<C_PhysicsComponent>();
	Physics->AngularInertia = 0.03f;
	Collider = AddComponentOfClass<C_SphereCollider>();
	Collider->SetCollisionType(COLLISION);
	Collider->SetCollisionFunction(OnCollision);
	Collider->SetRadius(0.2f);
	SetRoot(Mesh);
	SetScale(FVector3(0.2f, 0.2f, 0.2f));
}

GO_BilliardBall::~GO_BilliardBall()
{
}
