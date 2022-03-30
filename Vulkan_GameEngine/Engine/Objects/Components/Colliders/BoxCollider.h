#ifndef BOXCOLLISION_H
#define BOXBOLLISION_H

#include "CollisionComponent.h"
#include "Geometry/Box.h"

class S_Mesh;

class C_BoxCollider : public C_CollisionComponent
{
protected:
	S_Box CollisionBox;

public:
	C_BoxCollider(O_GameObject* owner);
	virtual ~C_BoxCollider();

	void GetDimensionsFromMesh(S_Mesh* mesh);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual FVector3 GetMin();
	virtual FVector3 GetMax();
	inline const FVector3& GetRelativeMin() const { return CollisionBox.Min; }
	inline const FVector3& GetRelativeMax() const { return CollisionBox.Max; }


	void SetComponentTransform(const FTransform& transform) override;
	void SetComponentPosition(const FVector3& position) override;
	void SetComponentRotation(const FQuaternion& rotation) override;
	void SetComponentScale(const FVector3& scale) override;

	S_Box GetCollisionBox();

	//bool Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const override;

	void SetBoxExtent(FVector3 extent);



	virtual void PostUpdate(float deltaTime) override;
};
#endif

