#ifndef BILLIARDBALL_H
#define BILLIARDBALL_H

#include "../Engine/Objects/GameObjects/GameObject.h"

class C_PhysicsComponent;
class C_SphereCollider;
class C_StaticMeshComponent;
struct S_CollisionData;
class FVector3;
class GO_BilliardTable;
class C_BoxCollider;

class GO_BilliardBall : public O_GameObject
{
public:
	C_PhysicsComponent* Physics;
	C_SphereCollider* Collider;
	C_StaticMeshComponent* Mesh;

	static void OnCollision(O_GameObject* self, const S_CollisionData& data);
	void BounceOnBall(GO_BilliardBall* otherBall, FVector3 pointOfImpact);
	void BounceOnWall(C_BoxCollider* wall, FVector3 pointOfImpact);
	void Update(float deltaTime) override;

	GO_BilliardBall(std::string name = "");
	~GO_BilliardBall();
protected:
	float Drag = 0.1f;
};
#endif

