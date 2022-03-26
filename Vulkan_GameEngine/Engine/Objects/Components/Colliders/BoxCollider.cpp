#include "BoxCollider.h"
#include "Objects/GameObjects/GameObject.h"
#include"Geometry/Sphere.h"
#include "CollisionHandler.h"

void C_BoxCollider::SetComponentPosition(const FVector3& position)
{
	C_TransformComponent::SetComponentPosition(position);
	CollisionBox.SetPosition(GetComponentAbsolutePosition());
}

void C_BoxCollider::SetComponentTransform(const FTransform& transform)
{
	C_TransformComponent::SetComponentTransform(transform);
	CollisionBox.SetPosition(GetComponentAbsolutePosition());
}

Box C_BoxCollider::GetCollisionBox()
{
	return CollisionBox;
}

//bool C_BoxCollision::Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const
//{
//	if (dynamic_cast<C_BoxCollision*>(otherCollider))
//	{
//		return false;
//	}
//	if (dynamic_cast<C_SphereCollision*>(otherCollider))
//	{
//		return SphereBoxCollision(dynamic_cast<C_SphereCollision*>(otherCollider)->GetCollisionSphere(), *CollisionBox, data);
//
//	}
//	return false;
//}

void C_BoxCollider::SetBoxExtent(FVector3 extent)
{
	CollisionBox.SetExtent(extent);
}

C_BoxCollider::C_BoxCollider(O_GameObject* owner) : C_CollisionComponent(owner)
{
	CollisionBox = Box(FVector3(1.0f, 1.0f, 1.0f), GetComponentAbsolutePosition());
}

C_BoxCollider::~C_BoxCollider()
{
}

void C_BoxCollider::PostUpdate(float deltaTime)
{
	if (!GetIsStatic() && CollisionType != ECollisionType::NO_COLLISION) {
		CollisionHandler::GetInstance()->OBBSpatialCheck(this);
	}
}
