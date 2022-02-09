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
	max = mesh->Vertices[0].Position;
	min = mesh->Vertices[0].Position;

	for (const auto& vertex : mesh->Vertices)
	{
		if (min.X > vertex.Position.X) min.X = vertex.Position.X;
		else if (max.X < vertex.Position.X) max.X = vertex.Position.X;

		if (min.Y > vertex.Position.Y) min.Y = vertex.Position.Y;
		else if (max.Y < vertex.Position.Y) max.Y = vertex.Position.Y;
		
		if (min.Z > vertex.Position.Z) min.Z = vertex.Position.Z;
		else if (max.Y < vertex.Position.Y) max.Y = vertex.Position.Y;
	}
}

void C_BoundingBox::Start()
{
	C_CollisionComponent::Start();
	model = GetComponentModelMatrix();
}

void C_BoundingBox::Update(float deltaTime)
{
	C_CollisionComponent::Update(deltaTime);
	model = GetComponentModelMatrix();
}

FVector3 C_BoundingBox::GetMin()
{
	//FVector4 temp = Transform->GetModelMatrix() * FVector4(BoxBounds.Min.X, BoxBounds.Min.Y, BoxBounds.Min.Z, 1.0f);
	FVector4 temp = GetComponentModelMatrix() * FVector4(min, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}

FVector3 C_BoundingBox::GetMax()
{

	//FVector4 temp = Transform->GetModelMatrix() * FVector4(BoxBounds.Max.X, BoxBounds.Max.Y, BoxBounds.Max.Z, 1.0f);
	FVector4 temp = GetComponentModelMatrix() * FVector4(max, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}
