#include "BilliardBall.h"
#include "PhysicsComponent.h"
#include "SphereCollision.h"
#include "StaticMeshComponent.h"
#include "Math/FVector3.h"
#include "Geometry/Sphere.h"
#include "PhysicsLib.h"
#include "BilliardTable.h"
#include "BoxCollision.h"
#include "Geometry/Box.h"

void GO_BilliardBall::OnCollision(O_GameObject* self, const S_CollisionData& data)
{
	if (dynamic_cast<GO_BilliardBall*>(data.OtherGameObject))
	{
		dynamic_cast<GO_BilliardBall*>(self)->BounceOnBall(dynamic_cast<GO_BilliardBall*>(data.OtherGameObject), data.CollisionPoint);
	}
	else if (dynamic_cast<GO_BilliardTable*>(data.OtherGameObject))
	{
		if (dynamic_cast<C_BoxCollision*>(data.OtherCollisonComponent))
		{
			dynamic_cast<GO_BilliardBall*>(self)->BounceOnWall(dynamic_cast<C_BoxCollision*>(data.OtherCollisonComponent), data.CollisionPoint);
		}
	}
}

void GO_BilliardBall::BounceOnBall(GO_BilliardBall* otherBall, FVector3 pointOfImpact)
{
	FVector3 distance = (GetPosition() - otherBall->GetPosition());
	FVector3 force =
		(
			  (distance) * (2 * otherBall->Physics->Mass / (Physics->Mass + otherBall->Physics->Mass))
			* ((Physics->GetVelocity() - otherBall->Physics->GetVelocity()) * (distance))
			/ pow((distance).Length(), 2)
		) * -1.0f;

	FPhysicsLib::AddForce(Physics, force, pointOfImpact);
}

void GO_BilliardBall::BounceOnWall(C_BoxCollision* wall, FVector3 pointOfImpact)
{
	FVector3 distance = (GetPosition() - pointOfImpact);
	FVector3 force =
		(
		      (distance) * 2.0f
			* (Physics->GetVelocity() * (distance))
			/ pow((distance).Length(), 2)
		) * -1.0f;

	FPhysicsLib::AddForce(Physics, force, pointOfImpact);
}

GO_BilliardBall::GO_BilliardBall() : O_GameObject()
{
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Physics = AddComponentOfClass<C_PhysicsComponent>();
	Collider = AddComponentOfClass<C_SphereCollision>();
	Collider->SetCollisionType(COLLISION);
	Collider->SetCollisionFunction(OnCollision);
	Collider->SetRadius(0.2f);
}

GO_BilliardBall::~GO_BilliardBall()
{
}
