#include "Apple.h"
#include "Level.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Renderers/RenderObject.h"
#include "Math/FQuaternion.h"

GO_Apple::GO_Apple(O_Level* level) : O_GameObject(level)
{
	FruitMesh = AddComponentOfClass<C_StaticMeshComponent>();
	FruitMesh->SetMeshName("GeoSphere");
	FruitMesh->SetMaterialName("Apple_Body");

	StemMesh = AddComponentOfClass<C_StaticMeshComponent>();
	StemMesh->SetMeshName("Cylinder001");
	StemMesh->SetMaterialName("Apple_Stem");
	
	Movement = AddComponentOfClass<C_MovementComponent>();
}

GO_Apple::~GO_Apple()
{
}

void GO_Apple::Start()
{
	SetRotation(FQuaternion({ 1, 0, 0 }, -90));
	Movement->SetAngularVelocity(FVector3(0.0f, 3.0f, 0.0f));
	O_GameObject::Start();
}
