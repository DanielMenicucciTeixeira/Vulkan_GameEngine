#include "BoxCollider.h"
#include "Geometry/Box.h"
#include "Objects/GameObjects/GameObject.h"
#include"Geometry/Sphere.h"
#include "CollisionHandler.h"
void C_BoxCollider::SetComponentPosition(const FVector3& position)
{
	C_TransformComponent::SetComponentPosition(position);
	CollisionBox->SetPosition(GetComponentAbsolutePosition());
	CollisionHandler::GetInstance()->OBBSpatialCheck(this);
}

void C_BoxCollider::SetComponentTransform(const FTransform& transform)
{
	C_TransformComponent::SetComponentTransform(transform);
	CollisionBox->SetPosition(GetComponentAbsolutePosition());
}

Box C_BoxCollider::GetCollisionBox()
{
	return *CollisionBox;
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
	CollisionBox->SetExtent(extent);
}

C_BoxCollider::C_BoxCollider(O_GameObject* owner) : C_CollisionComponent(owner)
{
	CollisionBox = new Box(FVector3(1.0f, 1.0f, 1.0f), GetComponentAbsolutePosition());
}

C_BoxCollider::~C_BoxCollider()
{
	if (CollisionBox) delete(CollisionBox);
}