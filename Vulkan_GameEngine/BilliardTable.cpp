#include "BilliardTable.h"
#include "BoxCollision.h"
#include "StaticMeshComponent.h"
#include "PhysicsComponent.h"
#include "Geometry/Box.h"

GO_BilliardTable::GO_BilliardTable(O_Level* level) : O_GameObject(level)
{
	Name = "Table";
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	SetRoot(Mesh);
	Mesh->SetMeshName("BilliardTable");
	Mesh->SetTextureName("Wood");
	Physics = AddComponentOfClass<C_PhysicsComponent>();
	
	BackWall = AddComponentOfClass<C_BoxCollision>();
	FrontWall = AddComponentOfClass<C_BoxCollision>();
	LeftWall = AddComponentOfClass<C_BoxCollision>();
	RightWall = AddComponentOfClass<C_BoxCollision>();

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
