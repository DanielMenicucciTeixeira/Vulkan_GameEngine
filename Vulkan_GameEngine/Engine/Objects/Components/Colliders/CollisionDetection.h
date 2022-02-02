#pragma once

class C_BoundingBox;

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

	bool Collision(C_BoundingBox a, C_BoundingBox b);

};

