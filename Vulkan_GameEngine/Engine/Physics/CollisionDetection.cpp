#include "CollisionDetection.h"
#include "Objects/Components/Colliders/BoundingBox.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Geometry/Ray.h"
#include "LevelGraph.h"
#include "Geometry/BoxBounds.h"

S_CollisionData CollisionDetection::collisionData = S_CollisionData();


CollisionDetection::~CollisionDetection()
{
}

bool CollisionDetection::Collision(Ray& a, const S_BoxBounds b)
{
	FVector3 rayOrigin = a.GetOrigin();
	FVector3 rayDirection = a.GetDirection().GetNormal();
	FVector3 boxMin = b.Min;
	FVector3 boxMax = b.Max;
	FMatrix4 modelMatrix = b.Model;

	float tMin = 10; //= LevelGraph::GetInstance()->GetActiveCamera()->GetNearPlane();
	float tMax = 10; //= LevelGraph::GetInstance()->GetActiveCamera()->GetFarPlane();

	FVector3 worldPosition(modelMatrix[3].X, modelMatrix[3].Y, modelMatrix[3].Z);
	FVector3 delta = worldPosition - rayOrigin;

	//X Axis
	FVector3 axis(modelMatrix[0].X, modelMatrix[0].Y, modelMatrix[0].Z);
	float deltaDot = delta * axis;
	float directionDot = rayDirection * axis;
	if (fabs(directionDot) > 0.0001f)
	{
		float t1 = (deltaDot + boxMin.X) / directionDot;
		float t2 = (deltaDot + boxMax.X) / directionDot;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;
		if (tMax < tMin) return false;
	}
	else if (-deltaDot + boxMin.X > 0.0f || -deltaDot + boxMax.X < 0.0f) return false;

	//Y Axis
	axis = FVector3(modelMatrix[1].X, modelMatrix[1].Y, modelMatrix[1].Z);
	deltaDot = delta * axis;
	directionDot = rayDirection * axis;
	if (fabs(directionDot) > 0.0001f)
	{
		float t1 = (deltaDot + boxMin.Y) / directionDot;
		float t2 = (deltaDot + boxMax.Y) / directionDot;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;
		if (tMax < tMin) return false;
	}
	else if (-deltaDot + boxMin.Y > 0.0f || -deltaDot + boxMax.Y < 0.0f) return false;

	//Z Axis
	axis = FVector3(modelMatrix[2].X, modelMatrix[2].Y, modelMatrix[2].Z);
	deltaDot = delta * axis;
	directionDot = rayDirection * axis;
	if (fabs(directionDot) > 0.0001f)
	{
		float t1 = (deltaDot + boxMin.Z) / directionDot;
		float t2 = (deltaDot + boxMax.Z) / directionDot;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;
		if (tMax < tMin) return false;
	}
	else if (-deltaDot + boxMin.Z > 0.0f || -deltaDot + boxMax.Z < 0.0f) return false;

	a.SetLenght(tMin);
	collisionData.CollisionPoint = a.GetOrigin() + (a.GetDirection() * tMin);
	return true;
	return false;
}
/*
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
		return new FVector3(((a.GetCollisionSphere().position - b.GetCollisionSphere().position).GetNormal() * b.GetRadius()) + b.GetCollisionSphere().position);
	}
	else return nullptr;
}
*/
S_CollisionData CollisionDetection::GetCollisionData()
{
	return collisionData;
}


