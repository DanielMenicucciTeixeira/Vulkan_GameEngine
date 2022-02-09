#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "TransformComponent.h"

#include <set>
#include <vector>
#include "Physics/CollisionData.h"
#include "Geometry/Simplex.h"

class Ray;
class Sphere;
class Box;
struct S_BoxBounds;
class FVector3;
class Plane;
class C_BoundingBox;
class O_GameObject;

enum ECollisionType
{
	NO_COLLISION,
	OVERLAP,
	COLLISION
};

class C_CollisionComponent : public C_TransformComponent
{
protected:
	std::set<C_CollisionComponent*> OverlapedColliders;
	ECollisionType CollisionType;
	FVector3 CollisionMeshCenter;

	static bool RaySphereCollision(const Ray& ray, const Sphere& sphere, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision = true);
	static bool RayBoxCollision(const Ray& ray, const Box& box, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision = true);
	static bool RayBoundingBoxCollision(Ray& ray, S_BoxBounds box, S_CollisionData& data);
	//static bool SphereSphereCollision(const Sphere& sphere0, const Sphere& sphere1, S_CollisionData& data, float tolerance = 0.01f);
	static bool SpherePlaneCollision(const Sphere& sphere, const FVector3& direction, const Plane& plane, S_CollisionData& data);
	static bool SphereBoxCollision(const Sphere& sphere, const Box& box, S_CollisionData& data);
	static bool BoundingBoxBoundingBoxCollision(const S_BoxBounds& box1, const S_BoxBounds& box2, S_CollisionData& data);

	virtual void OnCollision(const S_CollisionData& data);
	virtual void OnOverlapBegin(const S_CollisionData& data);
	virtual void OnOverlapEnd(C_CollisionComponent* otherCollider);

	void (*CollisionFunction)(O_GameObject* self, const S_CollisionData& data);
	void (*OverlapBeginFunction)(O_GameObject* self, const S_CollisionData& data);
	void (*OverlapEndFunction)(O_GameObject* self, C_CollisionComponent* otherCollider);

	static bool IsSeparatingPlane(const FVector3& RelativePosition, const FVector3& Plane, const S_BoxBounds& box1, const FVector3 box1Axis[3], const S_BoxBounds& box2, const FVector3 box2Axis[3]);

public:
	static bool RayCastSingleTarget(Ray& ray, S_CollisionData& data);
	static bool RayCastMultiTarget(Ray& ray, std::vector<S_CollisionData>& outData);

	inline ECollisionType GetCollisionType() { return CollisionType; }
	inline void SetCollisionType(ECollisionType type) { CollisionType = type; }
	inline void SetCollisionFunction(static void (*collisionFunction)(O_GameObject* self, const S_CollisionData& data)) { CollisionFunction = collisionFunction; }
	inline void SetOverlapBeginFunction(static void (*overlapBeginFunction)(O_GameObject* self, const S_CollisionData& data)) { OverlapBeginFunction = overlapBeginFunction; }
	inline void SetOverlapEndFunction(static void (*overlapEndFunction)(O_GameObject* self, C_CollisionComponent* otherCollider)) { OverlapEndFunction = overlapEndFunction; }

	void ChooseCollisionType(C_CollisionComponent* otherCollider, const S_CollisionData& data);
	bool IsCollidingWith(C_CollisionComponent* collider);
	inline void AddOverlapedCollider(C_CollisionComponent* collider) { OverlapedColliders.insert(collider); }
	virtual bool Collide(const C_CollisionComponent* otherCollider, S_CollisionData& data) const;
	virtual bool SpatialPartitionCheck(S_BoxBounds box);
	static void CheckForCollisions(std::vector<C_CollisionComponent*> colliderSet);

	virtual void Update(const float deltaTime) override;

	C_CollisionComponent(O_GameObject* owner, ECollisionType collisionType = NO_COLLISION);
	virtual ~C_CollisionComponent();

	inline virtual  FVector3 GetCollisionMeshCenter() const { return CollisionMeshCenter; }
	virtual FVector3 GetFurthestPoint(const FVector3& direction) const;
	static bool GJK(const C_CollisionComponent* colliderA, const C_CollisionComponent* colliderB);
	//Returns the difference between the furthest point in direction in meshA and the furthest point in -direction in meshB.
	static FVector3 GetFurthestMinkDiffPoint(const C_CollisionComponent* colliderA, const C_CollisionComponent* colliderB, const FVector3& direction);
	static bool NextSimplex(Simplex& points, FVector3& direction);
	static bool SameDirection(const FVector3& direction, const FVector3& pointToOrigin);
	static bool Line(Simplex& points, FVector3& direction);
	static bool Triangle(Simplex& points, FVector3& direction);
	static bool Tetrahedron(Simplex& points, FVector3& direction);

private:
	//bool CheckSimplexForOrigin(Simplex& simplex) const;

	friend class OctSpactilPartition;
};
#endif
