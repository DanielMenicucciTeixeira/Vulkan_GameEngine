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
	inline void SetModel(FMatrix4* model) { Model = model; }
	virtual void Start() override;

protected:
	FVector3 Min;
	FVector3 Max;
	FMatrix4* Model;
};
#endif

