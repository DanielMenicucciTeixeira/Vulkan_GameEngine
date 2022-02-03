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

	//Ray & Bounding box collision
	static bool Collision(Ray& a, const S_BoxBounds b);

	//AABB Collision.  Returns points of collision if true.
	bool Collision(C_BoundingBox a, C_BoundingBox b);

	//Box Box Collision.  Returns points of collision if true.
	bool Collision(C_BoxCollider a, C_BoxCollider b);

	//Sphere Sphere Collision.  Returns points of collision if true.
	FVector3 * Collision(C_SphereCollider a, C_SphereCollider b);

	//Return collision data of last detection
	S_CollisionData GetCollisionData();
	
private:
	static S_CollisionData collisionData;
};

