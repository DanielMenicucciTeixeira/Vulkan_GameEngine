#include "StaticMeshComponent.h"
#include "Objects/GameObjects/GameObject.h"
#include "Renderers/RenderObject.h"
#include "Math/FMatrix4.h"
#include "Math/FTransform.h"
#include "Math/FQuaternion.h"
#include "LevelGraph.h"
#include "Objects/Components/Colliders/BoundingBox.h"

#include <iostream>

void C_StaticMeshComponent::Start()
{
	Box->GetDimensionsFromMesh(Mesh);
	Box->SetComponentTransform(*Transform);
}

/*S_Texture* C_StaticMeshComponent::GetTextureDifuse() const
{
	return Material->TextureDifuse;
}*/
/*S_Texture* C_StaticMeshComponent::GetTextureSpecular() const
{
	return Material->TextureSpecular;
}*/

void C_StaticMeshComponent::SetInFrustum(bool inFrustum)
{
	//if (inFrustum != InFrustum) std::cout << "InFrustum = " << inFrustum << std::endl;
	InFrustum = inFrustum;
}

void C_StaticMeshComponent::UpdateModelMatrix()
{
	//*ModelMatrix = (Owner->GetTransform() + Transform).GetModelMatrix();
	*ModelMatrix = Owner->GetTransform().GetModelMatrix() * FTransform(GetComponentPosition(), GetComponentRotation(), GetComponentScale()).GetModelMatrix();
	//*ModelMatrix = FTransform(GetComponentPosition(), GetComponentRotation(), GetComponentScale()).GetModelMatrix() * Owner->GetTransform().GetModelMatrix();
}

void C_StaticMeshComponent::Update(const float deltaTime)
{
	UpdateModelMatrix();
}

C_StaticMeshComponent::C_StaticMeshComponent(O_GameObject* owner) : C_RenderableComponent(owner)
{
	ModelMatrix = new FMatrix4();
	UpdateModelMatrix();
	Box = Owner->AddComponentOfClass<C_BoundingBox>();
}

C_StaticMeshComponent::~C_StaticMeshComponent()
{
	LevelGraph::GetInstance()->RemoveMeshComponent(this);
	Box = nullptr;
	if (ModelMatrix) delete(ModelMatrix);
}
