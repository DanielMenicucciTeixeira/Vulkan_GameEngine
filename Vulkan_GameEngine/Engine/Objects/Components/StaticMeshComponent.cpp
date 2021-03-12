#include "StaticMeshComponent.h"
#include "Objects/GameObjects/GameObject.h"
#include "Renderers/RenderObject.h"
#include "Math/FMatrix4.h"
#include "Math/FTransform.h"

void C_StaticMeshComponent::Start()
{
	*ModelMatrix = (Owner->GetTransform() + Transform).GetModelMatrix();
}

void C_StaticMeshComponent::Update(const float deltaTime)
{
	*ModelMatrix = (Owner->GetTransform() + Transform).GetModelMatrix();
}

C_StaticMeshComponent::C_StaticMeshComponent(O_GameObject* owner) : C_RenderableComponent(owner)
{
	ModelMatrix = new FMatrix4();
}

C_StaticMeshComponent::~C_StaticMeshComponent()
{
	if (ModelMatrix) delete(ModelMatrix);
}
