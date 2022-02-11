#pragma once

#include "CollisionData.h"
#include "Geometry/BoxBounds.h"
#include "Math/Math.h"

class C_BoundingBox;
class C_BoxCollider;
class C_SphereCollider;
class Ray;
class Sphere;

using namespace MATH;

/// <summary>
/// The role of this class is to facilatate collision detection
/// </summary>
class CollisionDetection
{
public:
	CollisionDetection(const CollisionDetection&) = delete;
	CollisionDetection(CollisionDetection&&) = delete;
	CollisionDetection& operator =(const CollisionDetection&) = delete;
	CollisionDetection& operator =(CollisionDetection&&) = delete;

	CollisionDetection() = delete;
	~CollisionDetection();


	//Ray & BoundingBox single collision
	static bool RayAABBIntersection(Ray& a, const S_BoxBounds b);

	//TODO: Ray & Sphere single collision

	//TODO: Ray & BoundingBox multi collision

	//TODO: Ray & Sphere multi collision

	//Sphere & BoundingBox collision
	static bool SphereAABBIntersection(Sphere& a, const S_BoxBounds b);

	//Sphere & Sphere collision
	static bool SphereSphereIntersection(Sphere& a, Sphere b);

	/*
	//AABB Collision.  Returns points of collision if true.
	static bool Collision(C_BoundingBox a, C_BoundingBox b);

	//Box Box Collision.  Returns points of collision if true.
	static bool Collision(C_BoxCollider a, C_BoxCollider b);

	//Sphere Sphere Collision.  Returns points of collision if true.
	static FVector3 * Collision(C_SphereCollider a, C_SphereCollider b);
	*/

	//Return collision data of last detection
	static S_CollisionData GetCollisionData();
	
private:
	static S_CollisionData collisionData;
};

