#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Geometry/Box.h"
#include "Objects/GameObjects/GameObject.h"
#include "CollisionHandler.h"

void C_SphereCollider::SetComponentPosition(const FVector3& position)
{
	C_TransformComponent::SetComponentPosition(position);
	CollisionSphere.position = GetComponentAbsolutePosition();
	CollisionHandler::GetInstance()->SphereSpatialCheck(this);
}

void C_SphereCollider::SetComponentScale(const FVector3& scale)
{
	C_TransformComponent::SetComponentScale(scale);
	CollisionSphere.radius = CollisionSphere.radius * GetComponentAbsoluteScale().Z;
}

void C_SphereCollider::SetComponentTransform(const FTransform& transform)
{
	C_TransformComponent::SetComponentTransform(transform);
	CollisionSphere.position = GetComponentAbsolutePosition();
}

float C_SphereCollider::GetRadius()
{
	return CollisionSphere.radius;
}

void C_SphereCollider::SetRadius(const float& radius)
{
	CollisionSphere.radius = radius;
}

Sphere C_SphereCollider::GetCollisionSphere()
{
	return CollisionSphere;
}

void C_SphereCollider::Update(const float deltaTime)
{
	C_CollisionComponent::Update(deltaTime);

	CollisionSphere.position = GetComponentPosition() + GetOwner()->GetPosition();

}

void C_SphereCollider::PostUpdate(float deltaTime)
{
	CollisionHandler::GetInstance()->SphereSpatialCheck(this);

}

//bool C_SphereCollision::Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const
//{
//	if (dynamic_cast<C_BoxCollision*>(otherCollider))
//	{
//		return SphereBoxCollision(*CollisionSphere, dynamic_cast<C_BoxCollision*>(otherCollider)->GetCollisionBox(), data);
//	}
//	if (dynamic_cast<C_SphereCollision*>(otherCollider))
//	{
//		this;
//		return SphereSphereCollision(*CollisionSphere, dynamic_cast<C_SphereCollision*>(otherCollider)->GetCollisionSphere(), data);
//	}
//	return false;
//}

C_SphereCollider::C_SphereCollider(O_GameObject* owner) : C_CollisionComponent(owner)
{
	//CollisionSphere = new Sphere(GetComponentPosition() + GetOwner()->GetPosition(), 1.0f);
	CollisionSphere = Sphere(GetComponentPosition() + GetOwner()->GetPosition(), 1.0f);
}

C_SphereCollider::~C_SphereCollider()
{
	//if (CollisionSphere) delete(CollisionSphere);
}
