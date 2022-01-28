#pragma once

#include "CollisionData.h"

class C_BoundingBox;
class C_BoxCollider;
class C_SphereCollider;

/// <summary>
/// The role of this class is to facilatate collision detection
/// </summary>
class CollisionDetection
{
	CollisionDetection(const CollisionDetection&) = delete;
	CollisionDetection(CollisionDetection&&) = delete;
	CollisionDetection& operator =(const CollisionDetection&) = delete;
	CollisionDetection& operator =(CollisionDetection&&) = delete;

	CollisionDetection() = delete;

	//AABB Collision.  Returns points of collision if true.
	bool Collision(C_BoundingBox a, C_BoundingBox b);

	//Box Box Collision.  Returns points of collision if true.
	bool Collision(C_BoxCollider a, C_BoxCollider b);

	//Sphere Sphere Collision.  Returns points of collision if true.
	FVector3 * Collision(C_SphereCollider a, C_SphereCollider b);

	

};

