#include "CollisionDetection.h"
#include "Geometry/BoxBounds.h"
#include "Geometry/Box.h"
#include "Geometry/Sphere.h"
#include "Geometry/Ray.h"
#include "Objects/Components/CameraComponent.h"
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

	float tMin = LevelGraph::GetInstance()->GetActiveCamera()->GetNearPlane();
	float tMax = LevelGraph::GetInstance()->GetActiveCamera()->GetFarPlane();

	FVector3 worldPosition(modelMatrix[3].X, modelMatrix[3].Y, modelMatrix[3].Z);

	FVector3 delta = worldPosition - a.GetOrigin();

	//X Axis
	FVector3 axis(modelMatrix[0].X, modelMatrix[0].Y, modelMatrix[0].Z);

	float deltaDot = delta.GetDotProduct(axis);
	float directionDot = rayDirection.GetDotProduct(axis);

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
	deltaDot = delta.GetDotProduct(axis);
	directionDot = rayDirection.GetDotProduct(axis);

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
	deltaDot = delta.GetDotProduct(axis);
	directionDot = rayDirection.GetDotProduct(axis);

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
	collisionData.CollisionPoint = a.GetOrigin() + tMin;

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
	collisionData.CollisionPoint = a.GetOrigin() + t0;

	return true;
}
bool CollisionDetection::RayOBBIntersection(Ray a, Box b)
{
	FVector3 rayDirection = a.GetDirection().GetNormal();
	FVector3 boxMin = b.Min;
	FVector3 boxMax = b.Max;
	FMatrix4 modelMatrix = b.model;

	float tMin = LevelGraph::GetInstance()->GetActiveCamera()->GetNearPlane();
	float tMax = LevelGraph::GetInstance()->GetActiveCamera()->GetFarPlane();

	FVector3 worldPosition(modelMatrix[3].X, modelMatrix[3].Y, modelMatrix[3].Z);

	FVector3 delta = worldPosition - a.GetOrigin();

	//X Axis
	FVector3 axis(modelMatrix[0].X, modelMatrix[0].Y, modelMatrix[0].Z);

	float deltaDot = delta.GetDotProduct(axis);
	float directionDot = rayDirection.GetDotProduct(axis);

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
	deltaDot = delta.GetDotProduct(axis);
	directionDot = rayDirection.GetDotProduct(axis);

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
	deltaDot = delta.GetDotProduct(axis);
	directionDot = rayDirection.GetDotProduct(axis);

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
	collisionData.CollisionPoint = a.GetOrigin() + tMin;

	return true;
}
bool CollisionDetection::SphereAABBIntersection(Sphere a, const S_BoxBounds b)
{
	FVector3 bTransform = b.Model[3];

	FVector3 bMin = b.Min + bTransform;
	FVector3 bMax = b.Max + bTransform;

	float x = Math::Clamp(bMin.X, Math::Clamp(a.position.X, bMax.X, false), true);
	float y = Math::Clamp(bMin.Y, Math::Clamp(a.position.Y, bMax.Y, false), true);
	float z = Math::Clamp(bMin.Z, Math::Clamp(a.position.Z, bMax.Z, false), true);

	float distance = ((x - a.position.X) * (x - a.position.X) +
					  (y - a.position.Y) * (y - a.position.Y) +
				   	  (z - a.position.Z) * (z - a.position.Z));

	collisionData.CollisionPoint = FVector3(x, y, z);

	return distance < a.radius * a.radius;
}

bool CollisionDetection::SphereOBBIntersection(Sphere a, Box b)
{
	FVector3 bTransform = b.model[3];

	FVector3 bMin = b.Min + bTransform;
	FVector3 bMax = b.Max + bTransform;

	float x = Math::Clamp(bMin.X, Math::Clamp(a.position.X, bMax.X, false), true);
	float y = Math::Clamp(bMin.Y, Math::Clamp(a.position.Y, bMax.Y, false), true);
	float z = Math::Clamp(bMin.Z, Math::Clamp(a.position.Z, bMax.Z, false), true);

	float distance = ((x - a.position.X) * (x - a.position.X) +
		(y - a.position.Y) * (y - a.position.Y) +
		(z - a.position.Z) * (z - a.position.Z));

	collisionData.CollisionPoint = FVector3(x, y, z);

	return distance < a.radius* a.radius;
}

