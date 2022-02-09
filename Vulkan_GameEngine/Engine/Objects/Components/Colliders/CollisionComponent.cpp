#include "CollisionComponent.h"
#include "Geometry/Ray.h"
#include "Geometry/Sphere.h"
#include "Geometry/Box.h"
#include "Geometry/Plane.h"
#include "Geometry/BoxBounds.h"
#include "Math/FVector3.h"
#include <cmath>
#include <iostream>
#include "BoxCollider.h"
#include "BoundingBox.h"
#include "Objects\GameObjects\GameObject.h"
#include "Level.h"
#include "LevelGraph.h"
#include "../CameraComponent.h"

bool C_CollisionComponent::IsCollidingWith(C_CollisionComponent* collider)
{
	if (OverlapedColliders.count(collider)) return true;
	return false;
}

bool C_CollisionComponent::RaySphereCollision(const Ray& ray, const Sphere& sphere, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision)
{
	Ray line = ray;

	float a, b, c;
	a = 1.0f;
	b = (line.GetDirection() * (line.GetOrigin() - sphere.position) * 2.0f);
	c = ((line.GetOrigin() - sphere.position) * (line.GetOrigin() - sphere.position)) - (sphere.radius * sphere.radius);

	float delta = (b * b) - 4 * a * c;

	if (delta < 0)
	{
		return false;
	}
	else if (delta == 0)
	{
		collisionPoints[0] = line.GetPositionAtLenght(-b / (2.0f * a));
		return true;
	}
	else if (!stopAtFirstCollision)
	{
		float q = (b > 0) ? -0.5 * (b + sqrt(delta)) : -0.5 * (b - sqrt(delta));
		collisionPoints[0] = line.GetPositionAtLenght(q / a);
		collisionPoints[1] = line.GetPositionAtLenght(c / q);

		return true;
	}
	else
	{
		float q = (b > 0) ? -0.5 * (b + sqrt(delta)) : -0.5 * (b - sqrt(delta));
		FVector3 point0 = line.GetPositionAtLenght(q / a);
		FVector3 point1 = line.GetPositionAtLenght(c / q);

		if ((line.GetOrigin() - point0).Length() <= (line.GetOrigin() - point1).Length()) collisionPoints[0] = point0;
		else collisionPoints[0] = point1;

		return true;
	}
}

bool C_CollisionComponent::RayBoxCollision(const Ray& ray, const Box& box, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision)
{
	Ray line = ray;
	int i = 0;
	FVector3 interssection[6];
	for (auto plane : box.box)
	{
		if
			(
				plane.InterssectionPoint(line, interssection[i]) &&
				interssection[i].X <= box.GetExtent().X && interssection[i].X >= 0 &&
				interssection[i].Y <= box.GetExtent().Y && interssection[i].Y >= 0 &&
				interssection[i].Z <= box.GetExtent().Z && interssection[i].Z >= 0
				)
		{
			i++;
			if (i >= 2) break;
		}
	}


	if (i == 0) return false;
	else
	{
		if (stopAtFirstCollision)
		{
			FVector3 closestPoint = interssection[0];
			for (int j = 1; j <= i; j++)
			{
				if ((line.GetOrigin() - interssection[j]).Length() < (line.GetOrigin() - closestPoint).Length())
				{
					closestPoint = interssection[j];
				}
			}
			collisionPoints[0] = closestPoint;
		}
		else
		{
			for (int j = 0; j <= i; j++)
			{
				collisionPoints[j] = interssection[j];
			}
		}

		data.CollisionPoint = collisionPoints[0];
		return true;
	}

}

bool C_CollisionComponent::RayBoundingBoxCollision(Ray& ray, S_BoxBounds box,  S_CollisionData& data)
{
	FVector3 rayOrigin = ray.GetOrigin();
	FVector3 rayDirection = ray.GetDirection().GetNormal();
	FVector3 boxMin = box.Min;
	FVector3 boxMax = box.Max;
	FMatrix4 modelMatrix = box.Model;

	float tMin = LevelGraph::GetInstance()->GetActiveCamera()->GetNearPlane();
	float tMax = LevelGraph::GetInstance()->GetActiveCamera()->GetFarPlane();

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

	ray.SetLength(tMin);
	data.CollisionPoint = ray.GetOrigin() + (ray.GetDirection() * tMin);
	return true;
}

