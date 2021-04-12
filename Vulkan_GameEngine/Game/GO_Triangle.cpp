#include "GO_Triangle.h"
#include "Level.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Renderers/RenderObject.h"
#include "Math/FQuaternion.h"

GO_Triangle::GO_Triangle(L_Level* level) : O_GameObject(level)
{
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("SphereMesh");
	Mesh->SetMaterialName("Ball14");
	Movement = AddComponentOfClass<C_MovementComponent>();
}

void GO_Triangle::Start()
{
	SetRotation(FQuaternion({ 0, 0, 1 }, 180.0f));
	Movement->SetAngularVelocity(FVector3(0.0f, 3.0f, 0.0f));
	O_GameObject::Start();
}