#include "BilliardTable.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Geometry/Box.h"

GO_BilliardTable::GO_BilliardTable(L_Level* level, std::string name) : O_GameObject(level, name)
{
	Name = "Table";
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	SetRoot(Mesh);
	Mesh->SetMeshName("BilliardTable");
	Mesh->SetMaterialName("Wood");
	Physics = AddComponentOfClass<C_PhysicsComponent>();
	
	BackWall = AddComponentOfClass<C_BoxCollider>();
	FrontWall = AddComponentOfClass<C_BoxCollider>();
	LeftWall = AddComponentOfClass<C_BoxCollider>();
	RightWall = AddComponentOfClass<C_BoxCollider>();

	BackWall->SetCollisionType(COLLISION);
	FrontWall->SetCollisionType(COLLISION);
	LeftWall->SetCollisionType(COLLISION);
	RightWall->SetCollisionType(COLLISION);

	BackWall->SetBoxExtent(FVector3(30.0f, 0.1f, 10.f));
	FrontWall->SetBoxExtent(FVector3(30.0f, -0.1f, 10.f));
	LeftWall->SetBoxExtent(FVector3(0.1f, 30.0f, 10.f));
	RightWall->SetBoxExtent(FVector3(0.1f, 30.0f, 10.f));

	BackWall->SetComponentPosition(FVector3(-15.0f, 4.0f, -5.0f));
	FrontWall->SetComponentPosition(FVector3(-15.0f, 0.0f, -5.0f));
	LeftWall->SetComponentPosition(FVector3(-1.5f, -15.0f, -5.0f));
	RightWall->SetComponentPosition(FVector3(1.5f, -15.0f, -5.0f));
}
