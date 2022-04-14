#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "CollisionComponent.h"
#include "Geometry/BoxBounds.h"
#include <set>

class FMatrix4;
struct S_Mesh;

class C_BoundingBox : public C_CollisionComponent
{
public:
	C_BoundingBox(O_GameObject* owner, ECollisionType type = NO_COLLISION);
	virtual ~C_BoundingBox();
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	inline BoxBounds GetBoxBounds() const { return boxBounds; }

	void GetBoundsFromMesh(S_Mesh* mesh, FVector3 pos, FVector3 scale);


	virtual void SetComponentPosition(const FVector3& position) override;
	void SetComponentScale(const FVector3& scale) override;
	
	void RefreshBox();

protected:

	FVector3 scale;
	BoxBounds boxBounds;
};
#endif

