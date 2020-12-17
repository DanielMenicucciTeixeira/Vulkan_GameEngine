#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "TransformComponent.h"

#include <set>
#include <vector>
#include "CollisionData.h"

class Ray;
class Sphere;
class Box;
class FVector3;
class Plane;
class O_GameObject;

enum ECollisionType
{
	NO_COLLISION,
	OVERLAP,
	COLLISION,
	BOTH
};

class C_CollisionComponent : public C_TransformComponent
{
protected:
	std::set<C_CollisionComponent*> OverlapedColliders;
	ECollisionType CollisionType;

	static bool RaySphereCollision(const Ray& ray, const Sphere& sphere, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision = true);
	static bool RayBoxCollision(const Ray& ray, const Box& box, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision = true);
	static bool SphereSphereCollision(const Sphere& sphere0, const Sphere& sphere1, S_CollisionData& data, float tolerance = 0.01f);
	static bool SpherePlaneCollision(const Sphere& sphere, const FVector3& direction, const Plane& plane, S_CollisionData& data);
	static bool SphereBoxCollision(const Sphere& sphere, const Box& box, S_CollisionData& data);

	virtual void OnCollision(const S_CollisionData& data);
	virtual void OnOverlapBegin(const S_CollisionData& data);
	virtual void OnOverlapEnd(C_CollisionComponent* otherCollider);

	void (*CollisionFunction)(O_GameObject* self, const S_CollisionData& data);
	void (*OverlapBeginFunction)(O_GameObject* self, const S_CollisionData& data);
	void (*OverlapEndFunction)(O_GameObject* self, C_CollisionComponent* otherCollider);

public:
	inline ECollisionType GetCollisionType() { return CollisionType; }
	inline void SetCollisionType(ECollisionType type) { CollisionType = type; }
	inline void SetCollisionFunction(static void (*collisionFunction)(O_GameObject* self, const S_CollisionData& data)) { CollisionFunction = collisionFunction; }
	inline void SetOverlapBeginFunction(static void (*overlapBeginFunction)(O_GameObject* self, const S_CollisionData& data)) { OverlapBeginFunction = overlapBeginFunction; }
	inline void SetOverlapEndFunction(static void (*overlapEndFunction)(O_GameObject* self, C_CollisionComponent* otherCollider)) { OverlapEndFunction = overlapEndFunction; }

	void ChooseCollisionType(C_CollisionComponent* otherCollider, const S_CollisionData& data);
	bool IsCollidingWith(C_CollisionComponent* collider);
	inline void AddOverlapedCollider(C_CollisionComponent* collider) { OverlapedColliders.insert(collider); }
	virtual bool Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const;

	static void CheckForCollisions(std::vector<C_CollisionComponent*> colliderSet);

	void Update(float deltaTime) override;

	C_CollisionComponent(O_GameObject* owner, ECollisionType collisionType = NO_COLLISION);
	~C_CollisionComponent();
};
#endif
