#include "SphereCollision.h"
#include "Geometry/Sphere.h"
#include "BoxCollision.h"
#include "Geometry/Box.h"
#include "Objects/GameObjects/GameObject.h"

void C_SphereCollision::SetComponentPosition(const FVector3& position)
{
	C_TransformComponent::SetComponentPosition(position);
	CollisionSphere->position = GetComponentAbsolutePosition();
}

void C_SphereCollision::SetComponentScale(const FVector3& scale)
{
	C_TransformComponent::SetComponentScale(scale);
	CollisionSphere->radius = CollisionSphere->radius * GetComponentAbsoluteScale().Z;
}

void C_SphereCollision::SetComponentTransform(const FTransform& transform)
{
	C_TransformComponent::SetComponentTransform(transform);
	CollisionSphere->position = GetComponentAbsolutePosition();
}

float C_SphereCollision::GetRadius()
{
	return CollisionSphere->radius;
}

void C_SphereCollision::SetRadius(const float& radius)
{
	CollisionSphere->radius = radius;
}

Sphere C_SphereCollision::GetCollisionSphere()
{
	return *CollisionSphere;
}

void C_SphereCollision::Update(const float deltaTime)
{
	C_CollisionComponent::Update(deltaTime);

	CollisionSphere->position = GetComponentPosition() + GetOwner()->GetPosition();
}

bool C_SphereCollision::Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const
{
	if (dynamic_cast<C_BoxCollision*>(otherCollider))
	{
		return SphereBoxCollision(*CollisionSphere, dynamic_cast<C_BoxCollision*>(otherCollider)->GetCollisionBox(), data);
	}
	if (dynamic_cast<C_SphereCollision*>(otherCollider))
	{
		this;
		return SphereSphereCollision(*CollisionSphere, dynamic_cast<C_SphereCollision*>(otherCollider)->GetCollisionSphere(), data);
	}
	return false;
}

C_SphereCollision::C_SphereCollision(O_GameObject* owner) : C_CollisionComponent(owner)
{
	CollisionSphere = new Sphere(GetComponentPosition() + GetOwner()->GetPosition(), 1.0f);
}

C_SphereCollision::~C_SphereCollision()
{
	if (CollisionSphere) delete(CollisionSphere);
}
