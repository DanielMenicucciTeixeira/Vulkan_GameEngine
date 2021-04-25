#include "BoundingBox.h"
#include "Renderers/RenderObject.h"
#include "Math/FTransform.h"
#include "Objects/GameObjects/GameObject.h"

C_BoundingBox::C_BoundingBox(O_GameObject* owner, ECollisionType type) : C_CollisionComponent(owner, type)
{
}

C_BoundingBox::~C_BoundingBox()
{
}

void C_BoundingBox::GetDimensionsFromMesh(S_Mesh* mesh)
{
	Max = mesh->Vertices[0].Position;
	Min = mesh->Vertices[0].Position;

	for (const auto& vertex : mesh->Vertices)
	{
		if (Min.X > vertex.Position.X) Min.X = vertex.Position.X;
		else if (Max.X < vertex.Position.X) Max.X = vertex.Position.X;

		if (Min.Y > vertex.Position.Y) Min.Y = vertex.Position.Y;
		else if (Max.Y < vertex.Position.Y) Max.Y = vertex.Position.Y;
		
		if (Min.Z > vertex.Position.Z) Min.Z = vertex.Position.Z;
		else if (Max.Y < vertex.Position.Y) Max.Y = vertex.Position.Y;
	}
}

void C_BoundingBox::Start()
{
	C_CollisionComponent::Start();
}

FVector3 C_BoundingBox::GetMin()
{
	//FVector4 temp = Transform->GetModelMatrix() * FVector4(Max.X, Max.Y, Max.Z, 1.0f);
	FVector4 temp = GetComponentModelMatrix() * FVector4(Max.X, Max.Y, Max.Z, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}

FVector3 C_BoundingBox::GetMax()
{
	//FVector4 temp = Transform->GetModelMatrix() * FVector4(Min.X, Min.Y, Min.Z, 1.0f);
	FVector4 temp = GetComponentModelMatrix() * FVector4(Min.X, Min.Y, Min.Z, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}
