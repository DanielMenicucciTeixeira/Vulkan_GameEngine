#pragma once

#include "CollisionData.h"
#include "Geometry/BoxBounds.h"
#include "Math/Math.h"



class Ray;
class S_BoxBounds;
class Sphere;
class Box;

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

	//Ray

	//Ray & BoundingBox collision
	static bool RayAABBIntersection(Ray a, const S_BoxBounds b);

	//Ray & Sphere collision, Geometry based
	static bool RaySphereIntersection(Ray a, Sphere b);

	//Ray & Box collision
	static bool RayOBBIntersection(Ray a, Box b);

	//Sphere

	//Sphere & Sphere collision
	static bool SphereIntersection(Sphere a, Sphere b);

	//Sphere & Sphere collision w/ distance return
	static float SphereIntersectionDistance(Sphere a, Sphere b);

	//Sphere & BoundingBox collision
	static bool SphereAABBIntersection(Sphere a, const S_BoxBounds b);

	//Sphere & Box collision
	static bool SphereOBBIntersection(Sphere a, Box b);

	//AABB

	//AABB & AABB collision.
	static bool AABBIntersection(S_BoxBounds a, S_BoxBounds b);

	//AABB & Box collision
	static bool AABBOBBIntersection(S_BoxBounds a, Box b);

	//Box

	//Box & Box collision
	static bool OBBIntersection(Box a, Box b);





	//Return collision data of last detection
	static S_CollisionData GetCollisionData();
	
private:
	static S_CollisionData collisionData;
};