/*bool C_CollisionComponent::SphereSphereCollision(const Sphere& sphere0, const Sphere& sphere1, S_CollisionData& data, float tolerance)
{
	float distance = (sphere0.position - sphere1.position).Length();
	if (distance - (sphere0.radius + sphere1.radius) <= tolerance)
	{
		data.CollisionPoint = ((sphere0.position - sphere1.position).GetNormal() * sphere1.radius) + sphere1.position;
		return true;
	}
	else return false;
}*/

bool C_CollisionComponent::SpherePlaneCollision(const Sphere& sphere, const FVector3& direction, const Plane& plane, S_CollisionData& data)
{

	FVector3 collisionDirection = ((plane.GetPlaneNormal() * -1) + sphere.position).GetNormal();

	FVector3 collisionPoint;
	bool result = plane.InterssectionPoint(Ray(sphere.position, collisionDirection, sphere.radius, false), collisionPoint);
	data.CollisionPoint = collisionPoint;
	return result;
}

bool C_CollisionComponent::SphereBoxCollision(const Sphere& sphere, const Box& box, S_CollisionData& data)
{
	if
		(
			   (sphere.position.X - sphere.radius <= box.GetPosition().X + box.GetExtent().X && sphere.position.X + sphere.radius >= box.GetPosition().X)
			&& (sphere.position.Y - sphere.radius <= box.GetPosition().Y + box.GetExtent().Y && sphere.position.Y + sphere.radius >= box.GetPosition().Y)
			&& (sphere.position.Z - sphere.radius <= box.GetPosition().Z + box.GetExtent().Z && sphere.position.Z + sphere.radius >= box.GetPosition().Z)
		)
	{
		for (const auto& plane : box.box)
		{
			float distance = (sphere.position - plane.GetRandomPointInPlane()) * plane.GetPlaneNormal();
			if (distance < sphere.radius)
			{
				data.CollisionPoint = (plane.GetPlaneNormal() * sphere.radius) + sphere.position;
				return true;
			}
		}
	}
	return false;
}

bool C_CollisionComponent::BoundingBoxBoundingBoxCollision(const S_BoxBounds& box1, const S_BoxBounds& box2, S_CollisionData& data)
{
	FVector3 RelativePosition;
	RelativePosition = FVector3(box2.Model[3]) - FVector3(box1.Model[3]);
	
	FVector3 box1Axis[3] = { FVector3(box1.Model[0]), FVector3(box1.Model[1]), FVector3(box1.Model[2]) };
	FVector3 box2Axis[3] = { FVector3(box2.Model[0]), FVector3(box2.Model[1]), FVector3(box2.Model[2]) };

	return 
		(
			!IsSeparatingPlane(RelativePosition, box1Axis[0], box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1], box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2], box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[0], box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[1], box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box2Axis[2], box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[0]), box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[1]), box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[0].CrossProduct(box2Axis[2]), box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[0]), box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[1]), box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[1].CrossProduct(box2Axis[2]), box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[0]), box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[1]), box1, box1Axis, box2, box2Axis) &&
			!IsSeparatingPlane(RelativePosition, box1Axis[2].CrossProduct(box2Axis[2]), box1, box1Axis, box2, box2Axis)
		);
}

