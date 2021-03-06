#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "TransformComponent.h"

#include <set>
#include <vector>
#include "CollisionData.h"

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

	static bool RaySphereCollision(const Ray& ray, const Sphere& sphere, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision = true);
	static bool RayBoxCollision(const Ray& ray, const Box& box, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision = true);
	static bool RayBoundingBoxCollision(Ray& ray, S_BoxBounds box, S_CollisionData& data);
	static bool SphereSphereCollision(const Sphere& sphere0, const Sphere& sphere1, S_CollisionData& data, float tolerance = 0.01f);
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
	virtual bool Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const;
	virtual bool SpatialPartitionCheck(S_BoxBounds box);
	static void CheckForCollisions(std::vector<C_CollisionComponent*> colliderSet);

	virtual void Update(const float deltaTime) override;

	C_CollisionComponent(O_GameObject* owner, ECollisionType collisionType = NO_COLLISION);
	virtual ~C_CollisionComponent();

private:
	friend class OctSpactilPartition;
};
#endif
