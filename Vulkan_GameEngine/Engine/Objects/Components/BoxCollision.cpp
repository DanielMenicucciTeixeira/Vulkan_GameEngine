#include "BoxCollision.h"
#include "Geometry/Box.h"
#include "Objects/GameObjects/GameObject.h"
#include "SphereCollision.h"
#include"Geometry/Sphere.h"

void C_BoxCollision::SetComponentPosition(const FVector3& position)
{
	C_TransformComponent::SetComponentPosition(position);
	CollisionBox->SetPosition(GetComponentAbsolutePosition());
}

void C_BoxCollision::SetComponentTransform(const FTransform& transform)
{
	C_TransformComponent::SetComponentTransform(transform);
	CollisionBox->SetPosition(GetComponentAbsolutePosition());
}

Box C_BoxCollision::GetCollisionBox()
{
	return *CollisionBox;
}

bool C_BoxCollision::Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const
{
	if (dynamic_cast<C_BoxCollision*>(otherCollider))
	{
		return false;
	}
	if (dynamic_cast<C_SphereCollision*>(otherCollider))
	{
		return SphereBoxCollision(dynamic_cast<C_SphereCollision*>(otherCollider)->GetCollisionSphere(), *CollisionBox, data);

	}
	return false;
}

void C_BoxCollision::SetBoxExtent(FVector3 extent)
{
	CollisionBox->SetExtent(extent);
}

C_BoxCollision::C_BoxCollision(O_GameObject* owner) : C_CollisionComponent(owner)
{
	CollisionBox = new Box(FVector3(1.0f, 1.0f, 1.0f), GetComponentAbsolutePosition());
}

C_BoxCollision::~C_BoxCollision()
{
	if (CollisionBox) delete(CollisionBox);
}