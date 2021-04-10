#include "StaticMeshComponent.h"
#include "Objects/GameObjects/GameObject.h"
#include "Renderers/RenderObject.h"
#include "Math/FMatrix4.h"
#include "Math/FTransform.h"
#include "LevelGraph.h"

void C_StaticMeshComponent::Start()
{
	*ModelMatrix = (Owner->GetTransform() + Transform).GetModelMatrix();
}
S_Texture* C_StaticMeshComponent::GetTextureDifuse() const
{
	return Material->TextureDifuse;
}
S_Texture* C_StaticMeshComponent::GetTextureSpecular() const
{
	return Material->TextureSpecular;
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
	LevelGraph::GetInstance()->RemoveMeshComponent(this);
	if (ModelMatrix) delete(ModelMatrix);
}
