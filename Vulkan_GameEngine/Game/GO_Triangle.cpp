#include "GO_Triangle.h"
#include "Level.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Renderers/RenderObject.h"

GO_Triangle::GO_Triangle(O_Level* level) : O_GameObject(level)
{
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("SphereMesh");
	Mesh->SetMaterialName("Ball14");

	Movement = AddComponentOfClass<C_MovementComponent>();
}

void GO_Triangle::Start()
{
}

void GO_Triangle::Update(float deltaTime)
{
	counter += deltaTime;
	SetScale(0.3f);
	Movement->SetAngularVelocity(FVector3(0.0f, 5.0f, 0.0f));
	O_GameObject::Update(deltaTime);
}
