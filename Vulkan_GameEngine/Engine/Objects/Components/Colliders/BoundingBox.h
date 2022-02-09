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
	inline const FVector3& GetRelativeMin() const { return min; }
	inline const FVector3& GetRelativeMax() const { return max; }
	inline const FMatrix4& GetModelMatrix() const { return model; }
	//inline const S_BoxBounds& GetBoxBounds() const { return BoxBounds; }

protected:
	FVector3 max = FVector3();
	FVector3 min = FVector3();
	FMatrix4 model = FMatrix4();

	//S_BoxBounds BoxBounds;
};
#endif

