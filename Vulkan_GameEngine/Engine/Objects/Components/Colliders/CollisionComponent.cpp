#include "CollisionComponent.h"
#include "Geometry/Ray.h"
#include "Geometry/Sphere.h"
#include "Geometry/Box.h"
#include "Geometry/Plane.h"
#include "Math/FVector3.h"
#include <cmath>
#include <iostream>
#include "BoxCollider.h"
#include "Objects\GameObjects\GameObject.h"
#include "Level.h"
#include "LevelGraph.h"
#include "OctSpatialPartition.h"
#include "CollisionHandler.h"
#include "../CameraComponent.h"


bool C_CollisionComponent::SpherePlaneCollision(const Sphere& sphere, const FVector3& direction, const Plane& plane, S_CollisionData& data)
{

	FVector3 collisionDirection = ((plane.GetPlaneNormal() * -1) + sphere.position).GetNormal();

	FVector3 collisionPoint;
	bool result = plane.InterssectionPoint(Ray(sphere.position, collisionDirection, sphere.radius, false), collisionPoint);
	data.CollisionPoint = collisionPoint;
	return result;
}


void C_CollisionComponent::SetCollisionType(ECollisionType type)
{
	if (CollisionType == type) { return; }
	if (CollisionType != ECollisionType::NO_COLLISION) {
		CollisionHandler::GetInstance()->RemoveCollider(this);
	}
	CollisionType = type;

	if (CollisionType != ECollisionType::NO_COLLISION) {
		CollisionHandler::GetInstance()->AddCollider(this);
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
			break;
		case COLLISION:
			OnCollision(data);
			break;
		default:
			break;
		}
	}
	else OnOverlapBegin(data);
}

//void C_CollisionComponent::SetComponentPosition(const FVector3& position)
//{
	//Do the static check here as well?
//	Transform->SetPosition(position);
	//TODO:Do check to see if in same collider here.
	///Theory
	///
	/// have a pointer to keep track of where what partiton you are in.
	/// what do we do about the time in between the two?
	/// 
	/// Call function in coll handler that takes in the colliders shape and the cell pointer.
	/// 
	/// will have to make three new functions for spatial part to take in the collider types.
	/// 
	/// if the cells don't match up then remove the pointer from the list and add it to the new one, also change the pointer adress
	/// in the collider to the new one. (could this be done in the function by changing it?  would that work?)
	/// 
//}

void C_CollisionComponent::SetCurrentNodes(std::vector<OctNode*> node)
{
	CurrentNodes.clear();
	CurrentNodes = node;
}

void C_CollisionComponent::SetCurrentNodes(OctNode* node)
{
	CurrentNodes.push_back(node);
}

std::vector<OctNode*> C_CollisionComponent::GetCurrentNodes()
{
	return CurrentNodes;
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

bool C_CollisionComponent::CheckHasCollidedBefore(C_CollisionComponent* comp)
{
	if (prevOverlaps.find(comp) == prevOverlaps.end()) {
		return false;
	}
	return true;
}

void C_CollisionComponent::Update(const float deltaTime)
{
	C_TransformComponent::Update(deltaTime);

	if (CollisionType == ECollisionType::OVERLAP) { SetDoneOverlapCheck(false); }
}

C_CollisionComponent::C_CollisionComponent(O_GameObject* owner, ECollisionType collisionType) : C_TransformComponent(owner, owner->GetIsStatic()), O_Component(owner)
{
	CurrentNodes.reserve(5);
	SetCollisionType(collisionType);
}

C_CollisionComponent::~C_CollisionComponent()
{
	CurrentNodes.clear();
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
