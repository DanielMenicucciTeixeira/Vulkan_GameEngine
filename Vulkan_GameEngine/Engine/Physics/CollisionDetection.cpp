#include "CollisionDetection.h"
#include "Objects/Components/Colliders/BoundingBox.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Geometry/Ray.h"
#include "LevelGraph.h"

S_CollisionData CollisionDetection::collisionData = S_CollisionData();


CollisionDetection::~CollisionDetection()
{
}

bool CollisionDetection::RayAABBIntersection(Ray a, const S_BoxBounds b)
{
	FVector3 rayDirection = a.GetDirection().GetNormal();
	FVector3 boxMin = b.Min;
	FVector3 boxMax = b.Max;
	FMatrix4 modelMatrix = b.Model;

	//TODO: fix all commented issues in this.
	float tMin = 10;// CoreEngine::GetInstance()->GetCamera()->GetClippingPlanes().x;
	float tMax = 10; //CoreEngine::GetInstance()->GetCamera()->GetClippingPlanes().y;

	FVector3 worldPosition(modelMatrix[3].X, modelMatrix[3].Y, modelMatrix[3].Z);

	FVector3 delta = worldPosition - a.GetOrigin();

	//X Axis
	FVector3 axis(modelMatrix[0].X, modelMatrix[0].Y, modelMatrix[0].Z);
	//TODO: this is not dot product, replace glm::dot's functionalty with something else
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

	a.SetIntersectDistance(tMin);
	return true;
}
bool CollisionDetection::RaySphereIntersection(Ray a, Sphere b)
{
	FVector3 l = b.position - a.GetOrigin();
	float tca = l.GetDotProduct(a.GetDirection());
	if (tca < 0) { return false; }

	float d2 = l.GetDotProduct(l) - tca * tca;
	float radius2 = b.radius * b.radius;
	if (d2 > radius2) { return false; }

	float thc = sqrt(radius2 - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;

	if (t0 > t1) { std::swap(t0, t1); } 

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}

	a.SetIntersectDistance(t0);

	return true;
}
bool CollisionDetection::SphereAABBIntersection(Sphere a, const S_BoxBounds b)
{
	float x = Math::Clamp(b.Min.X, Math::Clamp(a.position.X, b.Max.X, false), true);
	float y = Math::Clamp(b.Min.Y, Math::Clamp(a.position.Y, b.Max.Y, false), true);
	float z = Math::Clamp(b.Min.Z, Math::Clamp(a.position.Z, b.Max.Z, false), true);

	float distance = ((x - a.position.X) * (x - a.position.X) +
					  (y - a.position.Y) * (y - a.position.Y) +
				   	  (z - a.position.Z) * (z - a.position.Z));
	
	return distance < a.radius * a.radius;
}

bool CollisionDetection::SphereIntersection(Sphere a, Sphere b)
{
	float distance = sqrtf((a.position.X - b.position.X) * (a.position.X - b.position.X) +
					  (a.position.Y - b.position.Y) * (a.position.Y - b.position.Y) +
					  (a.position.Z - b.position.Z) * (a.position.Z - b.position.Z));

	return distance < (a.radius + b.radius);
}

bool CollisionDetection::AABBIntersection(S_BoxBounds a, S_BoxBounds b)
{
	FVector3 aTransformPos = FVector3(a.Model[3].X, a.Model[3].Y, a.Model[3].Z);
	FVector3 bTransformPos = FVector3(b.Model[3].X, b.Model[3].Y, b.Model[3].Z);

	FVector3 minCorner = a.Min + aTransformPos;
	FVector3 maxCorner = a.Max + aTransformPos;

	FVector3 otherMinCorner = b.Min + bTransformPos;
	FVector3 otherMaxCorner = b.Max + bTransformPos;

	if (minCorner.X <= otherMaxCorner.X && maxCorner.X >= otherMinCorner.X &&
		minCorner.Y <= otherMaxCorner.Y && maxCorner.Y >= otherMinCorner.Y &&
		minCorner.Z <= otherMaxCorner.Z && maxCorner.Z >= otherMinCorner.Z) {
		return true;
	}

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


