#ifndef GO_TRIANGLE_H
#define GO_TRIANGLE_H
#include "Objects/GameObjects/GameObject.h"

class C_StaticMeshComponent;
class C_MovementComponent;

class GO_Triangle :  public O_GameObject
{
public:
	GO_Triangle(O_Level* level);
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	float counter;
	C_StaticMeshComponent* Mesh;
	C_MovementComponent* Movement;
};
#endif