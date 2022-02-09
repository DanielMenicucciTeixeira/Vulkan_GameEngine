#ifndef BILLIARDTABLE_H
#define BILLIARDTABLE_H

#include "../Engine/Objects/GameObjects/GameObject.h"

class C_PhysicsComponent;
class C_BoxCollider;
class C_StaticMeshComponent;
struct S_CollisionData;
class FVector3;
class L_Level;

class GO_BilliardTable : public O_GameObject
{
public:
	C_PhysicsComponent* Physics;
	C_StaticMeshComponent* Mesh;
	
	C_BoxCollider* BackWall;
	C_BoxCollider* FrontWall;
	C_BoxCollider* LeftWall;
	C_BoxCollider* RightWall;

	GO_BilliardTable(L_Level* level = nullptr, std::string name = "");
};
#endif

