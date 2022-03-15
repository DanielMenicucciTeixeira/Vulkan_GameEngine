#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "../TransformComponent.h"

#include <set>
#include <vector>
#include "Physics/CollisionData.h"
#include "Geometry/Simplex.h"

class Ray;
class Sphere;
class Box;
class FVector3;
class Plane;
class O_GameObject;
class OctNode;

enum ECollisionType
{
	NO_COLLISION,
	OVERLAP,
	COLLISION
};

enum class ColliderType {
	BoundingBox = 0,
	Sphere = 1,
	Box = 2
};

class C_CollisionComponent : public C_TransformComponent
{
public:
	inline ECollisionType GetCollisionType() { return CollisionType; }
	inline void SetCollisionType(ECollisionType type) { CollisionType = type; }
	inline void SetCollisionFunction(static void (*collisionFunction)(O_GameObject* self, const S_CollisionData& data)) { CollisionFunction = collisionFunction; }
	inline void SetOverlapBeginFunction(static void (*overlapBeginFunction)(O_GameObject* self, const S_CollisionData& data)) { OverlapBeginFunction = overlapBeginFunction; }
	inline void SetOverlapEndFunction(static void (*overlapEndFunction)(O_GameObject* self, C_CollisionComponent* otherCollider)) { OverlapEndFunction = overlapEndFunction; }

	inline ColliderType GetColliderType() { return colliderType; };

	void ChooseCollisionType(C_CollisionComponent* otherCollider, const S_CollisionData& data);

	//Node Functions

	void SetCurrentNodes(std::vector<OctNode*> nodes);
	void SetCurrentNodes(OctNode* node);
	std::vector<OctNode*> GetCurrentNodes();
	inline void ClearNodes() { CurrentNodes.clear(); }

	virtual void Update(const float deltaTime) override;

	C_CollisionComponent(O_GameObject* owner, ECollisionType collisionType = NO_COLLISION);
	virtual ~C_CollisionComponent();

	inline virtual FVector3 GetCollisionMeshCenter() const { return CollisionMeshCenter; }
	virtual FVector3 GetFurthestPoint(const FVector3& direction) const;
	static bool GJK(const C_CollisionComponent* colliderA, const C_CollisionComponent* colliderB);

	//Returns the difference between the furthest point in direction in meshA and the furthest point in -direction in meshB.
	static FVector3 GetFurthestMinkDiffPoint(const C_CollisionComponent* colliderA, const C_CollisionComponent* colliderB, const FVector3& direction);
	static bool NextSimplex(Simplex& points, FVector3& direction);
	static bool SameDirection(const FVector3& direction, const FVector3& pointToOrigin);
	static bool Line(Simplex& points, FVector3& direction);
	static bool Triangle(Simplex& points, FVector3& direction);
	static bool Tetrahedron(Simplex& points, FVector3& direction);

protected:
	ECollisionType CollisionType;
	FVector3 CollisionMeshCenter;

	static bool RayBoxCollision(const Ray& ray, const Box& box, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision = true);
	static bool SpherePlaneCollision(const Sphere& sphere, const FVector3& direction, const Plane& plane, S_CollisionData& data);
	static bool SphereBoxCollision(const Sphere& sphere, const Box& box, S_CollisionData& data);

	virtual void OnCollision(const S_CollisionData& data);
	virtual void OnOverlapBegin(const S_CollisionData& data);
	virtual void OnOverlapEnd(C_CollisionComponent* otherCollider);

	void (*CollisionFunction)(O_GameObject* self, const S_CollisionData& data);
	void (*OverlapBeginFunction)(O_GameObject* self, const S_CollisionData& data);
	void (*OverlapEndFunction)(O_GameObject* self, C_CollisionComponent* otherCollider);

private:
	//bool CheckSimplexForOrigin(Simplex& simplex) const;

	ColliderType colliderType;

	std::vector<OctNode*> CurrentNodes;
};
#endif
