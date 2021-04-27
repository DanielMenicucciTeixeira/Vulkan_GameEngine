#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "CollisionComponent.h"

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
	virtual FVector3 GetMin();
	virtual FVector3 GetMax();
	inline const FVector3& GetRelativeMin() const { return Min; }
	inline const FVector3& GetRelativeMax() const { return Max; }

protected:
	FVector3 Min;
	FVector3 Max;
};
#endif

