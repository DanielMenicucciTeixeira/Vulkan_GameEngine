#ifndef BILLIARDTABLE_H
#define BILLIARDTABLE_H

#include "GameObject.h"

class C_PhysicsComponent;
class C_BoxCollision;
class C_StaticMeshComponent;
struct S_CollisionData;
class FVector3;

class GO_BilliardTable : public O_GameObject
{
public:
	C_PhysicsComponent* Physics;
	C_StaticMeshComponent* Mesh;
	
	C_BoxCollision* BackWall;
	C_BoxCollision* FrontWall;
	C_BoxCollision* LeftWall;
	C_BoxCollision* RightWall;

	GO_BilliardTable();
};
#endif

