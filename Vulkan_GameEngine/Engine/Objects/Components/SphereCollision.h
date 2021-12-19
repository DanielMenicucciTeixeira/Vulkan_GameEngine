#ifndef SPHERECOLLISION_H
#define SPHERECOLLISION_H

#include "CollisionComponent.h"

class Sphere;

class C_SphereCollision : public C_CollisionComponent
{
protected:
	Sphere* CollisionSphere;

public:
	void SetComponentPosition(const FVector3& position) override;
	void SetComponentScale(const FVector3& scale) override;
	void SetComponentTransform(const FTransform& transform) override;
	float GetRadius();
	void SetRadius(const float& radius);
	Sphere GetCollisionSphere();

	void Update(const float deltaTime) override;

	//bool Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const override;

	C_SphereCollision(O_GameObject* owner);
	virtual ~C_SphereCollision();
};
#endif