bool C_CollisionComponent::IsSeparatingPlane(const FVector3& RelativePosition, const FVector3& Plane, const S_BoxBounds& box1, const FVector3 box1Axis[3], const S_BoxBounds& box2, const FVector3 box2Axis[3])
{
	FVector3 box1HalfSize = (box1.Max - box1.Min) / 2.0f;
	FVector3 box2HalfSize = (box2.Max - box2.Min) / 2.0f;

	return (
			fabs(RelativePosition * Plane) >
			(
			fabs((box1Axis[0] * box1HalfSize.X) * Plane) +
			fabs((box1Axis[1] * box1HalfSize.Y) * Plane) +
			fabs((box1Axis[2] * box1HalfSize.Z) * Plane) +
			fabs((box2Axis[0] * box2HalfSize.X) * Plane) +
			fabs((box2Axis[1] * box2HalfSize.Y) * Plane) +
			fabs((box2Axis[2] * box2HalfSize.Z) * Plane)
			)
		);
}
/*
bool C_CollisionComponent::RayCastSingleTarget(Ray& ray, S_CollisionData& data)
{
	bool hit = false;
	bool firstHit = false;
	S_CollisionData tempData;
	float closest;

	for (const auto& partition : LevelGraph::GetInstance()->GetIntersectedLeaves(ray)) for (const auto& collider : partition->GetColliders())
	{
		auto box = dynamic_cast<C_BoundingBox*>(collider);
		//TODO: Get Rid of
		//if (box) hit = RayBoundingBoxCollision(ray, box->GetBoxBounds(), tempData);
		if (hit)
		{
			if (firstHit)
			{
				if (ray.GetLength() < closest)
				{
					data = tempData;
					data.OtherCollisonComponent = collider;
					//data.OtherGameObject = collider->GetOwner();
					closest = ray.GetLength();
				}
			}
			else
			{
				data = tempData;
				data.OtherCollisonComponent = collider;
				//data.OtherGameObject = collider->GetOwner();
				closest = ray.GetLength();
				firstHit = true;
			}
		}
	}
	return firstHit;
}

bool C_CollisionComponent::RayCastMultiTarget(Ray& ray, std::vector<S_CollisionData>& outData)
{
	if (outData.size() != 0)
	{
		DebugLogger::Warning("outData was not empty, previous data will be lost!", "Core/Objects/Components/CollisionComponent.cpp", __LINE__);
		outData.clear();
		outData.resize(0);
	}

	bool hit = false;
	S_CollisionData tempData;
	float closest;

	for (const auto& partition : LevelGraph::GetInstance()->GetIntersectedLeaves(ray)) for (const auto& collider : partition->GetColliders())
	{
		auto box = dynamic_cast<C_BoundingBox*>(collider);
		//TODO: Get rid of
		//if (box) hit = RayBoundingBoxCollision(ray, box->GetBoxBounds(), tempData);
		if (hit) outData.push_back(tempData);
	}

	outData.shrink_to_fit();

	return outData.size() > size_t(0);
}
*/
void C_CollisionComponent::CheckForCollisions(std::vector<C_CollisionComponent*> colliderVector)
{
	if (colliderVector.size() <= 0) return;
	S_CollisionData data;
	for (int i = 0; i < colliderVector.size(); i++)
	{
		for (int j = i+1; j < colliderVector.size(); j++)
		{
			if (colliderVector[i]->Collide(colliderVector[j], data))
			{
				colliderVector[i]->AddOverlapedCollider(colliderVector[j]);
				colliderVector[j]->AddOverlapedCollider(colliderVector[i]);

				//data.OtherGameObject = colliderVector[j]->GetOwner();
				data.OtherCollisonComponent = colliderVector[j];
				colliderVector[i]->ChooseCollisionType(colliderVector[j], data);

				//data.OtherGameObject = colliderVector[i]->GetOwner();
				data.OtherCollisonComponent = colliderVector[i];
				colliderVector[j]->ChooseCollisionType(colliderVector[i], data);
			}
		}
	}
}

void C_CollisionComponent::ChooseCollisionType(C_CollisionComponent* otherCollider, const S_CollisionData& data)
{
	if (this->CollisionType == NO_COLLISION || otherCollider->CollisionType == NO_COLLISION) return;

	if (this->CollisionType == otherCollider->CollisionType)
	{
		switch (this->CollisionType)
		{
		case OVERLAP:
			OnOverlapBegin(data);
		case COLLISION:
			OnCollision(data);
		default:
			break;
		}
	}
	else OnOverlapBegin(data);
}


bool C_CollisionComponent::Collide(const C_CollisionComponent* otherCollider, S_CollisionData& data) const
{
	if (!otherCollider) return false;

	return GJK(this, otherCollider);
}

void C_CollisionComponent::OnCollision(const S_CollisionData& data)
{
	if(CollisionFunction) CollisionFunction(Owner, data);
}

void C_CollisionComponent::OnOverlapBegin(const S_CollisionData& data)
{
	if(OverlapBeginFunction) OverlapBeginFunction(Owner, data);
}

