#include "BoundingBox.h"
#include "Renderers/RenderObject.h"
#include "Math/FTransform.h"
#include "Objects/GameObjects/GameObject.h"
#include "Core/CollisionHandler.h"

C_BoundingBox::C_BoundingBox(O_GameObject* owner, ECollisionType type) : C_CollisionComponent(owner, type)
{
}

C_BoundingBox::~C_BoundingBox()
{
}

void C_BoundingBox::GetDimensionsFromMesh(S_Mesh* mesh)
{
	boxBounds.Max = mesh->Vertices[0].Position;
	boxBounds.Min = mesh->Vertices[0].Position;

	for (const auto& vertex : mesh->Vertices)
	{
		if (boxBounds.Min.X > vertex.Position.X) boxBounds.Min.X = vertex.Position.X;
		else if (boxBounds.Max.X < vertex.Position.X) boxBounds.Max.X = vertex.Position.X;

		if (boxBounds.Min.Y > vertex.Position.Y) boxBounds.Min.Y = vertex.Position.Y;
		else if (boxBounds.Max.Y < vertex.Position.Y) boxBounds.Max.Y = vertex.Position.Y;
		
		if (boxBounds.Min.Z > vertex.Position.Z) boxBounds.Min.Z = vertex.Position.Z;
		else if (boxBounds.Max.Y < vertex.Position.Y) boxBounds.Max.Y = vertex.Position.Y;
	}
}

void C_BoundingBox::Start()
{
	C_CollisionComponent::Start();
	boxBounds.Model = GetComponentModelMatrix();
}

void C_BoundingBox::Update(float deltaTime)
{
	//TODO:Move this to post update
	if (!GetIsStatic()) {
		for (auto& coll : CollisionHandler::GetInstance()->GetAABBCollision(boxBounds)) {
			//TODO:Collision Response here.
		}
	}


	C_CollisionComponent::Update(deltaTime);
	boxBounds.Model = GetComponentModelMatrix();
}

FVector3 C_BoundingBox::GetMin()
{
	//FVector4 temp = Transform->GetModelMatrix() * FVector4(BoxBounds.Min.X, BoxBounds.Min.Y, BoxBounds.Min.Z, 1.0f);
	FVector4 temp = GetComponentModelMatrix() * FVector4(boxBounds.Min, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}

FVector3 C_BoundingBox::GetMax()
{

	//FVector4 temp = Transform->GetModelMatrix() * FVector4(BoxBounds.Max.X, BoxBounds.Max.Y, BoxBounds.Max.Z, 1.0f);
	FVector4 temp = GetComponentModelMatrix() * FVector4(boxBounds.Max, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}
