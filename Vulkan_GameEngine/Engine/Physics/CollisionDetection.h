#pragma once

#include "CollisionData.h"

class C_BoundingBox;
class C_BoxCollider;
class C_SphereCollider;
class Ray;
class S_BoxBounds;


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

	//Ray & Bounding box collision
	static bool RayObbIntersection(Ray& a, const C_BoundingBox & b);

	//Ray & S_BoxBounds collision
	static bool RayObbIntersection(Ray& a, const S_BoxBounds& b);

	static bool Collision(Ray& a, const C_SphereCollider b);

	static bool Collision(C_BoundingBox a, const C_SphereCollider b);

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

