#include "CollisionDetection.h"
#include "BoundingBox.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

CollisionDetection::~CollisionDetection()
{
}

bool CollisionDetection::Collision(C_BoundingBox a, C_BoundingBox b)
{
	FVector3 minCorner = a.GetMin() + a.GetComponentAbsolutePosition();
	FVector3 maxCorner = a.GetMax() + a.GetComponentAbsolutePosition();

	FVector3 otherMinCorner = b.GetMin() + b.GetComponentAbsolutePosition();
	FVector3 otherMaxCorner = b.GetMax() + b.GetComponentAbsolutePosition();

	if (minCorner.X <= otherMaxCorner.X && maxCorner.X >= otherMinCorner.X &&
		minCorner.Y <= otherMaxCorner.Y && maxCorner.Y >= otherMinCorner.Y &&
		minCorner.Z <= otherMaxCorner.Z && maxCorner.Z >= otherMinCorner.Z) {
		return true;
	}

	return false;
}

bool CollisionDetection::Collision(C_BoxCollider a, C_BoxCollider b)
{

	return false;
}

FVector3 * CollisionDetection::Collision(C_SphereCollider a, C_SphereCollider b)
{
	float distance = (a.GetCollisionSphere().position - b.GetCollisionSphere().position).Length();
	if (distance - (a.GetRadius() + b.GetRadius()) <= 0.01f)
	{
		return FVector3((a.GetCollisionSphere().position - b.GetCollisionSphere()).GetNormal() * b.GetRadius()) + b.GetCollisionSphere().position;
	}
	else return NULL;
}


