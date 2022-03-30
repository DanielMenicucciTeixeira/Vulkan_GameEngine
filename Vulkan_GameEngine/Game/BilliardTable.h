#ifndef BILLIARDTABLE_H
#define BILLIARDTABLE_H

#include "../Engine/Objects/GameObjects/GameObject.h"

class C_PhysicsComponent;
class C_BoundingBox;
class C_StaticMeshComponent;
struct S_CollisionData;
class FVector3;

class GO_BilliardTable : public O_GameObject
{
public:
	C_PhysicsComponent* Physics;
	C_StaticMeshComponent* Mesh;
	
	C_BoundingBox* BackWall;
	C_BoundingBox* FrontWall;
	C_BoundingBox* LeftWall;
	C_BoundingBox* RightWall;

	GO_BilliardTable(std::string name = "");
};
#endif