void C_CollisionComponent::OnOverlapEnd(C_CollisionComponent* otherCollider)
{
	if (OverlapEndFunction)
	{
		OverlapEndFunction(Owner, otherCollider);
	}
}

void C_CollisionComponent::Update(const float deltaTime)
{
	C_TransformComponent::Update(deltaTime);
	S_CollisionData data;

	std::set<C_CollisionComponent*> collidersToRemove;

	for (const auto& collider : OverlapedColliders)
	{
		if (!Collide(collider, data))
		{
			OnOverlapEnd(collider);
			collidersToRemove.insert(collider);
		}
	}

	for (const auto& collider : collidersToRemove) OverlapedColliders.erase(collider);
}

C_CollisionComponent::C_CollisionComponent(O_GameObject* owner, ECollisionType collisionType) : C_TransformComponent(owner)
{
	CollisionType = collisionType;
	//Owner->GetLevel()->AddCollider(this);
}

C_CollisionComponent::~C_CollisionComponent()
{
}

FVector3 C_CollisionComponent::GetFurthestPoint(const FVector3& direction) const
{
	return FVector3();
}

bool C_CollisionComponent::GJK(const C_CollisionComponent* colliderA, const C_CollisionComponent* colliderB)
{
	// Get initial support point in any direction
	FVector3 support = GetFurthestMinkDiffPoint(colliderA, colliderB, FVector3(0.0f, 0.0f, 1.0f));
	
	// Simplex is an array of points, max count is 4
	Simplex points;
	points.PushFront(support);

	// New direction is towards the origin
	FVector3 direction = -support;
	support = GetFurthestMinkDiffPoint(colliderA, colliderB, direction);

	while (true) 
	{
		if (support * direction <= 0) 
		{
			return false; // no collision
		}

		Line(points, direction);
		points.PushFront(support);
		Triangle(points, direction);
		points.PushFront(support);
		if (Tetrahedron(points, direction))
		{
			return true;
		}
		else
		{
			points.PushFront(support);
		}
	}
}

FVector3 C_CollisionComponent::GetFurthestMinkDiffPoint(const C_CollisionComponent* colliderA, const C_CollisionComponent* colliderB, const FVector3& direction)
{
	return colliderA->GetFurthestPoint(direction) - colliderB->GetFurthestPoint(direction * -1.0f);
}

bool C_CollisionComponent::NextSimplex(Simplex& points, FVector3& direction)
{
	switch (points.size())
	{
	case 2: return Line(points, direction);
	case 3: return Triangle(points, direction);
	case 4: return Tetrahedron(points, direction);
	}

	return false;//this should never happen
}

bool C_CollisionComponent::SameDirection(const FVector3& direction, const FVector3& pointToOrigin)
{
	return direction * pointToOrigin;
}

bool C_CollisionComponent::Line(Simplex& points, FVector3& direction)
{
	FVector3 a = points[0];
	FVector3 b = points[1];

	FVector3 ab = b - a;
	FVector3 ao = -a;

	if (SameDirection(ab, ao)) 
	{
		direction = ab.CrossProduct(ao).CrossProduct(ab);
	}
	else 
	{
		points = { a };
		direction = ao;
	}

	return false;
}

bool C_CollisionComponent::Triangle(Simplex& points, FVector3& direction)
{
	FVector3 a = points[0];
	FVector3 b = points[1];
	FVector3 c = points[2];

	FVector3 ab = b - a;
	FVector3 ac = c - a;
	FVector3 ao = -a;

	FVector3 abc = ab.CrossProduct(ac);

	if (SameDirection(abc.CrossProduct(ac), ao)) 
	{
		if (SameDirection(ac, ao)) {
			points = { a, c };
			direction = ac.CrossProduct(ao).CrossProduct(ac);
		}

		else 
		{
			return Line(points = { a, b }, direction);
		}
	}

	else 
	{
		if (SameDirection(ab.CrossProduct(abc), ao)) 
		{
			return Line(points = { a, b }, direction);
		}

		else 
		{
			if (SameDirection(abc, ao)) 
			{
				direction = abc;
			}

			else 
			{
				points = { a, c, b };
				direction = -abc;
			}
		}
	}

	return false;
}

