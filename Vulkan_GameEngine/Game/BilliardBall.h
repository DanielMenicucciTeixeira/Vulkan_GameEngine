#ifndef BILLIARDBALL_H
#define BILLIARDBALL_H

#include "../Engine/Objects/GameObjects/GameObject.h"

class C_PhysicsComponent;
class C_SphereCollision;
class C_StaticMeshComponent;
struct S_CollisionData;
class FVector3;
class GO_BilliardTable;
class C_BoxCollision;
class L_Level;

class GO_BilliardBall : public O_GameObject
{
public:
	C_PhysicsComponent* Physics;
	C_SphereCollision* Collider;
	C_StaticMeshComponent* Mesh;

	static void OnCollision(O_GameObject* self, const S_CollisionData& data);
	void BounceOnBall(GO_BilliardBall* otherBall, FVector3 pointOfImpact);
	void BounceOnWall(C_BoxCollision* wall, FVector3 pointOfImpact);
	void Update(float deltaTime) override;

	GO_BilliardBall(L_Level* level = nullptr, std::string name = "");
	~GO_BilliardBall();
protected:
	float Drag = 0.1f;
};
#endif

