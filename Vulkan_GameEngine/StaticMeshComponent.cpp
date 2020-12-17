#include "StaticMeshComponent.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Math/FMatrix4.h"
#include "Math/FTransform.h"

void C_StaticMeshComponent::Start()
{
	*ModelMatrix = (Owner->GetTransform() + Transform).GetModelMatrix();
}

void C_StaticMeshComponent::Update(float deltaTime)
{
	*ModelMatrix = (Owner->GetTransform() + Transform).GetModelMatrix();
}

C_StaticMeshComponent::C_StaticMeshComponent(O_GameObject* owner) : C_RenderableComponent(owner)
{
	Transform = new FTransform();
	ModelMatrix = new FMatrix4();
}

C_StaticMeshComponent::~C_StaticMeshComponent()
{
	if (ModelMatrix) delete(ModelMatrix);
}
