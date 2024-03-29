#include "Wall.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Renderers/RenderObject.h"
#include "Math/FQuaternion.h"
#include "Objects/Components/MeshCollision.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Objects/Components/Colliders/BoundingBox.h"

GO_Wall::GO_Wall(std::string name) : O_GameObject(name, true)
{
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("Box001");
	Mesh->SetMaterialName("M_Brown");
	//Mesh->SetComponentScale(FVector3(50.0f, 10.0f, 1.0f));



	//AddComponentOfClass<C_PhysicsComponent>();

	Collider = AddComponentOfClass<C_MeshCollision>();
}

GO_Wall::~GO_Wall()
{
}

void GO_Wall::Update(float deltaTime)
{
	O_GameObject::Update(deltaTime);

}

void GO_Wall::Start()
{
	Mesh->SetComponentPosition(GetPosition());
	Collider->SetCollisionMesh(Mesh);
}

void GO_Wall::Overlap(O_GameObject* self, const S_CollisionData& data)
{
	int i = 0;
}