bool CollisionDetection::SphereIntersection(Sphere a, Sphere b)
{
	//TODO://Add in point of collision
	float distance = sqrtf((a.position.X - b.position.X) * (a.position.X - b.position.X) +
					  (a.position.Y - b.position.Y) * (a.position.Y - b.position.Y) +
					  (a.position.Z - b.position.Z) * (a.position.Z - b.position.Z));

	return distance < (a.radius + b.radius);
}

float CollisionDetection::SphereIntersectionDistance(Sphere a, Sphere b)
{
	float distance = sqrtf((a.position.X - b.position.X) * (a.position.X - b.position.X) +
		(a.position.Y - b.position.Y) * (a.position.Y - b.position.Y) +
		(a.position.Z - b.position.Z) * (a.position.Z - b.position.Z));

	return distance;
}

bool CollisionDetection::AABBIntersection(S_BoxBounds a, S_BoxBounds b)
{
	//TODO://Add in point of collision
	FVector3 aTransformPos = a.Model[3];
	FVector3 bTransformPos = b.Model[3];

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

bool CollisionDetection::AABBOBBIntersection(S_BoxBounds a, Box b)
{
	FVector3 RelativePosition;
	RelativePosition = FVector3(a.Model[3]) - FVector3(b.model[3]);

	FVector3 box1Axis[3] = { FVector3(1.0f), FVector3(1.0f), FVector3(1.0f) };
	FVector3 box2Axis[3] = { FVector3(b.model[0]), FVector3(b.model[1]), FVector3(b.model[2]) };

	FVector3 box1HalfSize = (a.Max - a.Min) / 2.0f;
	FVector3 box2HalfSize = (b.Max - b.Min) / 2.0f;

	return
		(
			!IsSeparatingPlane(RelativePosition, box1Axis[0], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[0], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[1], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[2], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[0]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[1]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[2]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[0]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[1]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[2]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[0]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[1]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[2]), box1HalfSize, box1Axis, box2HalfSize, box2Axis)
			);
}

bool CollisionDetection::OBBIntersection(Box a, Box b)
{
	FVector3 RelativePosition;
	RelativePosition = FVector3(a.model[3]) - FVector3(b.model[3]);

	FVector3 box1Axis[3] = { FVector3(a.model[0]), FVector3(a.model[1]), FVector3(a.model[2]) };
	FVector3 box2Axis[3] = { FVector3(b.model[0]), FVector3(b.model[1]), FVector3(b.model[2]) };

	FVector3 box1HalfSize = (a.Max - a.Min) / 2.0f;
	FVector3 box2HalfSize = (b.Max - b.Min) / 2.0f;

	return
		(
			!IsSeparatingPlane(RelativePosition, box1Axis[0], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[0], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[1], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[2], box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[0]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[1]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[2]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[0]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[1]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[2]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[0]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[1]), box1HalfSize, box1Axis, box2HalfSize, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[2]), box1HalfSize, box1Axis, box2HalfSize, box2Axis)
			);
}

S_CollisionData CollisionDetection::GetCollisionData()
{
	return collisionData;
}

bool CollisionDetection::IsSeparatingPlane(const FVector3 RelativePosition, const FVector3 plane, FVector3 box1HalfSize, FVector3 box1Axis[3], FVector3 box2HalfSize, FVector3 box2Axis[3])
{
	return (
		fabs(RelativePosition * plane) >
		(
			fabs((box1Axis[0] * box1HalfSize.X) * plane) +
			fabs((box1Axis[1] * box1HalfSize.Y) * plane) +
			fabs((box1Axis[2] * box1HalfSize.Z) * plane) +
			fabs((box2Axis[0] * box2HalfSize.X) * plane) +
			fabs((box2Axis[1] * box2HalfSize.Y) * plane) +
			fabs((box2Axis[2] * box2HalfSize.Z) * plane)
			)
		);
}


