#ifndef MESHCOLLISION_H
#define MESHCOLLISION_H

#include "Colliders/CollisionComponent.h"
#include "Math/FVector3.h"
#include "Geometry/Simplex.h"

class C_StaticMeshComponent;

class C_MeshCollision : public C_CollisionComponent
{
protected:
	C_StaticMeshComponent* CollisionMesh;
	//FVector3 CollisionMeshCenter;
	//bool CheckSimplexForOrigin(Simplex3D& simplex) const;

public:
	C_MeshCollision(O_GameObject* owner);
	virtual FVector3 GetFurthestPoint(const FVector3& direction) const override;

	virtual FVector3 GetCollisionMeshCenter() const override;

	//Returns the difference between the furthest point in direction in meshA and the furthest point in -direction in meshB.
	//static FVector3 GetFurthestMinkDiffPoint(const C_MeshCollision* meshA, const C_MeshCollision* meshB, FVector3 direction);

	void SetCollisionMesh(C_StaticMeshComponent* mesh);
	//inline FVector3 GetCollisionMeshCenter() const { return CollisionMeshCenter; }
	inline C_StaticMeshComponent* GetCollisionMesh() const { return CollisionMesh; }

	//virtual bool Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const override;
};

#endif // ! MESHCOLLISION_H