bool C_CollisionComponent::Tetrahedron(Simplex& points, FVector3& direction)
{
	FVector3 a = points[0];
	FVector3 b = points[1];
	FVector3 c = points[2];
	FVector3 d = points[3];

	FVector3 ab = b - a;
	FVector3 ac = c - a;
	FVector3 ad = d - a;
	FVector3 ao = -a;

	FVector3 abc = ab.CrossProduct(ac);
	FVector3 acd = ac.CrossProduct(ad);
	FVector3 adb = ad.CrossProduct(ab);

	if (SameDirection(abc, ao)) {
		return Triangle(points = { a, b, c }, direction);
	}

	if (SameDirection(acd, ao)) {
		return Triangle(points = { a, c, d }, direction);
	}

	if (SameDirection(adb, ao)) {
		return Triangle(points = { a, d, b }, direction);
	}

	return true;
}

//bool C_CollisionComponent::CheckSimplexForOrigin(Simplex& simplex) const
//{
//	/*float v0, v1, v2, v3, v4;
//	FVector4 nim = FVector4(0, 0, 0, 1);
//	v0 = FMatrix4(simplex.GetPointAtIndex(0), simplex.GetPointAtIndex(1), simplex.GetPointAtIndex(2), simplex.GetPointAtIndex(3)).Det();
//	v1 = FMatrix4(nim, simplex.GetPointAtIndex(1), simplex.GetPointAtIndex(2), simplex.GetPointAtIndex(3)).Det();
//	v2 = FMatrix4(simplex.GetPointAtIndex(0), nim, simplex.GetPointAtIndex(2), simplex.GetPointAtIndex(3)).Det();
//	v3 = FMatrix4(simplex.GetPointAtIndex(0), simplex.GetPointAtIndex(1), nim, simplex.GetPointAtIndex(3)).Det();
//	v4 = FMatrix4(simplex.GetPointAtIndex(0), simplex.GetPointAtIndex(1), simplex.GetPointAtIndex(2), nim).Det();
//
//	float v[] = { v1, v2, v3, v3 };
//
//	if (v0 < 0)
//	{
//		return false;
//	}
//	else if(v1 / v0 > 0 && v2 / v0 > 0 && v3 / v0 > 0 && v4 / v0 > 0)
//	{
//		return true;
//	}*/
//
//	FMatrix4 matrices[5];
//
//	matrices[0] = FMatrix4
//	(
//		FVector4(simplex.GetVertexAtIndex(0).X, simplex.GetVertexAtIndex(0).Y, simplex.GetVertexAtIndex(0).Z, 1.0f),
//		FVector4(simplex.GetVertexAtIndex(1).X, simplex.GetVertexAtIndex(1).Y, simplex.GetVertexAtIndex(1).Z, 1.0f),
//		FVector4(simplex.GetVertexAtIndex(2).X, simplex.GetVertexAtIndex(2).Y, simplex.GetVertexAtIndex(2).Z, 1.0f),
//		FVector4(simplex.GetVertexAtIndex(3).X, simplex.GetVertexAtIndex(3).Y, simplex.GetVertexAtIndex(3).Z, 1.0f)
//	);
//
//	int determinatCheckIndex;
//	float matrixDet = matrices[0].Det();
//	float tempDet;
//	for (determinatCheckIndex = 1; determinatCheckIndex < 4; determinatCheckIndex++)
//	{
//		matrices[determinatCheckIndex] = matrices[0];
//		matrices[determinatCheckIndex].SetRow(FVector4(0.0f, 0.0f, 0.0f, 1.0f), determinatCheckIndex - 1);
//		tempDet = matrices[determinatCheckIndex].Det();
//
//		//printf("M: %f\tT: %f\n", matrixDet, tempDet);
//		if (matrixDet / tempDet < 0)
//		{
//			simplex.SetVertexAtIndex(determinatCheckIndex, simplex.GetVertexAtIndex(4));
//			return false;
//		}
//	}
//
//	return true;
//}


template<typename T>
T* C_CollisionComponent::GetCollider() {
	switch (colliderType) {
	case 0:
		return dynamic_cast<C_BoundingBox>(this);
	}
}
