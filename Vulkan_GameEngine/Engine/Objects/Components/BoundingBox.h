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
	virtual bool SpatialPartitionCheck(S_BoxBounds box) override;
	inline const FVector3& GetRelativeMin() const { return BoxBounds.Min; }
	inline const FVector3& GetRelativeMax() const { return BoxBounds.Max; }
	inline const S_BoxBounds& GetBoxBounds() const { return BoxBounds; }

protected:
	S_BoxBounds BoxBounds;
};
#endif

