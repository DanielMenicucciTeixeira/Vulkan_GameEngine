#ifndef APPLE_H
#define APPLE_H

#include "Objects/GameObjects/GameObject.h"
class C_StaticMeshComponent;
class C_MovementComponent;

class GO_Apple : public O_GameObject
{
public:
	GO_Apple(O_Level* level);
	virtual ~GO_Apple();
	virtual void Start() override;
	C_StaticMeshComponent* FruitMesh;
	C_StaticMeshComponent* StemMesh;
	C_MovementComponent* Movement;
};

#endif

