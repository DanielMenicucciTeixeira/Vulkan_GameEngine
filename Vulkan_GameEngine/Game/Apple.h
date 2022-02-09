#ifndef APPLE_H
#define APPLE_H

#include "../Engine/Objects/GameObjects/GameObject.h"
#include "string"
class C_StaticMeshComponent;
class C_MovementComponent;

class GO_Apple : public O_GameObject
{
public:
	GO_Apple(L_Level* level, std::string name = "");
	virtual ~GO_Apple();
	virtual void Start() override;
	C_StaticMeshComponent* FruitMesh;
	C_StaticMeshComponent* StemMesh;
	C_MovementComponent* Movement;
};

#endif

