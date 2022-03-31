#include "Wall.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Renderers/RenderObject.h"
#include "Math/FQuaternion.h"
#include "Objects/Components/MeshCollision.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Objects/Components/Colliders/BoundingBox.h"

GO_Wall::GO_Wall(std::string name)
{
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("Box001");
	Mesh->SetMaterialName("M_diceTexture");

	AddComponentOfClass<C_PhysicsComponent>();

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
	Collider->SetCollisionMesh(Mesh);
}
