#pragma once

#include "CollisionData.h"
#include "Geometry/BoxBounds.h"
#include "Math/Math.h"



class Ray;
class BoxBounds;
class Sphere;
class S_Box;

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
	static bool RayAABBIntersection(Ray a, const BoxBounds b);

	//Ray & Sphere collision, Geometry based
	static bool RaySphereIntersection(Ray ray, Sphere sphere);

	//Ray & S_Box collision
	static bool RayOBBIntersection(Ray a, S_Box b);

	//Sphere

	//Sphere & Sphere collision
	static bool SphereIntersection(Sphere a, Sphere b);

	//Sphere & Sphere collision w/ distance return
	static float SphereIntersectionDistance(Sphere a, Sphere b);

	//Sphere & BoundingBox collision
	static bool SphereAABBIntersection(Sphere sphere, const BoxBounds box);

	//Sphere & S_Box collision
	static bool SphereOBBIntersection(Sphere a, S_Box b, FVector3 boxPos);

	//AABB

	//AABB & AABB collision.
	static bool AABBIntersection(BoxBounds a, BoxBounds b);

	//AABB & S_Box collision
	static bool AABBOBBIntersection(BoxBounds a, S_Box b, FVector3 comPos);

	//S_Box

	//S_Box & S_Box collision
	static bool OBBIntersection(S_Box a, S_Box b);



	//Return collision data of last detection
	static S_CollisionData GetCollisionData();
	
private:
	static S_CollisionData collisionData;

	static bool IsSeparatingPlane(const FVector3 RelativePosition, const FVector3 plane, FVector3 box1HalfSize, FVector3 box1Axis[3], FVector3 box2HalfSize, FVector3 box2Axis[3]);
};

