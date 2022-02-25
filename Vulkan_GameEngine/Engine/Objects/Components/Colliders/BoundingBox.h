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
	void GetDimensionsFromMesh(S_Mesh* mesh);
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual FVector3 GetMin();
	virtual FVector3 GetMax();
	inline const FVector3& GetRelativeMin() const { return boxBounds.Min; }
	inline const FVector3& GetRelativeMax() const { return boxBounds.Max; }
	inline const FMatrix4& GetModelMatrix() const { return boxBounds.Model; }
	inline S_BoxBounds GetBoxBounds() const { return boxBounds; }

	virtual void SetComponentPosition(const FVector3& position) override;

protected:

	S_BoxBounds boxBounds;
};
#endif

