#ifndef BOXCOLLISION_H
#define BOXBOLLISION_H

#include "CollisionComponent.h"

class Box;

class C_BoxCollision : public C_CollisionComponent
{
protected:
	Box* CollisionBox;

public:
	void SetComponentPosition(const FVector3& position) override;
	void SetComponentTransform(const FTransform& transform) override;
	Box GetCollisionBox();

	bool Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const override;

	void SetBoxExtent(FVector3 extent);

	C_BoxCollision(O_GameObject* owner);
	virtual ~C_BoxCollision();
};
#endif

