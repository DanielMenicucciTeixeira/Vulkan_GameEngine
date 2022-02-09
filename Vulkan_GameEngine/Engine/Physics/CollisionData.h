#ifndef COLLISIONDATA_H
#define COLLISIONDATA_H

class C_CollisionComponent;
//class O_GameObject;
//class FVector3;

#include "../Math/FVector3.h"

struct S_CollisionData
{

	//O_GameObject* OtherGameObject = nullptr;
	C_CollisionComponent* OtherCollisonComponent = nullptr;
	FVector3 CollisionPoint;
};
#endif