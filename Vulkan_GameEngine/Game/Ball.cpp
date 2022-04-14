#include "Ball.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Renderers/RenderObject.h"
#include "Math/FQuaternion.h"
#include "Objects/Components/MeshCollision.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Objects/Components/Colliders/SphereCollider.h"

GO_Ball::GO_Ball(std::string name)
{
	auto phy = AddComponentOfClass<C_PhysicsComponent>();
	phy->SetApplyGravity(true);
	phy->SetRubberness(0.0f);

	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("PoolBall");
	Mesh->SetMaterialName("M_PoolBall");
	Mesh->SetComponentScale(FVector3(0.5f));

	Mesh->SetComponentPosition(GetPosition());

	C_SphereCollider* spherePtr = AddComponentOfClass<C_SphereCollider>();
	spherePtr->SetCollisionType(ECollisionType::COLLISION);
	spherePtr->SetComponentScale(FVector3(1));
	//spherePtr->SetComponentPosition(FVector3(0.0f, -0.5f, 0.0f));

	Collider = AddComponentOfClass<C_MeshCollision>();
	//Mesh->SetComponentPosition(FVector3(0, 2.0f, 0));
}

GO_Ball::~GO_Ball()
{
}

void GO_Ball::Update(float deltaTime)
{
	O_GameObject::Update(deltaTime);
}

void GO_Ball::Start()
{
	Collider->SetCollisionMesh(Mesh);
}

void GO_Ball::OnOverlapBegin(O_GameObject* self, const S_CollisionData& data)
{
}
